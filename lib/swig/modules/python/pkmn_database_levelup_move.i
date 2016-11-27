/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/levelup_move.hpp>
%}

%include <python/stl_macros.i>

%extend pkmn::database::levelup_move {
    %pythoncode %{
        def __eq__(self, rhs):
            if self is rhs:
                return True
            elif not isinstance(rhs, levelup_move):
                return False
            else:
                return (self.move == rhs.move) and \
                       (self.level == rhs.level)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}

%include <pkmn/database/levelup_move.hpp>
PKMN_PYTHON_VECTOR(pkmn::database::levelup_move, levelup_move_list)
