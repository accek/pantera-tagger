AM_CXXFLAGS = @BOOST_CPPFLAGS@ @PROTOBUF_CFLAGS@ -I..

SUFFIXES = .proto .pb.cc .pb.h
.proto.pb.cc:
	protoc $< --cpp_out=./ --proto_path=${srcdir}
.proto.pb.h:
	protoc $< --cpp_out=./ --proto_path=${srcdir}

bin_PROGRAMS = xces2corpus corpus2corpus

xces2corpus_SOURCES = xces2corpus.cpp
xces2corpus_LDFLAGS = @BOOST_LDFLAGS@
xces2corpus_LDADD = libnlpcommon.la

corpus2corpus_SOURCES = corpus2corpus.cpp
corpus2corpus_LDFLAGS = @BOOST_LDFLAGS@
corpus2corpus_LDADD = libnlpcommon.la

lib_LTLIBRARIES = libnlpcommon.la
libnlpcommon_la_SOURCES = \
	corpus.pb.cc corpus.pb.h \
    progress.cpp ipipanlexer.cpp corpuswriter.cpp corpusreader.cpp
libnlpcommon_la_LDFLAGS = @BOOST_LDFLAGS@
libnlpcommon_la_LIBADD = @BOOST_REGEX_LIB@ @PROTOBUF_LIBS@
