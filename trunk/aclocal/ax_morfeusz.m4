# SYNOPSIS
#
#   AX_MORFEUSZ([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
#
# LICENSE
#
#   Copyright (c) 2008 Steven G. Johnson <stevenj@alum.mit.edu>
#
#   This program is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation, either version 3 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

AC_DEFUN([AX_MORFEUSZ], [

ax_cv_morfeusz=no
ax_cv_morfeusz_cflags=
ax_cv_morfeusz_ldflags=

with_morfeusz=yes
AC_ARG_WITH([morfeusz],
    AS_HELP_STRING([--without-morfeusz], [Ignore presence of morfeusz and disable it]))
AS_IF([test "x$with_morfeusz" != "xno"],
      [have_morfeusz=yes;
       AS_IF([test "x$with_morfeusz" != "xyes"],
              [ax_cv_morfeusz_cflags="-I$with_morfeusz/include";
               ax_cv_morfeusz_ldflags="-L$with_morfeusz/lib"])],
      [have_morfeusz=no])

AC_ARG_WITH([bundled_morfeusz],
    AS_HELP_STRING([--without-bundled-morfeusz], [Do not look in third_party/ for morfeusz]))
AS_IF([test "x$with_bundled_morfeusz" != "xno" -a "x$with_morfeusz" = "xyes"],
	  [ax_cv_morfeusz_cflags="-I$PWD/${ac_top_builddir}third_party/morfeusz/include";
	   ax_cv_morfeusz_ldflags="-L$PWD/${ac_top_builddir}third_party/morfeusz/lib"])

if test "x$have_morfeusz" = "xyes"; then
	AC_MSG_CHECKING([for morfeusz])
	AC_LANG_PUSH([C])
	SAVED_CFLAGS=$CFLAGS
	SAVED_LDFLAGS=$LDFLAGS
	CFLAGS="$CFLAGS $ax_cv_morfeusz_cflags"
	LDFLAGS="$LDFLAGS $ax_cv_morfeusz_ldflags -lmorfeusz"
	AC_LINK_IFELSE(
	[AC_LANG_PROGRAM(
	  [#include <morfeusz.h>],
	  [int main() { morfeusz_about(); return 0; }])],
	[AC_MSG_RESULT([yes])
     MORFEUSZ_CFLAGS="$ax_cv_morfeusz_cflags"
	 MORFEUSZ_LIBS="$ax_cv_morfeusz_ldflags -lmorfeusz"
	 ax_cv_morfeusz=yes],
	[AC_MSG_RESULT([no])])
	LDFLAGS=$SAVED_LDFLAGS
	CFLAGS=$SAVED_CFLAGS
	AC_LANG_POP([C])
fi

if test "x$ax_cv_morfeusz" = "xno"; then
  m4_default([$2],:)
else
  m4_default([$1], [AC_DEFINE(HAVE_MORFEUSZ,1,[Define if Morfeusz is enabled])])
fi
])dnl AX_MORFEUSZ
