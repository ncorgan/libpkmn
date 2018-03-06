/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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

// Convert getter/setter functions into attributes for more idiomatic Python.

%attributestring(pkmn::swig::game_save, std::string, game, get_game);
%attributestring(pkmn::swig::game_save, std::string, filepath, get_filepath);
%attribute(pkmn::swig::game_save, uint16_t, trainer_public_id, get_trainer_public_id, set_trainer_public_id);
%attribute(pkmn::swig::game_save, uint16_t, trainer_secret_id, get_trainer_secret_id, set_trainer_secret_id);
%attribute(pkmn::swig::game_save, uint32_t, trainer_id, get_trainer_id, set_trainer_id);
%attributestring(pkmn::swig::game_save, std::string, trainer_gender, get_trainer_gender, set_trainer_gender);
%attribute(pkmn::swig::game_save, int, money, get_money, set_money);
%attributeval(pkmn::swig::game_save, pkmn::swig::pokedex, pokedex, get_pokedex);
%attributeval(pkmn::swig::game_save, pkmn::swig::pokemon_party, pokemon_party, get_pokemon_party);
%attributeval(pkmn::swig::game_save, pkmn::swig::pokemon_pc, pokemon_pc, get_pokemon_pc);
%attributeval(pkmn::swig::game_save, pkmn::swig::item_bag, item_bag, get_item_bag);
%attributeval(pkmn::swig::game_save, pkmn::swig::item_list, item_pc, get_item_pc);
%attributeval(pkmn::swig::game_save, %arg(pkmn::swig::numeric_attribute_map<pkmn::game_save>), numeric_attributes, get_numeric_attributes);
%attributeval(pkmn::swig::game_save, %arg(pkmn::swig::string_attribute_map<pkmn::game_save>), string_attributes, get_string_attributes);
%attributeval(pkmn::swig::game_save, %arg(pkmn::swig::boolean_attribute_map<pkmn::game_save>), boolean_attributes, get_boolean_attributes);

/*
 * SWIG 3.0.8 introduced the SWIG_PYTHON_2_UNICODE macro, which allows the
 * Python 2 "unicode" type to be converted to a char* or std::string. There's
 * no way for a SWIG project to bring this in, so we need this ugly workaround
 * when using earlier verisons of SWIG.
 */
#if defined(SWIGPYTHON) && SWIG_VERSION < 0x030008
%attributestring(pkmn::swig::game_save, std::wstring, trainer_name, get_trainer_name, set_trainer_name);
%attributestring(pkmn::swig::game_save, std::wstring, rival_name, get_rival_name, set_rival_name);
#else
%attributestring(pkmn::swig::game_save, std::string, trainer_name, get_trainer_name, set_trainer_name);
%attributestring(pkmn::swig::game_save, std::string, rival_name, get_rival_name, set_rival_name);
#endif

%include "cpp_wrappers/game_save.hpp"
