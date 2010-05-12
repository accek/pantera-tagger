AM_CXXFLAGS = @BOOST_CPPFLAGS@ -I..

CLEANFILES = 

lib_LTLIBRARIES = libnlpcommon.la
libnlpcommon_la_SOURCES = \
    progress.cpp util.cpp category.cpp tagset.cpp \
	spejdtagsetloader.cpp
libnlpcommon_la_LDFLAGS = @BOOST_LDFLAGS@
libnlpcommon_la_LIBADD = @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@ \
						 @BOOST_SERIALIZATION_LIB@

libnlpcommonincludedir = $(includedir)/nlpcommon
libnlpcommoninclude_HEADERS = \
	exception.h ipipanlexer.h lexer.h progress.h tag.h util.h writer.h \
	tagset.h category.h pos.h scorer.h cascorer.h finderrors.h \
	datwriter.h datlexer.h stats.h poliqarp-weights.h category-weights.h \
	stats.h lexemesfilter.h

if HAVE_PROTOBUF

AM_CXXFLAGS += @PROTOBUF_CFLAGS@

SUFFIXES = .proto .pb.cc .pb.h
.proto.pb.cc:
	protoc $< --cpp_out=./ --proto_path=${srcdir}
.proto.pb.h:
	protoc $< --cpp_out=./ --proto_path=${srcdir}

libnlpcommon_la_SOURCES += corpus.pb.cc corpus.pb.h corpustagsetloader.cpp
libnlpcommon_la_LIBADD += @PROTOBUF_LIBS@
libnlpcommoninclude_HEADERS += corpus.pb.h corpuslexer.h corpuswriter.h \
							   corpustagsetloader.h
CLEANFILES += corpus.pb.h corpus.pb.cc

endif
