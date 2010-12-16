AM_CXXFLAGS = @BOOST_CPPFLAGS@ @OPENMP_CXXFLAGS@ @MORFEUSZ_CFLAGS@ \
	-I../third_party/TaKIPI18/Linux/Corpus \
	-DSEGMENT_PATH=\"$(libexecdir)/$(PACKAGE):@abs_top_srcdir@/third_party/segment\" \
	-DENGINES_PATH=\"$(pkgdatadir)/engines:@abs_top_srcdir@/engines\"

if HAVE_PROTOBUF
AM_CXXFLAGS += @PROTOBUF_CFLAGS@
endif

SUBDIRS = nlpcommon

SUFFIXES = .m4 .m4h
.m4.m4h:
	m4 -I rules/m4 -P $< > $@

Boost_LIBRARY_DIRS = $(abspath $(patsubst -L%,%,$(filter -L%,@BOOST_LDFLAGS@)))
Boost_INCLUDE_DIRS = $(abspath $(patsubst -I%,%,$(filter -I%,@BOOST_CPPFLAGS@)))
Icu_LIBRARY_DIRS = $(abspath $(patsubst -L%,%,$(filter -L%,@ICU_LIBS@)))
Icu_INCLUDE_DIRS = $(abspath $(patsubst -I%,%,$(filter -I%,@ICU_CFLAGS@)))
Morfeusz_LIBRARY_DIRS = $(abspath $(firstword $(patsubst -L%,%,$(filter -L%,@MORFEUSZ_LIBS@)) /usr/lib))


../third_party/TaKIPI18/Linux/bin/Corpus/libcorpus.so:
	( cd ../third_party/TaKIPI18/Linux/bin && \
	cmake \
		-DCMAKE_BUILD_TYPE=None \
		-DBOOST_LIBRARYDIR="$(Boost_LIBRARY_DIRS)" \
		-DBoost_INCLUDE_DIR="$(Boost_INCLUDE_DIRS)" \
		-DMORFEUSZ_LIBRARY="$(Morfeusz_LIBRARY_DIRS)" \
		-DICU_LIBRARY="$(Icu_LIBRARY_DIRS)/libicuuc.so" \
		-DICU_I18N_LIBRARY="$(Icu_LIBRARY_DIRS)/libicui18n.so" \
		-DICU_INCLUDE_DIR="$(Icu_INCLUDE_DIRS)" \
		-DCMAKE_CXX_FLAGS="$(CXXFLAGS)" \
		-DCMAKE_C_FLAGS="$(CFLAGS)" \
		-DCMAKE_SHARED_LINKER_FLAGS="$(LDFLAGS) -L$(Morfeusz_LIBRARY_DIRS)" \
		-DCMAKE_INSTALL_PREFIX="$(PREFIX)" \
		.. )
	+make -C ../third_party/TaKIPI18/Linux/bin corpus

clean-local:
	+make -C ../third_party/TaKIPI18/Linux/bin clean

install-exec-local: ../third_party/TaKIPI18/Linux/bin/Corpus/libcorpus.so
	$(INSTALL_PROGRAM) -D ../third_party/TaKIPI18/Linux/bin/Corpus/libcorpus.so $(pkglibdir)/libcorpus.so

GLOBAL_LINK_FLAGS = -Wl,-rpath -Wl,$(pkglibdir)

LIBCORPUS_LINK_FLAGS = -L../third_party/TaKIPI18/Linux/bin/Corpus \
					   -Wl,-rpath -Wl,@abs_top_srcdir@/third_party/TaKIPI18/Linux/bin/Corpus \
					   -lcorpus

LIBMORFEUSZ_LINK_FLAGS = -Wl,-rpath -Wl,@abs_top_srcdir@/third_party/morfeusz/lib

BUILT_SOURCES = rules/c1.m4h

#bin_PROGRAMS = xces2dat xces2txt dat2dat eval txt2xces \
#			   nkjp2xces pantera

bin_PROGRAMS = pantera wrapper_test
lib_LTLIBRARIES = libpantera.la

pantera_SOURCES = pantera.cpp pantera_rules.cpp \
	rules/a.h rules/c1.m4h rules/p1.h rules/p2.h \
	rules/impl.h rules/make.h brillengine.h \
	brillexeme.h unigram.h rules.h pantera_rules.h \
	pantera.h
