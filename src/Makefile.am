AM_CXXFLAGS = @BOOST_CPPFLAGS@ @OPENMP_CXXFLAGS@

SUBDIRS = nlpcommon

bin_PROGRAMS = btagger
btagger_SOURCES = btagger.cpp category.cpp tagset.cpp spejdtagsetloader.cpp \
	util.cpp
btagger_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_REGEX_LIB@

