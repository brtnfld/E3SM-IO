/*********************************************************************
 *
 * Copyright (C) 2021, Northwestern University
 * See COPYRIGHT notice in top-level directory.
 *
 * This program is part of the E3SM I/O benchmark.
 *
 *********************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstring>
#include <sys/stat.h>
#include <assert.h>

#include <hdf5.h>

#include <e3sm_io.h>
#include <e3sm_io_err.h>
#include <e3sm_io_driver_hdf5_int.hpp>
#include <e3sm_io_driver_h5blob.hpp>

e3sm_io_driver_h5blob::e3sm_io_driver_h5blob (e3sm_io_config *cfg) : e3sm_io_driver (cfg) {
    this->cfg = cfg;
    this->put_amount = 0;  /* write amount per driver, not per file */
    this->get_amount = 0;
}

e3sm_io_driver_h5blob::~e3sm_io_driver_h5blob () { }

int e3sm_io_driver_h5blob::create(std::string path,
                                  MPI_Comm comm,
                                  MPI_Info info,
                                  int *fid)
{
    int i, err=0, nvars;
    herr_t herr;
    hid_t faplid;
    h5blob_file *fp;
    NC *ncp;

    fp = new h5blob_file(*this);

    err = MPI_Comm_dup(comm, &fp->comm);
    CHECK_MPIERR

    faplid = H5Pcreate (H5P_FILE_ACCESS);
    CHECK_HID (faplid)
    herr = H5Pset_fapl_mpio(faplid, fp->comm, info);
    CHECK_HERR
    herr = H5Pset_all_coll_metadata_ops(faplid, true);
    CHECK_HERR
    herr = H5Pset_coll_metadata_write(faplid, true);
    CHECK_HERR

    fp->id = H5Fcreate(path.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, faplid);
    CHECK_HID (fp->id)

    *fid = this->files.size ();

    /* Use NetCDF header struct to store all metadata */
    ncp = (NC*) NCI_Calloc(1, sizeof(NC));

    fp->header = ncp;
    fp->num_puts = 0;

    /* allocate write buffer pointer array, one for each variable */
    nvars = cfg->nvars + MAX_NUM_DECOMP * NVARS_DECOMP;
    fp->vars = (var_buf*) malloc(nvars * sizeof(var_buf));
    for (i=0; i<nvars; i++) {
        fp->vars[i].nalloc = 0;
        fp->vars[i].nrecs = 0;
        fp->vars[i].buf = NULL;
        fp->vars[i].len = NULL;
    }
    fp->total_len = 0;

    this->files.push_back (fp);

err_out:
    if (faplid >= 0) { H5Pclose (faplid); }

    return err;
}

int e3sm_io_driver_h5blob::open(std::string path,
                                MPI_Comm comm,
                                MPI_Info info,
                                int *fid)
{
    throw "HDF5 blob I/O does not support file open yet";
    return -1;
}

