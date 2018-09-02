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
    internal class ItemsTestsCommon
    {
        public static void TestItemListEmptySlots(
            PKMN.ItemList itemList
        )
        {
            Assert.AreEqual(0, itemList.NumItems);
            for(int itemIndex = 0; itemIndex < itemList.Length; ++itemIndex)
            {
                Assert.AreEqual(PKMN.Item.NONE, itemList[itemIndex].Item);
                Assert.AreEqual(0, itemList[itemIndex].Amount);
            }
        }

        public static void TestItemListIndexOutOfRangeException(
            PKMN.ItemList itemList,
            PKMN.Item item
        )
        {
            int oldNumItems = itemList.NumItems;

            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList.Add(item, 0);
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList.Remove(item, 100);
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList.Remove(item, 0);
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList.Add(item, 100);
                }
            );

            // This shouldn't have added any items.
            Assert.AreEqual(oldNumItems, itemList.NumItems);
        }

        public static void TestItemBagInvalidItems(
            PKMN.ItemBag itemBag,
            PKMN.Item[] items
        )
        {
            foreach(PKMN.Item item in items)
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        itemBag.Add(item, 1);
                    }
                );
            }
        }

        public static void TestItemListInvalidItems(
            PKMN.ItemList itemList,
            PKMN.Item[] items
        )
        {
            int oldNumItems = itemList.NumItems;

            foreach(PKMN.Item itemName in items)
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        itemList.Add(itemName, 1);
                    }
                );
            }

            // This shouldn't have added any items.
            Assert.AreEqual(oldNumItems, itemList.NumItems);
        }

        public static void TestItemListSettingItems(
            PKMN.ItemList itemList,
            PKMN.Item[] items
        )
        {
            Assert.AreEqual(0, itemList.NumItems);
            Assert.AreEqual(8, items.Length);

            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList[-1].Item = items[0];
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList[itemList.Length].Item = items[0];
                }
            );

            itemList[0].Item = items[0];
            itemList[0].Amount = 50;
            itemList[1].Item = items[1];
            itemList[1].Amount = 40;
            itemList[2].Item = items[2];
            itemList[2].Amount = 30;

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(items[0], itemList[0].Item);
            Assert.AreEqual(50, itemList[0].Amount);
            Assert.AreEqual(items[1], itemList[1].Item);
            Assert.AreEqual(40, itemList[1].Amount);
            Assert.AreEqual(items[2], itemList[2].Item);
            Assert.AreEqual(30, itemList[2].Amount);

            // Make sure the item list being contiguous is enforced.
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList[10].Item = items[3];
                }
            );

            itemList[1].Item = PKMN.Item.NONE;

            Assert.AreEqual(2, itemList.NumItems);
            Assert.AreEqual(items[0], itemList[0].Item);
            Assert.AreEqual(50, itemList[0].Amount);
            Assert.AreEqual(items[2], itemList[1].Item);
            Assert.AreEqual(30, itemList[1].Amount);
            Assert.AreEqual(PKMN.Item.NONE, itemList[2].Item);
            Assert.AreEqual(0, itemList[2].Amount);

            itemList[0].Item = PKMN.Item.NONE;
            itemList[0].Item = PKMN.Item.NONE;

            Assert.AreEqual(0, itemList.NumItems);
            for(int itemIndex = 0; itemIndex < 3; ++itemIndex)
            {
                Assert.AreEqual(PKMN.Item.NONE, itemList[itemIndex].Item);
                Assert.AreEqual(0, itemList[itemIndex].Amount);
            }
        }

        public static void TestItemListAddingAndRemovingItems(
            PKMN.ItemList itemList,
            PKMN.Item[] items
        )
        {
            Assert.AreEqual(0, itemList.NumItems);
            Assert.AreEqual(8, items.Length);

            itemList.Add(items[0], 30);
            itemList.Add(items[1], 99);
            itemList.Add(items[2], 1);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(items[0], itemList[0].Item);
            Assert.AreEqual(30, itemList[0].Amount);
            Assert.AreEqual(items[1], itemList[1].Item);
            Assert.AreEqual(99, itemList[1].Amount);
            Assert.AreEqual(items[2], itemList[2].Item);
            Assert.AreEqual(1, itemList[2].Amount);

            itemList.Add(items[2], 15);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(items[0], itemList[0].Item);
            Assert.AreEqual(30, itemList[0].Amount);
            Assert.AreEqual(items[1], itemList[1].Item);
            Assert.AreEqual(99, itemList[1].Amount);
            Assert.AreEqual(items[2], itemList[2].Item);
            Assert.AreEqual(16, itemList[2].Amount);

            itemList.Remove(items[1], 20);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(items[0], itemList[0].Item);
            Assert.AreEqual(30, itemList[0].Amount);
            Assert.AreEqual(items[1], itemList[1].Item);
            Assert.AreEqual(79, itemList[1].Amount);
            Assert.AreEqual(items[2], itemList[2].Item);
            Assert.AreEqual(16, itemList[2].Amount);

            itemList.Move(0, 1);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(items[1], itemList[0].Item);
            Assert.AreEqual(79, itemList[0].Amount);
            Assert.AreEqual(items[0], itemList[1].Item);
            Assert.AreEqual(30, itemList[1].Amount);
            Assert.AreEqual(items[2], itemList[2].Item);
            Assert.AreEqual(16, itemList[2].Amount);

            itemList.Remove(items[0], 30);

            Assert.AreEqual(2, itemList.NumItems);
            Assert.AreEqual(items[1], itemList[0].Item);
            Assert.AreEqual(79, itemList[0].Amount);
            Assert.AreEqual(items[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual(PKMN.Item.NONE, itemList[2].Item);
            Assert.AreEqual(0, itemList[2].Amount);

            itemList.Add(items[3], 90);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(items[1], itemList[0].Item);
            Assert.AreEqual(79, itemList[0].Amount);
            Assert.AreEqual(items[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual(items[3], itemList[2].Item);
            Assert.AreEqual(90, itemList[2].Amount);

            itemList.Add(items[4], 2);

            Assert.AreEqual(4, itemList.NumItems);
            Assert.AreEqual(items[1], itemList[0].Item);
            Assert.AreEqual(79, itemList[0].Amount);
            Assert.AreEqual(items[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual(items[3], itemList[2].Item);
            Assert.AreEqual(90, itemList[2].Amount);
            Assert.AreEqual(items[4], itemList[3].Item);
            Assert.AreEqual(2, itemList[3].Amount);

            itemList.Remove(items[1], 30);

            Assert.AreEqual(4, itemList.NumItems);
            Assert.AreEqual(items[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(items[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual(items[3], itemList[2].Item);
            Assert.AreEqual(90, itemList[2].Amount);
            Assert.AreEqual(items[4], itemList[3].Item);
            Assert.AreEqual(2, itemList[3].Amount);

            itemList.Add(items[5], 12);

            Assert.AreEqual(5, itemList.NumItems);
            Assert.AreEqual(items[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(items[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual(items[3], itemList[2].Item);
            Assert.AreEqual(90, itemList[2].Amount);
            Assert.AreEqual(items[4], itemList[3].Item);
            Assert.AreEqual(2, itemList[3].Amount);
            Assert.AreEqual(items[5], itemList[4].Item);
            Assert.AreEqual(12, itemList[4].Amount);

            itemList.Remove(items[2], 16);

            Assert.AreEqual(4, itemList.NumItems);
            Assert.AreEqual(items[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(items[3], itemList[1].Item);
            Assert.AreEqual(90, itemList[1].Amount);
            Assert.AreEqual(items[4], itemList[2].Item);
            Assert.AreEqual(2, itemList[2].Amount);
            Assert.AreEqual(items[5], itemList[3].Item);
            Assert.AreEqual(12, itemList[3].Amount);

            itemList.Add(items[6], 65);

            Assert.AreEqual(5, itemList.NumItems);
            Assert.AreEqual(items[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(items[3], itemList[1].Item);
            Assert.AreEqual(90, itemList[1].Amount);
            Assert.AreEqual(items[4], itemList[2].Item);
            Assert.AreEqual(2, itemList[2].Amount);
            Assert.AreEqual(items[5], itemList[3].Item);
            Assert.AreEqual(12, itemList[3].Amount);
            Assert.AreEqual(items[6], itemList[4].Item);
            Assert.AreEqual(65, itemList[4].Amount);

            itemList.Add(items[7], 6);

            Assert.AreEqual(6, itemList.NumItems);
            Assert.AreEqual(items[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(items[3], itemList[1].Item);
            Assert.AreEqual(90, itemList[1].Amount);
            Assert.AreEqual(items[4], itemList[2].Item);
            Assert.AreEqual(2, itemList[2].Amount);
            Assert.AreEqual(items[5], itemList[3].Item);
            Assert.AreEqual(12, itemList[3].Amount);
            Assert.AreEqual(items[6], itemList[4].Item);
            Assert.AreEqual(65, itemList[4].Amount);
            Assert.AreEqual(items[7], itemList[5].Item);
            Assert.AreEqual(6, itemList[5].Amount);

            // Remove everything.
            itemList.Remove(items[1], 49);
            itemList.Remove(items[3], 90);
            itemList.Remove(items[4], 2);
            itemList.Remove(items[5], 12);
            itemList.Remove(items[6], 65);
            itemList.Remove(items[7], 6);

            Assert.AreEqual(0, itemList.NumItems);
            for(int itemIndex = 0; itemIndex < 8; ++itemIndex)
            {
                Assert.AreEqual(PKMN.Item.NONE, itemList[itemIndex].Item);
                Assert.AreEqual(0, itemList[itemIndex].Amount);
            }
        }
    }
}
