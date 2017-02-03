/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokemonPartyTest {
    /*
     * Red
     */
    [Test]
    public void RedPokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("Red"),
            "Red"
        );
    }

    /*
     * Blue
     */
    [Test]
    public void BluePokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("Blue"),
            "Blue"
        );
    }

    /*
     * Yellow
     */
    [Test]
    public void YellowPokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("Yellow"),
            "Yellow"
        );
    }

    /*
     * Gold
     */
    [Test]
    public void GoldPokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("Gold"),
            "Gold"
        );
    }

    /*
     * Silver
     */
    [Test]
    public void SilverPokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("Silver"),
            "Silver"
        );
    }

    /*
     * Crystal
     */
    [Test]
    public void CrystalPokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("Crystal"),
            "Crystal"
        );
    }

    /*
     * Ruby
     */
    [Test]
    public void RubyPokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("Ruby"),
            "Ruby"
        );
    }

    /*
     * Sapphire
     */
    [Test]
    public void SapphirePokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("Sapphire"),
            "Sapphire"
        );
    }

    /*
     * Emerald
     */
    [Test]
    public void EmeraldPokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("Emerald"),
            "Emerald"
        );
    }

    /*
     * FireRed
     */
    [Test]
    public void FireRedPokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("FireRed"),
            "FireRed"
        );
    }

    /*
     * LeafGreen
     */
    [Test]
    public void LeafGreenPokemonPartyTest() {
        PKMNTest.PokemonPartyTests.TestPokemonParty(
            new PKMN.PokemonParty("LeafGreen"),
            "LeafGreen"
        );
    }
}
