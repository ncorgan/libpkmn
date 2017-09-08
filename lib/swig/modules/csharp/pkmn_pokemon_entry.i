/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/pokemon_entry.hpp>
%}

%csmethodmodifiers pkmn::database::pokemon_entry::get_name() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_game() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_species() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_pokedex_entry() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_form() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_height() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_weight() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_chance_male() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_chance_female() const "private";
%rename(HasGenderDifferencesFunction) pkmn::database::pokemon_entry::has_gender_differences;
%csmethodmodifiers pkmn::database::pokemon_entry::has_gender_differences() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_base_happiness() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_types() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_abilities() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_hidden_ability() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_egg_groups() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_base_stats() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_EV_yields() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_experience_yield() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_abilities() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_levelup_moves() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_tm_hm_moves() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_egg_moves() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_tutor_moves() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_forms() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::get_evolutions() const "private";
%csmethodmodifiers pkmn::database::pokemon_entry::set_form(const std::string&) "private";

%ignore pkmn::database::pokemon_entry::get_species_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_id;
%ignore pkmn::database::pokemon_entry::get_form_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_index;
%ignore pkmn::database::pokemon_entry::get_game_id;
%include <pkmn/database/pokemon_entry.hpp>

%include <csharp/stl_macros.i>
PKMN_CSHARP_VECTOR(pkmn::database::pokemon_entry, PokemonEntry, PokemonEntryList)
