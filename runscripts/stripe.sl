#!/bin/bash
#SBATCH -q regular
#SBATCH -N 21
#SBATCH -C knl,quad,cache
#SBATCH -t 00:20:00
#SBATCH -o IDBG_logvl86abd_e3sm5e3aa_hdfc0f2b_pncc9b6b_adios294c2e_cori_stripe.txt
#SBATCH -e IDBG_logvl86abd_e3sm5e3aa_hdfc0f2b_pncc9b6b_adios294c2e_cori_stripe.err
#SBATCH -L SCRATCH
#SBATCH -A m2956

E3SM_IO_DATE="1634575447"
HDF5_LIB_PATH=/global/homes/k/khl7265/.local/hdf5/develop
HDF5_LIB_DATE="1632083763"
ADIOS2_LIB_PATH=/global/homes/k/khl7265/.local/adios2/2.7.1
ADIOS2_LIB_DATE="1625028628"
LOGVOL_LIB_PATH=/global/homes/k/khl7265/.local/log_io_vol/profiling
LOGVOL_LIB_DATE="1634539163"
PNC_LIB_PATH=/global/homes/k/khl7265/.local/pnetcdf/master

export LD_LIBRARY_PATH=${HDF5_LIB_PATH}/lib:${PNC_LIB_PATH}/lib:${ADIOS2_LIB_PATH}/lib64:${LOGVOL_LIB_PATH}/lib:${LD_LIBRARY_PATH}
export PNETCDF_SHOW_PERFORMANCE_INFO=1
#export PNETCDF_DEFAULT_CHUNK_DIM="ncol : 14563 ; nbnd : 2 ; Time : 1 ; ilev : 73 ; lev : 72 ; chars : 64 ;nCells : 16384 ; nEdges : 16384 ; nVertices : 16384 ; nVertLevelsP1 : 81 ; nVertLevels : 80 ; StrLen : 64 ;"
export PNETCDF_HINTS="nc_zip_delay_init=1;nc_zip_nrec=1;nc_zip_buffer_size=0"

export H5VL_LOG_METADATA_SHARE=1
export H5VL_LOG_METADATA_MERGE=0
export H5VL_LOG_METADATA_ZIP=1
export H5VL_LOG_SEL_ENCODING=1
export H5VL_LOG_SUBFILING=1
export E3SM_IO_HDF5_USE_LOGVOL_WRITEN=1
export H5VL_LOG_SHOW_PROFILING_INFO=1
export H5VL_LOG_PRINT_MPI_INFO=1

FSS=(FS_1_1M FS_1_16M FS_8_1M FS_8_16M)
NN=${SLURM_NNODES}
let NP=NN*64
let NR=240

ulimit -c unlimited

for FS in ${FSS[@]}
do
    echo "mkdir -p /global/cscratch1/sd/khl7265/${FS}/E3SM/hdf5_log/log/I1850GSWCNPRDCTCBC_hcru_hcru_1344p_raw"
    mkdir -p /global/cscratch1/sd/khl7265/${FS}/E3SM/hdf5_log/log/I1850GSWCNPRDCTCBC_hcru_hcru_1344p_raw
done

