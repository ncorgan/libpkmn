/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/types/time_duration.hpp>
%}

%rename(time_duration_internal) pkmn::time_duration;

%include <pkmn/types/time_duration.hpp>
