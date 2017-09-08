/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class PokemonPartyTests {

    internal static void TestEmptyPokemonParty(
        PKMN.PokemonParty party,
        string game
    ) {
        Assert.AreEqual(party.Game, game);
        Assert.AreEqual(party.Count, 6);

        // Make sure trying to get a Pokémon at an invalid index fails.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Pokemon pokemon = party[-1];
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Pokemon pokemon = party[party.Count];
            }
        );

        for(int i = 0; i < party.Count; ++i) {
            Assert.AreEqual(party[i].Species, "None");
            Assert.AreEqual(party[i].Game, game);

            for(int j = 0; j < party[i].Moves.Count; ++j) {
                Assert.AreEqual(party[i].Moves[j].Move, "None");
                Assert.AreEqual(party[i].Moves[j].PP, 0);
            }
        }
    }

    internal static void TestSettingPokemon(
        PKMN.PokemonParty party
    ) {
        PKMN.Pokemon originalFirst = party[0];
        PKMN.Pokemon originalSecond = party[1];

        // Make sure we can't set Pokémon at invalid indices.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                party[-1] = originalFirst;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                party[party.Count] = originalSecond;
            }
        );

        // Create Pokémon and place in party. The original variables should
        // still have the same underlying Pokémon.
        PKMN.Pokemon bulbasaur = new PKMN.Pokemon("Bulbasaur", party.Game, "", 5);
        PKMN.Pokemon charmander = new PKMN.Pokemon("Charmander", party.Game, "", 5);
        PKMN.Pokemon squirtle = new PKMN.Pokemon("Squirtle", party.Game, "", 5);

        party[0] = bulbasaur;
        Assert.AreEqual(party.NumPokemon, 1);
        Assert.AreEqual(party[0].Species, "Bulbasaur");
        party[1] = charmander;
        Assert.AreEqual(party.NumPokemon, 2);
        Assert.AreEqual(party[1].Species, "Charmander");

        // Replace one of the new ones.
        party[0] = squirtle;
        Assert.AreEqual(party.NumPokemon, 2);
        Assert.AreEqual(party[0].Species, "Squirtle");

        // Make sure we can't copy a Pokémon to itself.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                party[1] = party[1];
            }
        );

        // Copy a Pokémon whose memory is already part of the party.
        party[2] = party[1];
        Assert.AreEqual(party.NumPokemon, 3);
        Assert.AreEqual(party[2].Species, "Charmander");

        // We should be able to clear the last contiguous Pokémon.
        party[2] = originalFirst;
        Assert.AreEqual(party.NumPokemon, 2);
        Assert.AreEqual(party[2].Species, "None");

        // Put it back.
        party[2] = party[1];
        Assert.AreEqual(party.NumPokemon, 3);
        Assert.AreEqual(party[2].Species, "Charmander");

        // Check that Pokémon cannot be placed non-contiguously.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                party[1] = originalFirst;
            }
        );
        Assert.AreEqual(party.NumPokemon, 3);
        Assert.AreEqual(party[1].Species, "Charmander");

        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                party[4] = bulbasaur;
            }
        );
        Assert.AreEqual(party.NumPokemon, 3);
        Assert.AreEqual(party[4].Species, "None");

        // Now check everything we've created. Each variable should have
        // the same underlying Pokémon.
        Assert.AreEqual(party[0].Species, "Squirtle");
        Assert.AreEqual(party[1].Species, "Charmander");
        Assert.AreEqual(party[2].Species, "Charmander");
        Assert.AreEqual(originalFirst.Species, "None");
        Assert.AreEqual(originalSecond.Species, "None");
        Assert.AreEqual(bulbasaur.Species, "Bulbasaur");
        Assert.AreEqual(charmander.Species, "Charmander");
        Assert.AreEqual(squirtle.Species, "Squirtle");
    }

    public static void TestPokemonParty(
        PKMN.PokemonParty party,
        string game
    ) {
        TestEmptyPokemonParty(party, game);
        TestSettingPokemon(party);
    }
}

}
