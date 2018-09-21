/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon.hpp"
    #include "cpp_wrappers/pokemon_helpers.hpp"
%}

%import <python/pkmn_pokemon_helpers.i>

%include <attribute.i>

%ignore pkmn::swig::pokemon::pokemon(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::pokemon::pokemon(const pkmn::swig::pokemon&);
%ignore pkmn::swig::pokemon::get_internal() const;
%ignore pkmn::swig::pokemon::cptr();

// I'd like to change these to start with __, but then subclasses won't be able
// to access them. Starting with _ should show the same effect.
%rename(_get_species_internal) pkmn::swig::pokemon::get_species;
%rename(_get_game_internal) pkmn::swig::pokemon::get_game;
%rename(_get_form_internal) pkmn::swig::pokemon::get_form;
%rename(_set_form_internal) pkmn::swig::pokemon::set_form;
%rename(_get_is_egg_internal) pkmn::swig::pokemon::get_is_egg;
%rename(_set_is_egg_internal) pkmn::swig::pokemon::set_is_egg;
%rename(_get_condition_internal) pkmn::swig::pokemon::get_condition;
%rename(_set_condition_internal) pkmn::swig::pokemon::set_condition;
%rename(_get_nickname_internal) pkmn::swig::pokemon::get_nickname;
%rename(_set_nickname_internal) pkmn::swig::pokemon::set_nickname;
%rename(_get_gender_internal) pkmn::swig::pokemon::get_gender;
%rename(_set_gender_internal) pkmn::swig::pokemon::set_gender;
%rename(_get_is_shiny_internal) pkmn::swig::pokemon::get_is_shiny;
%rename(_set_shininess_internal) pkmn::swig::pokemon::set_shininess;
%rename(_get_held_item_internal) pkmn::swig::pokemon::get_held_item;
%rename(_set_held_item_internal) pkmn::swig::pokemon::set_held_item;
%rename(_get_nature_internal) pkmn::swig::pokemon::get_nature;
%rename(_set_nature_internal) pkmn::swig::pokemon::set_nature;
%rename(_get_pokerus_duration_internal) pkmn::swig::pokemon::get_pokerus_duration;
%rename(_set_pokerus_duration_internal) pkmn::swig::pokemon::set_pokerus_duration;
%rename(_get_original_trainer_name_internal) pkmn::swig::pokemon::get_original_trainer_name;
%rename(_set_original_trainer_name_internal) pkmn::swig::pokemon::set_original_trainer_name;
%rename(_get_original_trainer_public_id_internal) pkmn::swig::pokemon::get_original_trainer_public_id;
%rename(_set_original_trainer_public_id_internal) pkmn::swig::pokemon::set_original_trainer_public_id;
%rename(_get_original_trainer_secret_id_internal) pkmn::swig::pokemon::get_original_trainer_secret_id;
%rename(_set_original_trainer_secret_id_internal) pkmn::swig::pokemon::set_original_trainer_secret_id;
%rename(_get_original_trainer_id_internal) pkmn::swig::pokemon::get_original_trainer_id;
%rename(_set_original_trainer_id_internal) pkmn::swig::pokemon::set_original_trainer_id;
%rename(_get_original_trainer_gender_internal) pkmn::swig::pokemon::get_original_trainer_gender;
%rename(_set_original_trainer_gender_internal) pkmn::swig::pokemon::set_original_trainer_gender;
%rename(_get_language_internal) pkmn::swig::pokemon::get_language;
%rename(_set_language_internal) pkmn::swig::pokemon::set_language;
%rename(_get_current_trainer_friendship_internal) pkmn::swig::pokemon::get_current_trainer_friendship;
%rename(_set_current_trainer_friendship_internal) pkmn::swig::pokemon::set_current_trainer_friendship;
%rename(_get_ability_internal) pkmn::swig::pokemon::get_ability;
%rename(_set_ability_internal) pkmn::swig::pokemon::set_ability;
%rename(_get_ball_internal) pkmn::swig::pokemon::get_ball;
%rename(_set_ball_internal) pkmn::swig::pokemon::set_ball;
%rename(_get_level_met_internal) pkmn::swig::pokemon::get_level_met;
%rename(_set_level_met_internal) pkmn::swig::pokemon::set_level_met;
%rename(_get_location_met_internal) pkmn::swig::pokemon::get_location_met;
%rename(_set_location_met_internal) pkmn::swig::pokemon::set_location_met;
%rename(_get_location_met_as_egg_internal) pkmn::swig::pokemon::get_location_met_as_egg;
%rename(_set_location_met_as_egg_internal) pkmn::swig::pokemon::set_location_met_as_egg;
%rename(_get_original_game_internal) pkmn::swig::pokemon::get_original_game;
%rename(_set_original_game_internal) pkmn::swig::pokemon::set_original_game;
%rename(_get_personality_internal) pkmn::swig::pokemon::get_personality;
%rename(_set_personality_internal) pkmn::swig::pokemon::set_personality;
%rename(_get_experience_internal) pkmn::swig::pokemon::get_experience;
%rename(_set_experience_internal) pkmn::swig::pokemon::set_experience;
%rename(_get_level_internal) pkmn::swig::pokemon::get_level;
%rename(_set_level_internal) pkmn::swig::pokemon::set_level;
%rename(_get_current_hp_internal) pkmn::swig::pokemon::get_current_hp;
%rename(_set_current_hp_internal) pkmn::swig::pokemon::set_current_hp;
%rename(_get_icon_filepath_internal) pkmn::swig::pokemon::get_icon_filepath;
%rename(_get_sprite_filepath_internal) pkmn::swig::pokemon::get_sprite_filepath;
%rename(_get_database_entry_swig_internal) pkmn::swig::pokemon::get_database_entry_swig;
%rename(_set_EV_internal) pkmn::swig::pokemon::set_EV;
%rename(_set_IV_internal) pkmn::swig::pokemon::set_IV;
%rename(_set_move_internal) pkmn::swig::pokemon::set_move;
%rename(_set_move_pp_internal) pkmn::swig::pokemon::set_move_pp;
%rename(_set_contest_stat_internal) pkmn::swig::pokemon::set_contest_stat;
%rename(_set_marking_internal) pkmn::swig::pokemon::set_marking;
%rename(_set_ribbon_internal) pkmn::swig::pokemon::set_ribbon;

%rename(_get_database_entry_internal) pkmn::swig::pokemon::get_database_entry_internal;
%rename(_set_EVs_map_value_internal) pkmn::swig::pokemon::set_EVs_map_value_internal;
%rename(_set_IVs_map_value_internal) pkmn::swig::pokemon::set_IVs_map_value_internal;
%rename(_set_stats_map_value_internal) pkmn::swig::pokemon::set_stats_map_value_internal;
%rename(_set_moves_list_move_value_internal) pkmn::swig::pokemon::set_moves_list_move_value_internal;
%rename(_set_moves_list_pp_value_internal) pkmn::swig::pokemon::set_moves_list_pp_value_internal;
%rename(_set_contest_stats_map_value_internal) pkmn::swig::pokemon::set_contest_stats_map_value_internal;
%rename(_set_markings_map_value_internal) pkmn::swig::pokemon::set_markings_map_value_internal;
%rename(_set_ribbons_map_value_internal) pkmn::swig::pokemon::set_ribbons_map_value_internal;

// Rename functions specific to SWIG class whose name were changed to remove
// ambiguity from the base class.
%rename(clone) pkmn::swig::pokemon::clone_swig;
%rename(to_game) pkmn::swig::pokemon::to_game_swig;

// Convert getter/setter functions into attributes for more idiomatic Python.

%attributeval(pkmn::swig::pokemon, pkmn::swig::EV_map, EVs, get_EVs_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::IV_map, IVs, get_IVs_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::marking_map, markings, get_markings_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::ribbon_map, ribbons, get_ribbons_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::contest_stat_map, contest_stats, get_contest_stats_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::move_slots, moves, get_moves_helper);
%attributeval(pkmn::swig::pokemon, %arg(std::map<pkmn::e_stat, int>), stats, get_stats_swig);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::numeric_attribute_map<pkmn::pokemon>), numeric_attributes, get_numeric_attributes);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::string_attribute_map<pkmn::pokemon>), string_attributes, get_string_attributes);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::boolean_attribute_map<pkmn::pokemon>), boolean_attributes, get_boolean_attributes);

%feature("director", "1") pkmn::swig::pokemon;
%include "cpp_wrappers/pokemon.hpp"
