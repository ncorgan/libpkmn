/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/pokemon_entry.hpp>
%}

%include <attribute.i>

%attributestring(pkmn::database::pokemon_entry, std::string, Name, get_name);
%attributestring(pkmn::database::pokemon_entry, std::string, Game, get_game);
%attributestring(pkmn::database::pokemon_entry, std::string, Species, get_species);
%attributestring(pkmn::database::pokemon_entry, std::string, PokedexEntry, get_pokedex_entry);
%attributestring(pkmn::database::pokemon_entry, std::string, Form, get_form, set_form);
%attribute(pkmn::database::pokemon_entry, float, Height, get_height);
%attribute(pkmn::database::pokemon_entry, float, Weight, get_weight);
%attribute(pkmn::database::pokemon_entry, float, ChanceMale, get_chance_male);
%attribute(pkmn::database::pokemon_entry, float, ChanceFemale, get_chance_female);
%attribute(pkmn::database::pokemon_entry, bool, HasGenderDifferences, has_gender_differences);
%attribute(pkmn::database::pokemon_entry, int, BaseFriendship, get_base_friendship);
%attributeval(pkmn::database::pokemon_entry, %arg(std::pair<std::string, std::string>), Types, get_types);
%attributeval(pkmn::database::pokemon_entry, %arg(std::pair<std::string, std::string>), Abilities, get_abilities);
%attributestring(pkmn::database::pokemon_entry, std::string, HiddenAbility, get_hidden_ability);
%attributeval(pkmn::database::pokemon_entry, %arg(std::pair<std::string, std::string>), EggGroups, get_egg_groups);
%attributeval(pkmn::database::pokemon_entry, %arg(std::map<std::string, int>), BaseStats, get_base_stats);
%attributeval(pkmn::database::pokemon_entry, %arg(std::map<std::string, int>), EVYields, get_EV_yields);
%attribute(pkmn::database::pokemon_entry, int, ExperienceYield, get_experience_yield);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::levelup_move>), LevelupMoves, get_levelup_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::move_entry>), TMHMMoves, get_tm_hm_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::move_entry>), EggMoves, get_egg_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::move_entry>), TutorMoves, get_tutor_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<std::string>), Forms, get_forms);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::pokemon_entry>), Evolutions, get_evolutions);

%ignore pkmn::database::pokemon_entry::get_species_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_id;
%ignore pkmn::database::pokemon_entry::get_form_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_index;
%ignore pkmn::database::pokemon_entry::get_game_id;
%include <pkmn/database/pokemon_entry.hpp>

%include <csharp/stl_macros.i>
PKMN_CSHARP_VECTOR(pkmn::database::pokemon_entry, PokemonEntry, PokemonEntryList)
