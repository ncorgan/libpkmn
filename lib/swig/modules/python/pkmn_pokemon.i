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

// Rename functions specific to SWIG class whose name were changed to remove
// ambiguity from the base class.
%rename(clone) pkmn::swig::pokemon::clone_swig;
%rename(to_game) pkmn::swig::pokemon::to_game_swig;

// Since the C++ class name doesn't match the convention above, manually rename.
%rename(_set_is_shiny_internal) pkmn::swig::pokemon::set_shininess;

// Use SWIG's attribute functionality for getters not involved in directors and for getters whose
// classes don't have default constructors.

%attributeval(pkmn::swig::pokemon, pkmn::database::pokemon_entry, database_entry, get_database_entry_swig);
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

/*
 * We can't use SWIG's attributes to create these properties because we need
 * the get/set functions to support directors. I'd like the internal functions
 * to start with __, but then subclasses won't be able to access them. Starting
 * with _ should have the same effect.
 */
%define %pkmn_python_pokemon_readonly_property(prop_name)
%rename(_get_ ## prop_name ## _internal) pkmn::swig::pokemon::get_ ## prop_name;
%extend pkmn::swig::pokemon
{
%pythoncode %{
    __swig_getmethods__["prop_name"] = _get_ ## prop_name ## _internal
    if _newclass:
        prop_name = _swig_property(_get_ ## prop_name ## _internal)
%}
}
%enddef

%define %pkmn_python_pokemon_readwrite_property(prop_name)
%rename(_get_ ## prop_name ## _internal) pkmn::swig::pokemon::get_ ## prop_name;
%rename(_set_ ## prop_name ## _internal) pkmn::swig::pokemon::set_ ## prop_name;
%extend pkmn::swig::pokemon
{
%pythoncode %{
    __swig_getmethods__["prop_name"] = _get_ ## prop_name ## _internal
    __swig_setmethods__["prop_name"] = _set_ ## prop_name ## _internal
    if _newclass:
        prop_name = _swig_property(_get_ ## prop_name ## _internal, _set_ ## prop_name ## _internal)
%}
}
%enddef

%pkmn_python_pokemon_readonly_property(species)
%pkmn_python_pokemon_readonly_property(game)
%pkmn_python_pokemon_readwrite_property(form)
%pkmn_python_pokemon_readwrite_property(is_egg)
%pkmn_python_pokemon_readwrite_property(condition)
%pkmn_python_pokemon_readwrite_property(nickname)
%pkmn_python_pokemon_readwrite_property(gender)
%pkmn_python_pokemon_readwrite_property(is_shiny)
%pkmn_python_pokemon_readwrite_property(held_item)
%pkmn_python_pokemon_readwrite_property(nature)
%pkmn_python_pokemon_readwrite_property(pokerus_duration)
%pkmn_python_pokemon_readwrite_property(original_trainer_name)
%pkmn_python_pokemon_readwrite_property(original_trainer_public_id)
%pkmn_python_pokemon_readwrite_property(original_trainer_secret_id)
%pkmn_python_pokemon_readwrite_property(original_trainer_id)
%pkmn_python_pokemon_readwrite_property(original_trainer_gender)
%pkmn_python_pokemon_readwrite_property(language)
%pkmn_python_pokemon_readwrite_property(current_trainer_friendship)
%pkmn_python_pokemon_readwrite_property(ability)
%pkmn_python_pokemon_readwrite_property(ball)
%pkmn_python_pokemon_readwrite_property(level_met)
%pkmn_python_pokemon_readwrite_property(location_met)
%pkmn_python_pokemon_readwrite_property(location_met_as_egg)
%pkmn_python_pokemon_readwrite_property(original_game)
%pkmn_python_pokemon_readwrite_property(personality)
%pkmn_python_pokemon_readwrite_property(experience)
%pkmn_python_pokemon_readwrite_property(level)
%pkmn_python_pokemon_readwrite_property(current_hp)
%pkmn_python_pokemon_readonly_property(icon_filepath)
%pkmn_python_pokemon_readonly_property(sprite_filepath)

%feature("director", "1") pkmn::swig::pokemon;
%include "cpp_wrappers/pokemon.hpp"
