/*********************************************************************
 *
 * Copyright (C) 2021, Northwestern University
 * See COPYRIGHT notice in top-level directory.
 *
 * This program is part of the E3SM I/O benchmark.
 *
 *********************************************************************/

#pragma once

#include <map>
#include <vector>

#include <hdf5.h>
#include <mpi.h>

#include <e3sm_io.h>
#include <e3sm_io_driver.hpp>
#include <blob_ncmpio.h>

class e3sm_io_driver_h5blob : public e3sm_io_driver {
    typedef struct {
        int      nalloc;
        int      nrecs;
        void   **buf;  /* [nrecs] */
        size_t  *len;  /* [nrecs] */
    } var_buf;

    class h5blob_file {
        public:
            e3sm_io_driver_h5blob &driver;
            h5blob_file (e3sm_io_driver_h5blob &x) : driver (x) {};

            hid_t id;   /* HDF5 file ID */
            NC *header; /* metadata in NetCDF format */
            var_buf *vars;
            size_t total_len;
            int num_puts;
            MPI_Comm comm;
    };

    std::vector<h5blob_file *> files;

    e3sm_io_config *cfg;

   public:
    e3sm_io_driver_h5blob (e3sm_io_config *cfg);
    ~e3sm_io_driver_h5blob ();
    int create (std::string path, MPI_Comm comm, MPI_Info info, int *fid);
    int open (std::string path, MPI_Comm comm, MPI_Info info, int *fid);
    int close (int fid);
    int inq_file_info (int fid, MPI_Info *info);
    int inq_file_size (std::string path, MPI_Offset *size);
    int inq_put_size (MPI_Offset *size);
    int inq_get_size (MPI_Offset *size);
    int inq_malloc_size (MPI_Offset *size);
    int inq_malloc_max_size (MPI_Offset *size);
    int inq_rec_size (int fid, MPI_Offset *size);
    int expand_rec_size (int fid, MPI_Offset size);
    int def_var (int fid, std::string name, nc_type xtype, int ndim, int *dimids, int *did);
    int def_local_var (
        int fid, std::string name, nc_type xtype, int ndim, MPI_Offset *dsize, int *did);
    int inq_var (int fid, std::string name, int *did);
    int inq_var_name(int ncid, int varid, char *name);
    int inq_var_off (int fid, int vid, MPI_Offset *off);
    int def_dim (int fid, std::string name, MPI_Offset size, int *dimid);
    int inq_dim (int fid, std::string name, int *dimid);
    int inq_dimlen (int fid, int dimid, MPI_Offset *size);
    int enddef (int fid);
    int redef (int fid);
    int wait (int fid);
    int put_att (int fid, int vid, std::string name, nc_type xtype, MPI_Offset size, const void *buf);
    int get_att (int fid, int vid, std::string name, void *buf);
    int put_varl (int fid, int vid, MPI_Datatype itype, void *buf, e3sm_io_op_mode mode);
    int put_vara (int fid,
                  int vid,
                  MPI_Datatype itype,
                  MPI_Offset *start,
                  MPI_Offset *count,
                  void *buf,
                  e3sm_io_op_mode mode);
    int put_vars (int fid,
                  int vid,
                  MPI_Datatype itype,
                  MPI_Offset *start,
                  MPI_Offset *count,
                  MPI_Offset *stride,
                  void *buf,
                  e3sm_io_op_mode mode);
    int put_varn (int fid,
                  int vid,
                  MPI_Datatype itype,
                  int nreq,
                  MPI_Offset **starts,
                  MPI_Offset **counts,
                  void *buf,
                  e3sm_io_op_mode mode);
    int get_vara (int fid,
                  int vid,
                  MPI_Datatype itype,
                  MPI_Offset *start,
                  MPI_Offset *count,
                  void *buf,
                  e3sm_io_op_mode mode);
    int get_vars (int fid,
                  int vid,
                  MPI_Datatype itype,
                  MPI_Offset *start,
                  MPI_Offset *count,
                  MPI_Offset *stride,
                  void *buf,
                  e3sm_io_op_mode mode);
    int get_varn (int fid,
                  int vid,
                  MPI_Datatype itype,
                  int nreq,
                  MPI_Offset **starts,
                  MPI_Offset **counts,
                  void *buf,
                  e3sm_io_op_mode mode);

};

