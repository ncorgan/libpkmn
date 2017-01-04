/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/levelup_move.hpp>
%}

%csmethodmodifiers move "
/// <summary>Entry for move.</summary>
public";

%csmethodmodifiers level "
/// <summary>Level at which Pokémon learns the move.</summary>
public";

%include <csharp/stl_macros.i>

%include <pkmn/database/levelup_move.hpp>
PKMN_CSHARP_VECTOR(pkmn::database::levelup_move, LevelupMove, LevelupMoveList)
