/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpDaycareTest
{
    public void testEmptyDaycare(
        PKMN.Daycare daycare
    )
    {
        // Levelup Pokémon

        for(int levelupPokemonIndex = 0;
            levelupPokemonIndex < daycare.LevelupPokemon.Length;
            ++levelupPokemonIndex)
        {
            Assert.AreEqual(
                PKMN.Species.NONE,
                daycare.LevelupPokemon[levelupPokemonIndex].Species
            );
        }

        // Test invalid indices.
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Pokemon pokemon = daycare.LevelupPokemon[-1];
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Pokemon pokemon = daycare.LevelupPokemon[daycare.LevelupPokemon.Length];
            }
        );

        // Breeding Pokémon
        // TODO: test egg

        if(daycare.CanBreedPokemon)
        {
            for(int breedingPokemonIndex = 0;
                breedingPokemonIndex < daycare.BreedingPokemon.Length;
                ++breedingPokemonIndex)
            {
                Assert.AreEqual(PKMN.Species.NONE, daycare.BreedingPokemon[breedingPokemonIndex].Species);
            }

            // Test invalid indices.
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    PKMN.Pokemon pokemon = daycare.BreedingPokemon[-1];
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    PKMN.Pokemon pokemon = daycare.BreedingPokemon[daycare.BreedingPokemon.Length];
                }
            );
        }
    }

    public void testSettingPokemon(
        PKMN.Daycare daycare
    )
    {
        daycare.LevelupPokemon[0] = new PKMN.Pokemon(
                                            PKMN.Species.VENUSAUR,
                                            daycare.Game,
                                            "",
                                            50
                                        );
        Assert.AreEqual(PKMN.Species.VENUSAUR, daycare.LevelupPokemon[0].Species);

        if(daycare.LevelupPokemon.Length == 2)
        {
            daycare.LevelupPokemon[1] = new PKMN.Pokemon(
                                                PKMN.Species.CHARIZARD,
                                                daycare.Game,
                                                "",
                                                50
                                            );
            Assert.AreEqual(PKMN.Species.CHARIZARD, daycare.LevelupPokemon[1].Species);
        }

        if(daycare.CanBreedPokemon)
        {
            // TODO: validate genders
            daycare.BreedingPokemon[0] = new PKMN.Pokemon(
                                                 PKMN.Species.BLASTOISE,
                                                 daycare.Game,
                                                 "",
                                                 50
                                             );
            daycare.BreedingPokemon[0].Gender = PKMN.Gender.FEMALE;

            Assert.AreEqual(PKMN.Species.BLASTOISE, daycare.BreedingPokemon[0].Species);
            Assert.AreEqual(PKMN.Gender.FEMALE, daycare.BreedingPokemon[0].Gender);

            daycare.BreedingPokemon[1] = new PKMN.Pokemon(
                                                 PKMN.Species.MAROWAK,
                                                 daycare.Game,
                                                 "",
                                                 50
                                             );
            daycare.BreedingPokemon[1].Gender = PKMN.Gender.MALE;

            Assert.AreEqual(PKMN.Species.MAROWAK, daycare.BreedingPokemon[1].Species);
            Assert.AreEqual(PKMN.Gender.MALE, daycare.BreedingPokemon[1].Gender);

            // TODO: test egg
        }
    }

    [TestCase(PKMN.Game.RED, false)]
    [TestCase(PKMN.Game.BLUE, false)]
    [TestCase(PKMN.Game.YELLOW, false)]
    [TestCase(PKMN.Game.GOLD, true)]
    [TestCase(PKMN.Game.SILVER, true)]
    [TestCase(PKMN.Game.CRYSTAL, true)]
    /*[TestCase("Ruby", true)]
    [TestCase("Sapphire", true)]
    [TestCase("Emerald", true)]
    [TestCase("FireRed", true)]
    [TestCase("LeafGreen", true)]*/
    [TestCase(PKMN.Game.COLOSSEUM, false)]
    [TestCase(PKMN.Game.XD, false)]
    public void DaycareTest(
        PKMN.Game game,
        bool canBreed
    )
    {
        PKMN.Daycare daycare = new PKMN.Daycare(game);
        Assert.AreEqual(game, daycare.Game);
        Assert.AreEqual(canBreed, daycare.CanBreedPokemon);

        testEmptyDaycare(daycare);
        testSettingPokemon(daycare);
    }
}
