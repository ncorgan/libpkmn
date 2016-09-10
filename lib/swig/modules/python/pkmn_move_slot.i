/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/move_slot.hpp>
%}

%include <python/stl_macros.i>

%extend pkmn::move_slot {
    %pythoncode %{
        def __eq__(self, rhs):
            if self is rhs:
                return True
            elif not isinstance(rhs, move_slot):
                return False
            else:
                return (self.move == rhs.move) and \
                       (self.pp == rhs.pp)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}

%include <pkmn/move_slot.hpp>
PKMN_PYTHON_VECTOR(pkmn::move_slot, move_slot_list)
