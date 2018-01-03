/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

%typemap(csimports) pkmn::swig::pokemon2 "
using System;
using System.Runtime.InteropServices;
using Database;"

%ignore pkmn::swig::pokemon2::pokemon2();
%ignore pkmn::swig::pokemon2::pokemon2(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::pokemon2::pokemon2(const pkmn::swig::pokemon2&);
%ignore pkmn::swig::pokemon2::get_internal() const;

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokemon2::cptr() "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::pokemon2, std::string, Species, get_species);
%attributestring(pkmn::swig::pokemon2, std::string, Game, get_game);
%attributestring(pkmn::swig::pokemon2, std::string, Form, get_form, set_form);
%attribute(pkmn::swig::pokemon2, bool, IsEgg, is_egg, set_is_egg);
%attributestring(pkmn::swig::pokemon2, std::string, Nickname, get_nickname, set_nickname);
%attributeval(pkmn::swig::pokemon2, pkmn::database::pokemon_entry, DatabaseEntry, get_database_entry);
%attributestring(pkmn::swig::pokemon2, std::string, Condition, get_condition, set_condition);
%attributestring(pkmn::swig::pokemon2, std::string, Gender, get_gender, set_gender);
%attribute(pkmn::swig::pokemon2, bool, IsShiny, is_shiny, set_shininess);
%attributestring(pkmn::swig::pokemon2, std::string, HeldItem, get_held_item, set_held_item);
%attribute(pkmn::swig::pokemon2, int, PokerusDuration, get_pokerus_duration, set_pokerus_duration);
%attributestring(pkmn::swig::pokemon2, std::string, OriginalTrainerName, get_original_trainer_name, set_original_trainer_name);
%attribute(pkmn::swig::pokemon2, uint16_t, OriginalTrainerPublicID, get_original_trainer_public_id, set_original_trainer_public_id);
%attribute(pkmn::swig::pokemon2, uint16_t, OriginalTrainerSecretID, get_original_trainer_secret_id, set_original_trainer_secret_id);
%attribute(pkmn::swig::pokemon2, uint32_t, OriginalTrainerID, get_original_trainer_id, set_original_trainer_id);
%attributestring(pkmn::swig::pokemon2, std::string, OriginalTrainerGender, get_original_trainer_gender, set_original_trainer_gender);
%attribute(pkmn::swig::pokemon2, int, CurrentTrainerFriendship, get_current_trainer_friendship, set_current_trainer_friendship);
%attributestring(pkmn::swig::pokemon2, std::string, Ability, get_ability, set_ability);
%attributestring(pkmn::swig::pokemon2, std::string, Ball, get_ball, set_ball);
%attributestring(pkmn::swig::pokemon2, std::string, OriginalGame, get_original_game, set_original_game);
%attribute(pkmn::swig::pokemon2, uint32_t, Personality, get_personality, set_personality);
%attribute(pkmn::swig::pokemon2, int, Experience, get_experience, set_experience);
%attribute(pkmn::swig::pokemon2, int, Level, get_level, set_level);
%attribute(pkmn::swig::pokemon2, int, CurrentHP, get_current_hp, set_current_hp);
%attributeval(pkmn::swig::pokemon2, pkmn::swig::EV_map, EVs, get_EVs);
%attributeval(pkmn::swig::pokemon2, pkmn::swig::IV_map, IVs, get_IVs);
%attributeval(pkmn::swig::pokemon2, pkmn::swig::marking_map, Markings, get_markings);
%attributeval(pkmn::swig::pokemon2, pkmn::swig::ribbon_map, Ribbons, get_ribbons);
%attributeval(pkmn::swig::pokemon2, pkmn::swig::contest_stat_map, ContestStats, get_contest_stats);
%attributeval(pkmn::swig::pokemon2, pkmn::swig::move_slots2, Moves, get_moves);
%attributeval(pkmn::swig::pokemon2, %arg(std::map<std::string, int>), Stats, get_stats);
%attributestring(pkmn::swig::pokemon2, std::string, IconFilepath, get_icon_filepath);
%attributestring(pkmn::swig::pokemon2, std::string, SpriteFilepath, get_sprite_filepath);
%attributeval(pkmn::swig::pokemon2, %arg(pkmn::swig::numeric_attribute_map<pkmn::pokemon>), NumericAttributes, get_numeric_attributes);
%attributeval(pkmn::swig::pokemon2, %arg(pkmn::swig::string_attribute_map<pkmn::pokemon>), StringAttributes, get_string_attributes);

// As these are not simple getters and setters, this wrapping must be manually done.

%csmethodmodifiers pkmn::swig::pokemon2::get_level_met() "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_level_met(int) "private";
%csmethodmodifiers pkmn::swig::pokemon2::get_location_met(bool) "private";
%csmethodmodifiers pkmn::swig::pokemon2::set_location_met(const std::string&, bool) "private";

%typemap(cscode) pkmn::swig::pokemon2
%{
#if __DOXYGEN__
    // So internal functions aren't exposed in public documentation...
    public static readonly uint DEFAULT_TRAINER_ID;
    public static readonly string DEFAULT_TRAINER_NAME;
#else
    public static readonly uint DEFAULT_TRAINER_ID = PKMN.get_default_trainer_id();
    public static readonly string DEFAULT_TRAINER_NAME = PKMN.get_default_trainer_name();
#endif

    public int LevelMet
    {
        get { return GetLevelMet(); }
        set { SetLevelMet(value); }
    }

    // TODO: LevelMetAsEgg when implemented

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

    public bool Equals(Pokemon2 other)
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

        Pokemon2 otherAsPokemon = other as Pokemon2;
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

%include "cpp_wrappers/pokemon.hpp"
