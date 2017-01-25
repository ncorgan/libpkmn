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
    public void Gen1RedInvalidPokemonTest() {
        PKMNTest.Gen1PokemonTests.InvalidPokemonTest("Red");
    }

    [Test]
    public void Gen1RedFriendshipTest() {
        PKMNTest.Gen1PokemonTests.FriendshipTest("Red");
    }

    [Test]
    public void Gen1RedPokemonTest() {
        PKMNTest.Gen1PokemonTests.PokemonTest("Red");
    }

    /*
     * Blue
     */
    [Test]
    public void Gen1BlueInvalidPokemonTest() {
        PKMNTest.Gen1PokemonTests.InvalidPokemonTest("Blue");
    }

    [Test]
    public void Gen1BlueFriendshipTest() {
        PKMNTest.Gen1PokemonTests.FriendshipTest("Blue");
    }

    [Test]
    public void Gen1BluePokemonTest() {
        PKMNTest.Gen1PokemonTests.PokemonTest("Blue");
    }

    /*
     * Yellow
     */
    [Test]
    public void Gen1YellowInvalidPokemonTest() {
        PKMNTest.Gen1PokemonTests.InvalidPokemonTest("Yellow");
    }

    [Test]
    public void Gen1YellowFriendshipTest() {
        PKMNTest.Gen1PokemonTests.FriendshipTest("Yellow");
    }

    [Test]
    public void Gen1YellowPokemonTest() {
        PKMNTest.Gen1PokemonTests.PokemonTest("Yellow");
    }

    /*
     * Gold
     */
    [Test]
    public void Gen2GoldInvalidPokemonTest() {
        PKMNTest.Gen2PokemonTests.InvalidPokemonTest("Gold");
    }

    [Test]
    public void Gen2GoldUnownFormTest() {
        PKMNTest.Gen2PokemonTests.UnownFormTest("Gold");
    }

    [Test]
    public void Gen2GoldPokemonTest() {
        PKMNTest.Gen2PokemonTests.PokemonTest("Gold");
    }

    /*
     * Silver
     */
    [Test]
    public void Gen2SilverInvalidPokemonTest() {
        PKMNTest.Gen2PokemonTests.InvalidPokemonTest("Silver");
    }

    [Test]
    public void Gen2SilverUnownFormTest() {
        PKMNTest.Gen2PokemonTests.UnownFormTest("Silver");
    }

    [Test]
    public void Gen2SilverPokemonTest() {
        PKMNTest.Gen2PokemonTests.PokemonTest("Silver");
    }

    /*
     * Crystal
     */
    [Test]
    public void Gen2CrystalInvalidPokemonTest() {
        PKMNTest.Gen2PokemonTests.InvalidPokemonTest("Crystal");
    }

    [Test]
    public void Gen2CrystalUnownFormTest() {
        PKMNTest.Gen2PokemonTests.UnownFormTest("Crystal");
    }

    [Test]
    public void Gen2CrystalPokemonTest() {
        PKMNTest.Gen2PokemonTests.PokemonTest("Crystal");
    }

    /*
     * Ruby
     */
    [Test]
    public void GBARubyInvalidPokemonTest() {
        PKMNTest.GBAPokemonTests.InvalidPokemonTest("Ruby");
    }

    [Test]
    public void GBARubyPokemonTest() {
        PKMNTest.GBAPokemonTests.PokemonTest("Ruby");
    }

    /*
     * Sapphire
     */
    [Test]
    public void GBASapphireInvalidPokemonTest() {
        PKMNTest.GBAPokemonTests.InvalidPokemonTest("Sapphire");
    }

    [Test]
    public void GBASapphirePokemonTest() {
        PKMNTest.GBAPokemonTests.PokemonTest("Sapphire");
    }

    /*
     * Emerald
     */
    [Test]
    public void GBAEmeraldInvalidPokemonTest() {
        PKMNTest.GBAPokemonTests.InvalidPokemonTest("Emerald");
    }

    [Test]
    public void GBAEmeraldPokemonTest() {
        PKMNTest.GBAPokemonTests.PokemonTest("Emerald");
    }

    /*
     * FireRed
     */
    [Test]
    public void GBAFireRedInvalidPokemonTest() {
        PKMNTest.GBAPokemonTests.InvalidPokemonTest("FireRed");
    }

    [Test]
    public void GBAFireRedPokemonTest() {
        PKMNTest.GBAPokemonTests.PokemonTest("FireRed");
    }

    /*
     * LeafGreen
     */
    [Test]
    public void GBALeafGreenInvalidPokemonTest() {
        PKMNTest.GBAPokemonTests.InvalidPokemonTest("LeafGreen");
    }

    [Test]
    public void GBALeafGreenPokemonTest() {
        PKMNTest.GBAPokemonTests.PokemonTest("LeafGreen");
    }
}
