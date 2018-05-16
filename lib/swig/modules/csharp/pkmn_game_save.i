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

%typemap(csimports) pkmn::swig::game_save "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::game_save::game_save();
%ignore pkmn::swig::game_save::game_save(const pkmn::game_save::sptr&);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::game_save::cptr() "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::game_save, std::string, Game, get_game);
%attributestring(pkmn::swig::game_save, std::string, Filepath, get_filepath);
%attributestring(pkmn::swig::game_save, std::string, TrainerName, get_trainer_name, set_trainer_name);
%attribute(pkmn::swig::game_save, uint16_t, TrainerPublicID, get_trainer_public_id, set_trainer_public_id);
%attribute(pkmn::swig::game_save, uint16_t, TrainerSecretID, get_trainer_secret_id, set_trainer_secret_id);
%attribute(pkmn::swig::game_save, uint32_t, TrainerID, get_trainer_id, set_trainer_id);
%attributestring(pkmn::swig::game_save, std::string, TrainerGender, get_trainer_gender, set_trainer_gender);
%attributestring(pkmn::swig::game_save, std::string, RivalName, get_rival_name, set_rival_name);
%attribute(pkmn::swig::game_save, int, Money, get_money, set_money);
%attributeval(pkmn::swig::game_save, pkmn::swig::pokedex, Pokedex, get_pokedex);
%attributeval(pkmn::swig::game_save, pkmn::swig::pokemon_party, PokemonParty, get_pokemon_party);
%attributeval(pkmn::swig::game_save, pkmn::swig::pokemon_pc, PokemonPC, get_pokemon_pc);
%attributeval(pkmn::swig::game_save, pkmn::swig::item_bag, ItemBag, get_item_bag);
%attributeval(pkmn::swig::game_save, pkmn::swig::item_list, ItemPC, get_item_pc);
%attributeval(pkmn::swig::game_save, %arg(pkmn::swig::numeric_attribute_map<pkmn::game_save>), NumericAttributes, get_numeric_attributes);
%attributeval(pkmn::swig::game_save, %arg(pkmn::swig::string_attribute_map<pkmn::game_save>), StringAttributes, get_string_attributes);
%attributeval(pkmn::swig::game_save, %arg(pkmn::swig::boolean_attribute_map<pkmn::game_save>), BooleanAttributes, get_boolean_attributes);

%typemap(cscode) pkmn::swig::game_save
%{
    public bool Equals(GameSave rhs)
    {
        if(rhs == null)
        {
            return false;
        }
        else if(this == rhs)
        {
            return true;
        }
        else
        {
            return (this.Cptr() == rhs.Cptr());
        }
    }

    public override bool Equals(System.Object rhs)
    {
        if(rhs == null)
        {
            return false;
        }

        GameSave rhsAsGameSave = rhs as GameSave;
        if(rhsAsGameSave == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsGameSave);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/game_save.hpp"
