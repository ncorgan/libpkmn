/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

internal class ItemsTestCommon {
    public static void TestItemListEmptySlots(
        PKMN.ItemList itemList
    ) {
        PKMN.ItemEntry noneEntry = new PKMN.ItemEntry("None", itemList.GetGame());

        PKMN.ItemSlotList itemSlots = itemList.AsList();
        foreach(PKMN.ItemSlot itemSlot in itemSlots) {
            Assert.AreEqual(itemSlot.Item, noneEntry);
            Assert.AreEqual(itemSlot.Amount, 0);
        }
    }

    public static void TestItemListIndexOutOfRangeException(
        PKMN.ItemList itemList,
        string itemName
    ) {
        int numItems = itemList.GetNumItems();

        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                itemList.Add(itemName, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                itemList.Add(itemName, 100);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                itemList.Remove(itemName, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                itemList.Remove(itemName, 100);
            }
        );

        // This shouldn't have Added any items.
        Assert.AreEqual(itemList.GetNumItems(), numItems);
    }

    public static void TestItemBagInvalidItems(
        PKMN.ItemBag itemBag,
        string[] itemNames
    ) {
        foreach(string itemName in itemNames) {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    itemBag.Add(itemName, 1);
                }
            );
        }
    }

    public static void TestItemListInvalidItems(
        PKMN.ItemList itemList,
        string[] itemNames
    ) {
        int numItems = itemList.GetNumItems();

        foreach(string itemName in itemNames) {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    itemList.Add(itemName, 1);
                }
            );
        }

        // This shouldn't have added any items.
        Assert.AreEqual(itemList.GetNumItems(), numItems);
    }

    public static void TestItemListAddRemove(
        PKMN.ItemList itemList,
        string[] itemNames
    ) {
        Assert.AreEqual(itemNames.Length, 8);

        PKMN.ItemEntry noneEntry = new PKMN.ItemEntry("None", itemList.GetGame());

        itemList.Add(itemNames[0], 30);
        itemList.Add(itemNames[1], 99);
        itemList.Add(itemNames[2], 1);

        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[0]);
        Assert.AreEqual(itemList[0].Amount, 30);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[1].Amount, 99);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[2]);
        Assert.AreEqual(itemList[2].Amount, 1);
        Assert.AreEqual(itemList.GetNumItems(), 3);

        itemList.Add(itemNames[2], 15);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[0]);
        Assert.AreEqual(itemList[0].Amount, 30);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[1].Amount, 99);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[2]);
        Assert.AreEqual(itemList[2].Amount, 16);
        Assert.AreEqual(itemList.GetNumItems(), 3);

        itemList.Remove(itemNames[1], 20);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[0]);
        Assert.AreEqual(itemList[0].Amount, 30);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[1].Amount, 79);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[2]);
        Assert.AreEqual(itemList[2].Amount, 16);
        Assert.AreEqual(itemList.GetNumItems(), 3);

        itemList.Move(0, 1);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[0].Amount, 79);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[0]);
        Assert.AreEqual(itemList[1].Amount, 30);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[2]);
        Assert.AreEqual(itemList[2].Amount, 16);
        Assert.AreEqual(itemList.GetNumItems(), 3);

        itemList.Remove(itemNames[0], 30);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[0].Amount, 79);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[2]);
        Assert.AreEqual(itemList[1].Amount, 16);
        Assert.AreEqual(itemList[2].Item, noneEntry);
        Assert.AreEqual(itemList[2].Amount, 0);
        Assert.AreEqual(itemList.GetNumItems(), 2);

        itemList.Add(itemNames[3], 90);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[0].Amount, 79);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[2]);
        Assert.AreEqual(itemList[1].Amount, 16);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[3]);
        Assert.AreEqual(itemList[2].Amount, 90);
        Assert.AreEqual(itemList.GetNumItems(), 3);

        itemList.Add(itemNames[4], 2);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[0].Amount, 79);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[2]);
        Assert.AreEqual(itemList[1].Amount, 16);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[3]);
        Assert.AreEqual(itemList[2].Amount, 90);
        Assert.AreEqual(itemList[3].Item.GetName(), itemNames[4]);
        Assert.AreEqual(itemList[3].Amount, 2);
        Assert.AreEqual(itemList.GetNumItems(), 4);

        itemList.Remove(itemNames[1], 30);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[0].Amount, 49);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[2]);
        Assert.AreEqual(itemList[1].Amount, 16);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[3]);
        Assert.AreEqual(itemList[2].Amount, 90);
        Assert.AreEqual(itemList[3].Item.GetName(), itemNames[4]);
        Assert.AreEqual(itemList[3].Amount, 2);
        Assert.AreEqual(itemList.GetNumItems(), 4);

        itemList.Add(itemNames[5], 12);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[0].Amount, 49);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[2]);
        Assert.AreEqual(itemList[1].Amount, 16);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[3]);
        Assert.AreEqual(itemList[2].Amount, 90);
        Assert.AreEqual(itemList[3].Item.GetName(), itemNames[4]);
        Assert.AreEqual(itemList[3].Amount, 2);
        Assert.AreEqual(itemList[4].Item.GetName(), itemNames[5]);
        Assert.AreEqual(itemList[4].Amount, 12);
        Assert.AreEqual(itemList.GetNumItems(), 5);

        itemList.Remove(itemNames[2], 16);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[0].Amount, 49);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[3]);
        Assert.AreEqual(itemList[1].Amount, 90);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[4]);
        Assert.AreEqual(itemList[2].Amount, 2);
        Assert.AreEqual(itemList[3].Item.GetName(), itemNames[5]);
        Assert.AreEqual(itemList[3].Amount, 12);
        Assert.AreEqual(itemList.GetNumItems(), 4);

        itemList.Add(itemNames[6], 65);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[0].Amount, 49);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[3]);
        Assert.AreEqual(itemList[1].Amount, 90);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[4]);
        Assert.AreEqual(itemList[2].Amount, 2);
        Assert.AreEqual(itemList[3].Item.GetName(), itemNames[5]);
        Assert.AreEqual(itemList[3].Amount, 12);
        Assert.AreEqual(itemList[4].Item.GetName(), itemNames[6]);
        Assert.AreEqual(itemList[4].Amount, 65);
        Assert.AreEqual(itemList.GetNumItems(), 5);

        itemList.Add(itemNames[7], 6);
        Assert.AreEqual(itemList[0].Item.GetName(), itemNames[1]);
        Assert.AreEqual(itemList[0].Amount, 49);
        Assert.AreEqual(itemList[1].Item.GetName(), itemNames[3]);
        Assert.AreEqual(itemList[1].Amount, 90);
        Assert.AreEqual(itemList[2].Item.GetName(), itemNames[4]);
        Assert.AreEqual(itemList[2].Amount, 2);
        Assert.AreEqual(itemList[3].Item.GetName(), itemNames[5]);
        Assert.AreEqual(itemList[3].Amount, 12);
        Assert.AreEqual(itemList[4].Item.GetName(), itemNames[6]);
        Assert.AreEqual(itemList[4].Amount, 65);
        Assert.AreEqual(itemList[5].Item.GetName(), itemNames[7]);
        Assert.AreEqual(itemList[5].Amount, 6);
        Assert.AreEqual(itemList.GetNumItems(), 6);

        // Remove everything
        itemList.Remove(itemNames[4], 2);
        itemList.Remove(itemNames[1], 49);
        itemList.Remove(itemNames[7], 6);
        itemList.Remove(itemNames[5], 12);
        itemList.Remove(itemNames[3], 90);
        itemList.Remove(itemNames[6], 65);
        Assert.AreEqual(itemList.GetNumItems(), 0);
    }
}

}
