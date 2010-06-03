dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(btagger, 1.0)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

AC_SUBST(CXX)
AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)

AC_LANG([C++])

ACX_MPI
CXX="$MPICXX"
AC_SUBST(MPILIBS)

ACX_PTHREAD([], [AC_MSG_ERROR([pthread development bits not found])])
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"
LDFLAGS="$LDFLAGS $PTHREAD_LIBS"

AC_ARG_ENABLE(eclipse,
  [  --enable-eclipse  Support eclipse-aware build, ie. build \
   in 32-bit mode on Snow Leopard],
  [ eclipse=yes ])

if test "x$eclipse" = "xyes"; then
	case "$host" in
	  *-darwin*)
		AC_CHECK_SIZEOF(void *)
		if test "$ac_cv_sizeof_void_p" = 8; then
			dnl Build in 32-bit mode on Snow Leopard
			CXXFLAGS="$CXXFLAGS -arch i386"
			CFLAGS="$CFLAGS -arch i386"
		fi
	  	;;
	esac
fi

AX_BOOST_BASE([1.40.0])
AX_BOOST_REGEX
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_SERIALIZATION
AX_BOOST_MPI

PKG_CHECK_MODULES([PROTOBUF], [protobuf >= 2.0],
                  [have_protobuf=yes], [have_protobuf=no])
if test "x$have_protobuf" = "xyes"; then
    AC_CHECK_PROG(ax_cv_protoc, protoc, yes, no)
    if test "x$ax_cv_protoc" = "xyes"; then
        AC_SUBST(PROTOBUF_CFLAGS)
        AC_SUBST(PROTOBUF_LIBS)
        AC_DEFINE(HAVE_PROTOBUF, 1, [Google Protocol Buffers available])
    else
        have_protobuf=no
    fi
fi
AM_CONDITIONAL(HAVE_PROTOBUF, test "x$have_protobuf" = "xyes")

AC_ARG_WITH([openmp],
    AS_HELP_STRING([--without-openmp], [Do not use OpenMP parallelism]))

AS_IF([test "x$with_openmp" = "xno"], [], [AX_OPENMP])
AC_SUBST(OPENMP_CXXFLAGS)

AX_MORFEUSZ
AC_SUBST(MORFEUSZ_LIBS)
AC_SUBST(MORFEUSZ_CFLAGS)

AX_ICU
AC_SUBST(ICU_LIBS)
AC_SUBST(ICU_CFLAGS)

AC_ARG_WITH([tcmalloc],
    AS_HELP_STRING([--without-tcmalloc], [Ignore presence of tcmalloc and disable it]))

AS_IF([test "x$with_tcmalloc" != "xno"],
      [AC_SEARCH_LIBS([tc_malloc], [tcmalloc tcmalloc_minimal], [have_tcmalloc=yes], [have_tcmalloc=no])],
      [have_tcmalloc=no])

AS_IF([test "x$have_tcmalloc" = "xyes"],
      [LDFLAGS="$LDFLAGS"],
      [AS_IF([test "x$with_tcmalloc" = "xyes"],
             [AC_MSG_ERROR([tcmalloc requested but not found])
      ])
])

AC_ARG_WITH([profiler],
    AS_HELP_STRING([--with-profiler], [Ignore presence of profiler and disable it]))

AS_IF([test "x$with_profiler" = "xyes"],
      [AC_CHECK_LIB([profiler], [ProfilerStart], [have_profiler=yes], [have_profiler=no])],
      [have_profiler=no])

AS_IF([test "x$have_profiler" = "xyes"],
      [LDFLAGS="$LDFLAGS -lprofiler"],
      [AS_IF([test "x$with_profiler" = "xyes"],
             [AC_MSG_ERROR([profiler requested but not found])
      ])
])

AC_CONFIG_FILES(Makefile src/Makefile src/nlpcommon/Makefile)
AC_OUTPUT