int e3sm_io_driver_h5blob::close (int fid)
{
    char *buf_ptr;
    int i, j, err=0, rank, nvars;
    herr_t herr;
    hid_t dcpl_id, fspace, mspace, dxpl_id, header_blob, data_blob;
    hsize_t dim_len, off, len, one=1;
    h5blob_file *fp = this->files[fid];
    NC *ncp;
    size_t header_len;
    void *buf, *header_buf;
    MPI_Offset start, count, sum;

    /* prepare to write the header */
    ncp = fp->header;
    nvars = ncp->vars.ndefined;

    MPI_Comm_rank(fp->comm, &rank);

/*
printf("%s: nvars=%d num_puts=%d total_len=%lld header=%lld\n", __FILE__,ncp->vars.ndefined, fp->num_puts, fp->total_len, fp->header->xsz);
for (i=0; i<fp->num_puts; i++) printf("var[%d]=%s nrecs=%d buf len =%ld\n",i,ncp->vars.value[i]->name,fp->vars[i].nrecs,fp->vars[i].len[0]);
*/
    /* create a new dataset to store data blobs of all processes */

    /* Sum the data blob sizes across all processes */
    count = fp->total_len;
    err = MPI_Allreduce(&count, &sum, 1, MPI_OFFSET, MPI_SUM, fp->comm);
    CHECK_MPIERR

    /* Find write starting offset in data blob dataset for each process */
    start = 0;
    err = MPI_Exscan(&count, &start, 1, MPI_OFFSET, MPI_SUM, fp->comm);
    CHECK_MPIERR

    if (cfg->verbose) printf("%2d; start=%lld count=%lld\n",rank,start,count);

    /* pack header into a contiguous buffer */
    err = blob_ncmpio_pack_NC(ncp, &header_len, &header_buf);

    /* free up space occupied by the header metadata */
    blob_ncmpio_free_NC(ncp);

    /* set create property list */
    dcpl_id = H5Pcreate(H5P_DATASET_CREATE);
    CHECK_HID(dcpl_id)

    H5Pset_fill_value(dcpl_id, 0, NULL);
    H5Pset_fill_time(dcpl_id, H5D_FILL_TIME_NEVER);
    H5Pset_alloc_time(dcpl_id, H5D_ALLOC_TIME_DEFAULT);

    /* create a new dataset to store header blob */
    dim_len = header_len;
    fspace = H5Screate_simple(1, &dim_len, NULL);
    CHECK_HID(fspace);

    /* create a HDF5 blob dataset to store header */
    header_blob = H5Dcreate2(fp->id, "header_blob", H5T_NATIVE_UCHAR, fspace,
                             H5P_DEFAULT, dcpl_id, H5P_DEFAULT);
    CHECK_HID(header_blob)
    herr = H5Sclose(fspace);
    CHECK_HERR

    /* create a HDF5 dataset to store data blob */
    dim_len = sum;
    fspace = H5Screate_simple(1, &dim_len, NULL);
    CHECK_HID(fspace);

    /* create a HDF5 blob dataset to store climate data */
    data_blob = H5Dcreate2(fp->id, "data_blob", H5T_NATIVE_UCHAR, fspace,
                           H5P_DEFAULT, dcpl_id, H5P_DEFAULT);
    CHECK_HID(data_blob)

    herr = H5Pclose(dcpl_id);
    CHECK_HERR

    /* done with creating new datasets */

    /* Each process packs its write data into a contiguous buffer */
    buf = (void*) malloc(fp->total_len);
    buf_ptr = (char*) buf;
    for (i=0; i<nvars; i++) {
        if (fp->vars[i].nalloc == 0) continue; /* not written by this proc */
        memcpy(buf_ptr, fp->vars[i].buf[0], fp->vars[i].len[0]);
        free(fp->vars[i].buf[0]);
        buf_ptr += fp->vars[i].len[0];
        for (j=1; j<fp->vars[i].nrecs; j++) {
            memcpy(buf_ptr, fp->vars[i].buf[j], fp->vars[i].len[j]);
            free(fp->vars[i].buf[j]);
            buf_ptr += fp->vars[i].len[j];
        }
        free(fp->vars[i].buf);
        free(fp->vars[i].len);
    }
    assert(buf_ptr - (char*)buf == (long)fp->total_len);
    free(fp->vars);

    /* set collectie write mode for writing data blob */
    dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    CHECK_HID(dxpl_id)
    herr = H5Pset_dxpl_mpio(dxpl_id, H5FD_MPIO_COLLECTIVE);
    CHECK_HERR

    /* Setup hyperslab file space for each process */
    off = start;
    len = count;
    herr = H5Sselect_hyperslab(fspace, H5S_SELECT_SET, &off, NULL, &one, &len);
    CHECK_HERR

    /* set local memory space, a contiguous buffer of size fp->total_len */
    dim_len = fp->total_len;
    mspace = H5Screate_simple(1, &dim_len, NULL);
    CHECK_HID(mspace)

    /* all processes write to the data blob dataset collectively */
    herr = H5Dwrite(data_blob, H5T_NATIVE_UCHAR, mspace, fspace, dxpl_id, buf);
    CHECK_HERR

    /* this process writes fp->total_len amount in bytes */
    this->put_amount += fp->total_len;

    herr = H5Pclose(dxpl_id);
    CHECK_HERR
    herr = H5Sclose(mspace);
    CHECK_HERR

    herr = H5Sclose(fspace);
    CHECK_HERR

    free(buf);

    /* close data_blob dataset */
    herr = H5Dclose(data_blob);
    CHECK_HERR

    /* set independent write mode, as only root writes header blob */
    dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    CHECK_HID(dxpl_id)
    herr = H5Pset_dxpl_mpio(dxpl_id, H5FD_MPIO_INDEPENDENT);
    CHECK_HERR

    /* root writes to the header blob dataset entirely */
    if (rank == 0) {
        herr = H5Dwrite(header_blob, H5T_NATIVE_UCHAR, H5S_ALL, H5S_ALL,
                        dxpl_id, header_buf);
        CHECK_HERR

        /* root process writes header_len amount in bytes */
        this->put_amount += header_len;
    }

    herr = H5Pclose(dxpl_id);
    CHECK_HERR

    /* close header blob dataset */
    herr = H5Dclose(header_blob);
    CHECK_HERR

    free(header_buf);

    herr = H5Fclose(fp->id);
    CHECK_HERR

    free(fp->header);
    MPI_Comm_free(&fp->comm);

    delete fp;

err_out:
    return err;
}

