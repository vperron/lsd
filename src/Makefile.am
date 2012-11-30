lib_LTLIBRARIES = libshuppan.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libshuppan.pc

include_HEADERS = \
    ../include/shuppan.h

libshuppan_la_SOURCES = \
    shuppan.cc

libshuppan_la_CPPFLAGS = -Werror

if DISABLE_VERSION
libshuppan_la_LDFLAGS = -avoid-version
else
libshuppan_la_LDFLAGS = -version-info @JLTVER@
endif


INCLUDES = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/libshuppan.la

noinst_PROGRAMS = shuppan_tester shuppan_listener

shuppan_listener_SOURCES = shuppan_listener.cc
shuppan_tester_SOURCES = shuppan_tester.cc

TESTS = shuppan_selftest
