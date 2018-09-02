/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class PokemonPCTests
{
    private static void TestEmptyPokemonBox(
        PKMN.PokemonBox box
    )
    {
        for(int i = 0; i < box.Length; ++i)
        {
            Assert.AreEqual(box[i].Species, PKMN.Species.NONE);
            Assert.AreEqual(box[i].Game, box.Game);

            for(int j = 0; j < box[i].Moves.Count; ++j)
            {
                Assert.AreEqual(box[i].Moves[j].Move, PKMN.Move.NONE);
                Assert.AreEqual(box[i].Moves[j].PP, 0);
            }
        }

        // Make sure trying to get a Pokémon at an invalid index fails.
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Pokemon pokemon = box[-1];
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Pokemon pokemon = box[box.Length];
            }
        );
    }

    private static void TestBoxName(
        PKMN.PokemonBox box
    )
    {
        int generation = Util.GameToGeneration(box.Game);

        if(generation == 1)
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(box.Name, "");

            Assert.Throws<ApplicationException>(
                delegate
                {
                    box.Name = "ABCDEFGH";
                }
            );
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    box.Name = "ABCDEFGHI";
                }
            );

            box.Name = "ABCDEFGH";
            Assert.AreEqual(box.Name, "ABCDEFGH");
        }
    }

    private static void TestSettingPokemon(
        PKMN.PokemonBox box,
        PKMN.Game[] validOtherGames,
        PKMN.Game invalidOtherGame
    )
    {
        int generation = Util.GameToGeneration(box.Game);

        PKMN.Pokemon originalFirst = box[0];
        PKMN.Pokemon originalSecond = box[1];

        // Make sure we can't set Pokémon at invalid indices.
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                box[-1] = originalFirst;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                box[box.Length] = originalSecond;
            }
        );

        // Create Pokémon and place in box. The original variables should
        // still have the same underlying Pokémon.
        PKMN.Pokemon bulbasaur = new PKMN.Pokemon(PKMN.Species.BULBASAUR, box.Game, "", 5);
        PKMN.Pokemon charmander = new PKMN.Pokemon(PKMN.Species.CHARMANDER, box.Game, "", 5);
        PKMN.Pokemon squirtle = new PKMN.Pokemon(PKMN.Species.SQUIRTLE, box.Game, "", 5);

        box[0] = bulbasaur;
        Assert.AreEqual(box.NumPokemon, 1);
        box[1] = charmander;
        Assert.AreEqual(box.NumPokemon, 2);

        // Replace one of the new ones.
        box[0] = squirtle;
        Assert.AreEqual(box.NumPokemon, 2);

        // Copy a Pokémon already part of the box.
        box[2] = box[1];
        Assert.AreEqual(box.NumPokemon, 3);

        // We should always be able to clear the last contiguous Pokémon.
        box[2] = originalFirst;
        Assert.AreEqual(box.NumPokemon, 2);
        Assert.AreEqual(box[2].Species, PKMN.Species.NONE);

        // Put it back.
        box[2] = box[1];
        Assert.AreEqual(box.NumPokemon, 3);

        // Check that Pokémon can be placed non-contiguously in the correct games.
        if(generation <= 2)
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    box[1] = originalFirst;
                }
            );
            Assert.AreEqual(box.NumPokemon, 3);
            Assert.AreEqual(box[1].Species, PKMN.Species.CHARMANDER);

            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    box[4] = bulbasaur;
                }
            );
            Assert.AreEqual(box.NumPokemon, 3);
            Assert.AreEqual(box[4].Species, PKMN.Species.NONE);
        }
        else
        {
            box[1] = originalFirst;
            Assert.AreEqual(box.NumPokemon, 2);
            Assert.AreEqual(box[1].Species, PKMN.Species.NONE);

            box[4] = bulbasaur;
            Assert.AreEqual(box.NumPokemon, 3);
            Assert.AreEqual(box[4].Species, PKMN.Species.BULBASAUR);

            // Restore it to how it was.
            box[1] = charmander;
            box[4] = originalFirst;
            Assert.AreEqual(box.NumPokemon, 3);
            Assert.AreEqual(box[1].Species, PKMN.Species.CHARMANDER);
            Assert.AreEqual(box[4].Species, PKMN.Species.NONE);
        }

        // Now check everything we've created. Each variable should have
        // the same underlying Pokémon.
        Assert.AreEqual(box[0].Species, PKMN.Species.SQUIRTLE);
        Assert.AreEqual(box[1].Species, PKMN.Species.CHARMANDER);
        Assert.AreEqual(box[2].Species, PKMN.Species.CHARMANDER);
        Assert.AreEqual(originalFirst.Species, PKMN.Species.NONE);
        Assert.AreEqual(originalSecond.Species, PKMN.Species.NONE);
        Assert.AreEqual(bulbasaur.Species, PKMN.Species.BULBASAUR);
        Assert.AreEqual(charmander.Species, PKMN.Species.CHARMANDER);
        Assert.AreEqual(squirtle.Species, PKMN.Species.SQUIRTLE);

        // Make sure converting Pokémon before putting them in the box works (or doesn't)
        // as expected.
        foreach(PKMN.Game validGame in validOtherGames)
        {
            PKMN.Pokemon pikachu = new PKMN.Pokemon(
                                           PKMN.Species.PIKACHU,
                                           validGame,
                                           "",
                                           50
                                       );
            Assert.AreEqual(validGame, pikachu.Game);

            box[3] = pikachu;
            Assert.AreEqual(PKMN.Species.PIKACHU, box[3].Species);
            Assert.AreEqual(box.Game, box[3].Game);
            Assert.AreEqual(50, box[3].Level);
        }

        PKMN.Pokemon invalidPikachu = new PKMN.Pokemon(
                                              PKMN.Species.PIKACHU,
                                              invalidOtherGame,
                                              "",
                                              50
                                          );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                box[3] = invalidPikachu;
            }
        );
    }

    public static void TestPokemonBox(
        PKMN.PokemonBox box,
        PKMN.Game[] validOtherGames,
        PKMN.Game invalidOtherGame
    )
    {
        TestEmptyPokemonBox(box);
        TestBoxName(box);
        TestSettingPokemon(box, validOtherGames, invalidOtherGame);
    }

    private static void TestEmptyPokemonPC(
        PKMN.PokemonPC PC
    )
    {
        for(int i = 0; i < PC.Length; ++i)
        {
            Assert.AreEqual(PC[i].Game, PC.Game);
            TestEmptyPokemonBox(PC[i]);
        }
    }

    private static void TestBoxNames(
        PKMN.PokemonPC PC
    )
    {
        int generation = Util.GameToGeneration(PC.Game);

        if(generation == 1)
        {
            Assert.AreEqual(PC.BoxNames.Count, 0);

            Assert.Throws<ApplicationException>(
                delegate
                {
                    PC[0].Name = "ABCDEFGH";
                }
            );
        }
        else
        {
            for(int i = 0; i < PC.Length; ++i)
            {
                string boxName = String.Format("BOX{0}", i+1);
                PC[i].Name = boxName;
            }

            for(int i = 0; i < PC.BoxNames.Count; ++i)
            {
                string expectedBoxName = String.Format("BOX{0}", i+1);
                Assert.AreEqual(PC.BoxNames[i], expectedBoxName);
            }
        }
    }

    private static void TestSettingPokemonInBoxes(
        PKMN.PokemonPC PC,
        PKMN.Game[] validOtherGames,
        PKMN.Game invalidOtherGame
    )
    {
        for(int i = 0; i < PC.Length; ++i)
        {
            TestSettingPokemon(PC[i], validOtherGames, invalidOtherGame);
            Assert.AreEqual(PC[i][0].Species, PKMN.Species.SQUIRTLE);
            Assert.AreEqual(PC[i][1].Species, PKMN.Species.CHARMANDER);
        }
    }

    public static void TestPokemonPC(
        PKMN.PokemonPC PC,
        PKMN.Game[] validOtherGames,
        PKMN.Game invalidOtherGame
    )
    {
        TestEmptyPokemonPC(PC);
        TestBoxNames(PC);
        TestSettingPokemonInBoxes(PC, validOtherGames, invalidOtherGame);
    }
}

}
