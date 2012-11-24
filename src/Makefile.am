lib_LTLIBRARIES = libshuppan.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libshuppan.pc

include_HEADERS = \
    ../include/shuppan.h

libshuppan_la_SOURCES = \
    shuppan_service.c 

INCLUDES = -I$(top_srcdir)/include
bin_PROGRAMS = shuppan_tester
shuppan_tester_LDADD = libshuppan.la

TESTS = shuppan_selftest
