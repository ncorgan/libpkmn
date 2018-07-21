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
            Assert.AreEqual(PKMN.Species.NONE, party[i].Species);
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
        PKMN.Pokemon bulbasaur = new PKMN.Pokemon(PKMN.Species.BULBASAUR, party.Game, "", 5);
        PKMN.Pokemon charmander = new PKMN.Pokemon(PKMN.Species.CHARMANDER, party.Game, "", 5);
        PKMN.Pokemon squirtle = new PKMN.Pokemon(PKMN.Species.SQUIRTLE, party.Game, "", 5);

        party[0] = bulbasaur;
        Assert.AreEqual(1, party.NumPokemon);
        Assert.AreEqual(PKMN.Species.BULBASAUR, party[0].Species);
        party[1] = charmander;
        Assert.AreEqual(2, party.NumPokemon);
        Assert.AreEqual(PKMN.Species.CHARMANDER, party[1].Species);

        // Replace one of the new ones.
        party[0] = squirtle;
        Assert.AreEqual(2, party.NumPokemon);
        Assert.AreEqual(PKMN.Species.SQUIRTLE, party[0].Species);

        // Copy a Pokémon already part of the party.
        party[2] = party[1];
        Assert.AreEqual(3, party.NumPokemon);
        Assert.AreEqual(PKMN.Species.CHARMANDER, party[2].Species);

        // We should be able to clear the last contiguous Pokémon.
        party[2] = originalFirst;
        Assert.AreEqual(2, party.NumPokemon);
        Assert.AreEqual(PKMN.Species.NONE, party[2].Species);

        // Put it back.
        party[2] = party[1];
        Assert.AreEqual(3, party.NumPokemon);
        Assert.AreEqual(PKMN.Species.CHARMANDER, party[2].Species);

        // Check that Pokémon cannot be placed non-contiguously.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                party[1] = originalFirst;
            }
        );
        Assert.AreEqual(3, party.NumPokemon);
        Assert.AreEqual(PKMN.Species.CHARMANDER, party[1].Species);

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                party[4] = bulbasaur;
            }
        );
        Assert.AreEqual(3, party.NumPokemon);
        Assert.AreEqual(PKMN.Species.NONE, party[4].Species);

        // Now check everything we've created. Each variable should have
        // the same underlying Pokémon.
        Assert.AreEqual(PKMN.Species.SQUIRTLE, party[0].Species);
        Assert.AreEqual(PKMN.Species.CHARMANDER, party[1].Species);
        Assert.AreEqual(PKMN.Species.CHARMANDER, party[2].Species);
        Assert.AreEqual(PKMN.Species.NONE, originalFirst.Species);
        Assert.AreEqual(PKMN.Species.NONE, originalSecond.Species);
        Assert.AreEqual(PKMN.Species.BULBASAUR, bulbasaur.Species);
        Assert.AreEqual(PKMN.Species.CHARMANDER, charmander.Species);
        Assert.AreEqual(PKMN.Species.SQUIRTLE, squirtle.Species);

        // Make sure converting Pokémon before putting them in the party works (or doesn't)
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

            party[3] = pikachu;
            Assert.AreEqual(PKMN.Species.PIKACHU, party[3].Species);
            Assert.AreEqual(party.Game, party[3].Game);
            Assert.AreEqual(50, party[3].Level);
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
