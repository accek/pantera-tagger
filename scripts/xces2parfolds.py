#!/usr/bin/env python

import os.path
import sys
import itertools

fn = sys.argv[1]
numfolds = int(sys.argv[2])

dir = fn + '.split'
if not os.path.isdir(dir):
    os.mkdir(dir)

f = open(fn, 'r')

part_files = [ open(os.path.join(dir, 'part%d.xml' % i), 'w')
               for i in xrange(numfolds) ]
npart_files = [ open(os.path.join(dir, 'npart%d.xml' % i), 'w')
                for i in xrange(numfolds) ]

for o in itertools.chain(part_files, npart_files):
    o.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    o.write('<!DOCTYPE cesAna SYSTEM "xcesAnaIPI.dtd">\n')
    o.write('<cesAna version="1.0" type="lex disamb">\n')
    o.write('<chunkList>\n')

fold_no = 0
line_no = 0
depth = 0
for line in f:
    if (line_no % 100000) == 0:
        print 'Line', line_no
    line_no += 1

    line = line.strip()

    if line.startswith('<chunk '):
        depth += 1
    if line.startswith('<chunk type="p"'):
        fold_no = (fold_no + 1) % numfolds
    if depth > 0:
        for i in xrange(numfolds):
            if i == fold_no:
                part_files[i].write(line + '\n')
            else:
                npart_files[i].write(line + '\n')
    if line.startswith('</chunk>'):
        depth -= 1

for o in itertools.chain(part_files, npart_files):
    o.write('</chunkList>\n')
    o.write('</cesAna>\n')
    o.close()

f.close()
