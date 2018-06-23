/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokedexTest
{
    [TestCase(PKMN.Game.RED)]
    [TestCase(PKMN.Game.BLUE)]
    [TestCase(PKMN.Game.YELLOW)]
    [TestCase(PKMN.Game.GOLD)]
    [TestCase(PKMN.Game.SILVER)]
    [TestCase(PKMN.Game.CRYSTAL)]
    [TestCase(PKMN.Game.RUBY)]
    [TestCase(PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Game.EMERALD)]
    [TestCase(PKMN.Game.FIRERED)]
    [TestCase(PKMN.Game.LEAFGREEN)]
    public void PokedexTest(
        PKMN.Game game
    )
    {
        PKMNTest.PokedexTests.TestPokedex(
            new PKMN.Pokedex(game)
        );
    }
}
