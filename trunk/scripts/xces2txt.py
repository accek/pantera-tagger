#!/usr/bin/env python

import sys
import os.path
import re
import textwrap

def convert_file(filename):
    inp = open(filename, 'r')

    nospace = True
    paragraphs = []
    par = ''
    for line in inp:
        line = line.strip()
        if line.startswith('<orth>') and line.endswith('</orth>'):
            if not nospace:
                par += ' '
            par += line[6:-7].replace('&lt;', '<') \
                    .replace('&gt;', '>').replace('&amp;', '&')
            nospace = False
        if line.startswith('<chunk type="p"'):
            if par:
                paragraphs.append(par)
            par = ''
            nospace = True
        if line.startswith('<ns/>'):
            nospace = True
    if par:
        paragraphs.append(par)

    outp = open(os.path.splitext(filename)[0] + '.txt', 'w')
    for par in paragraphs:
        wrapped = textwrap.wrap(par, break_long_words=False,
                break_on_hyphens=False)
        outp.write('\n'.join(wrapped) + '\n\n')
    outp.close()

if not sys.argv[1:]:
    print "Usage:", sys.argv[0], " file_to_convert ..."
    print
    print "For each file, a converted output is saved to a .txt file"
    print "in the folder where the original resided."
    sys.exit(1)

for i, filename in enumerate(sys.argv[1:]):
    print '%3d/%d: %s' % (i + 1, len(sys.argv) - 1, filename)
    convert_file(filename)
