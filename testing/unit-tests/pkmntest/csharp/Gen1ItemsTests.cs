/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class Gen1ItemsTest
{
    private static string[] ItemNames =
    {
        "Potion", "Great Ball", "Ether", "PP Up",
        "TM34", "Moon Stone", "Bicycle", "Full Heal"
    };
    private static string[] WrongGenerationItemNames =
    {
        "Amulet Coin", "Apicot Berry", "Air Mail", "Air Balloon", "Aqua Suit"
    };

    public static void ItemListCommon(
        PKMN.ItemList itemList,
        string game
    )
    {
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
        ItemsTestsCommon.TestItemListSettingItems(
            itemList,
            ItemNames
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            itemList,
            ItemNames
        );

        PKMN.StringList fullItemList = PKMN.Database.Lists.ItemList(game);
        Assert.AreEqual(itemList.ValidItems.Count, fullItemList.Count);
    }

    public static void ItemListTest(
        PKMN.ItemList itemList,
        string game
    )
    {
        // Check unchanging and initial values.
        Assert.AreEqual(itemList.Name, "Items");
        Assert.AreEqual(itemList.Game, game);
        Assert.AreEqual(itemList.Length, 20);
        Assert.AreEqual(itemList.NumItems, 0);

        ItemListCommon(itemList, game);
    }

    public static void ItemPCTest(
        PKMN.ItemList itemPC,
        string game
    )
    {
        // Check unchanging and initial values.
        Assert.AreEqual(itemPC.Name, "PC");
        Assert.AreEqual(itemPC.Game, game);
        Assert.AreEqual(itemPC.Length, 50);
        Assert.AreEqual(itemPC.NumItems, 0);

        ItemListCommon(itemPC, game);
    }

    public static void ItemBagTest(
        PKMN.ItemBag itemBag,
        string game
    )
    {
        // Check unchanging and initial values.
        Assert.AreEqual(itemBag.Game, game);
        Assert.AreEqual(itemBag.Count, 1);

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            WrongGenerationItemNames
        );

        ItemListTest(itemBag["Items"], game);

        // Make sure adding items through the bag adds to the pocket.
        PKMN.ItemList itemPocket = itemBag["Items"];
        Assert.AreEqual(itemPocket.NumItems, 0);

        for(int i = 0; i < 8; ++i)
        {
            itemBag.Add(
                ItemNames[i],
                i+1
            );
        }
        for(int i = 0; i < 8; ++i)
        {
            Assert.AreEqual(itemPocket[i].Item, ItemNames[i]);
            Assert.AreEqual(itemPocket[i].Amount, i+1);
        }
        Assert.AreEqual(itemPocket[8].Item, "None");
        Assert.AreEqual(itemPocket[8].Amount, 0);

        for(int i = 0; i < 8; ++i)
        {
            itemBag.Remove(
                ItemNames[i],
                i+1
            );
        }
        for(int i = 0; i < 9; ++i)
        {
            Assert.AreEqual(itemPocket[i].Item, "None");
            Assert.AreEqual(itemPocket[i].Amount, 0);
        }
    }
}

}
