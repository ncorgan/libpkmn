/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class GCNItemsTest
{
    private static PKMN.Item[] ColosseumAllPocketItems =
    {
        PKMN.Item.POTION,
        PKMN.Item.EIN_FILE_S,
        PKMN.Item.GREAT_BALL,
        PKMN.Item.TM01,
        PKMN.Item.TM02,
        PKMN.Item.ASPEAR_BERRY,
        PKMN.Item.JOY_SCENT,
        PKMN.Item.EXCITE_SCENT
    };
    private static PKMN.Item[] XDAllPocketItems =
    {
        PKMN.Item.POTION,
        PKMN.Item.GONZAPS_KEY,
        PKMN.Item.GREAT_BALL,
        PKMN.Item.TM01,
        PKMN.Item.TM02,
        PKMN.Item.ASPEAR_BERRY,
        PKMN.Item.JOY_SCENT,
        PKMN.Item.BATTLE_CD_01
    };

    private static PKMN.Item[] ColosseumWrongGameAllPocketItems =
    {
        PKMN.Item.PINK_BOW, PKMN.Item.BLACK_SLUDGE,
        PKMN.Item.GS_BALL, PKMN.Item.GONZAPS_KEY, PKMN.Item.POFFIN_CASE,
        PKMN.Item.TM51, PKMN.Item.HM01,
        PKMN.Item.BERRY, PKMN.Item.OCCA_BERRY
    };
    private static PKMN.Item[] XDWrongGameAllPocketItems =
    {
        PKMN.Item.PINK_BOW, PKMN.Item.BLACK_SLUDGE,
        PKMN.Item.GS_BALL, PKMN.Item.EIN_FILE_S, PKMN.Item.POFFIN_CASE,
        PKMN.Item.TM51, PKMN.Item.HM01,
        PKMN.Item.BERRY, PKMN.Item.OCCA_BERRY
    };

    public static void ItemPocketTest(
        PKMN.ItemList itemPocket,
        PKMN.Game game
    )
    {
        bool colosseum = (game == PKMN.Game.COLOSSEUM);
        int expectedLength = colosseum ? 20 : 30;

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
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                itemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.EIN_FILE_S,
                        PKMN.Item.GREAT_BALL,
                        PKMN.Item.TM01,
                        PKMN.Item.ORAN_BERRY,
                        PKMN.Item.JOY_SCENT
                    }
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                itemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.GONZAPS_KEY,
                        PKMN.Item.GREAT_BALL,
                        PKMN.Item.TM01,
                        PKMN.Item.ORAN_BERRY,
                        PKMN.Item.BATTLE_CD_01
                    }
            );
        }

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
        bool colosseum = (game == PKMN.Game.COLOSSEUM);
        PKMN.Item keyItem = colosseum ? PKMN.Item.EIN_FILE_S : PKMN.Item.GONZAPS_KEY;
        int expectedLength = 43;

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
            keyItem
        );

        // Confirm items from other pockets can't be added.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.POTION,
                        PKMN.Item.GREAT_BALL,
                        PKMN.Item.TM01,
                        PKMN.Item.ORAN_BERRY,
                        PKMN.Item.JOY_SCENT
                    }
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.POTION,
                        PKMN.Item.GREAT_BALL,
                        PKMN.Item.TM01,
                        PKMN.Item.ORAN_BERRY,
                        PKMN.Item.JOY_SCENT,
                        PKMN.Item.BATTLE_CD_01
                    }
            );
        }

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
                    PKMN.Item.HELIX_FOSSIL,
                    PKMN.Item.TEA,
                    PKMN.Item.RUBY
                }
        );
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.MAGMA_EMBLEM,
                    PKMN.Item.OLD_SEA_MAP
                }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListSettingItems(
                keyItemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.JAIL_KEY,
                        PKMN.Item.ELEVATOR_KEY,
                        PKMN.Item.SMALL_TABLET,
                        PKMN.Item.F_DISK,
                        PKMN.Item.R_DISK,
                        PKMN.Item.L_DISK,
                        PKMN.Item.D_DISK,
                        PKMN.Item.U_DISK
                    }
            );
            ItemsTestsCommon.TestItemListAddingAndRemovingItems(
                keyItemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.JAIL_KEY,
                        PKMN.Item.ELEVATOR_KEY,
                        PKMN.Item.SMALL_TABLET,
                        PKMN.Item.F_DISK,
                        PKMN.Item.R_DISK,
                        PKMN.Item.L_DISK,
                        PKMN.Item.D_DISK,
                        PKMN.Item.U_DISK
                    }
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListSettingItems(
                keyItemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.KRANE_MEMO_1,
                        PKMN.Item.KRANE_MEMO_2,
                        PKMN.Item.KRANE_MEMO_3,
                        PKMN.Item.KRANE_MEMO_4,
                        PKMN.Item.KRANE_MEMO_5,
                        PKMN.Item.VOICE_CASE_1,
                        PKMN.Item.VOICE_CASE_2,
                        PKMN.Item.VOICE_CASE_3
                    }
            );
            ItemsTestsCommon.TestItemListAddingAndRemovingItems(
                keyItemPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.KRANE_MEMO_1,
                        PKMN.Item.KRANE_MEMO_2,
                        PKMN.Item.KRANE_MEMO_3,
                        PKMN.Item.KRANE_MEMO_4,
                        PKMN.Item.KRANE_MEMO_5,
                        PKMN.Item.VOICE_CASE_1,
                        PKMN.Item.VOICE_CASE_2,
                        PKMN.Item.VOICE_CASE_3
                    }
            );
        }

        Assert.AreEqual(keyItemPocket.ValidItems.Count, keyItemPocket.ValidItemNames.Count);
        Assert.Greater(keyItemPocket.ValidItems.Count, 0);
    }

    public static void BallPocketTest(
        PKMN.ItemList ballPocket,
        PKMN.Game game
    )
    {
        bool colosseum = (game == PKMN.Game.COLOSSEUM);
        int expectedLength = 16;

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
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                ballPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.EIN_FILE_S,
                        PKMN.Item.POTION,
                        PKMN.Item.TM01,
                        PKMN.Item.ORAN_BERRY,
                        PKMN.Item.JOY_SCENT
                    }
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                ballPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.GONZAPS_KEY,
                        PKMN.Item.POTION,
                        PKMN.Item.TM01,
                        PKMN.Item.ORAN_BERRY,
                        PKMN.Item.JOY_SCENT,
                        PKMN.Item.BATTLE_CD_01
                    }
            );
        }

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

    public static void TMPocketTest(
        PKMN.ItemList tmPocket,
        PKMN.Game game
    )
    {
        bool colosseum = (game == PKMN.Game.COLOSSEUM);
        int expectedLength = 64;

        // Check unchanging and initial values.
        Assert.AreEqual(tmPocket.Name, "TMs");
        Assert.AreEqual(tmPocket.Game, game);
        Assert.AreEqual(tmPocket.Length, expectedLength);
        Assert.AreEqual(tmPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(tmPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            tmPocket,
            PKMN.Item.TM01
        );

        // Confirm items from other pockets can't be added.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                tmPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.EIN_FILE_S,
                        PKMN.Item.POTION,
                        PKMN.Item.MASTER_BALL,
                        PKMN.Item.ORAN_BERRY,
                        PKMN.Item.JOY_SCENT
                    }
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                tmPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.GONZAPS_KEY,
                        PKMN.Item.POTION,
                        PKMN.Item.MASTER_BALL,
                        PKMN.Item.ORAN_BERRY,
                        PKMN.Item.JOY_SCENT,
                        PKMN.Item.BATTLE_CD_01
                    }
            );
        }

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmPocket,
            new PKMN.Item[]
            {
                PKMN.Item.TM51,
                PKMN.Item.HM01
            }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        PKMN.Item[] validItems =
        {
            PKMN.Item.TM01,
            PKMN.Item.TM02,
            PKMN.Item.TM03,
            PKMN.Item.TM04,
            PKMN.Item.TM05,
            PKMN.Item.TM06,
            PKMN.Item.TM07,
            PKMN.Item.TM08
        };
        ItemsTestsCommon.TestItemListSettingItems(
            tmPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            tmPocket,
            validItems
        );

        Assert.AreEqual(tmPocket.ValidItems.Count, tmPocket.ValidItemNames.Count);
        Assert.AreEqual(tmPocket.ValidItems.Count, 50);
    }

    public static void BerryPocketTest(
        PKMN.ItemList berryPocket,
        PKMN.Game game
    )
    {
        bool colosseum = (game == PKMN.Game.COLOSSEUM);
        int expectedLength = 46;

        // Check unchanging and initial values.
        Assert.AreEqual(berryPocket.Name, "Berries");
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
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                berryPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.EIN_FILE_S,
                        PKMN.Item.POTION,
                        PKMN.Item.MASTER_BALL,
                        PKMN.Item.TM01,
                        PKMN.Item.JOY_SCENT
                    }
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                berryPocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.GONZAPS_KEY,
                        PKMN.Item.POTION,
                        PKMN.Item.MASTER_BALL,
                        PKMN.Item.TM01,
                        PKMN.Item.JOY_SCENT,
                        PKMN.Item.BATTLE_CD_01
                    }
            );
        }

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

    public static void ColognePocketTest(
        PKMN.ItemList colognePocket,
        PKMN.Game game
    )
    {
        bool colosseum = (game == PKMN.Game.COLOSSEUM);
        int expectedLength = 3;

        // Check unchanging and initial values.
        Assert.AreEqual(colognePocket.Name, "Colognes");
        Assert.AreEqual(colognePocket.Game, game);
        Assert.AreEqual(colognePocket.Length, expectedLength);
        Assert.AreEqual(colognePocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(colognePocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            colognePocket,
            PKMN.Item.JOY_SCENT
        );

        // Confirm items from other pockets can't be added.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                colognePocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.EIN_FILE_S,
                        PKMN.Item.POTION,
                        PKMN.Item.MASTER_BALL,
                        PKMN.Item.TM01,
                        PKMN.Item.ORAN_BERRY
                    }
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                colognePocket,
                new PKMN.Item[]
                    {
                        PKMN.Item.GONZAPS_KEY,
                        PKMN.Item.POTION,
                        PKMN.Item.MASTER_BALL,
                        PKMN.Item.TM01,
                        PKMN.Item.ORAN_BERRY,
                        PKMN.Item.BATTLE_CD_01
                    }
            );
        }

        colognePocket.Add(PKMN.Item.JOY_SCENT, 3);
        colognePocket.Add(PKMN.Item.EXCITE_SCENT, 3);
        colognePocket.Add(PKMN.Item.VIVID_SCENT, 3);

        colognePocket.Remove(PKMN.Item.EXCITE_SCENT, 3);
        colognePocket.Remove(PKMN.Item.VIVID_SCENT, 1);

        Assert.AreEqual(colognePocket[0].Item, PKMN.Item.JOY_SCENT);
        Assert.AreEqual(colognePocket[0].Amount, 3);
        Assert.AreEqual(colognePocket[1].Item, PKMN.Item.VIVID_SCENT);
        Assert.AreEqual(colognePocket[1].Amount, 2);
        Assert.AreEqual(colognePocket[2].Item, PKMN.Item.NONE);
        Assert.AreEqual(colognePocket[2].Amount, 0);

        colognePocket.Remove(PKMN.Item.JOY_SCENT, 3);
        colognePocket.Remove(PKMN.Item.VIVID_SCENT, 2);

        Assert.AreEqual(colognePocket[0].Item, PKMN.Item.NONE);
        Assert.AreEqual(colognePocket[0].Amount, 0);
        Assert.AreEqual(colognePocket[1].Item, PKMN.Item.NONE);
        Assert.AreEqual(colognePocket[1].Amount, 0);
        Assert.AreEqual(colognePocket[2].Item, PKMN.Item.NONE);
        Assert.AreEqual(colognePocket[2].Amount, 0);

        Assert.AreEqual(colognePocket.ValidItems.Count, colognePocket.ValidItemNames.Count);
        Assert.AreEqual(colognePocket.ValidItems.Count, 3);
    }

    public static void BattleCDPocketTest(
        PKMN.ItemList battleCDPocket,
        PKMN.Game game
    ) {
        int expectedLength = 60;

        // Check unchanging and initial values.
        Assert.AreEqual(battleCDPocket.Name, "Battle CDs");
        Assert.AreEqual(battleCDPocket.Game, game);
        Assert.AreEqual(battleCDPocket.Length, expectedLength);
        Assert.AreEqual(battleCDPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(battleCDPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            battleCDPocket,
            PKMN.Item.BATTLE_CD_01
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            battleCDPocket,
            new PKMN.Item[]
                {
                    PKMN.Item.GONZAPS_KEY,
                    PKMN.Item.POTION,
                    PKMN.Item.MASTER_BALL,
                    PKMN.Item.TM01,
                    PKMN.Item.JOY_SCENT,
                    PKMN.Item.ORAN_BERRY,
                }
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        PKMN.Item[] validItems =
        {
            PKMN.Item.BATTLE_CD_01,
            PKMN.Item.BATTLE_CD_02,
            PKMN.Item.BATTLE_CD_03,
            PKMN.Item.BATTLE_CD_04,
            PKMN.Item.BATTLE_CD_05,
            PKMN.Item.BATTLE_CD_06,
            PKMN.Item.BATTLE_CD_07,
            PKMN.Item.BATTLE_CD_08
        };
        ItemsTestsCommon.TestItemListSettingItems(
            battleCDPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            battleCDPocket,
            validItems
        );

        Assert.AreEqual(battleCDPocket.ValidItems.Count, battleCDPocket.ValidItemNames.Count);
        Assert.AreEqual(battleCDPocket.ValidItems.Count, 60);
    }

    public static void ItemPCTest(
        PKMN.ItemList itemPC,
        PKMN.Game game
    )
    {
        bool colosseum = (game == PKMN.Game.COLOSSEUM);

        // Check unchanging and initial values.
        Assert.AreEqual(itemPC.Name, "PC");
        Assert.AreEqual(itemPC.Game, game);
        Assert.AreEqual(itemPC.Length, 235);
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
            colosseum ? ColosseumWrongGameAllPocketItems : XDWrongGameAllPocketItems
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListSettingItems(
            itemPC,
            colosseum ? ColosseumAllPocketItems : XDAllPocketItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            itemPC,
            colosseum ? ColosseumAllPocketItems : XDAllPocketItems
        );

        PKMN.ItemEnumList fullItemList = PKMN.Database.Lists.ItemList(game);
        PKMN.StringList fullItemNameList = PKMN.Database.Lists.ItemNameList(game);

        Assert.AreEqual(itemPC.ValidItems.Count, itemPC.ValidItemNames.Count);
        Assert.AreEqual(itemPC.ValidItems.Count, fullItemList.Count);
        Assert.AreEqual(itemPC.ValidItemNames.Count, fullItemNameList.Count);
    }

    public static void ItemBagTest(
        PKMN.ItemBag itemBag,
        PKMN.Game game
    )
    {
        bool colosseum = (game == PKMN.Game.COLOSSEUM);
        PKMN.Item keyItem = colosseum ? PKMN.Item.EIN_FILE_S : PKMN.Item.GONZAPS_KEY;

        // Check unchanging and initial values.
        Assert.AreEqual(itemBag.Game, game);
        Assert.AreEqual(itemBag.Count, (colosseum ? 6 : 7));

        ItemPocketTest(itemBag["Items"], game);
        KeyItemPocketTest(itemBag["Key Items"], game);
        BallPocketTest(itemBag["Poké Balls"], game);
        TMPocketTest(itemBag["TMs"], game);
        BerryPocketTest(itemBag["Berries"], game);
        ColognePocketTest(itemBag["Colognes"], game);
        if(!colosseum)
        {
            BattleCDPocketTest(itemBag["Battle CDs"], game);
        }

        // Make sure adding items through the bag adds to the proper pockets.
        Assert.AreEqual(itemBag["Items"].NumItems, 0);
        Assert.AreEqual(itemBag["Key Items"].NumItems, 0);
        Assert.AreEqual(itemBag["Poké Balls"].NumItems, 0);
        Assert.AreEqual(itemBag["TMs"].NumItems, 0);
        Assert.AreEqual(itemBag["Berries"].NumItems, 0);
        Assert.AreEqual(itemBag["Colognes"].NumItems, 0);
        if(!colosseum)
        {
            Assert.AreEqual(itemBag["Battle CDs"].NumItems, 0);
        }
        foreach(PKMN.Item item in (colosseum ? ColosseumAllPocketItems : XDAllPocketItems))
        {
            itemBag.Add(item, 5);
        }

        Assert.AreEqual(itemBag["Items"][0].Item, PKMN.Item.POTION);
        Assert.AreEqual(itemBag["Items"][0].Amount, 5);
        Assert.AreEqual(itemBag["Items"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Items"][1].Amount, 0);

        Assert.AreEqual(itemBag["Key Items"][0].Item, keyItem);
        Assert.AreEqual(itemBag["Key Items"][0].Amount, 5);
        Assert.AreEqual(itemBag["Key Items"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Key Items"][1].Amount, 0);

        Assert.AreEqual(itemBag["Poké Balls"][0].Item, PKMN.Item.GREAT_BALL);
        Assert.AreEqual(itemBag["Poké Balls"][0].Amount, 5);
        Assert.AreEqual(itemBag["Poké Balls"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Poké Balls"][1].Amount, 0);

        Assert.AreEqual(itemBag["TMs"][0].Item, PKMN.Item.TM01);
        Assert.AreEqual(itemBag["TMs"][0].Amount, 5);
        Assert.AreEqual(itemBag["TMs"][1].Item, PKMN.Item.TM02);
        Assert.AreEqual(itemBag["TMs"][1].Amount, 5);
        Assert.AreEqual(itemBag["TMs"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["TMs"][2].Amount, 0);

        Assert.AreEqual(itemBag["Berries"][0].Item, PKMN.Item.ASPEAR_BERRY);
        Assert.AreEqual(itemBag["Berries"][0].Amount, 5);
        Assert.AreEqual(itemBag["Berries"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Berries"][1].Amount, 0);

        Assert.AreEqual(itemBag["Colognes"][0].Item, PKMN.Item.JOY_SCENT);
        Assert.AreEqual(itemBag["Colognes"][0].Amount, 5);

        if(colosseum)
        {
            Assert.AreEqual(itemBag["Colognes"][1].Item, PKMN.Item.EXCITE_SCENT);
            Assert.AreEqual(itemBag["Colognes"][1].Amount, 5);
            Assert.AreEqual(itemBag["Colognes"][2].Item, PKMN.Item.NONE);
            Assert.AreEqual(itemBag["Colognes"][2].Amount, 0);
        }
        else
        {
            Assert.AreEqual(itemBag["Colognes"][1].Item, PKMN.Item.NONE);
            Assert.AreEqual(itemBag["Colognes"][1].Amount, 0);

            Assert.AreEqual(itemBag["Battle CDs"][0].Item, PKMN.Item.BATTLE_CD_01);
            Assert.AreEqual(itemBag["Battle CDs"][0].Amount, 5);
        }

        // Make sure removing items through the bag removes from the proper pockets.
        foreach(PKMN.Item item in (colosseum ? ColosseumAllPocketItems : XDAllPocketItems))
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

        Assert.AreEqual(itemBag["Poké Balls"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Poké Balls"][0].Amount, 0);
        Assert.AreEqual(itemBag["Poké Balls"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Poké Balls"][1].Amount, 0);

        Assert.AreEqual(itemBag["TMs"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["TMs"][0].Amount, 0);
        Assert.AreEqual(itemBag["TMs"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["TMs"][1].Amount, 0);
        Assert.AreEqual(itemBag["TMs"][2].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["TMs"][2].Amount, 0);

        Assert.AreEqual(itemBag["Berries"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Berries"][0].Amount, 0);
        Assert.AreEqual(itemBag["Berries"][1].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Berries"][1].Amount, 0);

        Assert.AreEqual(itemBag["Colognes"][0].Item, PKMN.Item.NONE);
        Assert.AreEqual(itemBag["Colognes"][0].Amount, 0);

        if(colosseum)
        {
            Assert.AreEqual(itemBag["Colognes"][1].Item, PKMN.Item.NONE);
            Assert.AreEqual(itemBag["Colognes"][1].Amount, 0);
            Assert.AreEqual(itemBag["Colognes"][2].Item, PKMN.Item.NONE);
            Assert.AreEqual(itemBag["Colognes"][2].Amount, 0);
        }
        else
        {
            Assert.AreEqual(itemBag["Colognes"][1].Item, PKMN.Item.NONE);
            Assert.AreEqual(itemBag["Colognes"][1].Amount, 0);

            Assert.AreEqual(itemBag["Battle CDs"][0].Item, PKMN.Item.NONE);
            Assert.AreEqual(itemBag["Battle CDs"][0].Amount, 0);
        }

        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            colosseum ? ColosseumWrongGameAllPocketItems : XDWrongGameAllPocketItems
        );
    }
}

}
