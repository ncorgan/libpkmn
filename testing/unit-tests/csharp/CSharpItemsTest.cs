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
}
