/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/item_entry.hpp>
%}

%extend pkmn::database::item_entry {
    %pythoncode %{
        def __eq__(self, rhs):
            if self is rhs:
                return True
            elif not isinstance(rhs, item_entry):
                return False
            else:
                return (self.get_name() == rhs.get_name()) and \
                       (self.get_game() == rhs.get_game())

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}

%include <pkmn/database/item_entry.hpp>
