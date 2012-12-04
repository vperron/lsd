# 
# Interface file generator Makefile.am
#
# Victor Perron <victor@iso3103.net> Dec 2012
#

PYTHON_WRAP = shuppan_wrap_python.cpp
SWIG_SOURCES = shuppan.i
BUILT_SOURCES = $(PYTHON_WRAP)

pkgpython_PYTHON = shuppan.py
pkgpyexec_LTLIBRARIES = _shuppan.la
_shuppan_la_SOURCES = $(PYTHON_WRAP) $(SWIG_SOURCES) shuppan.cpp
_shuppan_la_CPPFLAGS = $(SWIG_PYTHON_CPPFLAGS) 
_shuppan_la_LDFLAGS = -module -lshuppan

$(PYTHON_WRAP) : $(SWIG_SOURCES)
	$(SWIG) -c++ -threads $(SWIG_PYTHON_OPT) -I$(top_srcdir)/src -I$(top_srcdir)/include -o $@ $<

CLEANFILES=\
					 $(BUILT_SOURCES) \
					 $(pkgpython_PYTHON) \
					 *.h
