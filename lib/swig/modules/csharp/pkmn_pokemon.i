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

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokemon::cptr() "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::pokemon, pkmn::e_species, Species, get_species);
%attribute(pkmn::swig::pokemon, pkmn::e_game, Game, get_game);
%attributestring(pkmn::swig::pokemon, std::string, Form, get_form, set_form);
%attribute(pkmn::swig::pokemon, bool, IsEgg, is_egg, set_is_egg);
%attributestring(pkmn::swig::pokemon, std::string, Nickname, get_nickname, set_nickname);
%attributeval(pkmn::swig::pokemon, pkmn::database::pokemon_entry, DatabaseEntry, get_database_entry);
%attributestring(pkmn::swig::pokemon, std::string, Condition, get_condition, set_condition);
%attribute(pkmn::swig::pokemon, pkmn::e_gender, Gender, get_gender, set_gender);
%attribute(pkmn::swig::pokemon, bool, IsShiny, is_shiny, set_shininess);
%attributestring(pkmn::swig::pokemon, pkmn::e_item, HeldItem, get_held_item, set_held_item);
%attribute(pkmn::swig::pokemon, int, PokerusDuration, get_pokerus_duration, set_pokerus_duration);
%attributestring(pkmn::swig::pokemon, std::string, OriginalTrainerName, get_original_trainer_name, set_original_trainer_name);
%attribute(pkmn::swig::pokemon, uint16_t, OriginalTrainerPublicID, get_original_trainer_public_id, set_original_trainer_public_id);
%attribute(pkmn::swig::pokemon, uint16_t, OriginalTrainerSecretID, get_original_trainer_secret_id, set_original_trainer_secret_id);
%attribute(pkmn::swig::pokemon, uint32_t, OriginalTrainerID, get_original_trainer_id, set_original_trainer_id);
%attribute(pkmn::swig::pokemon, pkmn::e_gender, OriginalTrainerGender, get_original_trainer_gender, set_original_trainer_gender);
%attribute(pkmn::swig::pokemon, pkmn::e_language, Language, get_language, set_language);
%attribute(pkmn::swig::pokemon, int, CurrentTrainerFriendship, get_current_trainer_friendship, set_current_trainer_friendship);
%attributestring(pkmn::swig::pokemon, std::string, Ability, get_ability, set_ability);
%attributestring(pkmn::swig::pokemon, pkmn::e_ball, Ball, get_ball, set_ball);
%attribute(pkmn::swig::pokemon, int, LevelMet, get_level_met, set_level_met);
%attributestring(pkmn::swig::pokemon, std::string, LocationMet, get_location_met, set_location_met);
%attributestring(pkmn::swig::pokemon, std::string, LocationMetAsEgg, get_location_met_as_egg, set_location_met_as_egg);
%attribute(pkmn::swig::pokemon, pkmn::e_game, OriginalGame, get_original_game, set_original_game);
%attribute(pkmn::swig::pokemon, uint32_t, Personality, get_personality, set_personality);
%attribute(pkmn::swig::pokemon, int, Experience, get_experience, set_experience);
%attribute(pkmn::swig::pokemon, int, Level, get_level, set_level);
%attribute(pkmn::swig::pokemon, int, CurrentHP, get_current_hp, set_current_hp);
%attributeval(pkmn::swig::pokemon, pkmn::swig::EV_map, EVs, get_EVs);
%attributeval(pkmn::swig::pokemon, pkmn::swig::IV_map, IVs, get_IVs);
%attributeval(pkmn::swig::pokemon, pkmn::swig::marking_map, Markings, get_markings);
%attributeval(pkmn::swig::pokemon, pkmn::swig::ribbon_map, Ribbons, get_ribbons);
%attributeval(pkmn::swig::pokemon, pkmn::swig::contest_stat_map, ContestStats, get_contest_stats);
%attributeval(pkmn::swig::pokemon, pkmn::swig::move_slots, Moves, get_moves);
%attributeval(pkmn::swig::pokemon, %arg(std::map<pkmn::e_stat, int>), Stats, get_stats);
%attributestring(pkmn::swig::pokemon, std::string, IconFilepath, get_icon_filepath);
%attributestring(pkmn::swig::pokemon, std::string, SpriteFilepath, get_sprite_filepath);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::numeric_attribute_map<pkmn::pokemon>), NumericAttributes, get_numeric_attributes);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::string_attribute_map<pkmn::pokemon>), StringAttributes, get_string_attributes);
%attributeval(pkmn::swig::pokemon, %arg(pkmn::swig::boolean_attribute_map<pkmn::pokemon>), BooleanAttributes, get_boolean_attributes);

%typemap(cscode) pkmn::swig::pokemon
%{
    public System.Drawing.Image Icon
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

    public System.Drawing.Image Sprite
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

%include "cpp_wrappers/pokemon.hpp"
