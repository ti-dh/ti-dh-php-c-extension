dnl $Id$
dnl config.m4 for extension dh

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(dh, for dh support,
dnl Make sure that the comment is aligned:
dnl [  --with-dh             Include dh support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(dh, whether to enable dh support,
Make sure that the comment is aligned:
[  --enable-dh           Enable dh support])

if test "$PHP_DH" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-dh -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/dh.h"  # you most likely want to change this
  dnl if test -r $PHP_DH/$SEARCH_FOR; then # path given as parameter
  dnl   DH_DIR=$PHP_DH
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for dh files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       DH_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$DH_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the dh distribution])
  dnl fi

  dnl # --with-dh -> add include path
  dnl PHP_ADD_INCLUDE($DH_DIR/include)

  dnl # --with-dh -> check for lib and symbol presence
  dnl LIBNAME=dh # you may want to change this
  dnl LIBSYMBOL=dh # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $DH_DIR/$PHP_LIBDIR, DH_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_DHLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong dh lib version or lib not found])
  dnl ],[
  dnl   -L$DH_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(DH_SHARED_LIBADD)

  PHP_NEW_EXTENSION(dh, dh.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
