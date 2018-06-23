/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <std_string.i>

%import <stl_csharp.i>

%{
    #include <pkmn/database/lists.hpp>
%}

// Convert Doxygen docs to C# docstrings
%include <pkmn_csharp_docs.i>

%import <pkmn/enums/game.hpp>

%rename(AbilityList) get_ability_list;
%rename(GameList) get_game_list;
%rename(GamecubeShadowPokemonList) get_gamecube_shadow_pokemon_list;
%rename(HMMoveList) get_hm_move_list;
%rename(ItemList) get_item_list;
%rename(LocationList) get_location_list;
%rename(MoveList) get_move_list;
%rename(NatureList) get_nature_list;
%rename(PokemonList) get_pokemon_list;
%rename(RegionList) get_region_list;
%rename(RibbonList) get_ribbon_list;
%rename(SuperTrainingMedalList) get_super_training_medal_list;
%rename(TMMoveList) get_tm_move_list;
%rename(TypeList) get_type_list;
%include <pkmn/database/lists.hpp>
