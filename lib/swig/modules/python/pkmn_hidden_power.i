/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/calculations/hidden_power.hpp>
%}

%extend pkmn::calculations::hidden_power {
    %pythoncode %{
        def __eq__(self, rhs):
            if self is rhs:
                return True
            elif not isinstance(rhs, hidden_power):
                return False
            else:
                return (self.type == rhs.type) and \
                       (self.base_power == rhs.base_power)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}

%include <pkmn/calculations/hidden_power.hpp>
