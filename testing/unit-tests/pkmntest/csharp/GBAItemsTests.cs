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
    private static string[] AllPocketItemNames =
    {
        "Potion", "Mach Bike", "Great Ball", "TM01",
        "Aspear Berry", "Wailmer Pail", "Master Ball", "HM04"
    };
    private static string[] WrongGameAllPocketItemNames =
    {
        "Pink Bow", "Black Sludge",
        "Ein File S", "Gonzap's Key",
        "GS Ball", "Poffin Items",
        "TM51",
        "Berry", "Occa Berry"
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
        if((game == PKMN.Game.RUBY) || (game == PKMN.Game.SAPPHIRE))
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new string[]{"Helix Fossil", "Tea", "Ruby"}
            );
        }
        if(game != PKMN.Game.EMERALD)
        {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new string[]{"Magma Emblem", "Old Sea Map"}
            );
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        string[] validItems = {"Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
                               "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"};
        ItemsTestsCommon.TestItemListSettingItems(
            keyItemPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            keyItemPocket,
            validItems
        );

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
        string[] validItems = {"TM01", "HM01", "TM02", "HM02",
                               "TM03", "HM03", "TM04", "HM04"};
        ItemsTestsCommon.TestItemListSettingItems(
            tmhmPocket,
            validItems
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            tmhmPocket,
            validItems
        );

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
            "Potion"
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPC,
            WrongGameAllPocketItemNames
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListSettingItems(
            itemPC,
            AllPocketItemNames
        );
        ItemsTestsCommon.TestItemListAddingAndRemovingItems(
            itemPC,
            AllPocketItemNames
        );

        PKMN.StringList fullItemList = PKMN.Database.Lists.ItemList(game);
        if((game == PKMN.Game.FIRERED) || (game == PKMN.Game.LEAFGREEN))
        {
            Assert.AreEqual(itemPC.ValidItems.Count, fullItemList.Count-2);
            Assert.IsFalse(itemPC.ValidItems.Contains("Berry Pouch"));
            Assert.IsFalse(itemPC.ValidItems.Contains("TM Case"));
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
        foreach(string itemName in AllPocketItemNames)
        {
            itemBag.Add(itemName, 5);
        }

        Assert.AreEqual(itemBag["Items"][0].Item, "Potion");
        Assert.AreEqual(itemBag["Items"][0].Amount, 5);
        Assert.AreEqual(itemBag["Items"][1].Item, "None");
        Assert.AreEqual(itemBag["Items"][1].Amount, 0);

        Assert.AreEqual(itemBag["Key Items"][0].Item, "Mach Bike");
        Assert.AreEqual(itemBag["Key Items"][0].Amount, 5);
        Assert.AreEqual(itemBag["Key Items"][1].Item, "Wailmer Pail");
        Assert.AreEqual(itemBag["Key Items"][1].Amount, 5);
        Assert.AreEqual(itemBag["Key Items"][2].Item, "None");
        Assert.AreEqual(itemBag["Key Items"][2].Amount, 0);

        Assert.AreEqual(itemBag["Poké Balls"][0].Item, "Great Ball");
        Assert.AreEqual(itemBag["Poké Balls"][0].Amount, 5);
        Assert.AreEqual(itemBag["Poké Balls"][1].Item, "Master Ball");
        Assert.AreEqual(itemBag["Poké Balls"][1].Amount, 5);
        Assert.AreEqual(itemBag["Poké Balls"][2].Item, "None");
        Assert.AreEqual(itemBag["Poké Balls"][2].Amount, 0);

        Assert.AreEqual(itemBag[tmhmPocketName][0].Item, "TM01");
        Assert.AreEqual(itemBag[tmhmPocketName][0].Amount, 5);
        Assert.AreEqual(itemBag[tmhmPocketName][1].Item, "HM04");
        Assert.AreEqual(itemBag[tmhmPocketName][1].Amount, 5);
        Assert.AreEqual(itemBag[tmhmPocketName][2].Item, "None");
        Assert.AreEqual(itemBag[tmhmPocketName][2].Amount, 0);

        Assert.AreEqual(itemBag[berryPocketName][0].Item, "Aspear Berry");
        Assert.AreEqual(itemBag[berryPocketName][0].Amount, 5);
        Assert.AreEqual(itemBag[berryPocketName][1].Item, "None");
        Assert.AreEqual(itemBag[berryPocketName][1].Amount, 0);

        // Make sure removing items through the bag removes from the proper pockets.
        foreach(string itemName in AllPocketItemNames)
        {
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
        Assert.AreEqual(itemBag["Key Items"][2].Item, "None");
        Assert.AreEqual(itemBag["Key Items"][2].Amount, 0);

        Assert.AreEqual(itemBag["Poké Balls"][0].Item, "None");
        Assert.AreEqual(itemBag["Poké Balls"][0].Amount, 0);
        Assert.AreEqual(itemBag["Poké Balls"][1].Item, "None");
        Assert.AreEqual(itemBag["Poké Balls"][1].Amount, 0);
        Assert.AreEqual(itemBag["Poké Balls"][2].Item, "None");
        Assert.AreEqual(itemBag["Poké Balls"][2].Amount, 0);

        Assert.AreEqual(itemBag[tmhmPocketName][0].Item, "None");
        Assert.AreEqual(itemBag[tmhmPocketName][0].Amount, 0);
        Assert.AreEqual(itemBag[tmhmPocketName][1].Item, "None");
        Assert.AreEqual(itemBag[tmhmPocketName][1].Amount, 0);
        Assert.AreEqual(itemBag[tmhmPocketName][2].Item, "None");
        Assert.AreEqual(itemBag[tmhmPocketName][2].Amount, 0);

        Assert.AreEqual(itemBag[berryPocketName][0].Item, "None");
        Assert.AreEqual(itemBag[berryPocketName][0].Amount, 0);
        Assert.AreEqual(itemBag[berryPocketName][1].Item, "None");
        Assert.AreEqual(itemBag[berryPocketName][1].Amount, 0);

        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            WrongGameAllPocketItemNames
        );
    }
}

}
