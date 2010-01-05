#!/usr/bin/env python

import os.path
import sys

fn = sys.argv[1]
dir = fn + '.split'

if not os.path.isdir(dir):
    os.mkdir(dir)

PARTS = 10

f = open(fn, 'r')

part_files = [ open(os.path.join(dir, 'part%d.xml' % i), 'w') for i in xrange(PARTS) ]
npart_files = [ open(os.path.join(dir, 'npart%d.xml' % i), 'w') for i in xrange(PARTS) ]

part_no = 0
line_no = 0
for line in f:
    if (line_no % 100000) == 0:
        print 'Line', line_no
    line_no += 1

    for i in xrange(PARTS):
        if i == part_no:
            part_files[i].write(line)
        else:
            npart_files[i].write(line)
    if line.startswith('</chunk>'):
        part_no = (part_no + 1) % PARTS

map(lambda f: f.close(), part_files)
map(lambda f: f.close(), npart_files)
f.close()
