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

%ignore pkmn::swig::pokemon::pokemon();
%ignore pkmn::swig::pokemon::pokemon(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::pokemon::pokemon(const pkmn::swig::pokemon&);
%ignore pkmn::swig::pokemon::get_internal() const;
%ignore pkmn::swig::pokemon::cptr();

// Convert getter/setter functions into attributes for more idiomatic Python.

%attributestring(pkmn::swig::pokemon, std::string, species, get_species);
%attributestring(pkmn::swig::pokemon, std::string, game, get_game);
%attributestring(pkmn::swig::pokemon, std::string, form, get_form, set_form);
%attribute(pkmn::swig::pokemon, bool, is_egg, is_egg, set_is_egg);
%attributeval(pkmn::swig::pokemon, pkmn::database::pokemon_entry, database_entry, get_database_entry);
%attributestring(pkmn::swig::pokemon, std::string, condition, get_condition, set_condition);
%attributestring(pkmn::swig::pokemon, std::string, nickname, get_nickname, set_nickname);
%attribute(pkmn::swig::pokemon, pkmn::e_gender, gender, get_gender, set_gender);
%attribute(pkmn::swig::pokemon, bool, is_shiny, is_shiny, set_shininess);
%attributestring(pkmn::swig::pokemon, std::string, held_item, get_held_item, set_held_item);
%attribute(pkmn::swig::pokemon, int, pokerus_duration, get_pokerus_duration, set_pokerus_duration);
%attributestring(pkmn::swig::pokemon, std::string, original_trainer_name, get_original_trainer_name, set_original_trainer_name);
%attribute(pkmn::swig::pokemon, uint16_t, original_trainer_public_id, get_original_trainer_public_id, set_original_trainer_public_id);
%attribute(pkmn::swig::pokemon, uint16_t, original_trainer_secret_id, get_original_trainer_secret_id, set_original_trainer_secret_id);
%attribute(pkmn::swig::pokemon, uint32_t, original_trainer_id, get_original_trainer_id, set_original_trainer_id);
%attribute(pkmn::swig::pokemon, pkmn::e_gender, original_trainer_gender, get_original_trainer_gender, set_original_trainer_gender);
%attribute(pkmn::swig::pokemon, int, current_trainer_friendship, get_current_trainer_friendship, set_current_trainer_friendship);
%attributestring(pkmn::swig::pokemon, std::string, ability, get_ability, set_ability);
%attributestring(pkmn::swig::pokemon, std::string, ball, get_ball, set_ball);
%attribute(pkmn::swig::pokemon, int, level_met, get_level_met, set_level_met);
%attributestring(pkmn::swig::pokemon, std::string, location_met, get_location_met, set_location_met);
%attributestring(pkmn::swig::pokemon, std::string, location_met_as_egg, get_location_met_as_egg, set_location_met_as_egg);
%attributestring(pkmn::swig::pokemon, std::string, original_game, get_original_game, set_original_game);
%attribute(pkmn::swig::pokemon, uint32_t, personality, get_personality, set_personality);
%attribute(pkmn::swig::pokemon, int, experience, get_experience, set_experience);
%attribute(pkmn::swig::pokemon, int, level, get_level, set_level);
%attribute(pkmn::swig::pokemon, int, current_hp, get_current_hp, set_current_hp);
%attributeval(pkmn::swig::pokemon, pkmn::swig::EV_map, EVs, get_EVs);
%attributeval(pkmn::swig::pokemon, pkmn::swig::IV_map, IVs, get_IVs);
%attributeval(pkmn::swig::pokemon, pkmn::swig::marking_map, markings, get_markings);
%attributeval(pkmn::swig::pokemon, pkmn::swig::ribbon_map, ribbons, get_ribbons);
%attributeval(pkmn::swig::pokemon, pkmn::swig::contest_stat_map, contest_stats, get_contest_stats);
%attributeval(pkmn::swig::pokemon, pkmn::swig::move_slots, moves, get_moves);
%attributeval(pkmn::swig::pokemon, %arg(std::map<pkmn::e_stat, int>), stats, get_stats);
%attributestring(pkmn::swig::pokemon, std::string, icon_filepath, get_icon_filepath);
%attributestring(pkmn::swig::pokemon, std::string, sprite_filepath, get_sprite_filepath);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::numeric_attribute_map<pkmn::pokemon>), numeric_attributes, get_numeric_attributes);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::string_attribute_map<pkmn::pokemon>), string_attributes, get_string_attributes);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::boolean_attribute_map<pkmn::pokemon>), boolean_attributes, get_boolean_attributes);

%include "cpp_wrappers/pokemon.hpp"
