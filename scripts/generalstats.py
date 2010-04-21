#!/usr/bin/env python

from collections import defaultdict
from operator import itemgetter

partfiles = [ 'part%d.xml' % i for i in xrange(10) ]
total_tokens = 0
for pf in partfiles:
	total_tokens += open(pf).read().count('<orth>')
print r'\newcommand{\totaltokens}{%d}' % total_tokens

pos_errors = defaultdict(int)
for pf in partfiles:
	e = open(pf + '.errors.txt').read().split('\n')
	e = filter(lambda l: 'POS-MISMATCH' in l, e)
	e = map(str.split, e)
	for count, _, bad, good in e:
		pos_errors[good] += int(count)

pos_stats = sorted(pos_errors.iteritems(), key=itemgetter(1), reverse=True)
for pos, count in pos_stats:
	print '% bad_pos', pos, count, count*100.0/total_tokens

ps = (pos_stats[:len(pos_stats)/2], pos_stats[len(pos_stats)/2:])
print r'\newcommand{\posstatsA}{%'
for pos, count in ps[0]:
	print r'\pos{%s} & %d & %.2f\%%\\' % (pos[1:-1], count, count*100.0/total_tokens)
print '}'
print r'\newcommand{\posstatsB}{%'
for pos, count in ps[1]:
	print r'\pos{%s} & %d & %.2f\%%\\' % (pos[1:-1], count, count*100.0/total_tokens)
print '}'

pos_correctness = (1.0-sum(pos_errors.itervalues())/float(total_tokens))*100.0
print '% pos_correctness', pos_correctness
print r'\newcommand{\poscorrectness}{%.2f\%%}' % pos_correctness

cat_errors = defaultdict(int)
for pf in partfiles:
	e = open(pf + '.errors.txt').read().split('\n')
	e = filter(lambda l: 'CAT-MISMATCH' in l, e)
	e = map(str.split, e)
	for count, _, cat, bad, good in e:
		cat_errors[cat] += int(count)

cat_stats = sorted(cat_errors.iteritems(), key=itemgetter(1), reverse=True)
for cat, count in cat_stats:
	print '% bad_cat', cat, count, count*100.0/total_tokens
print r'\newcommand{\catstats}{%'
for cat, count in cat_stats:
	print r'\cat{%s} & %d & %.2f\%%\\' % (cat, count, count*100.0/total_tokens)
print '}'


catspec_errors = defaultdict(int)
for pf in partfiles:
	e = open(pf + '.errors.txt').read().split('\n')
	e = filter(lambda l: 'CAT-MISMATCH' in l, e)
	e = map(str.split, e)
	for count, _, cat, bad, good in e:
		catspec_errors[cat, bad[1:], good[1:-1]] += int(count)

catspec_stats = sorted(catspec_errors.iteritems(), key=itemgetter(1), reverse=True)[:15]
for catspec, count in catspec_stats:
	print '% bad_catspec', catspec[0], catspec[1], catspec[2], count, count*100.0/total_tokens
print r'\newcommand{\catspecstats}{%'
for catspec, count in catspec_stats:
	print r'\cat{%s(%s)} & \cat{%s(%s)} & %d & %.2f\%%\\' % (catspec[0], catspec[1], catspec[0], catspec[2], count, count*100.0/total_tokens)
print '}'



