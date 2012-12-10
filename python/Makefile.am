# 
# Interface file generator Makefile.am
#
# Victor Perron <victor@iso3103.net> Dec 2012
#

PYTHON_WRAP = lsd_wrap_python.cpp
SWIG_SOURCES = lsd.i
BUILT_SOURCES = $(PYTHON_WRAP)

pkgpython_PYTHON = lsd.py
pkgpyexec_LTLIBRARIES = _lsd.la
_lsd_la_SOURCES = $(PYTHON_WRAP) $(SWIG_SOURCES) lsd.cpp
_lsd_la_CPPFLAGS = $(SWIG_PYTHON_CPPFLAGS) 
_lsd_la_LDFLAGS = -module -llsd

$(PYTHON_WRAP) : $(SWIG_SOURCES)
	$(SWIG) -c++ -threads $(SWIG_PYTHON_OPT) -I$(top_srcdir)/src -I$(top_srcdir)/include -o $@ $<

CLEANFILES=\
					 $(BUILT_SOURCES) \
					 $(pkgpython_PYTHON) \
					 *.h
