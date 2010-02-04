AM_CXXFLAGS = @BOOST_CPPFLAGS@ @PROTOBUF_CFLAGS@ -I..

SUFFIXES = .proto .pb.cc .pb.h
.proto.pb.cc:
	protoc $< --cpp_out=./ --proto_path=${srcdir}
.proto.pb.h:
	protoc $< --cpp_out=./ --proto_path=${srcdir}


lib_LTLIBRARIES = libnlpcommon.la
libnlpcommon_la_SOURCES = \
	corpus.pb.cc corpus.pb.h \
    progress.cpp util.cpp category.cpp tagset.cpp \
	spejdtagsetloader.cpp corpustagsetloader.cpp
libnlpcommon_la_LDFLAGS = @BOOST_LDFLAGS@
libnlpcommon_la_LIBADD = @BOOST_REGEX_LIB@ @PROTOBUF_LIBS@

libnlpcommonincludedir = $(includedir)/nlpcommon
libnlpcommoninclude_HEADERS = corpus.pb.h corpuslexer.h corpuswriter.h \
	exception.h ipipanlexer.h lexer.h progress.h tag.h util.h writer.h \
	tagset.h category.h pos.h
