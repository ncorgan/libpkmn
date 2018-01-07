/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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
        PKMN.PokemonBox2 box
    )
    {
        for(int i = 0; i < box.Length; ++i)
        {
            Assert.AreEqual(box[i].Species, "None");
            Assert.AreEqual(box[i].Game, box.Game);

            for(int j = 0; j < box[i].Moves.Count; ++j)
            {
                Assert.AreEqual(box[i].Moves[j].Move, "None");
                Assert.AreEqual(box[i].Moves[j].PP, 0);
            }
        }

        // Make sure trying to get a Pokémon at an invalid index fails.
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Pokemon2 pokemon = box[-1];
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Pokemon2 pokemon = box[box.Length];
            }
        );
    }

    private static void TestBoxName(
        PKMN.PokemonBox2 box
    )
    {
        int generation = Util.GameToGeneration(box.Game);

        if(generation == 1)
        {
            Assert.Throws<ApplicationException>(
                delegate
                {
                    box.Name = "ABCDEFGH";
                }
            );
            Assert.Throws<ApplicationException>(
                delegate
                {
                    string name = box.Name;
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
        PKMN.PokemonBox2 box
    )
    {
        int generation = Util.GameToGeneration(box.Game);

        PKMN.Pokemon2 originalFirst = box[0];
        PKMN.Pokemon2 originalSecond = box[1];

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
        PKMN.Pokemon2 bulbasaur = new PKMN.Pokemon2("Bulbasaur", box.Game, "", 5);
        PKMN.Pokemon2 charmander = new PKMN.Pokemon2("Charmander", box.Game, "", 5);
        PKMN.Pokemon2 squirtle = new PKMN.Pokemon2("Squirtle", box.Game, "", 5);

        box[0] = bulbasaur;
        Assert.AreEqual(box.NumPokemon, 1);
        box[1] = charmander;
        Assert.AreEqual(box.NumPokemon, 2);

        // Replace one of the new ones.
        box[0] = squirtle;
        Assert.AreEqual(box.NumPokemon, 2);

        // Make sure we can't copy a Pokémon to itself.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                box[1] = box[1];
            }
        );

        // Copy a Pokémon whose memory is already part of the box.
        box[2] = box[1];
        Assert.AreEqual(box.NumPokemon, 3);

        // We should always be able to clear the last contiguous Pokémon.
        box[2] = originalFirst;
        Assert.AreEqual(box.NumPokemon, 2);
        Assert.AreEqual(box[2].Species, "None");

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
            Assert.AreEqual(box[1].Species, "Charmander");

            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    box[4] = bulbasaur;
                }
            );
            Assert.AreEqual(box.NumPokemon, 3);
            Assert.AreEqual(box[4].Species, "None");
        }
        else
        {
            box[1] = originalFirst;
            Assert.AreEqual(box.NumPokemon, 2);
            Assert.AreEqual(box[1].Species, "None");

            box[4] = bulbasaur;
            Assert.AreEqual(box.NumPokemon, 3);
            Assert.AreEqual(box[4].Species, "Bulbasaur");

            // Restore it to how it was.
            box[1] = charmander;
            box[4] = originalFirst;
            Assert.AreEqual(box.NumPokemon, 3);
            Assert.AreEqual(box[1].Species, "Charmander");
            Assert.AreEqual(box[4].Species, "None");
        }

        // Now check everything we've created. Each variable should have
        // the same underlying Pokémon.
        Assert.AreEqual(box[0].Species, "Squirtle");
        Assert.AreEqual(box[1].Species, "Charmander");
        Assert.AreEqual(box[2].Species, "Charmander");
        Assert.AreEqual(originalFirst.Species, "None");
        Assert.AreEqual(originalSecond.Species, "None");
        Assert.AreEqual(bulbasaur.Species, "Bulbasaur");
        Assert.AreEqual(charmander.Species, "Charmander");
        Assert.AreEqual(squirtle.Species, "Squirtle");
    }

    public static void TestPokemonBox(
        PKMN.PokemonBox2 box
    )
    {
        TestEmptyPokemonBox(box);
        TestBoxName(box);
        TestSettingPokemon(box);
    }

    private static void TestEmptyPokemonPC(
        PKMN.PokemonPC2 PC
    )
    {
        for(int i = 0; i < PC.Length; ++i)
        {
            Assert.AreEqual(PC[i].Game, PC.Game);
            TestEmptyPokemonBox(PC[i]);
        }
    }

    private static void TestBoxNames(
        PKMN.PokemonPC2 PC
    )
    {
        int generation = Util.GameToGeneration(PC.Game);

        if(generation == 1)
        {
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
        PKMN.PokemonPC2 PC
    )
    {
        for(int i = 0; i < PC.Length; ++i)
        {
            TestSettingPokemon(PC[i]);
            Assert.AreEqual(PC[i][0].Species, "Squirtle");
            Assert.AreEqual(PC[i][1].Species, "Charmander");
        }
    }

    public static void TestPokemonPC(
        PKMN.PokemonPC2 PC
    )
    {
        TestEmptyPokemonPC(PC);
        TestBoxNames(PC);
        TestSettingPokemonInBoxes(PC);
    }
}

}
