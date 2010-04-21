#!/bin/bash

BASEDIR="`dirname "$PWD/$0"`"
DIR="$1"

if ! [ -d "$DIR" ]; then
    echo "please specify directory as an argument"
	exit 1
fi

if ! [ -f "$BASEDIR/ProjectTagset.class" ]; then
	echo "Compiling Java source..."
	javac "$BASEDIR/ProjectTagset.java"
fi

cd "$DIR"
OUT="../projected/`basename "$DIR"`"
mkdir -p "$OUT"

for i in part*.xml*; do
	echo "Processing $DIR/$i..."
	java -cp "$BASEDIR" ProjectTagset "$DIR/$i" "$OUT/$i"
done
