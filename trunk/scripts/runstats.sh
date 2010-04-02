#!/bin/bash

if [ -z "$DIR" ]; then
    echo "DIR environment variable not set."
    exit 1
fi

for ODIR in $DIR-*; do
    echo $ODIR
    grep Stats $ODIR/*.out | awk '{ print $8; }' | tee "$ODIR/fmeasures.txt"
    grep Stats $ODIR/*.out | awk '{ print $4; }' | sed 's/,//g' > "$ODIR/precisions.txt"
    grep Stats $ODIR/*.out | awk '{ print $6; }' | sed 's/,//g' > "$ODIR/recalls.txt"
    grep maxresident $ODIR/*.out | awk '{ print $3; }' | sed 's/elapsed//' \
        | sed 's/:/*60+/' | bc > "$ODIR/times.txt"
done

