dnl $Id$
dnl config.m4 for extension asiohttp2

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(asiohttp2, for asiohttp2 support,
dnl  Make sure that the comment is aligned:
dnl [  --with-asiohttp2             Include asiohttp2 support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(asiohttp2, whether to enable asiohttp2 support,
 Make sure that the comment is aligned:
 [  --enable-asiohttp2           Enable asiohttp2 support])

if test "$PHP_ASIOHTTP2" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-asiohttp2 -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/asiohttp2.h"  # you most likely want to change this
  dnl if test -r $PHP_ASIOHTTP2/$SEARCH_FOR; then # path given as parameter
  dnl   ASIOHTTP2_DIR=$PHP_ASIOHTTP2
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for asiohttp2 files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ASIOHTTP2_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ASIOHTTP2_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the asiohttp2 distribution])
  dnl fi

  dnl # --with-asiohttp2 -> add include path
  dnl PHP_ADD_INCLUDE($ASIOHTTP2_DIR/include)

  dnl # --with-asiohttp2 -> check for lib and symbol presence
  dnl LIBNAME=asiohttp2 # you may want to change this
  dnl LIBSYMBOL=asiohttp2 # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ASIOHTTP2_DIR/$PHP_LIBDIR, ASIOHTTP2_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ASIOHTTP2LIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong asiohttp2 lib version or lib not found])
  dnl ],[
  dnl   -L$ASIOHTTP2_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ASIOHTTP2_SHARED_LIBADD)
  PHP_REQUIRE_CXX()
  PHP_SUBST(ASIOHTTP2_SHARED_LIBADD)
  PHP_ADD_INCLUDE("/usr/local/include")
  PHP_ADD_LIBRARY(stdc++, , ASIOHTTP2_SHARED_LIBADD)
  PHP_ADD_LIBRARY(nghttp2_asio, , ASIOHTTP2_SHARED_LIBADD)
  PHP_ADD_LIBRARY(ssl, , ASIOHTTP2_SHARED_LIBADD)
  PHP_ADD_LIBRARY(crypto, , ASIOHTTP2_SHARED_LIBADD)
  PHP_ADD_LIBRARY(boost_system, , ASIOHTTP2_SHARED_LIBADD)
  PHP_NEW_EXTENSION(asiohttp2, asiohttp2.cpp http2.cpp, $ext_shared, , "-std=gnu++11", "no")
fi
