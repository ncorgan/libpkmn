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
                  game.Equals("FireRed") || game.Equals("LeafGreen") ||
                  game.Equals("Colosseum") || game.Equals("XD")) {
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

        Assert.AreEqual(box.Game, game);

        if(generation == 1) {
            Assert.Throws<ApplicationException>(
                delegate {
                    string name = box.Name;
                }
            );
        } else {
            Assert.AreEqual(box.Name, "");
        }

        // Make sure trying to get a Pokémon at an invalid index fails.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Pokemon pokemon = box[-1];
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Pokemon pokemon = box[box.Count];
            }
        );

        for(int i = 0; i < box.Count; ++i) {
            Assert.AreEqual(box[i].Species, "None");
            Assert.AreEqual(box[i].Game, game);

            for(int j = 0; j < box[i].Moves.Count; ++j) {
                Assert.AreEqual(box[i].Moves[j].Move.Name, "None");
                Assert.AreEqual(box[i].Moves[j].PP, 0);
            }
        }
    }

    internal static void TestBoxName(
        PKMN.PokemonBox box
    ) {
        int generation = GameToGeneration(box.Game);

        if(generation == 1) {
            Assert.Throws<ApplicationException>(
                delegate {
                    box.Name = "ABCDEFGH";
                }
            );
            Assert.Throws<ApplicationException>(
                delegate {
                    string name = box.Name;
                }
            );
        } else {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    box.Name = "ABCDEFGHI";
                }
            );

            box.Name = "ABCDEFGH";
            Assert.AreEqual(box.Name, "ABCDEFGH");
        }
    }

    internal static void TestSettingPokemon(
        PKMN.PokemonBox box
    ) {
        int generation = GameToGeneration(box.Game);

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
                box[box.Count] = originalSecond;
            }
        );

        // Create Pokémon and place in box. The original variables should
        // still have the same underlying Pokémon.
        PKMN.Pokemon bulbasaur = new PKMN.Pokemon("Bulbasaur", box.Game, "", 5);
        PKMN.Pokemon charmander = new PKMN.Pokemon("Charmander", box.Game, "", 5);
        PKMN.Pokemon squirtle = new PKMN.Pokemon("Squirtle", box.Game, "", 5);

        box[0] = bulbasaur;
        Assert.AreEqual(box.NumPokemon, 1);
        box[1] = charmander;
        Assert.AreEqual(box.NumPokemon, 2);

        // Replace one of the new ones.
        box[0] = squirtle;
        Assert.AreEqual(box.NumPokemon, 2);

        // Make sure we can't copy a Pokémon to itself.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
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
        if(generation <= 2) {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    box[1] = originalFirst;
                }
            );
            Assert.AreEqual(box.NumPokemon, 3);
            Assert.AreEqual(box[1].Species, "Charmander");

            Assert.Throws<IndexOutOfRangeException>(
                delegate {
                    box[4] = bulbasaur;
                }
            );
            Assert.AreEqual(box.NumPokemon, 3);
            Assert.AreEqual(box[4].Species, "None");
        } else {
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
        Assert.AreEqual(PC.Game, game);

        for(int i = 0; i < PC.Count; ++i) {
            TestEmptyPokemonBox(PC[i], game);
        }
    }

    internal static void TestBoxNames(
        PKMN.PokemonPC PC
    ) {
        int generation = GameToGeneration(PC.Game);

        if(generation == 1) {
            Assert.Throws<ApplicationException>(
                delegate {
                    PC[0].Name = "ABCDEFGH";
                }
            );
        } else {
            for(int i = 0; i < PC.Count; ++i) {
                string boxName = String.Format("BOX{0}", i+1);
                PC[i].Name = boxName;
            }

            for(int i = 0; i < PC.BoxNames.Count; ++i) {
                string expectedBoxName = String.Format("BOX{0}", i+1);
                Assert.AreEqual(PC.BoxNames[i], expectedBoxName);
            }
        }
    }

    internal static void TestSettingPokemonInBoxes(
        PKMN.PokemonPC PC
    ) {
        for(int i = 0; i < PC.Count; ++i) {
            TestSettingPokemon(PC[i]);
            Assert.AreEqual(PC[i][0].Species, "Squirtle");
            Assert.AreEqual(PC[i][1].Species, "Charmander");
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
