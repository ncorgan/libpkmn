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
    public void Gen3RubyInvalidPokemonTest() {
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Ruby");
    }

    [Test]
    public void Gen3RubyGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Ruby");
    }

    [Test]
    public void Gen3RubyUnownFormTest() {
        PKMNTest.Gen3PokemonTests.UnownFormTest("Ruby");
    }

    [Test]
    public void Gen3RubyPokemonTest() {
        PKMNTest.Gen3PokemonTests.PokemonTest("Ruby");
    }

    /*
     * Sapphire
     */
    [Test]
    public void Gen3SapphireInvalidPokemonTest() {
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Sapphire");
    }

    [Test]
    public void Gen3SapphireGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Sapphire");
    }

    [Test]
    public void Gen3SapphireUnownFormTest() {
        PKMNTest.Gen3PokemonTests.UnownFormTest("Sapphire");
    }

    [Test]
    public void Gen3SapphirePokemonTest() {
        PKMNTest.Gen3PokemonTests.PokemonTest("Sapphire");
    }

    /*
     * Emerald
     */
    [Test]
    public void Gen3EmeraldInvalidPokemonTest() {
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Emerald");
    }

    [Test]
    public void Gen3EmeraldGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Emerald");
    }

    [Test]
    public void Gen3EmeraldUnownFormTest() {
        PKMNTest.Gen3PokemonTests.UnownFormTest("Emerald");
    }

    [Test]
    public void Gen3EmeraldPokemonTest() {
        PKMNTest.Gen3PokemonTests.PokemonTest("Emerald");
    }

    /*
     * FireRed
     */
    [Test]
    public void Gen3FireRedInvalidPokemonTest() {
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("FireRed");
    }

    [Test]
    public void Gen3FireRedGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("FireRed");
    }

    [Test]
    public void Gen3FireRedUnownFormTest() {
        PKMNTest.Gen3PokemonTests.UnownFormTest("FireRed");
    }

    [Test]
    public void Gen3FireRedPokemonTest() {
        PKMNTest.Gen3PokemonTests.PokemonTest("FireRed");
    }

    /*
     * LeafGreen
     */
    [Test]
    public void Gen3LeafGreenInvalidPokemonTest() {
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("LeafGreen");
    }

    [Test]
    public void Gen3LeafGreenGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("LeafGreen");
    }

    [Test]
    public void Gen3LeafGreenUnownFormTest() {
        PKMNTest.Gen3PokemonTests.UnownFormTest("LeafGreen");
    }

    [Test]
    public void Gen3LeafGreenPokemonTest() {
        PKMNTest.Gen3PokemonTests.PokemonTest("LeafGreen");
    }

    /*
     * Colosseum
     */
    [Test]
    public void Gen3ColosseumInvalidPokemonTest() {
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("Colosseum");
    }

    [Test]
    public void Gen3ColosseumGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("Colosseum");
    }

    [Test]
    public void Gen3ColosseumUnownFormTest() {
        PKMNTest.Gen3PokemonTests.UnownFormTest("Colosseum");
    }

    /*[Test]
    public void Gen3ColosseumPokemonTest() {
        PKMNTest.Gen3PokemonTests.PokemonTest("Colosseum");
    }*/

    /*
     * XD
     */
    [Test]
    public void Gen3XDInvalidPokemonTest() {
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest("XD");
    }

    [Test]
    public void Gen3XDGenderTest() {
        PKMNTest.PokemonTestsCommon.GenderTest("XD");
    }

    [Test]
    public void Gen3XDUnownFormTest() {
        PKMNTest.Gen3PokemonTests.UnownFormTest("XD");
    }

    /*[Test]
    public void Gen3XDPokemonTest() {
        PKMNTest.Gen3PokemonTests.PokemonTest("XD");
    }*/

    /*
     * I/O
     */
    [Test]
    public void _3GPKMTest() {
        PKMNTest.PokemonIOTests.Test3GPKM();
    }
}
