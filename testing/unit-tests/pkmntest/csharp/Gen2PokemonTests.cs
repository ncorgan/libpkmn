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

public class Gen2PokemonTests
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
                                               PKMN.Item.BERRY,
                                               new PKMN.Item[]{PKMN.Item.RAZZ_BERRY, PKMN.Item.BICYCLE},
                                               "Special",
                                               new string[]{"Sprout Tower", "Tohjo Falls"},
                                               new string[]{"Littleroot Town", "Petalburg Woods"},
                                               new string[]{"Slash", "Flamethrower", "Return", "Fire Blast"},
                                               new string[]{"Frenzy Plant", "Roost"},
                                               new PKMN.Game[]{PKMN.Game.GOLD},
                                               new PKMN.Game[]{PKMN.Game.GOLD}
                                           );

        PokemonTestCommon.TestCommon(pokemon, testParams);

        // Gender is tied to IVs, so make sure the abstraction reflects that.

        pokemon.Gender = PKMN.Gender.MALE;
        Assert.AreEqual(pokemon.IVs[PKMN.Stat.ATTACK], 15);
        pokemon.Gender = PKMN.Gender.FEMALE;
        Assert.Less(pokemon.IVs[PKMN.Stat.ATTACK], 15);

        pokemon.IVs[PKMN.Stat.ATTACK] = 0;
        Assert.AreEqual(pokemon.Gender, PKMN.Gender.FEMALE);
        pokemon.IVs[PKMN.Stat.ATTACK] = 15;
        Assert.AreEqual(pokemon.Gender, PKMN.Gender.MALE);

        // Shininess is tied to IVs, so make sure the abstraction reflects that.

        pokemon.IsShiny = false;
        Assert.AreEqual(pokemon.IVs[PKMN.Stat.ATTACK], 13);

        pokemon.IsShiny = true;
        Assert.AreEqual(pokemon.IVs[PKMN.Stat.ATTACK], 15);
        Assert.AreEqual(pokemon.IVs[PKMN.Stat.DEFENSE], 10);
        Assert.AreEqual(pokemon.IVs[PKMN.Stat.SPEED], 10);
        Assert.AreEqual(pokemon.IVs[PKMN.Stat.SPECIAL], 10);
    }
}

}
