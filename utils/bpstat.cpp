/*********************************************************************
 *
 * Copyright (C) 2021, Northwestern University
 * See COPYRIGHT notice in top-level directory.
 *
 * This program uses the E3SM I/O patterns recorded by the PIO library to
 * evaluate the performance of high-level I/O libraries.
 * The E3SM I/O patterns consist of a large number of small,
 * noncontiguous requests on each MPI process, which presents a challenge for
 * achieving a good performance.
 *
 * See README.md for compile and run instructions.
 *
 *********************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
//
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <map>
#include <string>
#include <vector>
//
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
//
#include <adios2.h>
#include <adios2_c.h>
#include <mpi.h>
//
#include <e3sm_io_err.h>

#define CHECK_AERR                                                    \
    {                                                                 \
        if (aerr != adios2_error_none) {                              \
            printf ("Error at line %d in %s:\n", __LINE__, __FILE__); \
            err = -1;                                                 \
            DEBUG_ABORT                                               \
            goto err_out;                                             \
        }                                                             \
    }

#define CHECK_APTR(P)                                              \
    {                                                              \
        if (P == NULL) {                                           \
            printf ("Error in %s:%d: NULL\n", __FILE__, __LINE__); \
            err = -1;                                              \
            DEBUG_ABORT                                            \
            goto err_out;                                          \
        }                                                          \
    }

typedef struct config {
    int np, rank;
    bool verbose = false;
} config;

typedef struct bpstat {
    int nvar     = 0;
    int natt     = 0;
    size_t vsize = 0;
    size_t asize = 0;
} bpstat;

inline size_t adios2_type_size (adios2_type type) {
    switch (type) {
        case adios2_type_int32_t:
            return 4;
        case adios2_type_int64_t:
            return 4;
        case adios2_type_float:
            return 4;
        case adios2_type_double:
            return 8;
        case adios2_type_uint8_t:
            return 1;
        case adios2_type_int8_t:
            return 1;
        case adios2_type_string:
            return 1;
        default:
            printf ("Error at line %d in %s: Unknown type %d\n", __LINE__, __FILE__, type);
    }

    return 0;
}

static void usage (char *argv0) {
    char *help =(char*)
        "Usage: %s [OPTION]... FILE \n"
        "       -v               Verbose mode\n"
        "       -h               Print help\n"
        "       FILE             Name of BP file to analysis\n";
    fprintf (stderr, help, argv0);
}

int bpstat_core (std::string inpath, config &cfg, bpstat &stat) {
    int err = 0;
    size_t i;
    adios2_error aerr;
    // size_t nstep;
    size_t nvar, natt;
    size_t nelem, esize, bsize;
    adios2_adios *adp = NULL;
    adios2_io *iop    = NULL;
    adios2_engine *ep = NULL;
    adios2_variable **vps;
    adios2_variable **vp;
    adios2_attribute **aps;
    adios2_attribute **ap;
    adios2_type type;
    adios2_step_status sstat;
    adios2_bool ret;
    char name[128];
    char *abufs[16];

    // No API to query ADIOS2 string attribute size
    // Preallocate 1 MiB
    abufs[0] = (char *)malloc (16 * 1048576);
    CHECK_PTR (abufs[0])
    for (i = 1; i < 16; i++) { abufs[i] = abufs[i - 1] + 1048576; }

    adp = adios2_init (MPI_COMM_SELF, "");
    CHECK_APTR (adp)
    iop = adios2_declare_io (adp, "bpstat");
    CHECK_APTR (iop)
    aerr = adios2_set_engine (iop, "BP3");
    CHECK_AERR

    ep = adios2_open (iop, inpath.c_str (), adios2_mode_read);
    CHECK_APTR (ep)

    // scorpio driver output only 1 step
    // aerr=adios2_steps(&nstep, ep);
    // CHECK_AERR

    aerr = adios2_begin_step (ep, adios2_step_mode_read, -1, &sstat);
    CHECK_AERR

    aerr = adios2_inquire_all_variables (&vps, &nvar, iop);
    CHECK_AERR
    for (vp = vps; vp < vps + nvar; vp++) {
        aerr = adios2_variable_name (name, &nelem, *vp);
        CHECK_AERR
        name[nelem] = '\0';

        aerr = adios2_variable_type (&type, *vp);
        CHECK_AERR
        esize = adios2_type_size (type);

        // There is no ADIOS2 API for querying number of blocks
        // Try until we got error
        nelem = 0;
        for (i = 0;; i++) {
            aerr = adios2_set_block_selection (*vp, i);
            if (aerr != adios2_error_none) { break; }

            aerr = adios2_selection_size (&bsize, *vp);
            if (aerr != adios2_error_none) { break; }

            nelem += bsize;
        }

        stat.vsize += nelem * esize;
        if (cfg.verbose) {
            printf ("Size of variable %s is %lld bytes\n", name, (long long)(nelem * esize));
        }
    }
    stat.nvar += nvar;

    aerr = adios2_inquire_all_attributes (&aps, &natt, iop);
    CHECK_AERR
    for (ap = aps; ap < aps + natt; ap++) {
        aerr = adios2_attribute_name (name, &nelem, *ap);
        CHECK_AERR
        name[nelem] = '\0';

        aerr = adios2_attribute_type (&type, *ap);
        CHECK_AERR
        esize = adios2_type_size (type);

        aerr = adios2_attribute_size (&nelem, *ap);
        CHECK_AERR

        // ADIOS2 strings are considered a single element
        // Read the string and count the length
        if (type == adios2_type_string) {
            esize = 0;
            aerr  = adios2_attribute_is_value (&ret, *ap);
            CHECK_AERR
            if (ret) {
                aerr = adios2_attribute_data (abufs[0], &nelem, *ap);
                CHECK_AERR
            } else {
                aerr = adios2_attribute_data (abufs, &nelem, *ap);
                CHECK_AERR
            }
            for (i = 0; i < nelem; i++) { esize += strlen (abufs[i]); }
            nelem = 1;
        }

        stat.asize += nelem * esize;
        if (cfg.verbose) {
            printf ("Size of attribute %s is %lld bytes\n", name, (long long)(nelem * esize));
        }
    }
    stat.natt += natt;

    aerr = adios2_end_step (ep);
    CHECK_AERR
    aerr = adios2_close (ep);
    CHECK_AERR

    aerr = adios2_remove_io (&ret, adp, "bpstat");
    CHECK_AERR
    adios2_finalize (adp);

    // Discarded C++ version due to lack of universal (untyped) varaible inquery API
    /*
    try {
        adios2::ADIOS ad (MPI_COMM_WORLD);
        adios2::IO io = ad.DeclareIO ("bpstat");
        io.SetEngine ("BP3");
        adios2::Engine eng                               = io.Open (inpath, adios2::Mode::Read);
        const std::map<std::string, adios2::Params> vars = io.AvailableVariables ();
        const std::map<std::string, adios2::Params> atts = io.AvailableAttributes ();

        for (const auto var : vars) {
            if (cfg.verbose) { std::cout << "Parsing variable " << var.first << std::endl; }

            for (const auto &p : var.second) {
                if (cfg.verbose) {
                    std::cout << var.first << "." << p.first << " = " << p.second << std::endl;
                }
            }
        }

        for (const auto att : atts) {
            if (cfg.verbose) { std::cout << "Parsing attribute " << att.first << std::endl; }

            for (const auto &p : att.second) {
                if (cfg.verbose) {
                    std::cout << att.first << "." << p.first << " = " << p.second << std::endl;
                }

                if (p.first == "Elements") {}
            }
        }
        eng.Close ();
    } catch (std::exception &e) { ERR_OUT (e.what ()) }
    */

