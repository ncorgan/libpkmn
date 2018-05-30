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
                "None",
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
                Assert.AreEqual("None", daycare.BreedingPokemon[breedingPokemonIndex].Species);
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
                                            "Venusaur",
                                            daycare.Game,
                                            "",
                                            50
                                        );
        Assert.AreEqual("Venusaur", daycare.LevelupPokemon[0].Species);

        if(daycare.LevelupPokemon.Length == 2)
        {
            daycare.LevelupPokemon[1] = new PKMN.Pokemon(
                                                "Charizard",
                                                daycare.Game,
                                                "",
                                                50
                                            );
            Assert.AreEqual("Charizard", daycare.LevelupPokemon[1].Species);
        }

        if(daycare.CanBreedPokemon)
        {
            // TODO: validate genders
            daycare.BreedingPokemon[0] = new PKMN.Pokemon(
                                                 "Blastoise",
                                                 daycare.Game,
                                                 "",
                                                 50
                                             );
            daycare.BreedingPokemon[0].Gender = "Female";

            Assert.AreEqual("Blastoise", daycare.BreedingPokemon[0].Species);
            Assert.AreEqual("Female", daycare.BreedingPokemon[0].Gender);

            daycare.BreedingPokemon[1] = new PKMN.Pokemon(
                                                 "Marowak",
                                                 daycare.Game,
                                                 "",
                                                 50
                                             );
            daycare.BreedingPokemon[1].Gender = "Male";

            Assert.AreEqual("Marowak", daycare.BreedingPokemon[1].Species);
            Assert.AreEqual("Male", daycare.BreedingPokemon[1].Gender);

            // TODO: test egg
        }
    }

    [TestCase("Red", false)]
    [TestCase("Blue", false)]
    [TestCase("Yellow", false)]
    [TestCase("Gold", true)]
    [TestCase("Silver", true)]
    [TestCase("Crystal", true)]
    /*[TestCase("Ruby", true)]
    [TestCase("Sapphire", true)]
    [TestCase("Emerald", true)]
    [TestCase("FireRed", true)]
    [TestCase("LeafGreen", true)]*/
    [TestCase("Colosseum", false)]
    [TestCase("XD", false)]
    public void DaycareTest(
        string game,
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
