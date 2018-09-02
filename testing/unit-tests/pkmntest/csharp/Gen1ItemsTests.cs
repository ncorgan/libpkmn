/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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
    private static PKMN.Item[] Items =
    {
        PKMN.Item.POTION,
        PKMN.Item.GREAT_BALL,
        PKMN.Item.ETHER,
        PKMN.Item.PP_UP,
        PKMN.Item.TM34,
        PKMN.Item.MOON_STONE,
        PKMN.Item.BICYCLE,
        PKMN.Item.FULL_HEAL
    };
    private static PKMN.Item[] WrongGenerationItems =
    {
        PKMN.Item.AMULET_COIN,
        PKMN.Item.APICOT_BERRY,
        PKMN.Item.AIR_MAIL,
        PKMN.Item.AIR_BALLOON,
        PKMN.Item.AQUA_SUIT
    };

    public static void ItemListCommon(
        PKMN.ItemList itemList,
        PKMN.Game game
    )
    {
        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(itemList);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            itemList,
            PKMN.Item.POTION
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemList,
            WrongGenerationItems
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListSettingItems(
            itemList,
            Items
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            itemList,
            Items
        );

        PKMN.ItemEnumList fullItemList = PKMN.Database.Lists.ItemList(game);

        Assert.AreEqual(itemList.ValidItems.Count, itemList.ValidItemNames.Count);
        Assert.AreEqual(itemList.ValidItems.Count, fullItemList.Count);
    }

    public static void ItemListTest(
        PKMN.ItemList itemList,
        PKMN.Game game
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
        PKMN.Game game
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
        PKMN.Game game
    )
    {
        // Check unchanging and initial values.
        Assert.AreEqual(itemBag.Game, game);
        Assert.AreEqual(itemBag.Count, 1);

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            WrongGenerationItems
        );

        ItemListTest(itemBag["Items"], game);

        // Make sure adding items through the bag adds to the pocket.
        PKMN.ItemList itemPocket = itemBag["Items"];
        Assert.AreEqual(itemPocket.NumItems, 0);

        for(int i = 0; i < 8; ++i)
        {
            itemBag.Add(
                Items[i],
                i+1
            );
        }
        for(int i = 0; i < 8; ++i)
        {
            Assert.AreEqual(itemPocket[i].Item, Items[i]);
            Assert.AreEqual(itemPocket[i].Amount, i+1);
        }
        Assert.AreEqual(itemPocket[8].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemPocket[8].Amount, 0);

        for(int i = 0; i < 8; ++i)
        {
            itemBag.Remove(
                Items[i],
                i+1
            );
        }
        for(int i = 0; i < 9; ++i)
        {
            Assert.AreEqual(itemPocket[i].Item, PKMN.Item.NONE);
            Assert.AreEqual(itemPocket[i].Amount, 0);
        }
    }
}

}
