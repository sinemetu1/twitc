AC_INIT([twitc], [-], [samdgarrett@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror])

AC_CONFIG_SRCDIR([src/])
AC_CONFIG_TESTDIR([src/test])
AC_CONFIG_MACRO_DIR([m4])

AC_PATH_PROG(SED, sed, "", $PATH:/bin:/usr/bin:/usr/local/bin)
if test -z "$SED"; then
  AC_MSG_WARN([sed was not found])
fi

AC_MSG_CHECKING([libtwitc version])
VERSION=`$SED -ne 's/^#define LIBTWITC_VERSION "\(.*\)"/\1/p' ${srcdir}/src/twitc.h 2>/dev/null`
if test -z "$VERSION"; then
  AC_MSG_ERROR([version number can not be retrieved from src/twitc.h])
fi

CFLAGS="$CFLAGS "
CFLAGS+="-Wall -Wstrict-prototypes -Wmissing-prototypes "
CFLAGS+="-Wmissing-declarations -Wshadow -Wpointer-arith -Wcast-qual "
CFLAGS+="-Wsign-compare -Iincludes -g "
AC_SUBST([CFLAGS])

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CC_C99
AM_PROG_AR
AC_LANG([C])
LT_INIT

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/test/Makefile
])

AC_CHECK_FUNCS([strdup])
AC_FUNC_MALLOC
AC_CHECK_HEADERS(curl/curl.h)
AC_CHECK_HEADERS(json/json.h)
AC_CHECK_LIB([curl], [curl_global_init])
AC_CHECK_LIB([json-c], [json_object_new_object])
AC_CHECK_LIB([oauth], [oauth_gen_nonce])

AC_OUTPUT

AC_MSG_NOTICE([
  twitc configured:
  -----------------------
  version: $VERSION
])