int e3sm_io_driver_h5blob::inq_file_info (int fid, MPI_Info *info) {
    int err = 0;
    herr_t herr;
    h5blob_file *fp = this->files[fid];
    hid_t pid;


    pid = H5Fget_access_plist (fp->id);
    CHECK_HID (pid);
    herr = H5Pget_fapl_mpio (pid, NULL, info);
    CHECK_HERR

err_out:
    if (pid != -1) H5Pclose (pid);
    return err;
}

int e3sm_io_driver_h5blob::inq_file_size (std::string path, MPI_Offset *size) {
    int err = 0;
    struct stat file_stat;

    err = stat (path.c_str (), &file_stat);
    CHECK_ERR

    *size = (MPI_Offset) (file_stat.st_size);

err_out:
    return err;
}

int e3sm_io_driver_h5blob::inq_put_size (int fid, MPI_Offset *size) {
    *size = this->put_amount;
    return 0;
}

int e3sm_io_driver_h5blob::inq_get_size (int fid, MPI_Offset *size) {
    *size = this->get_amount;
    return 0;
}

int e3sm_io_driver_h5blob::inq_malloc_size (MPI_Offset *size) {
    *size = 0;
    return 0;
}

int e3sm_io_driver_h5blob::inq_malloc_max_size (MPI_Offset *size) {
    *size = 0;
    return 0;
}

int e3sm_io_driver_h5blob::inq_rec_size (int fid, MPI_Offset *size) {

    *size = (MPI_Offset) (this->files[fid]->header->recsize);

    return 0;
}

int e3sm_io_driver_h5blob::def_var(int          fid,
                                   std::string  name,
                                   nc_type      xtype,
                                   int          ndims,
                                   int         *dimids,
                                   int         *varidp)
{
    /* add a variable object in NC */
    return blob_ncmpio_add_var(this->files[fid]->header, name.c_str(), xtype,
                               ndims, dimids, varidp);
}

int e3sm_io_driver_h5blob::def_local_var (
    int fid, std::string name, MPI_Datatype type, int ndim, MPI_Offset *dsize, int *did) {
    int err = 0;

    ERR_OUT ("HDF5 blob I/O does not support local variables")

err_out:
    return err;
}

int e3sm_io_driver_h5blob::inq_var (int fid, std::string name, int *did) {
    int err = 0;

    ERR_OUT ("HDF5 blob I/O does not implement inq_var yet")

err_out:
    return err;
}

int e3sm_io_driver_h5blob::inq_var_name (int fid, int varid, char *name) {
    NC *ncp = this->files[fid]->header;
    NC_var *varp = ncp->vars.value[varid];

    if (name != NULL) strcpy(name, varp->name);

    return 0;
}

int e3sm_io_driver_h5blob::inq_var_off (int fid, int vid, MPI_Offset *off) {
    throw "Function not supported";
    return -1;
}

int e3sm_io_driver_h5blob::def_dim(int          fid,
                                   std::string  name,
                                   MPI_Offset   size,
                                   int         *dimidp)
{
    return blob_ncmpio_add_dim(this->files[fid]->header, name.c_str(), size,
                               dimidp);
}

int e3sm_io_driver_h5blob::inq_dim(int fid, std::string name, int *dimid) {
    int i;
    size_t nchars;
    NC_dimarray *ncdap = &this->files[fid]->header->dims;

    if (ncdap->ndefined == 0) return -1; /* none defined yet */

    /* note that the number of dimensions allowed is < 2^32 */
    nchars = strlen(name.c_str());
    for (i=0; i<ncdap->ndefined; i++) {
        if (ncdap->value[i]->name_len == nchars &&
            strcmp(ncdap->value[i]->name, name.c_str()) == 0) {
            /* found the matched name */
            if (dimid != NULL) *dimid = i;
            return NC_NOERR; /* found it */
        }
    }
    return -1;
}

int e3sm_io_driver_h5blob::inq_dimlen (int fid, int dimid, MPI_Offset *size) {
    NC *ncp = this->files[fid]->header;
    NC_dim *dimp = ncp->dims.value[dimid];

    if (size != NULL) {
        if (dimp->size == NC_UNLIMITED)
            *size = ncp->numrecs;
        else
            *size = dimp->size;
    }
    return 0;
}

