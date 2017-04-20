/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
#include <ruby/encoding.h>
%}

%fragment("SWIG_FromCharPtrAndSize","header",fragment="SWIG_pchar_descriptor") {
SWIGINTERNINLINE VALUE 
SWIG_FromCharPtrAndSize(const char* carray, size_t size)
{
  if (carray) {
    if (size > LONG_MAX) {
      swig_type_info* pchar_descriptor = SWIG_pchar_descriptor();
      return pchar_descriptor ? 
	SWIG_NewPointerObj(%const_cast(carray,char *), pchar_descriptor, 0) : Qnil;
    } else {
      VALUE ret = rb_str_new(carray, %numeric_cast(size,long));
      return rb_str_export_to_enc(ret, rb_utf8_encoding());
    }
  } else {
    return Qnil;
  }
}
}
