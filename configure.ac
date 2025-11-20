AC_INIT([libsha1], [0.0.1], [greek.team.blog2@gmail.com])
AM_INIT_AUTOMAKE
LT_INIT
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_C_BIGENDIAN([ 
    AC_DEFINE([WORDS_BIGENDIAN], [1], [Define to 1 if big-endian]) 
    AC_SUBST([ENDIAN], [big]) 
], [ 
    AC_DEFINE([WORDS_BIGENDIAN], [0], [Define to 1 if big-endian]) 
    AC_SUBST([ENDIAN], [little]) 
], [
    AC_MSG_ERROR([Cannot determine system endianness])
], [
    AC_MSG_WARN([Universal binaries not fully supported])
])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
