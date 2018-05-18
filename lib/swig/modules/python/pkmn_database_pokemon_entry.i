/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/pokemon_entry.hpp>
%}

%include <attribute.i>

%attributestring(pkmn::database::pokemon_entry, std::string, name, get_name);
%attributestring(pkmn::database::pokemon_entry, std::string, game, get_game);
%attributestring(pkmn::database::pokemon_entry, std::string, species, get_species);
%attributestring(pkmn::database::pokemon_entry, std::string, pokedex_entry, get_pokedex_entry);
%attributestring(pkmn::database::pokemon_entry, std::string, form, get_form, set_form);
%attribute(pkmn::database::pokemon_entry, float, height, get_height);
%attribute(pkmn::database::pokemon_entry, float, weight, get_weight);
%attribute(pkmn::database::pokemon_entry, float, chance_male, get_chance_male);
%attribute(pkmn::database::pokemon_entry, float, chance_female, get_chance_female);
%attribute(pkmn::database::pokemon_entry, bool, has_gender_differences, has_gender_differences);
%attribute(pkmn::database::pokemon_entry, int, base_friendship, get_base_friendship);
%attributeval(pkmn::database::pokemon_entry, %arg(std::pair<std::string, std::string>), types, get_types);
%attributeval(pkmn::database::pokemon_entry, %arg(std::pair<std::string, std::string>), abilities, get_abilities);
%attributestring(pkmn::database::pokemon_entry, std::string, hidden_ability, get_hidden_ability);
%attributeval(pkmn::database::pokemon_entry, %arg(std::pair<std::string, std::string>), egg_groups, get_egg_groups);
%attributeval(pkmn::database::pokemon_entry, %arg(std::map<std::string, int>), base_stats, get_base_stats);
%attributeval(pkmn::database::pokemon_entry, %arg(std::map<std::string, int>), EV_yields, get_EV_yields);
%attribute(pkmn::database::pokemon_entry, int, experience_yield, get_experience_yield);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::levelup_move>), levelup_moves, get_levelup_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::move_entry>), tm_hm_moves, get_tm_hm_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::move_entry>), egg_moves, get_egg_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::move_entry>), tutor_moves, get_tutor_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<std::string>), forms, get_forms);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::pokemon_entry>), evolutions, get_evolutions);

%ignore pkmn::database::pokemon_entry::get_species_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_id;
%ignore pkmn::database::pokemon_entry::get_form_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_index;
%ignore pkmn::database::pokemon_entry::get_game_id;

%include <pkmn/database/pokemon_entry.hpp>

%include <python/stl_macros.i>
PKMN_PYTHON_VECTOR(pkmn::database::pokemon_entry, pokemon_entry_list)
