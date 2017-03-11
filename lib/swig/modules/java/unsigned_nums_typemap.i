/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * SWIG does no high-bounds check on numbers, which leads to the possibility of
 * invalid numbers being passed into functions where it matters, such as personality
 * or trainer IDs.
 */

%{
    #include <limits>
%}

%typemap(in) unsigned short, unsigned int, unsigned long %{
    if($input < 0) {
        SWIG_JavaException(jenv, SWIG_OverflowError, "Number must be non-negative");
        return $null;
    } else if($input > ($type)(std::numeric_limits<$type>::max())) {
        SWIG_JavaException(jenv, SWIG_OverflowError, "Number is too large");
        return $null;
    } else {
        $1 = ($type)$input;
    }
%}
