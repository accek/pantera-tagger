#!/usr/bin/env python

import sys
import os.path
import re

def convert_file(filename):
    inp = open(filename, 'r')
    outp = open(os.path.join(os.path.dirname(filename), 'morph.xml'), 'w')
    outp.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    outp.write('<!DOCTYPE cesAna SYSTEM "xcesAnaIPI.dtd">\n')
    outp.write('<cesAna version="1.0" type="lex disamb">\n')
    outp.write('<chunkList>\n')

    f = None
    base = None
    ctag = None
    forms = {}

    r = re.compile(
        r'(?P<chunk><(?P<ctype>p|s) )|'
        r'(?P<endchunk></(p|s)>)|'
        r'(?P<seg><seg)|'
        r'(?P<f><f (fVal="(?P<fval>.*?)" )?name="(?P<ff>.*?)")|'
        r'(?P<string><string>(?P<sstring>.*?)</string>)|'
        r'(?P<symbol><symbol .*value="(?P<ssymbol>.*?)"( xml:id="(?P<isymbol>.*?)")?)')

    for m in r.finditer(inp.read()):
        if m.group('chunk'):
            outp.write('<chunk type="%s">\n' % m.group('ctype'))
        elif m.group('endchunk'):
            outp.write('</chunk>\n')
        elif m.group('seg'):
            base = None
            ctag = None
            orth = None
            nps = False
            forms = {}
        elif m.group('f'):
            f = m.group('ff')
            if f == 'nps':
                nps = True
            elif f == 'choice':
                ch = m.group('fval')[1:]
                if nps:
                    outp.write('<ns/>\n')
                outp.write('<tok>\n')
                outp.write('<orth>%s</orth>\n' % orth)
                for k, v in forms.iteritems():
                    outp.write('<lex%s><base>%s</base><ctag>%s</ctag></lex>\n'
                            % (k == ch and ' disamb="1"' or '', v[0], v[1]))
                outp.write('</tok>\n')
        elif m.group('string'):
            s = m.group('sstring')
            if f == 'orth':
                orth = s
            elif f == 'base':
                base = s
        elif m.group('symbol'):
            v = m.group('ssymbol')
            if f == 'ctag':
                ctag = v
            elif f == 'msd':
                i = m.group('isymbol')
                tag = ctag
                if v:
                    tag += ':' + v
                forms[i] = (base, tag)

    outp.write('</chunkList>\n')
    outp.write('</cesAna>\n')

if not sys.argv[1:]:
    print "Usage:", sys.argv[0], " file_to_convert ..."
    print
    print "For each file, a converted output is saved to morph.xml file"
    print "in the folder where the original resided."
    sys.exit(1)

for i, filename in enumerate(sys.argv[1:]):
    print '%3d/%d: %s' % (i + 1, len(sys.argv) - 1, filename)
    convert_file(filename)
