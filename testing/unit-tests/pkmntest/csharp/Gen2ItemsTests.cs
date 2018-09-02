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

public class Gen2ItemsTest
{
    private static PKMN.Item[] AllPocketItems =
    {
        PKMN.Item.POTION,
        PKMN.Item.BICYCLE,
        PKMN.Item.GREAT_BALL,
        PKMN.Item.TM28,
        PKMN.Item.BERRY,
        PKMN.Item.SQUIRT_BOTTLE,
        PKMN.Item.FRIEND_BALL,
        PKMN.Item.HM01
    };
    private static PKMN.Item[] CrystalItems =
    {
        PKMN.Item.CLEAR_BELL,
        PKMN.Item.GS_BALL,
        PKMN.Item.BLUE_CARD,
        PKMN.Item.EGG_TICKET
    };
    private static PKMN.Item[] WrongGenerationAllPocketItems =
    {
        PKMN.Item.BLACK_SLUDGE,
        PKMN.Item.POFFIN_CASE,
        PKMN.Item.TM51,
        PKMN.Item.OCCA_BERRY
    };

    public static void ItemPocketTest(
        PKMN.ItemList itemPocket,
        PKMN.Game game
    )
    {
        // Check unchanging and initial values.
        Assert.AreEqual(itemPocket.Name, "Items");
        Assert.AreEqual(itemPocket.Game, game);
        Assert.AreEqual(itemPocket.Length, 20);
        Assert.AreEqual(itemPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(itemPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            itemPocket,
            PKMN.Item.POTION
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.BICYCLE,
                    PKMN.Item.MASTER_BALL,
                    PKMN.Item.HM01
                }
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.BLACK_FLUTE,
                    PKMN.Item.BLACK_SLUDGE,
                    PKMN.Item.BINDING_BAND,
                    PKMN.Item.BEEDRILLITE
                }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        PKMN.Item[] validItems =
        {
            PKMN.Item.POTION,
            PKMN.Item.HP_UP,
            PKMN.Item.WHITE_APRICORN,
            PKMN.Item.LUCKY_EGG,
            PKMN.Item.FLOWER_MAIL,
            PKMN.Item.BURN_HEAL,
            PKMN.Item.PSNCUREBERRY,
            PKMN.Item.STICK
        };
        ItemsTestsCommon.TestItemListSettingItems(
            itemPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            itemPocket,
            validItems
        );

        Assert.AreEqual(itemPocket.ValidItems.Count, itemPocket.ValidItemNames.Count);
        Assert.Greater(itemPocket.ValidItems.Count, 0);
    }

    public static void KeyItemPocketTest(
        PKMN.ItemList keyItemPocket,
        PKMN.Game game
    )
    {
        // Check unchanging and initial values.
        Assert.AreEqual(keyItemPocket.Name, "KeyItems");
        Assert.AreEqual(keyItemPocket.Game, game);
        Assert.AreEqual(keyItemPocket.Length, 25);
        Assert.AreEqual(keyItemPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(keyItemPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            keyItemPocket,
            PKMN.Item.BICYCLE
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.POTION,
                    PKMN.Item.MASTER_BALL,
                    PKMN.Item.HM01
                }
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.MACH_BIKE,
                    PKMN.Item.JADE_ORB,
                    PKMN.Item.LIGHT_STONE,
                    PKMN.Item.AQUA_SUIT
                }
        );

