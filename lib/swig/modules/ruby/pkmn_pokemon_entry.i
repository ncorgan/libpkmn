/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <pkmn/database/pokemon_entry.hpp>
%}

%rename("name") pkmn::database::pokemon_entry::get_name() const;
%rename("game") pkmn::database::pokemon_entry::get_game() const;
%rename("species") pkmn::database::pokemon_entry::get_species() const;
%rename("pokedex_entry") pkmn::database::pokemon_entry::get_pokedex_entry() const;
%rename("height") pkmn::database::pokemon_entry::get_height() const;
%rename("weight") pkmn::database::pokemon_entry::get_weight() const;
%rename("chance_male") pkmn::database::pokemon_entry::get_chance_male() const;
%rename("chance_female") pkmn::database::pokemon_entry::get_chance_female() const;
%rename("has_gender_differences?") pkmn::database::pokemon_entry::has_gender_differences() const;
%rename("base_friendship") pkmn::database::pokemon_entry::get_base_friendship() const;
%rename("types") pkmn::database::pokemon_entry::get_types() const;
%rename("abilities") pkmn::database::pokemon_entry::get_abilities() const;
%rename("hidden_ability") pkmn::database::pokemon_entry::get_hidden_ability() const;
%rename("egg_groups") pkmn::database::pokemon_entry::get_egg_groups() const;
%rename("base_stats") pkmn::database::pokemon_entry::get_base_stats() const;
%rename("EV_yields") pkmn::database::pokemon_entry::get_EV_yields() const;
%rename("experience_yield") pkmn::database::pokemon_entry::get_experience_yield() const;
%rename("levelup_moves") pkmn::database::pokemon_entry::get_levelup_moves() const;
%rename("tm_hm_moves") pkmn::database::pokemon_entry::get_tm_hm_moves() const;
%rename("egg_moves") pkmn::database::pokemon_entry::get_egg_moves() const;
%rename("tutor_moves") pkmn::database::pokemon_entry::get_tutor_moves() const;
%rename("forms") pkmn::database::pokemon_entry::get_forms() const;
%rename("evolutions") pkmn::database::pokemon_entry::get_evolutions() const;
%rename("icon_filepath") pkmn::database::pokemon_entry::get_icon_filepath() const;
%rename("sprite_filepath") pkmn::database::pokemon_entry::get_sprite_filepath() const;

%rename("form") pkmn::database::pokemon_entry::get_form() const;
%rename("form=") pkmn::database::pokemon_entry::set_form(const std::string&);

%ignore pkmn::database::pokemon_entry::get_species_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_id;
%ignore pkmn::database::pokemon_entry::get_form_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_index;
%ignore pkmn::database::pokemon_entry::get_game_id;
%include <pkmn/database/pokemon_entry.hpp>

PKMN_RUBY_VECTOR(pkmn::database::pokemon_entry, PokemonEntryList);
