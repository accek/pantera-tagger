AM_CXXFLAGS = @BOOST_CPPFLAGS@ -I.. -Wall \
	-finput-charset=UTF-8 \
	-DDEFAULT_TAGSET_PATH=\"${pkgdatadir}/tagsets:@abs_top_srcdir@/tagsets\"

CLEANFILES = 

lib_LTLIBRARIES = libnlpcommon.la
libnlpcommon_la_SOURCES = \
    progress.cpp util.cpp category.cpp tagset.cpp \
	spejdtagsetloader.cpp polish_segm_disamb.cpp
libnlpcommon_la_LDFLAGS = @BOOST_LDFLAGS@
libnlpcommon_la_LIBADD = @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@ \
						 @BOOST_SERIALIZATION_LIB@

libnlpcommonincludedir = $(includedir)/nlpcommon
libnlpcommoninclude_HEADERS = \
	exception.h ipipanlexer.h lexer.h progress.h tag.h util.h writer.h \
	tagset.h category.h pos.h scorer.h cascorer.h finderrors.h \
	datwriter.h datlexer.h poliqarp-weights.h category-weights.h \
	stats.h lexemesfilter.h plaintextwriter.h plaintextlexer.h morfeusz.h \
	nkjptextlexer.h nkjplexerdata.h nkjpwriter.h libsegmentsentencer.h \
	tagset_convert.h polish_tagset_convert.h _pstream.h segm_disamb.h \
	polish_segm_disamb.h lexeme.h

