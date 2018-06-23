/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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
    private static string[] ColosseumAllPocketItems =
    {
        "Potion", "Ein File S", "Great Ball", "TM01",
        "TM02", "Aspear Berry", "Joy Scent", "Excite Scent"
    };
    private static string[] XDAllPocketItems =
    {
        "Potion", "Gonzap's Key", "Great Ball", "TM01",
        "TM02", "Aspear Berry", "Joy Scent", "Battle CD 01"
    };
    private static string[] ColosseumWrongGameAllPocketItems =
    {
        "Pink Bow", "Black Sludge",
        "GS Ball", "Gonzap's Key", "Poffin Items",
        "TM51", "HM01",
        "Berry", "Occa Berry"
    };
    private static string[] XDWrongGameAllPocketItems =
    {
        "Pink Bow", "Black Sludge",
        "GS Ball", "Ein File S", "Poffin Items",
        "TM51", "HM01",
        "Berry", "Occa Berry",
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
            "Potion"
        );

        // Confirm items from other pockets can't be added.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                itemPocket,
                new string[]{"Ein File S", "Great Ball", "TM01", "Oran Berry", "Joy Scent"}
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                itemPocket,
                new string[]{"Gonzap's Key", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"}
            );
        }

        // Confirm items from other generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new string[]{"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        string[] validItems = {"Potion", "Orange Mail", "Lava Cookie", "Stardust",
                               "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"};
        ItemsTestsCommon.TestItemListSettingItems(
            itemPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            itemPocket,
            validItems
        );

        Assert.Greater(itemPocket.ValidItems.Count, 0);
    }

    public static void KeyItemPocketTest(
        PKMN.ItemList keyItemPocket,
        PKMN.Game game
    )
    {
        bool colosseum = (game == PKMN.Game.COLOSSEUM);
        string keyItem = colosseum ? "Ein File S" : "Gonzap's Key";
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
                new string[]{"Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent"}
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new string[]{"Potion", "Great Ball", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"}
            );
        }

        // Confirm items from other generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"}
        );

        // Confirm items from incompatible Generation III games can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"Helix Fossil", "Tea", "Ruby"}
        );
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"Magma Emblem", "Old Sea Map"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListSettingItems(
                keyItemPocket,
                new string[]{"Jail Key", "Elevator Key", "Small Tablet", "F-Disk",
                             "R-Disk", "L-Disk", "D-Disk", "U-Disk"}
            );
            ItemsTestsCommon.TestItemListAddingAndRemovingItems(
                keyItemPocket,
                new string[]{"Jail Key", "Elevator Key", "Small Tablet", "F-Disk",
                             "R-Disk", "L-Disk", "D-Disk", "U-Disk"}
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListSettingItems(
                keyItemPocket,
                new string[]{"Krane Memo 1", "Krane Memo 2", "Krane Memo 3", "Krane Memo 4",
                             "Krane Memo 5", "Voice Case 1", "Voice Case 2", "Voice Case 3"}
            );
            ItemsTestsCommon.TestItemListAddingAndRemovingItems(
                keyItemPocket,
                new string[]{"Krane Memo 1", "Krane Memo 2", "Krane Memo 3", "Krane Memo 4",
                             "Krane Memo 5", "Voice Case 1", "Voice Case 2", "Voice Case 3"}
            );
        }

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
            "Master Ball"
        );

        // Confirm items from other pockets can't be added.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                ballPocket,
                new string[]{"Ein File S", "Potion", "TM01", "Oran Berry", "Joy Scent"}
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                ballPocket,
                new string[]{"Gonzap's Key", "Potion", "TM01", "Oran Berry", "Joy Scent", "Battle CD 01"}
            );
        }

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            ballPocket,
            new string[]{"Moon Ball", "Heal Ball", "Dream Ball"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        string[] validItems = {"Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
                               "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"};
        ItemsTestsCommon.TestItemListSettingItems(
            ballPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            ballPocket,
            validItems
        );

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
            "TM01"
        );

        // Confirm items from other pockets can't be added.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                tmPocket,
                new string[]{"Ein File S", "Potion", "Master Ball", "Oran Berry", "Joy Scent"}
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                tmPocket,
                new string[]{"Gonzap's Key", "Potion", "Master Ball", "Oran Berry", "Joy Scent", "Battle CD 01"}
            );
        }

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmPocket,
            new string[]{"TM51", "HM01"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        string[] validItems = {"TM01", "TM02", "TM03", "TM04",
                               "TM05", "TM06", "TM07", "TM08"};
        ItemsTestsCommon.TestItemListSettingItems(
            tmPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            tmPocket,
            validItems
        );

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
            "Razz Berry"
        );

        // Confirm items from other pockets can't be added.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                berryPocket,
                new string[]{"Ein File S", "Potion", "Master Ball", "TM01", "Joy Scent"}
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                berryPocket,
                new string[]{"Gonzap's Key", "Potion", "Master Ball", "TM01", "Joy Scent", "Battle CD 01"}
            );
        }

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            berryPocket,
            new string[]{"Berry", "Occa Berry", "Roseli Berry"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        string[] validItems = {"Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
                               "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"};
        ItemsTestsCommon.TestItemListSettingItems(
            berryPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            berryPocket,
            validItems
        );

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
            "Joy Scent"
        );

        // Confirm items from other pockets can't be added.
        if(colosseum)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                colognePocket,
                new string[]{"Ein File S", "Potion", "Master Ball", "TM01", "Oran Berry"}
            );
        }
        else
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                colognePocket,
                new string[]{"Gonzap's Key", "Potion", "Master Ball", "TM01", "Oran Berry", "Battle CD 01"}
            );
        }

        colognePocket.Add("Joy Scent", 3);
        colognePocket.Add("Excite Scent", 3);
        colognePocket.Add("Vivid Scent", 3);

        colognePocket.Remove("Excite Scent", 3);
        colognePocket.Remove("Vivid Scent", 1);

        Assert.AreEqual(colognePocket[0].Item, "Joy Scent");
        Assert.AreEqual(colognePocket[0].Amount, 3);
        Assert.AreEqual(colognePocket[1].Item, "Vivid Scent");
        Assert.AreEqual(colognePocket[1].Amount, 2);
        Assert.AreEqual(colognePocket[2].Item, "None");
        Assert.AreEqual(colognePocket[2].Amount, 0);

        colognePocket.Remove("Joy Scent", 3);
        colognePocket.Remove("Vivid Scent", 2);

        Assert.AreEqual(colognePocket[0].Item, "None");
        Assert.AreEqual(colognePocket[0].Amount, 0);
        Assert.AreEqual(colognePocket[1].Item, "None");
        Assert.AreEqual(colognePocket[1].Amount, 0);
        Assert.AreEqual(colognePocket[2].Item, "None");
        Assert.AreEqual(colognePocket[2].Amount, 0);

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
            "Battle CD 01"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            battleCDPocket,
            new string[]{"Gonzap's Key", "Potion", "Master Ball", "TM01", "Joy Scent", "Oran Berry"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        string[] validItems = {"Battle CD 01", "Battle CD 02", "Battle CD 03", "Battle CD 04",
                               "Battle CD 05", "Battle CD 06", "Battle CD 07", "Battle CD 08"};
        ItemsTestsCommon.TestItemListSettingItems(
            battleCDPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            battleCDPocket,
            validItems
        );

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
            "Potion"
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

        PKMN.StringList fullItemList = PKMN.Database.Lists.ItemList(game);
        Assert.AreEqual(itemPC.ValidItems.Count, fullItemList.Count);
    }

    public static void ItemBagTest(
        PKMN.ItemBag itemBag,
        PKMN.Game game
    )
    {
        bool colosseum = (game == PKMN.Game.COLOSSEUM);
        string keyItem = colosseum ? "Ein File S" : "Gonzap's Key";

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
        foreach(string itemName in (colosseum ? ColosseumAllPocketItems : XDAllPocketItems)) {
            itemBag.Add(itemName, 5);
        }

        Assert.AreEqual(itemBag["Items"][0].Item, "Potion");
        Assert.AreEqual(itemBag["Items"][0].Amount, 5);
        Assert.AreEqual(itemBag["Items"][1].Item, "None");
        Assert.AreEqual(itemBag["Items"][1].Amount, 0);

        Assert.AreEqual(itemBag["Key Items"][0].Item, keyItem);
        Assert.AreEqual(itemBag["Key Items"][0].Amount, 5);
        Assert.AreEqual(itemBag["Key Items"][1].Item, "None");
        Assert.AreEqual(itemBag["Key Items"][1].Amount, 0);

        Assert.AreEqual(itemBag["Poké Balls"][0].Item, "Great Ball");
        Assert.AreEqual(itemBag["Poké Balls"][0].Amount, 5);
        Assert.AreEqual(itemBag["Poké Balls"][1].Item, "None");
        Assert.AreEqual(itemBag["Poké Balls"][1].Amount, 0);

        Assert.AreEqual(itemBag["TMs"][0].Item, "TM01");
        Assert.AreEqual(itemBag["TMs"][0].Amount, 5);
        Assert.AreEqual(itemBag["TMs"][1].Item, "TM02");
        Assert.AreEqual(itemBag["TMs"][1].Amount, 5);
        Assert.AreEqual(itemBag["TMs"][2].Item, "None");
        Assert.AreEqual(itemBag["TMs"][2].Amount, 0);

        Assert.AreEqual(itemBag["Berries"][0].Item, "Aspear Berry");
        Assert.AreEqual(itemBag["Berries"][0].Amount, 5);
        Assert.AreEqual(itemBag["Berries"][1].Item, "None");
        Assert.AreEqual(itemBag["Berries"][1].Amount, 0);

        Assert.AreEqual(itemBag["Colognes"][0].Item, "Joy Scent");
        Assert.AreEqual(itemBag["Colognes"][0].Amount, 5);

        if(colosseum)
        {
            Assert.AreEqual(itemBag["Colognes"][1].Item, "Excite Scent");
            Assert.AreEqual(itemBag["Colognes"][1].Amount, 5);
            Assert.AreEqual(itemBag["Colognes"][2].Item, "None");
            Assert.AreEqual(itemBag["Colognes"][2].Amount, 0);
        }
        else
        {
            Assert.AreEqual(itemBag["Colognes"][1].Item, "None");
            Assert.AreEqual(itemBag["Colognes"][1].Amount, 0);

            Assert.AreEqual(itemBag["Battle CDs"][0].Item, "Battle CD 01");
            Assert.AreEqual(itemBag["Battle CDs"][0].Amount, 5);
        }

        // Make sure removing items through the bag removes from the proper pockets.
        foreach(string itemName in (colosseum ? ColosseumAllPocketItems : XDAllPocketItems)) {
            itemBag.Remove(itemName, 5);
        }

        Assert.AreEqual(itemBag["Items"][0].Item, "None");
        Assert.AreEqual(itemBag["Items"][0].Amount, 0);
        Assert.AreEqual(itemBag["Items"][1].Item, "None");
        Assert.AreEqual(itemBag["Items"][1].Amount, 0);

        Assert.AreEqual(itemBag["Key Items"][0].Item, "None");
        Assert.AreEqual(itemBag["Key Items"][0].Amount, 0);
        Assert.AreEqual(itemBag["Key Items"][1].Item, "None");
        Assert.AreEqual(itemBag["Key Items"][1].Amount, 0);

        Assert.AreEqual(itemBag["Poké Balls"][0].Item, "None");
        Assert.AreEqual(itemBag["Poké Balls"][0].Amount, 0);
        Assert.AreEqual(itemBag["Poké Balls"][1].Item, "None");
        Assert.AreEqual(itemBag["Poké Balls"][1].Amount, 0);

        Assert.AreEqual(itemBag["TMs"][0].Item, "None");
        Assert.AreEqual(itemBag["TMs"][0].Amount, 0);
        Assert.AreEqual(itemBag["TMs"][1].Item, "None");
        Assert.AreEqual(itemBag["TMs"][1].Amount, 0);
        Assert.AreEqual(itemBag["TMs"][2].Item, "None");
        Assert.AreEqual(itemBag["TMs"][2].Amount, 0);

        Assert.AreEqual(itemBag["Berries"][0].Item, "None");
        Assert.AreEqual(itemBag["Berries"][0].Amount, 0);
        Assert.AreEqual(itemBag["Berries"][1].Item, "None");
        Assert.AreEqual(itemBag["Berries"][1].Amount, 0);

        Assert.AreEqual(itemBag["Colognes"][0].Item, "None");
        Assert.AreEqual(itemBag["Colognes"][0].Amount, 0);

        if(colosseum)
        {
            Assert.AreEqual(itemBag["Colognes"][1].Item, "None");
            Assert.AreEqual(itemBag["Colognes"][1].Amount, 0);
            Assert.AreEqual(itemBag["Colognes"][2].Item, "None");
            Assert.AreEqual(itemBag["Colognes"][2].Amount, 0);
        }
        else
        {
            Assert.AreEqual(itemBag["Colognes"][1].Item, "None");
            Assert.AreEqual(itemBag["Colognes"][1].Amount, 0);

            Assert.AreEqual(itemBag["Battle CDs"][0].Item, "None");
            Assert.AreEqual(itemBag["Battle CDs"][0].Amount, 0);
        }

        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            colosseum ? ColosseumWrongGameAllPocketItems : XDWrongGameAllPocketItems
        );
    }
}

}
