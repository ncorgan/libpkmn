/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpItemsTest {
    /*
     * Red
     */
    [Test]
    public void Gen1RedTestInvalidPokemon() {
        PKMNTest.Gen1PokemonTests.TestInvalidPokemon("Red");
    }

    /*
     * Blue
     */
    [Test]
    public void Gen1BlueTestInvalidPokemon() {
        PKMNTest.Gen1PokemonTests.TestInvalidPokemon("Blue");
    }

    /*
     * Yellow
     */
    [Test]
    public void Gen1YellowTestInvalidPokemon() {
        PKMNTest.Gen1PokemonTests.TestInvalidPokemon("Yellow");
    }

    /*
     * Gold
     */
    [Test]
    public void Gen2GoldTestInvalidPokemon() {
        PKMNTest.Gen2PokemonTests.TestInvalidPokemon("Gold");
    }

    /*
     * Silver
     */
    [Test]
    public void Gen2SilverTestInvalidPokemon() {
        PKMNTest.Gen2PokemonTests.TestInvalidPokemon("Silver");
    }

    /*
     * Crystal
     */
    [Test]
    public void Gen2CrystalTestInvalidPokemon() {
        PKMNTest.Gen2PokemonTests.TestInvalidPokemon("Crystal");
    }

    /*
     * Ruby
     */
    [Test]
    public void GBARubyTestInvalidPokemon() {
        PKMNTest.GBAPokemonTests.TestInvalidPokemon("Ruby");
    }

    /*
     * Sapphire
     */
    [Test]
    public void GBASapphireTestInvalidPokemon() {
        PKMNTest.GBAPokemonTests.TestInvalidPokemon("Sapphire");
    }

    /*
     * Emerald
     */
    [Test]
    public void GBAEmeraldTestInvalidPokemon() {
        PKMNTest.GBAPokemonTests.TestInvalidPokemon("Emerald");
    }

    /*
     * FireRed
     */
    [Test]
    public void GBAFireRedTestInvalidPokemon() {
        PKMNTest.GBAPokemonTests.TestInvalidPokemon("FireRed");
    }

    /*
     * LeafGreen
     */
    [Test]
    public void GBALeafGreenTestInvalidPokemon() {
        PKMNTest.GBAPokemonTests.TestInvalidPokemon("LeafGreen");
    }
}
