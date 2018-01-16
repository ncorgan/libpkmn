/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class PokedexTests
{
    private static System.Random rng = new System.Random();

    public static void TestPokedex(
        PKMN.Pokedex pokedex
    )
    {
        int generation = Util.GameToGeneration(pokedex.Game);

        // Test initial values.

        Assert.AreEqual(pokedex.NumSeen, 0);
        Assert.AreEqual(pokedex.AllSeen.Count, 0);
        Assert.AreEqual(pokedex.NumCaught, 0);
        Assert.AreEqual(pokedex.AllCaught.Count, 0);

        // Set random Pokémon to be seen and caught.

        PKMN.StringList allPokemon = PKMN.Database.Lists.PokemonList(generation, true);
        int numSeen = rng.Next(0, allPokemon.Count);
        int numCaught = rng.Next(0, allPokemon.Count);

        PKMN.IntList allSeenNums = new PKMN.IntList();
        while(allSeenNums.Count < numSeen)
        {
            int index = rng.Next(0, allPokemon.Count);
            if(allSeenNums.IndexOf(index) == -1)
            {
                allSeenNums.Add(index);

                string species = allPokemon[index];
                pokedex.HasSeen[species] = true;
                Assert.IsTrue(pokedex.HasSeen[species]);
            }
        }
        Assert.AreEqual(pokedex.NumSeen, allSeenNums.Count);
        Assert.AreEqual(pokedex.AllSeen.Count, allSeenNums.Count);

        PKMN.IntList allCaughtNums = new PKMN.IntList();
        while(allCaughtNums.Count < numCaught)
        {
            int index = rng.Next(0, allPokemon.Count);
            if(allCaughtNums.IndexOf(index) == -1)
            {
                allCaughtNums.Add(index);

                string species = allPokemon[index];
                pokedex.HasCaught[species] = true;
                Assert.IsTrue(pokedex.HasCaught[species]);
            }
        }
        Assert.AreEqual(pokedex.NumCaught, allCaughtNums.Count);
        Assert.AreEqual(pokedex.AllCaught.Count, allCaughtNums.Count);

        // Remove all entries.

        foreach(int pokemonNum in allSeenNums)
        {
            string species = allPokemon[pokemonNum];
            pokedex.HasSeen[species] = false;
            Assert.IsFalse(pokedex.HasSeen[species]);
        }
        Assert.AreEqual(pokedex.NumSeen, 0);
        Assert.AreEqual(pokedex.AllSeen.Count, 0);

        foreach(int pokemonNum in allCaughtNums)
        {
            string species = allPokemon[pokemonNum];
            pokedex.HasCaught[species] = false;
            Assert.IsFalse(pokedex.HasCaught[species]);
        }
        Assert.AreEqual(pokedex.NumCaught, 0);
        Assert.AreEqual(pokedex.AllCaught.Count, 0);
    }
}

}
