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

public class GBAItemsTest
{
    private static PKMN.Item[] AllPocketItems =
    {
        PKMN.Item.POTION,
        PKMN.Item.MACH_BIKE,
        PKMN.Item.GREAT_BALL,
        PKMN.Item.TM01,
        PKMN.Item.ASPEAR_BERRY,
        PKMN.Item.WAILMER_PAIL,
        PKMN.Item.MASTER_BALL,
        PKMN.Item.HM04
    };
    private static PKMN.Item[] WrongGameAllPocketItems =
    {
        PKMN.Item.PINK_BOW, PKMN.Item.BLACK_SLUDGE,
        PKMN.Item.EIN_FILE_S, PKMN.Item.GONZAPS_KEY,
        PKMN.Item.GS_BALL, PKMN.Item.POFFIN_CASE,
        PKMN.Item.TM51,
        PKMN.Item.BERRY, PKMN.Item.OCCA_BERRY
    };

    public static void ItemPocketTest(
        PKMN.ItemList itemPocket,
        PKMN.Game game
    )
    {
        int expectedLength = 0;
        if((game == PKMN.Game.RUBY) || (game == PKMN.Game.SAPPHIRE))
        {
            expectedLength = 20;
        }
        else if(game == PKMN.Game.EMERALD)
        {
            expectedLength = 30;
        }
        else
        {
            expectedLength = 42;
        }

        // Check unchanging and initial values.
        Assert.AreEqual(itemPocket.Name, "Items");
        Assert.AreEqual(itemPocket.Game, game);
        Assert.AreEqual(itemPocket.Length, expectedLength);
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
                    PKMN.Item.HM01,
                    PKMN.Item.RAZZ_BERRY
                }
        );

        // Confirm items from other generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.PINK_BOW,
                    PKMN.Item.BLACK_SLUDGE,
                    PKMN.Item.BINDING_BAND,
                    PKMN.Item.BEEDRILLITE
                }
        );

        // Make sure we can't add items from Gamecube games.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.TIME_FLUTE,
                    PKMN.Item.POKE_SNACK
                }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        PKMN.Item[] validItems =
        {
            PKMN.Item.POTION,
            PKMN.Item.ORANGE_MAIL,
            PKMN.Item.LAVA_COOKIE,
            PKMN.Item.STARDUST,
            PKMN.Item.SHADOW_MAIL,
            PKMN.Item.PINK_SCARF,
            PKMN.Item.ANTIDOTE,
            PKMN.Item.GREEN_SHARD
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
        int expectedLength = 0;
        if((game == PKMN.Game.RUBY) || (game == PKMN.Game.SAPPHIRE))
        {
            expectedLength = 20;
        }
        else
        {
            expectedLength = 30;
        }

        // Check unchanging and initial values.
        Assert.AreEqual(keyItemPocket.Name, "Key Items");
        Assert.AreEqual(keyItemPocket.Game, game);
        Assert.AreEqual(keyItemPocket.Length, expectedLength);
        Assert.AreEqual(keyItemPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(keyItemPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            keyItemPocket,
            PKMN.Item.BASEMENT_KEY
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.POTION,
                    PKMN.Item.MASTER_BALL,
                    PKMN.Item.HM01,
                    PKMN.Item.RAZZ_BERRY
                }
        );

        // Confirm items from other generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.GS_BALL,
                    PKMN.Item.POFFIN_CASE,
                    PKMN.Item.DNA_SPLICERS,
                    PKMN.Item.AQUA_SUIT
                }
        );

        // Confirm items from incompatible Generation III games can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.EIN_FILE_S,
                    PKMN.Item.POWERUP_PART,
                    PKMN.Item.GONZAPS_KEY,
                    PKMN.Item.KRANE_MEMO_1
                }
        );
        if((game == PKMN.Game.RUBY) || (game == PKMN.Game.SAPPHIRE))
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.HELIX_FOSSIL,
                        PKMN.Item.TEA,
                        PKMN.Item.RUBY
                    }
            );
        }
        if(game != PKMN.Game.EMERALD)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.MAGMA_EMBLEM,
                        PKMN.Item.OLD_SEA_MAP
                    }
            );
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        PKMN.Item[] validItems =
        {
            PKMN.Item.WAILMER_PAIL,
            PKMN.Item.BASEMENT_KEY,
            PKMN.Item.METEORITE,
            PKMN.Item.OLD_ROD,
            PKMN.Item.RED_ORB,
            PKMN.Item.ROOT_FOSSIL,
            PKMN.Item.CONTEST_PASS,
            PKMN.Item.EON_TICKET
        };
        ItemsTestsCommon.TestItemListSettingItems(
            keyItemPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            keyItemPocket,
            validItems
        );

        Assert.AreEqual(keyItemPocket.ValidItems.Count, keyItemPocket.ValidItemNames.Count);
        Assert.Greater(keyItemPocket.ValidItems.Count, 0);
    }

    public static void BallPocketTest(
        PKMN.ItemList ballPocket,
        PKMN.Game game
    )
    {
        int expectedLength = 0;
        if((game == PKMN.Game.FIRERED) || (game == PKMN.Game.LEAFGREEN))
        {
            expectedLength = 13;
        }
        else
        {
            expectedLength = 16;
        }

        // Check unchanging and initial values.
        Assert.AreEqual(ballPocket.Name, "Poké Balls");
        Assert.AreEqual(ballPocket.Game, game);
        Assert.AreEqual(ballPocket.Length, expectedLength);
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
                    PKMN.Item.HM01,
                    PKMN.Item.RAZZ_BERRY
                }
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            ballPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.MOON_BALL,
                    PKMN.Item.HEAL_BALL,
                    PKMN.Item.DREAM_BALL
                }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        PKMN.Item[] validItems =
        {
            PKMN.Item.MASTER_BALL,
            PKMN.Item.ULTRA_BALL,
            PKMN.Item.GREAT_BALL,
            PKMN.Item.POKE_BALL,
            PKMN.Item.SAFARI_BALL,
            PKMN.Item.NET_BALL,
            PKMN.Item.DIVE_BALL,
            PKMN.Item.NEST_BALL
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
        int expectedLength = 0;
        string expectedName = "";

        if((game == PKMN.Game.FIRERED) || (game == PKMN.Game.LEAFGREEN))
        {
            expectedLength = 58;
            expectedName = "TM Case";
        }
        else
        {
            expectedLength = 64;
            expectedName = "TMs & HMs";
        }

        // Check unchanging and initial values.
        Assert.AreEqual(tmhmPocket.Name, expectedName);
        Assert.AreEqual(tmhmPocket.Game, game);
        Assert.AreEqual(tmhmPocket.Length, expectedLength);
        Assert.AreEqual(tmhmPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(tmhmPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            tmhmPocket,
            PKMN.Item.TM01
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmhmPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.POTION,
                    PKMN.Item.BICYCLE,
                    PKMN.Item.GREAT_BALL,
                    PKMN.Item.RAZZ_BERRY
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
        PKMN.Item[] validItems =
        {
            PKMN.Item.TM01,
            PKMN.Item.HM01,
            PKMN.Item.TM02,
            PKMN.Item.HM02,
            PKMN.Item.TM03,
            PKMN.Item.HM03,
            PKMN.Item.TM04,
            PKMN.Item.HM04
        };
        ItemsTestsCommon.TestItemListSettingItems(
            tmhmPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            tmhmPocket,
            validItems
        );

        Assert.AreEqual(tmhmPocket.ValidItems.Count, tmhmPocket.ValidItemNames.Count);
        Assert.Greater(tmhmPocket.ValidItems.Count, 0);
    }

    public static void BerryPocketTest(
        PKMN.ItemList berryPocket,
        PKMN.Game game
    )
    {
        int expectedLength = 0;
        string expectedName = "";

        if((game == PKMN.Game.FIRERED) || (game == PKMN.Game.LEAFGREEN))
        {
            expectedLength = 43;
            expectedName = "Berry Pouch";
        }
        else
        {
            expectedLength = 46;
            expectedName = "Berries";
        }

        // Check unchanging and initial values.
        Assert.AreEqual(berryPocket.Name, expectedName);
        Assert.AreEqual(berryPocket.Game, game);
        Assert.AreEqual(berryPocket.Length, expectedLength);
        Assert.AreEqual(berryPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(berryPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            berryPocket,
            PKMN.Item.RAZZ_BERRY
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            berryPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.POTION,
                    PKMN.Item.BICYCLE,
                    PKMN.Item.GREAT_BALL,
                    PKMN.Item.HM02
                }
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            berryPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.BERRY,
                    PKMN.Item.OCCA_BERRY,
                    PKMN.Item.ROSELI_BERRY
                }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        PKMN.Item[] validItems =
        {
            PKMN.Item.CHERI_BERRY,
            PKMN.Item.RAZZ_BERRY,
            PKMN.Item.LUM_BERRY,
            PKMN.Item.PINAP_BERRY,
            PKMN.Item.ASPEAR_BERRY,
            PKMN.Item.IAPAPA_BERRY,
            PKMN.Item.WIKI_BERRY,
            PKMN.Item.APICOT_BERRY
        };
        ItemsTestsCommon.TestItemListSettingItems(
            berryPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            berryPocket,
            validItems
        );

        Assert.AreEqual(berryPocket.ValidItems.Count, berryPocket.ValidItemNames.Count);
        Assert.Greater(berryPocket.ValidItems.Count, 0);
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
            WrongGameAllPocketItems
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListSettingItems(
            itemPC,
            AllPocketItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            itemPC,
            AllPocketItems
        );

        Assert.AreEqual(itemPC.ValidItems.Count, itemPC.ValidItemNames.Count);

        PKMN.ItemEnumList fullItemList = PKMN.Database.Lists.ItemList(game);
        PKMN.StringList fullItemNameList = PKMN.Database.Lists.ItemNameList(game);
        Assert.AreEqual(fullItemList.Count, fullItemNameList.Count);

        if((game == PKMN.Game.FIRERED) || (game == PKMN.Game.LEAFGREEN))
        {
            Assert.AreEqual(itemPC.ValidItems.Count, fullItemList.Count-2);

            Assert.IsFalse(itemPC.ValidItems.Contains(PKMN.Item.BERRY_POUCH));
            Assert.IsFalse(itemPC.ValidItems.Contains(PKMN.Item.TM_CASE));

            Assert.IsFalse(itemPC.ValidItemNames.Contains("Berry Pouch"));
            Assert.IsFalse(itemPC.ValidItemNames.Contains("TM Case"));
        }
        else
        {
            Assert.AreEqual(itemPC.ValidItems.Count, fullItemList.Count);
        }
    }

    public static void ItemBagTest(
        PKMN.ItemBag itemBag,
        PKMN.Game game
    )
    {
        string tmhmPocketName = "";
        string berryPocketName = "";
        if((game == PKMN.Game.FIRERED) || (game == PKMN.Game.LEAFGREEN))
        {
            tmhmPocketName = "TM Case";
            berryPocketName = "Berry Pouch";
        }
        else
        {
            tmhmPocketName = "TMs & HMs";
            berryPocketName = "Berries";
        }

        // Check unchanging and initial values.
        Assert.AreEqual(itemBag.Game, game);
        Assert.AreEqual(itemBag.Count, 5);

        ItemPocketTest(itemBag["Items"], game);
        KeyItemPocketTest(itemBag["Key Items"], game);
        BallPocketTest(itemBag["Poké Balls"], game);
        TMHMPocketTest(itemBag[tmhmPocketName], game);
        BerryPocketTest(itemBag[berryPocketName], game);

        // Make sure adding items through the bag adds to the proper pockets.
        Assert.AreEqual(itemBag["Items"].NumItems, 0);
        Assert.AreEqual(itemBag["Key Items"].NumItems, 0);
        Assert.AreEqual(itemBag["Poké Balls"].NumItems, 0);
        Assert.AreEqual(itemBag[tmhmPocketName].NumItems, 0);
        Assert.AreEqual(itemBag[berryPocketName].NumItems, 0);
        foreach(PKMN.Item item in AllPocketItems)
        {
            itemBag.Add(item, 5);
        }

        Assert.AreEqual(itemBag["Items"][0].Item, PKMN.Item.POTION);
        Assert.AreEqual(itemBag["Items"][0].Amount, 5);
        Assert.AreEqual(itemBag["Items"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Items"][1].Amount, 0);

        Assert.AreEqual(itemBag["Key Items"][0].Item, PKMN.Item.MACH_BIKE);
        Assert.AreEqual(itemBag["Key Items"][0].Amount, 5);
        Assert.AreEqual(itemBag["Key Items"][1].Item, PKMN.Item.WAILMER_PAIL);
        Assert.AreEqual(itemBag["Key Items"][1].Amount, 5);
        Assert.AreEqual(itemBag["Key Items"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Key Items"][2].Amount, 0);

        Assert.AreEqual(itemBag["Poké Balls"][0].Item, PKMN.Item.GREAT_BALL);
        Assert.AreEqual(itemBag["Poké Balls"][0].Amount, 5);
        Assert.AreEqual(itemBag["Poké Balls"][1].Item, PKMN.Item.MASTER_BALL);
        Assert.AreEqual(itemBag["Poké Balls"][1].Amount, 5);
        Assert.AreEqual(itemBag["Poké Balls"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Poké Balls"][2].Amount, 0);

        Assert.AreEqual(itemBag[tmhmPocketName][0].Item, PKMN.Item.TM01);
        Assert.AreEqual(itemBag[tmhmPocketName][0].Amount, 5);
        Assert.AreEqual(itemBag[tmhmPocketName][1].Item, PKMN.Item.HM04);
        Assert.AreEqual(itemBag[tmhmPocketName][1].Amount, 5);
        Assert.AreEqual(itemBag[tmhmPocketName][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag[tmhmPocketName][2].Amount, 0);

        Assert.AreEqual(itemBag[berryPocketName][0].Item, PKMN.Item.ASPEAR_BERRY);
        Assert.AreEqual(itemBag[berryPocketName][0].Amount, 5);
        Assert.AreEqual(itemBag[berryPocketName][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag[berryPocketName][1].Amount, 0);

        // Make sure removing items through the bag removes from the proper pockets.
        foreach(PKMN.Item item in AllPocketItems)
        {
            itemBag.Remove(item, 5);
        }

        Assert.AreEqual(itemBag["Items"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Items"][0].Amount, 0);
        Assert.AreEqual(itemBag["Items"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Items"][1].Amount, 0);

        Assert.AreEqual(itemBag["Key Items"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Key Items"][0].Amount, 0);
        Assert.AreEqual(itemBag["Key Items"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Key Items"][1].Amount, 0);
        Assert.AreEqual(itemBag["Key Items"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Key Items"][2].Amount, 0);

        Assert.AreEqual(itemBag["Poké Balls"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Poké Balls"][0].Amount, 0);
        Assert.AreEqual(itemBag["Poké Balls"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Poké Balls"][1].Amount, 0);
        Assert.AreEqual(itemBag["Poké Balls"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Poké Balls"][2].Amount, 0);

        Assert.AreEqual(itemBag[tmhmPocketName][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag[tmhmPocketName][0].Amount, 0);
        Assert.AreEqual(itemBag[tmhmPocketName][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag[tmhmPocketName][1].Amount, 0);
        Assert.AreEqual(itemBag[tmhmPocketName][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag[tmhmPocketName][2].Amount, 0);

        Assert.AreEqual(itemBag[berryPocketName][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag[berryPocketName][0].Amount, 0);
        Assert.AreEqual(itemBag[berryPocketName][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag[berryPocketName][1].Amount, 0);

        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            WrongGameAllPocketItems
        );
    }
}

}
