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

%typemap(csimports) pkmn::swig::game_save2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::game_save2::game_save2();
%ignore pkmn::swig::game_save2::game_save2(const pkmn::game_save::sptr&);
%ignore pkmn::swig::game_save2::as_vector;

// Needed for equality check.
%csmethodmodifiers pkmn::swig::game_save2::cptr() "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::game_save2, std::string, Game, get_game);
%attributestring(pkmn::swig::game_save2, std::string, Filepath, get_filepath);
%attributestring(pkmn::swig::game_save2, std::string, TrainerName, get_trainer_name, set_trainer_name);
%attribute(pkmn::swig::game_save2, uint16_t, TrainerPublicID, get_trainer_public_id, set_trainer_public_id);
%attribute(pkmn::swig::game_save2, uint16_t, TrainerSecretID, get_trainer_secret_id, set_trainer_secret_id);
%attribute(pkmn::swig::game_save2, uint32_t, TrainerID, get_trainer_id, set_trainer_id);
%attributestring(pkmn::swig::game_save2, std::string, TrainerGender, get_trainer_gender, set_trainer_gender);
%attributestring(pkmn::swig::game_save2, std::string, RivalName, get_rival_name, set_rival_name);
%attribute(pkmn::swig::game_save2, int, Money, get_money, set_money);
%attributeval(pkmn::swig::game_save2, pkmn::swig::pokemon_party2, PokemonParty, get_pokemon_party);
%attributeval(pkmn::swig::game_save2, pkmn::swig::pokemon_pc2, PokemonPC, get_pokemon_pc);
%attributeval(pkmn::swig::game_save2, pkmn::swig::item_bag2, ItemBag, get_item_bag);
%attributeval(pkmn::swig::game_save2, pkmn::swig::item_list2, ItemPC, get_item_pc);

%typemap(cscode) pkmn::swig::game_save2
%{
    public static string DetectType(string filepath) {
        string ret = PKMNPINVOKE.detect_game_save_type(filepath);
        if(PKMNPINVOKE.SWIGPendingException.Pending) {
            throw PKMNPINVOKE.SWIGPendingException.Retrieve();
        }
        return ret;
    }

    public bool Equals(GameSave2 rhs)
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

        GameSave2 rhsAsGameSave = rhs as GameSave2;
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
