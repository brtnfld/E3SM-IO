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

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcpy(), strcat() */

#include <e3sm_io.h>
#include <e3sm_io_err.h>
#include <e3sm_io_case.hpp>

e3sm_io_all_cases::e3sm_io_all_cases () {}
e3sm_io_all_cases::~e3sm_io_all_cases () {}

int e3sm_io_all_cases::wr_test(e3sm_io_config &cfg,
                               e3sm_io_decom  &decom,
                               e3sm_io_driver &driver)
{
    int err=0;
    char *ext;
    case_meta *cmeta;

    /* construct I/O metadata */
    err = calc_metadata(&cfg, &decom);
    CHECK_ERR

    ext = strrchr(cfg.out_path, '.');

    if (cfg.run_case == G) {
        cmeta        = &cfg.G_case;
        cmeta->nrecs =  cfg.G_case.nrecs;
        cmeta->nvars = NVARS_G_CASE;

        /* construct file name */
        strcpy(cmeta->outfile, cfg.out_path);
        if (cfg.strategy == blob) { /* append subfile ID to subfile name */
            char sub_str[8];
            sprintf(sub_str, ".%04d", cfg.subfile_ID);
            strcat(cmeta->outfile, sub_str);
        }

        cfg.nvars = cmeta->nvars;
        err = var_wr_all_cases(cfg, decom, driver, cmeta);
        CHECK_ERR

        goto err_out;
    }

    /* Only F anc I cases write to h0 and h1 files */
    if (cfg.hx == 0 || cfg.hx == -1) {  /* h0 file */
        if (cfg.run_case == F) {
            cmeta        = &cfg.F_case_h0;
            cmeta->nrecs =  cfg.F_case_h0.nrecs;
            cmeta->nvars = NVARS_F_CASE_H0;
        }
        else if (cfg.run_case == I) {
            cmeta        = &cfg.I_case_h0;
            cmeta->nrecs =  cfg.I_case_h0.nrecs;
            cmeta->nvars = NVARS_I_CASE_H0;
        }

        /* construct file name */
        if (ext == NULL || (strcmp(ext, ".nc") && strcmp(ext, ".h5")))
            sprintf(cmeta->outfile, "%s_h0", cfg.out_path);
        else { /* add "_h0" before file extension */
            strcpy(cmeta->outfile, cfg.out_path);
            sprintf(cmeta->outfile + (ext - cfg.out_path), "_h0%s", ext);
        }
        if (cfg.strategy == blob) { /* append subfile ID to subfile name */
            char sub_str[8];
            sprintf(sub_str, ".%04d", cfg.subfile_ID);
            strcat(cmeta->outfile, sub_str);
        }

        cfg.hist = h0;
        cfg.nvars = cmeta->nvars;
        err = var_wr_all_cases(cfg, decom, driver, cmeta);
        CHECK_ERR
    }

    if (cfg.hx == 1 || cfg.hx == -1) {  /* h1 file */
        if (cfg.run_case == F) {
            cmeta        = &cfg.F_case_h1;
            cmeta->nrecs =  cfg.F_case_h1.nrecs;
            cmeta->nvars = NVARS_F_CASE_H1;
        }
        else if (cfg.run_case == I) {
            cmeta        = &cfg.I_case_h1;
            cmeta->nrecs =  cfg.I_case_h1.nrecs;
            cmeta->nvars = NVARS_I_CASE_H1;
        }

        /* construct file name */
        if (ext == NULL || (strcmp(ext, ".nc") && strcmp(ext, ".h5")))
            sprintf(cmeta->outfile, "%s_h1", cfg.out_path);
        else { /* add "_h1" before file extension */
            strcpy(cmeta->outfile, cfg.out_path);
            sprintf(cmeta->outfile + (ext - cfg.out_path), "_h1%s", ext);
        }
        if (cfg.strategy == blob) { /* append subfile ID to subfile name */
            char sub_str[8];
            sprintf(sub_str, ".%04d", cfg.subfile_ID);
            strcat(cmeta->outfile, sub_str);
        }

        cfg.hist = h1;
        cfg.nvars = cmeta->nvars;
        err = var_wr_all_cases(cfg, decom, driver, cmeta);
        CHECK_ERR
    }

err_out:
    if (cfg.sub_comm != MPI_COMM_NULL) {
        MPI_Comm_free(&cfg.sub_comm);
        cfg.sub_comm = MPI_COMM_NULL;
    }

    return err;
}

int e3sm_io_all_cases::rd_test(e3sm_io_config &cfg,
                               e3sm_io_decom &decom,
                               e3sm_io_driver &driver)
{
    return 0;
}