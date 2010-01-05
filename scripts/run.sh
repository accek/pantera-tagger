#!/bin/bash

DIR=/tmp/accek/wypluwka_2009-12-16-1756.xml.split

for ((i=0; i<10; i++)); do
    (
        for t in 6; do
            /usr/bin/time ./tagger $DIR-$t/npart$i.xml $DIR-$t/part$i.xml $t > $DIR-$t/part$i.out 2>&1
        done
    ) &
done

tail -f $DIR-*/*.out
