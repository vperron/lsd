lib_LTLIBRARIES = libshuppan.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libshuppan.pc

include_HEADERS = \
    ../include/shuppan.h

libshuppan_la_SOURCES = \
    shuppan.c 

if DISABLE_VERSION
libshuppan_la_LDFLAGS = -avoid-version
else
libshuppan_la_LDFLAGS = -version-info @JLTVER@
endif


INCLUDES = -I$(top_srcdir)/include
bin_PROGRAMS = shuppan_tester shuppan_listener
shuppan_tester_LDADD = libshuppan.la
shuppan_listener_LDADD = libshuppan.la

TESTS = shuppan_selftest
