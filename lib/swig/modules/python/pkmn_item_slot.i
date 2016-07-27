/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_slot.hpp>
%}

%include <types/stl_macros.i>

%extend pkmn::item_slot_t {
    item_slot_t() {
        return new pkmn::item_slot_t;
    }

    item_slot_t(
        const pkmn::database::item_entry &entry,
        const int item_amount
    ) {
        pkmn::item_slot_t* ret = new pkmn::item_slot_t;
        ret->item = entry;
        ret->amount = item_amount;
        return ret;
    }

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

%rename(item_slot) pkmn::item_slot_t;
%include <pkmn/item_slot.hpp>
PKMN_PYTHON_VECTOR(pkmn::item_slot_t, item_slot_list)
