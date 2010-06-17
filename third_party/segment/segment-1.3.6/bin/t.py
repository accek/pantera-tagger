#!/usr/bin/env python

from subprocess import *

p = Popen("./segment", shell=True, stdin=PIPE, stdout=PIPE)

for i in xrange(10):
    p.stdin.write("To jest akapit.\n\n")
    p.stdin.flush()
    print 'Answer:', p.stdout.read()

