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

public class Gen1PokemonTests
{
    public static void PokemonTest(
        PKMN.Game game,
        PKMN.Species species
    )
    {
        PKMN.Pokemon pokemon = new PKMN.Pokemon(species, game, "", 30);

        PokemonTestParams testParams = new PokemonTestParams(
                                               PKMN.Ball.GREAT_BALL,
                                               new PKMN.Ball[]{PKMN.Ball.GREAT_BALL},
                                               PKMN.Item.POTION,
                                               new PKMN.Item[]{PKMN.Item.POTION},
                                               "Special",
                                               new string[]{"Route 1"},
                                               new string[]{"Route 1"},
                                               new string[]{"Slash", "Flamethrower", "Tail Whip", "Fire Blast"},
                                               new string[]{"Return"},
                                               new PKMN.Game[]{PKMN.Game.RED},
                                               new PKMN.Game[]{PKMN.Game.RED}
                                           );

        PokemonTestCommon.TestCommon(pokemon, testParams);

        // Test attributes.

        Assert.AreEqual(pokemon.NumericAttributes["Catch rate"], 45);
    }
}

}
