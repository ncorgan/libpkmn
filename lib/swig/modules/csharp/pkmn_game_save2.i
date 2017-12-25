/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/game_save.hpp"
%}

%typemap(csimports) pkmn::swig::game_save2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::game_save2::game_save2();
%ignore pkmn::swig::game_save2::game_save2(const pkmn::game_save::sptr&);
%ignore pkmn::swig::game_save2::as_vector;

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::game_save2::get_game() "private";
%csmethodmodifiers pkmn::swig::game_save2::get_filepath() "private";
%csmethodmodifiers pkmn::swig::game_save2::get_trainer_name() "private";
%csmethodmodifiers pkmn::swig::game_save2::set_trainer_name(const std::string&) "private";
%csmethodmodifiers pkmn::swig::game_save2::get_trainer_public_id() "private";
%csmethodmodifiers pkmn::swig::game_save2::set_trainer_public_id(uint16_t) "private";
%csmethodmodifiers pkmn::swig::game_save2::get_trainer_secret_id() "private";
%csmethodmodifiers pkmn::swig::game_save2::set_trainer_secret_id(uint16_t) "private";
%csmethodmodifiers pkmn::swig::game_save2::get_trainer_id() "private";
%csmethodmodifiers pkmn::swig::game_save2::set_trainer_id(uint32_t) "private";
%csmethodmodifiers pkmn::swig::game_save2::get_trainer_gender() "private";
%csmethodmodifiers pkmn::swig::game_save2::set_trainer_gender(const std::string&) "private";
%csmethodmodifiers pkmn::swig::game_save2::get_rival_name() "private";
%csmethodmodifiers pkmn::swig::game_save2::set_rival_name(const std::string&) "private";
%csmethodmodifiers pkmn::swig::game_save2::get_money() "private";
%csmethodmodifiers pkmn::swig::game_save2::set_money(int) "private";
%csmethodmodifiers pkmn::swig::game_save2::get_pokemon_party() "private";
%csmethodmodifiers pkmn::swig::game_save2::get_pokemon_pc() "private";
%csmethodmodifiers pkmn::swig::game_save2::get_item_bag() "private";
%csmethodmodifiers pkmn::swig::game_save2::get_item_pc() "private";

%csmethodmodifiers pkmn::swig::game_save2::cptr() "private";

%typemap(cscode) pkmn::swig::game_save2
%{
    public static string DetectType(string filepath) {
        string ret = PKMNPINVOKE.detect_game_save_type(filepath);
        if(PKMNPINVOKE.SWIGPendingException.Pending) {
            throw PKMNPINVOKE.SWIGPendingException.Retrieve();
        }
        return ret;
    }

    public string Game
    {
        get { return GetGame(); }
    }

    public string Filepath
    {
        get { return GetFilepath(); }
    }

    public string TrainerName
    {
        get { return GetTrainerName(); }
        set { SetTrainerName(value); }
    }

    public ushort TrainerPublicID
    {
        get { return GetTrainerPublicID(); }
        set { SetTrainerPublicID(value); }
    }

    public ushort TrainerSecretID
    {
        get { return GetTrainerSecretID(); }
        set { SetTrainerSecretID(value); }
    }

    public uint TrainerID
    {
        get { return GetTrainerID(); }
        set { SetTrainerID(value); }
    }

    public string TrainerGender
    {
        get { return GetTrainerGender(); }
        set { SetTrainerGender(value); }
    }

    public string RivalName
    {
        get { return GetRivalName(); }
        set { SetRivalName(value); }
    }

    public int Money
    {
        get { return GetMoney(); }
        set { SetMoney(value); }
    }

    public PokemonParty2 PokemonParty
    {
        get { return GetPokemonParty(); }
    }

    public PokemonPC2 PokemonPC
    {
        get { return GetPokemonPC(); }
    }

    public ItemBag2 ItemBag
    {
        get { return GetItemBag(); }
    }

    public ItemList2 ItemPC
    {
        get { return GetItemPC(); }
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

        GameSave2 rhsAsGameSave2 = rhs as GameSave2;
        if(rhsAsGameSave2 == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsGameSave2);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/game_save.hpp"