TSTARTTIME=`date +%s.%N`
for i in $(seq 1);
do
    for FS in ${FSS[@]}
    do
        OUTDIR=/global/cscratch1/sd/khl7265/${FS}/E3SM/hdf5_log/log/I1850GSWCNPRDCTCBC_hcru_hcru_1344p_raw/
        
        echo "========================== ${FS} =========================="
        >&2 echo "========================== ${FS} =========================="

        echo "unset E3SM_IO_HINTS"
        unset E3SM_IO_HINTS
        
        echo "rm -rf ${OUTDIR}/*"
        rm -rf ${OUTDIR}/*

        echo "#%$=: exp: e3sm_io"
        echo "#%$=: runs: ${i}"
        echo "#%$=: h_num: -1"
        echo "#%$=: nrec: ${NR}"
        echo "#%$=: api: hdf5_log"
        echo "#%$=: strategy: log"
        echo "#%$=: cb_nodes: default"
        echo "#%$=: flush_freq: ${NR}"
        echo "#%$=: number_of_nodes: ${NN}"
        echo "#%$=: number_of_proc: ${NP}"

        STARTTIME=`date +%s.%N`

        echo "srun -n ${NP} -t 6 --cpu_bind=cores  ./../src/e3sm_io -g 1 -k -o ${OUTDIR}/testfile -a hdf5_log -f -1 -r ${NR} -x log -p -y ${NR} /global/cscratch1/sd/khl7265/FS_64_1M/E3SM/decom/I1850GSWCNPRDCTCBC_hcru_hcru_1344p_raw.nc"
        srun -n ${NP} -t 6 --cpu_bind=cores  ./../src/e3sm_io -g 1 -k -o ${OUTDIR}/testfile -a hdf5_log -f -1 -r ${NR} -x log -p -y ${NR} /global/cscratch1/sd/khl7265/FS_64_1M/E3SM/decom/I1850GSWCNPRDCTCBC_hcru_hcru_1344p_raw.nc

        ENDTIME=`date +%s.%N`
        TIMEDIFF=`echo "$ENDTIME - $STARTTIME" | bc | awk -F"." '{print $1"."$2}'`

        echo "#%$: exe_time: $TIMEDIFF"

        echo "ls -Rlah ${OUTDIR}"
        ls -Rlah ${OUTDIR}

        echo '-----+-----++------------+++++++++--+---'
    done

    FS=1_16M
    OUTDIR=/global/cscratch1/sd/khl7265/${FS}/E3SM/hdf5_log/log/I1850GSWCNPRDCTCBC_hcru_hcru_1344p_raw/

    echo "========================== ${FS} CB8 =========================="
    >&2 echo "========================== ${FS} CB8 =========================="

    echo "export E3SM_IO_HINTS=\"cb_node=8;cb_config_list=*:8;\""
    export E3SM_IO_HINTS="cb_node=8;cb_config_list=*:8;"
    
    echo "rm -rf ${OUTDIR}/*"
    rm -rf ${OUTDIR}/*
    
    echo "#%$=: exp: e3sm_io"
    echo "#%$=: runs: ${i}"
    echo "#%$=: h_num: -1"
    echo "#%$=: nrec: ${NR}"
    echo "#%$=: api: hdf5_log"
    echo "#%$=: strategy: log"
    echo "#%$=: cb_nodes: 8"
    echo "#%$=: flush_freq: ${NR}"
    echo "#%$=: number_of_nodes: ${NN}"
    echo "#%$=: number_of_proc: ${NP}"

    STARTTIME=`date +%s.%N`

    echo "srun -n ${NP} -t 6 --cpu_bind=cores  ./../src/e3sm_io -g 1 -k -o ${OUTDIR}/testfile -a hdf5_log -f -1 -r ${NR} -x log -p -y ${NR} /global/cscratch1/sd/khl7265/FS_64_1M/E3SM/decom/I1850GSWCNPRDCTCBC_hcru_hcru_1344p_raw.nc"
    srun -n ${NP} -t 6 --cpu_bind=cores  ./../src/e3sm_io -g 1 -k -o ${OUTDIR}/testfile -a hdf5_log -f -1 -r ${NR} -x log -p -y ${NR} /global/cscratch1/sd/khl7265/FS_64_1M/E3SM/decom/I1850GSWCNPRDCTCBC_hcru_hcru_1344p_raw.nc

    ENDTIME=`date +%s.%N`
    TIMEDIFF=`echo "$ENDTIME - $STARTTIME" | bc | awk -F"." '{print $1"."$2}'`

    echo "#%$: exe_time: $TIMEDIFF"

    echo "ls -Rlah ${OUTDIR}"
    ls -Rlah ${OUTDIR}

    echo '-----+-----++------------+++++++++--+---'
done

ENDTIME=`date +%s.%N`
TIMEDIFF=`echo "$ENDTIME - $TSTARTTIME" | bc | awk -F"." '{print $1"."$2}'`
echo "-------------------------------------------------------------"
echo "total_exe_time: $TIMEDIFF"
echo "-------------------------------------------------------------"