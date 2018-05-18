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
    [TestCase("Red")]
    [TestCase("Blue")]
    [TestCase("Yellow")]
    [TestCase("Gold")]
    [TestCase("Silver")]
    [TestCase("Crystal")]
    [TestCase("Ruby")]
    [TestCase("Sapphire")]
    [TestCase("Emerald")]
    [TestCase("FireRed")]
    [TestCase("LeafGreen")]
    public void PokedexTest(
        string game
    )
    {
        PKMNTest.PokedexTests.TestPokedex(
            new PKMN.Pokedex(game)
        );
    }
}
