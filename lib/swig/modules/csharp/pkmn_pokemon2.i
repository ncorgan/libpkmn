/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon.hpp"
%}

%typemap(csimports) pkmn::swig::pokemon2 "
using System;
using System.Runtime.InteropServices;
using Database;"

%ignore pkmn::swig::pokemon2::pokemon2();
%ignore pkmn::swig::pokemon2::pokemon2(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::pokemon2::pokemon2(const pkmn::swig::pokemon2&);
%ignore pkmn::swig::pokemon2::get_internal() const;

// Make C++ methods private, replace with properties for more idiomatic C#.

%rename("GetIsEgg") pkmn::swig::pokemon2::is_egg;
%rename("GetIsShiny") pkmn::swig::pokemon2::is_shiny;

%csmethodmodifiers pkmn::swig::pokemon2::get_species() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_game() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_form() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_form(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::is_egg() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_is_egg(bool) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_database_entry() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_condition() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_condition(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_nickname() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_nickname(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_gender() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_gender(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::is_shiny() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_shininess(bool) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_held_item() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_held_item(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_pokerus_duration() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_pokerus_duration(int) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_original_trainer_name() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_original_trainer_name(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_original_trainer_public_id() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_original_trainer_public_id(uint16_t) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_original_trainer_secret_id() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_original_trainer_secret_id(uint16_t) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_original_trainer_id() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_original_trainer_id(uint32_t) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_original_trainer_gender() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_original_trainer_gender(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_current_trainer_friendship() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_current_trainer_friendship(int) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_ability() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_ability(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_ball() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_ball(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_level_met() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_level_met(int) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_location_met(bool) "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_location_met(const std::string&, bool) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_original_game() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_original_game(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_personality() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_personality(uint32_t) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_experience() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_experience(int) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_level() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_level(int) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_EVs() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_IVs() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_markings() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_ribbons() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_contest_stats() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_moves() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_stats() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_icon_filepath() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_sprite_filepath() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_numeric_attributes() "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_string_attributes() "private";

%csmethodmodifiers pkmn::swig::pokemon2::cptr() "private";

%typemap(cscode) pkmn::swig::pokemon2
%{
    public string Species
    {
        get { return GetSpecies(); }
    }

    public string Game
    {
        get { return GetGame(); }
    }

    public string Form
    {
        get { return GetForm(); }
        set { SetForm(value); }
    }

    public bool IsEgg
    {
        get { return GetIsEgg(); }
        set { SetIsEgg(value); }
    }

    public PokemonEntry DatabaseEntry
    {
        get { return GetDatabaseEntry(); }
    }

    public string Condition
    {
        get { return GetCondition(); }
        set { SetCondition(value); }
    }

    public string Nickname
    {
        get { return GetNickname(); }
        set { SetNickname(value); }
    }

    public string Gender
    {
        get { return GetGender(); }
        set { SetGender(value); }
    }

    public bool IsShiny
    {
        get { return GetIsShiny(); }
        set { SetShininess(value); }
    }

    public string HeldItem
    {
        get { return GetHeldItem(); }
        set { SetHeldItem(value); }
    }

    public int PokerusDuration
    {
        get { return GetPokerusDuration(); }
        set { SetPokerusDuration(value); }
    }

    public string OriginalTrainerName
    {
        get { return GetOriginalTrainerName(); }
        set { SetOriginalTrainerName(value); }
    }

    public ushort OriginalTrainerPublicID
    {
        get { return GetOriginalTrainerPublicID(); }
        set { SetOriginalTrainerPublicID(value); }
    }

    public ushort OriginalTrainerSecretID
    {
        get { return GetOriginalTrainerSecretID(); }
        set { SetOriginalTrainerSecretID(value); }
    }

    public uint OriginalTrainerID
    {
        get { return GetOriginalTrainerID(); }
        set { SetOriginalTrainerID(value); }
    }

    public string OriginalTrainerGender
    {
        get { return GetOriginalTrainerGender(); }
        set { SetOriginalTrainerGender(value); }
    }

    public int CurrentTrainerFriendship
    {
        get { return GetCurrentTrainerFriendship(); }
        set { SetCurrentTrainerFriendship(value); }
    }

    public string Ability
    {
        get { return GetAbility(); }
        set { SetAbility(value); }
    }

    public string Ball
    {
        get { return GetBall(); }
        set { SetBall(value); }
    }

    public int LevelMet
    {
        get { return GetLevelMet(); }
        set { SetLevelMet(value); }
    }

    public string LocationMet
    {
        get { return GetLocationMet(false); }
        set { SetLocationMet(value, false); }
    }

    public string LocationMetAsEgg
    {
        get { return GetLocationMet(true); }
        set { SetLocationMet(value, true); }
    }

    public string OriginalGame
    {
        get { return GetOriginalGame(); }
        set { SetOriginalGame(value); }
    }

    public uint Personality
    {
        get { return GetPersonality(); }
        set { SetPersonality(value); }
    }

    public int Experience
    {
        get { return GetExperience(); }
        set { SetExperience(value); }
    }

    public int Level
    {
        get { return GetLevel(); }
        set { SetLevel(value); }
    }

    public EVMap EVs
    {
        get { return GetEVs(); }
    }

    public IVMap IVs
    {
        get { return GetIVs(); }
    }

    public MarkingMap Markings
    {
        get { return GetMarkings(); }
    }

    public RibbonMap Ribbons
    {
        get { return GetRibbons(); }
    }

    public ContestStatMap ContestStats
    {
        get { return GetContestStats(); }
    }

    public MoveSlots2 Moves
    {
        get { return GetMoves(); }
    }

    public StringIntDict Stats
    {
        get { return GetStats(); }
    }

    public string IconFilepath
    {
        get { return GetIconFilepath(); }
    }

    public string SpriteFilepath
    {
        get { return GetSpriteFilepath(); }
    }

    public PokemonNumericAttributeMap NumericAttributes
    {
        get { return GetNumericAttributes(); }
    }

    public PokemonStringAttributeMap StringAttributes
    {
        get { return GetStringAttributes(); }
    }
%}

%include "cpp_wrappers/pokemon.hpp"
