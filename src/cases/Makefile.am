#
# Copyright (C) 2021, Northwestern University and Argonne National Laboratory
# See COPYRIGHT notice in top-level directory.
#

SUFFIXES = .a .o .c .m4 .h .cpp .hpp

AM_CPPFLAGS  = -I${top_srcdir}/src
AM_CPPFLAGS += -I${top_srcdir}/src/drivers
AM_CPPFLAGS += -I${top_srcdir}/src/cases

noinst_LIBRARIES = libe3sm_io_cases.a

CXX_SRCS =  e3sm_io_case.cpp \
            header_def_F_case.cpp \
            header_def_G_case.cpp \
            header_def_I_case.cpp \
            header_inq_F_case.cpp \
            header_inq_G_case.cpp \
            var_rd_F_case.cpp \
            var_rd_G_case.cpp \
            var_wr_case.cpp \
            scorpio_wrapper.cpp \
            report_timing.cpp \
            utils.cpp

H_SRCS =    e3sm_io_case.hpp

libe3sm_io_cases_a_SOURCES = $(CXX_SRCS) $(H_SRCS)

CLEANFILES = core core.* *.gcda *.gcno *.gcov gmon.out

