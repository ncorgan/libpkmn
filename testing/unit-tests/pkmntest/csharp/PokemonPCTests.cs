/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class PokemonPCTests {

    internal static int GameToGeneration(
        string game
    ) {
        if(game.Equals("Red") || game.Equals("Blue") || game.Equals("Yellow")) {
            return 1;
        } else if(game.Equals("Gold") || game.Equals("Silver") || game.Equals("Crystal")) {
            return 2;
        } else if(game.Equals("Ruby") || game.Equals("Sapphire") || game.Equals("Emerald") ||
                  game.Equals("FireRed") || game.Equals("LeafGreen")) {
            return 3;
        } else {
            return 0;
        }
    }

    internal static void TestEmptyPokemonBox(
        PKMN.PokemonBox box,
        string game
    ) {
        int generation = GameToGeneration(game);

        Assert.AreEqual(box.GetGame(), game);

        if(generation == 1) {
            Assert.Throws<ApplicationException>(
                delegate {
                    box.GetName();
                }
            );
        } else {
            Assert.AreEqual(box.GetName(), "");
        }

        // Make sure trying to get a Pokémon at an invalid index fails.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Pokemon pokemon = box[-1];
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Pokemon pokemon = box[box.GetCapacity()];
            }
        );

        for(int i = 0; i < box.GetCapacity(); ++i) {
            Assert.AreEqual(box[i].GetSpecies(), "None");
            Assert.AreEqual(box[i].GetGame(), game);

            PKMN.MoveSlotList moves = box[i].GetMoves();
            for(int j = 0; j < moves.Count; ++j) {
                Assert.AreEqual(moves[j].Move.GetName(), "None");
                Assert.AreEqual(moves[j].PP, 0);
            }
        }
    }

    internal static void TestBoxName(
        PKMN.PokemonBox box
    ) {
        int generation = GameToGeneration(box.GetGame());

        if(generation == 1) {
            Assert.Throws<ApplicationException>(
                delegate {
                    box.SetName("ABCDEFGH");
                }
            );
            Assert.Throws<ApplicationException>(
                delegate {
                    box.GetName();
                }
            );
        } else {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    box.SetName("ABCDEFGHI");
                }
            );

            box.SetName("ABCDEFGH");
            Assert.AreEqual(box.GetName(), "ABCDEFGH");
        }
    }

    internal static void TestSettingPokemon(
        PKMN.PokemonBox box
    ) {
        string game = box.GetGame();

        PKMN.Pokemon originalFirst = box[0];
        PKMN.Pokemon originalSecond = box[1];

        // Make sure we can't set Pokémon at invalid indices.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                box[-1] = originalFirst;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                box[box.GetCapacity()] = originalSecond;
            }
        );

        // Create Pokémon and place in box. The original variables should
        // still have the same underlying Pokémon.
        PKMN.Pokemon bulbasaur = new PKMN.Pokemon("Bulbasaur", game, "", 5);
        PKMN.Pokemon charmander = new PKMN.Pokemon("Charmander", game, "", 5);
        PKMN.Pokemon squirtle = new PKMN.Pokemon("Squirtle", game, "", 5);

        box[0] = bulbasaur;
        box[1] = charmander;

        // Replace one of the new ones.
        box[0] = squirtle;

        // Make sure we can't copy a Pokémon to itself.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                box[1] = box[1];
            }
        );

        // Copy a Pokémon whose memory is already part of the box.
        box[2] = box[1];

        // Now check everything we've created. Each variable should have
        // the same underlying Pokémon.
        Assert.AreEqual(box[0].GetSpecies(), "Squirtle");
        Assert.AreEqual(box[1].GetSpecies(), "Charmander");
        Assert.AreEqual(box[2].GetSpecies(), "Charmander");
        Assert.AreEqual(originalFirst.GetSpecies(), "None");
        Assert.AreEqual(originalSecond.GetSpecies(), "None");
        Assert.AreEqual(bulbasaur.GetSpecies(), "Bulbasaur");
        Assert.AreEqual(charmander.GetSpecies(), "Charmander");
        Assert.AreEqual(squirtle.GetSpecies(), "Squirtle");
    }

    public static void TestPokemonBox(
        PKMN.PokemonBox box,
        string game
    ) {
        TestEmptyPokemonBox(box, game);
        TestBoxName(box);
        TestSettingPokemon(box);
    }

    internal static void TestEmptyPokemonPC(
        PKMN.PokemonPC PC,
        string game
    ) {
        Assert.AreEqual(PC.GetGame(), game);

        for(int i = 0; i < PC.GetNumBoxes(); ++i) {
            TestEmptyPokemonBox(PC[i], game);
        }
    }

    internal static void TestBoxNames(
        PKMN.PokemonPC PC
    ) {
        int generation = GameToGeneration(PC.GetGame());

        if(generation == 1) {
            Assert.Throws<ApplicationException>(
                delegate {
                    PC[0].SetName("ABCDEFGH");
                }
            );
        } else {
            for(int i = 0; i < PC.GetNumBoxes(); ++i) {
                string boxName = String.Format("BOX{0}", i+1);
                PC[i].SetName(boxName);
            }

            PKMN.StringList boxNames = PC.GetBoxNames();
            for(int i = 0; i < boxNames.Count; ++i) {
                string expectedBoxName = String.Format("BOX{0}", i+1);
                Assert.AreEqual(boxNames[i], expectedBoxName);
            }
        }
    }

    internal static void TestSettingPokemonInBoxes(
        PKMN.PokemonPC PC
    ) {
        for(int i = 0; i < PC.GetNumBoxes(); ++i) {
            TestSettingPokemon(PC[i]);
            Assert.AreEqual(PC[i][0].GetSpecies(), "Squirtle");
            Assert.AreEqual(PC[i][1].GetSpecies(), "Charmander");
        }
    }

    public static void TestPokemonPC(
        PKMN.PokemonPC PC,
        string game
    ) {
        TestEmptyPokemonPC(PC, game);
        TestBoxNames(PC);
        TestSettingPokemonInBoxes(PC);
    }
}

}
