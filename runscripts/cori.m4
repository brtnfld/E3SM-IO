changequote(`[[[', `]]]')dnl
changecom([[[###]]], [[[$$$]]])dnl
#!/bin/bash
#SBATCH -q VAR_QUEUE
#SBATCH -N VAR_NN
#SBATCH -C VAR_NODE_TYPE
#SBATCH -t 00:VAR_TL:00
#SBATCH -o VAR_NAME.txt
#SBATCH -e VAR_NAME.err
#SBATCH -L SCRATCH
#SBATCH -A VAR_ACC

CONFIG_FDBG=../datasets/f_case_866x72_16p.nc
CONFIG_F30=../datasets/f_case_48602x72_512p.nc
CONFIG_F120=/global/cscratch1/sd/khl7265/FS_64_1M/E3SM/decom/FC5AV1C-H01A_ne120_oRRS18v3_21600p_raw.nc
CONFIG_G=/global/cscratch1/sd/khl7265/FS_64_1M/E3SM/decom/GMPAS-NYF_T62_oRRS18to6v3_9600p_raw.nc
CONFIG_IDBG=../datasets/i_case_f19_g16_16p.nc 
CONFIG_I=/global/cscratch1/sd/khl7265/FS_64_1M/E3SM/decom/I1850GSWCNPRDCTCBC_hcru_hcru_1344p_raw.nc


IN_FDBG=/global/cscratch1/sd/khl7265/FS_64_1M/E3SM/realdata/F_DBG
IN_F30=/global/cscratch1/sd/khl7265/FS_64_1M/E3SM/realdata/F_30
IN_F120=/global/cscratch1/sd/khl7265/FS_64_1M/E3SM/realdata/F_120
IN_G=/global/cscratch1/sd/khl7265/FS_64_1M/E3SM/realdata/G
IN_I=/global/cscratch1/sd/khl7265/FS_64_1M/E3SM/realdata/I

E3SM_IO_DATE="VAR_E3SM_IO_DATE"
HDF5_LIB_PATH=VAR_HDF5_LIB_PATH
HDF5_LIB_DATE="VAR_HDF5_LIB_DATE"
ADIOS2_LIB_PATH=VAR_ADIOS2_LIB_PATH
ADIOS2_LIB_DATE="VAR_ADIOS2_LIB_DATE"
LOGVOL_LIB_PATH=VAR_LOGVOL_LIB_PATH
LOGVOL_LIB_DATE="VAR_LOGVOL_LIB_DATE"
PNC_LIB_PATH=VAR_PNC_LIB_PATH
NREC=VAR_RECS
PPN=VAR_PPN
RTL=VAR_RTL
CONFIG_POST=VAR_CONFIG
OUTDIR_ROOT=VAR_OUTDIR_ROOT

APPS=(../src/e3sm_io)
HXS=(VAR_HX)
CONFIG="CONFIG_${CONFIG_POST}"
CONFIG=${!CONFIG}
CONFIG_NAME=$(basename -- "${CONFIG}")
CONFIG_NAME="${CONFIG_NAME%.*}"
INDIR="IN_${CONFIG_POST}"
INDIR=${!INDIR}
APIS=(pnetcdf hdf5_log)
STRATES=(canonical log)
OPTIONS=(11111)
FFREQS=(1 VAR_RECS)
OPS=(VAR_OP)

NN=${SLURM_NNODES}
let NP=NN*PPN
let NA=NN*2
let NG=NN-1

# Print exp setting
echo "#%$: e3sm_io_hash: VAR_E3SM_IO_HASH"
echo "#%$: pnc_hash: VAR_PNC_HASH"
echo "#%$: pnc_path: ${PNC_LIB_PATH}"
echo "#%$: hdf5_hash: VAR_HDF5_HASH"
echo "#%$: hdf5_path: ${HDF5_LIB_PATH}"
echo "#%$: submit_date: VAR_SUBMIT_DATE"
echo "#%$: exe_date: $(date)"
echo "#%$: exp: e3sm_io"
echo "#%$: repeation: VAR_RUNS"
echo "#%$: output_folder: VAR_OUTDIR_ROOT"
echo "#%$: e3sm_config_post: ${CONFIG_POST}"
echo "#%$: e3sm_config: ${CONFIG}"
echo "#%$: number_of_nodes: ${NN}"
echo "#%$: number_of_proc: ${NP}"
echo '-----+-----++----***--------+++++**++++--+---'

for API in ${APIS[@]}
do
    for STRATE in ${STRATES[@]}
    do
        echo "mkdir -p ${OUTDIR_ROOT}/${API}/${STRATE}/${CONFIG_NAME}"
        mkdir -p ${OUTDIR_ROOT}/${API}/${STRATE}/${CONFIG_NAME}
    done
done


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

ulimit -c unlimited

TSTARTTIME=`date +%s.%N`

for i in $(seq VAR_RUNS);
do
    for FFREQ in ${FFREQS[@]}
    do
        for APP in ${APPS[@]}
        do
            for (( j=0; j<${#OPS}; j++ ));
            do
                OP=${OPS:$j:1}
                for HX in ${HXS[@]}
                do
                    for API in ${APIS[@]}
                    do
                        for STRATE in ${STRATES[@]}
                        do
                            if [ "${STRATE}" = "log" ] ; then
                                # Only hdf5_log can do log for now
                                if [ "${API}" != "hdf5_log" ] ; then
                                    continue
                                fi
                                # 1 subfile
                                let NG=1    
                                # 64 aggregators
                                ##export E3SM_IO_HINTS="romio_cb_write=enable;"
                                unset E3SM_IO_HINTS
                            elif [ "${STRATE}" = "canonical" ] ; then
                                # Only pnc can do canonical for now
                                if [ "${API}" != "pnetcdf" ] ; then
                                    continue
                                fi
                                # pnc always flush on each records
                                if [ "${FFREQ}" != "1" ] ; then
                                    continue
                                fi
                                # 1 subfile
                                let NG=1    
                                # 64 aggregators
                                #export E3SM_IO_HINTS="romio_cb_write=enable;"
                                unset E3SM_IO_HINTS
                            else
                                # hdf5_log cannot do blob
                                if [ "${API}" == "hdf5_log" ] ; then
                                    continue
                                fi
                                if [ "${CONFIG_POST}" = "I" ] ; then
                                    if [ "${API}" = "adios" ] ; then
                                        let NG=NN*2 # ADIOS method must double #subfiles
                                    else
                                        let NG=NN
                                    fi
                                else
                                    let NG=NN-1 # F and G case use #nodes - 1
                                fi
                                # 8 aggregators
                                export E3SM_IO_HINTS="cb_node=8;cb_config_list=*:8;romio_cb_write=enable"
                            fi

                            OUTDIR="${OUTDIR_ROOT}/${API}/${STRATE}/${CONFIG_NAME}"
                            echo "rm -rf ${OUTDIR}/*"
                            rm -rf ${OUTDIR}/*

                            for OPT in ${OPTIONS[@]}
                            do
                                DWRITE_N=${OPT:0:1}
                                META_ENCODE=${OPT:1:1}
                                META_ZIP=${OPT:2:1}
                                META_SHARE=${OPT:3:1}
                                SUBFILING=${OPT:4:1}

                                export H5VL_LOG_METADATA_SHARE=${META_SHARE}
                                export H5VL_LOG_METADATA_ZIP=${META_ZIP}
                                export H5VL_LOG_SEL_ENCODING=${META_ENCODE}
                                export H5VL_LOG_SUBFILING=${SUBFILING}
                                export E3SM_IO_HDF5_USE_LOGVOL_WRITEN=${DWRITE_N}

                                echo "========================== E3SM-IO ${API} ${OP} =========================="
                                >&2 echo "========================== E3SM-IO ${API} ${OP}=========================="
                                
                                CUR_E3SM_IO_DATE=$(stat -c %Y ../src/e3sm_io)
                                if [[ "${CUR_E3SM_IO_DATE}" != "${E3SM_IO_DATE}" ]]; then
                                    echo "Warning: e3sm_io changed after submission"
                                fi
                                CUR_HDF5_DATE=$(stat -c %Y ${HDF5_LIB_PATH}/lib/libhdf5.so.1000.0.0)
                                if [[ "${CUR_HDF5_DATE}" != "${HDF5_LIB_DATE}" ]]; then
                                    echo "Warning: libhdf5.so changed after submission"
                                fi
                                CUR_ADIOS2_DATE=$(stat -c %Y ${ADIOS2_LIB_PATH}/lib64/libadios2_c_mpi.so)
                                if [[ "${CUR_ADIOS2_DATE}" != "${ADIOS2_LIB_DATE}" ]]; then
                                    echo "Warning: libadios2_c_mpi.so changed after submission"
                                fi
                                CUR_LOGVOL_DATE=$(stat -c %Y ${LOGVOL_LIB_PATH}/lib/libH5VL_log.so)
                                if [[ "${CUR_LOGVOL_DATE}" != "${LOGVOL_LIB_DATE}" ]]; then
                                    echo "Warning: libH5VL_log.so changed after submission"
                                fi

                                echo "#%$: exp: e3sm_io"
                                echo "#%$: runs: ${i}"
                                echo "#%$: app: ${APP}"
                                echo "#%$: config: ${CONFIG}"
                                echo "#%$: h_num: ${HX}"
                                echo "#%$: nrec: ${NREC}"
                                echo "#%$: api: ${API}"
                                echo "#%$: strategy: ${STRATE}"
                                echo "#%$: flush_freq: ${FFREQ}"
                                echo "#%$: logvol_dwrite_n: ${DWRITE_N}"
                                echo "#%$: logvol_meta_encode: ${META_ENCODE}"
                                echo "#%$: logvol_meta_zip: ${META_ZIP}"
                                echo "#%$: logvol_meta_share: ${META_SHARE}"
                                echo "#%$: logvol_subfiling: ${SUBFILING}"
                                echo "#%$: operation: ${OP}"
                                echo "#%$: delay_init: 1"
                                echo "#%$: number_of_nodes: ${NN}"
                                echo "#%$: number_of_proc: ${NP}"
                            
                                STARTTIME=`date +%s.%N`

                                if [ "$OP" = "w" ] ; then
                                    echo "srun -n ${NP} -t ${RTL} --cpu_bind=cores  ./${APP} -g ${NG} -k -o ${OUTDIR}/testfile -a ${API} -f ${HX} -r ${NREC} -x ${STRATE} -p -y ${FFREQ} ${CONFIG}"
                                    srun -n ${NP} -t ${RTL} --cpu_bind=cores  ./${APP} -g ${NG} -k -o ${OUTDIR}/testfile -a ${API} -f ${HX} -r ${NREC} -x ${STRATE} -p -y ${FFREQ} ${CONFIG}
                                else
                                    echo "srun -n ${NP} -t ${RTL} --cpu_bind=cores  ./${APP} -g ${NG} -k -i ${OUTDIR}/testfile_rd -a ${API} -f ${HX} -r ${NREC} -x ${STRATE} -p -y ${FFREQ} ${CONFIG}"
                                    srun -n ${NP} -t ${RTL} --cpu_bind=cores  ./${APP} -g ${NG} -k -i ${OUTDIR}/testfile_rd -a ${API} -f ${HX} -r ${NREC} -x ${STRATE} -p -y ${FFREQ} ${CONFIG}
                                fi

                                ENDTIME=`date +%s.%N`
                                TIMEDIFF=`echo "$ENDTIME - $STARTTIME" | bc | awk -F"." '{print $1"."$2}'`

                                echo "#%$: exe_time: $TIMEDIFF"

                                echo "ls -Rlah ${OUTDIR}"
                                ls -Rlah ${OUTDIR}

                                echo '-----+-----++------------+++++++++--+---'

                                # Only logvol needs to run options
                                if [ "${API}" != "hdf5_log" ] ; then
                                    break
                                fi
                            done
                        done
                    done
                done
            done
        done
    done
done

ENDTIME=`date +%s.%N`
TIMEDIFF=`echo "$ENDTIME - $TSTARTTIME" | bc | awk -F"." '{print $1"."$2}'`
echo "-------------------------------------------------------------"
echo "total_exe_time: $TIMEDIFF"
echo "-------------------------------------------------------------"