pantera_CXXFLAGS = $(AM_CXXFLAGS)
pantera_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_SERIALIZATION_LIB@ \
				   @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@ \
				   @BOOST_SYSTEM_LIB@ @BOOST_FILESYSTEM_LIB@ \
				   @BOOST_IOSTREAMS_LIB@ \
				   @BOOST_MPI_LIB@ @ICU_LIBS@ @LIBICONV@ \
				   @MORFEUSZ_LIBS@ $(LIBCORPUS_LINK_FLAGS) \
				   $(GLOBAL_LINK_FLAGS) $(LIBMORFEUSZ_LINK_FLAGS)
pantera_LDADD = nlpcommon/libnlpcommon.la \
				../third_party/TaKIPI18/Linux/bin/Corpus/libcorpus.so

libpantera_la_SOURCES = wrapper.cpp pantera_rules.cpp
include_HEADERS = wrapper.h

wrapper_test_SOURCES = wrapper_test.cpp \
	rules/a.h rules/c1.m4h rules/p1.h rules/p2.h \
	rules/impl.h rules/make.h brillengine.h \
	brillexeme.h unigram.h rules.h pantera_rules.h \
	pantera.h
wrapper_test_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_SERIALIZATION_LIB@ \
				   @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@ \
				   @BOOST_SYSTEM_LIB@ @BOOST_FILESYSTEM_LIB@ \
				   @BOOST_IOSTREAMS_LIB@ \
				   @BOOST_MPI_LIB@ @ICU_LIBS@ @LIBICONV@ \
				   @MORFEUSZ_LIBS@ $(LIBCORPUS_LINK_FLAGS) \
				   $(GLOBAL_LINK_FLAGS) $(LIBMORFEUSZ_LINK_FLAGS)
wrapper_test_LDADD = libpantera.la nlpcommon/libnlpcommon.la \
				../third_party/TaKIPI18/Linux/bin/Corpus/libcorpus.so

#eval_SOURCES = eval.cpp
#eval_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_REGEX_LIB@ \
#			   @BOOST_PROGRAM_OPTIONS_LIB@
#eval_LDADD = nlpcommon/libnlpcommon.la
#
#xces2dat_SOURCES = xces2dat.cpp
#xces2dat_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_SERIALIZATION_LIB@ \
#				   @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@
#xces2dat_LDADD = nlpcommon/libnlpcommon.la
#
#xces2txt_SOURCES = xces2txt.cpp
#xces2txt_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_SERIALIZATION_LIB@ \
#				   @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@
#xces2txt_LDADD = nlpcommon/libnlpcommon.la
#
#txt2xces_SOURCES = txt2xces.cpp
#txt2xces_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_SERIALIZATION_LIB@ \
#				   @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@ \
#				   @MORFEUSZ_LIBS@ -liconv $(LIBCORPUS_LINK_FLAGS)
#txt2xces_LDADD = nlpcommon/libnlpcommon.la
#
#nkjp2xces_SOURCES = nkjp2xces.cpp
#nkjp2xces_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_SERIALIZATION_LIB@ \
#				   @BOOST_REGEX_LIB@ @BOOST_PROGRAM_OPTIONS_LIB@ \
#				   @MORFEUSZ_LIBS@ -liconv $(LIBCORPUS_LINK_FLAGS)
#nkjp2xces_LDADD = nlpcommon/libnlpcommon.la
#
#dat2dat_SOURCES = dat2dat.cpp
#dat2dat_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_SERIALIZATION_LIB@
#dat2dat_LDADD = nlpcommon/libnlpcommon.la
#
#if HAVE_PROTOBUF
#
#bin_PROGRAMS += xces2corpus corpus2corpus
#
#xces2corpus_SOURCES = xces2corpus.cpp
#xces2corpus_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_PROGRAM_OPTIONS_LIB@
#xces2corpus_LDADD = nlpcommon/libnlpcommon.la
#
#corpus2corpus_SOURCES = corpus2corpus.cpp
#corpus2corpus_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_PROGRAM_OPTIONS_LIB@
#corpus2corpus_LDADD = nlpcommon/libnlpcommon.la
#
#endif
