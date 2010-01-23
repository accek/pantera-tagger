# SYNOPSIS
#
#   AX_PROTOBUF([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
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

AC_DEFUN([AX_PROTOBUF], [

ax_cv_protobuf=no

# Check for Google Protocol Buffers
AC_CHECK_PROG(ax_cv_protoc, protoc, yes, no)
if test "x$ax_cv_protoc" = "xyes"; then
  AC_LANG_PUSH([C++])
  SAVED_LDFLAGS=$LDFLAGS
  LDFLAGS="$LDFLAGS -lprotobuf"
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [#include <google/protobuf/message.h>],
      [google::protobuf::Message dummy])],
    [PROTOBUF_LIBS="-lprotobuf";
     ax_cv_protobuf=yes])
  LDFLAGS=$SAVED_LDFLAGS
  AC_LANG_POP([C++])
fi

if test "x$ax_cv_protobuf" = "xno"; then
  m4_default([$2],:)
else
  m4_default([$1], [AC_DEFINE(HAVE_PROTOBUF,1,[Define if Google Protocol Buffers are enabled])])
fi
])dnl AX_PROTOBUF
