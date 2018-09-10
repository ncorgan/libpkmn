/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module(directors="0") Lists

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <std_string.i>

%import <stl_csharp.i>

%{
    #include <pkmn/database/lists.hpp>
%}

// Convert Doxygen docs to C# docstrings
%include <pkmn_csharp_docs.i>

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

%rename(AbilityNameList) get_ability_name_list;
%rename(GameNameList) get_game_name_list;
%rename(GamecubeShadowPokemonNameList) get_gamecube_shadow_pokemon_name_list;
%rename(HMMoveNameList) get_hm_move_name_list;
%rename(ItemNameList) get_item_name_list;
%rename(LocationNameList) get_location_name_list;
%rename(MoveNameList) get_move_name_list;
%rename(NatureNameList) get_nature_name_list;
%rename(PokemonNameList) get_pokemon_name_list;
%rename(RegionNameList) get_region_name_list;
%rename(RibbonNameList) get_ribbon_name_list;
%rename(SuperTrainingMedalNameList) get_super_training_medal_name_list;
%rename(TMMoveNameList) get_tm_move_name_list;
%rename(TypeNameList) get_type_name_list;

%include <pkmn/database/lists.hpp>
