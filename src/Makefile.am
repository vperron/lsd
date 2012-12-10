lib_LTLIBRARIES = liblsd.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblsd.pc

include_HEADERS = \
    ../include/lsd.h

liblsd_la_SOURCES = \
    lsd.c

if DISABLE_VERSION
liblsd_la_LDFLAGS = -avoid-version
else
liblsd_la_LDFLAGS = -version-info @LTVER@
endif

if HOST_ANDROID
liblsd_la_LDFLAGS += -llog
endif

INCLUDES = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/liblsd.la

noinst_PROGRAMS = lsd_tester lsd_listener lsd_sender

lsd_listener_SOURCES = lsd_listener.c
lsd_sender_SOURCES = lsd_sender.c
lsd_tester_SOURCES = lsd_tester.c

TESTS = lsd_selftest
