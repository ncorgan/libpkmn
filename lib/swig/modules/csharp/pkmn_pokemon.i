/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon.hpp>
%}

%typemap(csimports) pkmn::pokemon "
using System;
using System.Runtime.InteropServices;
using Database;"
%typemap(csimports) pkmn::shared_ptr<pkmn::pokemon> "
using System;
using System.Runtime.InteropServices;
using Database;"

%rename(IsShinyFunction) pkmn::pokemon::is_shiny;
%rename(pokemon_base) pkmn::pokemon;

%csmethodmodifiers pkmn::pokemon::get_species "private";
%csmethodmodifiers pkmn::pokemon::get_game "private";
%csmethodmodifiers pkmn::pokemon::get_form "private";
%csmethodmodifiers pkmn::pokemon::set_form(const std::string&) "private";
%csmethodmodifiers pkmn::pokemon::get_database_entry "private";
%csmethodmodifiers pkmn::pokemon::get_nickname "private";
%csmethodmodifiers pkmn::pokemon::set_nickname "private";
%csmethodmodifiers pkmn::pokemon::get_gender "private";
%csmethodmodifiers pkmn::pokemon::set_gender(const std::string&) "private";
%csmethodmodifiers pkmn::pokemon::is_shiny "private";
%csmethodmodifiers pkmn::pokemon::set_shininess "private";
%csmethodmodifiers pkmn::pokemon::get_trainer_name "private";
%csmethodmodifiers pkmn::pokemon::set_trainer_name "private";
%csmethodmodifiers pkmn::pokemon::get_trainer_public_id "private";
%csmethodmodifiers pkmn::pokemon::get_trainer_secret_id "private";
%csmethodmodifiers pkmn::pokemon::get_trainer_id "private";
%csmethodmodifiers pkmn::pokemon::set_trainer_public_id(uint16_t) "private";
%csmethodmodifiers pkmn::pokemon::set_trainer_secret_id(uint16_t) "private";
%csmethodmodifiers pkmn::pokemon::set_trainer_id(uint32_t) "private";
%csmethodmodifiers pkmn::pokemon::get_trainer_gender "private";
%csmethodmodifiers pkmn::pokemon::set_trainer_gender(const std::string&) "private";
%csmethodmodifiers pkmn::pokemon::get_friendship "private";
%csmethodmodifiers pkmn::pokemon::set_friendship(int) "private";
%csmethodmodifiers pkmn::pokemon::get_nature "private";
%csmethodmodifiers pkmn::pokemon::set_nature(const std::string&) "private";
%csmethodmodifiers pkmn::pokemon::get_ability "private";
%csmethodmodifiers pkmn::pokemon::set_ability(const std::string&) "private";
%csmethodmodifiers pkmn::pokemon::get_ball "private";
%csmethodmodifiers pkmn::pokemon::set_ball(const std::string&) "private";
%csmethodmodifiers pkmn::pokemon::get_level_met "private";
%csmethodmodifiers pkmn::pokemon::set_level_met(int) "private";
%csmethodmodifiers pkmn::pokemon::get_original_game "private";
%csmethodmodifiers pkmn::pokemon::set_original_game(const std::string&) "private";
%csmethodmodifiers pkmn::pokemon::get_personality "private";
%csmethodmodifiers pkmn::pokemon::set_personality(uint32_t) "private";
%csmethodmodifiers pkmn::pokemon::get_experience "private";
%csmethodmodifiers pkmn::pokemon::set_experience(int) "private";
%csmethodmodifiers pkmn::pokemon::get_level "private";
%csmethodmodifiers pkmn::pokemon::set_level(int) "private";
%csmethodmodifiers pkmn::pokemon::get_markings "private";
%csmethodmodifiers pkmn::pokemon::get_ribbons "private";
%csmethodmodifiers pkmn::pokemon::get_contest_stats "private";
%csmethodmodifiers pkmn::pokemon::get_moves "private";
%csmethodmodifiers pkmn::pokemon::get_EVs "private";
%csmethodmodifiers pkmn::pokemon::get_IVs "private";
%csmethodmodifiers pkmn::pokemon::get_stats "private";
%csmethodmodifiers pkmn::pokemon::get_icon_filepath "private";
%csmethodmodifiers pkmn::pokemon::get_sprite_filepath "private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::pokemon>::__cptr "private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::pokemon>::__sptr_eq "private";

