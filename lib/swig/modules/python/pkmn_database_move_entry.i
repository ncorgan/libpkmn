/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/move_entry.hpp>
%}

%include <python/stl_macros.i>

%extend pkmn::database::move_entry {
    %pythoncode %{
        def __eq__(self, rhs):
            if self is rhs:
                return True
            elif not isinstance(rhs, move_entry):
                return False
            else:
                return (self.get_name() == rhs.get_name()) and \
                       (self.get_game() == rhs.get_game())

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}

%include <pkmn/database/move_entry.hpp>
PKMN_PYTHON_VECTOR(pkmn::database::move_entry, move_entry_list)
