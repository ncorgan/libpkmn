/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/pokemon_entry.hpp>
%}

%include <attribute.i>

%attribute(pkmn::database::pokemon_entry, e_species, Species, get_species);
%attributestring(pkmn::database::pokemon_entry, std::string, SpeciesName, get_species_name);
%attribute(pkmn::database::pokemon_entry, e_game, Game, get_game);
%attributestring(pkmn::database::pokemon_entry, std::string, Category, get_category);
%attributestring(pkmn::database::pokemon_entry, std::string, PokedexEntry, get_pokedex_entry);
%attributestring(pkmn::database::pokemon_entry, std::string, Form, get_form, set_form);
%attribute(pkmn::database::pokemon_entry, float, Height, get_height);
%attribute(pkmn::database::pokemon_entry, float, Weight, get_weight);
%attribute(pkmn::database::pokemon_entry, float, ChanceMale, get_chance_male);
%attribute(pkmn::database::pokemon_entry, float, ChanceFemale, get_chance_female);
%attribute(pkmn::database::pokemon_entry, bool, HasGenderDifferences, has_gender_differences);
%attribute(pkmn::database::pokemon_entry, int, BaseFriendship, get_base_friendship);
%attributeval(pkmn::database::pokemon_entry, %arg(std::pair<pkmn::e_type, pkmn::e_type>), Types, get_types);
%attributeval(pkmn::database::pokemon_entry, %arg(std::pair<pkmn::e_ability, pkmn::e_ability>), Abilities, get_abilities);
%attribute(pkmn::database::pokemon_entry, pkmn::e_ability, HiddenAbility, get_hidden_ability);
%attributeval(pkmn::database::pokemon_entry, %arg(std::pair<pkmn::e_egg_group, pkmn::e_egg_group>), EggGroups, get_egg_groups);
%attributeval(pkmn::database::pokemon_entry, %arg(std::map<pkmn::e_stat, int>), BaseStats, get_base_stats);
%attributeval(pkmn::database::pokemon_entry, %arg(std::map<pkmn::e_stat, int>), EVYields, get_EV_yields);
%attribute(pkmn::database::pokemon_entry, int, ExperienceYield, get_experience_yield);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::levelup_move>), LevelupMoves, get_levelup_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::e_move>), TMHMMoves, get_tm_hm_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::e_move>), EggMoves, get_egg_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::e_move>), TutorMoves, get_tutor_moves);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<std::string>), Forms, get_forms);
%attributeval(pkmn::database::pokemon_entry, %arg(std::vector<pkmn::database::pokemon_entry>), Evolutions, get_evolutions);

%ignore pkmn::database::pokemon_entry::get_species_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_id;
%ignore pkmn::database::pokemon_entry::get_form_id;
%ignore pkmn::database::pokemon_entry::get_pokemon_index;
%ignore pkmn::database::pokemon_entry::get_game_id;

%typemap(cscode) pkmn::database::pokemon_entry
%{
    public System.Drawing.Image GetIcon(bool isFemale)
    {
        string iconFilepath = this.GetIconFilepath(isFemale);

        /*
         * Until recent versions, Mono's System.Drawing.Image implementation used
         * a version of libgdiplus that doesn't support 64bpp PNGs. If this is
         * the case, this function will write a temporary copy that Mono can import.
         *
         * Which exception is thrown depends on the version of libgdiplus.
         */
        try
        {
            return System.Drawing.Image.FromFile(iconFilepath);
        }
        catch (System.ArgumentException)
        {
            string tmpFilepath = PKMN.ConvertImageForMono(iconFilepath);
            System.Drawing.Image tmpImage = System.Drawing.Image.FromFile(tmpFilepath);
            System.IO.File.Delete(tmpFilepath);

            return tmpImage;
        }
        catch (System.NotSupportedException)
        {
            string tmpFilepath = PKMN.ConvertImageForMono(iconFilepath);
            System.Drawing.Image tmpImage = System.Drawing.Image.FromFile(tmpFilepath);
            System.IO.File.Delete(tmpFilepath);

            return tmpImage;
        }
    }

    public System.Drawing.Image GetSprite(bool isFemale, bool isShiny)
    {
        string spriteFilepath = this.GetSpriteFilepath(isFemale, isShiny);

        /*
         * Until recent versions, Mono's System.Drawing.Image implementation used
         * a version of libgdiplus that doesn't support 64bpp PNGs. If this is
         * the case, this function will write a temporary copy that Mono can import.
         *
         * Which exception is thrown depends on the version of libgdiplus.
         */
        try
        {
            return System.Drawing.Image.FromFile(spriteFilepath);
        }
        catch (System.ArgumentException)
        {
            string tmpFilepath = PKMN.ConvertImageForMono(spriteFilepath);
            System.Drawing.Image tmpImage = System.Drawing.Image.FromFile(tmpFilepath);
            System.IO.File.Delete(tmpFilepath);

            return tmpImage;
        }
        catch (System.NotSupportedException)
        {
            string tmpFilepath = PKMN.ConvertImageForMono(spriteFilepath);
            System.Drawing.Image tmpImage = System.Drawing.Image.FromFile(tmpFilepath);
            System.IO.File.Delete(tmpFilepath);

            return tmpImage;
        }
    }

    public bool Equals(PokemonEntry rhs)
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
            return this.Species.Equals(rhs.Species) &&
                   this.Game.Equals(rhs.Game) &&
                   this.Form.Equals(rhs.Form);
        }
    }

    public override bool Equals(System.Object rhs)
    {
        if(rhs == null)
        {
            return false;
        }

        PokemonEntry rhsEntry = rhs as PokemonEntry;
        if(rhsEntry == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsEntry);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<Species>(this.Species)
                                       .AddValue<Game>(this.Game)
                                       .AddValue<string>(this.Form)
                                       .ToHashCode();
    }
%}

%include <pkmn/database/pokemon_entry.hpp>

%include <csharp/stl_macros.i>
PKMN_CSHARP_VECTOR(pkmn::database::pokemon_entry, PokemonEntry, PokemonEntryList)
