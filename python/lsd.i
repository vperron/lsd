/* File : lsd.i */
%module(directors="1") lsd

%{
#include "lsd.hpp"
%}


%include "typemaps.i"
%include "std_string.i"

%typemap(in) (const uint8_t *arg0, size_t len)
{
  if (!PyString_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a string");
    return NULL;
  }
  $1 = (uint8_t*) PyString_AsString($input);
  $2 = PyString_Size($input); 
}



%typemap(argout) (const uint8_t *arg0, size_t len) {
   PyObject *o;
%#if PY_VERSION_HEX >= 0x03000000
   o = PyUnicode_FromStringAndSize((const char*)$1,$2);
%#else
   o = PyString_FromStringAndSize((const char*)$1,$2);
%#endif
   $result = SWIG_Python_AppendOutput($result,o);
}


%feature("director") LsdApi;

%include "lsd.hpp"

