/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/calculations/spinda_spots.hpp>
%}

%extend pkmn::calculations::spinda_coords {
    %pythoncode %{
        def __eq__(self, rhs):
            if self is rhs:
                return True
            elif not isinstance(rhs, spinda_coords):
                return False
            else:
                return (self.x == rhs.x) and \
                       (self.y == rhs.y)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}

%extend pkmn::calculations::spinda_spots {
    %pythoncode %{
        def __eq__(self, rhs):
            if self is rhs:
                return True
            elif not isinstance(rhs, spinda_spots):
                return False
            else:
                return (self.top_left == rhs.top_left) and \
                       (self.top_right == rhs.top_right) and \
                       (self.bottom_left == rhs.bottom_left) and \
                       (self.bottom_right == rhs.bottom_right)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}

%include <pkmn/calculations/spinda_spots.hpp>
