/* File : shuppan.i */
%module(directors="1") shuppan

%{
#include "shuppan.hpp"
%}


%include "typemaps.i"
%include "std_string.i"

%typemap(in) (const char *data, size_t len)
{
  if (!PyString_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a string");
    return NULL;
  }
  $1 = PyString_AsString($input);
  $2 = PyString_Size($input); 
}


%typemap(argout) (const char *data, size_t len) {
   PyObject *o;
%#if PY_VERSION_HEX >= 0x03000000
   o = PyUnicode_FromStringAndSize($1,$2);
%#else
   o = PyString_FromStringAndSize($1,$2);
%#endif
   $result = SWIG_Python_AppendOutput($result,o);
}


%feature("director") ShuppanApi;

%include "shuppan.hpp"

