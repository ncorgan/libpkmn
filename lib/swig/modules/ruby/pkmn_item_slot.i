/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/item_slot.hpp"
%}

%include <attribute.i>

%attribute(pkmn::swig::item_slot, pkmn::e_item, item, get_item, set_item);
%attribute(pkmn::swig::item_slot, int, amount, get_amount, set_amount);

%include "cpp_wrappers/item_slot.hpp"

// Needed to avoid compile error
%{
    namespace swig
    {
        template <> struct traits<pkmn::swig::item_slot>
        {
            typedef pointer_category category;
            static const char* type_name()
            {
                return "pkmn::swig::item_slot";
            }
        };
    }
%}
