/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/game_save.hpp>
%}

%rename(game_save_base) pkmn::game_save;

%csmethodmodifiers pkmn::game_save::get_filepath() "private";
%csmethodmodifiers pkmn::game_save::get_game() "private";
%csmethodmodifiers pkmn::game_save::get_trainer_name() "private";
%csmethodmodifiers pkmn::game_save::set_trainer_name(const std::string&) "private";
%csmethodmodifiers pkmn::game_save::get_trainer_id() "private";
%csmethodmodifiers pkmn::game_save::set_trainer_id(uint32_t) "private";
%csmethodmodifiers pkmn::game_save::get_trainer_public_id() "private";
%csmethodmodifiers pkmn::game_save::set_trainer_public_id(uint16_t) "private";
%csmethodmodifiers pkmn::game_save::get_trainer_secret_id() "private";
%csmethodmodifiers pkmn::game_save::set_trainer_secret_id(uint16_t) "private";
%csmethodmodifiers pkmn::game_save::get_trainer_gender() "private";
%csmethodmodifiers pkmn::game_save::set_trainer_gender(const std::string&) "private";
%csmethodmodifiers pkmn::game_save::get_rival_name() "private";
%csmethodmodifiers pkmn::game_save::set_rival_name(const std::string&) "private";
%csmethodmodifiers pkmn::game_save::get_money() "private";
%csmethodmodifiers pkmn::game_save::set_money(int) "private";
%csmethodmodifiers pkmn::game_save::get_pokedex() "private";
%csmethodmodifiers pkmn::game_save::get_pokemon_party() "private";
%csmethodmodifiers pkmn::game_save::get_pokemon_pc() "private";
%csmethodmodifiers pkmn::game_save::get_item_bag() "private";
%csmethodmodifiers pkmn::game_save::get_item_pc() "private";
%csmethodmodifiers std::shared_ptr<pkmn::game_save>::__cptr "private";
%csmethodmodifiers std::shared_ptr<pkmn::game_save>::__sptr_eq "private";

%typemap(cscode) std::shared_ptr<pkmn::game_save> %{
    public string Filepath {
        get {
            return this.GetFilepath();
        }
    }

    public string Game {
        get {
            return this.GetGame();
        }
    }

    public string TrainerName {
        get {
            return this.GetTrainerName();
        }
        set {
            this.SetTrainerName(value);
        }
    }

    public uint TrainerID {
        get {
            return this.GetTrainerID();
        }
        set {
            this.SetTrainerID(value);
        }
    }

    public ushort TrainerPublicID {
        get {
            return this.GetTrainerPublicID();
        }
        set {
            this.SetTrainerPublicID(value);
        }
    }

    public ushort TrainerSecretID {
        get {
            return this.GetTrainerSecretID();
        }
        set {
            this.SetTrainerSecretID(value);
        }
    }

    /// <summary> Returns the type of save at the given filepath.
    public static string DetectType(string filepath) {
        string ret = PKMNPINVOKE.detect_game_save_type(filepath);
        if(PKMNPINVOKE.SWIGPendingException.Pending) {
            throw PKMNPINVOKE.SWIGPendingException.Retrieve();
        }
        return ret;
    }

    public string TrainerGender {
        get {
            return this.GetTrainerGender();
        }
        set {
            this.SetTrainerGender(value);
        }
    }

    public string RivalName {
        get {
            return this.GetRivalName();
        }
        set {
            this.SetRivalName(value);
        }
    }

    public int Money {
        get {
            return this.GetMoney();
        }
        set {
            this.SetMoney(value);
        }
    }

    public Pokedex Pokedex {
        get {
            return this.GetPokedex();
        }
    }

    public PokemonParty PokemonParty {
        get {
            return this.GetPokemonParty();
        }
    }

    public PokemonPC PokemonPC {
        get {
            return this.GetPokemonPC();
        }
    }

    public ItemBag ItemBag {
        get {
            return this.GetItemBag();
        }
    }

    public ItemList ItemPC {
        get {
            return this.GetItemPC();
        }
    }

    /// <summary>Compares two GameSave instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">GameSave with which to compare self</param>
    /// <returns>Whether or not GameSave instances are equal</returns>
    /// </remarks>
    public bool Equals(GameSave rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of GameSave to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not GameSave and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        GameSave rhsSptr = rhs as GameSave;
        if(rhsSptr == null) {
            return false;
        } else {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given GameSave.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<ulong>(__cptr())
                              .ToHashCode();
    }
%}

%ignore from_file;
%ignore detect_type;
%include <pkmn/game_save.hpp>
%template(GameSave) std::shared_ptr<pkmn::game_save>;
