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
    internal class ItemsTestsCommon
    {
        public static void TestItemListEmptySlots(
            PKMN.ItemList2 itemList
        )
        {
            Assert.AreEqual(0, itemList.NumItems);
            for(int itemIndex = 0; itemIndex < itemList.Length; ++itemIndex)
            {
                Assert.AreEqual("None", itemList[itemIndex].Item);
                Assert.AreEqual(0, itemList[itemIndex].Amount);
            }
        }

        public static void TestItemListIndexOutOfRangeException(
            PKMN.ItemList2 itemList,
            string itemName
        )
        {
            int oldNumItems = itemList.NumItems;

            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList.Add(itemName, 0);
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList.Remove(itemName, 100);
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList.Remove(itemName, 0);
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList.Add(itemName, 100);
                }
            );

            // This shouldn't have added any items.
            Assert.AreEqual(oldNumItems, itemList.NumItems);
        }

        public static void TestItemBagInvalidItems(
            PKMN.ItemBag2 itemBag,
            string[] itemNames
        )
        {
            foreach(string itemName in itemNames)
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        itemBag.Add(itemName, 1);
                    }
                );
            }
        }

        public static void TestItemListInvalidItems(
            PKMN.ItemList2 itemList,
            string[] itemNames
        )
        {
            int oldNumItems = itemList.NumItems;

            foreach(string itemName in itemNames)
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
            PKMN.ItemList2 itemList,
            string[] itemNames
        )
        {
            Assert.AreEqual(0, itemList.NumItems);
            Assert.AreEqual(8, itemNames.Length);

            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList[-1].Item = itemNames[0];
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList[itemList.Length].Item = itemNames[0];
                }
            );

            itemList[0].Item = itemNames[0];
            itemList[0].Amount = 50;
            itemList[1].Item = itemNames[1];
            itemList[1].Amount = 40;
            itemList[2].Item = itemNames[2];
            itemList[2].Amount = 30;

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(itemNames[0], itemList[0].Item);
            Assert.AreEqual(50, itemList[0].Amount);
            Assert.AreEqual(itemNames[1], itemList[1].Item);
            Assert.AreEqual(40, itemList[1].Amount);
            Assert.AreEqual(itemNames[2], itemList[2].Item);
            Assert.AreEqual(30, itemList[2].Amount);

            // Make sure the item list being contiguous is enforced.
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    itemList[10].Item = itemNames[3];
                }
            );

            itemList[1].Item = "None";

            Assert.AreEqual(2, itemList.NumItems);
            Assert.AreEqual(itemNames[0], itemList[0].Item);
            Assert.AreEqual(50, itemList[0].Amount);
            Assert.AreEqual(itemNames[2], itemList[1].Item);
            Assert.AreEqual(30, itemList[1].Amount);
            Assert.AreEqual("None", itemList[2].Item);
            Assert.AreEqual(0, itemList[2].Amount);

            itemList[0].Item = "None";
            itemList[0].Item = "None";

            Assert.AreEqual(0, itemList.NumItems);
            for(int itemIndex = 0; itemIndex < 3; ++itemIndex)
            {
                Assert.AreEqual("None", itemList[itemIndex].Item);
                Assert.AreEqual(0, itemList[itemIndex].Amount);
            }
        }

        public static void TestItemListAddingAndRemovingItems(
            PKMN.ItemList2 itemList,
            string[] itemNames
        )
        {
            Assert.AreEqual(0, itemList.NumItems);
            Assert.AreEqual(8, itemNames.Length);

            itemList.Add(itemNames[0], 30);
            itemList.Add(itemNames[1], 99);
            itemList.Add(itemNames[2], 1);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(itemNames[0], itemList[0].Item);
            Assert.AreEqual(30, itemList[0].Amount);
            Assert.AreEqual(itemNames[1], itemList[1].Item);
            Assert.AreEqual(99, itemList[1].Amount);
            Assert.AreEqual(itemNames[2], itemList[2].Item);
            Assert.AreEqual(1, itemList[2].Amount);

            itemList.Add(itemNames[2], 15);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(itemNames[0], itemList[0].Item);
            Assert.AreEqual(30, itemList[0].Amount);
            Assert.AreEqual(itemNames[1], itemList[1].Item);
            Assert.AreEqual(99, itemList[1].Amount);
            Assert.AreEqual(itemNames[2], itemList[2].Item);
            Assert.AreEqual(16, itemList[2].Amount);

            itemList.Remove(itemNames[1], 20);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(itemNames[0], itemList[0].Item);
            Assert.AreEqual(30, itemList[0].Amount);
            Assert.AreEqual(itemNames[1], itemList[1].Item);
            Assert.AreEqual(79, itemList[1].Amount);
            Assert.AreEqual(itemNames[2], itemList[2].Item);
            Assert.AreEqual(16, itemList[2].Amount);

            itemList.Move(0, 1);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(itemNames[1], itemList[0].Item);
            Assert.AreEqual(79, itemList[0].Amount);
            Assert.AreEqual(itemNames[0], itemList[1].Item);
            Assert.AreEqual(30, itemList[1].Amount);
            Assert.AreEqual(itemNames[2], itemList[2].Item);
            Assert.AreEqual(16, itemList[2].Amount);

            itemList.Remove(itemNames[0], 30);

            Assert.AreEqual(2, itemList.NumItems);
            Assert.AreEqual(itemNames[1], itemList[0].Item);
            Assert.AreEqual(79, itemList[0].Amount);
            Assert.AreEqual(itemNames[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual("None", itemList[2].Item);
            Assert.AreEqual(0, itemList[2].Amount);

            itemList.Add(itemNames[3], 90);

            Assert.AreEqual(3, itemList.NumItems);
            Assert.AreEqual(itemNames[1], itemList[0].Item);
            Assert.AreEqual(79, itemList[0].Amount);
            Assert.AreEqual(itemNames[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual(itemNames[3], itemList[2].Item);
            Assert.AreEqual(90, itemList[2].Amount);

            itemList.Add(itemNames[4], 2);

            Assert.AreEqual(4, itemList.NumItems);
            Assert.AreEqual(itemNames[1], itemList[0].Item);
            Assert.AreEqual(79, itemList[0].Amount);
            Assert.AreEqual(itemNames[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual(itemNames[3], itemList[2].Item);
            Assert.AreEqual(90, itemList[2].Amount);
            Assert.AreEqual(itemNames[4], itemList[3].Item);
            Assert.AreEqual(2, itemList[3].Amount);

            itemList.Remove(itemNames[1], 30);

            Assert.AreEqual(4, itemList.NumItems);
            Assert.AreEqual(itemNames[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(itemNames[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual(itemNames[3], itemList[2].Item);
            Assert.AreEqual(90, itemList[2].Amount);
            Assert.AreEqual(itemNames[4], itemList[3].Item);
            Assert.AreEqual(2, itemList[3].Amount);

            itemList.Add(itemNames[5], 12);

            Assert.AreEqual(5, itemList.NumItems);
            Assert.AreEqual(itemNames[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(itemNames[2], itemList[1].Item);
            Assert.AreEqual(16, itemList[1].Amount);
            Assert.AreEqual(itemNames[3], itemList[2].Item);
            Assert.AreEqual(90, itemList[2].Amount);
            Assert.AreEqual(itemNames[4], itemList[3].Item);
            Assert.AreEqual(2, itemList[3].Amount);
            Assert.AreEqual(itemNames[5], itemList[4].Item);
            Assert.AreEqual(12, itemList[4].Amount);

            itemList.Remove(itemNames[2], 16);

            Assert.AreEqual(4, itemList.NumItems);
            Assert.AreEqual(itemNames[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(itemNames[3], itemList[1].Item);
            Assert.AreEqual(90, itemList[1].Amount);
            Assert.AreEqual(itemNames[4], itemList[2].Item);
            Assert.AreEqual(2, itemList[2].Amount);
            Assert.AreEqual(itemNames[5], itemList[3].Item);
            Assert.AreEqual(12, itemList[3].Amount);

            itemList.Add(itemNames[6], 65);

            Assert.AreEqual(5, itemList.NumItems);
            Assert.AreEqual(itemNames[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(itemNames[3], itemList[1].Item);
            Assert.AreEqual(90, itemList[1].Amount);
            Assert.AreEqual(itemNames[4], itemList[2].Item);
            Assert.AreEqual(2, itemList[2].Amount);
            Assert.AreEqual(itemNames[5], itemList[3].Item);
            Assert.AreEqual(12, itemList[3].Amount);
            Assert.AreEqual(itemNames[6], itemList[4].Item);
            Assert.AreEqual(65, itemList[4].Amount);

            itemList.Add(itemNames[7], 6);

            Assert.AreEqual(6, itemList.NumItems);
            Assert.AreEqual(itemNames[1], itemList[0].Item);
            Assert.AreEqual(49, itemList[0].Amount);
            Assert.AreEqual(itemNames[3], itemList[1].Item);
            Assert.AreEqual(90, itemList[1].Amount);
            Assert.AreEqual(itemNames[4], itemList[2].Item);
            Assert.AreEqual(2, itemList[2].Amount);
            Assert.AreEqual(itemNames[5], itemList[3].Item);
            Assert.AreEqual(12, itemList[3].Amount);
            Assert.AreEqual(itemNames[6], itemList[4].Item);
            Assert.AreEqual(65, itemList[4].Amount);
            Assert.AreEqual(itemNames[7], itemList[5].Item);
            Assert.AreEqual(6, itemList[5].Amount);

            // Remove everything.
            itemList.Remove(itemNames[1], 49);
            itemList.Remove(itemNames[3], 90);
            itemList.Remove(itemNames[4], 2);
            itemList.Remove(itemNames[5], 12);
            itemList.Remove(itemNames[6], 65);
            itemList.Remove(itemNames[7], 6);

            Assert.AreEqual(0, itemList.NumItems);
            for(int itemIndex = 0; itemIndex < 8; ++itemIndex)
            {
                Assert.AreEqual("None", itemList[itemIndex].Item);
                Assert.AreEqual(0, itemList[itemIndex].Amount);
            }
        }
    }
}
