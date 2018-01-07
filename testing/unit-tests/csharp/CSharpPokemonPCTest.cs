/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokemonPCTest
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
    [TestCase("Colosseum")]
    [TestCase("XD")]
    public void PokemonBoxTest(
        string game
    )
    {
        PKMNTest.PokemonPCTests.TestPokemonBox(
            new PKMN.PokemonBox(game)
        );
    }

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
    [TestCase("Colosseum")]
    [TestCase("XD")]
    public void PokemonPCTest(
        string game
    )
    {
        PKMNTest.PokemonPCTests.TestPokemonPC(
            new PKMN.PokemonPC(game)
        );
    }
}
