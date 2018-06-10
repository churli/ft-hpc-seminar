#!/usr/bin/env bash

THREADS="1 2 4 8 16 20 22 32 40"
RESOLUTIONS="100 1000 5000 10000"
STRIDES="1 2 4 8 16 32"

AVERAGES=10

EXE="$1"
FLAGS="-n 1 -A ${AVERAGES}"
LOGFILE="experiments.log"
if [[ -f ${LOGFILE} ]]; then
    mv ${LOGFILE} ${LOGFILE}.bak
fi

for res in ${RESOLUTIONS}; do
    for t in ${THREADS}; do
        for stride in ${STRIDES}; do
            ${EXE} ${FLAGS} --hpx:threads ${t} -r ${res}x${res} -S ${stride} | tee -a ${LOGFILE}
        done
    done
done

#eof
