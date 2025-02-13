#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([gtalx], [0.1.0.1], [bjdodo@hotmail.com])
AM_INIT_AUTOMAKE
## this section needs ltmain.sh that I copied from /usr/share/libtool
AM_PROG_LIBTOOL
LIBTOOL="$LIBTOOL --silent"

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AC_DEFINE(DEBUG, 1, [Build as a debug build])

# Checks for libraries.

HAVE_EXPAT=no
AC_CHECK_LIB(expat, XML_ParserCreate, HAVE_EXPAT="yes")
if test "x$HAVE_EXPAT" = xyes ; then
  EXPAT_LIBS="-lexpat"
  AC_SUBST(EXPAT_LIBS)
else
  AC_ERROR([Expat is required to build libjingle. You can get it from http://expat.sourceforge.net/])
fi

AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)
AC_DEFINE(HAVE_GLIB, 1, [Glib is required for oRTP code])

## QT stuff based on http://www.filewatcher.com/p/kadu-0.4.0.tar.bz2.1198118/kadu/configure.ac.html

QT4LIBDIR=""
QT4_SEARCH="/usr/lib /usr/lib/qt4 /usr/lib/qt4/lib /usr/local/qt4 /usr/local/qt4/lib /usr/lib64 /usr/lib64/qt4 /usr/lib/i386-linux-gnu /usr/lib/i386-linux-gnu/qt4 /usr/lib/x86_64-linux-gnu/qt4 /usr/lib/x86_64-linux-gnu"
for i in $QT4_SEARCH; do
    if test -f $i/libQt3Support.so -a x$QT4LIBDIR = x; then QT4LIBDIR=$i; fi
done

if test "x$QT4LIBDIR" = x ; then
  AC_ERROR([qt4 libraries not found. qt4 is required to build gtalx.])
fi

AC_SUBST(QT4LIBDIR)

echo "qt4 lib directory: " $QT4LIBDIR

QT4INCDIR=""
QT4_SEARCH="/usr/lib/qt4 /usr/lib/qt4/include /usr/local/qt4 /usr/local/qt4/include /usr/include /usr/include/qt4 /usr/local/lib/qt4 /usr/local/include /usr/local/include/qt4"
for i in $QT4_SEARCH; do
    if test -f $i/Qt3Support/q3accel.h -a x$QT4INCDIR = x; then QT4INCDIR=$i; fi
done

if test "x$QT4INCDIR" = x ; then
  AC_ERROR([qt4 includes not found. qt4 is required to build gtalx.])
fi

AC_SUBST(QT4INCDIR)

echo "qt4 include directory: " $QT4INCDIR

PKG_CHECK_MODULES(ORTP, ortp >= 0.7.0, enable_ortp=yes, enable_ortp=no)
if test x$enable_ortp = xno ; then
	enable_phone=no
	AC_MSG_NOTICE([oRTP is required to build gtalx. You can get it from http://www.linphone.org/ortp/])
else
	AC_SUBST(ORTP_CFLAGS)
	AC_SUBST(ORTP_LIBS)
	AC_DEFINE(HAVE_ORTP, 1, [oRTP provides RTP supprt])
fi

PKG_CHECK_MODULES(SPEEX, speex >= 1.1.6, enable_speex=yes, enable_speex=no)
if test x$enable_speex = xno ; then
	enable_phone=no
	AC_MSG_NOTICE([speex is required to build gtalx. You can get it from http://www.speex.org])
else
	AC_SUBST(SPEEX_CFLAGS)
	AC_SUBST(SPEEX_LIBS)
	AC_DEFINE(HAVE_SPEEX, 1, [speex provides speex support])
        speex_found=yes
fi

PKG_CHECK_MODULES(MEDIASTREAMER, mediastreamer >= 0.0.0, enable_mediastreamer=yes, enable_mediastreamer=no)
if test x$enable_mediastreamer = xno ; then
	enable_phone=no
	AC_MSG_NOTICE([mediastreamer is required to build gtalx. You can get it from http://download.savannah.gnu.org/releases/linphone/mediastreamer/])
else
	AC_SUBST(MEDIASTREAMER_CFLAGS)
	AC_SUBST(MEDIASTREAMER_LIBS)
	AC_DEFINE(HAVE_MEDIASTREAMER, 1, [mediastreamer found])
        mediastreamer_found=yes
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
#AC_HEADER_STDBOOL
#AC_C_CONST

# Checks for library functions.
#AC_CHECK_FUNCS([mkdir strerror])

AC_OUTPUT([Makefile
           src/Makefile
           share/Makefile
           share/gtalx/Makefile
           share/gtalx/languagefiles/Makefile
           share/gtalx/smileys/Makefile
           share/gtalx/smileys/default/Makefile
	   share/gtalx/themes/Makefile
	   share/gtalx/themes/default/Makefile
	])

pushd src
moc calldlgmanager.h > moc_calldlgmanager.cpp
moc contactswidget.h > moc_contactswidget.cpp
moc dlgabout.h > moc_dlgabout.cpp
moc dlgcall.h > moc_dlgcall.cpp
moc dlgconnect.h > moc_dlgconnect.cpp
moc dlgmain.h > moc_dlgmain.cpp
moc dlgsettings.h > moc_dlgsettings.cpp
moc jinglecallbackimpl.h > moc_jinglecallbackimpl.cpp
moc trayiconmanager.h > moc_trayiconmanager.cpp
popd
