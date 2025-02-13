## Makefile.am -- Process this file with automake to produce Makefile.in

bin_PROGRAMS = gtalx

gtalx_SOURCES = jinglecallbackimpl.cpp main.cpp dlgconnect.cpp dlgmain.cpp \
		contactsmanager.cpp contactswidget.cpp settingsmanager.cpp smileysmanager.cpp utils.cpp dlgcall.cpp dlgabout.cpp \
		moc_dlgconnect.cpp moc_dlgmain.cpp moc_contactswidget.cpp moc_dlgcall.cpp moc_dlgabout.cpp \
		translator.cpp calldlgmanager.cpp moc_jinglecallbackimpl.cpp moc_calldlgmanager.cpp \
		trayiconmanager.cpp moc_trayiconmanager.cpp themesmanager.cpp dlgsettings.cpp moc_dlgsettings.cpp

noinst_HEADERS = jinglecallbackimpl.h main.h dlgconnect.h \
		dlgmain.h contactsmanager.h contactswidget.h settingsmanager.h smileysmanager.h utils.h dlgcall.h dlgabout.h \
		translator.h calldlgmanager.h trayiconmanager.h themesmanager.h dlgsettings.h

gtalx_CXXFLAGS = $(AM_CXXFLAGS) \
		$(ORTP_CFLAGS) $(SPEEX_CFLAGS) $(MEDIASTREAMER_CFLAGS) \
		-I$(QT4INCDIR)/QtCore -I$(QT4INCDIR)/QtGui \
		-I$(QT4INCDIR) \
		-I. -I$(top_srcdir)/../libjingle-modified $(GLIB_CFLAGS) -I/usr/include -I/usr/local/include -ggdb -O0

gtalx_LDFLAGS =
gtalx_LDADD =   \
               $(top_srcdir)/../libjingle-modified/interface/libjingleinterface.la \
	           -lQtCore -L$(QT4LIBDIR) -lQtGui -L$(QT4LIBDIR)


AM_CPPFLAGS  = -DPOSIX