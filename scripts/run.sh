#!/bin/bash

DIR=/tmp/accek/wypluwka_2009-12-16-1756.xml.split
THRS=6

export OMP_NUM_THREADS=2

for t in $THRS; do
    if [ ! -d "$DIR-$t" ]; then
        echo "Copying $DIR -> $DIR-$t ..."
        cp -R "$DIR" "$DIR-$t"
    fi
done

for ((i=0; i<10; i++)); do
    (
        for t in $THRS; do
            if [ ! -d "$DIR-$t" ]; then
                echo "Copying $DIR -> $DIR-$t ..."
                cp -R "$DIR" "$DIR-$t"
            fi
            /usr/bin/time ./src/btagger $DIR-$t/npart$i.xml $DIR-$t/part$i.xml $t > $DIR-$t/part$i.out 2>&1
        done
    ) &
done

tail -f $DIR-*/*.out
