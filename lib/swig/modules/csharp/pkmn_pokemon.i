/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon.hpp"
    #include "cpp_wrappers/pokemon_helpers.hpp"
%}

%import <csharp/pkmn_pokemon_helpers.i>

%include <attribute.i>

%typemap(csimports) pkmn::swig::pokemon "
using System;
using System.Runtime.InteropServices;
using Database;"

%ignore pkmn::swig::pokemon::pokemon();
%ignore pkmn::swig::pokemon::pokemon(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::pokemon::pokemon(const pkmn::swig::pokemon&);
%ignore pkmn::swig::pokemon::get_internal() const;

// Modify these to limit access.
%csmethodmodifiers pkmn::swig::pokemon::cptr() "private";
%csmethodmodifiers pkmn::swig::pokemon::get_species() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_game() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_form() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_form(const std::string&) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_is_egg() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_is_egg(bool) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_condition() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_condition(pkmn::e_condition) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_nickname() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_nickname(const std::string&) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_gender() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_gender(pkmn::e_gender) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_is_shiny() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_shininess(bool) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_held_item() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_held_item(pkmn::e_item) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_nature() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_nature(pkmn::e_nature) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_pokerus_duration() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_pokerus_duration(int) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_original_trainer_name() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_original_trainer_name(const std::string&) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_original_trainer_public_id() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_original_trainer_public_id(uint16_t) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_original_trainer_secret_id() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_original_trainer_secret_id(uint16_t) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_original_trainer_id() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_original_trainer_id(uint32_t) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_original_trainer_gender() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_original_trainer_gender(pkmn::e_gender) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_language() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_language(pkmn::e_language) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_current_trainer_friendship() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_current_trainer_friendship(int) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_ability() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_ability(pkmn::e_ability) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_ball() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_ball(pkmn::e_ball) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_level_met() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_level_met(int) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_location_met() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_location_met(const std::string&) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_location_met_as_egg() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_location_met_as_egg(const std::string&) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_original_game() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_original_game(pkmn::e_game) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_personality() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_personality(uint32_t) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_experience() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_experience(int) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_level() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_level(int) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_current_hp() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::set_current_hp(int) "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_icon_filepath() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_sprite_filepath() const "protected virtual";
%csmethodmodifiers pkmn::swig::pokemon::get_database_entry_swig() const "protected virtual";

// Rename functions specific to SWIG class whose name were changed to remove
// ambiguity from the base class.
%rename(Clone) pkmn::swig::pokemon::clone_swig;
%rename(ToGame) pkmn::swig::pokemon::to_game_swig;
%rename(GetDatabaseEntry) pkmn::swig::pokemon::get_database_entry_swig;
%rename(GetStats) pkmn::swig::pokemon::get_stats_swig;

// This will also prevent these from being overridden.
%attributeval(pkmn::swig::pokemon, pkmn::swig::EV_map, EVs, get_EVs_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::IV_map, IVs, get_IVs_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::marking_map, Markings, get_markings_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::ribbon_map, Ribbons, get_ribbons_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::contest_stat_map, ContestStats, get_contest_stats_helper);
%attributeval(pkmn::swig::pokemon, pkmn::swig::move_slots, Moves, get_moves_helper);
%attributeval(pkmn::swig::pokemon, %arg(std::map<pkmn::e_stat, int>), Stats, get_stats_swig);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::numeric_attribute_map<pkmn::pokemon>), NumericAttributes, get_numeric_attributes);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::string_attribute_map<pkmn::pokemon>), StringAttributes, get_string_attributes);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::boolean_attribute_map<pkmn::pokemon>), BooleanAttributes, get_boolean_attributes);

