/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class Gen2PokemonTests
{
    public static void PokemonTest(
        string game,
        string species
    )
    {
        PKMN.Pokemon pokemon = new PKMN.Pokemon(species, game, "", 30);

        PokemonTestParams testParams = new PokemonTestParams(
                                               "Great Ball",
                                               new string[]{"Great Ball"},
                                               "Berry",
                                               new string[]{"Razz Berry", "Bicycle"},
                                               "Special",
                                               new string[]{"Sprout Tower", "Tohjo Falls"},
                                               new string[]{"Littleroot Town", "Petalburg Woods"},
                                               new string[]{"Slash", "Flamethrower", "Return", "Fire Blast"},
                                               new string[]{"Frenzy Plant", "Roost"},
                                               new string[]{"Gold"},
                                               new string[]{"Gold"}
                                           );

        PokemonTestCommon.TestCommon(pokemon, testParams);

        // Gender is tied to IVs, so make sure the abstraction reflects that.

        pokemon.Gender = PKMN.Gender.MALE;
        Assert.AreEqual(pokemon.IVs["Attack"], 15);
        pokemon.Gender = PKMN.Gender.FEMALE;
        Assert.Less(pokemon.IVs["Attack"], 15);

        pokemon.IVs["Attack"] = 0;
        Assert.AreEqual(pokemon.Gender, PKMN.Gender.FEMALE);
        pokemon.IVs["Attack"] = 15;
        Assert.AreEqual(pokemon.Gender, PKMN.Gender.MALE);

        // Shininess is tied to IVs, so make sure the abstraction reflects that.

        pokemon.IsShiny = false;
        Assert.AreEqual(pokemon.IVs["Attack"], 13);

        pokemon.IsShiny = true;
        Assert.AreEqual(pokemon.IVs["Attack"], 15);
        Assert.AreEqual(pokemon.IVs["Defense"], 10);
        Assert.AreEqual(pokemon.IVs["Speed"], 10);
        Assert.AreEqual(pokemon.IVs["Special"], 10);
    }
}

}
