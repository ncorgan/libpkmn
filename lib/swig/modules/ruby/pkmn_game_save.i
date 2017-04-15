/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <pkmn/game_save.hpp>
%}

%include <std_string.i>

%rename("filepath") get_filepath;
%rename("game") get_game;
%rename("trainer_name") get_trainer_name;
%rename("trainer_name=") set_trainer_name;
%rename("trainer_id") get_trainer_id;
%rename("trainer_id=") set_trainer_id;
%rename("trainer_public_id") get_trainer_public_id;
%rename("trainer_public_id=") set_trainer_public_id;
%rename("trainer_secret_id") get_trainer_secret_id;
%rename("trainer_secret_id=") set_trainer_secret_id;
%rename("trainer_gender") get_trainer_gender;
%rename("trainer_gender=") set_trainer_gender;
%rename("rival_name") get_rival_name;
%rename("rival_name=") set_rival_name;
%rename("money") get_money;
%rename("money=") set_money;
%rename("pokemon_party") get_pokemon_party;
%rename("pokemon_pc") get_pokemon_pc;
%rename("item_bag") get_item_bag;
%rename("item_pc") get_item_pc;

%rename(GameSaveInternal) pkmn::game_save;
%include <pkmn/game_save.hpp>

// Suppress shadowing warning when adding static methods.
%warnfilter(508) pkmn::shared_ptr<pkmn::game_save>;

%extend pkmn::shared_ptr<pkmn::game_save> {

    pkmn::shared_ptr<pkmn::game_save>(
        const std::string& filepath
    ) {
        return new pkmn::shared_ptr<pkmn::game_save>(pkmn::game_save::from_file(filepath));
    }

    static std::string detect_type(const std::string& filepath) {
        return pkmn::game_save::detect_type(filepath);
    }

    bool __eq__(const pkmn::shared_ptr<pkmn::game_save>& rhs) {
        return ((*self) == rhs);
    }
}

%template(GameSave) pkmn::shared_ptr<pkmn::game_save>;
