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
                return (self.left_ear == rhs.left_ear) and \
                       (self.right_ear == rhs.right_ear) and \
                       (self.left_face == rhs.left_face) and \
                       (self.right_face == rhs.right_face)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}

%include <pkmn/calculations/spinda_spots.hpp>