        // Crystal-specific items.
        if(game == PKMN.Game.CRYSTAL)
        {
            foreach(PKMN.Item crystalItem in CrystalItems)
            {
                keyItemPocket.Add(crystalItem, 1);
                keyItemPocket.Remove(crystalItem, 1);
            }

            Assert.AreEqual(keyItemPocket.NumItems, 0);
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                CrystalItems
            );
        }

        // Make sure we can't add or remove more than a single item.
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                keyItemPocket.Add(PKMN.Item.BICYCLE, 5);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                keyItemPocket.Remove(PKMN.Item.BICYCLE, 5);
            }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        PKMN.Item[] validItems =
        {
            PKMN.Item.BICYCLE,
            PKMN.Item.BASEMENT_KEY,
            PKMN.Item.SECRETPOTION,
            PKMN.Item.MYSTERY_EGG,
            PKMN.Item.SILVER_WING,
            PKMN.Item.LOST_ITEM,
            PKMN.Item.SQUIRT_BOTTLE,
            PKMN.Item.RAINBOW_WING
        };

        for(int itemIndex = 0; itemIndex < validItems.Length; ++itemIndex)
        {
            if(itemIndex < 4)
            {
                keyItemPocket.Add(validItems[itemIndex], 1);
            }
            else
            {
                keyItemPocket[itemIndex].Item = validItems[itemIndex];
            }
        }

        keyItemPocket.Remove(validItems[2], 1);
        keyItemPocket[2].Item = PKMN.Item.NONE;
        Assert.AreEqual(keyItemPocket.NumItems, 6);

        // Empty the rest for the bag test.
        while(keyItemPocket[0].Item != PKMN.Item.NONE)
        {
            keyItemPocket[0].Item = PKMN.Item.NONE;
        }

        Assert.AreEqual(keyItemPocket.ValidItems.Count, keyItemPocket.ValidItemNames.Count);
        Assert.Greater(keyItemPocket.ValidItems.Count, 0);
    }

    public static void BallPocketTest(
        PKMN.ItemList ballPocket,
        PKMN.Game game
    )
    {
        // Check unchanging and initial values.
        Assert.AreEqual(ballPocket.Name, "Balls");
        Assert.AreEqual(ballPocket.Game, game);
        Assert.AreEqual(ballPocket.Length, 12);
        Assert.AreEqual(ballPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(ballPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            ballPocket,
            PKMN.Item.MASTER_BALL
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            ballPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.POTION,
                    PKMN.Item.BICYCLE,
                    PKMN.Item.HM01
                }
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            ballPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.PREMIER_BALL,
                    PKMN.Item.HEAL_BALL,
                    PKMN.Item.DREAM_BALL
                }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        PKMN.Item[] validItems =
        {
            PKMN.Item.GREAT_BALL,
            PKMN.Item.POKE_BALL,
            PKMN.Item.PARK_BALL,
            PKMN.Item.FAST_BALL,
            PKMN.Item.MASTER_BALL,
            PKMN.Item.FRIEND_BALL,
            PKMN.Item.LOVE_BALL,
            PKMN.Item.LEVEL_BALL
        };
        ItemsTestsCommon.TestItemListSettingItems(
            ballPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            ballPocket,
            validItems
        );

        Assert.AreEqual(ballPocket.ValidItems.Count, ballPocket.ValidItemNames.Count);
        Assert.Greater(ballPocket.ValidItems.Count, 0);
    }

    public static void TMHMPocketTest(
        PKMN.ItemList tmhmPocket,
        PKMN.Game game
    )
    {
        // Check unchanging and initial values.
        Assert.AreEqual(tmhmPocket.Name, "TM/HM");
        Assert.AreEqual(tmhmPocket.Game, game);
        Assert.AreEqual(tmhmPocket.Length, 57);
        Assert.AreEqual(tmhmPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        Assert.AreEqual(tmhmPocket.Length, 57);
        for(int tmIndex = 1; tmIndex <= 50; ++tmIndex)
        {
            PKMN.Item expectedItem = (PKMN.Item.TM01 + (tmIndex-1));
            Assert.AreEqual(tmhmPocket[tmIndex-1].Item, expectedItem);
            Assert.AreEqual(tmhmPocket[tmIndex-1].Amount, 0);
        }
        for(int hmIndex = 1; hmIndex <= 7; ++hmIndex)
        {
            PKMN.Item expectedItem = (PKMN.Item.HM01 + (hmIndex-1));
            Assert.AreEqual(tmhmPocket[50+hmIndex-1].Item, expectedItem);
            Assert.AreEqual(tmhmPocket[50+hmIndex-1].Amount, 0);
        }

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            tmhmPocket,
            PKMN.Item.TM10
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmhmPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.BICYCLE,
                    PKMN.Item.MASTER_BALL,
                    PKMN.Item.BICYCLE
                }
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmhmPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.TM51
                }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        for(int tmIndex = 1; tmIndex <= 50; ++tmIndex)
        {
            PKMN.Item item = (PKMN.Item.TM01 + (tmIndex-1));
            tmhmPocket.Add(item, 50);
            Assert.AreEqual(tmhmPocket.NumItems, tmIndex);
            Assert.AreEqual(tmhmPocket[tmIndex-1].Item, item);
            Assert.AreEqual(tmhmPocket[tmIndex-1].Amount, 50);
        }
        for(int tmIndex = 50; tmIndex >= 1; --tmIndex)
        {
            PKMN.Item item = (PKMN.Item.TM01 + (tmIndex-1));
            tmhmPocket.Remove(item, 50);
            Assert.AreEqual(tmhmPocket.NumItems, tmIndex-1);
            Assert.AreEqual(tmhmPocket[tmIndex-1].Item, item);
            Assert.AreEqual(tmhmPocket[tmIndex-1].Amount, 0);
        }

        for(int hmIndex = 1; hmIndex <= 7; ++hmIndex)
        {
            PKMN.Item item = (PKMN.Item.HM01 + (hmIndex-1));
            tmhmPocket.Add(item, 1);
            Assert.AreEqual(tmhmPocket.NumItems, hmIndex);
            Assert.AreEqual(tmhmPocket[50+hmIndex-1].Item, item);
            Assert.AreEqual(tmhmPocket[50+hmIndex-1].Amount, 1);
        }
        for(int hmIndex = 7; hmIndex >= 1; --hmIndex)
        {
            PKMN.Item item = (PKMN.Item.HM01 + (hmIndex-1));
            tmhmPocket.Remove(item, 1);
            Assert.AreEqual(tmhmPocket.NumItems, hmIndex-1);
            Assert.AreEqual(tmhmPocket[50+hmIndex-1].Item, item);
            Assert.AreEqual(tmhmPocket[50+hmIndex-1].Amount, 0);
        }

        Assert.AreEqual(tmhmPocket.ValidItems.Count, tmhmPocket.ValidItemNames.Count);
        Assert.AreEqual(tmhmPocket.ValidItems.Count, 57);
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

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(itemPC);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            itemPC,
            PKMN.Item.POTION
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPC,
            WrongGenerationAllPocketItems
        );

        // Crystal-specific items.
        if(game == PKMN.Game.CRYSTAL)
        {
            foreach(PKMN.Item crystalItem in CrystalItems)
            {
                itemPC.Add(crystalItem, 1);
                itemPC.Remove(crystalItem, 1);
            }

            Assert.AreEqual(itemPC.NumItems, 0);
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                itemPC,
                CrystalItems
            );
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListSettingItems(
            itemPC,
            AllPocketItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            itemPC,
            AllPocketItems
        );

        PKMN.ItemEnumList fullItemList = PKMN.Database.Lists.ItemList(game);

        Assert.AreEqual(itemPC.ValidItems.Count, itemPC.ValidItemNames.Count);
        Assert.AreEqual(itemPC.ValidItems.Count, fullItemList.Count);
    }

    public static void ItemBagTest(
        PKMN.ItemBag itemBag,
        PKMN.Game game
    )
    {
        // Check unchanging and initial values.
        Assert.AreEqual(itemBag.Game, game);
        Assert.AreEqual(itemBag.Count, 4);

        ItemPocketTest(itemBag["Items"], game);
        KeyItemPocketTest(itemBag["KeyItems"], game);
        BallPocketTest(itemBag["Balls"], game);
        TMHMPocketTest(itemBag["TM/HM"], game);

        // Make sure adding items through the bag adds to the proper pockets.
        Assert.AreEqual(itemBag["Items"].NumItems, 0);
        Assert.AreEqual(itemBag["KeyItems"].NumItems, 0);
        Assert.AreEqual(itemBag["Balls"].NumItems, 0);
        Assert.AreEqual(itemBag["TM/HM"].NumItems, 0);
        foreach(PKMN.Item item in AllPocketItems)
        {
            itemBag.Add(item, 1);
        }

        Assert.AreEqual(itemBag["Items"][0].Item, PKMN.Item.POTION);
        Assert.AreEqual(itemBag["Items"][0].Amount, 1);
        Assert.AreEqual(itemBag["Items"][1].Item, PKMN.Item.BERRY);
        Assert.AreEqual(itemBag["Items"][1].Amount, 1);
        Assert.AreEqual(itemBag["Items"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Items"][2].Amount, 0);

        Assert.AreEqual(itemBag["KeyItems"][0].Item, PKMN.Item.BICYCLE);
        Assert.AreEqual(itemBag["KeyItems"][0].Amount, 1);
        Assert.AreEqual(itemBag["KeyItems"][1].Item, PKMN.Item.SQUIRT_BOTTLE);
        Assert.AreEqual(itemBag["KeyItems"][1].Amount, 1);
        Assert.AreEqual(itemBag["KeyItems"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["KeyItems"][2].Amount, 0);

        Assert.AreEqual(itemBag["Balls"][0].Item, PKMN.Item.GREAT_BALL);
        Assert.AreEqual(itemBag["Balls"][0].Amount, 1);
        Assert.AreEqual(itemBag["Balls"][1].Item, PKMN.Item.FRIEND_BALL);
        Assert.AreEqual(itemBag["Balls"][1].Amount, 1);
        Assert.AreEqual(itemBag["Balls"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Balls"][2].Amount, 0);

        Assert.AreEqual(itemBag["TM/HM"][0].Item, PKMN.Item.TM01);
        Assert.AreEqual(itemBag["TM/HM"][0].Amount, 0);
        Assert.AreEqual(itemBag["TM/HM"][1].Item, PKMN.Item.TM02);
        Assert.AreEqual(itemBag["TM/HM"][1].Amount, 0);
        Assert.AreEqual(itemBag["TM/HM"][27].Item, PKMN.Item.TM28);
        Assert.AreEqual(itemBag["TM/HM"][27].Amount, 1);
        Assert.AreEqual(itemBag["TM/HM"][50].Item, PKMN.Item.HM01);
        Assert.AreEqual(itemBag["TM/HM"][50].Amount, 1);

        // Make sure removing items through the bag removes from the proper pockets.
        foreach(PKMN.Item item in AllPocketItems)
        {
            itemBag.Remove(item, 1);
        }

        Assert.AreEqual(itemBag["Items"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Items"][0].Amount, 0);
        Assert.AreEqual(itemBag["Items"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Items"][1].Amount, 0);
        Assert.AreEqual(itemBag["Items"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Items"][2].Amount, 0);

        Assert.AreEqual(itemBag["KeyItems"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["KeyItems"][0].Amount, 0);
        Assert.AreEqual(itemBag["KeyItems"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["KeyItems"][1].Amount, 0);
        Assert.AreEqual(itemBag["KeyItems"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["KeyItems"][2].Amount, 0);

        Assert.AreEqual(itemBag["Balls"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Balls"][0].Amount, 0);
        Assert.AreEqual(itemBag["Balls"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Balls"][1].Amount, 0);
        Assert.AreEqual(itemBag["Balls"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Balls"][2].Amount, 0);

        Assert.AreEqual(itemBag["TM/HM"][0].Item, PKMN.Item.TM01);
        Assert.AreEqual(itemBag["TM/HM"][0].Amount, 0);
        Assert.AreEqual(itemBag["TM/HM"][1].Item, PKMN.Item.TM02);
        Assert.AreEqual(itemBag["TM/HM"][1].Amount, 0);
        Assert.AreEqual(itemBag["TM/HM"][27].Item, PKMN.Item.TM28);
        Assert.AreEqual(itemBag["TM/HM"][27].Amount, 0);
        Assert.AreEqual(itemBag["TM/HM"][50].Item, PKMN.Item.HM01);
        Assert.AreEqual(itemBag["TM/HM"][50].Amount, 0);

        // Make sure we can't add Crystal-specific items with a Gold/Silver bag.
        if(game == PKMN.Game.CRYSTAL)
        {
            for(int i = 0; i < CrystalItems.Length; ++i)
            {
                itemBag.Add(CrystalItems[i], 1);
                Assert.AreEqual(itemBag["KeyItems"][i].Item, CrystalItems[i]);
                Assert.AreEqual(itemBag["KeyItems"][i].Amount, 1);
            }
            for(int i = CrystalItems.Length-1; i >= 0; --i)
            {
                itemBag.Remove(CrystalItems[i], 1);
                Assert.AreEqual(itemBag["KeyItems"][i].Item, PKMN.Item.NONE);
                Assert.AreEqual(itemBag["KeyItems"][i].Amount, 0);
            }
        }
        else
        {
            ItemsTestsCommon.TestItemBagInvalidItems(
                itemBag,
                CrystalItems
            );
        }

        // Make sure we can't add items from later generations.
        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            WrongGenerationAllPocketItems
        );
    }
}

}
