/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/game_save.hpp"
%}

%include <attribute.i>

%ignore pkmn::swig::game_save::game_save();
%ignore pkmn::swig::game_save::game_save(const pkmn::game_save::sptr&);
%ignore pkmn::swig::game_save::cptr();

// Convert getter/setter functions into attributes for more idiomatic Lua.

%attributestring(pkmn::swig::game_save, std::string, game, get_game);
%attributestring(pkmn::swig::game_save, std::string, filepath, get_filepath);
%attributestring(pkmn::swig::game_save, std::string, trainer_name, get_trainer_name, set_trainer_name);
%attribute(pkmn::swig::game_save, uint16_t, trainer_public_id, get_trainer_public_id, set_trainer_public_id);
%attribute(pkmn::swig::game_save, uint16_t, trainer_secret_id, get_trainer_secret_id, set_trainer_secret_id);
%attribute(pkmn::swig::game_save, uint32_t, trainer_id, get_trainer_id, set_trainer_id);
%attributestring(pkmn::swig::game_save, std::string, trainer_gender, get_trainer_gender, set_trainer_gender);
%attributestring(pkmn::swig::game_save, std::string, rival_name, get_rival_name, set_rival_name);
%attribute(pkmn::swig::game_save, int, money, get_money, set_money);
%attributeval(pkmn::swig::game_save, pkmn::swig::pokemon_party, pokemon_party, get_pokemon_party);
%attributeval(pkmn::swig::game_save, pkmn::swig::pokemon_pc, pokemon_pc, get_pokemon_pc);
%attributeval(pkmn::swig::game_save, pkmn::swig::item_bag, item_bag, get_item_bag);
%attributeval(pkmn::swig::game_save, pkmn::swig::item_list, item_pc, get_item_pc);

%include "cpp_wrappers/game_save.hpp"
