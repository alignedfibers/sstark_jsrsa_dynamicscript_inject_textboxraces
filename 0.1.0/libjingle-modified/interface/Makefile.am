
libjingleinterface_la_SOURCES = callclient.cpp jinglewrapper.cpp fileclient.cpp errorhandler.cpp contactsmanager.cpp globalsettings.cpp

noinst_HEADERS = callclient.h jinglewrapper.h libjingleif.h fileclient.h errorhandler.h contactsmanager.h globalsettings.h

AM_CPPFLAGS = -DLINUX -DPOSIX -DINTERNAL_BUILD

noinst_LTLIBRARIES= libjingleinterface.la

libjingleinterface_la_LIBADD =   \
	       $(top_srcdir)/../libjingle-modified/talk/login/libcricketlogin.la \
	       $(top_srcdir)/../libjingle-modified/talk/chat/libcricketchat.la \
	       $(top_srcdir)/../libjingle-modified/talk/session/phone/libcricketsessionphone.la \
	       $(top_srcdir)/../libjingle-modified/talk/p2p/client/libcricketp2pclient.la \
	       $(top_srcdir)/../libjingle-modified/talk/p2p/base/libcricketp2pbase.la \
	       $(top_srcdir)/../libjingle-modified/talk/xmpp/libcricketxmpp.la \
	       $(top_srcdir)/../libjingle-modified/talk/xmllite/libcricketxmllite.la \
	       $(top_srcdir)/../libjingle-modified/talk/base/libcricketbase.la \
               $(top_srcdir)/../libjingle-modified/talk/session/fileshare/libcricketsessionfileshare.la \
               $(top_srcdir)/../libjingle-modified/talk/session/tunnel/libcricketsessiontunnel.la \
	       $(EXPAT_LIBS) -lpthread -lssl -lcrypto $(GLIB_LIBS) \
	       $(SPEEX_LIBS) $(ORTP_LIBS) $(MEDIASTREAMER_LIBS)