/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class PokemonPartyTests
{
    private static void TestEmptyPokemonParty(
        PKMN.PokemonParty party
    )
    {
        Assert.AreEqual(6, party.Length);

        for(int i = 0; i < party.Length; ++i)
        {
            Assert.AreEqual("None", party[i].Species);
            Assert.AreEqual(party.Game, party[i].Game);

            for(int j = 0; j < party[i].Moves.Count; ++j)
            {
                Assert.AreEqual("None", party[i].Moves[j].Move);
                Assert.AreEqual(0, party[i].Moves[j].PP);
            }
        }

        // Make sure trying to get a Pokémon at an invalid index fails.
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Pokemon pokemon = party[-1];
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Pokemon pokemon = party[party.Length];
            }
        );
    }

    private static void TestSettingPokemon(
        PKMN.PokemonParty party,
        PKMN.Game[] validOtherGames,
        PKMN.Game invalidOtherGame
    )
    {
        PKMN.Pokemon originalFirst = party[0];
        PKMN.Pokemon originalSecond = party[1];

        // Make sure we can't set Pokémon at invalid indices.
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                party[-1] = originalFirst;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                party[party.Length] = originalSecond;
            }
        );

        // Create Pokémon and place in party. The original variables should
        // still have the same underlying Pokémon.
        PKMN.Pokemon bulbasaur = new PKMN.Pokemon("Bulbasaur", party.Game, "", 5);
        PKMN.Pokemon charmander = new PKMN.Pokemon("Charmander", party.Game, "", 5);
        PKMN.Pokemon squirtle = new PKMN.Pokemon("Squirtle", party.Game, "", 5);

        party[0] = bulbasaur;
        Assert.AreEqual(1, party.NumPokemon);
        Assert.AreEqual("Bulbasaur", party[0].Species);
        party[1] = charmander;
        Assert.AreEqual(2, party.NumPokemon);
        Assert.AreEqual("Charmander", party[1].Species);

        // Replace one of the new ones.
        party[0] = squirtle;
        Assert.AreEqual(2, party.NumPokemon);
        Assert.AreEqual("Squirtle", party[0].Species);

        // Make sure we can't copy a Pokémon to itself.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                party[1] = party[1];
            }
        );

        // Copy a Pokémon whose memory is already part of the party.
        party[2] = party[1];
        Assert.AreEqual(3, party.NumPokemon);
        Assert.AreEqual("Charmander", party[2].Species);

        // We should be able to clear the last contiguous Pokémon.
        party[2] = originalFirst;
        Assert.AreEqual(2, party.NumPokemon);
        Assert.AreEqual("None", party[2].Species);

        // Put it back.
        party[2] = party[1];
        Assert.AreEqual(3, party.NumPokemon);
        Assert.AreEqual("Charmander", party[2].Species);

        // Check that Pokémon cannot be placed non-contiguously.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                party[1] = originalFirst;
            }
        );
        Assert.AreEqual(3, party.NumPokemon);
        Assert.AreEqual("Charmander", party[1].Species);

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                party[4] = bulbasaur;
            }
        );
        Assert.AreEqual(3, party.NumPokemon);
        Assert.AreEqual("None", party[4].Species);

        // Now check everything we've created. Each variable should have
        // the same underlying Pokémon.
        Assert.AreEqual("Squirtle", party[0].Species);
        Assert.AreEqual("Charmander", party[1].Species);
        Assert.AreEqual("Charmander", party[2].Species);
        Assert.AreEqual("None", originalFirst.Species);
        Assert.AreEqual("None", originalSecond.Species);
        Assert.AreEqual("Bulbasaur", bulbasaur.Species);
        Assert.AreEqual("Charmander", charmander.Species);
        Assert.AreEqual("Squirtle", squirtle.Species);

        // Make sure converting Pokémon before putting them in the party works (or doesn't)
        // as expected.
        foreach(PKMN.Game validGame in validOtherGames)
        {
            PKMN.Pokemon pikachu = new PKMN.Pokemon("Pikachu", validGame, "", 50);
            Assert.AreEqual(validGame, pikachu.Game);

            party[3] = pikachu;
            Assert.AreEqual("Pikachu", party[3].Species);
            Assert.AreEqual(party.Game, party[3].Game);
            Assert.AreEqual(50, party[3].Level);
        }

        PKMN.Pokemon invalidPikachu = new PKMN.Pokemon("Pikachu", invalidOtherGame, "", 50);
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                party[3] = invalidPikachu;
            }
        );
    }

    public static void TestPokemonParty(
        PKMN.PokemonParty party,
        PKMN.Game[] validOtherGames,
        PKMN.Game invalidOtherGame
    )
    {
        TestEmptyPokemonParty(party);
        TestSettingPokemon(party, validOtherGames, invalidOtherGame);
    }
}

}