int e3sm_io_driver_h5blob::enddef (int fid) { return 0; }
int e3sm_io_driver_h5blob::redef (int fid) { return 0; }
int e3sm_io_driver_h5blob::wait (int fid) {
    int err = 0;
    herr_t herr;
    h5blob_file *fp = this->files[fid];

    herr = H5Fflush (fp->id, H5F_SCOPE_GLOBAL);
    CHECK_HERR

err_out:
    return err;
}

int e3sm_io_driver_h5blob::put_att(int          fid,
                                   int          varid,
                                   std::string  name,
                                   nc_type      xtype,
                                   MPI_Offset   nelems,
                                   const void  *buf)
{
    return blob_ncmpio_put_att(this->files[fid]->header, varid, name.c_str(),
                               xtype, nelems, buf);
}

int e3sm_io_driver_h5blob::get_att(int          fid,
                                   int          varid,
                                   std::string  name,
                                   void        *buf)
{
    return blob_ncmpio_get_att(this->files[fid]->header, varid, name.c_str(),
                               buf);
}

int e3sm_io_driver_h5blob::put_varl (
    int fid, int vid, MPI_Datatype type, void *buf, e3sm_io_op_mode mode) {
    int err = 0;

    ERR_OUT ("HDF5 does not support local variables")

err_out:
    return err;
}

int e3sm_io_driver_h5blob::put_vara(int fid,
                                    int varid,
                                    MPI_Datatype itype,
                                    MPI_Offset *start,
                                    MPI_Offset *count,
                                    void *buf,
                                    e3sm_io_op_mode mode)
{
    int i, xsz;
    MPI_Datatype xtype;
    MPI_Offset nelems, len;
    NC *ncp = this->files[fid]->header;
    NC_var *varp = ncp->vars.value[varid];
    var_buf *vbuf = this->files[fid]->vars + varid;

    this->files[fid]->num_puts++;

    xtype = varp->xtype;
    MPI_Type_size(xtype, &xsz);

    nelems = 1;
    if (varp->ndims > 0 && start == NULL) { /* var API */
        if (varp->shape[0] != NC_UNLIMITED) nelems *= varp->shape[0];
        for (i=1; i<varp->ndims; i++)
            nelems *= varp->shape[i];

/*
if (varp->ndims==1) printf("%s var %s varid %d: ndims=%d shape[0]=%lld nelems=%lld\n",__func__,varp->name,varid,varp->ndims,varp->shape[0],nelems);
if (varp->ndims==2) printf("%s var %s varid %d: ndims=%d shape[0]=%lld %lld nelems=%lld\n",__func__,varp->name,varid,varp->ndims,varp->shape[0],varp->shape[1],nelems);
if (varp->ndims==3) printf("%s var %s varid %d: ndims=%d shape[0]=%lld %lld %lld nelems=%lld\n",__func__,varp->name,varid,varp->ndims,varp->shape[0],varp->shape[1],varp->shape[2],nelems);
*/
    }
    else if (varp->ndims > 0 && count != NULL) {
        if (varp->shape[0] != NC_UNLIMITED) nelems *= count[0];
        for (i=1; i<varp->ndims; i++)
            nelems *= count[i];
/*
if (varp->ndims==1) printf("%s var %s varid %d: ndims=%d count[0]=%lld nelems=%lld\n",__func__,varp->name,varid,varp->ndims,count[0],nelems);
if (varp->ndims==2) printf("%s var %s varid %d: ndims=%d count[0]=%lld %lld nelems=%lld\n",__func__,varp->name,varid,varp->ndims,count[0],count[1],nelems);
if (varp->ndims==3) printf("%s var %s varid %d: ndims=%d count[0]=%lld %lld %lld nelems=%lld\n",__func__,varp->name,varid,varp->ndims,count[0],count[1],count[2],nelems);
*/
    }

    len = nelems * xsz;

    if (itype != xtype) {
        /* type cast when external and internal data type sizes are different */
        if (itype == MPI_DOUBLE && xtype == MPI_FLOAT) {
            double *dbl_buf = (double*)buf;
            float *flt_buf = (float*) malloc(nelems * sizeof(float));
            for (i=0; i<nelems; i++)
                flt_buf[i] = (float) dbl_buf[i];
            buf = flt_buf;
        }
        else {
            printf("var %s itype xtype mismatched\n",varp->name);
            return -1;
        }
    }
    else { /* type matched, allocate buffer and copy over */
        void *tmp = (void*) malloc(len);
        memcpy(tmp, buf, len);
        buf = tmp;
    }

    if (IS_RECVAR(varp) && count != NULL && count[0] > 1)
        throw "Error: writing 2 or more records is not supported yet";

    this->files[fid]->total_len += len;

    if (IS_RECVAR(varp)) {
        assert(start != NULL);
        if (start[0] > vbuf->nalloc + 64)
            throw "Error: non-sequential write to records is not supported yet";
        if (start[0] == vbuf->nalloc) {
            vbuf->nalloc += 64;
            vbuf->buf = (void**)  realloc(vbuf->buf, vbuf->nalloc * sizeof(void*));
            vbuf->len = (size_t*) realloc(vbuf->len, vbuf->nalloc * sizeof(size_t));
            for (i=0; i<64; i++)
                vbuf->len[vbuf->nalloc - 64 + i] = 0;
        }

        vbuf->nrecs = (start[0]+1 > vbuf->nrecs) ? start[0]+1 : vbuf->nrecs;

        vbuf->buf[start[0]] = buf;
        vbuf->len[start[0]] = len;

        MPI_Offset new_numrecs = start[0];
        new_numrecs += (count == NULL) ? 1 : count[0];
        if (ncp->numrecs < new_numrecs)
            ncp->numrecs = new_numrecs;
// printf("%s RECORD var %s\n", __FILE__,ncp->vars.value[varid]->name);
    }
    else { /* fixed-size variable */
        vbuf->nrecs = 0;
        if (vbuf->nalloc == 0) {
            vbuf->nalloc = 1;
            vbuf->buf = (void**)  realloc(vbuf->buf, sizeof(void*));
            vbuf->len = (size_t*) realloc(vbuf->len, sizeof(size_t));
        }
        vbuf->buf[0] = buf;
        vbuf->len[0] = len;
// printf("%s FIXED var %s\n", __FILE__,ncp->vars.value[varid]->name);
    }

    return 0;
}

