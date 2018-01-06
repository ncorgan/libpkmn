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

public class Gen1PokemonTests
{
    public static void PokemonTest(
        string game,
        string species
    )
    {
        PKMN.Pokemon2 pokemon = new PKMN.Pokemon2(species, game, "", 30);

        PokemonTestParams testParams = new PokemonTestParams(
                                               "Great Ball",
                                               new string[]{"Great Ball"},
                                               "Potion",
                                               new string[]{"Potion"},
                                               "Special",
                                               new string[]{"Route 1"},
                                               new string[]{"Route 1"},
                                               new string[]{"Slash", "Flamethrower", "Tail Whip", "Fire Blast"},
                                               new string[]{"Return"},
                                               new string[]{"Red"},
                                               new string[]{"Red"}
                                           );

        PokemonTestCommon.TestCommon(pokemon, testParams);

        // Test attributes.

        Assert.AreEqual(pokemon.NumericAttributes["Catch rate"], 45);
    }
}

}