err_out:;
    if (abufs[0]) { free (abufs[0]); }
    return err;
}

inline int get_n_sbufiles (config &cfg, std::string inpath) {
    int ret = -1;
    int fno;
    std::filesystem::path fname, subfname;

    if (std::filesystem::is_directory (inpath + ".dir")) {
        for (auto &f : std::filesystem::directory_iterator (inpath + ".dir")) {
            fname    = std::filesystem::path (inpath).filename ();
            subfname = f.path ().filename ();

            if (cfg.verbose) { std::cout << "Iterating " << subfname << std::endl; }

            fno = atoi (subfname.extension ().c_str () + 1);
            if (fname == subfname.replace_extension ()) {  // replace_extension is in place, must
                                                           // retrieve the value before its gone
                if (ret < fno) { ret = fno; }
            }
        }
        ret++;  // #subfile = largest subfile num + 1
    } else if (std::filesystem::is_regular_file (inpath)) {
        ret = 0;  // Single file
    }

    return ret;
}

int main (int argc, char *argv[]) {
    int err = 0, nerrs = 0;
    int i;
    int nsub;
    config cfg;
    bpstat stat;
    bpstat stat_all;
    std::string inpath;

    MPI_Init (&argc, &argv);

    MPI_Comm_size (MPI_COMM_WORLD, &(cfg.np));
    MPI_Comm_rank (MPI_COMM_WORLD, &(cfg.rank));

    /* get command-line arguments */
    while ((i = getopt (argc, argv, "hv")) != EOF) {
        switch (i) {
            case 'v':
                cfg.verbose = true;
                break;
            case 'h':
            default:
                usage (argv[0]);
                return 1;
        }
    }

    inpath = std::string (argv[optind]);
    if (cfg.rank == 0) {  // Rank 0 scan folder
        nsub = get_n_sbufiles (cfg, inpath);
        err  = MPI_Bcast (&nsub, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        err = MPI_Bcast (&nsub, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    CHECK_MPIERR

    if (nsub == 0) {  // Single file, rank 0 parse
        if (cfg.rank == 0) {
            err = bpstat_core (inpath, cfg, stat_all);
            if (err != 0) { nerrs++; }
        }
    } else if (nsub > 0) {
        for (i = cfg.rank; i < nsub; i += cfg.np) {
            err = bpstat_core (inpath + ".dir/" +
                                   std::filesystem::path (inpath).filename ().string () + "." +
                                   std::to_string (i),
                               cfg, stat);
            if (err != 0) { nerrs++; }
        }

        stat_all.nvar = stat.nvar;
        stat_all.natt = stat.natt;
        err = MPI_Reduce (&(stat.asize), &(stat_all.asize), 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0,
                          MPI_COMM_WORLD);
        CHECK_MPIERR
        err = MPI_Reduce (&(stat.vsize), &(stat_all.vsize), 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0,
                          MPI_COMM_WORLD);
        CHECK_MPIERR
    } else {
        if (cfg.rank == 0) { std::cout << "Error, cannot open " << inpath << "." << std::endl; }
    }

    if (cfg.rank == 0) {
        std::cout << "Num subfiles: " << nsub << std::endl;
        std::cout << "Num variables: " << stat_all.nvar << std::endl;
        std::cout << "Total variable size: " << stat_all.vsize << std::endl;
        std::cout << "Num attributes: " << stat_all.natt << std::endl;
        std::cout << "Total attribute size: " << stat_all.asize << std::endl;
        std::cout << "Total object size (MiB): "
                  << ((double)(stat_all.vsize + stat_all.asize) / 1048576) << std::endl;
        std::cout << "Total object size (GiB): "
                  << ((double)(stat_all.vsize + stat_all.asize) / 1048576 / 1024) << std::endl;
    }

err_out:;
    MPI_Bcast (&nerrs, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (nerrs && (cfg.rank == 0)) {
        std::cout << "Filed to parse " << nerrs << " files." << std::endl;
    }
    MPI_Finalize ();
    return 0;
}