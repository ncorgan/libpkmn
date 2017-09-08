/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class GBAItemsTest {

    private static string[] AllPocketItemNames = {
        "Potion", "Mach Bike", "Great Ball", "TM01",
        "Aspear Berry", "Wailmer Pail", "Master Ball", "HM04"
    };
    private static string[] WrongGameAllPocketItemNames = {
        "Pink Bow", "Black Sludge",
        "Ein File S", "Gonzap's Key",
        "GS Ball", "Poffin Items",
        "TM51",
        "Berry", "Occa Berry"
    };

    public static void ItemPocketTest(
        PKMN.ItemList itemPocket,
        string game
    ) {
        int expectedCapacity = 0;
        if(game.Equals("Ruby") || game.Equals("Sapphire")) {
            expectedCapacity = 20;
        } else if(game.Equals("Emerald")) {
            expectedCapacity = 30;
        } else {
            expectedCapacity = 42;
        }

        // Check unchanging and initial values.
        Assert.AreEqual(itemPocket.Name, "Items");
        Assert.AreEqual(itemPocket.Game, game);
        Assert.AreEqual(itemPocket.Capacity, expectedCapacity);
        Assert.AreEqual(itemPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(itemPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            itemPocket,
            "Potion"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new string[]{"Bicycle", "Master Ball", "HM01", "Razz Berry"}
        );

        // Confirm items from other generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new string[]{"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"}
        );

        // Make sure we can't add items from Gamecube games.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new string[]{"Time Flute", "Poké Snack"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            itemPocket,
            new string[]{"Potion", "Orange Mail", "Lava Cookie", "Stardust",
                         "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"}
        );

        Assert.Greater(itemPocket.ValidItems.Count, 0);
    }

    public static void KeyItemPocketTest(
        PKMN.ItemList keyItemPocket,
        string game
    ) {
        int expectedCapacity = 0;
        if(game.Equals("Ruby") || game.Equals("Sapphire")) {
            expectedCapacity = 20;
        } else {
            expectedCapacity = 30;
        }

        // Check unchanging and initial values.
        Assert.AreEqual(keyItemPocket.Name, "Key Items");
        Assert.AreEqual(keyItemPocket.Game, game);
        Assert.AreEqual(keyItemPocket.Capacity, expectedCapacity);
        Assert.AreEqual(keyItemPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(keyItemPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            keyItemPocket,
            "Basement Key"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"Potion", "Master Ball", "HM01", "Razz Berry"}
        );

        // Confirm items from other generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"}
        );

        // Confirm items from incompatible Generation III games can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"Ein File S", "Powerup Part", "Gonzap's Key", "Krane Memo 1"}
        );
        if(game.Equals("Ruby") || game.Equals("Sapphire")) {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new string[]{"Helix Fossil", "Tea", "Ruby"}
            );
        }
        if(!game.Equals("Emerald")) {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new string[]{"Magma Emblem", "Old Sea Map"}
            );
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            keyItemPocket,
            new string[]{"Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
                         "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"}
        );

        Assert.Greater(keyItemPocket.ValidItems.Count, 0);
    }

    public static void BallPocketTest(
        PKMN.ItemList ballPocket,
        string game
    ) {
        int expectedCapacity = 0;
        if(game.Equals("FireRed") || game.Equals("LeafGreen")) {
            expectedCapacity = 13;
        } else {
            expectedCapacity = 16;
        }

        // Check unchanging and initial values.
        Assert.AreEqual(ballPocket.Name, "Poké Balls");
        Assert.AreEqual(ballPocket.Game, game);
        Assert.AreEqual(ballPocket.Capacity, expectedCapacity);
        Assert.AreEqual(ballPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(ballPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            ballPocket,
            "Master Ball"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            ballPocket,
            new string[]{"Potion", "Bicycle", "HM01", "Razz Berry"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            ballPocket,
            new string[]{"Moon Ball", "Heal Ball", "Dream Ball"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            ballPocket,
            new string[]{"Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
                         "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"}
        );

        Assert.Greater(ballPocket.ValidItems.Count, 0);
    }

    public static void TMHMPocketTest(
        PKMN.ItemList tmhmPocket,
        string game
    ) {
        int expectedCapacity = 0;
        string expectedName = "";

        if(game.Equals("FireRed") || game.Equals("LeafGreen")) {
            expectedCapacity = 58;
            expectedName = "TM Case";
        } else {
            expectedCapacity = 64;
            expectedName = "TMs & HMs";
        }

        // Check unchanging and initial values.
        Assert.AreEqual(tmhmPocket.Name, expectedName);
        Assert.AreEqual(tmhmPocket.Game, game);
        Assert.AreEqual(tmhmPocket.Capacity, expectedCapacity);
        Assert.AreEqual(tmhmPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(tmhmPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            tmhmPocket,
            "TM01"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmhmPocket,
            new string[]{"Potion", "Bicycle", "Great Ball", "Razz Berry"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmhmPocket,
            new string[]{"TM51"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            tmhmPocket,
            new string[]{"TM01", "HM01", "TM02", "HM02",
                         "TM03", "HM03", "TM04", "HM04"}
        );

        Assert.Greater(tmhmPocket.ValidItems.Count, 0);
    }

    public static void BerryPocketTest(
        PKMN.ItemList berryPocket,
        string game
    ) {
        int expectedCapacity = 0;
        string expectedName = "";

        if(game.Equals("FireRed") || game.Equals("LeafGreen")) {
            expectedCapacity = 43;
            expectedName = "Berry Pouch";
        } else {
            expectedCapacity = 46;
            expectedName = "Berries";
        }

        // Check unchanging and initial values.
        Assert.AreEqual(berryPocket.Name, expectedName);
        Assert.AreEqual(berryPocket.Game, game);
        Assert.AreEqual(berryPocket.Capacity, expectedCapacity);
        Assert.AreEqual(berryPocket.NumItems, 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(berryPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            berryPocket,
            "Razz Berry"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            berryPocket,
            new string[]{"Potion", "Bicycle", "Great Ball", "HM02"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            berryPocket,
            new string[]{"Berry", "Occa Berry", "Roseli Berry"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            berryPocket,
            new string[]{"Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
                         "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"}
        );

        Assert.Greater(berryPocket.ValidItems.Count, 0);
    }

    public static void ItemPCTest(
        PKMN.ItemList itemPC,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(itemPC.Name, "PC");
        Assert.AreEqual(itemPC.Game, game);
        Assert.AreEqual(itemPC.Capacity, 50);
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
            WrongGameAllPocketItemNames
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            itemPC,
            AllPocketItemNames
        );

        PKMN.StringList fullItemList = PKMN.Database.Lists.ItemList(game);
        Assert.AreEqual(itemPC.ValidItems.Count, fullItemList.Count);
    }

    public static void ItemBagTest(
        PKMN.ItemBag itemBag,
        string game
    ) {
        string tmhmPocketName = "";
        string berryPocketName = "";
        if(game.Equals("FireRed") || game.Equals("LeafGreen")) {
            tmhmPocketName = "TM Case";
            berryPocketName = "Berry Pouch";
        } else {
            tmhmPocketName = "TMs & HMs";
            berryPocketName = "Berries";
        }

        // Check unchanging and initial values.
        Assert.AreEqual(itemBag.Game, game);

        PKMN.ItemPockets ItemBag = itemBag.Pockets;
        Assert.AreEqual(ItemBag.Count, 5);

        ItemPocketTest(ItemBag["Items"], game);
        KeyItemPocketTest(ItemBag["Key Items"], game);
        BallPocketTest(ItemBag["Poké Balls"], game);
        TMHMPocketTest(ItemBag[tmhmPocketName], game);
        BerryPocketTest(ItemBag[berryPocketName], game);

        // Make sure adding items through the bag adds to the proper pockets.
        Assert.AreEqual(ItemBag["Items"].NumItems, 0);
        Assert.AreEqual(ItemBag["Key Items"].NumItems, 0);
        Assert.AreEqual(ItemBag["Poké Balls"].NumItems, 0);
        Assert.AreEqual(ItemBag[tmhmPocketName].NumItems, 0);
        Assert.AreEqual(ItemBag[berryPocketName].NumItems, 0);
        foreach(string itemName in AllPocketItemNames) {
            itemBag.Add(itemName, 5);
        }

        Assert.AreEqual(ItemBag["Items"][0].Item, "Potion");
        Assert.AreEqual(ItemBag["Items"][0].Amount, 5);
        Assert.AreEqual(ItemBag["Items"][1].Item, "None");
        Assert.AreEqual(ItemBag["Items"][1].Amount, 0);

        Assert.AreEqual(ItemBag["Key Items"][0].Item, "Mach Bike");
        Assert.AreEqual(ItemBag["Key Items"][0].Amount, 5);
        Assert.AreEqual(ItemBag["Key Items"][1].Item, "Wailmer Pail");
        Assert.AreEqual(ItemBag["Key Items"][1].Amount, 5);
        Assert.AreEqual(ItemBag["Key Items"][2].Item, "None");
        Assert.AreEqual(ItemBag["Key Items"][2].Amount, 0);

        Assert.AreEqual(ItemBag["Poké Balls"][0].Item, "Great Ball");
        Assert.AreEqual(ItemBag["Poké Balls"][0].Amount, 5);
        Assert.AreEqual(ItemBag["Poké Balls"][1].Item, "Master Ball");
        Assert.AreEqual(ItemBag["Poké Balls"][1].Amount, 5);
        Assert.AreEqual(ItemBag["Poké Balls"][2].Item, "None");
        Assert.AreEqual(ItemBag["Poké Balls"][2].Amount, 0);

        Assert.AreEqual(ItemBag[tmhmPocketName][0].Item, "TM01");
        Assert.AreEqual(ItemBag[tmhmPocketName][0].Amount, 5);
        Assert.AreEqual(ItemBag[tmhmPocketName][1].Item, "HM04");
        Assert.AreEqual(ItemBag[tmhmPocketName][1].Amount, 5);
        Assert.AreEqual(ItemBag[tmhmPocketName][2].Item, "None");
        Assert.AreEqual(ItemBag[tmhmPocketName][2].Amount, 0);

        Assert.AreEqual(ItemBag[berryPocketName][0].Item, "Aspear Berry");
        Assert.AreEqual(ItemBag[berryPocketName][0].Amount, 5);
        Assert.AreEqual(ItemBag[berryPocketName][1].Item, "None");
        Assert.AreEqual(ItemBag[berryPocketName][1].Amount, 0);

        // Make sure removing items through the bag removes from the proper pockets.
        foreach(string itemName in AllPocketItemNames) {
            itemBag.Remove(itemName, 5);
        }

        Assert.AreEqual(ItemBag["Items"][0].Item, "None");
        Assert.AreEqual(ItemBag["Items"][0].Amount, 0);
        Assert.AreEqual(ItemBag["Items"][1].Item, "None");
        Assert.AreEqual(ItemBag["Items"][1].Amount, 0);

        Assert.AreEqual(ItemBag["Key Items"][0].Item, "None");
        Assert.AreEqual(ItemBag["Key Items"][0].Amount, 0);
        Assert.AreEqual(ItemBag["Key Items"][1].Item, "None");
        Assert.AreEqual(ItemBag["Key Items"][1].Amount, 0);
        Assert.AreEqual(ItemBag["Key Items"][2].Item, "None");
        Assert.AreEqual(ItemBag["Key Items"][2].Amount, 0);

        Assert.AreEqual(ItemBag["Poké Balls"][0].Item, "None");
        Assert.AreEqual(ItemBag["Poké Balls"][0].Amount, 0);
        Assert.AreEqual(ItemBag["Poké Balls"][1].Item, "None");
        Assert.AreEqual(ItemBag["Poké Balls"][1].Amount, 0);
        Assert.AreEqual(ItemBag["Poké Balls"][2].Item, "None");
        Assert.AreEqual(ItemBag["Poké Balls"][2].Amount, 0);

        Assert.AreEqual(ItemBag[tmhmPocketName][0].Item, "None");
        Assert.AreEqual(ItemBag[tmhmPocketName][0].Amount, 0);
        Assert.AreEqual(ItemBag[tmhmPocketName][1].Item, "None");
        Assert.AreEqual(ItemBag[tmhmPocketName][1].Amount, 0);
        Assert.AreEqual(ItemBag[tmhmPocketName][2].Item, "None");
        Assert.AreEqual(ItemBag[tmhmPocketName][2].Amount, 0);

        Assert.AreEqual(ItemBag[berryPocketName][0].Item, "None");
        Assert.AreEqual(ItemBag[berryPocketName][0].Amount, 0);
        Assert.AreEqual(ItemBag[berryPocketName][1].Item, "None");
        Assert.AreEqual(ItemBag[berryPocketName][1].Amount, 0);

        /*
         * Make sure we can't add items from later generations or incompatible
         * Generation III games.
         */
        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            WrongGameAllPocketItemNames
        );
    }
}

}
