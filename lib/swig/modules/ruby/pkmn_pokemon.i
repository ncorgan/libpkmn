/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <pkmn/pokemon.hpp>
%}

%include <std_string.i>

%rename("species") get_species;
%rename("game") get_game;
%rename("form") get_form;
%rename("form=") set_form;
%rename("database_entry") get_database_entry;
%rename("nickname") get_nickname;
%rename("nickname=") set_nickname;
%rename("gender") get_gender;
%rename("gender=") set_gender;
%rename("is_shiny?") is_shiny;
%rename("is_shiny=") set_shininess;
// TODO: held item
%rename("trainer_name") get_trainer_name;
%rename("trainer_name=") set_trainer_name;
%rename("trainer_public_id") get_trainer_public_id;
%rename("trainer_public_id=") set_trainer_public_id;
%rename("trainer_secret_id") get_trainer_secret_id;
%rename("trainer_secret_id=") set_trainer_secret_id;
%rename("trainer_id") get_trainer_id;
%rename("trainer_id=") set_trainer_id;
%rename("trainer_gender") get_trainer_gender;
%rename("trainer_gender=") set_trainer_gender;
%rename("friendship") get_friendship;
%rename("friendship=") set_friendship;
%rename("ability") get_ability;
%rename("ability=") set_ability;
%rename("ball") get_ball;
%rename("ball=") set_ball;
%rename("level_met") get_level_met;
%rename("level_met=") set_level_met;
%rename("original_game") get_original_game;
%rename("original_game=") set_original_game;
%rename("personality") get_personality;
%rename("personality=") set_personality;
%rename("experience") get_experience;
%rename("experience=") set_experience;
%rename("level") get_level;
%rename("level=") set_level;
%rename("markings") get_markings;
%rename("ribbons") get_ribbons;
%rename("contest_stats") get_contest_stats;
%rename("moves") get_moves;
%rename("EVs") get_EVs;
%rename("IVs") get_IVs;
%rename("stats") get_stats;
%rename("icon_filepath") get_icon_filepath;
%rename("sprite_filepath") get_sprite_filepath;

%rename(PokemonInternal) pkmn::pokemon;
%include <pkmn/pokemon.hpp>

// Suppress shadowing warning when adding static variables.
%warnfilter(508) pkmn::shared_ptr<pkmn::pokemon>;

%extend pkmn::shared_ptr<pkmn::pokemon> {

    pkmn::shared_ptr<pkmn::pokemon>(
        const std::string& species,
        const std::string& game,
        const std::string& form,
        int level
    ) {
        return new pkmn::shared_ptr<pkmn::pokemon>(pkmn::pokemon::make(species, game, form, level));
    }

    pkmn::shared_ptr<pkmn::pokemon>(
        const std::string& filepath
    ) {
        return new pkmn::shared_ptr<pkmn::pokemon>(pkmn::pokemon::from_file(filepath));
    }

    bool __eq__(const pkmn::shared_ptr<pkmn::pokemon>& rhs) {
        return ((*self) == rhs);
    }

    static const uint32_t LIBPKMN_OT_ID = pkmn::pokemon::LIBPKMN_OT_ID;
    static const std::string LIBPKMN_OT_NAME = pkmn::pokemon::LIBPKMN_OT_NAME;
}

%template(Pokemon) pkmn::shared_ptr<pkmn::pokemon>;
PKMN_RUBY_VECTOR(pkmn::pokemon::sptr, PokemonList);
