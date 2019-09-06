dnl ++ Starting
dnl +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

AC_PREREQ(2.50)
AC_INIT(dirb,2.22,darkraver@open-labs.org)
AC_CONFIG_SRCDIR(src/dirb.c)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)

AC_CONFIG_FILES(Makefile src/Makefile gendict_src/Makefile web2dic/Makefile)


dnl ++ Checks for curl-config
dnl +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 AC_CACHE_VAL(my_cv_curl_vers,[
 my_cv_curl_vers=NONE
 check="7.10.1"
 check_hex="070A01"
 
 AC_MSG_CHECKING([for libcurl >= $check])
 
 if eval curl-config --version 2>/dev/null >/dev/null; then
   ver=`curl-config --version | sed -e "s/libcurl //g"`
   hex_ver=`curl-config --vernum | tr 'a-f' 'A-F'`
   int_ver=`printf %i 0x$hex_ver`
   check_int=`printf %i 0x$check_hex`

   if test $int_ver -ge $check_int; then
     my_cv_curl_vers="$ver"
     AC_MSG_RESULT([$my_cv_curl_vers])
   else
     AC_MSG_RESULT(FAILED)
     AC_MSG_ERROR([LibCurl version $ver is too old. Need version $check or higher.])
   fi

 else
   AC_MSG_RESULT(FAILED)
   AC_MSG_ERROR([Curl-config was not found])
 fi
 ])

NETWORK_CFLAGS="`curl-config --cflags`"
NETWORK_LIBS="`curl-config --libs`"
AC_SUBST(NETWORK_CFLAGS)
AC_SUBST(NETWORK_LIBS)


dnl ++ Checks for curl_easy_init in libcurl
dnl +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

AC_CHECK_LIB(curl, curl_easy_init,, AC_MSG_ERROR(Can't find function curl_easy_init in -lcurl. LibCurl is required.))


dnl ++ Final message
dnl +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

AC_OUTPUT
echo "                                     "
echo " DIRB ${VERSION} build configuration."
echo "                                     "
echo " Now you must execute: \"make\"      "
echo "                                     "
