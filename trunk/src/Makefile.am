AM_CXXFLAGS = @BOOST_CPPFLAGS@ @OPENMP_CXXFLAGS@

SUBDIRS = nlpcommon

bin_PROGRAMS = btagger xces2corpus corpus2corpus

btagger_SOURCES = btagger.cpp
btagger_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_REGEX_LIB@
btagger_LDADD = nlpcommon/libnlpcommon.la

xces2corpus_SOURCES = xces2corpus.cpp
xces2corpus_LDFLAGS = @BOOST_LDFLAGS@
xces2corpus_LDADD = nlpcommon/libnlpcommon.la

corpus2corpus_SOURCES = corpus2corpus.cpp
corpus2corpus_LDFLAGS = @BOOST_LDFLAGS@
corpus2corpus_LDADD = nlpcommon/libnlpcommon.la

