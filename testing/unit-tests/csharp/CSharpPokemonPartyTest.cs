/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokemonPartyTest
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
    public void PokemonPartyTest(
        string game
    )
    {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty2(game)
        );
    }
}