%typemap(cscode) pkmn::swig::pokemon
%{
    /*
     * We can't use SWIG's attributes to create these properties because we need
     * the Get/Set functions to support directors.
     */

    public Species Species
    {
        get { return GetSpecies(); }
    }
    public Game Game
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
    public string Nickname
    {
        get { return GetNickname(); }
        set { SetNickname(value); }
    }
    public PokemonEntry DatabaseEntry
    {
        get { return GetDatabaseEntry(); }
    }
    public Condition Condition
    {
        get { return GetCondition(); }
        set { SetCondition(value); }
    }
    public Gender Gender
    {
        get { return GetGender(); }
        set { SetGender(value); }
    }
    public bool IsShiny
    {
        get { return GetIsShiny(); }
        set { SetShininess(value); }
    }
    public Item HeldItem
    {
        get { return GetHeldItem(); }
        set { SetHeldItem(value); }
    }
    public Nature Nature
    {
        get { return GetNature(); }
        set { SetNature(value); }
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
    public Gender OriginalTrainerGender
    {
        get { return GetOriginalTrainerGender(); }
        set { SetOriginalTrainerGender(value); }
    }
    public Language Language
    {
        get { return GetLanguage(); }
        set { SetLanguage(value); }
    }
    public int CurrentTrainerFriendship
    {
        get { return GetCurrentTrainerFriendship(); }
        set { SetCurrentTrainerFriendship(value); }
    }
    public Ability Ability
    {
        get { return GetAbility(); }
        set { SetAbility(value); }
    }
    public Ball Ball
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
        get { return GetLocationMet(); }
        set { SetLocationMet(value); }
    }
    public string LocationMetAsEgg
    {
        get { return GetLocationMetAsEgg(); }
        set { SetLocationMetAsEgg(value); }
    }
    public Game OriginalGame
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
    public int CurrentHP
    {
        get { return GetCurrentHP(); }
        set { SetCurrentHP(value); }
    }
    public string IconFilepath
    {
        get { return GetIconFilepath(); }
    }
    public string SpriteFilepath
    {
        get { return GetSpriteFilepath(); }
    }

    public virtual System.Drawing.Image Icon
    {
        get
        {
            /*
             * Until recent versions, Mono's System.Drawing.Image implementation used
             * a version of libgdiplus that doesn't support 64bpp PNGs. If this is
             * the case, this function will write a temporary copy that Mono can import.
             *
             * Which exception is thrown depends on the version of libgdiplus.
             */
            try
            {
                return System.Drawing.Image.FromFile(this.IconFilepath);
            }
            catch (System.ArgumentException)
            {
                string tmpFilepath = PKMN.ConvertImageForMono(this.IconFilepath);
                System.Drawing.Image tmpImage = System.Drawing.Image.FromFile(tmpFilepath);
                System.IO.File.Delete(tmpFilepath);

                return tmpImage;
            }
            catch (System.NotSupportedException)
            {
                string tmpFilepath = PKMN.ConvertImageForMono(this.IconFilepath);
                System.Drawing.Image tmpImage = System.Drawing.Image.FromFile(tmpFilepath);
                System.IO.File.Delete(tmpFilepath);

                return tmpImage;
            }
        }
    }

    public virtual System.Drawing.Image Sprite
    {
        get
        {
            /*
             * Until recent versions, Mono's System.Drawing.Image implementation used
             * a version of libgdiplus that doesn't support 64bpp PNGs. If this is
             * the case, this function will write a temporary copy that Mono can import.
             *
             * Which exception is thrown depends on the version of libgdiplus.
             */
            try
            {
                return System.Drawing.Image.FromFile(this.SpriteFilepath);
            }
            catch (System.ArgumentException)
            {
                string tmpFilepath = PKMN.ConvertImageForMono(this.SpriteFilepath);
                System.Drawing.Image tmpImage = System.Drawing.Image.FromFile(tmpFilepath);
                System.IO.File.Delete(tmpFilepath);

                return tmpImage;
            }
            catch (System.NotSupportedException)
            {
                string tmpFilepath = PKMN.ConvertImageForMono(this.SpriteFilepath);
                System.Drawing.Image tmpImage = System.Drawing.Image.FromFile(tmpFilepath);
                System.IO.File.Delete(tmpFilepath);

                return tmpImage;
            }
        }
    }

    public bool Equals(Pokemon other)
    {
        if(other == null)
        {
            return false;
        }
        else if(this == other)
        {
            return true;
        }
        else
        {
            return (this.Cptr() == other.Cptr());
        }
    }

    public override bool Equals(System.Object other)
    {
        if(other == null)
        {
            return false;
        }

        Pokemon otherAsPokemon = other as Pokemon;
        if(otherAsPokemon == null)
        {
            return false;
        }
        else
        {
            return this.Equals(otherAsPokemon);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .ToHashCode();
    }
%}

%feature("director", "1") pkmn::swig::pokemon;
%include "cpp_wrappers/pokemon.hpp"