int e3sm_io_driver_h5blob::put_vars (int fid,
                                   int vid,
                                   MPI_Datatype type,
                                   MPI_Offset *start,
                                   MPI_Offset *count,
                                   MPI_Offset *stride,
                                   void *buf,
                                   e3sm_io_op_mode mode) {
    throw "HDF5 blob I/O does not support put_vars yet";
    return -1;
}

int e3sm_io_driver_h5blob::put_varn (int fid,
                                   int vid,
                                   MPI_Datatype type,
                                   int nreq,
                                   MPI_Offset **starts,
                                   MPI_Offset **counts,
                                   void *buf,
                                   e3sm_io_op_mode mode) {
    throw "HDF5 blob I/O does not support put_varn yet";
    return -1;
}

int e3sm_io_driver_h5blob::put_vard (int fid,
                                   int vid,
                                   MPI_Datatype type,
                                   MPI_Offset nelem,
                                   MPI_Datatype ftype,
                                   void *buf,
                                   e3sm_io_op_mode mode) {
    throw "HDF5 blob I/O does not support put_vard yet";
    return -1;
}

int e3sm_io_driver_h5blob::get_vara (int fid,
                                   int vid,
                                   MPI_Datatype type,
                                   MPI_Offset *start,
                                   MPI_Offset *count,
                                   void *buf,
                                   e3sm_io_op_mode mode) {
    throw "HDF5 blob I/O does not support get_vara yet";
    return -1;
}

int e3sm_io_driver_h5blob::get_vars (int fid,
                                   int vid,
                                   MPI_Datatype type,
                                   MPI_Offset *start,
                                   MPI_Offset *count,
                                   MPI_Offset *stride,
                                   void *buf,
                                   e3sm_io_op_mode mode) {
    throw "HDF5 blob I/O does not support get_vars yet";
    return -1;
}

int e3sm_io_driver_h5blob::get_varn (int fid,
                                   int vid,
                                   MPI_Datatype type,
                                   int nreq,
                                   MPI_Offset **starts,
                                   MPI_Offset **counts,
                                   void *buf,
                                   e3sm_io_op_mode mode) {
    throw "HDF5 blob I/O does not support get_varn yet";
    return -1;
}

int e3sm_io_driver_h5blob::get_vard (int fid,
                                   int vid,
                                   MPI_Datatype type,
                                   MPI_Offset nelem,
                                   MPI_Datatype ftype,
                                   void *buf,
                                   e3sm_io_op_mode mode) {
    throw "HDF5 blob I/O does not support get_vard yet";
    return -1;
}