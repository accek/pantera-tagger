#!/bin/bash

if [ -z "$DIR" ]; then
    echo "DIR environment variable not set."
    exit 1
fi

stats() {
	IFILE=$1
	OFILE=$2
	HDR=$3
	COL=$4
    grep $HDR $IFILE | awk "{ print \$$COL; }" | sed 's/,//g' >> $OFILE
}

PARTS="fmeasure precision recall time strong_correctness \
	avg_score fmeasure_pos precision_pos recall_pos \
	strong_correctness_pos"

for ODIR in $DIR-*; do
    echo $ODIR
	cd $ODIR

	for p in $PARTS; do
		> "$p.txt"
	done

	for i in *-eval.out; do
		if [ -f "$i.stats" ]; then
			j="$i.stats";
		else
			j="$i"
		fi

		stats $j precision.txt Stats 4
		stats $j recall.txt Stats 6
		stats $j fmeasure.txt Stats 8
		stats $j strong_correctness.txt Stats 10
		stats $j avg_score.txt Stats 12

		stats $j precision_pos.txt POSstats 4
		stats $j recall_pos.txt POSstats 6
		stats $j fmeasure_pos.txt POSstats 8
		stats $j strong_correctness_pos.txt POSstats 10

		grep maxresident $i | awk '{ print $3; }' | sed 's/elapsed//' \
			| sed 's/:/*60+/' | bc >> "time.txt"
	done

	( for p in $PARTS; do
		echo $p
		cat "$p.txt"
		echo
	done ) > stats.txt

	cat stats.txt
done

