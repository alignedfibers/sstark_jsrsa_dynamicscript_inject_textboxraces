AC_INIT([libjingle-gtalx], [0.1.0.1], [bjdodo@hotmail.com])
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-zip])
AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL
LIBTOOL="$LIBTOOL --silent"
AC_PROG_INSTALL
#AC_DEFINE(PRODUCTION_BUILD, 1, [Build as a production build])
#AC_DEFINE(PRODUCTION, 1, [Build as a production build])
AC_DEFINE(DEBUG, 1, [Build as a debug build])
AC_DEFINE(POSIX, 1, [If we're using configure, we're on POSIX])
AC_DEFINE(FEATURE_ENABLE_SSL, 1, [Enable SSL])
AC_DEFINE(FEATURE_ENABLE_CHAT_ARCHIVING, 1, [Chat archving])
AC_DEFINE(FEATURE_ENABLE_VOICEMAIL, 1, [voice mail])
AC_DEFINE(LOGGING, 1, [Logging])

HAVE_EXPAT=no
AC_CHECK_LIB(expat, XML_ParserCreate, HAVE_EXPAT="yes")
if test "x$HAVE_EXPAT" = xyes ; then
  EXPAT_LIBS="-lexpat"
  AC_SUBST(EXPAT_LIBS)
else
  AC_ERROR([Expat is required to build libjingle. You can get it from http://expat.sourceforge.net/])
fi

enable_phone=yes
if test `uname -s` = Linux ; then
  AC_DEFINE(LINUX, 1, [Building on Linux])
  if test x`ls talk/third_party/gips/VoiceEngine_Linux_gcc.a` != x ; then
    enable_gips=yes
    MEDIA_LIBS=$PWD/talk/third_party/gips/VoiceEngine_Linux_gcc.a
  fi 
elif test `uname -s` = Darwin ; then
  AC_DEFINE(OSX, 1, [Building on OSX])
  if test x`ls talk/third_party/gips/VoiceEngine_mac_gcc.a` != x ; then
    enable_gips=yes
    MEDIA_LIBS="$PWD/talk/third_party/gips/VoiceEngine_mac_gcc.a -framework CoreAudio -framework AudioToolbox"
  fi
fi

if test x$enable_gips = xyes ; then
  AC_DEFINE(HAVE_GIPS, 1, [Have GIPS Voice Engine])
elif test `uname -s` = Linux ; then

AC_CHECK_HEADERS(alsa/asoundlib.h,
	[AC_CHECK_LIB(asound, snd_pcm_open,
		[ALSA_LIBS="-lasound" ; AC_DEFINE(__ALSA_ENABLED__,1,[Defined when alsa support is enabled]) ])
	]
)
AC_SUBST(ALSA_LIBS)

PKG_CHECK_MODULES(GLIB, glib-2.0 gmodule-2.0 gthread-2.0, enable_glib=yes, enable_glib=no)
if test x$enable_glib = xno; then
	enable_phone=no
	AC_MSG_NOTICE([GLib 2.0 is required to build libjingle. You can get it from http://www.gtk.org/])
else
	AC_SUBST(GLIB_CFLAGS)
	AC_SUBST(GLIB_LIBS)
	AC_DEFINE(HAVE_GLIB, 1, [Glib is required for oRTP code])
fi

PKG_CHECK_MODULES(ORTP, ortp >= 0.7.0, enable_ortp=yes, enable_ortp=no)
if test x$enable_ortp = xno ; then
	enable_phone=no
	AC_MSG_NOTICE([oRTP is required to build libjingle. You can get it from http://www.linphone.org/ortp/])
else
	AC_SUBST(ORTP_CFLAGS)
	AC_SUBST(ORTP_LIBS)
	AC_DEFINE(HAVE_ORTP, 1, [oRTP provides RTP supprt])
fi

PKG_CHECK_MODULES(SPEEX, speex >= 1.1.6, enable_speex=yes, enable_speex=no)
if test x$enable_speex = xno ; then
	enable_phone=no
	AC_MSG_NOTICE([speex is required to build libjingle. You can get it from http://www.speex.org])
else
	AC_SUBST(SPEEX_CFLAGS)
	AC_SUBST(SPEEX_LIBS)
	AC_DEFINE(HAVE_SPEEX, 1, [speex provides speex support])
        speex_found=yes
fi

#PKG_CHECK_MODULES(MEDIASTREAMER, mediastreamer >= 0.0.0, enable_mediastreamer=yes, enable_mediastreamer=no)
#if test x$enable_mediastreamer = xno ; then
#	enable_phone=no
#	AC_MSG_NOTICE([mediastreamer is required to build libjingle. You can get it from http://download.savannah.gnu.org/releases/linphone/mediastreamer/])
#else
#	AC_SUBST(MEDIASTREAMER_CFLAGS)
#	AC_SUBST(MEDIASTREAMER_LIBS)
#	AC_DEFINE(HAVE_MEDIASTREAMER, 1, [mediastreamer found])
#        mediastreamer_found=yes
#fi

PKG_CHECK_MODULES(MEDIASTREAMER, mediastreamer >= 2.7.0, enable_new_mediastreamer=yes, enable_new_mediastreamer=no)
if test x$enable_new_mediastreamer = xno ; then
        PKG_CHECK_MODULES(MEDIASTREAMER, mediastreamer >= 2.0.0, enable_mediastreamer=yes, enable_mediastreamer=no)
        if test x$enable_mediastreamer = xno ; then
                enable_phone=no
                AC_MSG_NOTICE([mediastreamer is required to build libjingle. You can get it from http://download.savannah.gnu.org/releases/linphone/mediastreamer/])
        else
                AC_SUBST(MEDIASTREAMER_CFLAGS)
                AC_SUBST(MEDIASTREAMER_LIBS)
                AC_DEFINE(HAVE_MEDIASTREAMER, 1, [mediastreamer found])
                mediastreamer_found=yes
                AC_MSG_NOTICE([old mediastreamer detected])
        fi
else
        AC_SUBST(MEDIASTREAMER_CFLAGS)
        AC_SUBST(MEDIASTREAMER_LIBS)
        AC_DEFINE(HAVE_MEDIASTREAMER, 1, [mediastreamer found])
        AC_DEFINE(HAVE_MEDIASTREAMER28, 1, [mediastreamer found])
        mediastreamer_found=yes
        AC_MSG_NOTICE([new mediastreamer detected])
fi


AC_ARG_WITH( ilbc,
      [  --with-ilbc      Set prefix where ilbc headers and libs can be found (ex:/usr, /usr/local, none to disable ilbc support) [default=/usr] ],
      [ ilbc_prefix=${withval}],[ ilbc_prefix="/usr" ])

if test "$ilbc_prefix" = "none" ; then
        AC_MSG_NOTICE([iLBC codec support disabled. ])
else
        ILBC_CFLAGS=" -I${ilbc_prefix}/include/ilbc"
        ILBC_LIBS="-L${ilbc_prefix}/lib -lilbc -lm"
        CPPFLAGS_save=$CPPFLAGS
        CPPFLAGS=$ILBC_CFLAGS
        LDFLAGS_save=$LDFLAGS
        LDFLAGS=$ILBC_LIBS
        AC_CHECK_HEADERS(iLBC_decode.h,[AC_CHECK_LIB(ilbc,iLBC_decode,ilbc_found=yes,ilbc_found=no)
        ],ilbc_found=no)

        CPPFLAGS=$CPPFLAGS_save
        LDFLAGS=$LDFLAGS_save

        if test "$ilbc_found" = "no" ; then
                AC_MSG_WARN([Could not find ilbc headers or libs. Please install ilbc package from http://www.linphone.org if you want iLBC codec support in libjingle.])
                ILBC_CFLAGS=
                ILBC_LIBS=
        else
                AC_DEFINE(HAVE_ILBC,1,[Defined when we have ilbc codec lib])
                AC_SUBST(ILBC_CFLAGS)
                AC_SUBST(ILBC_LIBS)
        fi
fi

else
  enable_phone=no
fi  #  Linux check

AM_CONDITIONAL(GIPS, test x$enable_gips = xyes)
AM_CONDITIONAL(PHONE, test x$enable_phone = xyes)
AC_SUBST(MEDIA_LIBS)

AC_OUTPUT([Makefile
	  talk/Makefile
          talk/base/Makefile
	  talk/login/Makefile
	  talk/chat/Makefile
	  talk/examples/Makefile
	  talk/examples/call/Makefile
	  talk/examples/pcp/Makefile
	  talk/p2p/Makefile
	  talk/p2p/base/Makefile
    	  talk/p2p/client/Makefile
          talk/session/Makefile
	  talk/session/fileshare/Makefile
          talk/session/tunnel/Makefile
          talk/session/phone/Makefile
          talk/xmllite/Makefile
	  talk/xmpp/Makefile
          interface/Makefile
	])

echo
echo $PACKAGE $VERSION
echo

if test x$enable_phone = xyes ; then
	echo
	echo Supported Examples: call pcp
	echo Supported Codecs:
	if test x$enable_gips = xyes ; then
		echo GIPS: yes
	else
		echo Speex: $speex_found
		echo iLBC:  $ilbc_found
		echo MULAW: yes
	fi
else
	echo Supported Examples: pcp
fi
echo
