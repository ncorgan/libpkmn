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

%typemap(in,checkfn="lua_isnumber") unsigned short, unsigned int, unsigned long %{
    SWIG_contract_assert((lua_tonumber(L, $input) >= 0), "Number must be non-negative")
    SWIG_contract_assert((lua_tonumber(L, $input) <= lua_Number(std::numeric_limits<$type>::max())), "Number is too large")
    $1 = ($type)lua_tonumber(L, $input);
%}
