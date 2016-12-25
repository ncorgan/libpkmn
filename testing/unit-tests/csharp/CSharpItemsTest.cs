/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
    public void Gen1RedItemListTest() {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList("Items", "Red"),
            "Red"
        );
    }
    [Test]
    public void Gen1RedItemPCTest() {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Red"),
            "Red"
        );
    }
    [Test]
    public void Gen1RedItemBagTest() {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Red"),
            "Red"
        );
    }

    /*
     * Blue
     */
    [Test]
    public void Gen1BlueItemListTest() {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList("Items", "Blue"),
            "Blue"
        );
    }
    [Test]
    public void Gen1BlueItemPCTest() {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Blue"),
            "Blue"
        );
    }
    [Test]
    public void Gen1BlueItemBagTest() {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Blue"),
            "Blue"
        );
    }

    /*
     * Yellow
     */
    [Test]
    public void Gen1YellowItemListTest() {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList("Items", "Yellow"),
            "Yellow"
        );
    }
    [Test]
    public void Gen1YellowItemPCTest() {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Yellow"),
            "Yellow"
        );
    }
    [Test]
    public void Gen1YellowItemBagTest() {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Yellow"),
            "Yellow"
        );
    }

    /*
     * Gold
     */
    [Test]
    public void Gen2GoldItemPocketTest() {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldKeyItemPocketTest() {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("KeyItems", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldBallPocketTest() {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList("Balls", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldTMHMPocketTest() {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM/HM", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldItemPCTest() {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldItemBagTest() {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Gold"),
            "Gold"
        );
    }

    /*
     * Silver
     */
    [Test]
    public void Gen2SilverItemPocketTest() {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverKeyItemPocketTest() {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("KeyItems", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverBallPocketTest() {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList("Balls", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverTMHMPocketTest() {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM/HM", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverItemPCTest() {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverItemBagTest() {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Silver"),
            "Silver"
        );
    }

    /*
     * Crystal
     */
    [Test]
    public void Gen2CrystalItemPocketTest() {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalKeyItemPocketTest() {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("KeyItems", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalBallPocketTest() {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList("Balls", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalTMHMPocketTest() {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM/HM", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalItemPCTest() {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalItemBagTest() {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Crystal"),
            "Crystal"
        );
    }
}
