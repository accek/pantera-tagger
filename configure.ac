dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(btagger, 1.0)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CC
AC_PROG_CXX

AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)

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

AC_LANG_PUSH(C++)
AX_OPENMP
AC_LANG_POP(C++)

CXXFLAGS="$CXXFLAGS $BOOST_CPPFLAGS $OPENMP_CXXFLAGS"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS $BOOST_REGEX_LIB"

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

