#pragma once

#include "e3sm_io.h"
#include "e3sm_io_driver.hpp"

class e3sm_io_case {
   public:
    virtual ~e3sm_io_case () {};
    virtual int wr_test (e3sm_io_config &cfg, e3sm_io_decom &decom, e3sm_io_driver &driver)   = 0;
    virtual int rd_test (e3sm_io_config &cfg, e3sm_io_decom &decom, e3sm_io_driver &driver)   = 0;
    virtual int load_data (e3sm_io_config &cfg, e3sm_io_decom &decom, e3sm_io_driver &driver) = 0;
};

class e3sm_io_case_F : public e3sm_io_case {
    double *dbl_buf_h0 = NULL, *dbl_buf_h1 = NULL;
    itype *rec_buf_h0 = NULL, *rec_buf_h1 = NULL;
    char txt_buf[2][16];
    int int_buf[2][10];

   public:
    e3sm_io_case_F ();
    ~e3sm_io_case_F ();
    int load_data (e3sm_io_config &cfg, e3sm_io_decom &decom, e3sm_io_driver &driver);
    int wr_test (e3sm_io_config &cfg, e3sm_io_decom &decom, e3sm_io_driver &driver);
    int rd_test (e3sm_io_config &cfg, e3sm_io_decom &decom, e3sm_io_driver &driver);
};

class e3sm_io_case_G : public e3sm_io_case {
    double *D1_rec_dbl_buf = NULL, *D3_rec_dbl_buf = NULL, *D4_rec_dbl_buf = NULL,
           *D5_rec_dbl_buf = NULL, *D6_rec_dbl_buf = NULL, *D1_fix_dbl_buf = NULL;
    int *D1_fix_int_buf = NULL, *D2_fix_int_buf = NULL, *D3_fix_int_buf = NULL,
        *D4_fix_int_buf = NULL, *D5_fix_int_buf = NULL;

   public:
    e3sm_io_case_G ();
    ~e3sm_io_case_G ();
    int load_data (e3sm_io_config &cfg, e3sm_io_decom &decom, e3sm_io_driver &driver);
    int wr_test (e3sm_io_config &cfg, e3sm_io_decom &decom, e3sm_io_driver &driver);
    int rd_test (e3sm_io_config &cfg, e3sm_io_decom &decom, e3sm_io_driver &driver);
};