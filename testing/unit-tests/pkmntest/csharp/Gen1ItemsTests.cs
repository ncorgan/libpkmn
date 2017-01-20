/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class Gen1ItemsTest {

    private static string[] ItemNames = {
        "Potion", "Great Ball", "Ether", "PP Up",
        "TM34", "Moon Stone", "Bicycle", "Full Heal"
    };
    private static string[] WrongGenerationItemNames = {
        "Amulet Coin", "Apicot Berry", "Air Mail", "Air Balloon", "Aqua Suit"
    };

    public static void ItemListCommon(
        PKMN.ItemList itemList,
        string game
    ) {
        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(itemList);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            itemList,
            "Potion"
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemList,
            WrongGenerationItemNames
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            itemList,
            ItemNames
        );

        PKMN.StringList validItems = itemList.GetValidItems();
        PKMN.StringList fullItemList = PKMN.Database.GetItemList(game);
        Assert.AreEqual(validItems.Count, fullItemList.Count);
    }

    public static void ItemListTest(
        PKMN.ItemList itemList,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(itemList.GetName(), "Items");
        Assert.AreEqual(itemList.GetGame(), game);
        Assert.AreEqual(itemList.GetCapacity(), 20);
        Assert.AreEqual(itemList.GetNumItems(), 0);

        ItemListCommon(itemList, game);
    }

    public static void ItemPCTest(
        PKMN.ItemList itemPC,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(itemPC.GetName(), "PC");
        Assert.AreEqual(itemPC.GetGame(), game);
        Assert.AreEqual(itemPC.GetCapacity(), 50);
        Assert.AreEqual(itemPC.GetNumItems(), 0);

        ItemListCommon(itemPC, game);
    }

    public static void ItemBagTest(
        PKMN.ItemBag itemBag,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(itemBag.GetGame(), game);

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            WrongGenerationItemNames
        );

        PKMN.ItemPockets pockets = itemBag.GetPockets();
        Assert.AreEqual(pockets.Count, 1);
        ItemListTest(pockets["Items"], game);

        // Make sure adding items through the bag adds to the pocket.
        PKMN.ItemList itemPocket = pockets["Items"];
        Assert.AreEqual(itemPocket.GetNumItems(), 0);

        for(int i = 0; i < 8; ++i) {
            itemBag.Add(
                ItemNames[i],
                i+1
            );
        }
        for(int i = 0; i < 8; ++i) {
            Assert.AreEqual(itemPocket[i].Item.Name, ItemNames[i]);
            Assert.AreEqual(itemPocket[i].Amount, i+1);
        }
        Assert.AreEqual(itemPocket[8].Item.Name, "None");
        Assert.AreEqual(itemPocket[8].Amount, 0);

        for(int i = 0; i < 8; ++i) {
            itemBag.Remove(
                ItemNames[i],
                i+1
            );
        }
        for(int i = 0; i < 9; ++i) {
            Assert.AreEqual(itemPocket[i].Item.Name, "None");
            Assert.AreEqual(itemPocket[i].Amount, 0);
        }
    }
}

}
