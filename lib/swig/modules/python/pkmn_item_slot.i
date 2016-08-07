/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_slot.hpp>
%}

%include <python/stl_macros.i>

%extend pkmn::item_slot {
    %pythoncode %{
        def __eq__(self, rhs):
            if self is rhs:
                return True
            elif not isinstance(rhs, item_slot):
                return False
            else:
                return (self.item == rhs.item) and \
                       (self.amount == rhs.amount)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}

%include <pkmn/item_slot.hpp>
PKMN_PYTHON_VECTOR(pkmn::item_slot, item_slot_list)
