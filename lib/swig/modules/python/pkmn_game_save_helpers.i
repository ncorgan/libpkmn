/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/time_duration.hpp"
%}

%include <attribute.i>

//
// Time Played helper
//

%ignore pkmn::swig::game_save_time_played::game_save_time_played(const pkmn::game_save::sptr&);

// Replace methods with attributes for more idiomatic Python.
%attribute(pkmn::swig::game_save_time_played, int, hours, get_hours, set_hours);
%attribute(pkmn::swig::game_save_time_played, int, minutes, get_minutes, set_minutes);
%attribute(pkmn::swig::game_save_time_played, int, seconds, get_seconds, set_seconds);
%attribute(pkmn::swig::game_save_time_played, int, frames, get_frames, set_frames);

%include "cpp_wrappers/time_duration.hpp"
