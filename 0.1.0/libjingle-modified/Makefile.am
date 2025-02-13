SUBDIRS=talk interface

noinst_HEADERS = config.h

EXTRA_DIST=README.win DOCUMENTATION

dist-hook:
	sed -i -f libjingle-modified/talk/sanitize.sed `find $(distdir) -type f`
