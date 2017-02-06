/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokemonTest {
    /*
     * Red
     */
    [Test]
    public void Gen1RedInvalidPokemonTest() {
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Red");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Blue");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Yellow");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Gold");
    }

    [Test]
    public void Gen2GoldGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Gold");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Silver");
    }

    [Test]
    public void Gen2SilverGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Silver");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Crystal");
    }

    [Test]
    public void Gen2CrystalGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Crystal");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Ruby");
    }

    [Test]
    public void GBARubyGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Ruby");
    }

    [Test]
    public void GBARubyUnownFormTest() {
        PKMNTest.GBAPokemonTests.UnownFormTest("Ruby");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Sapphire");
    }

    [Test]
    public void GBASapphireGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Sapphire");
    }

    [Test]
    public void GBASapphireUnownFormTest() {
        PKMNTest.GBAPokemonTests.UnownFormTest("Sapphire");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Emerald");
    }

    [Test]
    public void GBAEmeraldGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Emerald");
    }

    [Test]
    public void GBAEmeraldUnownFormTest() {
        PKMNTest.GBAPokemonTests.UnownFormTest("Emerald");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("FireRed");
    }

    [Test]
    public void GBAFireRedGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("FireRed");
    }

    [Test]
    public void GBAFireRedUnownFormTest() {
        PKMNTest.GBAPokemonTests.UnownFormTest("FireRed");
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
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("LeafGreen");
    }

    [Test]
    public void GBALeafGreenGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("LeafGreen");
    }

    [Test]
    public void GBALeafGreenUnownFormTest() {
        PKMNTest.GBAPokemonTests.UnownFormTest("LeafGreen");
    }

    [Test]
    public void GBALeafGreenPokemonTest() {
        PKMNTest.GBAPokemonTests.PokemonTest("LeafGreen");
    }

    /*
     * Other
     */
    [Test]
    public void _3GPKMTest() {
        PKMNTest.PokemonIOTests.Test3GPKM();
    }
}
