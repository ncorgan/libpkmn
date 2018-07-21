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

        Assert.AreEqual(0, pokedex.NumSeen);
        Assert.AreEqual(0, pokedex.AllSeen.Count);
        Assert.AreEqual(0, pokedex.NumCaught);
        Assert.AreEqual(0, pokedex.AllCaught.Count);

        // Set random Pokémon to be seen and caught.

        PKMN.SpeciesEnumList allPokemon = PKMN.Database.Lists.PokemonList(generation, true);
        int numSeen = rng.Next(0, allPokemon.Count);
        int numCaught = rng.Next(0, allPokemon.Count);

        PKMN.IntList allSeenNums = new PKMN.IntList();
        while(allSeenNums.Count < numSeen)
        {
            int index = rng.Next(0, allPokemon.Count);
            if(allSeenNums.IndexOf(index) == -1)
            {
                allSeenNums.Add(index);

                PKMN.Species species = allPokemon[index];
                pokedex.SeenPokemonMap[species] = true;
                Assert.IsTrue(pokedex.SeenPokemonMap[species]);
            }
        }
        Assert.AreEqual(allSeenNums.Count, pokedex.NumSeen);
        Assert.AreEqual(allSeenNums.Count, pokedex.AllSeen.Count);

        PKMN.IntList allCaughtNums = new PKMN.IntList();
        while(allCaughtNums.Count < numCaught)
        {
            int index = rng.Next(0, allPokemon.Count);
            if(allCaughtNums.IndexOf(index) == -1)
            {
                allCaughtNums.Add(index);

                PKMN.Species species = allPokemon[index];
                pokedex.CaughtPokemonMap[species] = true;
                Assert.IsTrue(pokedex.CaughtPokemonMap[species]);
            }
        }
        Assert.AreEqual(allCaughtNums.Count, pokedex.NumCaught);
        Assert.AreEqual(allCaughtNums.Count, pokedex.AllCaught.Count);

        // Remove all entries.

        foreach(PKMN.Species species in pokedex.AllSeen)
        {
            pokedex.SeenPokemonMap[species] = false;
            Assert.IsFalse(pokedex.SeenPokemonMap[species]);
        }
        Assert.AreEqual(0, pokedex.NumSeen);
        Assert.AreEqual(0, pokedex.AllSeen.Count);

        foreach(PKMN.Species species in pokedex.AllCaught)
        {
            pokedex.CaughtPokemonMap[species] = false;
            Assert.IsFalse(pokedex.CaughtPokemonMap[species]);
        }
        Assert.AreEqual(0, pokedex.NumCaught);
        Assert.AreEqual(0, pokedex.AllCaught.Count);
    }
}

}
