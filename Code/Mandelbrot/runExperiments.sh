#!/usr/bin/env bash

THREADS="1 2 4 8 16 20 22 32 40"
RESOLUTIONS="100 1000 5000 10000"
STRIDES="1 2 4 8 16 32"

AVERAGES=10

EXE="$1"
FLAGS="-n 1 -A ${AVERAGES}"
LOGFILE="experiments.log"

for res in ${RESOLUTIONS}; do
    for stride in ${STRIDES}; do
        for t in ${THREADS}; do
            ${EXE} ${FLAGS} --hpx:threads ${t} -r ${res}x${res} -S ${stride} > ${LOGFILE}
        done
    done
done

#eof
