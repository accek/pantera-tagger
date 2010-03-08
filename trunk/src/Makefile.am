AM_CXXFLAGS = @BOOST_CPPFLAGS@ @OPENMP_CXXFLAGS@

if HAVE_PROTOBUF
AM_CXXFLAGS += @PROTOBUF_CFLAGS@
endif

SUBDIRS = nlpcommon

SUFFIXES = .m4 .m4h

.m4.m4h:
	m4 -I rules/m4 -P $< > $@

BUILT_SOURCES = rules/c1.m4h

bin_PROGRAMS = btagger xces2dat dat2dat

btagger_SOURCES = btagger.cpp \
	rules/a.h rules/c1.m4h rules/p1.h rules/p2.h \
	rules/impl.h rules/make.h brillengine.h \
	brillexeme.h unigram.h rules.h
btagger_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@
btagger_LDADD = nlpcommon/libnlpcommon.la

xces2dat_SOURCES = xces2dat.cpp
xces2dat_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_SERIALIZATION_LIB@
xces2dat_LDADD = nlpcommon/libnlpcommon.la

dat2dat_SOURCES = dat2dat.cpp
dat2dat_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_SERIALIZATION_LIB@
dat2dat_LDADD = nlpcommon/libnlpcommon.la

if HAVE_PROTOBUF

bin_PROGRAMS += xces2corpus corpus2corpus

xces2corpus_SOURCES = xces2corpus.cpp
xces2corpus_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_PROGRAM_OPTIONS_LIB@
xces2corpus_LDADD = nlpcommon/libnlpcommon.la

corpus2corpus_SOURCES = corpus2corpus.cpp
corpus2corpus_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_PROGRAM_OPTIONS_LIB@
corpus2corpus_LDADD = nlpcommon/libnlpcommon.la

endif