%typemap(cscode) pkmn::shared_ptr<pkmn::pokemon> %{
    public string Species {
        get {
            return GetSpecies();
        }
    }

    public string Game {
        get {
            return GetGame();
        }
    }

    public string Form {
        get {
            return GetForm();
        }
        set {
            SetForm(value);
        }
    }

    public PokemonEntry DatabaseEntry {
        get {
            return GetDatabaseEntry();
        }
    }

    public string Nickname {
        get {
            return GetNickname();
        }
        set {
            SetNickname(value);
        }
    }

    public string Gender {
        get {
            return GetGender();
        }
        set {
            SetGender(value);
        }
    }

    public bool IsShiny {
        get {
            return IsShinyFunction();
        }
        set {
            SetShininess(value);
        }
    }

    // How to do held item?

    public string TrainerName {
        get {
            return GetTrainerName();
        }
        set {
            SetTrainerName(value);
        }
    }

    public uint TrainerID {
        get {
            return GetTrainerID();
        }
        set {
            SetTrainerID(value);
        }
    }

    public ushort TrainerPublicID {
        get {
            return GetTrainerPublicID();
        }
        set {
            SetTrainerPublicID(value);
        }
    }

    public ushort TrainerSecretID {
        get {
            return GetTrainerSecretID();
        }
        set {
            SetTrainerSecretID(value);
        }
    }

    public string TrainerGender {
        get {
            return GetTrainerGender();
        }
        set {
            SetTrainerGender(value);
        }
    }

    public int Friendship {
        get {
            return GetFriendship();
        }
        set {
            SetFriendship(value);
        }
    }

    public string Nature {
        get {
            return GetNature();
        }
        set {
            SetNature(value);
        }
    }

    public string Ability {
        get {
            return GetAbility();
        }
        set {
            SetAbility(value);
        }
    }

    public string Ball {
        get {
            return GetBall();
        }
        set {
            SetBall(value);
        }
    }

    public int LevelMet {
        get {
            return GetLevelMet();
        }
        set {
            SetLevelMet(value);
        }
    }

    public string OriginalGame {
        get {
            return GetOriginalGame();
        }
        set {
            SetOriginalGame(value);
        }
    }

    public uint Personality {
        get {
            return GetPersonality();
        }
        set {
            SetPersonality(value);
        }
    }

    public int Experience {
        get {
            return GetExperience();
        }
        set {
            SetExperience(value);
        }
    }

    public int Level {
        get {
            return GetLevel();
        }
        set {
            SetLevel(value);
        }
    }

    public StringBoolDict Markings {
        get {
            return GetMarkings();
        }
    }

    public StringBoolDict Ribbons {
        get {
            return GetRibbons();
        }
    }

    public StringIntDict ContestStats {
        get {
            return GetContestStats();
        }
    }

    public MoveSlotList Moves {
        get {
            return GetMoves();
        }
    }

    public StringIntDict EVs {
        get {
            return GetEVs();
        }
    }

    public StringIntDict IVs {
        get {
            return GetIVs();
        }
    }

    public StringIntDict Stats {
        get {
            return GetStats();
        }
    }

    public string IconFilepath {
        get {
            return GetIconFilepath();
        }
    }

    public string SpriteFilepath {
        get {
            return GetSpriteFilepath();
        }
    }

    /// <summary>Compares two Pokemon instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">Pokemon with which to compare self</param>
    /// <returns>Whether or not Pokemon instances are equal</returns>
    /// </remarks>
    public bool Equals(Pokemon rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of Pokemon to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not Pokemon and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        Pokemon rhsSptr = rhs as Pokemon;
        if(rhsSptr == null) {
            return false;
        } else {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given Pokemon.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<ulong>(__cptr())
                              .ToHashCode();
    }

    public static readonly uint LIBPKMN_OT_ID = 2105214279;
    public static readonly string LIBPKMN_OT_NAME = "LibPKMN";
%}

%ignore LIBPKMN_OT_ID;
%ignore LIBPKMN_OT_NAME;
%ignore from_file;
%ignore get_native_pc_data;
%ignore get_native_party_data;
%include <pkmn/pokemon.hpp>
%template(Pokemon) pkmn::shared_ptr<pkmn::pokemon>;

%import <csharp/stl_macros.i>
PKMN_CSHARP_VECTOR(pkmn::pokemon::sptr, Pokemon, PokemonList)
