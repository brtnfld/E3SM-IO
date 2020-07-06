/*********************************************************************
 *
 * Copyright (C) 2018, Northwestern University
 * See COPYRIGHT notice in top-level directory.
 *
 * This program is part of the E3SM I/O benchmark.
 *
 *********************************************************************/

#include <assert.h>
#include <e3sm_io.h>
#include <netcdf.h>
#include <netcdf_par.h>

#define INQ_VID(A, B, D, E, F, C) nc_inq_varid (A, B, C)
#define NOP(A, B, D, E, C)        NC_NOERR
#define NOP2(A, B, D, E, F, C)    NC_NOERR

#define NC4_DEF_VAR(A, B, C, D, E, F)                                            \
    ({                                                                           \
        int ret;                                                                 \
        ret = nc_def_var (A, B, C, D, E, F);                                     \
        if (ret == NC_NOERR) { ret = nc_var_par_access (A, *F, NC_COLLECTIVE); } \
        ret;                                                                     \
    })

/*----< def_F_case_h0() >----------------------------------------------------*/
int def_F_case_h0_nc4 (int ncid,             /* file ID */
                       const size_t dims[2], /* dimension sizes */
                       int nvars,            /* number of variables */
                       int *varids)          /* variable IDs */
{
    /* Total 408 variables */
    int lat, lon, area, lev, hyam, hybm, P0, ilev, hyai, hybi, time, date, datesec, time_bnds,
        date_written, time_written, ndbase, nsbase, nbdate, nbsec, mdt, ndcur, nscur, co2vmr,
        ch4vmr, n2ovmr, f11vmr, f12vmr, sol_tsi, nsteph, AEROD_v, ANRAIN, ANSNOW, AODABS, AODABSBC,
        AODBC, AODDUST, AODDUST1, AODDUST3, AODDUST4, AODMODE1, AODMODE2, AODMODE3, AODMODE4,
        AODNIR, AODPOM, AODSO4, AODSOA, AODSS, AODUV, AODVIS, AQRAIN, AQSNOW, AQ_DMS, AQ_H2O2,
        AQ_H2SO4, AQ_O3, AQ_SO2, AQ_SOAG, AREI, AREL, AWNC, AWNI, CCN3, CDNUMC, CLDHGH, CLDICE,
        CLDLIQ, CLDLOW, CLDMED, CLDTOT, CLOUD, CLOUDFRAC_CLUBB, CONCLD, DCQ, DF_DMS, DF_H2O2,
        DF_H2SO4, DF_O3, DF_SO2, DF_SOAG, DMS_SRF, DP_KCLDBASE, DP_MFUP_MAX, DP_WCLDBASE, DSTSFMBL,
        DTCOND, DTENDTH, DTENDTQ, EXTINCT, FICE, FLDS, FLNS, FLNSC, FLNT, FLNTC, FLUT, FLUTC, FREQI,
        FREQL, FREQR, FREQS, FSDS, FSDSC, FSNS, FSNSC, FSNT, FSNTC, FSNTOA, FSNTOAC, FSUTOA,
        FSUTOAC, F_eff, H2O2_SRF, H2SO4_SRF, H2SO4_sfgaex1, ICEFRAC, ICIMR, ICWMR, IWC, LANDFRAC,
        LHFLX, LINOZ_DO3, LINOZ_DO3_PSC, LINOZ_O3CLIM, LINOZ_O3COL, LINOZ_SSO3, LINOZ_SZA, LND_MBL,
        LWCF, Mass_bc, Mass_dst, Mass_mom, Mass_ncl, Mass_pom, Mass_so4, Mass_soa, NUMICE, NUMLIQ,
        NUMRAI, NUMSNO, O3, O3_SRF, OCNFRAC, OMEGA, OMEGA500, OMEGAT, PBLH, PHIS, PRECC, PRECL,
        PRECSC, PRECSL, PS, PSL, Q, QFLX, QREFHT, QRL, QRS, RAINQM, RAM1, RELHUM, SFDMS, SFH2O2,
        SFH2SO4, SFO3, SFSO2, SFSOAG, SFbc_a1, SFbc_a3, SFbc_a4, SFdst_a1, SFdst_a3, SFmom_a1,
        SFmom_a2, SFmom_a3, SFmom_a4, SFncl_a1, SFncl_a2, SFncl_a3, SFnum_a1, SFnum_a2, SFnum_a3,
        SFnum_a4, SFpom_a1, SFpom_a3, SFpom_a4, SFso4_a1, SFso4_a2, SFso4_a3, SFsoa_a1, SFsoa_a2,
        SFsoa_a3, SHFLX, SH_KCLDBASE, SH_MFUP_MAX, SH_WCLDBASE, SNOWHICE, SNOWHLND, SNOWQM, SO2,
        SO2_CLXF, SO2_SRF, SOAG_CLXF, SOAG_SRF, SOAG_sfgaex1, SOLIN, SSAVIS, SSTSFMBL, SSTSFMBL_OM,
        SWCF, T, TAUGWX, TAUGWY, TAUX, TAUY, TGCLDCWP, TGCLDIWP, TGCLDLWP, TH7001000, TMQ, TREFHT,
        TROP_P, TROP_T, TS, TSMN, TSMX, TUH, TUQ, TVH, TVQ, U, U10, UU, V, VQ, VT, VU, VV, WD_H2O2,
        WD_H2SO4, WD_SO2, WSUB, Z3, aero_water, airFV, bc_a1DDF, bc_a1SFWET, bc_a1_SRF,
        bc_a1_sfgaex1, bc_a3DDF, bc_a3SFWET, bc_a3_SRF, bc_a4DDF, bc_a4SFWET, bc_a4_CLXF, bc_a4_SRF,
        bc_a4_sfgaex1, bc_c1DDF, bc_c1SFWET, bc_c3DDF, bc_c3SFWET, bc_c4DDF, bc_c4SFWET, chla,
        dst_a1DDF, dst_a1SF, dst_a1SFWET, dst_a1_SRF, dst_a3DDF, dst_a3SF, dst_a3SFWET, dst_a3_SRF,
        dst_c1DDF, dst_c1SFWET, dst_c3DDF, dst_c3SFWET, hstobie_linoz, mlip, mom_a1DDF, mom_a1SF,
        mom_a1SFWET, mom_a1_SRF, mom_a1_sfgaex1, mom_a2DDF, mom_a2SF, mom_a2SFWET, mom_a2_SRF,
        mom_a3DDF, mom_a3SFWET, mom_a3_SRF, mom_a4DDF, mom_a4SF, mom_a4SFWET, mom_a4_SRF,
        mom_a4_sfgaex1, mom_c1DDF, mom_c1SFWET, mom_c2DDF, mom_c2SFWET, mom_c3DDF, mom_c3SFWET,
        mom_c4DDF, mom_c4SFWET, mpoly, mprot, ncl_a1DDF, ncl_a1SF, ncl_a1SFWET, ncl_a1_SRF,
        ncl_a2DDF, ncl_a2SF, ncl_a2SFWET, ncl_a2_SRF, ncl_a3DDF, ncl_a3SF, ncl_a3SFWET, ncl_a3_SRF,
        ncl_c1DDF, ncl_c1SFWET, ncl_c2DDF, ncl_c2SFWET, ncl_c3DDF, ncl_c3SFWET, num_a1DDF, num_a1SF,
        num_a1SFWET, num_a1_CLXF, num_a1_SRF, num_a1_sfgaex1, num_a2DDF, num_a2SFWET, num_a2_CLXF,
        num_a2_SRF, num_a3DDF, num_a3SF, num_a3SFWET, num_a3_SRF, num_a4DDF, num_a4SFWET,
        num_a4_CLXF, num_a4_SRF, num_a4_sfgaex1, num_c1DDF, num_c1SFWET, num_c2DDF, num_c2SFWET,
        num_c3DDF, num_c3SFWET, num_c4DDF, num_c4SFWET, pom_a1DDF, pom_a1SFWET, pom_a1_SRF,
        pom_a1_sfgaex1, pom_a3DDF, pom_a3SFWET, pom_a3_SRF, pom_a4DDF, pom_a4SFWET, pom_a4_CLXF,
        pom_a4_SRF, pom_a4_sfgaex1, pom_c1DDF, pom_c1SFWET, pom_c3DDF, pom_c3SFWET, pom_c4DDF,
        pom_c4SFWET, so4_a1DDF, so4_a1SFWET, so4_a1_CLXF, so4_a1_SRF, so4_a1_sfgaex1, so4_a2DDF,
        so4_a2SFWET, so4_a2_CLXF, so4_a2_SRF, so4_a2_sfgaex1, so4_a3DDF, so4_a3SFWET, so4_a3_SRF,
        so4_a3_sfgaex1, so4_c1DDF, so4_c1SFWET, so4_c2DDF, so4_c2SFWET, so4_c3DDF, so4_c3SFWET,
        soa_a1DDF, soa_a1SFWET, soa_a1_SRF, soa_a1_sfgaex1, soa_a2DDF, soa_a2SFWET, soa_a2_SRF,
        soa_a2_sfgaex1, soa_a3DDF, soa_a3SFWET, soa_a3_SRF, soa_a3_sfgaex1, soa_c1DDF, soa_c1SFWET,
        soa_c2DDF, soa_c2SFWET, soa_c3DDF, soa_c3SFWET;

    int i, err, nerrs = 0, dimids[3], iattr, mdims = 1;
    int dim_ncol, dim_time, dim_nbnd, dim_chars, dim_lev, dim_ilev;
    float fillv = 1.e+36f, missv = 1.e+36f;

    /* global attributes: */
    iattr = 4;
    err   = nc_put_att (ncid, NC_GLOBAL, "ne", NC_INT, 1, &iattr);
    NC4ERR
    err = nc_put_att (ncid, NC_GLOBAL, "np", NC_INT, 1, &iattr);
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "Conventions", 6, "CF-1.0");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "source", 3, "CAM");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "case", 20, "FC5AV1C-H01B_ne4_ne4");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "title", 5, "UNSET");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "logname", 6, "wkliao");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "host", 10, "compute001");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "Version", 6, "$Name$");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "revision_Id", 4, "$Id$");
    NC4ERR
    err = nc_put_att_text (
        ncid, NC_GLOBAL, "initial_file", 86,
        "/home/climate1/acme/inputdata/atm/cam/inic/homme/cami_mam3_Linoz_ne4np4_L72_c160909.nc");
    NC4ERR
    err = nc_put_att_text (
        ncid, NC_GLOBAL, "topography_file", 79,
        "/home/climate1/acme/inputdata/atm/cam/topo/USGS-gtopo30_ne4np4_16x.c20160612.nc");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "time_period_freq", 5, "day_5");
    NC4ERR

    /* define dimensions */
    err = nc_def_dim (ncid, "ncol", dims[1], &dim_ncol);
    NC4ERR
    err = nc_def_dim (ncid, "time", NC_UNLIMITED, &dim_time);
    NC4ERR
    err = nc_def_dim (ncid, "nbnd", 2, &dim_nbnd);
    NC4ERR
    err = nc_def_dim (ncid, "chars", 8, &dim_chars);
    NC4ERR
    err = nc_def_dim (ncid, "lev", dims[0], &dim_lev);
    NC4ERR
    err = nc_def_dim (ncid, "ilev", dims[0] + 1, &dim_ilev);
    NC4ERR

    i = 0;

    /* define variables */
    dimids[0] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "lat", NC_DOUBLE, 1, dimids, &lat);
    NC4ERR
    err = nc_put_att_text (ncid, lat, "long_name", 8, "latitude");
    NC4ERR
    err = nc_put_att_text (ncid, lat, "units", 13, "degrees_north");
    NC4ERR
    varids[i++] = lat;

    dimids[0] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "lon", NC_DOUBLE, 1, dimids, &lon);
    NC4ERR
    err = nc_put_att_text (ncid, lon, "long_name", 9, "longitude");
    NC4ERR
    err = nc_put_att_text (ncid, lon, "units", 12, "degrees_east");
    NC4ERR
    varids[i++] = lon;

    dimids[0] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "area", NC_DOUBLE, 1, dimids, &area);
    NC4ERR
    err = nc_put_att_text (ncid, area, "long_name", 14, "gll grid areas");
    NC4ERR
    varids[i++] = area;

    dimids[0] = dim_lev;
    err       = NC4_DEF_VAR (ncid, "lev", NC_DOUBLE, 1, dimids, &lev);
    NC4ERR
    err = nc_put_att_text (ncid, lev, "long_name", 38, "hybrid level at midpoints (1000*(A+B))");
    NC4ERR
    err = nc_put_att_text (ncid, lev, "units", 3, "hPa");
    NC4ERR
    err = nc_put_att_text (ncid, lev, "positive", 4, "down");
    NC4ERR
    err = nc_put_att_text (ncid, lev, "standard_name", 43,
                           "atmosphere_hybrid_sigma_pressure_coordinate");
    NC4ERR
    err = nc_put_att_text (ncid, lev, "formula_terms", 29, "a: hyam b: hybm p0: P0 ps: PS");
    NC4ERR
    varids[i++] = lev;

    dimids[0] = dim_lev;
    err       = NC4_DEF_VAR (ncid, "hyam", NC_DOUBLE, 1, dimids, &hyam);
    NC4ERR
    err = nc_put_att_text (ncid, hyam, "long_name", 39, "hybrid A coefficient at layer midpoints");
    NC4ERR
    varids[i++] = hyam;

    dimids[0] = dim_lev;
    err       = NC4_DEF_VAR (ncid, "hybm", NC_DOUBLE, 1, dimids, &hybm);
    NC4ERR
    err = nc_put_att_text (ncid, hybm, "long_name", 39, "hybrid B coefficient at layer midpoints");
    NC4ERR
    varids[i++] = hybm;

    dimids[0] = dim_lev;
    err       = NC4_DEF_VAR (ncid, "P0", NC_DOUBLE, 0, NULL, &P0);
    NC4ERR
    err = nc_put_att_text (ncid, P0, "long_name", 18, "reference pressure");
    NC4ERR
    err = nc_put_att_text (ncid, P0, "units", 2, "Pa");
    NC4ERR
    varids[i++] = P0;

    dimids[0] = dim_ilev;
    err       = NC4_DEF_VAR (ncid, "ilev", NC_DOUBLE, 1, dimids, &ilev);
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "long_name", 39, "hybrid level at interfaces (1000*(A+B))");
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "units", 3, "hPa");
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "positive", 4, "down");
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "standard_name", 43,
                           "atmosphere_hybrid_sigma_pressure_coordinate");
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "formula_terms", 29, "a: hyai b: hybi p0: P0 ps: PS");
    NC4ERR
    varids[i++] = ilev;

    dimids[0] = dim_ilev;
    err       = NC4_DEF_VAR (ncid, "hyai", NC_DOUBLE, 1, dimids, &hyai);
    NC4ERR
    err = nc_put_att_text (ncid, hyai, "long_name", 40, "hybrid A coefficient at layer interfaces");
    NC4ERR
    varids[i++] = hyai;

    dimids[0] = dim_ilev;
    err       = NC4_DEF_VAR (ncid, "hybi", NC_DOUBLE, 1, dimids, &hybi);
    NC4ERR
    err = nc_put_att_text (ncid, hybi, "long_name", 40, "hybrid B coefficient at layer interfaces");
    NC4ERR
    varids[i++] = hybi;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "time", NC_DOUBLE, 1, dimids, &time);
    NC4ERR
    err = nc_put_att_text (ncid, time, "long_name", 4, "time");
    NC4ERR
    err = nc_put_att_text (ncid, time, "units", 30, "days since 0001-01-01 00:00:00");
    NC4ERR
    err = nc_put_att_text (ncid, time, "calendar", 6, "noleap");
    NC4ERR
    err = nc_put_att_text (ncid, time, "bounds", 9, "time_bnds");
    NC4ERR
    varids[i++] = time;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "date", NC_INT, 1, dimids, &date);
    NC4ERR
    err = nc_put_att_text (ncid, date, "long_name", 23, "current date (YYYYMMDD)");
    NC4ERR
    varids[i++] = date;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "datesec", NC_INT, 1, dimids, &datesec);
    NC4ERR
    err = nc_put_att_text (ncid, datesec, "long_name", 31, "current seconds of current date");
    NC4ERR
    varids[i++] = datesec;

    dimids[0] = dim_time;
    dimids[1] = dim_nbnd;
    err       = NC4_DEF_VAR (ncid, "time_bnds", NC_DOUBLE, 2, dimids, &time_bnds);
    NC4ERR
    err = nc_put_att_text (ncid, time_bnds, "long_name", 23, "time interval endpoints");
    NC4ERR
    varids[i++] = time_bnds;

    dimids[0] = dim_time;
    dimids[1] = dim_chars;
    err       = NC4_DEF_VAR (ncid, "date_written", NC_CHAR, 2, dimids, &date_written);
    NC4ERR
    varids[i++] = date_written;

    dimids[0] = dim_time;
    dimids[1] = dim_chars;
    err       = NC4_DEF_VAR (ncid, "time_written", NC_CHAR, 2, dimids, &time_written);
    NC4ERR
    varids[i++] = time_written;

    err = NC4_DEF_VAR (ncid, "ndbase", NC_INT, 0, NULL, &ndbase);
    NC4ERR
    err = nc_put_att_text (ncid, ndbase, "long_name", 8, "base day");
    NC4ERR
    varids[i++] = ndbase;
    err         = NC4_DEF_VAR (ncid, "nsbase", NC_INT, 0, NULL, &nsbase);
    NC4ERR
    err = nc_put_att_text (ncid, nsbase, "long_name", 19, "seconds of base day");
    NC4ERR
    varids[i++] = nsbase;

    err = NC4_DEF_VAR (ncid, "nbdate", NC_INT, 0, NULL, &nbdate);
    NC4ERR
    err = nc_put_att_text (ncid, nbdate, "long_name", 20, "base date (YYYYMMDD)");
    NC4ERR
    varids[i++] = nbdate;

    err = NC4_DEF_VAR (ncid, "nbsec", NC_INT, 0, NULL, &nbsec);
    NC4ERR
    err = nc_put_att_text (ncid, nbsec, "long_name", 20, "seconds of base date");
    NC4ERR
    varids[i++] = nbsec;

    err = NC4_DEF_VAR (ncid, "mdt", NC_INT, 0, NULL, &mdt);
    NC4ERR
    err = nc_put_att_text (ncid, mdt, "long_name", 8, "timestep");
    NC4ERR
    err = nc_put_att_text (ncid, mdt, "units", 1, "s");
    NC4ERR
    varids[i++] = mdt;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "ndcur", NC_INT, 1, dimids, &ndcur);
    NC4ERR
    err = nc_put_att_text (ncid, ndcur, "long_name", 27, "current day (from base day)");
    NC4ERR
    varids[i++] = ndcur;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "nscur", NC_INT, 1, dimids, &nscur);
    NC4ERR
    err = nc_put_att_text (ncid, nscur, "long_name", 30, "current seconds of current day");
    NC4ERR
    varids[i++] = nscur;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "co2vmr", NC_DOUBLE, 1, dimids, &co2vmr);
    NC4ERR
    err = nc_put_att_text (ncid, co2vmr, "long_name", 23, "co2 volume mixing ratio");
    NC4ERR
    varids[i++] = co2vmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "ch4vmr", NC_DOUBLE, 1, dimids, &ch4vmr);
    NC4ERR
    err = nc_put_att_text (ncid, ch4vmr, "long_name", 23, "ch4 volume mixing ratio");
    NC4ERR
    varids[i++] = ch4vmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "n2ovmr", NC_DOUBLE, 1, dimids, &n2ovmr);
    NC4ERR
    err = nc_put_att_text (ncid, n2ovmr, "long_name", 23, "n2o volume mixing ratio");
    NC4ERR
    varids[i++] = n2ovmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "f11vmr", NC_DOUBLE, 1, dimids, &f11vmr);
    NC4ERR
    err = nc_put_att_text (ncid, f11vmr, "long_name", 23, "f11 volume mixing ratio");
    NC4ERR
    varids[i++] = f11vmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "f12vmr", NC_DOUBLE, 1, dimids, &f12vmr);
    NC4ERR
    err = nc_put_att_text (ncid, f12vmr, "long_name", 23, "f12 volume mixing ratio");
    NC4ERR
    varids[i++] = f12vmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "sol_tsi", NC_DOUBLE, 1, dimids, &sol_tsi);
    NC4ERR
    err = nc_put_att_text (ncid, sol_tsi, "long_name", 22, "total solar irradiance");
    NC4ERR
    err = nc_put_att_text (ncid, sol_tsi, "units", 4, "W/m2");
    NC4ERR
    varids[i++] = sol_tsi;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "nsteph", NC_INT, 1, dimids, &nsteph);
    NC4ERR
    err = nc_put_att_text (ncid, nsteph, "long_name", 16, "current timestep");
    NC4ERR
    varids[i++] = nsteph;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AEROD_v", NC_FLOAT, 2, dimids, &AEROD_v);
    NC4ERR
    err = nc_put_att_float (ncid, AEROD_v, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AEROD_v, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AEROD_v, "units", 1, "1");
    NC4ERR
    err = nc_put_att_text (ncid, AEROD_v, "long_name", 43,
                           "Total Aerosol Optical Depth in visible band");
    NC4ERR
    err = nc_put_att_text (ncid, AEROD_v, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AEROD_v;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ANRAIN", NC_FLOAT, 3, dimids, &ANRAIN);
    NC4ERR
    err = nc_put_att_int (ncid, ANRAIN, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, ANRAIN, "units", 3, "m-3");
    NC4ERR
    err = nc_put_att_text (ncid, ANRAIN, "long_name", 24, "Average rain number conc");
    NC4ERR
    err = nc_put_att_text (ncid, ANRAIN, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ANRAIN;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ANSNOW", NC_FLOAT, 3, dimids, &ANSNOW);
    NC4ERR
    err = nc_put_att_int (ncid, ANSNOW, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, ANSNOW, "units", 3, "m-3");
    NC4ERR
    err = nc_put_att_text (ncid, ANSNOW, "long_name", 24, "Average snow number conc");
    NC4ERR
    err = nc_put_att_text (ncid, ANSNOW, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ANSNOW;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODABS", NC_FLOAT, 2, dimids, &AODABS);
    NC4ERR
    err = nc_put_att_float (ncid, AODABS, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODABS, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err =
        nc_put_att_text (ncid, AODABS, "long_name", 39, "Aerosol absorption optical depth 550 nm");
    NC4ERR
    err = nc_put_att_text (ncid, AODABS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODABS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODABSBC", NC_FLOAT, 2, dimids, &AODABSBC);
    NC4ERR
    err = nc_put_att_float (ncid, AODABSBC, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODABSBC, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODABSBC, "long_name", 47,
                           "Aerosol absorption optical depth 550 nm from BC");
    NC4ERR
    err = nc_put_att_text (ncid, AODABSBC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODABSBC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODBC", NC_FLOAT, 2, dimids, &AODBC);
    NC4ERR
    err = nc_put_att_float (ncid, AODBC, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODBC, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODBC, "long_name", 36, "Aerosol optical depth 550 nm from BC");
    NC4ERR
    err = nc_put_att_text (ncid, AODBC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODBC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODDUST", NC_FLOAT, 2, dimids, &AODDUST);
    NC4ERR
    err = nc_put_att_float (ncid, AODDUST, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODDUST, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err =
        nc_put_att_text (ncid, AODDUST, "long_name", 38, "Aerosol optical depth 550 nm from dust");
    NC4ERR
    err = nc_put_att_text (ncid, AODDUST, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODDUST;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODDUST1", NC_FLOAT, 2, dimids, &AODDUST1);
    NC4ERR
    err = nc_put_att_float (ncid, AODDUST1, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODDUST1, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODDUST1, "long_name", 46,
                           "Aerosol optical depth 550 nm model 1 from dust");
    NC4ERR
    err = nc_put_att_text (ncid, AODDUST1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODDUST1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODDUST3", NC_FLOAT, 2, dimids, &AODDUST3);
    NC4ERR
    err = nc_put_att_float (ncid, AODDUST3, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODDUST3, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODDUST3, "long_name", 46,
                           "Aerosol optical depth 550 nm model 3 from dust");
    NC4ERR
    err = nc_put_att_text (ncid, AODDUST3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODDUST3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODDUST4", NC_FLOAT, 2, dimids, &AODDUST4);
    NC4ERR
    err = nc_put_att_float (ncid, AODDUST4, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODDUST4, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODDUST4, "long_name", 46,
                           "Aerosol optical depth 550 nm model 4 from dust");
    NC4ERR
    err = nc_put_att_text (ncid, AODDUST4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODDUST4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODMODE1", NC_FLOAT, 2, dimids, &AODMODE1);
    NC4ERR
    err = nc_put_att_float (ncid, AODMODE1, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODMODE1, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODMODE1, "long_name", 35, "Aerosol optical depth 550 nm mode 1");
    NC4ERR
    err = nc_put_att_text (ncid, AODMODE1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODMODE1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODMODE2", NC_FLOAT, 2, dimids, &AODMODE2);
    NC4ERR
    err = nc_put_att_float (ncid, AODMODE2, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODMODE2, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODMODE2, "long_name", 35, "Aerosol optical depth 550 nm mode 2");
    NC4ERR
    err = nc_put_att_text (ncid, AODMODE2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODMODE2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODMODE3", NC_FLOAT, 2, dimids, &AODMODE3);
    NC4ERR
    err = nc_put_att_float (ncid, AODMODE3, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODMODE3, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODMODE3, "long_name", 35, "Aerosol optical depth 550 nm mode 3");
    NC4ERR
    err = nc_put_att_text (ncid, AODMODE3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODMODE3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODMODE4", NC_FLOAT, 2, dimids, &AODMODE4);
    NC4ERR
    err = nc_put_att_float (ncid, AODMODE4, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODMODE4, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODMODE4, "long_name", 35, "Aerosol optical depth 550 nm mode 4");
    NC4ERR
    err = nc_put_att_text (ncid, AODMODE4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODMODE4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODNIR", NC_FLOAT, 2, dimids, &AODNIR);
    NC4ERR
    err = nc_put_att_float (ncid, AODNIR, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODNIR, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODNIR, "long_name", 28, "Aerosol optical depth 850 nm");
    NC4ERR
    err = nc_put_att_text (ncid, AODNIR, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODNIR;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODPOM", NC_FLOAT, 2, dimids, &AODPOM);
    NC4ERR
    err = nc_put_att_float (ncid, AODPOM, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODPOM, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODPOM, "long_name", 37, "Aerosol optical depth 550 nm from POM");
    NC4ERR
    err = nc_put_att_text (ncid, AODPOM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODPOM;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODSO4", NC_FLOAT, 2, dimids, &AODSO4);
    NC4ERR
    err = nc_put_att_float (ncid, AODSO4, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODSO4, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODSO4, "long_name", 37, "Aerosol optical depth 550 nm from SO4");
    NC4ERR
    err = nc_put_att_text (ncid, AODSO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODSO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODSOA", NC_FLOAT, 2, dimids, &AODSOA);
    NC4ERR
    err = nc_put_att_float (ncid, AODSOA, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODSOA, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODSOA, "long_name", 37, "Aerosol optical depth 550 nm from SOA");
    NC4ERR
    err = nc_put_att_text (ncid, AODSOA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODSOA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODSS", NC_FLOAT, 2, dimids, &AODSS);
    NC4ERR
    err = nc_put_att_float (ncid, AODSS, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODSS, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err =
        nc_put_att_text (ncid, AODSS, "long_name", 41, "Aerosol optical depth 550 nm from seasalt");
    NC4ERR
    err = nc_put_att_text (ncid, AODSS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODSS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODUV", NC_FLOAT, 2, dimids, &AODUV);
    NC4ERR
    err = nc_put_att_float (ncid, AODUV, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODUV, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODUV, "long_name", 28, "Aerosol optical depth 350 nm");
    NC4ERR
    err = nc_put_att_text (ncid, AODUV, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODUV;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AODVIS", NC_FLOAT, 2, dimids, &AODVIS);
    NC4ERR
    err = nc_put_att_float (ncid, AODVIS, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, AODVIS, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, AODVIS, "long_name", 28, "Aerosol optical depth 550 nm");
    NC4ERR
    err = nc_put_att_text (ncid, AODVIS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODVIS;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AQRAIN", NC_FLOAT, 3, dimids, &AQRAIN);
    NC4ERR
    err = nc_put_att_int (ncid, AQRAIN, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, AQRAIN, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, AQRAIN, "long_name", 25, "Average rain mixing ratio");
    NC4ERR
    err = nc_put_att_text (ncid, AQRAIN, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQRAIN;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AQSNOW", NC_FLOAT, 3, dimids, &AQSNOW);
    NC4ERR
    err = nc_put_att_int (ncid, AQSNOW, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, AQSNOW, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, AQSNOW, "long_name", 25, "Average snow mixing ratio");
    NC4ERR
    err = nc_put_att_text (ncid, AQSNOW, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQSNOW;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AQ_DMS", NC_FLOAT, 2, dimids, &AQ_DMS);
    NC4ERR
    err = nc_put_att_text (ncid, AQ_DMS, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        nc_put_att_text (ncid, AQ_DMS, "long_name", 39, "DMS aqueous chemistry (for gas species)");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_DMS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_DMS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AQ_H2O2", NC_FLOAT, 2, dimids, &AQ_H2O2);
    NC4ERR
    err = nc_put_att_text (ncid, AQ_H2O2, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_H2O2, "long_name", 40,
                           "H2O2 aqueous chemistry (for gas species)");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_H2O2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_H2O2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AQ_H2SO4", NC_FLOAT, 2, dimids, &AQ_H2SO4);
    NC4ERR
    err = nc_put_att_text (ncid, AQ_H2SO4, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_H2SO4, "long_name", 41,
                           "H2SO4 aqueous chemistry (for gas species)");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_H2SO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_H2SO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AQ_O3", NC_FLOAT, 2, dimids, &AQ_O3);
    NC4ERR
    err = nc_put_att_text (ncid, AQ_O3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_O3, "long_name", 38, "O3 aqueous chemistry (for gas species)");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_O3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_O3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AQ_SO2", NC_FLOAT, 2, dimids, &AQ_SO2);
    NC4ERR
    err = nc_put_att_text (ncid, AQ_SO2, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        nc_put_att_text (ncid, AQ_SO2, "long_name", 39, "SO2 aqueous chemistry (for gas species)");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_SO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_SO2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AQ_SOAG", NC_FLOAT, 2, dimids, &AQ_SOAG);
    NC4ERR
    err = nc_put_att_text (ncid, AQ_SOAG, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_SOAG, "long_name", 40,
                           "SOAG aqueous chemistry (for gas species)");
    NC4ERR
    err = nc_put_att_text (ncid, AQ_SOAG, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_SOAG;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AREI", NC_FLOAT, 3, dimids, &AREI);
    NC4ERR
    err = nc_put_att_int (ncid, AREI, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, AREI, "units", 6, "Micron");
    NC4ERR
    err = nc_put_att_text (ncid, AREI, "long_name", 28, "Average ice effective radius");
    NC4ERR
    err = nc_put_att_text (ncid, AREI, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AREI;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AREL", NC_FLOAT, 3, dimids, &AREL);
    NC4ERR
    err = nc_put_att_int (ncid, AREL, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, AREL, "units", 6, "Micron");
    NC4ERR
    err = nc_put_att_text (ncid, AREL, "long_name", 32, "Average droplet effective radius");
    NC4ERR
    err = nc_put_att_text (ncid, AREL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AREL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AWNC", NC_FLOAT, 3, dimids, &AWNC);
    NC4ERR
    err = nc_put_att_int (ncid, AWNC, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, AWNC, "units", 3, "m-3");
    NC4ERR
    err = nc_put_att_text (ncid, AWNC, "long_name", 31, "Average cloud water number conc");
    NC4ERR
    err = nc_put_att_text (ncid, AWNC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AWNC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "AWNI", NC_FLOAT, 3, dimids, &AWNI);
    NC4ERR
    err = nc_put_att_int (ncid, AWNI, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, AWNI, "units", 3, "m-3");
    NC4ERR
    err = nc_put_att_text (ncid, AWNI, "long_name", 29, "Average cloud ice number conc");
    NC4ERR
    err = nc_put_att_text (ncid, AWNI, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AWNI;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CCN3", NC_FLOAT, 3, dimids, &CCN3);
    NC4ERR
    err = nc_put_att_int (ncid, CCN3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, CCN3, "units", 5, "#/cm3");
    NC4ERR
    err = nc_put_att_text (ncid, CCN3, "long_name", 27, "CCN concentration at S=0.1%");
    NC4ERR
    err = nc_put_att_text (ncid, CCN3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CCN3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CDNUMC", NC_FLOAT, 2, dimids, &CDNUMC);
    NC4ERR
    err = nc_put_att_text (ncid, CDNUMC, "units", 4, "1/m2");
    NC4ERR
    err = nc_put_att_text (ncid, CDNUMC, "long_name", 43,
                           "Vertically-integrated droplet concentration");
    NC4ERR
    err = nc_put_att_text (ncid, CDNUMC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CDNUMC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLDHGH", NC_FLOAT, 2, dimids, &CLDHGH);
    NC4ERR
    err = nc_put_att_text (ncid, CLDHGH, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLDHGH, "long_name", 32, "Vertically-integrated high cloud");
    NC4ERR
    err = nc_put_att_text (ncid, CLDHGH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDHGH;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLDICE", NC_FLOAT, 3, dimids, &CLDICE);
    NC4ERR
    err = nc_put_att_int (ncid, CLDICE, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, CLDICE, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, CLDICE, "long_name", 34, "Grid box averaged cloud ice amount");
    NC4ERR
    err = nc_put_att_text (ncid, CLDICE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDICE;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLDLIQ", NC_FLOAT, 3, dimids, &CLDLIQ);
    NC4ERR
    err = nc_put_att_int (ncid, CLDLIQ, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, CLDLIQ, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, CLDLIQ, "long_name", 37, "Grid box averaged cloud liquid amount");
    NC4ERR
    err = nc_put_att_text (ncid, CLDLIQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDLIQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLDLOW", NC_FLOAT, 2, dimids, &CLDLOW);
    NC4ERR
    err = nc_put_att_text (ncid, CLDLOW, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLDLOW, "long_name", 31, "Vertically-integrated low cloud");
    NC4ERR
    err = nc_put_att_text (ncid, CLDLOW, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDLOW;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLDMED", NC_FLOAT, 2, dimids, &CLDMED);
    NC4ERR
    err = nc_put_att_text (ncid, CLDMED, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLDMED, "long_name", 37, "Vertically-integrated mid-level cloud");
    NC4ERR
    err = nc_put_att_text (ncid, CLDMED, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDMED;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLDTOT", NC_FLOAT, 2, dimids, &CLDTOT);
    NC4ERR
    err = nc_put_att_text (ncid, CLDTOT, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLDTOT, "long_name", 33, "Vertically-integrated total cloud");
    NC4ERR
    err = nc_put_att_text (ncid, CLDTOT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDTOT;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLOUD", NC_FLOAT, 3, dimids, &CLOUD);
    NC4ERR
    err = nc_put_att_int (ncid, CLOUD, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, CLOUD, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLOUD, "long_name", 14, "Cloud fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLOUD, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLOUD;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLOUDFRAC_CLUBB", NC_FLOAT, 3, dimids, &CLOUDFRAC_CLUBB);
    NC4ERR
    err = nc_put_att_int (ncid, CLOUDFRAC_CLUBB, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, CLOUDFRAC_CLUBB, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLOUDFRAC_CLUBB, "long_name", 14, "Cloud Fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLOUDFRAC_CLUBB, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLOUDFRAC_CLUBB;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CONCLD", NC_FLOAT, 3, dimids, &CONCLD);
    NC4ERR
    err = nc_put_att_int (ncid, CONCLD, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, CONCLD, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CONCLD, "long_name", 22, "Convective cloud cover");
    NC4ERR
    err = nc_put_att_text (ncid, CONCLD, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CONCLD;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DCQ", NC_FLOAT, 3, dimids, &DCQ);
    NC4ERR
    err = nc_put_att_int (ncid, DCQ, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, DCQ, "units", 7, "kg/kg/s");
    NC4ERR
    err = nc_put_att_text (ncid, DCQ, "long_name", 33, "Q tendency due to moist processes");
    NC4ERR
    err = nc_put_att_text (ncid, DCQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DCQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DF_DMS", NC_FLOAT, 2, dimids, &DF_DMS);
    NC4ERR
    err = nc_put_att_text (ncid, DF_DMS, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, DF_DMS, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = nc_put_att_text (ncid, DF_DMS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_DMS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DF_H2O2", NC_FLOAT, 2, dimids, &DF_H2O2);
    NC4ERR
    err = nc_put_att_text (ncid, DF_H2O2, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, DF_H2O2, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = nc_put_att_text (ncid, DF_H2O2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_H2O2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DF_H2SO4", NC_FLOAT, 2, dimids, &DF_H2SO4);
    NC4ERR
    err = nc_put_att_text (ncid, DF_H2SO4, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, DF_H2SO4, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = nc_put_att_text (ncid, DF_H2SO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_H2SO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DF_O3", NC_FLOAT, 2, dimids, &DF_O3);
    NC4ERR
    err = nc_put_att_text (ncid, DF_O3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, DF_O3, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = nc_put_att_text (ncid, DF_O3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_O3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DF_SO2", NC_FLOAT, 2, dimids, &DF_SO2);
    NC4ERR
    err = nc_put_att_text (ncid, DF_SO2, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, DF_SO2, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = nc_put_att_text (ncid, DF_SO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_SO2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DF_SOAG", NC_FLOAT, 2, dimids, &DF_SOAG);
    NC4ERR
    err = nc_put_att_text (ncid, DF_SOAG, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, DF_SOAG, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = nc_put_att_text (ncid, DF_SOAG, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_SOAG;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DMS_SRF", NC_FLOAT, 2, dimids, &DMS_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, DMS_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = nc_put_att_text (ncid, DMS_SRF, "long_name", 19, "DMS in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, DMS_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DMS_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DP_KCLDBASE", NC_FLOAT, 2, dimids, &DP_KCLDBASE);
    NC4ERR
    err = nc_put_att_text (ncid, DP_KCLDBASE, "units", 1, "1");
    NC4ERR
    err = nc_put_att_text (ncid, DP_KCLDBASE, "long_name", 32, "Deep conv. cloudbase level index");
    NC4ERR
    err = nc_put_att_text (ncid, DP_KCLDBASE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DP_KCLDBASE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DP_MFUP_MAX", NC_FLOAT, 2, dimids, &DP_MFUP_MAX);
    NC4ERR
    err = nc_put_att_text (ncid, DP_MFUP_MAX, "units", 5, "kg/m2");
    NC4ERR
    err = nc_put_att_text (ncid, DP_MFUP_MAX, "long_name", 39,
                           "Deep conv. column-max updraft mass flux");
    NC4ERR
    err = nc_put_att_text (ncid, DP_MFUP_MAX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DP_MFUP_MAX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DP_WCLDBASE", NC_FLOAT, 2, dimids, &DP_WCLDBASE);
    NC4ERR
    err = nc_put_att_text (ncid, DP_WCLDBASE, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, DP_WCLDBASE, "long_name", 38,
                           "Deep conv. cloudbase vertical velocity");
    NC4ERR
    err = nc_put_att_text (ncid, DP_WCLDBASE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DP_WCLDBASE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DSTSFMBL", NC_FLOAT, 2, dimids, &DSTSFMBL);
    NC4ERR
    err = nc_put_att_text (ncid, DSTSFMBL, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, DSTSFMBL, "long_name", 28, "Mobilization flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, DSTSFMBL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DSTSFMBL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DTCOND", NC_FLOAT, 3, dimids, &DTCOND);
    NC4ERR
    err = nc_put_att_int (ncid, DTCOND, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, DTCOND, "units", 3, "K/s");
    NC4ERR
    err = nc_put_att_text (ncid, DTCOND, "long_name", 28, "T tendency - moist processes");
    NC4ERR
    err = nc_put_att_text (ncid, DTCOND, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DTCOND;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DTENDTH", NC_FLOAT, 2, dimids, &DTENDTH);
    NC4ERR
    err = nc_put_att_text (ncid, DTENDTH, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, DTENDTH, "long_name", 69,
                           "Dynamic Tendency of Total (vertically integrated) moist static energy");
    NC4ERR
    err = nc_put_att_text (ncid, DTENDTH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DTENDTH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "DTENDTQ", NC_FLOAT, 2, dimids, &DTENDTQ);
    NC4ERR
    err = nc_put_att_text (ncid, DTENDTQ, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, DTENDTQ, "long_name", 67,
                           "Dynamic Tendency of Total (vertically integrated) specific humidity");
    NC4ERR
    err = nc_put_att_text (ncid, DTENDTQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DTENDTQ;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "EXTINCT", NC_FLOAT, 3, dimids, &EXTINCT);
    NC4ERR
    err = nc_put_att_int (ncid, EXTINCT, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_float (ncid, EXTINCT, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, EXTINCT, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, EXTINCT, "units", 2, "/m");
    NC4ERR
    err = nc_put_att_text (ncid, EXTINCT, "long_name", 18, "Aerosol extinction");
    NC4ERR
    err = nc_put_att_text (ncid, EXTINCT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = EXTINCT;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FICE", NC_FLOAT, 3, dimids, &FICE);
    NC4ERR
    err = nc_put_att_int (ncid, FICE, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, FICE, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, FICE, "long_name", 35, "Fractional ice content within cloud");
    NC4ERR
    err = nc_put_att_text (ncid, FICE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FICE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FLDS", NC_FLOAT, 2, dimids, &FLDS);
    NC4ERR
    err = nc_put_att_text (ncid, FLDS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FLDS, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FLDS, "long_name", 36, "Downwelling longwave flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, FLDS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLDS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FLNS", NC_FLOAT, 2, dimids, &FLNS);
    NC4ERR
    err = nc_put_att_text (ncid, FLNS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FLNS, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FLNS, "long_name", 28, "Net longwave flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, FLNS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLNS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FLNSC", NC_FLOAT, 2, dimids, &FLNSC);
    NC4ERR
    err = nc_put_att_text (ncid, FLNSC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FLNSC, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FLNSC, "long_name", 37, "Clearsky net longwave flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, FLNSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLNSC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FLNT", NC_FLOAT, 2, dimids, &FLNT);
    NC4ERR
    err = nc_put_att_text (ncid, FLNT, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FLNT, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FLNT, "long_name", 33, "Net longwave flux at top of model");
    NC4ERR
    err = nc_put_att_text (ncid, FLNT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLNT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FLNTC", NC_FLOAT, 2, dimids, &FLNTC);
    NC4ERR
    err = nc_put_att_text (ncid, FLNTC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FLNTC, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FLNTC, "long_name", 42,
                           "Clearsky net longwave flux at top of model");
    NC4ERR
    err = nc_put_att_text (ncid, FLNTC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLNTC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FLUT", NC_FLOAT, 2, dimids, &FLUT);
    NC4ERR
    err = nc_put_att_text (ncid, FLUT, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FLUT, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FLUT, "long_name", 39, "Upwelling longwave flux at top of model");
    NC4ERR
    err = nc_put_att_text (ncid, FLUT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLUT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FLUTC", NC_FLOAT, 2, dimids, &FLUTC);
    NC4ERR
    err = nc_put_att_text (ncid, FLUTC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FLUTC, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FLUTC, "long_name", 48,
                           "Clearsky upwelling longwave flux at top of model");
    NC4ERR
    err = nc_put_att_text (ncid, FLUTC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLUTC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FREQI", NC_FLOAT, 3, dimids, &FREQI);
    NC4ERR
    err = nc_put_att_int (ncid, FREQI, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, FREQI, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, FREQI, "long_name", 28, "Fractional occurrence of ice");
    NC4ERR
    err = nc_put_att_text (ncid, FREQI, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FREQI;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FREQL", NC_FLOAT, 3, dimids, &FREQL);
    NC4ERR
    err = nc_put_att_int (ncid, FREQL, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, FREQL, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, FREQL, "long_name", 31, "Fractional occurrence of liquid");
    NC4ERR
    err = nc_put_att_text (ncid, FREQL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FREQL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FREQR", NC_FLOAT, 3, dimids, &FREQR);
    NC4ERR
    err = nc_put_att_int (ncid, FREQR, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, FREQR, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, FREQR, "long_name", 29, "Fractional occurrence of rain");
    NC4ERR
    err = nc_put_att_text (ncid, FREQR, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FREQR;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FREQS", NC_FLOAT, 3, dimids, &FREQS);
    NC4ERR
    err = nc_put_att_int (ncid, FREQS, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, FREQS, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, FREQS, "long_name", 29, "Fractional occurrence of snow");
    NC4ERR
    err = nc_put_att_text (ncid, FREQS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FREQS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSDS", NC_FLOAT, 2, dimids, &FSDS);
    NC4ERR
    err = nc_put_att_text (ncid, FSDS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSDS, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSDS, "long_name", 33, "Downwelling solar flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, FSDS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSDS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSDSC", NC_FLOAT, 2, dimids, &FSDSC);
    NC4ERR
    err = nc_put_att_text (ncid, FSDSC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSDSC, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSDSC, "long_name", 42,
                           "Clearsky downwelling solar flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, FSDSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSDSC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSNS", NC_FLOAT, 2, dimids, &FSNS);
    NC4ERR
    err = nc_put_att_text (ncid, FSNS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSNS, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSNS, "long_name", 25, "Net solar flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, FSNS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSNSC", NC_FLOAT, 2, dimids, &FSNSC);
    NC4ERR
    err = nc_put_att_text (ncid, FSNSC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSNSC, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSNSC, "long_name", 34, "Clearsky net solar flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, FSNSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNSC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSNT", NC_FLOAT, 2, dimids, &FSNT);
    NC4ERR
    err = nc_put_att_text (ncid, FSNT, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSNT, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSNT, "long_name", 30, "Net solar flux at top of model");
    NC4ERR
    err = nc_put_att_text (ncid, FSNT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSNTC", NC_FLOAT, 2, dimids, &FSNTC);
    NC4ERR
    err = nc_put_att_text (ncid, FSNTC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSNTC, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSNTC, "long_name", 39, "Clearsky net solar flux at top of model");
    NC4ERR
    err = nc_put_att_text (ncid, FSNTC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNTC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSNTOA", NC_FLOAT, 2, dimids, &FSNTOA);
    NC4ERR
    err = nc_put_att_text (ncid, FSNTOA, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSNTOA, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSNTOA, "long_name", 35, "Net solar flux at top of atmosphere");
    NC4ERR
    err = nc_put_att_text (ncid, FSNTOA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNTOA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSNTOAC", NC_FLOAT, 2, dimids, &FSNTOAC);
    NC4ERR
    err = nc_put_att_text (ncid, FSNTOAC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSNTOAC, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSNTOAC, "long_name", 44,
                           "Clearsky net solar flux at top of atmosphere");
    NC4ERR
    err = nc_put_att_text (ncid, FSNTOAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNTOAC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSUTOA", NC_FLOAT, 2, dimids, &FSUTOA);
    NC4ERR
    err = nc_put_att_text (ncid, FSUTOA, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSUTOA, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSUTOA, "long_name", 41,
                           "Upwelling solar flux at top of atmosphere");
    NC4ERR
    err = nc_put_att_text (ncid, FSUTOA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSUTOA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FSUTOAC", NC_FLOAT, 2, dimids, &FSUTOAC);
    NC4ERR
    err = nc_put_att_text (ncid, FSUTOAC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FSUTOAC, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FSUTOAC, "long_name", 50,
                           "Clearsky upwelling solar flux at top of atmosphere");
    NC4ERR
    err = nc_put_att_text (ncid, FSUTOAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSUTOAC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "F_eff", NC_FLOAT, 2, dimids, &F_eff);
    NC4ERR
    err = nc_put_att_text (ncid, F_eff, "units", 1, "1");
    NC4ERR
    err = nc_put_att_text (ncid, F_eff, "long_name", 52,
                           "Effective enrichment factor of marine organic matter");
    NC4ERR
    err = nc_put_att_text (ncid, F_eff, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = F_eff;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "H2O2_SRF", NC_FLOAT, 2, dimids, &H2O2_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, H2O2_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = nc_put_att_text (ncid, H2O2_SRF, "long_name", 20, "H2O2 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, H2O2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = H2O2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "H2SO4_SRF", NC_FLOAT, 2, dimids, &H2SO4_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, H2SO4_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = nc_put_att_text (ncid, H2SO4_SRF, "long_name", 21, "H2SO4 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, H2SO4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = H2SO4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "H2SO4_sfgaex1", NC_FLOAT, 2, dimids, &H2SO4_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, H2SO4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, H2SO4_sfgaex1, "long_name", 50,
                           "H2SO4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, H2SO4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = H2SO4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ICEFRAC", NC_FLOAT, 2, dimids, &ICEFRAC);
    NC4ERR
    err = nc_put_att_text (ncid, ICEFRAC, "units", 8, "fraction");
    NC4ERR
    err =
        nc_put_att_text (ncid, ICEFRAC, "long_name", 39, "Fraction of sfc area covered by sea-ice");
    NC4ERR
    err = nc_put_att_text (ncid, ICEFRAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ICEFRAC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ICIMR", NC_FLOAT, 3, dimids, &ICIMR);
    NC4ERR
    err = nc_put_att_int (ncid, ICIMR, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, ICIMR, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, ICIMR, "long_name", 36, "Prognostic in-cloud ice mixing ratio");
    NC4ERR
    err = nc_put_att_text (ncid, ICIMR, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ICIMR;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ICWMR", NC_FLOAT, 3, dimids, &ICWMR);
    NC4ERR
    err = nc_put_att_int (ncid, ICWMR, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, ICWMR, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, ICWMR, "long_name", 38, "Prognostic in-cloud water mixing ratio");
    NC4ERR
    err = nc_put_att_text (ncid, ICWMR, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ICWMR;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "IWC", NC_FLOAT, 3, dimids, &IWC);
    NC4ERR
    err = nc_put_att_int (ncid, IWC, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, IWC, "units", 5, "kg/m3");
    NC4ERR
    err = nc_put_att_text (ncid, IWC, "long_name", 34, "Grid box average ice water content");
    NC4ERR
    err = nc_put_att_text (ncid, IWC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = IWC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LANDFRAC", NC_FLOAT, 2, dimids, &LANDFRAC);
    NC4ERR
    err = nc_put_att_text (ncid, LANDFRAC, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, LANDFRAC, "long_name", 36, "Fraction of sfc area covered by land");
    NC4ERR
    err = nc_put_att_text (ncid, LANDFRAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LANDFRAC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LHFLX", NC_FLOAT, 2, dimids, &LHFLX);
    NC4ERR
    err = nc_put_att_text (ncid, LHFLX, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, LHFLX, "long_name", 24, "Surface latent heat flux");
    NC4ERR
    err = nc_put_att_text (ncid, LHFLX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LHFLX;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LINOZ_DO3", NC_FLOAT, 3, dimids, &LINOZ_DO3);
    NC4ERR
    err = nc_put_att_int (ncid, LINOZ_DO3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_DO3, "units", 2, "/s");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_DO3, "long_name", 48,
                           "ozone vmr tendency by linearized ozone chemistry");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_DO3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_DO3;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LINOZ_DO3_PSC", NC_FLOAT, 3, dimids, &LINOZ_DO3_PSC);
    NC4ERR
    err = nc_put_att_int (ncid, LINOZ_DO3_PSC, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_DO3_PSC, "units", 2, "/s");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_DO3_PSC, "long_name", 50,
                           "ozone vmr loss by PSCs using Carille et al. (1990)");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_DO3_PSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_DO3_PSC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LINOZ_O3CLIM", NC_FLOAT, 3, dimids, &LINOZ_O3CLIM);
    NC4ERR
    err = nc_put_att_int (ncid, LINOZ_O3CLIM, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_O3CLIM, "units", 7, "mol/mol");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_O3CLIM, "long_name", 29, "climatology of ozone in LINOZ");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_O3CLIM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_O3CLIM;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LINOZ_O3COL", NC_FLOAT, 3, dimids, &LINOZ_O3COL);
    NC4ERR
    err = nc_put_att_int (ncid, LINOZ_O3COL, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_O3COL, "units", 2, "DU");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_O3COL, "long_name", 18, "ozone column above");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_O3COL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_O3COL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LINOZ_SSO3", NC_FLOAT, 3, dimids, &LINOZ_SSO3);
    NC4ERR
    err = nc_put_att_int (ncid, LINOZ_SSO3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_SSO3, "units", 2, "kg");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_SSO3, "long_name", 27, "steady state ozone in LINOZ");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_SSO3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_SSO3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LINOZ_SZA", NC_FLOAT, 2, dimids, &LINOZ_SZA);
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_SZA, "units", 7, "degrees");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_SZA, "long_name", 27, "solar zenith angle in LINOZ");
    NC4ERR
    err = nc_put_att_text (ncid, LINOZ_SZA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_SZA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LND_MBL", NC_FLOAT, 2, dimids, &LND_MBL);
    NC4ERR
    err = nc_put_att_text (ncid, LND_MBL, "units", 4, "frac");
    NC4ERR
    err = nc_put_att_text (ncid, LND_MBL, "long_name", 23, "Soil erodibility factor");
    NC4ERR
    err = nc_put_att_text (ncid, LND_MBL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LND_MBL;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LWCF", NC_FLOAT, 2, dimids, &LWCF);
    NC4ERR
    err = nc_put_att_text (ncid, LWCF, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, LWCF, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, LWCF, "long_name", 22, "Longwave cloud forcing");
    NC4ERR
    err = nc_put_att_text (ncid, LWCF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LWCF;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Mass_bc", NC_FLOAT, 3, dimids, &Mass_bc);
    NC4ERR
    err = nc_put_att_int (ncid, Mass_bc, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, Mass_bc, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, Mass_bc, "long_name", 64,
                           "sum of bc mass concentration bc_a1+bc_c1+bc_a3+bc_c3+bc_a4+bc_c4");
    NC4ERR
    err = nc_put_att_text (ncid, Mass_bc, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_bc;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Mass_dst", NC_FLOAT, 3, dimids, &Mass_dst);
    NC4ERR
    err = nc_put_att_int (ncid, Mass_dst, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, Mass_dst, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, Mass_dst, "long_name", 57,
                           "sum of dst mass concentration dst_a1+dst_c1+dst_a3+dst_c3");
    NC4ERR
    err = nc_put_att_text (ncid, Mass_dst, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_dst;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Mass_mom", NC_FLOAT, 3, dimids, &Mass_mom);
    NC4ERR
    err = nc_put_att_int (ncid, Mass_mom, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, Mass_mom, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (
        ncid, Mass_mom, "long_name", 85,
        "sum of mom mass concentration mom_a1+mom_c1+mom_a2+mom_c2+mom_a3+mom_c3+mom_a4+mom_c4");
    NC4ERR
    err = nc_put_att_text (ncid, Mass_mom, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_mom;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Mass_ncl", NC_FLOAT, 3, dimids, &Mass_ncl);
    NC4ERR
    err = nc_put_att_int (ncid, Mass_ncl, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, Mass_ncl, "units", 5, "kg/kg");
    NC4ERR
    err =
        nc_put_att_text (ncid, Mass_ncl, "long_name", 71,
                         "sum of ncl mass concentration ncl_a1+ncl_c1+ncl_a2+ncl_c2+ncl_a3+ncl_c3");
    NC4ERR
    err = nc_put_att_text (ncid, Mass_ncl, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_ncl;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Mass_pom", NC_FLOAT, 3, dimids, &Mass_pom);
    NC4ERR
    err = nc_put_att_int (ncid, Mass_pom, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, Mass_pom, "units", 5, "kg/kg");
    NC4ERR
    err =
        nc_put_att_text (ncid, Mass_pom, "long_name", 71,
                         "sum of pom mass concentration pom_a1+pom_c1+pom_a3+pom_c3+pom_a4+pom_c4");
    NC4ERR
    err = nc_put_att_text (ncid, Mass_pom, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_pom;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Mass_so4", NC_FLOAT, 3, dimids, &Mass_so4);
    NC4ERR
    err = nc_put_att_int (ncid, Mass_so4, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, Mass_so4, "units", 5, "kg/kg");
    NC4ERR
    err =
        nc_put_att_text (ncid, Mass_so4, "long_name", 71,
                         "sum of so4 mass concentration so4_a1+so4_c1+so4_a2+so4_c2+so4_a3+so4_c3");
    NC4ERR
    err = nc_put_att_text (ncid, Mass_so4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_so4;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Mass_soa", NC_FLOAT, 3, dimids, &Mass_soa);
    NC4ERR
    err = nc_put_att_int (ncid, Mass_soa, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, Mass_soa, "units", 5, "kg/kg");
    NC4ERR
    err =
        nc_put_att_text (ncid, Mass_soa, "long_name", 71,
                         "sum of soa mass concentration soa_a1+soa_c1+soa_a2+soa_c2+soa_a3+soa_c3");
    NC4ERR
    err = nc_put_att_text (ncid, Mass_soa, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_soa;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "NUMICE", NC_FLOAT, 3, dimids, &NUMICE);
    NC4ERR
    err = nc_put_att_int (ncid, NUMICE, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, NUMICE, "units", 4, "1/kg");
    NC4ERR
    err = nc_put_att_text (ncid, NUMICE, "long_name", 34, "Grid box averaged cloud ice number");
    NC4ERR
    err = nc_put_att_text (ncid, NUMICE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = NUMICE;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "NUMLIQ", NC_FLOAT, 3, dimids, &NUMLIQ);
    NC4ERR
    err = nc_put_att_int (ncid, NUMLIQ, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, NUMLIQ, "units", 4, "1/kg");
    NC4ERR
    err = nc_put_att_text (ncid, NUMLIQ, "long_name", 37, "Grid box averaged cloud liquid number");
    NC4ERR
    err = nc_put_att_text (ncid, NUMLIQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = NUMLIQ;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "NUMRAI", NC_FLOAT, 3, dimids, &NUMRAI);
    NC4ERR
    err = nc_put_att_int (ncid, NUMRAI, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, NUMRAI, "units", 4, "1/kg");
    NC4ERR
    err = nc_put_att_text (ncid, NUMRAI, "long_name", 29, "Grid box averaged rain number");
    NC4ERR
    err = nc_put_att_text (ncid, NUMRAI, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = NUMRAI;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "NUMSNO", NC_FLOAT, 3, dimids, &NUMSNO);
    NC4ERR
    err = nc_put_att_int (ncid, NUMSNO, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, NUMSNO, "units", 4, "1/kg");
    NC4ERR
    err = nc_put_att_text (ncid, NUMSNO, "long_name", 29, "Grid box averaged snow number");
    NC4ERR
    err = nc_put_att_text (ncid, NUMSNO, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = NUMSNO;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "O3", NC_FLOAT, 3, dimids, &O3);
    NC4ERR
    err = nc_put_att_int (ncid, O3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, O3, "units", 7, "mol/mol");
    NC4ERR
    err = nc_put_att_text (ncid, O3, "long_name", 16, "O3 concentration");
    NC4ERR
    err = nc_put_att_text (ncid, O3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = O3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "O3_SRF", NC_FLOAT, 2, dimids, &O3_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, O3_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = nc_put_att_text (ncid, O3_SRF, "long_name", 18, "O3 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, O3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = O3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "OCNFRAC", NC_FLOAT, 2, dimids, &OCNFRAC);
    NC4ERR
    err = nc_put_att_text (ncid, OCNFRAC, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, OCNFRAC, "long_name", 37, "Fraction of sfc area covered by ocean");
    NC4ERR
    err = nc_put_att_text (ncid, OCNFRAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = OCNFRAC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "OMEGA", NC_FLOAT, 3, dimids, &OMEGA);
    NC4ERR
    err = nc_put_att_int (ncid, OMEGA, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA, "units", 4, "Pa/s");
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA, "long_name", 28, "Vertical velocity (pressure)");
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = OMEGA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "OMEGA500", NC_FLOAT, 2, dimids, &OMEGA500);
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA500, "units", 4, "Pa/s");
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA500, "long_name", 46,
                           "Vertical velocity at 500 mbar pressure surface");
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA500, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = OMEGA500;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "OMEGAT", NC_FLOAT, 3, dimids, &OMEGAT);
    NC4ERR
    err = nc_put_att_int (ncid, OMEGAT, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, OMEGAT, "units", 6, "K Pa/s");
    NC4ERR
    err = nc_put_att_text (ncid, OMEGAT, "long_name", 18, "Vertical heat flux");
    NC4ERR
    err = nc_put_att_text (ncid, OMEGAT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = OMEGAT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PBLH", NC_FLOAT, 2, dimids, &PBLH);
    NC4ERR
    err = nc_put_att_text (ncid, PBLH, "units", 1, "m");
    NC4ERR
    err = nc_put_att_text (ncid, PBLH, "long_name", 10, "PBL height");
    NC4ERR
    err = nc_put_att_text (ncid, PBLH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PBLH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PHIS", NC_FLOAT, 2, dimids, &PHIS);
    NC4ERR
    err = nc_put_att_text (ncid, PHIS, "units", 5, "m2/s2");
    NC4ERR
    err = nc_put_att_text (ncid, PHIS, "long_name", 20, "Surface geopotential");
    NC4ERR
    varids[i++] = PHIS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PRECC", NC_FLOAT, 2, dimids, &PRECC);
    NC4ERR
    err = nc_put_att_text (ncid, PRECC, "units", 3, "m/s");
    NC4ERR
    err =
        nc_put_att_text (ncid, PRECC, "long_name", 41, "Convective precipitation rate (liq + ice)");
    NC4ERR
    err = nc_put_att_text (ncid, PRECC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PRECC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PRECL", NC_FLOAT, 2, dimids, &PRECL);
    NC4ERR
    err = nc_put_att_text (ncid, PRECL, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, PRECL, "long_name", 51,
                           "Large-scale (stable) precipitation rate (liq + ice)");
    NC4ERR
    err = nc_put_att_text (ncid, PRECL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PRECL;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PRECSC", NC_FLOAT, 2, dimids, &PRECSC);
    NC4ERR
    err = nc_put_att_text (ncid, PRECSC, "units", 3, "m/s");
    NC4ERR
    err =
        nc_put_att_text (ncid, PRECSC, "long_name", 39, "Convective snow rate (water equivalent)");
    NC4ERR
    err = nc_put_att_text (ncid, PRECSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PRECSC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PRECSL", NC_FLOAT, 2, dimids, &PRECSL);
    NC4ERR
    err = nc_put_att_text (ncid, PRECSL, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, PRECSL, "long_name", 49,
                           "Large-scale (stable) snow rate (water equivalent)");
    NC4ERR
    err = nc_put_att_text (ncid, PRECSL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PRECSL;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PS", NC_FLOAT, 2, dimids, &PS);
    NC4ERR
    err = nc_put_att_text (ncid, PS, "units", 2, "Pa");
    NC4ERR
    err = nc_put_att_text (ncid, PS, "long_name", 16, "Surface pressure");
    NC4ERR
    err = nc_put_att_text (ncid, PS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PSL", NC_FLOAT, 2, dimids, &PSL);
    NC4ERR
    err = nc_put_att_text (ncid, PSL, "units", 2, "Pa");
    NC4ERR
    err = nc_put_att_text (ncid, PSL, "long_name", 18, "Sea level pressure");
    NC4ERR
    err = nc_put_att_text (ncid, PSL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PSL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Q", NC_FLOAT, 3, dimids, &Q);
    NC4ERR
    err = nc_put_att_int (ncid, Q, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, Q, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, Q, "long_name", 17, "Specific humidity");
    NC4ERR
    err = nc_put_att_text (ncid, Q, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Q;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "QFLX", NC_FLOAT, 2, dimids, &QFLX);
    NC4ERR
    err = nc_put_att_text (ncid, QFLX, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, QFLX, "long_name", 18, "Surface water flux");
    NC4ERR
    err = nc_put_att_text (ncid, QFLX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = QFLX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "QREFHT", NC_FLOAT, 2, dimids, &QREFHT);
    NC4ERR
    err = nc_put_att_text (ncid, QREFHT, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, QREFHT, "long_name", 25, "Reference height humidity");
    NC4ERR
    err = nc_put_att_text (ncid, QREFHT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = QREFHT;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "QRL", NC_FLOAT, 3, dimids, &QRL);
    NC4ERR
    err = nc_put_att_int (ncid, QRL, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, QRL, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, QRL, "units", 3, "K/s");
    NC4ERR
    err = nc_put_att_text (ncid, QRL, "long_name", 21, "Longwave heating rate");
    NC4ERR
    err = nc_put_att_text (ncid, QRL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = QRL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "QRS", NC_FLOAT, 3, dimids, &QRS);
    NC4ERR
    err = nc_put_att_int (ncid, QRS, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, QRS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, QRS, "units", 3, "K/s");
    NC4ERR
    err = nc_put_att_text (ncid, QRS, "long_name", 18, "Solar heating rate");
    NC4ERR
    err = nc_put_att_text (ncid, QRS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = QRS;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "RAINQM", NC_FLOAT, 3, dimids, &RAINQM);
    NC4ERR
    err = nc_put_att_int (ncid, RAINQM, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, RAINQM, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, RAINQM, "long_name", 29, "Grid box averaged rain amount");
    NC4ERR
    err = nc_put_att_text (ncid, RAINQM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = RAINQM;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "RAM1", NC_FLOAT, 2, dimids, &RAM1);
    NC4ERR
    err = nc_put_att_text (ncid, RAM1, "units", 4, "frac");
    NC4ERR
    err = nc_put_att_text (ncid, RAM1, "long_name", 4, "RAM1");
    NC4ERR
    err = nc_put_att_text (ncid, RAM1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = RAM1;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "RELHUM", NC_FLOAT, 3, dimids, &RELHUM);
    NC4ERR
    err = nc_put_att_int (ncid, RELHUM, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, RELHUM, "units", 7, "percent");
    NC4ERR
    err = nc_put_att_text (ncid, RELHUM, "long_name", 17, "Relative humidity");
    NC4ERR
    err = nc_put_att_text (ncid, RELHUM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = RELHUM;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFDMS", NC_FLOAT, 2, dimids, &SFDMS);
    NC4ERR
    err = nc_put_att_text (ncid, SFDMS, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFDMS, "long_name", 16, "DMS surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFDMS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFDMS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFH2O2", NC_FLOAT, 2, dimids, &SFH2O2);
    NC4ERR
    err = nc_put_att_text (ncid, SFH2O2, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFH2O2, "long_name", 17, "H2O2 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFH2O2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFH2O2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFH2SO4", NC_FLOAT, 2, dimids, &SFH2SO4);
    NC4ERR
    err = nc_put_att_text (ncid, SFH2SO4, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFH2SO4, "long_name", 18, "H2SO4 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFH2SO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFH2SO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFO3", NC_FLOAT, 2, dimids, &SFO3);
    NC4ERR
    err = nc_put_att_text (ncid, SFO3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFO3, "long_name", 15, "O3 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFO3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFO3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFSO2", NC_FLOAT, 2, dimids, &SFSO2);
    NC4ERR
    err = nc_put_att_text (ncid, SFSO2, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFSO2, "long_name", 16, "SO2 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFSO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFSO2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFSOAG", NC_FLOAT, 2, dimids, &SFSOAG);
    NC4ERR
    err = nc_put_att_text (ncid, SFSOAG, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFSOAG, "long_name", 17, "SOAG surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFSOAG, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFSOAG;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFbc_a1", NC_FLOAT, 2, dimids, &SFbc_a1);
    NC4ERR
    err = nc_put_att_text (ncid, SFbc_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFbc_a1, "long_name", 18, "bc_a1 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFbc_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFbc_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFbc_a3", NC_FLOAT, 2, dimids, &SFbc_a3);
    NC4ERR
    err = nc_put_att_text (ncid, SFbc_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFbc_a3, "long_name", 18, "bc_a3 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFbc_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFbc_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFbc_a4", NC_FLOAT, 2, dimids, &SFbc_a4);
    NC4ERR
    err = nc_put_att_text (ncid, SFbc_a4, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFbc_a4, "long_name", 18, "bc_a4 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFbc_a4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFbc_a4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFdst_a1", NC_FLOAT, 2, dimids, &SFdst_a1);
    NC4ERR
    err = nc_put_att_text (ncid, SFdst_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFdst_a1, "long_name", 19, "dst_a1 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFdst_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFdst_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFdst_a3", NC_FLOAT, 2, dimids, &SFdst_a3);
    NC4ERR
    err = nc_put_att_text (ncid, SFdst_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFdst_a3, "long_name", 19, "dst_a3 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFdst_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFdst_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFmom_a1", NC_FLOAT, 2, dimids, &SFmom_a1);
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a1, "long_name", 19, "mom_a1 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFmom_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFmom_a2", NC_FLOAT, 2, dimids, &SFmom_a2);
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a2, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a2, "long_name", 19, "mom_a2 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFmom_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFmom_a3", NC_FLOAT, 2, dimids, &SFmom_a3);
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a3, "long_name", 19, "mom_a3 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFmom_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFmom_a4", NC_FLOAT, 2, dimids, &SFmom_a4);
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a4, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a4, "long_name", 19, "mom_a4 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFmom_a4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFmom_a4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFncl_a1", NC_FLOAT, 2, dimids, &SFncl_a1);
    NC4ERR
    err = nc_put_att_text (ncid, SFncl_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFncl_a1, "long_name", 19, "ncl_a1 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFncl_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFncl_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFncl_a2", NC_FLOAT, 2, dimids, &SFncl_a2);
    NC4ERR
    err = nc_put_att_text (ncid, SFncl_a2, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFncl_a2, "long_name", 19, "ncl_a2 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFncl_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFncl_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFncl_a3", NC_FLOAT, 2, dimids, &SFncl_a3);
    NC4ERR
    err = nc_put_att_text (ncid, SFncl_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFncl_a3, "long_name", 19, "ncl_a3 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFncl_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFncl_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFnum_a1", NC_FLOAT, 2, dimids, &SFnum_a1);
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a1, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a1, "long_name", 19, "num_a1 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFnum_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFnum_a2", NC_FLOAT, 2, dimids, &SFnum_a2);
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a2, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a2, "long_name", 19, "num_a2 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFnum_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFnum_a3", NC_FLOAT, 2, dimids, &SFnum_a3);
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a3, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a3, "long_name", 19, "num_a3 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFnum_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFnum_a4", NC_FLOAT, 2, dimids, &SFnum_a4);
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a4, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a4, "long_name", 19, "num_a4 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFnum_a4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFnum_a4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFpom_a1", NC_FLOAT, 2, dimids, &SFpom_a1);
    NC4ERR
    err = nc_put_att_text (ncid, SFpom_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFpom_a1, "long_name", 19, "pom_a1 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFpom_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFpom_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFpom_a3", NC_FLOAT, 2, dimids, &SFpom_a3);
    NC4ERR
    err = nc_put_att_text (ncid, SFpom_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFpom_a3, "long_name", 19, "pom_a3 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFpom_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFpom_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFpom_a4", NC_FLOAT, 2, dimids, &SFpom_a4);
    NC4ERR
    err = nc_put_att_text (ncid, SFpom_a4, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFpom_a4, "long_name", 19, "pom_a4 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFpom_a4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFpom_a4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFso4_a1", NC_FLOAT, 2, dimids, &SFso4_a1);
    NC4ERR
    err = nc_put_att_text (ncid, SFso4_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFso4_a1, "long_name", 19, "so4_a1 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFso4_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFso4_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFso4_a2", NC_FLOAT, 2, dimids, &SFso4_a2);
    NC4ERR
    err = nc_put_att_text (ncid, SFso4_a2, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFso4_a2, "long_name", 19, "so4_a2 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFso4_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFso4_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFso4_a3", NC_FLOAT, 2, dimids, &SFso4_a3);
    NC4ERR
    err = nc_put_att_text (ncid, SFso4_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFso4_a3, "long_name", 19, "so4_a3 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFso4_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFso4_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFsoa_a1", NC_FLOAT, 2, dimids, &SFsoa_a1);
    NC4ERR
    err = nc_put_att_text (ncid, SFsoa_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFsoa_a1, "long_name", 19, "soa_a1 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFsoa_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFsoa_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFsoa_a2", NC_FLOAT, 2, dimids, &SFsoa_a2);
    NC4ERR
    err = nc_put_att_text (ncid, SFsoa_a2, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFsoa_a2, "long_name", 19, "soa_a2 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFsoa_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFsoa_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SFsoa_a3", NC_FLOAT, 2, dimids, &SFsoa_a3);
    NC4ERR
    err = nc_put_att_text (ncid, SFsoa_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SFsoa_a3, "long_name", 19, "soa_a3 surface flux");
    NC4ERR
    err = nc_put_att_text (ncid, SFsoa_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFsoa_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SHFLX", NC_FLOAT, 2, dimids, &SHFLX);
    NC4ERR
    err = nc_put_att_text (ncid, SHFLX, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, SHFLX, "long_name", 26, "Surface sensible heat flux");
    NC4ERR
    err = nc_put_att_text (ncid, SHFLX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SHFLX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SH_KCLDBASE", NC_FLOAT, 2, dimids, &SH_KCLDBASE);
    NC4ERR
    err = nc_put_att_text (ncid, SH_KCLDBASE, "units", 1, "1");
    NC4ERR
    err =
        nc_put_att_text (ncid, SH_KCLDBASE, "long_name", 35, "Shallow conv. cloudbase level index");
    NC4ERR
    err = nc_put_att_text (ncid, SH_KCLDBASE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SH_KCLDBASE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SH_MFUP_MAX", NC_FLOAT, 2, dimids, &SH_MFUP_MAX);
    NC4ERR
    err = nc_put_att_text (ncid, SH_MFUP_MAX, "units", 5, "kg/m2");
    NC4ERR
    err = nc_put_att_text (ncid, SH_MFUP_MAX, "long_name", 42,
                           "Shallow conv. column-max updraft mass flux");
    NC4ERR
    err = nc_put_att_text (ncid, SH_MFUP_MAX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SH_MFUP_MAX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SH_WCLDBASE", NC_FLOAT, 2, dimids, &SH_WCLDBASE);
    NC4ERR
    err = nc_put_att_text (ncid, SH_WCLDBASE, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, SH_WCLDBASE, "long_name", 41,
                           "Shallow conv. cloudbase vertical velocity");
    NC4ERR
    err = nc_put_att_text (ncid, SH_WCLDBASE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SH_WCLDBASE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SNOWHICE", NC_FLOAT, 2, dimids, &SNOWHICE);
    NC4ERR
    err = nc_put_att_text (ncid, SNOWHICE, "units", 1, "m");
    NC4ERR
    err = nc_put_att_text (ncid, SNOWHICE, "long_name", 19, "Snow depth over ice");
    NC4ERR
    err = nc_put_att_text (ncid, SNOWHICE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SNOWHICE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SNOWHLND", NC_FLOAT, 2, dimids, &SNOWHLND);
    NC4ERR
    err = nc_put_att_text (ncid, SNOWHLND, "units", 1, "m");
    NC4ERR
    err = nc_put_att_text (ncid, SNOWHLND, "long_name", 27, "Water equivalent snow depth");
    NC4ERR
    err = nc_put_att_text (ncid, SNOWHLND, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SNOWHLND;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SNOWQM", NC_FLOAT, 3, dimids, &SNOWQM);
    NC4ERR
    err = nc_put_att_int (ncid, SNOWQM, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, SNOWQM, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, SNOWQM, "long_name", 29, "Grid box averaged snow amount");
    NC4ERR
    err = nc_put_att_text (ncid, SNOWQM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SNOWQM;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SO2", NC_FLOAT, 3, dimids, &SO2);
    NC4ERR
    err = nc_put_att_int (ncid, SO2, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, SO2, "units", 7, "mol/mol");
    NC4ERR
    err = nc_put_att_text (ncid, SO2, "long_name", 17, "SO2 concentration");
    NC4ERR
    err = nc_put_att_text (ncid, SO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SO2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SO2_CLXF", NC_FLOAT, 2, dimids, &SO2_CLXF);
    NC4ERR
    err = nc_put_att_text (ncid, SO2_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SO2_CLXF, "long_name", 47,
                           "vertically intergrated external forcing for SO2");
    NC4ERR
    err = nc_put_att_text (ncid, SO2_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SO2_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SO2_SRF", NC_FLOAT, 2, dimids, &SO2_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, SO2_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = nc_put_att_text (ncid, SO2_SRF, "long_name", 19, "SO2 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, SO2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SO2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SOAG_CLXF", NC_FLOAT, 2, dimids, &SOAG_CLXF);
    NC4ERR
    err = nc_put_att_text (ncid, SOAG_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SOAG_CLXF, "long_name", 48,
                           "vertically intergrated external forcing for SOAG");
    NC4ERR
    err = nc_put_att_text (ncid, SOAG_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SOAG_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SOAG_SRF", NC_FLOAT, 2, dimids, &SOAG_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, SOAG_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = nc_put_att_text (ncid, SOAG_SRF, "long_name", 20, "SOAG in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, SOAG_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SOAG_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SOAG_sfgaex1", NC_FLOAT, 2, dimids, &SOAG_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, SOAG_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SOAG_sfgaex1, "long_name", 49,
                           "SOAG gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, SOAG_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SOAG_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SOLIN", NC_FLOAT, 2, dimids, &SOLIN);
    NC4ERR
    err = nc_put_att_text (ncid, SOLIN, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, SOLIN, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, SOLIN, "long_name", 16, "Solar insolation");
    NC4ERR
    err = nc_put_att_text (ncid, SOLIN, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SOLIN;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SSAVIS", NC_FLOAT, 2, dimids, &SSAVIS);
    NC4ERR
    err = nc_put_att_float (ncid, SSAVIS, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, SSAVIS, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, SSAVIS, "long_name", 29, "Aerosol singel-scatter albedo");
    NC4ERR
    err = nc_put_att_text (ncid, SSAVIS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SSAVIS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SSTSFMBL", NC_FLOAT, 2, dimids, &SSTSFMBL);
    NC4ERR
    err = nc_put_att_text (ncid, SSTSFMBL, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SSTSFMBL, "long_name", 28, "Mobilization flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, SSTSFMBL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SSTSFMBL;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SSTSFMBL_OM", NC_FLOAT, 2, dimids, &SSTSFMBL_OM);
    NC4ERR
    err = nc_put_att_text (ncid, SSTSFMBL_OM, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, SSTSFMBL_OM, "long_name", 53,
                           "Mobilization flux of marine organic matter at surface");
    NC4ERR
    err = nc_put_att_text (ncid, SSTSFMBL_OM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SSTSFMBL_OM;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SWCF", NC_FLOAT, 2, dimids, &SWCF);
    NC4ERR
    err = nc_put_att_text (ncid, SWCF, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, SWCF, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, SWCF, "long_name", 23, "Shortwave cloud forcing");
    NC4ERR
    err = nc_put_att_text (ncid, SWCF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SWCF;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "T", NC_FLOAT, 3, dimids, &T);
    NC4ERR
    err = nc_put_att_int (ncid, T, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, T, "units", 1, "K");
    NC4ERR
    err = nc_put_att_text (ncid, T, "long_name", 11, "Temperature");
    NC4ERR
    err = nc_put_att_text (ncid, T, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = T;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TAUGWX", NC_FLOAT, 2, dimids, &TAUGWX);
    NC4ERR
    err = nc_put_att_text (ncid, TAUGWX, "units", 4, "N/m2");
    NC4ERR
    err = nc_put_att_text (ncid, TAUGWX, "long_name", 33, "Zonal gravity wave surface stress");
    NC4ERR
    err = nc_put_att_text (ncid, TAUGWX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TAUGWX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TAUGWY", NC_FLOAT, 2, dimids, &TAUGWY);
    NC4ERR
    err = nc_put_att_text (ncid, TAUGWY, "units", 4, "N/m2");
    NC4ERR
    err = nc_put_att_text (ncid, TAUGWY, "long_name", 38, "Meridional gravity wave surface stress");
    NC4ERR
    err = nc_put_att_text (ncid, TAUGWY, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TAUGWY;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TAUX", NC_FLOAT, 2, dimids, &TAUX);
    NC4ERR
    err = nc_put_att_text (ncid, TAUX, "units", 4, "N/m2");
    NC4ERR
    err = nc_put_att_text (ncid, TAUX, "long_name", 20, "Zonal surface stress");
    NC4ERR
    err = nc_put_att_text (ncid, TAUX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TAUX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TAUY", NC_FLOAT, 2, dimids, &TAUY);
    NC4ERR
    err = nc_put_att_text (ncid, TAUY, "units", 4, "N/m2");
    NC4ERR
    err = nc_put_att_text (ncid, TAUY, "long_name", 25, "Meridional surface stress");
    NC4ERR
    err = nc_put_att_text (ncid, TAUY, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TAUY;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TGCLDCWP", NC_FLOAT, 2, dimids, &TGCLDCWP);
    NC4ERR
    err = nc_put_att_text (ncid, TGCLDCWP, "units", 5, "kg/m2");
    NC4ERR
    err = nc_put_att_text (ncid, TGCLDCWP, "long_name", 48,
                           "Total grid-box cloud water path (liquid and ice)");
    NC4ERR
    err = nc_put_att_text (ncid, TGCLDCWP, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TGCLDCWP;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TGCLDIWP", NC_FLOAT, 2, dimids, &TGCLDIWP);
    NC4ERR
    err = nc_put_att_text (ncid, TGCLDIWP, "units", 5, "kg/m2");
    NC4ERR
    err = nc_put_att_text (ncid, TGCLDIWP, "long_name", 35, "Total grid-box cloud ice water path");
    NC4ERR
    err = nc_put_att_text (ncid, TGCLDIWP, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TGCLDIWP;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TGCLDLWP", NC_FLOAT, 2, dimids, &TGCLDLWP);
    NC4ERR
    err = nc_put_att_text (ncid, TGCLDLWP, "units", 5, "kg/m2");
    NC4ERR
    err =
        nc_put_att_text (ncid, TGCLDLWP, "long_name", 38, "Total grid-box cloud liquid water path");
    NC4ERR
    err = nc_put_att_text (ncid, TGCLDLWP, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TGCLDLWP;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TH7001000", NC_FLOAT, 2, dimids, &TH7001000);
    NC4ERR
    err = nc_put_att_text (ncid, TH7001000, "units", 1, "K");
    NC4ERR
    err = nc_put_att_text (ncid, TH7001000, "long_name", 33, "Theta difference 700 mb - 1000 mb");
    NC4ERR
    err = nc_put_att_text (ncid, TH7001000, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TH7001000;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TMQ", NC_FLOAT, 2, dimids, &TMQ);
    NC4ERR
    err = nc_put_att_text (ncid, TMQ, "units", 5, "kg/m2");
    NC4ERR
    err = nc_put_att_text (ncid, TMQ, "long_name", 48,
                           "Total (vertically integrated) precipitable water");
    NC4ERR
    err = nc_put_att_text (ncid, TMQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TMQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TREFHT", NC_FLOAT, 2, dimids, &TREFHT);
    NC4ERR
    err = nc_put_att_text (ncid, TREFHT, "units", 1, "K");
    NC4ERR
    err = nc_put_att_text (ncid, TREFHT, "long_name", 28, "Reference height temperature");
    NC4ERR
    err = nc_put_att_text (ncid, TREFHT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TREFHT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TROP_P", NC_FLOAT, 2, dimids, &TROP_P);
    NC4ERR
    err = nc_put_att_float (ncid, TROP_P, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, TROP_P, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, TROP_P, "units", 2, "Pa");
    NC4ERR
    err = nc_put_att_text (ncid, TROP_P, "long_name", 19, "Tropopause Pressure");
    NC4ERR
    err = nc_put_att_text (ncid, TROP_P, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TROP_P;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TROP_T", NC_FLOAT, 2, dimids, &TROP_T);
    NC4ERR
    err = nc_put_att_float (ncid, TROP_T, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = nc_put_att_float (ncid, TROP_T, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = nc_put_att_text (ncid, TROP_T, "units", 1, "K");
    NC4ERR
    err = nc_put_att_text (ncid, TROP_T, "long_name", 22, "Tropopause Temperature");
    NC4ERR
    err = nc_put_att_text (ncid, TROP_T, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TROP_T;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TS", NC_FLOAT, 2, dimids, &TS);
    NC4ERR
    err = nc_put_att_text (ncid, TS, "units", 1, "K");
    NC4ERR
    err = nc_put_att_text (ncid, TS, "long_name", 31, "Surface temperature (radiative)");
    NC4ERR
    err = nc_put_att_text (ncid, TS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TSMN", NC_FLOAT, 2, dimids, &TSMN);
    NC4ERR
    err = nc_put_att_text (ncid, TSMN, "units", 1, "K");
    NC4ERR
    err = nc_put_att_text (ncid, TSMN, "long_name", 46,
                           "Minimum surface temperature over output period");
    NC4ERR
    err = nc_put_att_text (ncid, TSMN, "cell_methods", 13, "time: minimum");
    NC4ERR
    varids[i++] = TSMN;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TSMX", NC_FLOAT, 2, dimids, &TSMX);
    NC4ERR
    err = nc_put_att_text (ncid, TSMX, "units", 1, "K");
    NC4ERR
    err = nc_put_att_text (ncid, TSMX, "long_name", 46,
                           "Maximum surface temperature over output period");
    NC4ERR
    err = nc_put_att_text (ncid, TSMX, "cell_methods", 13, "time: maximum");
    NC4ERR
    varids[i++] = TSMX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TUH", NC_FLOAT, 2, dimids, &TUH);
    NC4ERR
    err = nc_put_att_text (ncid, TUH, "units", 3, "W/m");
    NC4ERR
    err = nc_put_att_text (ncid, TUH, "long_name", 44,
                           "Total (vertically integrated) zonal MSE flux");
    NC4ERR
    err = nc_put_att_text (ncid, TUH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TUH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TUQ", NC_FLOAT, 2, dimids, &TUQ);
    NC4ERR
    err = nc_put_att_text (ncid, TUQ, "units", 6, "kg/m/s");
    NC4ERR
    err = nc_put_att_text (ncid, TUQ, "long_name", 46,
                           "Total (vertically integrated) zonal water flux");
    NC4ERR
    err = nc_put_att_text (ncid, TUQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TUQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TVH", NC_FLOAT, 2, dimids, &TVH);
    NC4ERR
    err = nc_put_att_text (ncid, TVH, "units", 3, "W/m");
    NC4ERR
    err = nc_put_att_text (ncid, TVH, "long_name", 49,
                           "Total (vertically integrated) meridional MSE flux");
    NC4ERR
    err = nc_put_att_text (ncid, TVH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TVH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TVQ", NC_FLOAT, 2, dimids, &TVQ);
    NC4ERR
    err = nc_put_att_text (ncid, TVQ, "units", 6, "kg/m/s");
    NC4ERR
    err = nc_put_att_text (ncid, TVQ, "long_name", 51,
                           "Total (vertically integrated) meridional water flux");
    NC4ERR
    err = nc_put_att_text (ncid, TVQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TVQ;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "U", NC_FLOAT, 3, dimids, &U);
    NC4ERR
    err = nc_put_att_int (ncid, U, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, U, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, U, "long_name", 10, "Zonal wind");
    NC4ERR
    err = nc_put_att_text (ncid, U, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = U;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "U10", NC_FLOAT, 2, dimids, &U10);
    NC4ERR
    err = nc_put_att_text (ncid, U10, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, U10, "long_name", 14, "10m wind speed");
    NC4ERR
    err = nc_put_att_text (ncid, U10, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = U10;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "UU", NC_FLOAT, 3, dimids, &UU);
    NC4ERR
    err = nc_put_att_int (ncid, UU, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, UU, "units", 5, "m2/s2");
    NC4ERR
    err = nc_put_att_text (ncid, UU, "long_name", 22, "Zonal velocity squared");
    NC4ERR
    err = nc_put_att_text (ncid, UU, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = UU;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "V", NC_FLOAT, 3, dimids, &V);
    NC4ERR
    err = nc_put_att_int (ncid, V, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, V, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, V, "long_name", 15, "Meridional wind");
    NC4ERR
    err = nc_put_att_text (ncid, V, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = V;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "VQ", NC_FLOAT, 3, dimids, &VQ);
    NC4ERR
    err = nc_put_att_int (ncid, VQ, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, VQ, "units", 8, "m/skg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, VQ, "long_name", 26, "Meridional water transport");
    NC4ERR
    err = nc_put_att_text (ncid, VQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = VQ;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "VT", NC_FLOAT, 3, dimids, &VT);
    NC4ERR
    err = nc_put_att_int (ncid, VT, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, VT, "units", 5, "K m/s");
    NC4ERR
    err = nc_put_att_text (ncid, VT, "long_name", 25, "Meridional heat transport");
    NC4ERR
    err = nc_put_att_text (ncid, VT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = VT;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "VU", NC_FLOAT, 3, dimids, &VU);
    NC4ERR
    err = nc_put_att_int (ncid, VU, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, VU, "units", 5, "m2/s2");
    NC4ERR
    err = nc_put_att_text (ncid, VU, "long_name", 33, "Meridional flux of zonal momentum");
    NC4ERR
    err = nc_put_att_text (ncid, VU, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = VU;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "VV", NC_FLOAT, 3, dimids, &VV);
    NC4ERR
    err = nc_put_att_int (ncid, VV, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, VV, "units", 5, "m2/s2");
    NC4ERR
    err = nc_put_att_text (ncid, VV, "long_name", 27, "Meridional velocity squared");
    NC4ERR
    err = nc_put_att_text (ncid, VV, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = VV;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "WD_H2O2", NC_FLOAT, 2, dimids, &WD_H2O2);
    NC4ERR
    err = nc_put_att_text (ncid, WD_H2O2, "units", 4, "kg/s");
    NC4ERR
    err = nc_put_att_text (ncid, WD_H2O2, "long_name", 31, "H2O2             wet deposition");
    NC4ERR
    err = nc_put_att_text (ncid, WD_H2O2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = WD_H2O2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "WD_H2SO4", NC_FLOAT, 2, dimids, &WD_H2SO4);
    NC4ERR
    err = nc_put_att_text (ncid, WD_H2SO4, "units", 4, "kg/s");
    NC4ERR
    err = nc_put_att_text (ncid, WD_H2SO4, "long_name", 31, "H2SO4            wet deposition");
    NC4ERR
    err = nc_put_att_text (ncid, WD_H2SO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = WD_H2SO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "WD_SO2", NC_FLOAT, 2, dimids, &WD_SO2);
    NC4ERR
    err = nc_put_att_text (ncid, WD_SO2, "units", 4, "kg/s");
    NC4ERR
    err = nc_put_att_text (ncid, WD_SO2, "long_name", 31, "SO2              wet deposition");
    NC4ERR
    err = nc_put_att_text (ncid, WD_SO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = WD_SO2;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "WSUB", NC_FLOAT, 3, dimids, &WSUB);
    NC4ERR
    err = nc_put_att_int (ncid, WSUB, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, WSUB, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, WSUB, "long_name", 37, "Diagnostic sub-grid vertical velocity");
    NC4ERR
    err = nc_put_att_text (ncid, WSUB, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = WSUB;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Z3", NC_FLOAT, 3, dimids, &Z3);
    NC4ERR
    err = nc_put_att_int (ncid, Z3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, Z3, "units", 1, "m");
    NC4ERR
    err = nc_put_att_text (ncid, Z3, "long_name", 37, "Geopotential Height (above sea level)");
    NC4ERR
    err = nc_put_att_text (ncid, Z3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Z3;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "aero_water", NC_FLOAT, 3, dimids, &aero_water);
    NC4ERR
    err = nc_put_att_int (ncid, aero_water, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, aero_water, "units", 1, "m");
    NC4ERR
    err =
        nc_put_att_text (ncid, aero_water, "long_name", 70,
                         "sum of aerosol water of interstitial modes wat_a1+wat_a2+wat_a3+wat_a4");
    NC4ERR
    err = nc_put_att_text (ncid, aero_water, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = aero_water;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "airFV", NC_FLOAT, 2, dimids, &airFV);
    NC4ERR
    err = nc_put_att_text (ncid, airFV, "units", 4, "frac");
    NC4ERR
    err = nc_put_att_text (ncid, airFV, "long_name", 2, "FV");
    NC4ERR
    err = nc_put_att_text (ncid, airFV, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = airFV;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a1DDF", NC_FLOAT, 2, dimids, &bc_a1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1DDF, "long_name", 49,
                           "bc_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a1SFWET", NC_FLOAT, 2, dimids, &bc_a1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a1_SRF", NC_FLOAT, 2, dimids, &bc_a1_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1_SRF, "long_name", 21, "bc_a1 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a1_sfgaex1", NC_FLOAT, 2, dimids, &bc_a1_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1_sfgaex1, "long_name", 50,
                           "bc_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a3DDF", NC_FLOAT, 2, dimids, &bc_a3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a3DDF, "long_name", 49,
                           "bc_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a3SFWET", NC_FLOAT, 2, dimids, &bc_a3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a3_SRF", NC_FLOAT, 2, dimids, &bc_a3_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a3_SRF, "long_name", 21, "bc_a3 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a4DDF", NC_FLOAT, 2, dimids, &bc_a4DDF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4DDF, "long_name", 49,
                           "bc_a4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a4SFWET", NC_FLOAT, 2, dimids, &bc_a4SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a4_CLXF", NC_FLOAT, 2, dimids, &bc_a4_CLXF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4_CLXF, "long_name", 49,
                           "vertically intergrated external forcing for bc_a4");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a4_SRF", NC_FLOAT, 2, dimids, &bc_a4_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4_SRF, "long_name", 21, "bc_a4 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_a4_sfgaex1", NC_FLOAT, 2, dimids, &bc_a4_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4_sfgaex1, "long_name", 50,
                           "bc_a4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, bc_a4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_c1DDF", NC_FLOAT, 2, dimids, &bc_c1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_c1DDF, "long_name", 49,
                           "bc_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, bc_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_c1SFWET", NC_FLOAT, 2, dimids, &bc_c1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, bc_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        nc_put_att_text (ncid, bc_c1SFWET, "long_name", 36, "bc_c1 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, bc_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_c3DDF", NC_FLOAT, 2, dimids, &bc_c3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_c3DDF, "long_name", 49,
                           "bc_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, bc_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_c3SFWET", NC_FLOAT, 2, dimids, &bc_c3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, bc_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        nc_put_att_text (ncid, bc_c3SFWET, "long_name", 36, "bc_c3 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, bc_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_c4DDF", NC_FLOAT, 2, dimids, &bc_c4DDF);
    NC4ERR
    err = nc_put_att_text (ncid, bc_c4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, bc_c4DDF, "long_name", 49,
                           "bc_c4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, bc_c4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "bc_c4SFWET", NC_FLOAT, 2, dimids, &bc_c4SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, bc_c4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        nc_put_att_text (ncid, bc_c4SFWET, "long_name", 36, "bc_c4 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, bc_c4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "chla", NC_FLOAT, 2, dimids, &chla);
    NC4ERR
    err = nc_put_att_text (ncid, chla, "units", 6, "mg L-1");
    NC4ERR
    err = nc_put_att_text (ncid, chla, "long_name", 22, "ocean input data: chla");
    NC4ERR
    err = nc_put_att_text (ncid, chla, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = chla;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_a1DDF", NC_FLOAT, 2, dimids, &dst_a1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1DDF, "long_name", 50,
                           "dst_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_a1SF", NC_FLOAT, 2, dimids, &dst_a1SF);
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1SF, "long_name", 28, "dst_a1 dust surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a1SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_a1SFWET", NC_FLOAT, 2, dimids, &dst_a1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_a1_SRF", NC_FLOAT, 2, dimids, &dst_a1_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1_SRF, "long_name", 22, "dst_a1 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_a3DDF", NC_FLOAT, 2, dimids, &dst_a3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3DDF, "long_name", 50,
                           "dst_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_a3SF", NC_FLOAT, 2, dimids, &dst_a3SF);
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3SF, "long_name", 28, "dst_a3 dust surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a3SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_a3SFWET", NC_FLOAT, 2, dimids, &dst_a3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_a3_SRF", NC_FLOAT, 2, dimids, &dst_a3_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3_SRF, "long_name", 22, "dst_a3 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, dst_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_c1DDF", NC_FLOAT, 2, dimids, &dst_c1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, dst_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_c1DDF, "long_name", 50,
                           "dst_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, dst_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_c1SFWET", NC_FLOAT, 2, dimids, &dst_c1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, dst_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_c1SFWET, "long_name", 37,
                           "dst_c1 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, dst_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_c3DDF", NC_FLOAT, 2, dimids, &dst_c3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, dst_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_c3DDF, "long_name", 50,
                           "dst_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, dst_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "dst_c3SFWET", NC_FLOAT, 2, dimids, &dst_c3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, dst_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, dst_c3SFWET, "long_name", 37,
                           "dst_c3 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, dst_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "hstobie_linoz", NC_FLOAT, 3, dimids, &hstobie_linoz);
    NC4ERR
    err = nc_put_att_int (ncid, hstobie_linoz, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, hstobie_linoz, "units", 22, "fraction of model time");
    NC4ERR
    err = nc_put_att_text (ncid, hstobie_linoz, "long_name", 27, "Lowest possible Linoz level");
    NC4ERR
    varids[i++] = hstobie_linoz;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mlip", NC_FLOAT, 2, dimids, &mlip);
    NC4ERR
    err = nc_put_att_text (ncid, mlip, "units", 4, "uM C");
    NC4ERR
    err = nc_put_att_text (ncid, mlip, "long_name", 22, "ocean input data: mlip");
    NC4ERR
    err = nc_put_att_text (ncid, mlip, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mlip;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a1DDF", NC_FLOAT, 2, dimids, &mom_a1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1DDF, "long_name", 50,
                           "mom_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a1SF", NC_FLOAT, 2, dimids, &mom_a1SF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1SF, "long_name", 31, "mom_a1 seasalt surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a1SFWET", NC_FLOAT, 2, dimids, &mom_a1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a1_SRF", NC_FLOAT, 2, dimids, &mom_a1_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1_SRF, "long_name", 22, "mom_a1 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a1_sfgaex1", NC_FLOAT, 2, dimids, &mom_a1_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1_sfgaex1, "long_name", 51,
                           "mom_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a2DDF", NC_FLOAT, 2, dimids, &mom_a2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2DDF, "long_name", 50,
                           "mom_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a2SF", NC_FLOAT, 2, dimids, &mom_a2SF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2SF, "long_name", 31, "mom_a2 seasalt surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a2SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a2SFWET", NC_FLOAT, 2, dimids, &mom_a2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a2_SRF", NC_FLOAT, 2, dimids, &mom_a2_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2_SRF, "long_name", 22, "mom_a2 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a3DDF", NC_FLOAT, 2, dimids, &mom_a3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a3DDF, "long_name", 50,
                           "mom_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a3SFWET", NC_FLOAT, 2, dimids, &mom_a3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a3_SRF", NC_FLOAT, 2, dimids, &mom_a3_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a3_SRF, "long_name", 22, "mom_a3 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a4DDF", NC_FLOAT, 2, dimids, &mom_a4DDF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4DDF, "long_name", 50,
                           "mom_a4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a4SF", NC_FLOAT, 2, dimids, &mom_a4SF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4SF, "long_name", 31, "mom_a4 seasalt surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a4SFWET", NC_FLOAT, 2, dimids, &mom_a4SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a4_SRF", NC_FLOAT, 2, dimids, &mom_a4_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4_SRF, "long_name", 22, "mom_a4 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_a4_sfgaex1", NC_FLOAT, 2, dimids, &mom_a4_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4_sfgaex1, "long_name", 51,
                           "mom_a4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, mom_a4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_c1DDF", NC_FLOAT, 2, dimids, &mom_c1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c1DDF, "long_name", 50,
                           "mom_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_c1SFWET", NC_FLOAT, 2, dimids, &mom_c1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, mom_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c1SFWET, "long_name", 37,
                           "mom_c1 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_c2DDF", NC_FLOAT, 2, dimids, &mom_c2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_c2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c2DDF, "long_name", 50,
                           "mom_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_c2SFWET", NC_FLOAT, 2, dimids, &mom_c2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, mom_c2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c2SFWET, "long_name", 37,
                           "mom_c2 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_c3DDF", NC_FLOAT, 2, dimids, &mom_c3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c3DDF, "long_name", 50,
                           "mom_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_c3SFWET", NC_FLOAT, 2, dimids, &mom_c3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, mom_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c3SFWET, "long_name", 37,
                           "mom_c3 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_c4DDF", NC_FLOAT, 2, dimids, &mom_c4DDF);
    NC4ERR
    err = nc_put_att_text (ncid, mom_c4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c4DDF, "long_name", 50,
                           "mom_c4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mom_c4SFWET", NC_FLOAT, 2, dimids, &mom_c4SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, mom_c4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c4SFWET, "long_name", 37,
                           "mom_c4 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, mom_c4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mpoly", NC_FLOAT, 2, dimids, &mpoly);
    NC4ERR
    err = nc_put_att_text (ncid, mpoly, "units", 4, "uM C");
    NC4ERR
    err = nc_put_att_text (ncid, mpoly, "long_name", 23, "ocean input data: mpoly");
    NC4ERR
    err = nc_put_att_text (ncid, mpoly, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mpoly;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "mprot", NC_FLOAT, 2, dimids, &mprot);
    NC4ERR
    err = nc_put_att_text (ncid, mprot, "units", 4, "uM C");
    NC4ERR
    err = nc_put_att_text (ncid, mprot, "long_name", 23, "ocean input data: mprot");
    NC4ERR
    err = nc_put_att_text (ncid, mprot, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mprot;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a1DDF", NC_FLOAT, 2, dimids, &ncl_a1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1DDF, "long_name", 50,
                           "ncl_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a1SF", NC_FLOAT, 2, dimids, &ncl_a1SF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1SF, "long_name", 31, "ncl_a1 seasalt surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a1SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a1SFWET", NC_FLOAT, 2, dimids, &ncl_a1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a1_SRF", NC_FLOAT, 2, dimids, &ncl_a1_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1_SRF, "long_name", 22, "ncl_a1 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a2DDF", NC_FLOAT, 2, dimids, &ncl_a2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2DDF, "long_name", 50,
                           "ncl_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a2SF", NC_FLOAT, 2, dimids, &ncl_a2SF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2SF, "long_name", 31, "ncl_a2 seasalt surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a2SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a2SFWET", NC_FLOAT, 2, dimids, &ncl_a2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a2_SRF", NC_FLOAT, 2, dimids, &ncl_a2_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2_SRF, "long_name", 22, "ncl_a2 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a3DDF", NC_FLOAT, 2, dimids, &ncl_a3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3DDF, "long_name", 50,
                           "ncl_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a3SF", NC_FLOAT, 2, dimids, &ncl_a3SF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3SF, "long_name", 31, "ncl_a3 seasalt surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a3SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a3SFWET", NC_FLOAT, 2, dimids, &ncl_a3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_a3_SRF", NC_FLOAT, 2, dimids, &ncl_a3_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3_SRF, "long_name", 22, "ncl_a3 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_c1DDF", NC_FLOAT, 2, dimids, &ncl_c1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c1DDF, "long_name", 50,
                           "ncl_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_c1SFWET", NC_FLOAT, 2, dimids, &ncl_c1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c1SFWET, "long_name", 37,
                           "ncl_c1 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_c2DDF", NC_FLOAT, 2, dimids, &ncl_c2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c2DDF, "long_name", 50,
                           "ncl_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_c2SFWET", NC_FLOAT, 2, dimids, &ncl_c2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c2SFWET, "long_name", 37,
                           "ncl_c2 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_c3DDF", NC_FLOAT, 2, dimids, &ncl_c3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c3DDF, "long_name", 50,
                           "ncl_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "ncl_c3SFWET", NC_FLOAT, 2, dimids, &ncl_c3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c3SFWET, "long_name", 37,
                           "ncl_c3 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, ncl_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a1DDF", NC_FLOAT, 2, dimids, &num_a1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a1DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1DDF, "long_name", 50,
                           "num_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a1SF", NC_FLOAT, 2, dimids, &num_a1SF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a1SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1SF, "long_name", 28, "num_a1 dust surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a1SFWET", NC_FLOAT, 2, dimids, &num_a1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, num_a1SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a1_CLXF", NC_FLOAT, 2, dimids, &num_a1_CLXF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a1_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1_CLXF, "long_name", 50,
                           "vertically intergrated external forcing for num_a1");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a1_SRF", NC_FLOAT, 2, dimids, &num_a1_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a1_SRF, "units", 5, " 1/kg");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1_SRF, "long_name", 22, "num_a1 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a1_sfgaex1", NC_FLOAT, 2, dimids, &num_a1_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, num_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1_sfgaex1, "long_name", 51,
                           "num_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, num_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a2DDF", NC_FLOAT, 2, dimids, &num_a2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a2DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a2DDF, "long_name", 50,
                           "num_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, num_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a2SFWET", NC_FLOAT, 2, dimids, &num_a2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, num_a2SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, num_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a2_CLXF", NC_FLOAT, 2, dimids, &num_a2_CLXF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a2_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a2_CLXF, "long_name", 50,
                           "vertically intergrated external forcing for num_a2");
    NC4ERR
    err = nc_put_att_text (ncid, num_a2_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a2_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a2_SRF", NC_FLOAT, 2, dimids, &num_a2_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a2_SRF, "units", 5, " 1/kg");
    NC4ERR
    err = nc_put_att_text (ncid, num_a2_SRF, "long_name", 22, "num_a2 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, num_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a3DDF", NC_FLOAT, 2, dimids, &num_a3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a3DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a3DDF, "long_name", 50,
                           "num_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, num_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a3SF", NC_FLOAT, 2, dimids, &num_a3SF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a3SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a3SF, "long_name", 28, "num_a3 dust surface emission");
    NC4ERR
    err = nc_put_att_text (ncid, num_a3SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a3SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a3SFWET", NC_FLOAT, 2, dimids, &num_a3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, num_a3SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, num_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a3_SRF", NC_FLOAT, 2, dimids, &num_a3_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a3_SRF, "units", 5, " 1/kg");
    NC4ERR
    err = nc_put_att_text (ncid, num_a3_SRF, "long_name", 22, "num_a3 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, num_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a4DDF", NC_FLOAT, 2, dimids, &num_a4DDF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a4DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4DDF, "long_name", 50,
                           "num_a4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a4SFWET", NC_FLOAT, 2, dimids, &num_a4SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, num_a4SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a4_CLXF", NC_FLOAT, 2, dimids, &num_a4_CLXF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a4_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4_CLXF, "long_name", 50,
                           "vertically intergrated external forcing for num_a4");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a4_SRF", NC_FLOAT, 2, dimids, &num_a4_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, num_a4_SRF, "units", 5, " 1/kg");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4_SRF, "long_name", 22, "num_a4 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_a4_sfgaex1", NC_FLOAT, 2, dimids, &num_a4_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, num_a4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4_sfgaex1, "long_name", 51,
                           "num_a4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, num_a4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_c1DDF", NC_FLOAT, 2, dimids, &num_c1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, num_c1DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_c1DDF, "long_name", 50,
                           "num_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, num_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_c1SFWET", NC_FLOAT, 2, dimids, &num_c1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, num_c1SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_c1SFWET, "long_name", 37,
                           "num_c1 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, num_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_c2DDF", NC_FLOAT, 2, dimids, &num_c2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, num_c2DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_c2DDF, "long_name", 50,
                           "num_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, num_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_c2SFWET", NC_FLOAT, 2, dimids, &num_c2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, num_c2SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_c2SFWET, "long_name", 37,
                           "num_c2 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, num_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_c3DDF", NC_FLOAT, 2, dimids, &num_c3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, num_c3DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_c3DDF, "long_name", 50,
                           "num_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, num_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_c3SFWET", NC_FLOAT, 2, dimids, &num_c3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, num_c3SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_c3SFWET, "long_name", 37,
                           "num_c3 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, num_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_c4DDF", NC_FLOAT, 2, dimids, &num_c4DDF);
    NC4ERR
    err = nc_put_att_text (ncid, num_c4DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_c4DDF, "long_name", 50,
                           "num_c4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, num_c4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "num_c4SFWET", NC_FLOAT, 2, dimids, &num_c4SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, num_c4SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, num_c4SFWET, "long_name", 37,
                           "num_c4 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, num_c4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a1DDF", NC_FLOAT, 2, dimids, &pom_a1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1DDF, "long_name", 50,
                           "pom_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a1SFWET", NC_FLOAT, 2, dimids, &pom_a1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a1_SRF", NC_FLOAT, 2, dimids, &pom_a1_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1_SRF, "long_name", 22, "pom_a1 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a1_sfgaex1", NC_FLOAT, 2, dimids, &pom_a1_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1_sfgaex1, "long_name", 51,
                           "pom_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a3DDF", NC_FLOAT, 2, dimids, &pom_a3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a3DDF, "long_name", 50,
                           "pom_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a3SFWET", NC_FLOAT, 2, dimids, &pom_a3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a3_SRF", NC_FLOAT, 2, dimids, &pom_a3_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a3_SRF, "long_name", 22, "pom_a3 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a4DDF", NC_FLOAT, 2, dimids, &pom_a4DDF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4DDF, "long_name", 50,
                           "pom_a4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a4SFWET", NC_FLOAT, 2, dimids, &pom_a4SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a4_CLXF", NC_FLOAT, 2, dimids, &pom_a4_CLXF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4_CLXF, "long_name", 50,
                           "vertically intergrated external forcing for pom_a4");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a4_SRF", NC_FLOAT, 2, dimids, &pom_a4_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4_SRF, "long_name", 22, "pom_a4 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_a4_sfgaex1", NC_FLOAT, 2, dimids, &pom_a4_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4_sfgaex1, "long_name", 51,
                           "pom_a4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, pom_a4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_c1DDF", NC_FLOAT, 2, dimids, &pom_c1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c1DDF, "long_name", 50,
                           "pom_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_c1SFWET", NC_FLOAT, 2, dimids, &pom_c1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, pom_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c1SFWET, "long_name", 37,
                           "pom_c1 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_c3DDF", NC_FLOAT, 2, dimids, &pom_c3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c3DDF, "long_name", 50,
                           "pom_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_c3SFWET", NC_FLOAT, 2, dimids, &pom_c3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, pom_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c3SFWET, "long_name", 37,
                           "pom_c3 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_c4DDF", NC_FLOAT, 2, dimids, &pom_c4DDF);
    NC4ERR
    err = nc_put_att_text (ncid, pom_c4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c4DDF, "long_name", 50,
                           "pom_c4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "pom_c4SFWET", NC_FLOAT, 2, dimids, &pom_c4SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, pom_c4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c4SFWET, "long_name", 37,
                           "pom_c4 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, pom_c4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a1DDF", NC_FLOAT, 2, dimids, &so4_a1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1DDF, "long_name", 50,
                           "so4_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a1SFWET", NC_FLOAT, 2, dimids, &so4_a1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a1_CLXF", NC_FLOAT, 2, dimids, &so4_a1_CLXF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1_CLXF, "long_name", 50,
                           "vertically intergrated external forcing for so4_a1");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a1_SRF", NC_FLOAT, 2, dimids, &so4_a1_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1_SRF, "long_name", 22, "so4_a1 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a1_sfgaex1", NC_FLOAT, 2, dimids, &so4_a1_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1_sfgaex1, "long_name", 51,
                           "so4_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a2DDF", NC_FLOAT, 2, dimids, &so4_a2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2DDF, "long_name", 50,
                           "so4_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a2SFWET", NC_FLOAT, 2, dimids, &so4_a2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a2_CLXF", NC_FLOAT, 2, dimids, &so4_a2_CLXF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2_CLXF, "long_name", 50,
                           "vertically intergrated external forcing for so4_a2");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a2_SRF", NC_FLOAT, 2, dimids, &so4_a2_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2_SRF, "long_name", 22, "so4_a2 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a2_sfgaex1", NC_FLOAT, 2, dimids, &so4_a2_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2_sfgaex1, "long_name", 51,
                           "so4_a2 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a2_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a3DDF", NC_FLOAT, 2, dimids, &so4_a3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3DDF, "long_name", 50,
                           "so4_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a3SFWET", NC_FLOAT, 2, dimids, &so4_a3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a3_SRF", NC_FLOAT, 2, dimids, &so4_a3_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3_SRF, "long_name", 22, "so4_a3 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_a3_sfgaex1", NC_FLOAT, 2, dimids, &so4_a3_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3_sfgaex1, "long_name", 51,
                           "so4_a3 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, so4_a3_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a3_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_c1DDF", NC_FLOAT, 2, dimids, &so4_c1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c1DDF, "long_name", 50,
                           "so4_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_c1SFWET", NC_FLOAT, 2, dimids, &so4_c1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, so4_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c1SFWET, "long_name", 37,
                           "so4_c1 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_c2DDF", NC_FLOAT, 2, dimids, &so4_c2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_c2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c2DDF, "long_name", 50,
                           "so4_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_c2SFWET", NC_FLOAT, 2, dimids, &so4_c2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, so4_c2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c2SFWET, "long_name", 37,
                           "so4_c2 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_c3DDF", NC_FLOAT, 2, dimids, &so4_c3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, so4_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c3DDF, "long_name", 50,
                           "so4_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "so4_c3SFWET", NC_FLOAT, 2, dimids, &so4_c3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, so4_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c3SFWET, "long_name", 37,
                           "so4_c3 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, so4_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a1DDF", NC_FLOAT, 2, dimids, &soa_a1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1DDF, "long_name", 50,
                           "soa_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a1SFWET", NC_FLOAT, 2, dimids, &soa_a1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a1_SRF", NC_FLOAT, 2, dimids, &soa_a1_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1_SRF, "long_name", 22, "soa_a1 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a1_sfgaex1", NC_FLOAT, 2, dimids, &soa_a1_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1_sfgaex1, "long_name", 51,
                           "soa_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a2DDF", NC_FLOAT, 2, dimids, &soa_a2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2DDF, "long_name", 50,
                           "soa_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a2SFWET", NC_FLOAT, 2, dimids, &soa_a2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a2_SRF", NC_FLOAT, 2, dimids, &soa_a2_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2_SRF, "long_name", 22, "soa_a2 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a2_sfgaex1", NC_FLOAT, 2, dimids, &soa_a2_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2_sfgaex1, "long_name", 51,
                           "soa_a2 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a2_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a2_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a3DDF", NC_FLOAT, 2, dimids, &soa_a3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3DDF, "long_name", 50,
                           "soa_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a3SFWET", NC_FLOAT, 2, dimids, &soa_a3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a3_SRF", NC_FLOAT, 2, dimids, &soa_a3_SRF);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3_SRF, "long_name", 22, "soa_a3 in bottom layer");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_a3_sfgaex1", NC_FLOAT, 2, dimids, &soa_a3_sfgaex1);
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3_sfgaex1, "long_name", 51,
                           "soa_a3 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = nc_put_att_text (ncid, soa_a3_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a3_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_c1DDF", NC_FLOAT, 2, dimids, &soa_c1DDF);
    NC4ERR
    err = nc_put_att_text (ncid, soa_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c1DDF, "long_name", 50,
                           "soa_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_c1SFWET", NC_FLOAT, 2, dimids, &soa_c1SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, soa_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c1SFWET, "long_name", 37,
                           "soa_c1 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_c2DDF", NC_FLOAT, 2, dimids, &soa_c2DDF);
    NC4ERR
    err = nc_put_att_text (ncid, soa_c2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c2DDF, "long_name", 50,
                           "soa_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_c2SFWET", NC_FLOAT, 2, dimids, &soa_c2SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, soa_c2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c2SFWET, "long_name", 37,
                           "soa_c2 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_c3DDF", NC_FLOAT, 2, dimids, &soa_c3DDF);
    NC4ERR
    err = nc_put_att_text (ncid, soa_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c3DDF, "long_name", 50,
                           "soa_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "soa_c3SFWET", NC_FLOAT, 2, dimids, &soa_c3SFWET);
    NC4ERR
    err = nc_put_att_text (ncid, soa_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c3SFWET, "long_name", 37,
                           "soa_c3 wet deposition flux at surface");
    NC4ERR
    err = nc_put_att_text (ncid, soa_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c3SFWET;

    assert (i == nvars);

fn_exit:
    return nerrs;
}

/*----< inq_F_case_h0() >----------------------------------------------------*/
int inq_F_case_h0_nc4 (int ncid,       /* file ID */
                       size_t dims[2], /* dimension sizes */
                       int nvars,      /* number of variables */
                       int *varids)    /* variable IDs */
{
    /* Total 408 variables */
    int lat, lon, area, lev, hyam, hybm, P0, ilev, hyai, hybi, time, date, datesec, time_bnds,
        date_written, time_written, ndbase, nsbase, nbdate, nbsec, mdt, ndcur, nscur, co2vmr,
        ch4vmr, n2ovmr, f11vmr, f12vmr, sol_tsi, nsteph, AEROD_v, ANRAIN, ANSNOW, AODABS, AODABSBC,
        AODBC, AODDUST, AODDUST1, AODDUST3, AODDUST4, AODMODE1, AODMODE2, AODMODE3, AODMODE4,
        AODNIR, AODPOM, AODSO4, AODSOA, AODSS, AODUV, AODVIS, AQRAIN, AQSNOW, AQ_DMS, AQ_H2O2,
        AQ_H2SO4, AQ_O3, AQ_SO2, AQ_SOAG, AREI, AREL, AWNC, AWNI, CCN3, CDNUMC, CLDHGH, CLDICE,
        CLDLIQ, CLDLOW, CLDMED, CLDTOT, CLOUD, CLOUDFRAC_CLUBB, CONCLD, DCQ, DF_DMS, DF_H2O2,
        DF_H2SO4, DF_O3, DF_SO2, DF_SOAG, DMS_SRF, DP_KCLDBASE, DP_MFUP_MAX, DP_WCLDBASE, DSTSFMBL,
        DTCOND, DTENDTH, DTENDTQ, EXTINCT, FICE, FLDS, FLNS, FLNSC, FLNT, FLNTC, FLUT, FLUTC, FREQI,
        FREQL, FREQR, FREQS, FSDS, FSDSC, FSNS, FSNSC, FSNT, FSNTC, FSNTOA, FSNTOAC, FSUTOA,
        FSUTOAC, F_eff, H2O2_SRF, H2SO4_SRF, H2SO4_sfgaex1, ICEFRAC, ICIMR, ICWMR, IWC, LANDFRAC,
        LHFLX, LINOZ_DO3, LINOZ_DO3_PSC, LINOZ_O3CLIM, LINOZ_O3COL, LINOZ_SSO3, LINOZ_SZA, LND_MBL,
        LWCF, Mass_bc, Mass_dst, Mass_mom, Mass_ncl, Mass_pom, Mass_so4, Mass_soa, NUMICE, NUMLIQ,
        NUMRAI, NUMSNO, O3, O3_SRF, OCNFRAC, OMEGA, OMEGA500, OMEGAT, PBLH, PHIS, PRECC, PRECL,
        PRECSC, PRECSL, PS, PSL, Q, QFLX, QREFHT, QRL, QRS, RAINQM, RAM1, RELHUM, SFDMS, SFH2O2,
        SFH2SO4, SFO3, SFSO2, SFSOAG, SFbc_a1, SFbc_a3, SFbc_a4, SFdst_a1, SFdst_a3, SFmom_a1,
        SFmom_a2, SFmom_a3, SFmom_a4, SFncl_a1, SFncl_a2, SFncl_a3, SFnum_a1, SFnum_a2, SFnum_a3,
        SFnum_a4, SFpom_a1, SFpom_a3, SFpom_a4, SFso4_a1, SFso4_a2, SFso4_a3, SFsoa_a1, SFsoa_a2,
        SFsoa_a3, SHFLX, SH_KCLDBASE, SH_MFUP_MAX, SH_WCLDBASE, SNOWHICE, SNOWHLND, SNOWQM, SO2,
        SO2_CLXF, SO2_SRF, SOAG_CLXF, SOAG_SRF, SOAG_sfgaex1, SOLIN, SSAVIS, SSTSFMBL, SSTSFMBL_OM,
        SWCF, T, TAUGWX, TAUGWY, TAUX, TAUY, TGCLDCWP, TGCLDIWP, TGCLDLWP, TH7001000, TMQ, TREFHT,
        TROP_P, TROP_T, TS, TSMN, TSMX, TUH, TUQ, TVH, TVQ, U, U10, UU, V, VQ, VT, VU, VV, WD_H2O2,
        WD_H2SO4, WD_SO2, WSUB, Z3, aero_water, airFV, bc_a1DDF, bc_a1SFWET, bc_a1_SRF,
        bc_a1_sfgaex1, bc_a3DDF, bc_a3SFWET, bc_a3_SRF, bc_a4DDF, bc_a4SFWET, bc_a4_CLXF, bc_a4_SRF,
        bc_a4_sfgaex1, bc_c1DDF, bc_c1SFWET, bc_c3DDF, bc_c3SFWET, bc_c4DDF, bc_c4SFWET, chla,
        dst_a1DDF, dst_a1SF, dst_a1SFWET, dst_a1_SRF, dst_a3DDF, dst_a3SF, dst_a3SFWET, dst_a3_SRF,
        dst_c1DDF, dst_c1SFWET, dst_c3DDF, dst_c3SFWET, hstobie_linoz, mlip, mom_a1DDF, mom_a1SF,
        mom_a1SFWET, mom_a1_SRF, mom_a1_sfgaex1, mom_a2DDF, mom_a2SF, mom_a2SFWET, mom_a2_SRF,
        mom_a3DDF, mom_a3SFWET, mom_a3_SRF, mom_a4DDF, mom_a4SF, mom_a4SFWET, mom_a4_SRF,
        mom_a4_sfgaex1, mom_c1DDF, mom_c1SFWET, mom_c2DDF, mom_c2SFWET, mom_c3DDF, mom_c3SFWET,
        mom_c4DDF, mom_c4SFWET, mpoly, mprot, ncl_a1DDF, ncl_a1SF, ncl_a1SFWET, ncl_a1_SRF,
        ncl_a2DDF, ncl_a2SF, ncl_a2SFWET, ncl_a2_SRF, ncl_a3DDF, ncl_a3SF, ncl_a3SFWET, ncl_a3_SRF,
        ncl_c1DDF, ncl_c1SFWET, ncl_c2DDF, ncl_c2SFWET, ncl_c3DDF, ncl_c3SFWET, num_a1DDF, num_a1SF,
        num_a1SFWET, num_a1_CLXF, num_a1_SRF, num_a1_sfgaex1, num_a2DDF, num_a2SFWET, num_a2_CLXF,
        num_a2_SRF, num_a3DDF, num_a3SF, num_a3SFWET, num_a3_SRF, num_a4DDF, num_a4SFWET,
        num_a4_CLXF, num_a4_SRF, num_a4_sfgaex1, num_c1DDF, num_c1SFWET, num_c2DDF, num_c2SFWET,
        num_c3DDF, num_c3SFWET, num_c4DDF, num_c4SFWET, pom_a1DDF, pom_a1SFWET, pom_a1_SRF,
        pom_a1_sfgaex1, pom_a3DDF, pom_a3SFWET, pom_a3_SRF, pom_a4DDF, pom_a4SFWET, pom_a4_CLXF,
        pom_a4_SRF, pom_a4_sfgaex1, pom_c1DDF, pom_c1SFWET, pom_c3DDF, pom_c3SFWET, pom_c4DDF,
        pom_c4SFWET, so4_a1DDF, so4_a1SFWET, so4_a1_CLXF, so4_a1_SRF, so4_a1_sfgaex1, so4_a2DDF,
        so4_a2SFWET, so4_a2_CLXF, so4_a2_SRF, so4_a2_sfgaex1, so4_a3DDF, so4_a3SFWET, so4_a3_SRF,
        so4_a3_sfgaex1, so4_c1DDF, so4_c1SFWET, so4_c2DDF, so4_c2SFWET, so4_c3DDF, so4_c3SFWET,
        soa_a1DDF, soa_a1SFWET, soa_a1_SRF, soa_a1_sfgaex1, soa_a2DDF, soa_a2SFWET, soa_a2_SRF,
        soa_a2_sfgaex1, soa_a3DDF, soa_a3SFWET, soa_a3_SRF, soa_a3_sfgaex1, soa_c1DDF, soa_c1SFWET,
        soa_c2DDF, soa_c2SFWET, soa_c3DDF, soa_c3SFWET;

    int i, err, nerrs = 0, dimids[3], iattr, mdims = 1;
    int dim_ncol, dim_time, dim_nbnd, dim_chars, dim_lev, dim_ilev;
    float fillv = 1.e+36f, missv = 1.e+36f;

    /* global attributes: */
    iattr = 4;
    err   = NOP2 (ncid, NC_GLOBAL, "ne", NC_INT, 1, &iattr);
    NC4ERR
    err = NOP2 (ncid, NC_GLOBAL, "np", NC_INT, 1, &iattr);
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "Conventions", 6, "CF-1.0");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "source", 3, "CAM");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "case", 20, "FC5AV1C-H01B_ne4_ne4");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "title", 5, "UNSET");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "logname", 6, "wkliao");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "host", 10, "compute001");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "Version", 6, "$Name$");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "revision_Id", 4, "$Id$");
    NC4ERR
    err = NOP (
        ncid, NC_GLOBAL, "initial_file", 86,
        "/home/climate1/acme/inputdata/atm/cam/inic/homme/cami_mam3_Linoz_ne4np4_L72_c160909.nc");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "topography_file", 79,
               "/home/climate1/acme/inputdata/atm/cam/topo/USGS-gtopo30_ne4np4_16x.c20160612.nc");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "time_period_freq", 5, "day_5");
    NC4ERR

    /* inquery dimensions */
    err = nc_inq_dimid (ncid, "ncol", &dim_ncol);
    NC4ERR
    // err = nc_inq_dimid(ncid, "time", &dim_time); NC4ERR
    // err = nc_inq_dimid(ncid, "nbnd", &dim_nbnd); NC4ERR
    // err = nc_inq_dimid(ncid, "chars", &dim_chars); NC4ERR
    err = nc_inq_dimid (ncid, "lev", &dim_lev);
    NC4ERR
    // err = nc_inq_dimid(ncid, "ilev", &dim_ilev); NC4ERR

    err = nc_inq_dimlen (ncid, dim_ncol, dims + 1);
    NC4ERR
    err = nc_inq_dimlen (ncid, dim_lev, dims);
    NC4ERR
    /*
    err = nc_def_dim(ncid, "ncol", dims[1],      &dim_ncol); NC4ERR
    err = nc_def_dim(ncid, "time", NC_UNLIMITED, &dim_time); NC4ERR
    err = nc_def_dim(ncid, "nbnd",  2,           &dim_nbnd); NC4ERR
    err = nc_def_dim(ncid, "chars", 8,           &dim_chars); NC4ERR
    err = nc_def_dim(ncid, "lev",   dims[0],     &dim_lev); NC4ERR
    err = nc_def_dim(ncid, "ilev",  dims[0]+1,   &dim_ilev); NC4ERR
    */

    i = 0;

    /* define variables */
    dimids[0] = dim_ncol;
    err       = INQ_VID (ncid, "lat", NC_DOUBLE, 1, dimids, &lat);
    NC4ERR
    err = NOP (ncid, lat, "long_name", 8, "latitude");
    NC4ERR
    err = NOP (ncid, lat, "units", 13, "degrees_north");
    NC4ERR
    varids[i++] = lat;

    dimids[0] = dim_ncol;
    err       = INQ_VID (ncid, "lon", NC_DOUBLE, 1, dimids, &lon);
    NC4ERR
    err = NOP (ncid, lon, "long_name", 9, "longitude");
    NC4ERR
    err = NOP (ncid, lon, "units", 12, "degrees_east");
    NC4ERR
    varids[i++] = lon;

    dimids[0] = dim_ncol;
    err       = INQ_VID (ncid, "area", NC_DOUBLE, 1, dimids, &area);
    NC4ERR
    err = NOP (ncid, area, "long_name", 14, "gll grid areas");
    NC4ERR
    varids[i++] = area;

    dimids[0] = dim_lev;
    err       = INQ_VID (ncid, "lev", NC_DOUBLE, 1, dimids, &lev);
    NC4ERR
    err = NOP (ncid, lev, "long_name", 38, "hybrid level at midpoints (1000*(A+B))");
    NC4ERR
    err = NOP (ncid, lev, "units", 3, "hPa");
    NC4ERR
    err = NOP (ncid, lev, "positive", 4, "down");
    NC4ERR
    err = NOP (ncid, lev, "standard_name", 43, "atmosphere_hybrid_sigma_pressure_coordinate");
    NC4ERR
    err = NOP (ncid, lev, "formula_terms", 29, "a: hyam b: hybm p0: P0 ps: PS");
    NC4ERR
    varids[i++] = lev;

    dimids[0] = dim_lev;
    err       = INQ_VID (ncid, "hyam", NC_DOUBLE, 1, dimids, &hyam);
    NC4ERR
    err = NOP (ncid, hyam, "long_name", 39, "hybrid A coefficient at layer midpoints");
    NC4ERR
    varids[i++] = hyam;

    dimids[0] = dim_lev;
    err       = INQ_VID (ncid, "hybm", NC_DOUBLE, 1, dimids, &hybm);
    NC4ERR
    err = NOP (ncid, hybm, "long_name", 39, "hybrid B coefficient at layer midpoints");
    NC4ERR
    varids[i++] = hybm;

    dimids[0] = dim_lev;
    err       = INQ_VID (ncid, "P0", NC_DOUBLE, 0, NULL, &P0);
    NC4ERR
    err = NOP (ncid, P0, "long_name", 18, "reference pressure");
    NC4ERR
    err = NOP (ncid, P0, "units", 2, "Pa");
    NC4ERR
    varids[i++] = P0;

    dimids[0] = dim_ilev;
    err       = INQ_VID (ncid, "ilev", NC_DOUBLE, 1, dimids, &ilev);
    NC4ERR
    err = NOP (ncid, ilev, "long_name", 39, "hybrid level at interfaces (1000*(A+B))");
    NC4ERR
    err = NOP (ncid, ilev, "units", 3, "hPa");
    NC4ERR
    err = NOP (ncid, ilev, "positive", 4, "down");
    NC4ERR
    err = NOP (ncid, ilev, "standard_name", 43, "atmosphere_hybrid_sigma_pressure_coordinate");
    NC4ERR
    err = NOP (ncid, ilev, "formula_terms", 29, "a: hyai b: hybi p0: P0 ps: PS");
    NC4ERR
    varids[i++] = ilev;

    dimids[0] = dim_ilev;
    err       = INQ_VID (ncid, "hyai", NC_DOUBLE, 1, dimids, &hyai);
    NC4ERR
    err = NOP (ncid, hyai, "long_name", 40, "hybrid A coefficient at layer interfaces");
    NC4ERR
    varids[i++] = hyai;

    dimids[0] = dim_ilev;
    err       = INQ_VID (ncid, "hybi", NC_DOUBLE, 1, dimids, &hybi);
    NC4ERR
    err = NOP (ncid, hybi, "long_name", 40, "hybrid B coefficient at layer interfaces");
    NC4ERR
    varids[i++] = hybi;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "time", NC_DOUBLE, 1, dimids, &time);
    NC4ERR
    err = NOP (ncid, time, "long_name", 4, "time");
    NC4ERR
    err = NOP (ncid, time, "units", 30, "days since 0001-01-01 00:00:00");
    NC4ERR
    err = NOP (ncid, time, "calendar", 6, "noleap");
    NC4ERR
    err = NOP (ncid, time, "bounds", 9, "time_bnds");
    NC4ERR
    varids[i++] = time;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "date", NC_INT, 1, dimids, &date);
    NC4ERR
    err = NOP (ncid, date, "long_name", 23, "current date (YYYYMMDD)");
    NC4ERR
    varids[i++] = date;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "datesec", NC_INT, 1, dimids, &datesec);
    NC4ERR
    err = NOP (ncid, datesec, "long_name", 31, "current seconds of current date");
    NC4ERR
    varids[i++] = datesec;

    dimids[0] = dim_time;
    dimids[1] = dim_nbnd;
    err       = INQ_VID (ncid, "time_bnds", NC_DOUBLE, 2, dimids, &time_bnds);
    NC4ERR
    err = NOP (ncid, time_bnds, "long_name", 23, "time interval endpoints");
    NC4ERR
    varids[i++] = time_bnds;

    dimids[0] = dim_time;
    dimids[1] = dim_chars;
    err       = INQ_VID (ncid, "date_written", NC_CHAR, 2, dimids, &date_written);
    NC4ERR
    varids[i++] = date_written;

    dimids[0] = dim_time;
    dimids[1] = dim_chars;
    err       = INQ_VID (ncid, "time_written", NC_CHAR, 2, dimids, &time_written);
    NC4ERR
    varids[i++] = time_written;

    err = INQ_VID (ncid, "ndbase", NC_INT, 0, NULL, &ndbase);
    NC4ERR
    err = NOP (ncid, ndbase, "long_name", 8, "base day");
    NC4ERR
    varids[i++] = ndbase;
    err         = INQ_VID (ncid, "nsbase", NC_INT, 0, NULL, &nsbase);
    NC4ERR
    err = NOP (ncid, nsbase, "long_name", 19, "seconds of base day");
    NC4ERR
    varids[i++] = nsbase;

    err = INQ_VID (ncid, "nbdate", NC_INT, 0, NULL, &nbdate);
    NC4ERR
    err = NOP (ncid, nbdate, "long_name", 20, "base date (YYYYMMDD)");
    NC4ERR
    varids[i++] = nbdate;

    err = INQ_VID (ncid, "nbsec", NC_INT, 0, NULL, &nbsec);
    NC4ERR
    err = NOP (ncid, nbsec, "long_name", 20, "seconds of base date");
    NC4ERR
    varids[i++] = nbsec;

    err = INQ_VID (ncid, "mdt", NC_INT, 0, NULL, &mdt);
    NC4ERR
    err = NOP (ncid, mdt, "long_name", 8, "timestep");
    NC4ERR
    err = NOP (ncid, mdt, "units", 1, "s");
    NC4ERR
    varids[i++] = mdt;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "ndcur", NC_INT, 1, dimids, &ndcur);
    NC4ERR
    err = NOP (ncid, ndcur, "long_name", 27, "current day (from base day)");
    NC4ERR
    varids[i++] = ndcur;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "nscur", NC_INT, 1, dimids, &nscur);
    NC4ERR
    err = NOP (ncid, nscur, "long_name", 30, "current seconds of current day");
    NC4ERR
    varids[i++] = nscur;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "co2vmr", NC_DOUBLE, 1, dimids, &co2vmr);
    NC4ERR
    err = NOP (ncid, co2vmr, "long_name", 23, "co2 volume mixing ratio");
    NC4ERR
    varids[i++] = co2vmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "ch4vmr", NC_DOUBLE, 1, dimids, &ch4vmr);
    NC4ERR
    err = NOP (ncid, ch4vmr, "long_name", 23, "ch4 volume mixing ratio");
    NC4ERR
    varids[i++] = ch4vmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "n2ovmr", NC_DOUBLE, 1, dimids, &n2ovmr);
    NC4ERR
    err = NOP (ncid, n2ovmr, "long_name", 23, "n2o volume mixing ratio");
    NC4ERR
    varids[i++] = n2ovmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "f11vmr", NC_DOUBLE, 1, dimids, &f11vmr);
    NC4ERR
    err = NOP (ncid, f11vmr, "long_name", 23, "f11 volume mixing ratio");
    NC4ERR
    varids[i++] = f11vmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "f12vmr", NC_DOUBLE, 1, dimids, &f12vmr);
    NC4ERR
    err = NOP (ncid, f12vmr, "long_name", 23, "f12 volume mixing ratio");
    NC4ERR
    varids[i++] = f12vmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "sol_tsi", NC_DOUBLE, 1, dimids, &sol_tsi);
    NC4ERR
    err = NOP (ncid, sol_tsi, "long_name", 22, "total solar irradiance");
    NC4ERR
    err = NOP (ncid, sol_tsi, "units", 4, "W/m2");
    NC4ERR
    varids[i++] = sol_tsi;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "nsteph", NC_INT, 1, dimids, &nsteph);
    NC4ERR
    err = NOP (ncid, nsteph, "long_name", 16, "current timestep");
    NC4ERR
    varids[i++] = nsteph;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AEROD_v", NC_FLOAT, 2, dimids, &AEROD_v);
    NC4ERR
    err = NOP2 (ncid, AEROD_v, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AEROD_v, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AEROD_v, "units", 1, "1");
    NC4ERR
    err = NOP (ncid, AEROD_v, "long_name", 43, "Total Aerosol Optical Depth in visible band");
    NC4ERR
    err = NOP (ncid, AEROD_v, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AEROD_v;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "ANRAIN", NC_FLOAT, 3, dimids, &ANRAIN);
    NC4ERR
    err = NOP2 (ncid, ANRAIN, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, ANRAIN, "units", 3, "m-3");
    NC4ERR
    err = NOP (ncid, ANRAIN, "long_name", 24, "Average rain number conc");
    NC4ERR
    err = NOP (ncid, ANRAIN, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ANRAIN;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "ANSNOW", NC_FLOAT, 3, dimids, &ANSNOW);
    NC4ERR
    err = NOP2 (ncid, ANSNOW, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, ANSNOW, "units", 3, "m-3");
    NC4ERR
    err = NOP (ncid, ANSNOW, "long_name", 24, "Average snow number conc");
    NC4ERR
    err = NOP (ncid, ANSNOW, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ANSNOW;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODABS", NC_FLOAT, 2, dimids, &AODABS);
    NC4ERR
    err = NOP2 (ncid, AODABS, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODABS, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODABS, "long_name", 39, "Aerosol absorption optical depth 550 nm");
    NC4ERR
    err = NOP (ncid, AODABS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODABS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODABSBC", NC_FLOAT, 2, dimids, &AODABSBC);
    NC4ERR
    err = NOP2 (ncid, AODABSBC, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODABSBC, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODABSBC, "long_name", 48, "Aerosol absorption optical depth 550 nm from BC");
    NC4ERR
    err = NOP (ncid, AODABSBC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODABSBC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODBC", NC_FLOAT, 2, dimids, &AODBC);
    NC4ERR
    err = NOP2 (ncid, AODBC, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODBC, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODBC, "long_name", 37, "Aerosol optical depth 550 nm from BC");
    NC4ERR
    err = NOP (ncid, AODBC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODBC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODDUST", NC_FLOAT, 2, dimids, &AODDUST);
    NC4ERR
    err = NOP2 (ncid, AODDUST, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODDUST, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODDUST, "long_name", 38, "Aerosol optical depth 550 nm from dust");
    NC4ERR
    err = NOP (ncid, AODDUST, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODDUST;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODDUST1", NC_FLOAT, 2, dimids, &AODDUST1);
    NC4ERR
    err = NOP2 (ncid, AODDUST1, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODDUST1, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODDUST1, "long_name", 46, "Aerosol optical depth 550 nm model 1 from dust");
    NC4ERR
    err = NOP (ncid, AODDUST1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODDUST1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODDUST3", NC_FLOAT, 2, dimids, &AODDUST3);
    NC4ERR
    err = NOP2 (ncid, AODDUST3, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODDUST3, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODDUST3, "long_name", 46, "Aerosol optical depth 550 nm model 3 from dust");
    NC4ERR
    err = NOP (ncid, AODDUST3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODDUST3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODDUST4", NC_FLOAT, 2, dimids, &AODDUST4);
    NC4ERR
    err = NOP2 (ncid, AODDUST4, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODDUST4, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODDUST4, "long_name", 46, "Aerosol optical depth 550 nm model 4 from dust");
    NC4ERR
    err = NOP (ncid, AODDUST4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODDUST4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODMODE1", NC_FLOAT, 2, dimids, &AODMODE1);
    NC4ERR
    err = NOP2 (ncid, AODMODE1, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODMODE1, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODMODE1, "long_name", 35, "Aerosol optical depth 550 nm mode 1");
    NC4ERR
    err = NOP (ncid, AODMODE1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODMODE1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODMODE2", NC_FLOAT, 2, dimids, &AODMODE2);
    NC4ERR
    err = NOP2 (ncid, AODMODE2, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODMODE2, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODMODE2, "long_name", 35, "Aerosol optical depth 550 nm mode 2");
    NC4ERR
    err = NOP (ncid, AODMODE2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODMODE2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODMODE3", NC_FLOAT, 2, dimids, &AODMODE3);
    NC4ERR
    err = NOP2 (ncid, AODMODE3, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODMODE3, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODMODE3, "long_name", 35, "Aerosol optical depth 550 nm mode 3");
    NC4ERR
    err = NOP (ncid, AODMODE3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODMODE3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODMODE4", NC_FLOAT, 2, dimids, &AODMODE4);
    NC4ERR
    err = NOP2 (ncid, AODMODE4, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODMODE4, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODMODE4, "long_name", 35, "Aerosol optical depth 550 nm mode 4");
    NC4ERR
    err = NOP (ncid, AODMODE4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODMODE4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODNIR", NC_FLOAT, 2, dimids, &AODNIR);
    NC4ERR
    err = NOP2 (ncid, AODNIR, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODNIR, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODNIR, "long_name", 28, "Aerosol optical depth 850 nm");
    NC4ERR
    err = NOP (ncid, AODNIR, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODNIR;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODPOM", NC_FLOAT, 2, dimids, &AODPOM);
    NC4ERR
    err = NOP2 (ncid, AODPOM, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODPOM, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODPOM, "long_name", 37, "Aerosol optical depth 550 nm from POM");
    NC4ERR
    err = NOP (ncid, AODPOM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODPOM;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODSO4", NC_FLOAT, 2, dimids, &AODSO4);
    NC4ERR
    err = NOP2 (ncid, AODSO4, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODSO4, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODSO4, "long_name", 37, "Aerosol optical depth 550 nm from SO4");
    NC4ERR
    err = NOP (ncid, AODSO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODSO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODSOA", NC_FLOAT, 2, dimids, &AODSOA);
    NC4ERR
    err = NOP2 (ncid, AODSOA, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODSOA, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODSOA, "long_name", 37, "Aerosol optical depth 550 nm from SOA");
    NC4ERR
    err = NOP (ncid, AODSOA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODSOA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODSS", NC_FLOAT, 2, dimids, &AODSS);
    NC4ERR
    err = NOP2 (ncid, AODSS, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODSS, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODSS, "long_name", 41, "Aerosol optical depth 550 nm from seasalt");
    NC4ERR
    err = NOP (ncid, AODSS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODSS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODUV", NC_FLOAT, 2, dimids, &AODUV);
    NC4ERR
    err = NOP2 (ncid, AODUV, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODUV, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODUV, "long_name", 28, "Aerosol optical depth 350 nm");
    NC4ERR
    err = NOP (ncid, AODUV, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODUV;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AODVIS", NC_FLOAT, 2, dimids, &AODVIS);
    NC4ERR
    err = NOP2 (ncid, AODVIS, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, AODVIS, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, AODVIS, "long_name", 28, "Aerosol optical depth 550 nm");
    NC4ERR
    err = NOP (ncid, AODVIS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AODVIS;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "AQRAIN", NC_FLOAT, 3, dimids, &AQRAIN);
    NC4ERR
    err = NOP2 (ncid, AQRAIN, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, AQRAIN, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, AQRAIN, "long_name", 25, "Average rain mixing ratio");
    NC4ERR
    err = NOP (ncid, AQRAIN, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQRAIN;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "AQSNOW", NC_FLOAT, 3, dimids, &AQSNOW);
    NC4ERR
    err = NOP2 (ncid, AQSNOW, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, AQSNOW, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, AQSNOW, "long_name", 25, "Average snow mixing ratio");
    NC4ERR
    err = NOP (ncid, AQSNOW, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQSNOW;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AQ_DMS", NC_FLOAT, 2, dimids, &AQ_DMS);
    NC4ERR
    err = NOP (ncid, AQ_DMS, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, AQ_DMS, "long_name", 39, "DMS aqueous chemistry (for gas species)");
    NC4ERR
    err = NOP (ncid, AQ_DMS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_DMS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AQ_H2O2", NC_FLOAT, 2, dimids, &AQ_H2O2);
    NC4ERR
    err = NOP (ncid, AQ_H2O2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, AQ_H2O2, "long_name", 40, "H2O2 aqueous chemistry (for gas species)");
    NC4ERR
    err = NOP (ncid, AQ_H2O2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_H2O2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AQ_H2SO4", NC_FLOAT, 2, dimids, &AQ_H2SO4);
    NC4ERR
    err = NOP (ncid, AQ_H2SO4, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, AQ_H2SO4, "long_name", 41, "H2SO4 aqueous chemistry (for gas species)");
    NC4ERR
    err = NOP (ncid, AQ_H2SO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_H2SO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AQ_O3", NC_FLOAT, 2, dimids, &AQ_O3);
    NC4ERR
    err = NOP (ncid, AQ_O3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, AQ_O3, "long_name", 38, "O3 aqueous chemistry (for gas species)");
    NC4ERR
    err = NOP (ncid, AQ_O3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_O3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AQ_SO2", NC_FLOAT, 2, dimids, &AQ_SO2);
    NC4ERR
    err = NOP (ncid, AQ_SO2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, AQ_SO2, "long_name", 39, "SO2 aqueous chemistry (for gas species)");
    NC4ERR
    err = NOP (ncid, AQ_SO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_SO2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "AQ_SOAG", NC_FLOAT, 2, dimids, &AQ_SOAG);
    NC4ERR
    err = NOP (ncid, AQ_SOAG, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, AQ_SOAG, "long_name", 40, "SOAG aqueous chemistry (for gas species)");
    NC4ERR
    err = NOP (ncid, AQ_SOAG, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AQ_SOAG;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "AREI", NC_FLOAT, 3, dimids, &AREI);
    NC4ERR
    err = NOP2 (ncid, AREI, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, AREI, "units", 6, "Micron");
    NC4ERR
    err = NOP (ncid, AREI, "long_name", 28, "Average ice effective radius");
    NC4ERR
    err = NOP (ncid, AREI, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AREI;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "AREL", NC_FLOAT, 3, dimids, &AREL);
    NC4ERR
    err = NOP2 (ncid, AREL, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, AREL, "units", 6, "Micron");
    NC4ERR
    err = NOP (ncid, AREL, "long_name", 32, "Average droplet effective radius");
    NC4ERR
    err = NOP (ncid, AREL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AREL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "AWNC", NC_FLOAT, 3, dimids, &AWNC);
    NC4ERR
    err = NOP2 (ncid, AWNC, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, AWNC, "units", 3, "m-3");
    NC4ERR
    err = NOP (ncid, AWNC, "long_name", 31, "Average cloud water number conc");
    NC4ERR
    err = NOP (ncid, AWNC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AWNC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "AWNI", NC_FLOAT, 3, dimids, &AWNI);
    NC4ERR
    err = NOP2 (ncid, AWNI, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, AWNI, "units", 3, "m-3");
    NC4ERR
    err = NOP (ncid, AWNI, "long_name", 29, "Average cloud ice number conc");
    NC4ERR
    err = NOP (ncid, AWNI, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = AWNI;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "CCN3", NC_FLOAT, 3, dimids, &CCN3);
    NC4ERR
    err = NOP2 (ncid, CCN3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, CCN3, "units", 5, "#/cm3");
    NC4ERR
    err = NOP (ncid, CCN3, "long_name", 27, "CCN concentration at S=0.1%");
    NC4ERR
    err = NOP (ncid, CCN3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CCN3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "CDNUMC", NC_FLOAT, 2, dimids, &CDNUMC);
    NC4ERR
    err = NOP (ncid, CDNUMC, "units", 4, "1/m2");
    NC4ERR
    err = NOP (ncid, CDNUMC, "long_name", 43, "Vertically-integrated droplet concentration");
    NC4ERR
    err = NOP (ncid, CDNUMC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CDNUMC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "CLDHGH", NC_FLOAT, 2, dimids, &CLDHGH);
    NC4ERR
    err = NOP (ncid, CLDHGH, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CLDHGH, "long_name", 32, "Vertically-integrated high cloud");
    NC4ERR
    err = NOP (ncid, CLDHGH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDHGH;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "CLDICE", NC_FLOAT, 3, dimids, &CLDICE);
    NC4ERR
    err = NOP2 (ncid, CLDICE, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, CLDICE, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, CLDICE, "long_name", 34, "Grid box averaged cloud ice amount");
    NC4ERR
    err = NOP (ncid, CLDICE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDICE;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "CLDLIQ", NC_FLOAT, 3, dimids, &CLDLIQ);
    NC4ERR
    err = NOP2 (ncid, CLDLIQ, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, CLDLIQ, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, CLDLIQ, "long_name", 37, "Grid box averaged cloud liquid amount");
    NC4ERR
    err = NOP (ncid, CLDLIQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDLIQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "CLDLOW", NC_FLOAT, 2, dimids, &CLDLOW);
    NC4ERR
    err = NOP (ncid, CLDLOW, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CLDLOW, "long_name", 31, "Vertically-integrated low cloud");
    NC4ERR
    err = NOP (ncid, CLDLOW, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDLOW;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "CLDMED", NC_FLOAT, 2, dimids, &CLDMED);
    NC4ERR
    err = NOP (ncid, CLDMED, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CLDMED, "long_name", 37, "Vertically-integrated mid-level cloud");
    NC4ERR
    err = NOP (ncid, CLDMED, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDMED;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "CLDTOT", NC_FLOAT, 2, dimids, &CLDTOT);
    NC4ERR
    err = NOP (ncid, CLDTOT, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CLDTOT, "long_name", 33, "Vertically-integrated total cloud");
    NC4ERR
    err = NOP (ncid, CLDTOT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLDTOT;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "CLOUD", NC_FLOAT, 3, dimids, &CLOUD);
    NC4ERR
    err = NOP2 (ncid, CLOUD, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, CLOUD, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CLOUD, "long_name", 14, "Cloud fraction");
    NC4ERR
    err = NOP (ncid, CLOUD, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLOUD;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "CLOUDFRAC_CLUBB", NC_FLOAT, 3, dimids, &CLOUDFRAC_CLUBB);
    NC4ERR
    err = NOP2 (ncid, CLOUDFRAC_CLUBB, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, CLOUDFRAC_CLUBB, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CLOUDFRAC_CLUBB, "long_name", 14, "Cloud Fraction");
    NC4ERR
    err = NOP (ncid, CLOUDFRAC_CLUBB, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CLOUDFRAC_CLUBB;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "CONCLD", NC_FLOAT, 3, dimids, &CONCLD);
    NC4ERR
    err = NOP2 (ncid, CONCLD, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, CONCLD, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CONCLD, "long_name", 22, "Convective cloud cover");
    NC4ERR
    err = NOP (ncid, CONCLD, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = CONCLD;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "DCQ", NC_FLOAT, 3, dimids, &DCQ);
    NC4ERR
    err = NOP2 (ncid, DCQ, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, DCQ, "units", 7, "kg/kg/s");
    NC4ERR
    err = NOP (ncid, DCQ, "long_name", 33, "Q tendency due to moist processes");
    NC4ERR
    err = NOP (ncid, DCQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DCQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DF_DMS", NC_FLOAT, 2, dimids, &DF_DMS);
    NC4ERR
    err = NOP (ncid, DF_DMS, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, DF_DMS, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = NOP (ncid, DF_DMS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_DMS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DF_H2O2", NC_FLOAT, 2, dimids, &DF_H2O2);
    NC4ERR
    err = NOP (ncid, DF_H2O2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, DF_H2O2, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = NOP (ncid, DF_H2O2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_H2O2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DF_H2SO4", NC_FLOAT, 2, dimids, &DF_H2SO4);
    NC4ERR
    err = NOP (ncid, DF_H2SO4, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, DF_H2SO4, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = NOP (ncid, DF_H2SO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_H2SO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DF_O3", NC_FLOAT, 2, dimids, &DF_O3);
    NC4ERR
    err = NOP (ncid, DF_O3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, DF_O3, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = NOP (ncid, DF_O3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_O3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DF_SO2", NC_FLOAT, 2, dimids, &DF_SO2);
    NC4ERR
    err = NOP (ncid, DF_SO2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, DF_SO2, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = NOP (ncid, DF_SO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_SO2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DF_SOAG", NC_FLOAT, 2, dimids, &DF_SOAG);
    NC4ERR
    err = NOP (ncid, DF_SOAG, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, DF_SOAG, "long_name", 19, "dry deposition flux");
    NC4ERR
    err = NOP (ncid, DF_SOAG, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DF_SOAG;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DMS_SRF", NC_FLOAT, 2, dimids, &DMS_SRF);
    NC4ERR
    err = NOP (ncid, DMS_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = NOP (ncid, DMS_SRF, "long_name", 19, "DMS in bottom layer");
    NC4ERR
    err = NOP (ncid, DMS_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DMS_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DP_KCLDBASE", NC_FLOAT, 2, dimids, &DP_KCLDBASE);
    NC4ERR
    err = NOP (ncid, DP_KCLDBASE, "units", 1, "1");
    NC4ERR
    err = NOP (ncid, DP_KCLDBASE, "long_name", 32, "Deep conv. cloudbase level index");
    NC4ERR
    err = NOP (ncid, DP_KCLDBASE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DP_KCLDBASE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DP_MFUP_MAX", NC_FLOAT, 2, dimids, &DP_MFUP_MAX);
    NC4ERR
    err = NOP (ncid, DP_MFUP_MAX, "units", 5, "kg/m2");
    NC4ERR
    err = NOP (ncid, DP_MFUP_MAX, "long_name", 39, "Deep conv. column-max updraft mass flux");
    NC4ERR
    err = NOP (ncid, DP_MFUP_MAX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DP_MFUP_MAX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DP_WCLDBASE", NC_FLOAT, 2, dimids, &DP_WCLDBASE);
    NC4ERR
    err = NOP (ncid, DP_WCLDBASE, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, DP_WCLDBASE, "long_name", 38, "Deep conv. cloudbase vertical velocity");
    NC4ERR
    err = NOP (ncid, DP_WCLDBASE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DP_WCLDBASE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DSTSFMBL", NC_FLOAT, 2, dimids, &DSTSFMBL);
    NC4ERR
    err = NOP (ncid, DSTSFMBL, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, DSTSFMBL, "long_name", 28, "Mobilization flux at surface");
    NC4ERR
    err = NOP (ncid, DSTSFMBL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DSTSFMBL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "DTCOND", NC_FLOAT, 3, dimids, &DTCOND);
    NC4ERR
    err = NOP2 (ncid, DTCOND, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, DTCOND, "units", 3, "K/s");
    NC4ERR
    err = NOP (ncid, DTCOND, "long_name", 28, "T tendency - moist processes");
    NC4ERR
    err = NOP (ncid, DTCOND, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DTCOND;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DTENDTH", NC_FLOAT, 2, dimids, &DTENDTH);
    NC4ERR
    err = NOP (ncid, DTENDTH, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, DTENDTH, "long_name", 69,
               "Dynamic Tendency of Total (vertically integrated) moist static energy");
    NC4ERR
    err = NOP (ncid, DTENDTH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DTENDTH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "DTENDTQ", NC_FLOAT, 2, dimids, &DTENDTQ);
    NC4ERR
    err = NOP (ncid, DTENDTQ, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, DTENDTQ, "long_name", 67,
               "Dynamic Tendency of Total (vertically integrated) specific humidity");
    NC4ERR
    err = NOP (ncid, DTENDTQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = DTENDTQ;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "EXTINCT", NC_FLOAT, 3, dimids, &EXTINCT);
    NC4ERR
    err = NOP2 (ncid, EXTINCT, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP2 (ncid, EXTINCT, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, EXTINCT, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, EXTINCT, "units", 2, "/m");
    NC4ERR
    err = NOP (ncid, EXTINCT, "long_name", 18, "Aerosol extinction");
    NC4ERR
    err = NOP (ncid, EXTINCT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = EXTINCT;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "FICE", NC_FLOAT, 3, dimids, &FICE);
    NC4ERR
    err = NOP2 (ncid, FICE, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, FICE, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, FICE, "long_name", 35, "Fractional ice content within cloud");
    NC4ERR
    err = NOP (ncid, FICE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FICE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FLDS", NC_FLOAT, 2, dimids, &FLDS);
    NC4ERR
    err = NOP (ncid, FLDS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FLDS, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FLDS, "long_name", 36, "Downwelling longwave flux at surface");
    NC4ERR
    err = NOP (ncid, FLDS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLDS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FLNS", NC_FLOAT, 2, dimids, &FLNS);
    NC4ERR
    err = NOP (ncid, FLNS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FLNS, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FLNS, "long_name", 28, "Net longwave flux at surface");
    NC4ERR
    err = NOP (ncid, FLNS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLNS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FLNSC", NC_FLOAT, 2, dimids, &FLNSC);
    NC4ERR
    err = NOP (ncid, FLNSC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FLNSC, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FLNSC, "long_name", 37, "Clearsky net longwave flux at surface");
    NC4ERR
    err = NOP (ncid, FLNSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLNSC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FLNT", NC_FLOAT, 2, dimids, &FLNT);
    NC4ERR
    err = NOP (ncid, FLNT, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FLNT, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FLNT, "long_name", 33, "Net longwave flux at top of model");
    NC4ERR
    err = NOP (ncid, FLNT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLNT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FLNTC", NC_FLOAT, 2, dimids, &FLNTC);
    NC4ERR
    err = NOP (ncid, FLNTC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FLNTC, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FLNTC, "long_name", 42, "Clearsky net longwave flux at top of model");
    NC4ERR
    err = NOP (ncid, FLNTC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLNTC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FLUT", NC_FLOAT, 2, dimids, &FLUT);
    NC4ERR
    err = NOP (ncid, FLUT, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FLUT, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FLUT, "long_name", 39, "Upwelling longwave flux at top of model");
    NC4ERR
    err = NOP (ncid, FLUT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLUT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FLUTC", NC_FLOAT, 2, dimids, &FLUTC);
    NC4ERR
    err = NOP (ncid, FLUTC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FLUTC, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FLUTC, "long_name", 48, "Clearsky upwelling longwave flux at top of model");
    NC4ERR
    err = NOP (ncid, FLUTC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FLUTC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "FREQI", NC_FLOAT, 3, dimids, &FREQI);
    NC4ERR
    err = NOP2 (ncid, FREQI, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, FREQI, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, FREQI, "long_name", 28, "Fractional occurrence of ice");
    NC4ERR
    err = NOP (ncid, FREQI, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FREQI;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "FREQL", NC_FLOAT, 3, dimids, &FREQL);
    NC4ERR
    err = NOP2 (ncid, FREQL, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, FREQL, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, FREQL, "long_name", 31, "Fractional occurrence of liquid");
    NC4ERR
    err = NOP (ncid, FREQL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FREQL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "FREQR", NC_FLOAT, 3, dimids, &FREQR);
    NC4ERR
    err = NOP2 (ncid, FREQR, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, FREQR, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, FREQR, "long_name", 29, "Fractional occurrence of rain");
    NC4ERR
    err = NOP (ncid, FREQR, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FREQR;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "FREQS", NC_FLOAT, 3, dimids, &FREQS);
    NC4ERR
    err = NOP2 (ncid, FREQS, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, FREQS, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, FREQS, "long_name", 29, "Fractional occurrence of snow");
    NC4ERR
    err = NOP (ncid, FREQS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FREQS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSDS", NC_FLOAT, 2, dimids, &FSDS);
    NC4ERR
    err = NOP (ncid, FSDS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSDS, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FSDS, "long_name", 33, "Downwelling solar flux at surface");
    NC4ERR
    err = NOP (ncid, FSDS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSDS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSDSC", NC_FLOAT, 2, dimids, &FSDSC);
    NC4ERR
    err = NOP (ncid, FSDSC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSDSC, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FSDSC, "long_name", 42, "Clearsky downwelling solar flux at surface");
    NC4ERR
    err = NOP (ncid, FSDSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSDSC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSNS", NC_FLOAT, 2, dimids, &FSNS);
    NC4ERR
    err = NOP (ncid, FSNS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSNS, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FSNS, "long_name", 25, "Net solar flux at surface");
    NC4ERR
    err = NOP (ncid, FSNS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSNSC", NC_FLOAT, 2, dimids, &FSNSC);
    NC4ERR
    err = NOP (ncid, FSNSC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSNSC, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FSNSC, "long_name", 34, "Clearsky net solar flux at surface");
    NC4ERR
    err = NOP (ncid, FSNSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNSC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSNT", NC_FLOAT, 2, dimids, &FSNT);
    NC4ERR
    err = NOP (ncid, FSNT, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSNT, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FSNT, "long_name", 30, "Net solar flux at top of model");
    NC4ERR
    err = NOP (ncid, FSNT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSNTC", NC_FLOAT, 2, dimids, &FSNTC);
    NC4ERR
    err = NOP (ncid, FSNTC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSNTC, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FSNTC, "long_name", 39, "Clearsky net solar flux at top of model");
    NC4ERR
    err = NOP (ncid, FSNTC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNTC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSNTOA", NC_FLOAT, 2, dimids, &FSNTOA);
    NC4ERR
    err = NOP (ncid, FSNTOA, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSNTOA, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FSNTOA, "long_name", 35, "Net solar flux at top of atmosphere");
    NC4ERR
    err = NOP (ncid, FSNTOA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNTOA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSNTOAC", NC_FLOAT, 2, dimids, &FSNTOAC);
    NC4ERR
    err = NOP (ncid, FSNTOAC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSNTOAC, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FSNTOAC, "long_name", 44, "Clearsky net solar flux at top of atmosphere");
    NC4ERR
    err = NOP (ncid, FSNTOAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSNTOAC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSUTOA", NC_FLOAT, 2, dimids, &FSUTOA);
    NC4ERR
    err = NOP (ncid, FSUTOA, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSUTOA, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FSUTOA, "long_name", 41, "Upwelling solar flux at top of atmosphere");
    NC4ERR
    err = NOP (ncid, FSUTOA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSUTOA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FSUTOAC", NC_FLOAT, 2, dimids, &FSUTOAC);
    NC4ERR
    err = NOP (ncid, FSUTOAC, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FSUTOAC, "units", 4, "W/m2");
    NC4ERR
    err =
        NOP (ncid, FSUTOAC, "long_name", 50, "Clearsky upwelling solar flux at top of atmosphere");
    NC4ERR
    err = NOP (ncid, FSUTOAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = FSUTOAC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "F_eff", NC_FLOAT, 2, dimids, &F_eff);
    NC4ERR
    err = NOP (ncid, F_eff, "units", 1, "1");
    NC4ERR
    err =
        NOP (ncid, F_eff, "long_name", 52, "Effective enrichment factor of marine organic matter");
    NC4ERR
    err = NOP (ncid, F_eff, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = F_eff;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "H2O2_SRF", NC_FLOAT, 2, dimids, &H2O2_SRF);
    NC4ERR
    err = NOP (ncid, H2O2_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = NOP (ncid, H2O2_SRF, "long_name", 20, "H2O2 in bottom layer");
    NC4ERR
    err = NOP (ncid, H2O2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = H2O2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "H2SO4_SRF", NC_FLOAT, 2, dimids, &H2SO4_SRF);
    NC4ERR
    err = NOP (ncid, H2SO4_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = NOP (ncid, H2SO4_SRF, "long_name", 21, "H2SO4 in bottom layer");
    NC4ERR
    err = NOP (ncid, H2SO4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = H2SO4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "H2SO4_sfgaex1", NC_FLOAT, 2, dimids, &H2SO4_sfgaex1);
    NC4ERR
    err = NOP (ncid, H2SO4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, H2SO4_sfgaex1, "long_name", 50,
               "H2SO4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, H2SO4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = H2SO4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ICEFRAC", NC_FLOAT, 2, dimids, &ICEFRAC);
    NC4ERR
    err = NOP (ncid, ICEFRAC, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, ICEFRAC, "long_name", 39, "Fraction of sfc area covered by sea-ice");
    NC4ERR
    err = NOP (ncid, ICEFRAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ICEFRAC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "ICIMR", NC_FLOAT, 3, dimids, &ICIMR);
    NC4ERR
    err = NOP2 (ncid, ICIMR, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, ICIMR, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, ICIMR, "long_name", 36, "Prognostic in-cloud ice mixing ratio");
    NC4ERR
    err = NOP (ncid, ICIMR, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ICIMR;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "ICWMR", NC_FLOAT, 3, dimids, &ICWMR);
    NC4ERR
    err = NOP2 (ncid, ICWMR, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, ICWMR, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, ICWMR, "long_name", 38, "Prognostic in-cloud water mixing ratio");
    NC4ERR
    err = NOP (ncid, ICWMR, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ICWMR;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "IWC", NC_FLOAT, 3, dimids, &IWC);
    NC4ERR
    err = NOP2 (ncid, IWC, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, IWC, "units", 5, "kg/m3");
    NC4ERR
    err = NOP (ncid, IWC, "long_name", 34, "Grid box average ice water content");
    NC4ERR
    err = NOP (ncid, IWC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = IWC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "LANDFRAC", NC_FLOAT, 2, dimids, &LANDFRAC);
    NC4ERR
    err = NOP (ncid, LANDFRAC, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, LANDFRAC, "long_name", 36, "Fraction of sfc area covered by land");
    NC4ERR
    err = NOP (ncid, LANDFRAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LANDFRAC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "LHFLX", NC_FLOAT, 2, dimids, &LHFLX);
    NC4ERR
    err = NOP (ncid, LHFLX, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, LHFLX, "long_name", 24, "Surface latent heat flux");
    NC4ERR
    err = NOP (ncid, LHFLX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LHFLX;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "LINOZ_DO3", NC_FLOAT, 3, dimids, &LINOZ_DO3);
    NC4ERR
    err = NOP2 (ncid, LINOZ_DO3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, LINOZ_DO3, "units", 2, "/s");
    NC4ERR
    err =
        NOP (ncid, LINOZ_DO3, "long_name", 48, "ozone vmr tendency by linearized ozone chemistry");
    NC4ERR
    err = NOP (ncid, LINOZ_DO3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_DO3;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "LINOZ_DO3_PSC", NC_FLOAT, 3, dimids, &LINOZ_DO3_PSC);
    NC4ERR
    err = NOP2 (ncid, LINOZ_DO3_PSC, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, LINOZ_DO3_PSC, "units", 2, "/s");
    NC4ERR
    err = NOP (ncid, LINOZ_DO3_PSC, "long_name", 50,
               "ozone vmr loss by PSCs using Carille et al. (1990)");
    NC4ERR
    err = NOP (ncid, LINOZ_DO3_PSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_DO3_PSC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "LINOZ_O3CLIM", NC_FLOAT, 3, dimids, &LINOZ_O3CLIM);
    NC4ERR
    err = NOP2 (ncid, LINOZ_O3CLIM, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, LINOZ_O3CLIM, "units", 7, "mol/mol");
    NC4ERR
    err = NOP (ncid, LINOZ_O3CLIM, "long_name", 29, "climatology of ozone in LINOZ");
    NC4ERR
    err = NOP (ncid, LINOZ_O3CLIM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_O3CLIM;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "LINOZ_O3COL", NC_FLOAT, 3, dimids, &LINOZ_O3COL);
    NC4ERR
    err = NOP2 (ncid, LINOZ_O3COL, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, LINOZ_O3COL, "units", 2, "DU");
    NC4ERR
    err = NOP (ncid, LINOZ_O3COL, "long_name", 18, "ozone column above");
    NC4ERR
    err = NOP (ncid, LINOZ_O3COL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_O3COL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "LINOZ_SSO3", NC_FLOAT, 3, dimids, &LINOZ_SSO3);
    NC4ERR
    err = NOP2 (ncid, LINOZ_SSO3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, LINOZ_SSO3, "units", 2, "kg");
    NC4ERR
    err = NOP (ncid, LINOZ_SSO3, "long_name", 27, "steady state ozone in LINOZ");
    NC4ERR
    err = NOP (ncid, LINOZ_SSO3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_SSO3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "LINOZ_SZA", NC_FLOAT, 2, dimids, &LINOZ_SZA);
    NC4ERR
    err = NOP (ncid, LINOZ_SZA, "units", 7, "degrees");
    NC4ERR
    err = NOP (ncid, LINOZ_SZA, "long_name", 27, "solar zenith angle in LINOZ");
    NC4ERR
    err = NOP (ncid, LINOZ_SZA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LINOZ_SZA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "LND_MBL", NC_FLOAT, 2, dimids, &LND_MBL);
    NC4ERR
    err = NOP (ncid, LND_MBL, "units", 4, "frac");
    NC4ERR
    err = NOP (ncid, LND_MBL, "long_name", 23, "Soil erodibility factor");
    NC4ERR
    err = NOP (ncid, LND_MBL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LND_MBL;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "LWCF", NC_FLOAT, 2, dimids, &LWCF);
    NC4ERR
    err = NOP (ncid, LWCF, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, LWCF, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, LWCF, "long_name", 22, "Longwave cloud forcing");
    NC4ERR
    err = NOP (ncid, LWCF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = LWCF;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "Mass_bc", NC_FLOAT, 3, dimids, &Mass_bc);
    NC4ERR
    err = NOP2 (ncid, Mass_bc, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, Mass_bc, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, Mass_bc, "long_name", 64,
               "sum of bc mass concentration bc_a1+bc_c1+bc_a3+bc_c3+bc_a4+bc_c4");
    NC4ERR
    err = NOP (ncid, Mass_bc, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_bc;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "Mass_dst", NC_FLOAT, 3, dimids, &Mass_dst);
    NC4ERR
    err = NOP2 (ncid, Mass_dst, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, Mass_dst, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, Mass_dst, "long_name", 57,
               "sum of dst mass concentration dst_a1+dst_c1+dst_a3+dst_c3");
    NC4ERR
    err = NOP (ncid, Mass_dst, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_dst;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "Mass_mom", NC_FLOAT, 3, dimids, &Mass_mom);
    NC4ERR
    err = NOP2 (ncid, Mass_mom, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, Mass_mom, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (
        ncid, Mass_mom, "long_name", 85,
        "sum of mom mass concentration mom_a1+mom_c1+mom_a2+mom_c2+mom_a3+mom_c3+mom_a4+mom_c4");
    NC4ERR
    err = NOP (ncid, Mass_mom, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_mom;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "Mass_ncl", NC_FLOAT, 3, dimids, &Mass_ncl);
    NC4ERR
    err = NOP2 (ncid, Mass_ncl, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, Mass_ncl, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, Mass_ncl, "long_name", 71,
               "sum of ncl mass concentration ncl_a1+ncl_c1+ncl_a2+ncl_c2+ncl_a3+ncl_c3");
    NC4ERR
    err = NOP (ncid, Mass_ncl, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_ncl;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "Mass_pom", NC_FLOAT, 3, dimids, &Mass_pom);
    NC4ERR
    err = NOP2 (ncid, Mass_pom, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, Mass_pom, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, Mass_pom, "long_name", 71,
               "sum of pom mass concentration pom_a1+pom_c1+pom_a3+pom_c3+pom_a4+pom_c4");
    NC4ERR
    err = NOP (ncid, Mass_pom, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_pom;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "Mass_so4", NC_FLOAT, 3, dimids, &Mass_so4);
    NC4ERR
    err = NOP2 (ncid, Mass_so4, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, Mass_so4, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, Mass_so4, "long_name", 71,
               "sum of so4 mass concentration so4_a1+so4_c1+so4_a2+so4_c2+so4_a3+so4_c3");
    NC4ERR
    err = NOP (ncid, Mass_so4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_so4;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "Mass_soa", NC_FLOAT, 3, dimids, &Mass_soa);
    NC4ERR
    err = NOP2 (ncid, Mass_soa, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, Mass_soa, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, Mass_soa, "long_name", 71,
               "sum of soa mass concentration soa_a1+soa_c1+soa_a2+soa_c2+soa_a3+soa_c3");
    NC4ERR
    err = NOP (ncid, Mass_soa, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Mass_soa;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "NUMICE", NC_FLOAT, 3, dimids, &NUMICE);
    NC4ERR
    err = NOP2 (ncid, NUMICE, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, NUMICE, "units", 4, "1/kg");
    NC4ERR
    err = NOP (ncid, NUMICE, "long_name", 34, "Grid box averaged cloud ice number");
    NC4ERR
    err = NOP (ncid, NUMICE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = NUMICE;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "NUMLIQ", NC_FLOAT, 3, dimids, &NUMLIQ);
    NC4ERR
    err = NOP2 (ncid, NUMLIQ, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, NUMLIQ, "units", 4, "1/kg");
    NC4ERR
    err = NOP (ncid, NUMLIQ, "long_name", 37, "Grid box averaged cloud liquid number");
    NC4ERR
    err = NOP (ncid, NUMLIQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = NUMLIQ;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "NUMRAI", NC_FLOAT, 3, dimids, &NUMRAI);
    NC4ERR
    err = NOP2 (ncid, NUMRAI, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, NUMRAI, "units", 4, "1/kg");
    NC4ERR
    err = NOP (ncid, NUMRAI, "long_name", 29, "Grid box averaged rain number");
    NC4ERR
    err = NOP (ncid, NUMRAI, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = NUMRAI;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "NUMSNO", NC_FLOAT, 3, dimids, &NUMSNO);
    NC4ERR
    err = NOP2 (ncid, NUMSNO, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, NUMSNO, "units", 4, "1/kg");
    NC4ERR
    err = NOP (ncid, NUMSNO, "long_name", 29, "Grid box averaged snow number");
    NC4ERR
    err = NOP (ncid, NUMSNO, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = NUMSNO;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "O3", NC_FLOAT, 3, dimids, &O3);
    NC4ERR
    err = NOP2 (ncid, O3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, O3, "units", 7, "mol/mol");
    NC4ERR
    err = NOP (ncid, O3, "long_name", 16, "O3 concentration");
    NC4ERR
    err = NOP (ncid, O3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = O3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "O3_SRF", NC_FLOAT, 2, dimids, &O3_SRF);
    NC4ERR
    err = NOP (ncid, O3_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = NOP (ncid, O3_SRF, "long_name", 18, "O3 in bottom layer");
    NC4ERR
    err = NOP (ncid, O3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = O3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "OCNFRAC", NC_FLOAT, 2, dimids, &OCNFRAC);
    NC4ERR
    err = NOP (ncid, OCNFRAC, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, OCNFRAC, "long_name", 37, "Fraction of sfc area covered by ocean");
    NC4ERR
    err = NOP (ncid, OCNFRAC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = OCNFRAC;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "OMEGA", NC_FLOAT, 3, dimids, &OMEGA);
    NC4ERR
    err = NOP2 (ncid, OMEGA, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, OMEGA, "units", 4, "Pa/s");
    NC4ERR
    err = NOP (ncid, OMEGA, "long_name", 28, "Vertical velocity (pressure)");
    NC4ERR
    err = NOP (ncid, OMEGA, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = OMEGA;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "OMEGA500", NC_FLOAT, 2, dimids, &OMEGA500);
    NC4ERR
    err = NOP (ncid, OMEGA500, "units", 4, "Pa/s");
    NC4ERR
    err = NOP (ncid, OMEGA500, "long_name", 46, "Vertical velocity at 500 mbar pressure surface");
    NC4ERR
    err = NOP (ncid, OMEGA500, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = OMEGA500;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "OMEGAT", NC_FLOAT, 3, dimids, &OMEGAT);
    NC4ERR
    err = NOP2 (ncid, OMEGAT, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, OMEGAT, "units", 6, "K Pa/s");
    NC4ERR
    err = NOP (ncid, OMEGAT, "long_name", 18, "Vertical heat flux");
    NC4ERR
    err = NOP (ncid, OMEGAT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = OMEGAT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PBLH", NC_FLOAT, 2, dimids, &PBLH);
    NC4ERR
    err = NOP (ncid, PBLH, "units", 1, "m");
    NC4ERR
    err = NOP (ncid, PBLH, "long_name", 10, "PBL height");
    NC4ERR
    err = NOP (ncid, PBLH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PBLH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PHIS", NC_FLOAT, 2, dimids, &PHIS);
    NC4ERR
    err = NOP (ncid, PHIS, "units", 5, "m2/s2");
    NC4ERR
    err = NOP (ncid, PHIS, "long_name", 20, "Surface geopotential");
    NC4ERR
    varids[i++] = PHIS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PRECC", NC_FLOAT, 2, dimids, &PRECC);
    NC4ERR
    err = NOP (ncid, PRECC, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, PRECC, "long_name", 41, "Convective precipitation rate (liq + ice)");
    NC4ERR
    err = NOP (ncid, PRECC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PRECC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PRECL", NC_FLOAT, 2, dimids, &PRECL);
    NC4ERR
    err = NOP (ncid, PRECL, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, PRECL, "long_name", 51, "Large-scale (stable) precipitation rate (liq + ice)");
    NC4ERR
    err = NOP (ncid, PRECL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PRECL;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PRECSC", NC_FLOAT, 2, dimids, &PRECSC);
    NC4ERR
    err = NOP (ncid, PRECSC, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, PRECSC, "long_name", 39, "Convective snow rate (water equivalent)");
    NC4ERR
    err = NOP (ncid, PRECSC, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PRECSC;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PRECSL", NC_FLOAT, 2, dimids, &PRECSL);
    NC4ERR
    err = NOP (ncid, PRECSL, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, PRECSL, "long_name", 49, "Large-scale (stable) snow rate (water equivalent)");
    NC4ERR
    err = NOP (ncid, PRECSL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PRECSL;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PS", NC_FLOAT, 2, dimids, &PS);
    NC4ERR
    err = NOP (ncid, PS, "units", 2, "Pa");
    NC4ERR
    err = NOP (ncid, PS, "long_name", 16, "Surface pressure");
    NC4ERR
    err = NOP (ncid, PS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PSL", NC_FLOAT, 2, dimids, &PSL);
    NC4ERR
    err = NOP (ncid, PSL, "units", 2, "Pa");
    NC4ERR
    err = NOP (ncid, PSL, "long_name", 18, "Sea level pressure");
    NC4ERR
    err = NOP (ncid, PSL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = PSL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "Q", NC_FLOAT, 3, dimids, &Q);
    NC4ERR
    err = NOP2 (ncid, Q, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, Q, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, Q, "long_name", 17, "Specific humidity");
    NC4ERR
    err = NOP (ncid, Q, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Q;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "QFLX", NC_FLOAT, 2, dimids, &QFLX);
    NC4ERR
    err = NOP (ncid, QFLX, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, QFLX, "long_name", 18, "Surface water flux");
    NC4ERR
    err = NOP (ncid, QFLX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = QFLX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "QREFHT", NC_FLOAT, 2, dimids, &QREFHT);
    NC4ERR
    err = NOP (ncid, QREFHT, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, QREFHT, "long_name", 25, "Reference height humidity");
    NC4ERR
    err = NOP (ncid, QREFHT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = QREFHT;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "QRL", NC_FLOAT, 3, dimids, &QRL);
    NC4ERR
    err = NOP2 (ncid, QRL, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, QRL, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, QRL, "units", 3, "K/s");
    NC4ERR
    err = NOP (ncid, QRL, "long_name", 21, "Longwave heating rate");
    NC4ERR
    err = NOP (ncid, QRL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = QRL;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "QRS", NC_FLOAT, 3, dimids, &QRS);
    NC4ERR
    err = NOP2 (ncid, QRS, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, QRS, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, QRS, "units", 3, "K/s");
    NC4ERR
    err = NOP (ncid, QRS, "long_name", 18, "Solar heating rate");
    NC4ERR
    err = NOP (ncid, QRS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = QRS;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "RAINQM", NC_FLOAT, 3, dimids, &RAINQM);
    NC4ERR
    err = NOP2 (ncid, RAINQM, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, RAINQM, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, RAINQM, "long_name", 29, "Grid box averaged rain amount");
    NC4ERR
    err = NOP (ncid, RAINQM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = RAINQM;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "RAM1", NC_FLOAT, 2, dimids, &RAM1);
    NC4ERR
    err = NOP (ncid, RAM1, "units", 4, "frac");
    NC4ERR
    err = NOP (ncid, RAM1, "long_name", 4, "RAM1");
    NC4ERR
    err = NOP (ncid, RAM1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = RAM1;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "RELHUM", NC_FLOAT, 3, dimids, &RELHUM);
    NC4ERR
    err = NOP2 (ncid, RELHUM, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, RELHUM, "units", 7, "percent");
    NC4ERR
    err = NOP (ncid, RELHUM, "long_name", 17, "Relative humidity");
    NC4ERR
    err = NOP (ncid, RELHUM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = RELHUM;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFDMS", NC_FLOAT, 2, dimids, &SFDMS);
    NC4ERR
    err = NOP (ncid, SFDMS, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFDMS, "long_name", 16, "DMS surface flux");
    NC4ERR
    err = NOP (ncid, SFDMS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFDMS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFH2O2", NC_FLOAT, 2, dimids, &SFH2O2);
    NC4ERR
    err = NOP (ncid, SFH2O2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFH2O2, "long_name", 17, "H2O2 surface flux");
    NC4ERR
    err = NOP (ncid, SFH2O2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFH2O2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFH2SO4", NC_FLOAT, 2, dimids, &SFH2SO4);
    NC4ERR
    err = NOP (ncid, SFH2SO4, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFH2SO4, "long_name", 18, "H2SO4 surface flux");
    NC4ERR
    err = NOP (ncid, SFH2SO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFH2SO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFO3", NC_FLOAT, 2, dimids, &SFO3);
    NC4ERR
    err = NOP (ncid, SFO3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFO3, "long_name", 15, "O3 surface flux");
    NC4ERR
    err = NOP (ncid, SFO3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFO3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFSO2", NC_FLOAT, 2, dimids, &SFSO2);
    NC4ERR
    err = NOP (ncid, SFSO2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFSO2, "long_name", 16, "SO2 surface flux");
    NC4ERR
    err = NOP (ncid, SFSO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFSO2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFSOAG", NC_FLOAT, 2, dimids, &SFSOAG);
    NC4ERR
    err = NOP (ncid, SFSOAG, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFSOAG, "long_name", 17, "SOAG surface flux");
    NC4ERR
    err = NOP (ncid, SFSOAG, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFSOAG;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFbc_a1", NC_FLOAT, 2, dimids, &SFbc_a1);
    NC4ERR
    err = NOP (ncid, SFbc_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFbc_a1, "long_name", 18, "bc_a1 surface flux");
    NC4ERR
    err = NOP (ncid, SFbc_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFbc_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFbc_a3", NC_FLOAT, 2, dimids, &SFbc_a3);
    NC4ERR
    err = NOP (ncid, SFbc_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFbc_a3, "long_name", 18, "bc_a3 surface flux");
    NC4ERR
    err = NOP (ncid, SFbc_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFbc_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFbc_a4", NC_FLOAT, 2, dimids, &SFbc_a4);
    NC4ERR
    err = NOP (ncid, SFbc_a4, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFbc_a4, "long_name", 18, "bc_a4 surface flux");
    NC4ERR
    err = NOP (ncid, SFbc_a4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFbc_a4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFdst_a1", NC_FLOAT, 2, dimids, &SFdst_a1);
    NC4ERR
    err = NOP (ncid, SFdst_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFdst_a1, "long_name", 19, "dst_a1 surface flux");
    NC4ERR
    err = NOP (ncid, SFdst_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFdst_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFdst_a3", NC_FLOAT, 2, dimids, &SFdst_a3);
    NC4ERR
    err = NOP (ncid, SFdst_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFdst_a3, "long_name", 19, "dst_a3 surface flux");
    NC4ERR
    err = NOP (ncid, SFdst_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFdst_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFmom_a1", NC_FLOAT, 2, dimids, &SFmom_a1);
    NC4ERR
    err = NOP (ncid, SFmom_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFmom_a1, "long_name", 19, "mom_a1 surface flux");
    NC4ERR
    err = NOP (ncid, SFmom_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFmom_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFmom_a2", NC_FLOAT, 2, dimids, &SFmom_a2);
    NC4ERR
    err = NOP (ncid, SFmom_a2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFmom_a2, "long_name", 19, "mom_a2 surface flux");
    NC4ERR
    err = NOP (ncid, SFmom_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFmom_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFmom_a3", NC_FLOAT, 2, dimids, &SFmom_a3);
    NC4ERR
    err = NOP (ncid, SFmom_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFmom_a3, "long_name", 19, "mom_a3 surface flux");
    NC4ERR
    err = NOP (ncid, SFmom_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFmom_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFmom_a4", NC_FLOAT, 2, dimids, &SFmom_a4);
    NC4ERR
    err = NOP (ncid, SFmom_a4, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFmom_a4, "long_name", 19, "mom_a4 surface flux");
    NC4ERR
    err = NOP (ncid, SFmom_a4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFmom_a4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFncl_a1", NC_FLOAT, 2, dimids, &SFncl_a1);
    NC4ERR
    err = NOP (ncid, SFncl_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFncl_a1, "long_name", 19, "ncl_a1 surface flux");
    NC4ERR
    err = NOP (ncid, SFncl_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFncl_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFncl_a2", NC_FLOAT, 2, dimids, &SFncl_a2);
    NC4ERR
    err = NOP (ncid, SFncl_a2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFncl_a2, "long_name", 19, "ncl_a2 surface flux");
    NC4ERR
    err = NOP (ncid, SFncl_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFncl_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFncl_a3", NC_FLOAT, 2, dimids, &SFncl_a3);
    NC4ERR
    err = NOP (ncid, SFncl_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFncl_a3, "long_name", 19, "ncl_a3 surface flux");
    NC4ERR
    err = NOP (ncid, SFncl_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFncl_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFnum_a1", NC_FLOAT, 2, dimids, &SFnum_a1);
    NC4ERR
    err = NOP (ncid, SFnum_a1, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, SFnum_a1, "long_name", 19, "num_a1 surface flux");
    NC4ERR
    err = NOP (ncid, SFnum_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFnum_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFnum_a2", NC_FLOAT, 2, dimids, &SFnum_a2);
    NC4ERR
    err = NOP (ncid, SFnum_a2, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, SFnum_a2, "long_name", 19, "num_a2 surface flux");
    NC4ERR
    err = NOP (ncid, SFnum_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFnum_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFnum_a3", NC_FLOAT, 2, dimids, &SFnum_a3);
    NC4ERR
    err = NOP (ncid, SFnum_a3, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, SFnum_a3, "long_name", 19, "num_a3 surface flux");
    NC4ERR
    err = NOP (ncid, SFnum_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFnum_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFnum_a4", NC_FLOAT, 2, dimids, &SFnum_a4);
    NC4ERR
    err = NOP (ncid, SFnum_a4, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, SFnum_a4, "long_name", 19, "num_a4 surface flux");
    NC4ERR
    err = NOP (ncid, SFnum_a4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFnum_a4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFpom_a1", NC_FLOAT, 2, dimids, &SFpom_a1);
    NC4ERR
    err = NOP (ncid, SFpom_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFpom_a1, "long_name", 19, "pom_a1 surface flux");
    NC4ERR
    err = NOP (ncid, SFpom_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFpom_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFpom_a3", NC_FLOAT, 2, dimids, &SFpom_a3);
    NC4ERR
    err = NOP (ncid, SFpom_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFpom_a3, "long_name", 19, "pom_a3 surface flux");
    NC4ERR
    err = NOP (ncid, SFpom_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFpom_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFpom_a4", NC_FLOAT, 2, dimids, &SFpom_a4);
    NC4ERR
    err = NOP (ncid, SFpom_a4, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFpom_a4, "long_name", 19, "pom_a4 surface flux");
    NC4ERR
    err = NOP (ncid, SFpom_a4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFpom_a4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFso4_a1", NC_FLOAT, 2, dimids, &SFso4_a1);
    NC4ERR
    err = NOP (ncid, SFso4_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFso4_a1, "long_name", 19, "so4_a1 surface flux");
    NC4ERR
    err = NOP (ncid, SFso4_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFso4_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFso4_a2", NC_FLOAT, 2, dimids, &SFso4_a2);
    NC4ERR
    err = NOP (ncid, SFso4_a2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFso4_a2, "long_name", 19, "so4_a2 surface flux");
    NC4ERR
    err = NOP (ncid, SFso4_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFso4_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFso4_a3", NC_FLOAT, 2, dimids, &SFso4_a3);
    NC4ERR
    err = NOP (ncid, SFso4_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFso4_a3, "long_name", 19, "so4_a3 surface flux");
    NC4ERR
    err = NOP (ncid, SFso4_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFso4_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFsoa_a1", NC_FLOAT, 2, dimids, &SFsoa_a1);
    NC4ERR
    err = NOP (ncid, SFsoa_a1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFsoa_a1, "long_name", 19, "soa_a1 surface flux");
    NC4ERR
    err = NOP (ncid, SFsoa_a1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFsoa_a1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFsoa_a2", NC_FLOAT, 2, dimids, &SFsoa_a2);
    NC4ERR
    err = NOP (ncid, SFsoa_a2, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFsoa_a2, "long_name", 19, "soa_a2 surface flux");
    NC4ERR
    err = NOP (ncid, SFsoa_a2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFsoa_a2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SFsoa_a3", NC_FLOAT, 2, dimids, &SFsoa_a3);
    NC4ERR
    err = NOP (ncid, SFsoa_a3, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SFsoa_a3, "long_name", 19, "soa_a3 surface flux");
    NC4ERR
    err = NOP (ncid, SFsoa_a3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SFsoa_a3;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SHFLX", NC_FLOAT, 2, dimids, &SHFLX);
    NC4ERR
    err = NOP (ncid, SHFLX, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, SHFLX, "long_name", 26, "Surface sensible heat flux");
    NC4ERR
    err = NOP (ncid, SHFLX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SHFLX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SH_KCLDBASE", NC_FLOAT, 2, dimids, &SH_KCLDBASE);
    NC4ERR
    err = NOP (ncid, SH_KCLDBASE, "units", 1, "1");
    NC4ERR
    err = NOP (ncid, SH_KCLDBASE, "long_name", 35, "Shallow conv. cloudbase level index");
    NC4ERR
    err = NOP (ncid, SH_KCLDBASE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SH_KCLDBASE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SH_MFUP_MAX", NC_FLOAT, 2, dimids, &SH_MFUP_MAX);
    NC4ERR
    err = NOP (ncid, SH_MFUP_MAX, "units", 5, "kg/m2");
    NC4ERR
    err = NOP (ncid, SH_MFUP_MAX, "long_name", 42, "Shallow conv. column-max updraft mass flux");
    NC4ERR
    err = NOP (ncid, SH_MFUP_MAX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SH_MFUP_MAX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SH_WCLDBASE", NC_FLOAT, 2, dimids, &SH_WCLDBASE);
    NC4ERR
    err = NOP (ncid, SH_WCLDBASE, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, SH_WCLDBASE, "long_name", 41, "Shallow conv. cloudbase vertical velocity");
    NC4ERR
    err = NOP (ncid, SH_WCLDBASE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SH_WCLDBASE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SNOWHICE", NC_FLOAT, 2, dimids, &SNOWHICE);
    NC4ERR
    err = NOP (ncid, SNOWHICE, "units", 1, "m");
    NC4ERR
    err = NOP (ncid, SNOWHICE, "long_name", 19, "Snow depth over ice");
    NC4ERR
    err = NOP (ncid, SNOWHICE, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SNOWHICE;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SNOWHLND", NC_FLOAT, 2, dimids, &SNOWHLND);
    NC4ERR
    err = NOP (ncid, SNOWHLND, "units", 1, "m");
    NC4ERR
    err = NOP (ncid, SNOWHLND, "long_name", 27, "Water equivalent snow depth");
    NC4ERR
    err = NOP (ncid, SNOWHLND, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SNOWHLND;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "SNOWQM", NC_FLOAT, 3, dimids, &SNOWQM);
    NC4ERR
    err = NOP2 (ncid, SNOWQM, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, SNOWQM, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, SNOWQM, "long_name", 29, "Grid box averaged snow amount");
    NC4ERR
    err = NOP (ncid, SNOWQM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SNOWQM;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "SO2", NC_FLOAT, 3, dimids, &SO2);
    NC4ERR
    err = NOP2 (ncid, SO2, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, SO2, "units", 7, "mol/mol");
    NC4ERR
    err = NOP (ncid, SO2, "long_name", 17, "SO2 concentration");
    NC4ERR
    err = NOP (ncid, SO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SO2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SO2_CLXF", NC_FLOAT, 2, dimids, &SO2_CLXF);
    NC4ERR
    err = NOP (ncid, SO2_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = NOP (ncid, SO2_CLXF, "long_name", 47, "vertically intergrated external forcing for SO2");
    NC4ERR
    err = NOP (ncid, SO2_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SO2_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SO2_SRF", NC_FLOAT, 2, dimids, &SO2_SRF);
    NC4ERR
    err = NOP (ncid, SO2_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = NOP (ncid, SO2_SRF, "long_name", 19, "SO2 in bottom layer");
    NC4ERR
    err = NOP (ncid, SO2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SO2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SOAG_CLXF", NC_FLOAT, 2, dimids, &SOAG_CLXF);
    NC4ERR
    err = NOP (ncid, SOAG_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err =
        NOP (ncid, SOAG_CLXF, "long_name", 48, "vertically intergrated external forcing for SOAG");
    NC4ERR
    err = NOP (ncid, SOAG_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SOAG_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SOAG_SRF", NC_FLOAT, 2, dimids, &SOAG_SRF);
    NC4ERR
    err = NOP (ncid, SOAG_SRF, "units", 7, "mol/mol");
    NC4ERR
    err = NOP (ncid, SOAG_SRF, "long_name", 20, "SOAG in bottom layer");
    NC4ERR
    err = NOP (ncid, SOAG_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SOAG_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SOAG_sfgaex1", NC_FLOAT, 2, dimids, &SOAG_sfgaex1);
    NC4ERR
    err = NOP (ncid, SOAG_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SOAG_sfgaex1, "long_name", 49,
               "SOAG gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, SOAG_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SOAG_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SOLIN", NC_FLOAT, 2, dimids, &SOLIN);
    NC4ERR
    err = NOP (ncid, SOLIN, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, SOLIN, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, SOLIN, "long_name", 16, "Solar insolation");
    NC4ERR
    err = NOP (ncid, SOLIN, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SOLIN;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SSAVIS", NC_FLOAT, 2, dimids, &SSAVIS);
    NC4ERR
    err = NOP2 (ncid, SSAVIS, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, SSAVIS, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, SSAVIS, "long_name", 29, "Aerosol singel-scatter albedo");
    NC4ERR
    err = NOP (ncid, SSAVIS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SSAVIS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SSTSFMBL", NC_FLOAT, 2, dimids, &SSTSFMBL);
    NC4ERR
    err = NOP (ncid, SSTSFMBL, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SSTSFMBL, "long_name", 28, "Mobilization flux at surface");
    NC4ERR
    err = NOP (ncid, SSTSFMBL, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SSTSFMBL;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SSTSFMBL_OM", NC_FLOAT, 2, dimids, &SSTSFMBL_OM);
    NC4ERR
    err = NOP (ncid, SSTSFMBL_OM, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, SSTSFMBL_OM, "long_name", 53,
               "Mobilization flux of marine organic matter at surface");
    NC4ERR
    err = NOP (ncid, SSTSFMBL_OM, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SSTSFMBL_OM;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SWCF", NC_FLOAT, 2, dimids, &SWCF);
    NC4ERR
    err = NOP (ncid, SWCF, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, SWCF, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, SWCF, "long_name", 23, "Shortwave cloud forcing");
    NC4ERR
    err = NOP (ncid, SWCF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = SWCF;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "T", NC_FLOAT, 3, dimids, &T);
    NC4ERR
    err = NOP2 (ncid, T, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, T, "units", 1, "K");
    NC4ERR
    err = NOP (ncid, T, "long_name", 11, "Temperature");
    NC4ERR
    err = NOP (ncid, T, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = T;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TAUGWX", NC_FLOAT, 2, dimids, &TAUGWX);
    NC4ERR
    err = NOP (ncid, TAUGWX, "units", 4, "N/m2");
    NC4ERR
    err = NOP (ncid, TAUGWX, "long_name", 33, "Zonal gravity wave surface stress");
    NC4ERR
    err = NOP (ncid, TAUGWX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TAUGWX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TAUGWY", NC_FLOAT, 2, dimids, &TAUGWY);
    NC4ERR
    err = NOP (ncid, TAUGWY, "units", 4, "N/m2");
    NC4ERR
    err = NOP (ncid, TAUGWY, "long_name", 38, "Meridional gravity wave surface stress");
    NC4ERR
    err = NOP (ncid, TAUGWY, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TAUGWY;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TAUX", NC_FLOAT, 2, dimids, &TAUX);
    NC4ERR
    err = NOP (ncid, TAUX, "units", 4, "N/m2");
    NC4ERR
    err = NOP (ncid, TAUX, "long_name", 20, "Zonal surface stress");
    NC4ERR
    err = NOP (ncid, TAUX, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TAUX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TAUY", NC_FLOAT, 2, dimids, &TAUY);
    NC4ERR
    err = NOP (ncid, TAUY, "units", 4, "N/m2");
    NC4ERR
    err = NOP (ncid, TAUY, "long_name", 25, "Meridional surface stress");
    NC4ERR
    err = NOP (ncid, TAUY, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TAUY;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TGCLDCWP", NC_FLOAT, 2, dimids, &TGCLDCWP);
    NC4ERR
    err = NOP (ncid, TGCLDCWP, "units", 5, "kg/m2");
    NC4ERR
    err = NOP (ncid, TGCLDCWP, "long_name", 48, "Total grid-box cloud water path (liquid and ice)");
    NC4ERR
    err = NOP (ncid, TGCLDCWP, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TGCLDCWP;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TGCLDIWP", NC_FLOAT, 2, dimids, &TGCLDIWP);
    NC4ERR
    err = NOP (ncid, TGCLDIWP, "units", 5, "kg/m2");
    NC4ERR
    err = NOP (ncid, TGCLDIWP, "long_name", 35, "Total grid-box cloud ice water path");
    NC4ERR
    err = NOP (ncid, TGCLDIWP, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TGCLDIWP;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TGCLDLWP", NC_FLOAT, 2, dimids, &TGCLDLWP);
    NC4ERR
    err = NOP (ncid, TGCLDLWP, "units", 5, "kg/m2");
    NC4ERR
    err = NOP (ncid, TGCLDLWP, "long_name", 38, "Total grid-box cloud liquid water path");
    NC4ERR
    err = NOP (ncid, TGCLDLWP, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TGCLDLWP;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TH7001000", NC_FLOAT, 2, dimids, &TH7001000);
    NC4ERR
    err = NOP (ncid, TH7001000, "units", 1, "K");
    NC4ERR
    err = NOP (ncid, TH7001000, "long_name", 33, "Theta difference 700 mb - 1000 mb");
    NC4ERR
    err = NOP (ncid, TH7001000, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TH7001000;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TMQ", NC_FLOAT, 2, dimids, &TMQ);
    NC4ERR
    err = NOP (ncid, TMQ, "units", 5, "kg/m2");
    NC4ERR
    err = NOP (ncid, TMQ, "long_name", 48, "Total (vertically integrated) precipitable water");
    NC4ERR
    err = NOP (ncid, TMQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TMQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TREFHT", NC_FLOAT, 2, dimids, &TREFHT);
    NC4ERR
    err = NOP (ncid, TREFHT, "units", 1, "K");
    NC4ERR
    err = NOP (ncid, TREFHT, "long_name", 28, "Reference height temperature");
    NC4ERR
    err = NOP (ncid, TREFHT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TREFHT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TROP_P", NC_FLOAT, 2, dimids, &TROP_P);
    NC4ERR
    err = NOP2 (ncid, TROP_P, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, TROP_P, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, TROP_P, "units", 2, "Pa");
    NC4ERR
    err = NOP (ncid, TROP_P, "long_name", 19, "Tropopause Pressure");
    NC4ERR
    err = NOP (ncid, TROP_P, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TROP_P;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TROP_T", NC_FLOAT, 2, dimids, &TROP_T);
    NC4ERR
    err = NOP2 (ncid, TROP_T, _FillValue, NC_FLOAT, 1, &fillv);
    NC4ERR
    err = NOP2 (ncid, TROP_T, "missing_value", NC_FLOAT, 1, &missv);
    NC4ERR
    err = NOP (ncid, TROP_T, "units", 1, "K");
    NC4ERR
    err = NOP (ncid, TROP_T, "long_name", 22, "Tropopause Temperature");
    NC4ERR
    err = NOP (ncid, TROP_T, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TROP_T;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TS", NC_FLOAT, 2, dimids, &TS);
    NC4ERR
    err = NOP (ncid, TS, "units", 1, "K");
    NC4ERR
    err = NOP (ncid, TS, "long_name", 31, "Surface temperature (radiative)");
    NC4ERR
    err = NOP (ncid, TS, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TSMN", NC_FLOAT, 2, dimids, &TSMN);
    NC4ERR
    err = NOP (ncid, TSMN, "units", 1, "K");
    NC4ERR
    err = NOP (ncid, TSMN, "long_name", 46, "Minimum surface temperature over output period");
    NC4ERR
    err = NOP (ncid, TSMN, "cell_methods", 13, "time: minimum");
    NC4ERR
    varids[i++] = TSMN;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TSMX", NC_FLOAT, 2, dimids, &TSMX);
    NC4ERR
    err = NOP (ncid, TSMX, "units", 1, "K");
    NC4ERR
    err = NOP (ncid, TSMX, "long_name", 46, "Maximum surface temperature over output period");
    NC4ERR
    err = NOP (ncid, TSMX, "cell_methods", 13, "time: maximum");
    NC4ERR
    varids[i++] = TSMX;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TUH", NC_FLOAT, 2, dimids, &TUH);
    NC4ERR
    err = NOP (ncid, TUH, "units", 3, "W/m");
    NC4ERR
    err = NOP (ncid, TUH, "long_name", 44, "Total (vertically integrated) zonal MSE flux");
    NC4ERR
    err = NOP (ncid, TUH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TUH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TUQ", NC_FLOAT, 2, dimids, &TUQ);
    NC4ERR
    err = NOP (ncid, TUQ, "units", 6, "kg/m/s");
    NC4ERR
    err = NOP (ncid, TUQ, "long_name", 46, "Total (vertically integrated) zonal water flux");
    NC4ERR
    err = NOP (ncid, TUQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TUQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TVH", NC_FLOAT, 2, dimids, &TVH);
    NC4ERR
    err = NOP (ncid, TVH, "units", 3, "W/m");
    NC4ERR
    err = NOP (ncid, TVH, "long_name", 49, "Total (vertically integrated) meridional MSE flux");
    NC4ERR
    err = NOP (ncid, TVH, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TVH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TVQ", NC_FLOAT, 2, dimids, &TVQ);
    NC4ERR
    err = NOP (ncid, TVQ, "units", 6, "kg/m/s");
    NC4ERR
    err = NOP (ncid, TVQ, "long_name", 51, "Total (vertically integrated) meridional water flux");
    NC4ERR
    err = NOP (ncid, TVQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = TVQ;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "U", NC_FLOAT, 3, dimids, &U);
    NC4ERR
    err = NOP2 (ncid, U, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, U, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, U, "long_name", 10, "Zonal wind");
    NC4ERR
    err = NOP (ncid, U, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = U;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "U10", NC_FLOAT, 2, dimids, &U10);
    NC4ERR
    err = NOP (ncid, U10, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, U10, "long_name", 14, "10m wind speed");
    NC4ERR
    err = NOP (ncid, U10, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = U10;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "UU", NC_FLOAT, 3, dimids, &UU);
    NC4ERR
    err = NOP2 (ncid, UU, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, UU, "units", 5, "m2/s2");
    NC4ERR
    err = NOP (ncid, UU, "long_name", 22, "Zonal velocity squared");
    NC4ERR
    err = NOP (ncid, UU, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = UU;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "V", NC_FLOAT, 3, dimids, &V);
    NC4ERR
    err = NOP2 (ncid, V, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, V, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, V, "long_name", 15, "Meridional wind");
    NC4ERR
    err = NOP (ncid, V, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = V;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "VQ", NC_FLOAT, 3, dimids, &VQ);
    NC4ERR
    err = NOP2 (ncid, VQ, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, VQ, "units", 8, "m/skg/kg");
    NC4ERR
    err = NOP (ncid, VQ, "long_name", 26, "Meridional water transport");
    NC4ERR
    err = NOP (ncid, VQ, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = VQ;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "VT", NC_FLOAT, 3, dimids, &VT);
    NC4ERR
    err = NOP2 (ncid, VT, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, VT, "units", 5, "K m/s");
    NC4ERR
    err = NOP (ncid, VT, "long_name", 25, "Meridional heat transport");
    NC4ERR
    err = NOP (ncid, VT, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = VT;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "VU", NC_FLOAT, 3, dimids, &VU);
    NC4ERR
    err = NOP2 (ncid, VU, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, VU, "units", 5, "m2/s2");
    NC4ERR
    err = NOP (ncid, VU, "long_name", 33, "Meridional flux of zonal momentum");
    NC4ERR
    err = NOP (ncid, VU, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = VU;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "VV", NC_FLOAT, 3, dimids, &VV);
    NC4ERR
    err = NOP2 (ncid, VV, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, VV, "units", 5, "m2/s2");
    NC4ERR
    err = NOP (ncid, VV, "long_name", 27, "Meridional velocity squared");
    NC4ERR
    err = NOP (ncid, VV, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = VV;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "WD_H2O2", NC_FLOAT, 2, dimids, &WD_H2O2);
    NC4ERR
    err = NOP (ncid, WD_H2O2, "units", 4, "kg/s");
    NC4ERR
    err = NOP (ncid, WD_H2O2, "long_name", 31, "H2O2             wet deposition");
    NC4ERR
    err = NOP (ncid, WD_H2O2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = WD_H2O2;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "WD_H2SO4", NC_FLOAT, 2, dimids, &WD_H2SO4);
    NC4ERR
    err = NOP (ncid, WD_H2SO4, "units", 4, "kg/s");
    NC4ERR
    err = NOP (ncid, WD_H2SO4, "long_name", 31, "H2SO4            wet deposition");
    NC4ERR
    err = NOP (ncid, WD_H2SO4, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = WD_H2SO4;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "WD_SO2", NC_FLOAT, 2, dimids, &WD_SO2);
    NC4ERR
    err = NOP (ncid, WD_SO2, "units", 4, "kg/s");
    NC4ERR
    err = NOP (ncid, WD_SO2, "long_name", 31, "SO2              wet deposition");
    NC4ERR
    err = NOP (ncid, WD_SO2, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = WD_SO2;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "WSUB", NC_FLOAT, 3, dimids, &WSUB);
    NC4ERR
    err = NOP2 (ncid, WSUB, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, WSUB, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, WSUB, "long_name", 37, "Diagnostic sub-grid vertical velocity");
    NC4ERR
    err = NOP (ncid, WSUB, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = WSUB;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "Z3", NC_FLOAT, 3, dimids, &Z3);
    NC4ERR
    err = NOP2 (ncid, Z3, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, Z3, "units", 1, "m");
    NC4ERR
    err = NOP (ncid, Z3, "long_name", 37, "Geopotential Height (above sea level)");
    NC4ERR
    err = NOP (ncid, Z3, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = Z3;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "aero_water", NC_FLOAT, 3, dimids, &aero_water);
    NC4ERR
    err = NOP2 (ncid, aero_water, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, aero_water, "units", 1, "m");
    NC4ERR
    err = NOP (ncid, aero_water, "long_name", 70,
               "sum of aerosol water of interstitial modes wat_a1+wat_a2+wat_a3+wat_a4");
    NC4ERR
    err = NOP (ncid, aero_water, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = aero_water;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "airFV", NC_FLOAT, 2, dimids, &airFV);
    NC4ERR
    err = NOP (ncid, airFV, "units", 4, "frac");
    NC4ERR
    err = NOP (ncid, airFV, "long_name", 2, "FV");
    NC4ERR
    err = NOP (ncid, airFV, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = airFV;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a1DDF", NC_FLOAT, 2, dimids, &bc_a1DDF);
    NC4ERR
    err = NOP (ncid, bc_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        NOP (ncid, bc_a1DDF, "long_name", 49, "bc_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, bc_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a1SFWET", NC_FLOAT, 2, dimids, &bc_a1SFWET);
    NC4ERR
    err = NOP (ncid, bc_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, bc_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, bc_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a1_SRF", NC_FLOAT, 2, dimids, &bc_a1_SRF);
    NC4ERR
    err = NOP (ncid, bc_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, bc_a1_SRF, "long_name", 21, "bc_a1 in bottom layer");
    NC4ERR
    err = NOP (ncid, bc_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a1_sfgaex1", NC_FLOAT, 2, dimids, &bc_a1_sfgaex1);
    NC4ERR
    err = NOP (ncid, bc_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, bc_a1_sfgaex1, "long_name", 51,
               "bc_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, bc_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a3DDF", NC_FLOAT, 2, dimids, &bc_a3DDF);
    NC4ERR
    err = NOP (ncid, bc_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        NOP (ncid, bc_a3DDF, "long_name", 49, "bc_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, bc_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a3SFWET", NC_FLOAT, 2, dimids, &bc_a3SFWET);
    NC4ERR
    err = NOP (ncid, bc_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, bc_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, bc_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a3_SRF", NC_FLOAT, 2, dimids, &bc_a3_SRF);
    NC4ERR
    err = NOP (ncid, bc_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, bc_a3_SRF, "long_name", 21, "bc_a3 in bottom layer");
    NC4ERR
    err = NOP (ncid, bc_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a4DDF", NC_FLOAT, 2, dimids, &bc_a4DDF);
    NC4ERR
    err = NOP (ncid, bc_a4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        NOP (ncid, bc_a4DDF, "long_name", 49, "bc_a4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, bc_a4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a4SFWET", NC_FLOAT, 2, dimids, &bc_a4SFWET);
    NC4ERR
    err = NOP (ncid, bc_a4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, bc_a4SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, bc_a4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a4_CLXF", NC_FLOAT, 2, dimids, &bc_a4_CLXF);
    NC4ERR
    err = NOP (ncid, bc_a4_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = NOP (ncid, bc_a4_CLXF, "long_name", 49,
               "vertically intergrated external forcing for bc_a4");
    NC4ERR
    err = NOP (ncid, bc_a4_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a4_SRF", NC_FLOAT, 2, dimids, &bc_a4_SRF);
    NC4ERR
    err = NOP (ncid, bc_a4_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, bc_a4_SRF, "long_name", 21, "bc_a4 in bottom layer");
    NC4ERR
    err = NOP (ncid, bc_a4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_a4_sfgaex1", NC_FLOAT, 2, dimids, &bc_a4_sfgaex1);
    NC4ERR
    err = NOP (ncid, bc_a4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, bc_a4_sfgaex1, "long_name", 50,
               "bc_a4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, bc_a4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_a4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_c1DDF", NC_FLOAT, 2, dimids, &bc_c1DDF);
    NC4ERR
    err = NOP (ncid, bc_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        NOP (ncid, bc_c1DDF, "long_name", 49, "bc_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, bc_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_c1SFWET", NC_FLOAT, 2, dimids, &bc_c1SFWET);
    NC4ERR
    err = NOP (ncid, bc_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, bc_c1SFWET, "long_name", 36, "bc_c1 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, bc_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_c3DDF", NC_FLOAT, 2, dimids, &bc_c3DDF);
    NC4ERR
    err = NOP (ncid, bc_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        NOP (ncid, bc_c3DDF, "long_name", 49, "bc_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, bc_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_c3SFWET", NC_FLOAT, 2, dimids, &bc_c3SFWET);
    NC4ERR
    err = NOP (ncid, bc_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, bc_c3SFWET, "long_name", 36, "bc_c3 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, bc_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_c4DDF", NC_FLOAT, 2, dimids, &bc_c4DDF);
    NC4ERR
    err = NOP (ncid, bc_c4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err =
        NOP (ncid, bc_c4DDF, "long_name", 49, "bc_c4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, bc_c4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "bc_c4SFWET", NC_FLOAT, 2, dimids, &bc_c4SFWET);
    NC4ERR
    err = NOP (ncid, bc_c4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, bc_c4SFWET, "long_name", 36, "bc_c4 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, bc_c4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = bc_c4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "chla", NC_FLOAT, 2, dimids, &chla);
    NC4ERR
    err = NOP (ncid, chla, "units", 6, "mg L-1");
    NC4ERR
    err = NOP (ncid, chla, "long_name", 22, "ocean input data: chla");
    NC4ERR
    err = NOP (ncid, chla, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = chla;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_a1DDF", NC_FLOAT, 2, dimids, &dst_a1DDF);
    NC4ERR
    err = NOP (ncid, dst_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_a1DDF, "long_name", 50,
               "dst_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, dst_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_a1SF", NC_FLOAT, 2, dimids, &dst_a1SF);
    NC4ERR
    err = NOP (ncid, dst_a1SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_a1SF, "long_name", 28, "dst_a1 dust surface emission");
    NC4ERR
    err = NOP (ncid, dst_a1SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a1SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_a1SFWET", NC_FLOAT, 2, dimids, &dst_a1SFWET);
    NC4ERR
    err = NOP (ncid, dst_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, dst_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_a1_SRF", NC_FLOAT, 2, dimids, &dst_a1_SRF);
    NC4ERR
    err = NOP (ncid, dst_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, dst_a1_SRF, "long_name", 22, "dst_a1 in bottom layer");
    NC4ERR
    err = NOP (ncid, dst_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_a3DDF", NC_FLOAT, 2, dimids, &dst_a3DDF);
    NC4ERR
    err = NOP (ncid, dst_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_a3DDF, "long_name", 50,
               "dst_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, dst_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_a3SF", NC_FLOAT, 2, dimids, &dst_a3SF);
    NC4ERR
    err = NOP (ncid, dst_a3SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_a3SF, "long_name", 28, "dst_a3 dust surface emission");
    NC4ERR
    err = NOP (ncid, dst_a3SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a3SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_a3SFWET", NC_FLOAT, 2, dimids, &dst_a3SFWET);
    NC4ERR
    err = NOP (ncid, dst_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, dst_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_a3_SRF", NC_FLOAT, 2, dimids, &dst_a3_SRF);
    NC4ERR
    err = NOP (ncid, dst_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, dst_a3_SRF, "long_name", 22, "dst_a3 in bottom layer");
    NC4ERR
    err = NOP (ncid, dst_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_c1DDF", NC_FLOAT, 2, dimids, &dst_c1DDF);
    NC4ERR
    err = NOP (ncid, dst_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_c1DDF, "long_name", 50,
               "dst_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, dst_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_c1SFWET", NC_FLOAT, 2, dimids, &dst_c1SFWET);
    NC4ERR
    err = NOP (ncid, dst_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_c1SFWET, "long_name", 37, "dst_c1 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, dst_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_c3DDF", NC_FLOAT, 2, dimids, &dst_c3DDF);
    NC4ERR
    err = NOP (ncid, dst_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_c3DDF, "long_name", 50,
               "dst_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, dst_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "dst_c3SFWET", NC_FLOAT, 2, dimids, &dst_c3SFWET);
    NC4ERR
    err = NOP (ncid, dst_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, dst_c3SFWET, "long_name", 37, "dst_c3 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, dst_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = dst_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "hstobie_linoz", NC_FLOAT, 3, dimids, &hstobie_linoz);
    NC4ERR
    err = NOP2 (ncid, hstobie_linoz, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, hstobie_linoz, "units", 22, "fraction of model time");
    NC4ERR
    err = NOP (ncid, hstobie_linoz, "long_name", 27, "Lowest possible Linoz level");
    NC4ERR
    varids[i++] = hstobie_linoz;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mlip", NC_FLOAT, 2, dimids, &mlip);
    NC4ERR
    err = NOP (ncid, mlip, "units", 4, "uM C");
    NC4ERR
    err = NOP (ncid, mlip, "long_name", 22, "ocean input data: mlip");
    NC4ERR
    err = NOP (ncid, mlip, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mlip;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a1DDF", NC_FLOAT, 2, dimids, &mom_a1DDF);
    NC4ERR
    err = NOP (ncid, mom_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a1DDF, "long_name", 50,
               "mom_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, mom_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a1SF", NC_FLOAT, 2, dimids, &mom_a1SF);
    NC4ERR
    err = NOP (ncid, mom_a1SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a1SF, "long_name", 31, "mom_a1 seasalt surface emission");
    NC4ERR
    err = NOP (ncid, mom_a1SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a1SFWET", NC_FLOAT, 2, dimids, &mom_a1SFWET);
    NC4ERR
    err = NOP (ncid, mom_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, mom_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a1_SRF", NC_FLOAT, 2, dimids, &mom_a1_SRF);
    NC4ERR
    err = NOP (ncid, mom_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, mom_a1_SRF, "long_name", 22, "mom_a1 in bottom layer");
    NC4ERR
    err = NOP (ncid, mom_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a1_sfgaex1", NC_FLOAT, 2, dimids, &mom_a1_sfgaex1);
    NC4ERR
    err = NOP (ncid, mom_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a1_sfgaex1, "long_name", 51,
               "mom_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, mom_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a2DDF", NC_FLOAT, 2, dimids, &mom_a2DDF);
    NC4ERR
    err = NOP (ncid, mom_a2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a2DDF, "long_name", 50,
               "mom_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, mom_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a2SF", NC_FLOAT, 2, dimids, &mom_a2SF);
    NC4ERR
    err = NOP (ncid, mom_a2SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a2SF, "long_name", 31, "mom_a2 seasalt surface emission");
    NC4ERR
    err = NOP (ncid, mom_a2SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a2SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a2SFWET", NC_FLOAT, 2, dimids, &mom_a2SFWET);
    NC4ERR
    err = NOP (ncid, mom_a2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, mom_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a2_SRF", NC_FLOAT, 2, dimids, &mom_a2_SRF);
    NC4ERR
    err = NOP (ncid, mom_a2_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, mom_a2_SRF, "long_name", 22, "mom_a2 in bottom layer");
    NC4ERR
    err = NOP (ncid, mom_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a3DDF", NC_FLOAT, 2, dimids, &mom_a3DDF);
    NC4ERR
    err = NOP (ncid, mom_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a3DDF, "long_name", 50,
               "mom_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, mom_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a3SFWET", NC_FLOAT, 2, dimids, &mom_a3SFWET);
    NC4ERR
    err = NOP (ncid, mom_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, mom_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a3_SRF", NC_FLOAT, 2, dimids, &mom_a3_SRF);
    NC4ERR
    err = NOP (ncid, mom_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, mom_a3_SRF, "long_name", 22, "mom_a3 in bottom layer");
    NC4ERR
    err = NOP (ncid, mom_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a4DDF", NC_FLOAT, 2, dimids, &mom_a4DDF);
    NC4ERR
    err = NOP (ncid, mom_a4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a4DDF, "long_name", 50,
               "mom_a4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, mom_a4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a4SF", NC_FLOAT, 2, dimids, &mom_a4SF);
    NC4ERR
    err = NOP (ncid, mom_a4SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a4SF, "long_name", 31, "mom_a4 seasalt surface emission");
    NC4ERR
    err = NOP (ncid, mom_a4SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a4SFWET", NC_FLOAT, 2, dimids, &mom_a4SFWET);
    NC4ERR
    err = NOP (ncid, mom_a4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a4SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, mom_a4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a4_SRF", NC_FLOAT, 2, dimids, &mom_a4_SRF);
    NC4ERR
    err = NOP (ncid, mom_a4_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, mom_a4_SRF, "long_name", 22, "mom_a4 in bottom layer");
    NC4ERR
    err = NOP (ncid, mom_a4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_a4_sfgaex1", NC_FLOAT, 2, dimids, &mom_a4_sfgaex1);
    NC4ERR
    err = NOP (ncid, mom_a4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_a4_sfgaex1, "long_name", 51,
               "mom_a4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, mom_a4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_a4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_c1DDF", NC_FLOAT, 2, dimids, &mom_c1DDF);
    NC4ERR
    err = NOP (ncid, mom_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_c1DDF, "long_name", 50,
               "mom_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, mom_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_c1SFWET", NC_FLOAT, 2, dimids, &mom_c1SFWET);
    NC4ERR
    err = NOP (ncid, mom_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_c1SFWET, "long_name", 37, "mom_c1 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, mom_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_c2DDF", NC_FLOAT, 2, dimids, &mom_c2DDF);
    NC4ERR
    err = NOP (ncid, mom_c2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_c2DDF, "long_name", 50,
               "mom_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, mom_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_c2SFWET", NC_FLOAT, 2, dimids, &mom_c2SFWET);
    NC4ERR
    err = NOP (ncid, mom_c2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_c2SFWET, "long_name", 37, "mom_c2 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, mom_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_c3DDF", NC_FLOAT, 2, dimids, &mom_c3DDF);
    NC4ERR
    err = NOP (ncid, mom_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_c3DDF, "long_name", 50,
               "mom_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, mom_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_c3SFWET", NC_FLOAT, 2, dimids, &mom_c3SFWET);
    NC4ERR
    err = NOP (ncid, mom_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_c3SFWET, "long_name", 37, "mom_c3 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, mom_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_c4DDF", NC_FLOAT, 2, dimids, &mom_c4DDF);
    NC4ERR
    err = NOP (ncid, mom_c4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_c4DDF, "long_name", 50,
               "mom_c4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, mom_c4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mom_c4SFWET", NC_FLOAT, 2, dimids, &mom_c4SFWET);
    NC4ERR
    err = NOP (ncid, mom_c4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, mom_c4SFWET, "long_name", 37, "mom_c4 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, mom_c4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mom_c4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mpoly", NC_FLOAT, 2, dimids, &mpoly);
    NC4ERR
    err = NOP (ncid, mpoly, "units", 4, "uM C");
    NC4ERR
    err = NOP (ncid, mpoly, "long_name", 23, "ocean input data: mpoly");
    NC4ERR
    err = NOP (ncid, mpoly, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mpoly;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "mprot", NC_FLOAT, 2, dimids, &mprot);
    NC4ERR
    err = NOP (ncid, mprot, "units", 4, "uM C");
    NC4ERR
    err = NOP (ncid, mprot, "long_name", 23, "ocean input data: mprot");
    NC4ERR
    err = NOP (ncid, mprot, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = mprot;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a1DDF", NC_FLOAT, 2, dimids, &ncl_a1DDF);
    NC4ERR
    err = NOP (ncid, ncl_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_a1DDF, "long_name", 50,
               "ncl_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, ncl_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a1SF", NC_FLOAT, 2, dimids, &ncl_a1SF);
    NC4ERR
    err = NOP (ncid, ncl_a1SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_a1SF, "long_name", 31, "ncl_a1 seasalt surface emission");
    NC4ERR
    err = NOP (ncid, ncl_a1SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a1SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a1SFWET", NC_FLOAT, 2, dimids, &ncl_a1SFWET);
    NC4ERR
    err = NOP (ncid, ncl_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, ncl_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a1_SRF", NC_FLOAT, 2, dimids, &ncl_a1_SRF);
    NC4ERR
    err = NOP (ncid, ncl_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, ncl_a1_SRF, "long_name", 22, "ncl_a1 in bottom layer");
    NC4ERR
    err = NOP (ncid, ncl_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a2DDF", NC_FLOAT, 2, dimids, &ncl_a2DDF);
    NC4ERR
    err = NOP (ncid, ncl_a2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_a2DDF, "long_name", 50,
               "ncl_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, ncl_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a2SF", NC_FLOAT, 2, dimids, &ncl_a2SF);
    NC4ERR
    err = NOP (ncid, ncl_a2SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_a2SF, "long_name", 31, "ncl_a2 seasalt surface emission");
    NC4ERR
    err = NOP (ncid, ncl_a2SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a2SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a2SFWET", NC_FLOAT, 2, dimids, &ncl_a2SFWET);
    NC4ERR
    err = NOP (ncid, ncl_a2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, ncl_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a2_SRF", NC_FLOAT, 2, dimids, &ncl_a2_SRF);
    NC4ERR
    err = NOP (ncid, ncl_a2_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, ncl_a2_SRF, "long_name", 22, "ncl_a2 in bottom layer");
    NC4ERR
    err = NOP (ncid, ncl_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a3DDF", NC_FLOAT, 2, dimids, &ncl_a3DDF);
    NC4ERR
    err = NOP (ncid, ncl_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_a3DDF, "long_name", 50,
               "ncl_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, ncl_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a3SF", NC_FLOAT, 2, dimids, &ncl_a3SF);
    NC4ERR
    err = NOP (ncid, ncl_a3SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_a3SF, "long_name", 31, "ncl_a3 seasalt surface emission");
    NC4ERR
    err = NOP (ncid, ncl_a3SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a3SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a3SFWET", NC_FLOAT, 2, dimids, &ncl_a3SFWET);
    NC4ERR
    err = NOP (ncid, ncl_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, ncl_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_a3_SRF", NC_FLOAT, 2, dimids, &ncl_a3_SRF);
    NC4ERR
    err = NOP (ncid, ncl_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, ncl_a3_SRF, "long_name", 22, "ncl_a3 in bottom layer");
    NC4ERR
    err = NOP (ncid, ncl_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_c1DDF", NC_FLOAT, 2, dimids, &ncl_c1DDF);
    NC4ERR
    err = NOP (ncid, ncl_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_c1DDF, "long_name", 50,
               "ncl_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, ncl_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_c1SFWET", NC_FLOAT, 2, dimids, &ncl_c1SFWET);
    NC4ERR
    err = NOP (ncid, ncl_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_c1SFWET, "long_name", 37, "ncl_c1 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, ncl_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_c2DDF", NC_FLOAT, 2, dimids, &ncl_c2DDF);
    NC4ERR
    err = NOP (ncid, ncl_c2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_c2DDF, "long_name", 50,
               "ncl_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, ncl_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_c2SFWET", NC_FLOAT, 2, dimids, &ncl_c2SFWET);
    NC4ERR
    err = NOP (ncid, ncl_c2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_c2SFWET, "long_name", 37, "ncl_c2 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, ncl_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_c3DDF", NC_FLOAT, 2, dimids, &ncl_c3DDF);
    NC4ERR
    err = NOP (ncid, ncl_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_c3DDF, "long_name", 50,
               "ncl_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, ncl_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "ncl_c3SFWET", NC_FLOAT, 2, dimids, &ncl_c3SFWET);
    NC4ERR
    err = NOP (ncid, ncl_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, ncl_c3SFWET, "long_name", 37, "ncl_c3 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, ncl_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = ncl_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a1DDF", NC_FLOAT, 2, dimids, &num_a1DDF);
    NC4ERR
    err = NOP (ncid, num_a1DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_a1DDF, "long_name", 50,
               "num_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, num_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a1SF", NC_FLOAT, 2, dimids, &num_a1SF);
    NC4ERR
    err = NOP (ncid, num_a1SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, num_a1SF, "long_name", 28, "num_a1 dust surface emission");
    NC4ERR
    err = NOP (ncid, num_a1SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a1SFWET", NC_FLOAT, 2, dimids, &num_a1SFWET);
    NC4ERR
    err = NOP (ncid, num_a1SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, num_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a1_CLXF", NC_FLOAT, 2, dimids, &num_a1_CLXF);
    NC4ERR
    err = NOP (ncid, num_a1_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = NOP (ncid, num_a1_CLXF, "long_name", 50,
               "vertically intergrated external forcing for num_a1");
    NC4ERR
    err = NOP (ncid, num_a1_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a1_SRF", NC_FLOAT, 2, dimids, &num_a1_SRF);
    NC4ERR
    err = NOP (ncid, num_a1_SRF, "units", 5, " 1/kg");
    NC4ERR
    err = NOP (ncid, num_a1_SRF, "long_name", 22, "num_a1 in bottom layer");
    NC4ERR
    err = NOP (ncid, num_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a1_sfgaex1", NC_FLOAT, 2, dimids, &num_a1_sfgaex1);
    NC4ERR
    err = NOP (ncid, num_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, num_a1_sfgaex1, "long_name", 51,
               "num_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, num_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a2DDF", NC_FLOAT, 2, dimids, &num_a2DDF);
    NC4ERR
    err = NOP (ncid, num_a2DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_a2DDF, "long_name", 50,
               "num_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, num_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a2SFWET", NC_FLOAT, 2, dimids, &num_a2SFWET);
    NC4ERR
    err = NOP (ncid, num_a2SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, num_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a2_CLXF", NC_FLOAT, 2, dimids, &num_a2_CLXF);
    NC4ERR
    err = NOP (ncid, num_a2_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = NOP (ncid, num_a2_CLXF, "long_name", 50,
               "vertically intergrated external forcing for num_a2");
    NC4ERR
    err = NOP (ncid, num_a2_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a2_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a2_SRF", NC_FLOAT, 2, dimids, &num_a2_SRF);
    NC4ERR
    err = NOP (ncid, num_a2_SRF, "units", 5, " 1/kg");
    NC4ERR
    err = NOP (ncid, num_a2_SRF, "long_name", 22, "num_a2 in bottom layer");
    NC4ERR
    err = NOP (ncid, num_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a3DDF", NC_FLOAT, 2, dimids, &num_a3DDF);
    NC4ERR
    err = NOP (ncid, num_a3DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_a3DDF, "long_name", 50,
               "num_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, num_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a3SF", NC_FLOAT, 2, dimids, &num_a3SF);
    NC4ERR
    err = NOP (ncid, num_a3SF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, num_a3SF, "long_name", 28, "num_a3 dust surface emission");
    NC4ERR
    err = NOP (ncid, num_a3SF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a3SF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a3SFWET", NC_FLOAT, 2, dimids, &num_a3SFWET);
    NC4ERR
    err = NOP (ncid, num_a3SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, num_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a3_SRF", NC_FLOAT, 2, dimids, &num_a3_SRF);
    NC4ERR
    err = NOP (ncid, num_a3_SRF, "units", 5, " 1/kg");
    NC4ERR
    err = NOP (ncid, num_a3_SRF, "long_name", 22, "num_a3 in bottom layer");
    NC4ERR
    err = NOP (ncid, num_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a4DDF", NC_FLOAT, 2, dimids, &num_a4DDF);
    NC4ERR
    err = NOP (ncid, num_a4DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_a4DDF, "long_name", 50,
               "num_a4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, num_a4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a4SFWET", NC_FLOAT, 2, dimids, &num_a4SFWET);
    NC4ERR
    err = NOP (ncid, num_a4SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_a4SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, num_a4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a4_CLXF", NC_FLOAT, 2, dimids, &num_a4_CLXF);
    NC4ERR
    err = NOP (ncid, num_a4_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = NOP (ncid, num_a4_CLXF, "long_name", 50,
               "vertically intergrated external forcing for num_a4");
    NC4ERR
    err = NOP (ncid, num_a4_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a4_SRF", NC_FLOAT, 2, dimids, &num_a4_SRF);
    NC4ERR
    err = NOP (ncid, num_a4_SRF, "units", 5, " 1/kg");
    NC4ERR
    err = NOP (ncid, num_a4_SRF, "long_name", 22, "num_a4 in bottom layer");
    NC4ERR
    err = NOP (ncid, num_a4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_a4_sfgaex1", NC_FLOAT, 2, dimids, &num_a4_sfgaex1);
    NC4ERR
    err = NOP (ncid, num_a4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, num_a4_sfgaex1, "long_name", 51,
               "num_a4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, num_a4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_a4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_c1DDF", NC_FLOAT, 2, dimids, &num_c1DDF);
    NC4ERR
    err = NOP (ncid, num_c1DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_c1DDF, "long_name", 50,
               "num_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, num_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_c1SFWET", NC_FLOAT, 2, dimids, &num_c1SFWET);
    NC4ERR
    err = NOP (ncid, num_c1SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_c1SFWET, "long_name", 37, "num_c1 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, num_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_c2DDF", NC_FLOAT, 2, dimids, &num_c2DDF);
    NC4ERR
    err = NOP (ncid, num_c2DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_c2DDF, "long_name", 50,
               "num_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, num_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_c2SFWET", NC_FLOAT, 2, dimids, &num_c2SFWET);
    NC4ERR
    err = NOP (ncid, num_c2SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_c2SFWET, "long_name", 37, "num_c2 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, num_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_c3DDF", NC_FLOAT, 2, dimids, &num_c3DDF);
    NC4ERR
    err = NOP (ncid, num_c3DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_c3DDF, "long_name", 50,
               "num_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, num_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_c3SFWET", NC_FLOAT, 2, dimids, &num_c3SFWET);
    NC4ERR
    err = NOP (ncid, num_c3SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_c3SFWET, "long_name", 37, "num_c3 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, num_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_c4DDF", NC_FLOAT, 2, dimids, &num_c4DDF);
    NC4ERR
    err = NOP (ncid, num_c4DDF, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_c4DDF, "long_name", 50,
               "num_c4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, num_c4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "num_c4SFWET", NC_FLOAT, 2, dimids, &num_c4SFWET);
    NC4ERR
    err = NOP (ncid, num_c4SFWET, "units", 7, " 1/m2/s");
    NC4ERR
    err = NOP (ncid, num_c4SFWET, "long_name", 37, "num_c4 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, num_c4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = num_c4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a1DDF", NC_FLOAT, 2, dimids, &pom_a1DDF);
    NC4ERR
    err = NOP (ncid, pom_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_a1DDF, "long_name", 50,
               "pom_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, pom_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a1SFWET", NC_FLOAT, 2, dimids, &pom_a1SFWET);
    NC4ERR
    err = NOP (ncid, pom_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, pom_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a1_SRF", NC_FLOAT, 2, dimids, &pom_a1_SRF);
    NC4ERR
    err = NOP (ncid, pom_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, pom_a1_SRF, "long_name", 22, "pom_a1 in bottom layer");
    NC4ERR
    err = NOP (ncid, pom_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a1_sfgaex1", NC_FLOAT, 2, dimids, &pom_a1_sfgaex1);
    NC4ERR
    err = NOP (ncid, pom_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_a1_sfgaex1, "long_name", 51,
               "pom_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, pom_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a3DDF", NC_FLOAT, 2, dimids, &pom_a3DDF);
    NC4ERR
    err = NOP (ncid, pom_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_a3DDF, "long_name", 50,
               "pom_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, pom_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a3SFWET", NC_FLOAT, 2, dimids, &pom_a3SFWET);
    NC4ERR
    err = NOP (ncid, pom_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, pom_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a3_SRF", NC_FLOAT, 2, dimids, &pom_a3_SRF);
    NC4ERR
    err = NOP (ncid, pom_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, pom_a3_SRF, "long_name", 22, "pom_a3 in bottom layer");
    NC4ERR
    err = NOP (ncid, pom_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a4DDF", NC_FLOAT, 2, dimids, &pom_a4DDF);
    NC4ERR
    err = NOP (ncid, pom_a4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_a4DDF, "long_name", 50,
               "pom_a4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, pom_a4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a4SFWET", NC_FLOAT, 2, dimids, &pom_a4SFWET);
    NC4ERR
    err = NOP (ncid, pom_a4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_a4SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, pom_a4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a4_CLXF", NC_FLOAT, 2, dimids, &pom_a4_CLXF);
    NC4ERR
    err = NOP (ncid, pom_a4_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = NOP (ncid, pom_a4_CLXF, "long_name", 50,
               "vertically intergrated external forcing for pom_a4");
    NC4ERR
    err = NOP (ncid, pom_a4_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a4_SRF", NC_FLOAT, 2, dimids, &pom_a4_SRF);
    NC4ERR
    err = NOP (ncid, pom_a4_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, pom_a4_SRF, "long_name", 22, "pom_a4 in bottom layer");
    NC4ERR
    err = NOP (ncid, pom_a4_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_a4_sfgaex1", NC_FLOAT, 2, dimids, &pom_a4_sfgaex1);
    NC4ERR
    err = NOP (ncid, pom_a4_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_a4_sfgaex1, "long_name", 51,
               "pom_a4 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, pom_a4_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_a4_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_c1DDF", NC_FLOAT, 2, dimids, &pom_c1DDF);
    NC4ERR
    err = NOP (ncid, pom_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_c1DDF, "long_name", 50,
               "pom_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, pom_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_c1SFWET", NC_FLOAT, 2, dimids, &pom_c1SFWET);
    NC4ERR
    err = NOP (ncid, pom_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_c1SFWET, "long_name", 37, "pom_c1 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, pom_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_c3DDF", NC_FLOAT, 2, dimids, &pom_c3DDF);
    NC4ERR
    err = NOP (ncid, pom_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_c3DDF, "long_name", 50,
               "pom_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, pom_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_c3SFWET", NC_FLOAT, 2, dimids, &pom_c3SFWET);
    NC4ERR
    err = NOP (ncid, pom_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_c3SFWET, "long_name", 37, "pom_c3 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, pom_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_c4DDF", NC_FLOAT, 2, dimids, &pom_c4DDF);
    NC4ERR
    err = NOP (ncid, pom_c4DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_c4DDF, "long_name", 50,
               "pom_c4 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, pom_c4DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c4DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "pom_c4SFWET", NC_FLOAT, 2, dimids, &pom_c4SFWET);
    NC4ERR
    err = NOP (ncid, pom_c4SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, pom_c4SFWET, "long_name", 37, "pom_c4 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, pom_c4SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = pom_c4SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a1DDF", NC_FLOAT, 2, dimids, &so4_a1DDF);
    NC4ERR
    err = NOP (ncid, so4_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_a1DDF, "long_name", 50,
               "so4_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, so4_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a1SFWET", NC_FLOAT, 2, dimids, &so4_a1SFWET);
    NC4ERR
    err = NOP (ncid, so4_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, so4_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a1_CLXF", NC_FLOAT, 2, dimids, &so4_a1_CLXF);
    NC4ERR
    err = NOP (ncid, so4_a1_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = NOP (ncid, so4_a1_CLXF, "long_name", 50,
               "vertically intergrated external forcing for so4_a1");
    NC4ERR
    err = NOP (ncid, so4_a1_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a1_SRF", NC_FLOAT, 2, dimids, &so4_a1_SRF);
    NC4ERR
    err = NOP (ncid, so4_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, so4_a1_SRF, "long_name", 22, "so4_a1 in bottom layer");
    NC4ERR
    err = NOP (ncid, so4_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a1_sfgaex1", NC_FLOAT, 2, dimids, &so4_a1_sfgaex1);
    NC4ERR
    err = NOP (ncid, so4_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_a1_sfgaex1, "long_name", 51,
               "so4_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, so4_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a2DDF", NC_FLOAT, 2, dimids, &so4_a2DDF);
    NC4ERR
    err = NOP (ncid, so4_a2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_a2DDF, "long_name", 50,
               "so4_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, so4_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a2SFWET", NC_FLOAT, 2, dimids, &so4_a2SFWET);
    NC4ERR
    err = NOP (ncid, so4_a2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, so4_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a2_CLXF", NC_FLOAT, 2, dimids, &so4_a2_CLXF);
    NC4ERR
    err = NOP (ncid, so4_a2_CLXF, "units", 11, "molec/cm2/s");
    NC4ERR
    err = NOP (ncid, so4_a2_CLXF, "long_name", 50,
               "vertically intergrated external forcing for so4_a2");
    NC4ERR
    err = NOP (ncid, so4_a2_CLXF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2_CLXF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a2_SRF", NC_FLOAT, 2, dimids, &so4_a2_SRF);
    NC4ERR
    err = NOP (ncid, so4_a2_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, so4_a2_SRF, "long_name", 22, "so4_a2 in bottom layer");
    NC4ERR
    err = NOP (ncid, so4_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a2_sfgaex1", NC_FLOAT, 2, dimids, &so4_a2_sfgaex1);
    NC4ERR
    err = NOP (ncid, so4_a2_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_a2_sfgaex1, "long_name", 51,
               "so4_a2 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, so4_a2_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a2_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a3DDF", NC_FLOAT, 2, dimids, &so4_a3DDF);
    NC4ERR
    err = NOP (ncid, so4_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_a3DDF, "long_name", 50,
               "so4_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, so4_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a3SFWET", NC_FLOAT, 2, dimids, &so4_a3SFWET);
    NC4ERR
    err = NOP (ncid, so4_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, so4_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a3_SRF", NC_FLOAT, 2, dimids, &so4_a3_SRF);
    NC4ERR
    err = NOP (ncid, so4_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, so4_a3_SRF, "long_name", 22, "so4_a3 in bottom layer");
    NC4ERR
    err = NOP (ncid, so4_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_a3_sfgaex1", NC_FLOAT, 2, dimids, &so4_a3_sfgaex1);
    NC4ERR
    err = NOP (ncid, so4_a3_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_a3_sfgaex1, "long_name", 51,
               "so4_a3 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, so4_a3_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_a3_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_c1DDF", NC_FLOAT, 2, dimids, &so4_c1DDF);
    NC4ERR
    err = NOP (ncid, so4_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_c1DDF, "long_name", 50,
               "so4_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, so4_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_c1SFWET", NC_FLOAT, 2, dimids, &so4_c1SFWET);
    NC4ERR
    err = NOP (ncid, so4_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_c1SFWET, "long_name", 37, "so4_c1 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, so4_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_c2DDF", NC_FLOAT, 2, dimids, &so4_c2DDF);
    NC4ERR
    err = NOP (ncid, so4_c2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_c2DDF, "long_name", 50,
               "so4_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, so4_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_c2SFWET", NC_FLOAT, 2, dimids, &so4_c2SFWET);
    NC4ERR
    err = NOP (ncid, so4_c2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_c2SFWET, "long_name", 37, "so4_c2 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, so4_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_c3DDF", NC_FLOAT, 2, dimids, &so4_c3DDF);
    NC4ERR
    err = NOP (ncid, so4_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_c3DDF, "long_name", 50,
               "so4_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, so4_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "so4_c3SFWET", NC_FLOAT, 2, dimids, &so4_c3SFWET);
    NC4ERR
    err = NOP (ncid, so4_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, so4_c3SFWET, "long_name", 37, "so4_c3 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, so4_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = so4_c3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a1DDF", NC_FLOAT, 2, dimids, &soa_a1DDF);
    NC4ERR
    err = NOP (ncid, soa_a1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_a1DDF, "long_name", 50,
               "soa_a1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, soa_a1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a1SFWET", NC_FLOAT, 2, dimids, &soa_a1SFWET);
    NC4ERR
    err = NOP (ncid, soa_a1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_a1SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, soa_a1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a1_SRF", NC_FLOAT, 2, dimids, &soa_a1_SRF);
    NC4ERR
    err = NOP (ncid, soa_a1_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, soa_a1_SRF, "long_name", 22, "soa_a1 in bottom layer");
    NC4ERR
    err = NOP (ncid, soa_a1_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a1_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a1_sfgaex1", NC_FLOAT, 2, dimids, &soa_a1_sfgaex1);
    NC4ERR
    err = NOP (ncid, soa_a1_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_a1_sfgaex1, "long_name", 51,
               "soa_a1 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, soa_a1_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a1_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a2DDF", NC_FLOAT, 2, dimids, &soa_a2DDF);
    NC4ERR
    err = NOP (ncid, soa_a2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_a2DDF, "long_name", 50,
               "soa_a2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, soa_a2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a2SFWET", NC_FLOAT, 2, dimids, &soa_a2SFWET);
    NC4ERR
    err = NOP (ncid, soa_a2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_a2SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, soa_a2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a2_SRF", NC_FLOAT, 2, dimids, &soa_a2_SRF);
    NC4ERR
    err = NOP (ncid, soa_a2_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, soa_a2_SRF, "long_name", 22, "soa_a2 in bottom layer");
    NC4ERR
    err = NOP (ncid, soa_a2_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a2_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a2_sfgaex1", NC_FLOAT, 2, dimids, &soa_a2_sfgaex1);
    NC4ERR
    err = NOP (ncid, soa_a2_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_a2_sfgaex1, "long_name", 51,
               "soa_a2 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, soa_a2_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a2_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a3DDF", NC_FLOAT, 2, dimids, &soa_a3DDF);
    NC4ERR
    err = NOP (ncid, soa_a3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_a3DDF, "long_name", 50,
               "soa_a3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, soa_a3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a3SFWET", NC_FLOAT, 2, dimids, &soa_a3SFWET);
    NC4ERR
    err = NOP (ncid, soa_a3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_a3SFWET, "long_name", 30, "Wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, soa_a3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a3SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a3_SRF", NC_FLOAT, 2, dimids, &soa_a3_SRF);
    NC4ERR
    err = NOP (ncid, soa_a3_SRF, "units", 5, "kg/kg");
    NC4ERR
    err = NOP (ncid, soa_a3_SRF, "long_name", 22, "soa_a3 in bottom layer");
    NC4ERR
    err = NOP (ncid, soa_a3_SRF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a3_SRF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_a3_sfgaex1", NC_FLOAT, 2, dimids, &soa_a3_sfgaex1);
    NC4ERR
    err = NOP (ncid, soa_a3_sfgaex1, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_a3_sfgaex1, "long_name", 51,
               "soa_a3 gas-aerosol-exchange primary column tendency");
    NC4ERR
    err = NOP (ncid, soa_a3_sfgaex1, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_a3_sfgaex1;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_c1DDF", NC_FLOAT, 2, dimids, &soa_c1DDF);
    NC4ERR
    err = NOP (ncid, soa_c1DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_c1DDF, "long_name", 50,
               "soa_c1 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, soa_c1DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c1DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_c1SFWET", NC_FLOAT, 2, dimids, &soa_c1SFWET);
    NC4ERR
    err = NOP (ncid, soa_c1SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_c1SFWET, "long_name", 37, "soa_c1 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, soa_c1SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c1SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_c2DDF", NC_FLOAT, 2, dimids, &soa_c2DDF);
    NC4ERR
    err = NOP (ncid, soa_c2DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_c2DDF, "long_name", 50,
               "soa_c2 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, soa_c2DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c2DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_c2SFWET", NC_FLOAT, 2, dimids, &soa_c2SFWET);
    NC4ERR
    err = NOP (ncid, soa_c2SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_c2SFWET, "long_name", 37, "soa_c2 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, soa_c2SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c2SFWET;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_c3DDF", NC_FLOAT, 2, dimids, &soa_c3DDF);
    NC4ERR
    err = NOP (ncid, soa_c3DDF, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_c3DDF, "long_name", 50,
               "soa_c3 dry deposition flux at bottom (grav + turb)");
    NC4ERR
    err = NOP (ncid, soa_c3DDF, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c3DDF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "soa_c3SFWET", NC_FLOAT, 2, dimids, &soa_c3SFWET);
    NC4ERR
    err = NOP (ncid, soa_c3SFWET, "units", 7, "kg/m2/s");
    NC4ERR
    err = NOP (ncid, soa_c3SFWET, "long_name", 37, "soa_c3 wet deposition flux at surface");
    NC4ERR
    err = NOP (ncid, soa_c3SFWET, "cell_methods", 10, "time: mean");
    NC4ERR
    varids[i++] = soa_c3SFWET;

    assert (i == nvars);

fn_exit:
    return nerrs;
}

/*----< def_F_case_h1() >----------------------------------------------------*/
int def_F_case_h1_nc4 (int ncid,             /* file ID */
                       const size_t dims[2], /* dimension sizes */
                       int nvars,            /* number of variables */
                       int *varids)          /* variable IDs */
{
    /* Total 51 variables */
    int lat, lon, area, lev, hyam, hybm, P0, ilev, hyai, hybi, time, date, datesec, time_bnds,
        date_written, time_written, ndbase, nsbase, nbdate, nbsec, mdt, ndcur, nscur, co2vmr,
        ch4vmr, n2ovmr, f11vmr, f12vmr, sol_tsi, nsteph, CLDHGH, CLDLOW, CLDMED, FLNT, LWCF,
        OMEGA500, OMEGA850, PRECT, PS, SWCF, T850, TMQ, TS, U, U250, U850, UBOT, V250, V850, VBOT,
        Z500;

    int i, err, nerrs = 0, dimids[3], iattr, mdims = 1;
    int dim_ncol, dim_time, dim_nbnd, dim_chars, dim_lev, dim_ilev;

    /* global attributes: */
    iattr = 4;
    err   = nc_put_att (ncid, NC_GLOBAL, "ne", NC_INT, 1, &iattr);
    NC4ERR
    err = nc_put_att (ncid, NC_GLOBAL, "np", NC_INT, 1, &iattr);
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "Conventions", 6, "CF-1.0");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "source", 3, "CAM");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "case", 20, "FC5AV1C-H01B_ne4_ne4");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "title", 5, "UNSET");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "logname", 6, "wkliao");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "host", 10, "compute001");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "Version", 6, "$Name$");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "revision_Id", 4, "$Id$");
    NC4ERR
    err = nc_put_att_text (
        ncid, NC_GLOBAL, "initial_file", 86,
        "/home/climate1/acme/inputdata/atm/cam/inic/homme/cami_mam3_Linoz_ne4np4_L72_c160909.nc");
    NC4ERR
    err = nc_put_att_text (
        ncid, NC_GLOBAL, "topography_file", 79,
        "/home/climate1/acme/inputdata/atm/cam/topo/USGS-gtopo30_ne4np4_16x.c20160612.nc");
    NC4ERR
    err = nc_put_att_text (ncid, NC_GLOBAL, "time_period_freq", 6, "hour_2");
    NC4ERR

    /* define dimensions */
    err = nc_def_dim (ncid, "ncol", dims[1], &dim_ncol);
    NC4ERR
    err = nc_def_dim (ncid, "time", NC_UNLIMITED, &dim_time);
    NC4ERR
    err = nc_def_dim (ncid, "nbnd", 2, &dim_nbnd);
    NC4ERR
    err = nc_def_dim (ncid, "chars", 8, &dim_chars);
    NC4ERR
    err = nc_def_dim (ncid, "lev", dims[0], &dim_lev);
    NC4ERR
    err = nc_def_dim (ncid, "ilev", dims[0] + 1, &dim_ilev);
    NC4ERR

    i = 0;

    /* define variables */
    dimids[0] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "lat", NC_DOUBLE, 1, dimids, &lat);
    NC4ERR
    err = nc_put_att_text (ncid, lat, "long_name", 8, "latitude");
    NC4ERR
    err = nc_put_att_text (ncid, lat, "units", 13, "degrees_north");
    NC4ERR
    varids[i++] = lat;

    dimids[0] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "lon", NC_DOUBLE, 1, dimids, &lon);
    NC4ERR
    err = nc_put_att_text (ncid, lon, "long_name", 9, "longitude");
    NC4ERR
    err = nc_put_att_text (ncid, lon, "units", 12, "degrees_east");
    NC4ERR
    varids[i++] = lon;

    dimids[0] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "area", NC_DOUBLE, 1, dimids, &area);
    NC4ERR
    err = nc_put_att_text (ncid, area, "long_name", 14, "gll grid areas");
    NC4ERR
    varids[i++] = area;

    dimids[0] = dim_lev;
    err       = NC4_DEF_VAR (ncid, "lev", NC_DOUBLE, 1, dimids, &lev);
    NC4ERR
    err = nc_put_att_text (ncid, lev, "long_name", 38, "hybrid level at midpoints (1000*(A+B))");
    NC4ERR
    err = nc_put_att_text (ncid, lev, "units", 3, "hPa");
    NC4ERR
    err = nc_put_att_text (ncid, lev, "positive", 4, "down");
    NC4ERR
    err = nc_put_att_text (ncid, lev, "standard_name", 43,
                           "atmosphere_hybrid_sigma_pressure_coordinate");
    NC4ERR
    err = nc_put_att_text (ncid, lev, "formula_terms", 29, "a: hyam b: hybm p0: P0 ps: PS");
    NC4ERR
    varids[i++] = lev;

    dimids[0] = dim_lev;
    err       = NC4_DEF_VAR (ncid, "hyam", NC_DOUBLE, 1, dimids, &hyam);
    NC4ERR
    err = nc_put_att_text (ncid, hyam, "long_name", 39, "hybrid A coefficient at layer midpoints");
    NC4ERR
    varids[i++] = hyam;

    dimids[0] = dim_lev;
    err       = NC4_DEF_VAR (ncid, "hybm", NC_DOUBLE, 1, dimids, &hybm);
    NC4ERR
    err = nc_put_att_text (ncid, hybm, "long_name", 39, "hybrid B coefficient at layer midpoints");
    NC4ERR
    varids[i++] = hybm;

    dimids[0] = dim_lev;
    err       = NC4_DEF_VAR (ncid, "P0", NC_DOUBLE, 0, NULL, &P0);
    NC4ERR
    err = nc_put_att_text (ncid, P0, "long_name", 18, "reference pressure");
    NC4ERR
    err = nc_put_att_text (ncid, P0, "units", 2, "Pa");
    NC4ERR
    varids[i++] = P0;

    dimids[0] = dim_ilev;
    err       = NC4_DEF_VAR (ncid, "ilev", NC_DOUBLE, 1, dimids, &ilev);
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "long_name", 39, "hybrid level at interfaces (1000*(A+B))");
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "units", 3, "hPa");
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "positive", 4, "down");
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "standard_name", 43,
                           "atmosphere_hybrid_sigma_pressure_coordinate");
    NC4ERR
    err = nc_put_att_text (ncid, ilev, "formula_terms", 29, "a: hyai b: hybi p0: P0 ps: PS");
    NC4ERR
    varids[i++] = ilev;

    dimids[0] = dim_ilev;
    err       = NC4_DEF_VAR (ncid, "hyai", NC_DOUBLE, 1, dimids, &hyai);
    NC4ERR
    err = nc_put_att_text (ncid, hyai, "long_name", 40, "hybrid A coefficient at layer interfaces");
    NC4ERR
    varids[i++] = hyai;

    dimids[0] = dim_ilev;
    err       = NC4_DEF_VAR (ncid, "hybi", NC_DOUBLE, 1, dimids, &hybi);
    NC4ERR
    err = nc_put_att_text (ncid, hybi, "long_name", 40, "hybrid B coefficient at layer interfaces");
    NC4ERR
    varids[i++] = hybi;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "time", NC_DOUBLE, 1, dimids, &time);
    NC4ERR
    err = nc_put_att_text (ncid, time, "long_name", 4, "time");
    NC4ERR
    err = nc_put_att_text (ncid, time, "units", 30, "days since 0001-01-01 00:00:00");
    NC4ERR
    err = nc_put_att_text (ncid, time, "calendar", 6, "noleap");
    NC4ERR
    err = nc_put_att_text (ncid, time, "bounds", 9, "time_bnds");
    NC4ERR
    varids[i++] = time;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "date", NC_INT, 1, dimids, &date);
    NC4ERR
    err = nc_put_att_text (ncid, date, "long_name", 23, "current date (YYYYMMDD)");
    NC4ERR
    varids[i++] = date;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "datesec", NC_INT, 1, dimids, &datesec);
    NC4ERR
    err = nc_put_att_text (ncid, datesec, "long_name", 31, "current seconds of current date");
    NC4ERR
    varids[i++] = datesec;

    dimids[0] = dim_time;
    dimids[1] = dim_nbnd;
    err       = NC4_DEF_VAR (ncid, "time_bnds", NC_DOUBLE, 2, dimids, &time_bnds);
    NC4ERR
    err = nc_put_att_text (ncid, time_bnds, "long_name", 23, "time interval endpoints");
    NC4ERR
    varids[i++] = time_bnds;

    dimids[0] = dim_time;
    dimids[1] = dim_chars;
    err       = NC4_DEF_VAR (ncid, "date_written", NC_CHAR, 2, dimids, &date_written);
    NC4ERR
    varids[i++] = date_written;

    dimids[0] = dim_time;
    dimids[1] = dim_chars;
    err       = NC4_DEF_VAR (ncid, "time_written", NC_CHAR, 2, dimids, &time_written);
    NC4ERR
    varids[i++] = time_written;

    err = NC4_DEF_VAR (ncid, "ndbase", NC_INT, 0, NULL, &ndbase);
    NC4ERR
    err = nc_put_att_text (ncid, ndbase, "long_name", 8, "base day");
    NC4ERR
    varids[i++] = ndbase;
    err         = NC4_DEF_VAR (ncid, "nsbase", NC_INT, 0, NULL, &nsbase);
    NC4ERR
    err = nc_put_att_text (ncid, nsbase, "long_name", 19, "seconds of base day");
    NC4ERR
    varids[i++] = nsbase;

    err = NC4_DEF_VAR (ncid, "nbdate", NC_INT, 0, NULL, &nbdate);
    NC4ERR
    err = nc_put_att_text (ncid, nbdate, "long_name", 20, "base date (YYYYMMDD)");
    NC4ERR
    varids[i++] = nbdate;

    err = NC4_DEF_VAR (ncid, "nbsec", NC_INT, 0, NULL, &nbsec);
    NC4ERR
    err = nc_put_att_text (ncid, nbsec, "long_name", 20, "seconds of base date");
    NC4ERR
    varids[i++] = nbsec;

    err = NC4_DEF_VAR (ncid, "mdt", NC_INT, 0, NULL, &mdt);
    NC4ERR
    err = nc_put_att_text (ncid, mdt, "long_name", 8, "timestep");
    NC4ERR
    err = nc_put_att_text (ncid, mdt, "units", 1, "s");
    NC4ERR
    varids[i++] = mdt;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "ndcur", NC_INT, 1, dimids, &ndcur);
    NC4ERR
    err = nc_put_att_text (ncid, ndcur, "long_name", 27, "current day (from base day)");
    NC4ERR
    varids[i++] = ndcur;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "nscur", NC_INT, 1, dimids, &nscur);
    NC4ERR
    err = nc_put_att_text (ncid, nscur, "long_name", 30, "current seconds of current day");
    NC4ERR
    varids[i++] = nscur;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "co2vmr", NC_DOUBLE, 1, dimids, &co2vmr);
    NC4ERR
    err = nc_put_att_text (ncid, co2vmr, "long_name", 23, "co2 volume mixing ratio");
    NC4ERR
    varids[i++] = co2vmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "ch4vmr", NC_DOUBLE, 1, dimids, &ch4vmr);
    NC4ERR
    err = nc_put_att_text (ncid, ch4vmr, "long_name", 23, "ch4 volume mixing ratio");
    NC4ERR
    varids[i++] = ch4vmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "n2ovmr", NC_DOUBLE, 1, dimids, &n2ovmr);
    NC4ERR
    err = nc_put_att_text (ncid, n2ovmr, "long_name", 23, "n2o volume mixing ratio");
    NC4ERR
    varids[i++] = n2ovmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "f11vmr", NC_DOUBLE, 1, dimids, &f11vmr);
    NC4ERR
    err = nc_put_att_text (ncid, f11vmr, "long_name", 23, "f11 volume mixing ratio");
    NC4ERR
    varids[i++] = f11vmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "f12vmr", NC_DOUBLE, 1, dimids, &f12vmr);
    NC4ERR
    err = nc_put_att_text (ncid, f12vmr, "long_name", 23, "f12 volume mixing ratio");
    NC4ERR
    varids[i++] = f12vmr;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "sol_tsi", NC_DOUBLE, 1, dimids, &sol_tsi);
    NC4ERR
    err = nc_put_att_text (ncid, sol_tsi, "long_name", 22, "total solar irradiance");
    NC4ERR
    err = nc_put_att_text (ncid, sol_tsi, "units", 4, "W/m2");
    NC4ERR
    varids[i++] = sol_tsi;

    dimids[0] = dim_time;
    err       = NC4_DEF_VAR (ncid, "nsteph", NC_INT, 1, dimids, &nsteph);
    NC4ERR
    err = nc_put_att_text (ncid, nsteph, "long_name", 16, "current timestep");
    NC4ERR
    varids[i++] = nsteph;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLDHGH", NC_FLOAT, 2, dimids, &CLDHGH);
    NC4ERR
    err = nc_put_att_text (ncid, CLDHGH, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLDHGH, "long_name", 32, "Vertically-integrated high cloud");
    NC4ERR
    varids[i++] = CLDHGH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLDLOW", NC_FLOAT, 2, dimids, &CLDLOW);
    NC4ERR
    err = nc_put_att_text (ncid, CLDLOW, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLDLOW, "long_name", 31, "Vertically-integrated low cloud");
    NC4ERR
    varids[i++] = CLDLOW;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "CLDMED", NC_FLOAT, 2, dimids, &CLDMED);
    NC4ERR
    err = nc_put_att_text (ncid, CLDMED, "units", 8, "fraction");
    NC4ERR
    err = nc_put_att_text (ncid, CLDMED, "long_name", 37, "Vertically-integrated mid-level cloud");
    NC4ERR
    varids[i++] = CLDMED;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "FLNT", NC_FLOAT, 2, dimids, &FLNT);
    NC4ERR
    err = nc_put_att_text (ncid, FLNT, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, FLNT, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, FLNT, "long_name", 33, "Net longwave flux at top of model");
    NC4ERR
    varids[i++] = FLNT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "LWCF", NC_FLOAT, 2, dimids, &LWCF);
    NC4ERR
    err = nc_put_att_text (ncid, LWCF, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, LWCF, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, LWCF, "long_name", 22, "Longwave cloud forcing");
    NC4ERR
    varids[i++] = LWCF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "OMEGA500", NC_FLOAT, 2, dimids, &OMEGA500);
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA500, "units", 4, "Pa/s");
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA500, "long_name", 46,
                           "Vertical velocity at 500 mbar pressure surface");
    NC4ERR
    varids[i++] = OMEGA500;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "OMEGA850", NC_FLOAT, 2, dimids, &OMEGA850);
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA850, "units", 4, "Pa/s");
    NC4ERR
    err = nc_put_att_text (ncid, OMEGA850, "long_name", 46,
                           "Vertical velocity at 850 mbar pressure surface");
    NC4ERR
    varids[i++] = OMEGA850;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PRECT", NC_FLOAT, 2, dimids, &PRECT);
    NC4ERR
    err = nc_put_att_text (ncid, PRECT, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, PRECT, "long_name", 65,
                           "Total (convective and large-scale) precipitation rate (liq + ice)");
    NC4ERR
    varids[i++] = PRECT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "PS", NC_FLOAT, 2, dimids, &PS);
    NC4ERR
    err = nc_put_att_text (ncid, PS, "units", 2, "Pa");
    NC4ERR
    err = nc_put_att_text (ncid, PS, "long_name", 16, "Surface pressure");
    NC4ERR
    varids[i++] = PS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "SWCF", NC_FLOAT, 2, dimids, &SWCF);
    NC4ERR
    err = nc_put_att_text (ncid, SWCF, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = nc_put_att_text (ncid, SWCF, "units", 4, "W/m2");
    NC4ERR
    err = nc_put_att_text (ncid, SWCF, "long_name", 23, "Shortwave cloud forcing");
    NC4ERR
    varids[i++] = SWCF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "T850", NC_FLOAT, 2, dimids, &T850);
    NC4ERR
    err = nc_put_att_text (ncid, T850, "units", 1, "K");
    NC4ERR
    err = nc_put_att_text (ncid, T850, "long_name", 40, "Temperature at 850 mbar pressure surface");
    NC4ERR
    varids[i++] = T850;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TMQ", NC_FLOAT, 2, dimids, &TMQ);
    NC4ERR
    err = nc_put_att_text (ncid, TMQ, "units", 5, "kg/m2");
    NC4ERR
    err = nc_put_att_text (ncid, TMQ, "long_name", 48,
                           "Total (vertically integrated) precipitable water");
    NC4ERR
    varids[i++] = TMQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "TS", NC_FLOAT, 2, dimids, &TS);
    NC4ERR
    err = nc_put_att_text (ncid, TS, "units", 1, "K");
    NC4ERR
    err = nc_put_att_text (ncid, TS, "long_name", 31, "Surface temperature (radiative)");
    NC4ERR
    varids[i++] = TS;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "U", NC_FLOAT, 3, dimids, &U);
    NC4ERR
    err = nc_put_att_int (ncid, U, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = nc_put_att_text (ncid, U, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, U, "long_name", 10, "Zonal wind");
    NC4ERR
    varids[i++] = U;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "U250", NC_FLOAT, 2, dimids, &U250);
    NC4ERR
    err = nc_put_att_text (ncid, U250, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, U250, "long_name", 39, "Zonal wind at 250 mbar pressure surface");
    NC4ERR
    varids[i++] = U250;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "U850", NC_FLOAT, 2, dimids, &U850);
    NC4ERR
    err = nc_put_att_text (ncid, U850, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, U850, "long_name", 39, "Zonal wind at 850 mbar pressure surface");
    NC4ERR
    varids[i++] = U850;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "UBOT", NC_FLOAT, 2, dimids, &UBOT);
    NC4ERR
    err = nc_put_att_text (ncid, UBOT, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, UBOT, "long_name", 29, "Lowest model level zonal wind");
    NC4ERR
    varids[i++] = UBOT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "V250", NC_FLOAT, 2, dimids, &V250);
    NC4ERR
    err = nc_put_att_text (ncid, V250, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, V250, "long_name", 44,
                           "Meridional wind at 250 mbar pressure surface");
    NC4ERR
    varids[i++] = V250;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "V850", NC_FLOAT, 2, dimids, &V850);
    NC4ERR
    err = nc_put_att_text (ncid, V850, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, V850, "long_name", 44,
                           "Meridional wind at 850 mbar pressure surface");
    NC4ERR
    varids[i++] = V850;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "VBOT", NC_FLOAT, 2, dimids, &VBOT);
    NC4ERR
    err = nc_put_att_text (ncid, VBOT, "units", 3, "m/s");
    NC4ERR
    err = nc_put_att_text (ncid, VBOT, "long_name", 34, "Lowest model level meridional wind");
    NC4ERR
    varids[i++] = VBOT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = NC4_DEF_VAR (ncid, "Z500", NC_FLOAT, 2, dimids, &Z500);
    NC4ERR
    err = nc_put_att_text (ncid, Z500, "units", 1, "m");
    NC4ERR
    err = nc_put_att_text (ncid, Z500, "long_name", 43,
                           "Geopotential Z at 500 mbar pressure surface");
    NC4ERR
    varids[i++] = Z500;

    assert (i == nvars);

fn_exit:
    return nerrs;
}

/*----< inq_F_case_h1() >----------------------------------------------------*/
int inq_F_case_h1_nc4 (int ncid,       /* file ID */
                       size_t dims[2], /* dimension sizes */
                       int nvars,      /* number of variables */
                       int *varids)    /* variable IDs */
{
    /* Total 51 variables */
    int lat, lon, area, lev, hyam, hybm, P0, ilev, hyai, hybi, time, date, datesec, time_bnds,
        date_written, time_written, ndbase, nsbase, nbdate, nbsec, mdt, ndcur, nscur, co2vmr,
        ch4vmr, n2ovmr, f11vmr, f12vmr, sol_tsi, nsteph, CLDHGH, CLDLOW, CLDMED, FLNT, LWCF,
        OMEGA500, OMEGA850, PRECT, PS, SWCF, T850, TMQ, TS, U, U250, U850, UBOT, V250, V850, VBOT,
        Z500;

    int i, err, nerrs = 0, dimids[3], iattr, mdims = 1;
    int dim_ncol, dim_time, dim_nbnd, dim_chars, dim_lev, dim_ilev;

    /* global attributes: */
    iattr = 4;
    err   = NOP2 (ncid, NC_GLOBAL, "ne", NC_INT, 1, &iattr);
    NC4ERR
    err = NOP2 (ncid, NC_GLOBAL, "np", NC_INT, 1, &iattr);
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "Conventions", 6, "CF-1.0");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "source", 3, "CAM");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "case", 20, "FC5AV1C-H01B_ne4_ne4");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "title", 5, "UNSET");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "logname", 6, "wkliao");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "host", 10, "compute001");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "Version", 6, "$Name$");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "revision_Id", 4, "$Id$");
    NC4ERR
    err = NOP (
        ncid, NC_GLOBAL, "initial_file", 86,
        "/home/climate1/acme/inputdata/atm/cam/inic/homme/cami_mam3_Linoz_ne4np4_L72_c160909.nc");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "topography_file", 79,
               "/home/climate1/acme/inputdata/atm/cam/topo/USGS-gtopo30_ne4np4_16x.c20160612.nc");
    NC4ERR
    err = NOP (ncid, NC_GLOBAL, "time_period_freq", 6, "hour_2");
    NC4ERR

    /* inquery dimensions */
    err = nc_inq_dimid (ncid, "ncol", &dim_ncol);
    NC4ERR
    // err = nc_inq_dimid(ncid, "time", &dim_time); NC4ERR
    // err = nc_inq_dimid(ncid, "nbnd", &dim_nbnd); NC4ERR
    // err = nc_inq_dimid(ncid, "chars", &dim_chars); NC4ERR
    err = nc_inq_dimid (ncid, "lev", &dim_lev);
    NC4ERR
    // err = nc_inq_dimid(ncid, "ilev", &dim_ilev); NC4ERR

    err = nc_inq_dimlen (ncid, dim_ncol, dims + 1);
    NC4ERR
    err = nc_inq_dimlen (ncid, dim_lev, dims);
    NC4ERR
    /*
    err = nc_def_dim(ncid, "ncol", dims[1],      &dim_ncol); NC4ERR
    err = nc_def_dim(ncid, "time", NC_UNLIMITED, &dim_time); NC4ERR
    err = nc_def_dim(ncid, "nbnd",  2,           &dim_nbnd); NC4ERR
    err = nc_def_dim(ncid, "chars", 8,           &dim_chars); NC4ERR
    err = nc_def_dim(ncid, "lev",   dims[0],     &dim_lev); NC4ERR
    err = nc_def_dim(ncid, "ilev",  dims[0]+1,   &dim_ilev); NC4ERR
    */

    i = 0;

    /* define variables */
    dimids[0] = dim_ncol;
    err       = INQ_VID (ncid, "lat", NC_DOUBLE, 1, dimids, &lat);
    NC4ERR
    err = NOP (ncid, lat, "long_name", 8, "latitude");
    NC4ERR
    err = NOP (ncid, lat, "units", 13, "degrees_north");
    NC4ERR
    varids[i++] = lat;

    dimids[0] = dim_ncol;
    err       = INQ_VID (ncid, "lon", NC_DOUBLE, 1, dimids, &lon);
    NC4ERR
    err = NOP (ncid, lon, "long_name", 9, "longitude");
    NC4ERR
    err = NOP (ncid, lon, "units", 12, "degrees_east");
    NC4ERR
    varids[i++] = lon;

    dimids[0] = dim_ncol;
    err       = INQ_VID (ncid, "area", NC_DOUBLE, 1, dimids, &area);
    NC4ERR
    err = NOP (ncid, area, "long_name", 14, "gll grid areas");
    NC4ERR
    varids[i++] = area;

    dimids[0] = dim_lev;
    err       = INQ_VID (ncid, "lev", NC_DOUBLE, 1, dimids, &lev);
    NC4ERR
    err = NOP (ncid, lev, "long_name", 38, "hybrid level at midpoints (1000*(A+B))");
    NC4ERR
    err = NOP (ncid, lev, "units", 3, "hPa");
    NC4ERR
    err = NOP (ncid, lev, "positive", 4, "down");
    NC4ERR
    err = NOP (ncid, lev, "standard_name", 43, "atmosphere_hybrid_sigma_pressure_coordinate");
    NC4ERR
    err = NOP (ncid, lev, "formula_terms", 29, "a: hyam b: hybm p0: P0 ps: PS");
    NC4ERR
    varids[i++] = lev;

    dimids[0] = dim_lev;
    err       = INQ_VID (ncid, "hyam", NC_DOUBLE, 1, dimids, &hyam);
    NC4ERR
    err = NOP (ncid, hyam, "long_name", 39, "hybrid A coefficient at layer midpoints");
    NC4ERR
    varids[i++] = hyam;

    dimids[0] = dim_lev;
    err       = INQ_VID (ncid, "hybm", NC_DOUBLE, 1, dimids, &hybm);
    NC4ERR
    err = NOP (ncid, hybm, "long_name", 39, "hybrid B coefficient at layer midpoints");
    NC4ERR
    varids[i++] = hybm;

    dimids[0] = dim_lev;
    err       = INQ_VID (ncid, "P0", NC_DOUBLE, 0, NULL, &P0);
    NC4ERR
    err = NOP (ncid, P0, "long_name", 18, "reference pressure");
    NC4ERR
    err = NOP (ncid, P0, "units", 2, "Pa");
    NC4ERR
    varids[i++] = P0;

    dimids[0] = dim_ilev;
    err       = INQ_VID (ncid, "ilev", NC_DOUBLE, 1, dimids, &ilev);
    NC4ERR
    err = NOP (ncid, ilev, "long_name", 39, "hybrid level at interfaces (1000*(A+B))");
    NC4ERR
    err = NOP (ncid, ilev, "units", 3, "hPa");
    NC4ERR
    err = NOP (ncid, ilev, "positive", 4, "down");
    NC4ERR
    err = NOP (ncid, ilev, "standard_name", 43, "atmosphere_hybrid_sigma_pressure_coordinate");
    NC4ERR
    err = NOP (ncid, ilev, "formula_terms", 29, "a: hyai b: hybi p0: P0 ps: PS");
    NC4ERR
    varids[i++] = ilev;

    dimids[0] = dim_ilev;
    err       = INQ_VID (ncid, "hyai", NC_DOUBLE, 1, dimids, &hyai);
    NC4ERR
    err = NOP (ncid, hyai, "long_name", 40, "hybrid A coefficient at layer interfaces");
    NC4ERR
    varids[i++] = hyai;

    dimids[0] = dim_ilev;
    err       = INQ_VID (ncid, "hybi", NC_DOUBLE, 1, dimids, &hybi);
    NC4ERR
    err = NOP (ncid, hybi, "long_name", 40, "hybrid B coefficient at layer interfaces");
    NC4ERR
    varids[i++] = hybi;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "time", NC_DOUBLE, 1, dimids, &time);
    NC4ERR
    err = NOP (ncid, time, "long_name", 4, "time");
    NC4ERR
    err = NOP (ncid, time, "units", 30, "days since 0001-01-01 00:00:00");
    NC4ERR
    err = NOP (ncid, time, "calendar", 6, "noleap");
    NC4ERR
    err = NOP (ncid, time, "bounds", 9, "time_bnds");
    NC4ERR
    varids[i++] = time;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "date", NC_INT, 1, dimids, &date);
    NC4ERR
    err = NOP (ncid, date, "long_name", 23, "current date (YYYYMMDD)");
    NC4ERR
    varids[i++] = date;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "datesec", NC_INT, 1, dimids, &datesec);
    NC4ERR
    err = NOP (ncid, datesec, "long_name", 31, "current seconds of current date");
    NC4ERR
    varids[i++] = datesec;

    dimids[0] = dim_time;
    dimids[1] = dim_nbnd;
    err       = INQ_VID (ncid, "time_bnds", NC_DOUBLE, 2, dimids, &time_bnds);
    NC4ERR
    err = NOP (ncid, time_bnds, "long_name", 23, "time interval endpoints");
    NC4ERR
    varids[i++] = time_bnds;

    dimids[0] = dim_time;
    dimids[1] = dim_chars;
    err       = INQ_VID (ncid, "date_written", NC_CHAR, 2, dimids, &date_written);
    NC4ERR
    varids[i++] = date_written;

    dimids[0] = dim_time;
    dimids[1] = dim_chars;
    err       = INQ_VID (ncid, "time_written", NC_CHAR, 2, dimids, &time_written);
    NC4ERR
    varids[i++] = time_written;

    err = INQ_VID (ncid, "ndbase", NC_INT, 0, NULL, &ndbase);
    NC4ERR
    err = NOP (ncid, ndbase, "long_name", 8, "base day");
    NC4ERR
    varids[i++] = ndbase;
    err         = INQ_VID (ncid, "nsbase", NC_INT, 0, NULL, &nsbase);
    NC4ERR
    err = NOP (ncid, nsbase, "long_name", 19, "seconds of base day");
    NC4ERR
    varids[i++] = nsbase;

    err = INQ_VID (ncid, "nbdate", NC_INT, 0, NULL, &nbdate);
    NC4ERR
    err = NOP (ncid, nbdate, "long_name", 20, "base date (YYYYMMDD)");
    NC4ERR
    varids[i++] = nbdate;

    err = INQ_VID (ncid, "nbsec", NC_INT, 0, NULL, &nbsec);
    NC4ERR
    err = NOP (ncid, nbsec, "long_name", 20, "seconds of base date");
    NC4ERR
    varids[i++] = nbsec;

    err = INQ_VID (ncid, "mdt", NC_INT, 0, NULL, &mdt);
    NC4ERR
    err = NOP (ncid, mdt, "long_name", 8, "timestep");
    NC4ERR
    err = NOP (ncid, mdt, "units", 1, "s");
    NC4ERR
    varids[i++] = mdt;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "ndcur", NC_INT, 1, dimids, &ndcur);
    NC4ERR
    err = NOP (ncid, ndcur, "long_name", 27, "current day (from base day)");
    NC4ERR
    varids[i++] = ndcur;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "nscur", NC_INT, 1, dimids, &nscur);
    NC4ERR
    err = NOP (ncid, nscur, "long_name", 30, "current seconds of current day");
    NC4ERR
    varids[i++] = nscur;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "co2vmr", NC_DOUBLE, 1, dimids, &co2vmr);
    NC4ERR
    err = NOP (ncid, co2vmr, "long_name", 23, "co2 volume mixing ratio");
    NC4ERR
    varids[i++] = co2vmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "ch4vmr", NC_DOUBLE, 1, dimids, &ch4vmr);
    NC4ERR
    err = NOP (ncid, ch4vmr, "long_name", 23, "ch4 volume mixing ratio");
    NC4ERR
    varids[i++] = ch4vmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "n2ovmr", NC_DOUBLE, 1, dimids, &n2ovmr);
    NC4ERR
    err = NOP (ncid, n2ovmr, "long_name", 23, "n2o volume mixing ratio");
    NC4ERR
    varids[i++] = n2ovmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "f11vmr", NC_DOUBLE, 1, dimids, &f11vmr);
    NC4ERR
    err = NOP (ncid, f11vmr, "long_name", 23, "f11 volume mixing ratio");
    NC4ERR
    varids[i++] = f11vmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "f12vmr", NC_DOUBLE, 1, dimids, &f12vmr);
    NC4ERR
    err = NOP (ncid, f12vmr, "long_name", 23, "f12 volume mixing ratio");
    NC4ERR
    varids[i++] = f12vmr;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "sol_tsi", NC_DOUBLE, 1, dimids, &sol_tsi);
    NC4ERR
    err = NOP (ncid, sol_tsi, "long_name", 22, "total solar irradiance");
    NC4ERR
    err = NOP (ncid, sol_tsi, "units", 4, "W/m2");
    NC4ERR
    varids[i++] = sol_tsi;

    dimids[0] = dim_time;
    err       = INQ_VID (ncid, "nsteph", NC_INT, 1, dimids, &nsteph);
    NC4ERR
    err = NOP (ncid, nsteph, "long_name", 16, "current timestep");
    NC4ERR
    varids[i++] = nsteph;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "CLDHGH", NC_FLOAT, 2, dimids, &CLDHGH);
    NC4ERR
    err = NOP (ncid, CLDHGH, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CLDHGH, "long_name", 32, "Vertically-integrated high cloud");
    NC4ERR
    varids[i++] = CLDHGH;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "CLDLOW", NC_FLOAT, 2, dimids, &CLDLOW);
    NC4ERR
    err = NOP (ncid, CLDLOW, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CLDLOW, "long_name", 31, "Vertically-integrated low cloud");
    NC4ERR
    varids[i++] = CLDLOW;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "CLDMED", NC_FLOAT, 2, dimids, &CLDMED);
    NC4ERR
    err = NOP (ncid, CLDMED, "units", 8, "fraction");
    NC4ERR
    err = NOP (ncid, CLDMED, "long_name", 37, "Vertically-integrated mid-level cloud");
    NC4ERR
    varids[i++] = CLDMED;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "FLNT", NC_FLOAT, 2, dimids, &FLNT);
    NC4ERR
    err = NOP (ncid, FLNT, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, FLNT, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, FLNT, "long_name", 33, "Net longwave flux at top of model");
    NC4ERR
    varids[i++] = FLNT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "LWCF", NC_FLOAT, 2, dimids, &LWCF);
    NC4ERR
    err = NOP (ncid, LWCF, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, LWCF, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, LWCF, "long_name", 22, "Longwave cloud forcing");
    NC4ERR
    varids[i++] = LWCF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "OMEGA500", NC_FLOAT, 2, dimids, &OMEGA500);
    NC4ERR
    err = NOP (ncid, OMEGA500, "units", 4, "Pa/s");
    NC4ERR
    err = NOP (ncid, OMEGA500, "long_name", 46, "Vertical velocity at 500 mbar pressure surface");
    NC4ERR
    varids[i++] = OMEGA500;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "OMEGA850", NC_FLOAT, 2, dimids, &OMEGA850);
    NC4ERR
    err = NOP (ncid, OMEGA850, "units", 4, "Pa/s");
    NC4ERR
    err = NOP (ncid, OMEGA850, "long_name", 46, "Vertical velocity at 850 mbar pressure surface");
    NC4ERR
    varids[i++] = OMEGA850;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PRECT", NC_FLOAT, 2, dimids, &PRECT);
    NC4ERR
    err = NOP (ncid, PRECT, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, PRECT, "long_name", 65,
               "Total (convective and large-scale) precipitation rate (liq + ice)");
    NC4ERR
    varids[i++] = PRECT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "PS", NC_FLOAT, 2, dimids, &PS);
    NC4ERR
    err = NOP (ncid, PS, "units", 2, "Pa");
    NC4ERR
    err = NOP (ncid, PS, "long_name", 16, "Surface pressure");
    NC4ERR
    varids[i++] = PS;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "SWCF", NC_FLOAT, 2, dimids, &SWCF);
    NC4ERR
    err = NOP (ncid, SWCF, "Sampling_Sequence", 8, "rad_lwsw");
    NC4ERR
    err = NOP (ncid, SWCF, "units", 4, "W/m2");
    NC4ERR
    err = NOP (ncid, SWCF, "long_name", 23, "Shortwave cloud forcing");
    NC4ERR
    varids[i++] = SWCF;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "T850", NC_FLOAT, 2, dimids, &T850);
    NC4ERR
    err = NOP (ncid, T850, "units", 1, "K");
    NC4ERR
    err = NOP (ncid, T850, "long_name", 40, "Temperature at 850 mbar pressure surface");
    NC4ERR
    varids[i++] = T850;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TMQ", NC_FLOAT, 2, dimids, &TMQ);
    NC4ERR
    err = NOP (ncid, TMQ, "units", 5, "kg/m2");
    NC4ERR
    err = NOP (ncid, TMQ, "long_name", 48, "Total (vertically integrated) precipitable water");
    NC4ERR
    varids[i++] = TMQ;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "TS", NC_FLOAT, 2, dimids, &TS);
    NC4ERR
    err = NOP (ncid, TS, "units", 1, "K");
    NC4ERR
    err = NOP (ncid, TS, "long_name", 31, "Surface temperature (radiative)");
    NC4ERR
    varids[i++] = TS;

    dimids[0] = dim_time;
    dimids[1] = dim_lev;
    dimids[2] = dim_ncol;
    err       = INQ_VID (ncid, "U", NC_FLOAT, 3, dimids, &U);
    NC4ERR
    err = NOP2 (ncid, U, "mdims", NC_INT, 1, &mdims);
    NC4ERR
    err = NOP (ncid, U, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, U, "long_name", 10, "Zonal wind");
    NC4ERR
    varids[i++] = U;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "U250", NC_FLOAT, 2, dimids, &U250);
    NC4ERR
    err = NOP (ncid, U250, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, U250, "long_name", 39, "Zonal wind at 250 mbar pressure surface");
    NC4ERR
    varids[i++] = U250;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "U850", NC_FLOAT, 2, dimids, &U850);
    NC4ERR
    err = NOP (ncid, U850, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, U850, "long_name", 39, "Zonal wind at 850 mbar pressure surface");
    NC4ERR
    varids[i++] = U850;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "UBOT", NC_FLOAT, 2, dimids, &UBOT);
    NC4ERR
    err = NOP (ncid, UBOT, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, UBOT, "long_name", 29, "Lowest model level zonal wind");
    NC4ERR
    varids[i++] = UBOT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "V250", NC_FLOAT, 2, dimids, &V250);
    NC4ERR
    err = NOP (ncid, V250, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, V250, "long_name", 44, "Meridional wind at 250 mbar pressure surface");
    NC4ERR
    varids[i++] = V250;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "V850", NC_FLOAT, 2, dimids, &V850);
    NC4ERR
    err = NOP (ncid, V850, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, V850, "long_name", 44, "Meridional wind at 850 mbar pressure surface");
    NC4ERR
    varids[i++] = V850;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "VBOT", NC_FLOAT, 2, dimids, &VBOT);
    NC4ERR
    err = NOP (ncid, VBOT, "units", 3, "m/s");
    NC4ERR
    err = NOP (ncid, VBOT, "long_name", 34, "Lowest model level meridional wind");
    NC4ERR
    varids[i++] = VBOT;

    dimids[0] = dim_time;
    dimids[1] = dim_ncol;
    err       = INQ_VID (ncid, "Z500", NC_FLOAT, 2, dimids, &Z500);
    NC4ERR
    err = NOP (ncid, Z500, "units", 1, "m");
    NC4ERR
    err = NOP (ncid, Z500, "long_name", 43, "Geopotential Z at 500 mbar pressure surface");
    NC4ERR
    varids[i++] = Z500;

    assert (i == nvars);

fn_exit:
    return nerrs;
}