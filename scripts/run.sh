#!/bin/bash

#DIR=~/studia/doktoranckie/tager/dane/wypluwka_2009-12-16-1756.xml.split

if [ -z "$DIR" ]; then
    echo "DIR environment variable not set."
    exit 1
fi

THRS="6 4 2"

export OMP_NUM_THREADS=1
PROCESSES=12

export LC_ALL="pl_PL.UTF-8"

for t in $THRS; do
    if [ ! -d "$DIR-$t" ]; then
        echo "Copying $DIR -> $DIR-$t ..."
        cp -Rl "$DIR" "$DIR-$t"
    fi
done

for t in $THRS; do
    for ((i=0; i<10; i++)); do
        /usr/bin/time mpirun -np $PROCESSES ./src/btagger $DIR-$t/npart$i.xml $DIR-$t/part$i.xml $t 2>&1 | tee $DIR-$t/part$i.out
    done
done

