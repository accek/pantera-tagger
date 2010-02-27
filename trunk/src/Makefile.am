AM_CXXFLAGS = @BOOST_CPPFLAGS@ @OPENMP_CXXFLAGS@

if HAVE_PROTOBUF
AM_CXXFLAGS += @PROTOBUF_CFLAGS@
endif

SUBDIRS = nlpcommon

SUFFIXES = .m4 .m4h

.m4.m4h:
	m4 -I rules/m4 -P $< > $@

bin_PROGRAMS = btagger

btagger_SOURCES = rules/c1.m4h \
				  btagger.cpp
btagger_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@
btagger_LDADD = nlpcommon/libnlpcommon.la

if HAVE_PROTOBUF

bin_PROGRAMS += xces2corpus corpus2corpus

xces2corpus_SOURCES = xces2corpus.cpp
xces2corpus_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_PROGRAM_OPTIONS_LIB@
xces2corpus_LDADD = nlpcommon/libnlpcommon.la

corpus2corpus_SOURCES = corpus2corpus.cpp
corpus2corpus_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_PROGRAM_OPTIONS_LIB@
corpus2corpus_LDADD = nlpcommon/libnlpcommon.la

endif
