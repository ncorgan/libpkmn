/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class Gen3PokemonTests
{
    private static string[] ContestTypes = {"Cool", "Beauty", "Cute", "Smart", "Tough"};
    private static string[] ContestLevels = {"Super", "Hyper", "Master"};
    private static string[] Ribbons = {"Champion", "Winning", "Victory", "Artist",
                                       "Effort", "Marine", "Land", "Sky",
                                       "Country", "National", "Earth", "World"};

    static private void CheckInitialRibbonMap(
        PKMN.Pokemon pokemon
    )
    {
        foreach(string contestType in ContestTypes)
        {
            Assert.IsTrue(pokemon.Ribbons.ContainsKey(contestType));
            Assert.IsFalse(pokemon.Ribbons[contestType]);

            foreach(string contestLevel in ContestLevels)
            {
                string ribbonName = contestType + " " + contestLevel;
                Assert.IsTrue(pokemon.Ribbons.ContainsKey(ribbonName));
                Assert.IsFalse(pokemon.Ribbons[ribbonName]);
            }
        }

        foreach(string ribbon in Ribbons)
        {
            Assert.IsTrue(pokemon.Ribbons.ContainsKey(ribbon));
            Assert.IsFalse(pokemon.Ribbons[ribbon]);
        }
    }

    static private void TestContestRibbons(
        PKMN.Pokemon pokemon
    )
    {
        foreach(string contestType in ContestTypes)
        {
            string ribbonName = contestType;
            string superRibbonName = contestType + " Super";
            string hyperRibbonName = contestType + " Hyper";
            string masterRibbonName = contestType + " Master";

            pokemon.Ribbons[hyperRibbonName] = true;
            Assert.IsTrue(pokemon.Ribbons[ribbonName]);
            Assert.IsTrue(pokemon.Ribbons[superRibbonName]);
            Assert.IsTrue(pokemon.Ribbons[hyperRibbonName]);
            Assert.IsFalse(pokemon.Ribbons[masterRibbonName]);

            pokemon.Ribbons[superRibbonName] = false;
            Assert.IsTrue(pokemon.Ribbons[ribbonName]);
            Assert.IsFalse(pokemon.Ribbons[superRibbonName]);
            Assert.IsFalse(pokemon.Ribbons[hyperRibbonName]);
            Assert.IsFalse(pokemon.Ribbons[masterRibbonName]);
        }
    }

    static private void TestRibbons(
        PKMN.Pokemon pokemon
    )
    {
        foreach(string ribbon in Ribbons)
        {
            pokemon.Ribbons[ribbon] = true;
            Assert.IsTrue(pokemon.Ribbons[ribbon]);
        }
    }

    static public void PokemonTest(
        PKMN.Game game,
        PKMN.Species species
    )
    {
        PKMN.Pokemon pokemon = new PKMN.Pokemon(species, game, "", 30);

        PokemonTestParams testParams;
        bool isGamecube = (game == PKMN.Game.COLOSSEUM) || (game == PKMN.Game.XD);

        if(isGamecube)
        {
            testParams = new PokemonTestParams(
                                 PKMN.Ball.GREAT_BALL,
                                 new PKMN.Ball[]{PKMN.Ball.FRIEND_BALL, PKMN.Ball.HEAL_BALL},
                                 PKMN.Item.RAZZ_BERRY,
                                 new PKMN.Item[]{PKMN.Item.BERRY, PKMN.Item.MACH_BIKE},
                                 "Distant land",
                                 new string[]{"Phenac City", "Orre Colosseum"},
                                 new string[]{"New Bark Town", "Twinleaf Town"},
                                 new PKMN.Move[]{
                                     PKMN.Move.SWALLOW,
                                     PKMN.Move.FLAMETHROWER,
                                     PKMN.Move.RETURN,
                                     PKMN.Move.FIRE_BLAST
                                 },
                                 new PKMN.Move[]{
                                     PKMN.Move.ROOST,
                                     PKMN.Move.FLAME_BURST
                                 },
                                 new PKMN.Game[]{
                                         PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD,
                                         PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
                                         PKMN.Game.COLOSSEUM, PKMN.Game.XD
                                     },
                                 new PKMN.Game[]{PKMN.Game.GOLD, PKMN.Game.HEARTGOLD}
                             );
        }
        else
        {
            testParams = new PokemonTestParams(
                                 PKMN.Ball.GREAT_BALL,
                                 new PKMN.Ball[]{PKMN.Ball.FRIEND_BALL, PKMN.Ball.HEAL_BALL},
                                 PKMN.Item.RAZZ_BERRY,
                                 new PKMN.Item[]{PKMN.Item.BERRY, PKMN.Item.MACH_BIKE},
                                 "Fateful encounter",
                                 new string[]{"Petalburg Woods", "Viridian Forest"},
                                 new string[]{"New Bark Town", "Twinleaf Town"},
                                 new PKMN.Move[]{
                                     PKMN.Move.SWALLOW,
                                     PKMN.Move.FLAMETHROWER,
                                     PKMN.Move.RETURN,
                                     PKMN.Move.FIRE_BLAST
                                 },
                                 new PKMN.Move[]{
                                     PKMN.Move.SHADOW_SKY,
                                     PKMN.Move.ROOST
                                 },
                                 new PKMN.Game[]{
                                         PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD,
                                         PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
                                         PKMN.Game.COLOSSEUM, PKMN.Game.XD
                                     },
                                 new PKMN.Game[]{PKMN.Game.GOLD, PKMN.Game.HEARTGOLD}
                             );
        }

        PokemonTestCommon.TestCommon(pokemon, testParams);

        CheckInitialRibbonMap(pokemon);
        TestContestRibbons(pokemon);
        TestRibbons(pokemon);

        // Gender and personality are tied, so make sure they affect each other.

        pokemon.Gender = PKMN.Gender.FEMALE;
        Assert.Less((pokemon.Personality & 0xFF), 0xFF);
        pokemon.Gender = PKMN.Gender.MALE;
        Assert.AreEqual((pokemon.Personality & 0xFF), 0xFF);

        pokemon.Personality = 0x1234AB00;
        Assert.AreEqual(pokemon.Gender, PKMN.Gender.FEMALE);
        pokemon.Personality = 0xCD5678FF;
        Assert.AreEqual(pokemon.Gender, PKMN.Gender.MALE);

        // Setting shininess should affect personality.

        pokemon.IsShiny = false;
        uint nonShinyPersonality = pokemon.Personality;

        pokemon.IsShiny = true;
        Assert.AreNotEqual(pokemon.Personality, nonShinyPersonality);

        // Shadow Pokémon should only work in Gamecube games.

        if(isGamecube)
        {
            PKMN.Species shadowSpecies = (game == PKMN.Game.COLOSSEUM) ? PKMN.Species.LEDIAN
                                                                       : PKMN.Species.LEDYBA;

            PKMN.Pokemon shadowPokemon = new PKMN.Pokemon(shadowSpecies, game, "", 50);
            Assert.AreEqual(shadowPokemon.Form, "Standard");
            shadowPokemon.Form = "Shadow";
            Assert.AreEqual(shadowPokemon.Form, "Shadow");
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon(PKMN.Species.LEDYBA, game, "Shadow", 50);
                }
            );
        }
    }
}

}
