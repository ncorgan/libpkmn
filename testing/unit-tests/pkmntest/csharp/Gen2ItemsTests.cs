/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class Gen2ItemsTest {

    private static string[] AllPocketItemNames = {
        "Potion", "Bicycle", "Great Ball", "TM28",
        "Berry", "SquirtBottle", "Friend Ball", "HM01"
    };
    private static string[] CrystalItemNames = {
        "Clear Bell", "GS Ball", "Blue Card", "Egg Ticket"
    };
    private static string[] WrongGenerationAllPocketItemNames = {
        "Black Sludge", "Poffin Items", "TM51", "Occa Berry"
    };

    public static void ItemPocketTest(
        PKMN.ItemList itemPocket,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(itemPocket.GetName(), "Items");
        Assert.AreEqual(itemPocket.GetGame(), game);
        Assert.AreEqual(itemPocket.GetCapacity(), 20);
        Assert.AreEqual(itemPocket.GetNumItems(), 0);

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
            new string[]{"Bicycle", "Master Ball", "HM01"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new string[]{"Black Flute", "Black Sludge", "Binding Band", "Beedrillite"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            itemPocket,
            new string[]{"Potion", "HP Up", "Wht Apricorn", "Lucky Egg",
                         "Flower Mail", "Burn Heal", "PSNCureBerry", "Stick"}
        );

        PKMN.StringList validItems = itemPocket.GetValidItems();
        Assert.Greater(validItems.Count, 0);
    }

    public static void KeyItemPocketTest(
        PKMN.ItemList keyItemPocket,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(keyItemPocket.GetName(), "KeyItems");
        Assert.AreEqual(keyItemPocket.GetGame(), game);
        Assert.AreEqual(keyItemPocket.GetCapacity(), 26);
        Assert.AreEqual(keyItemPocket.GetNumItems(), 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(keyItemPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            keyItemPocket,
            "Bicycle"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"Potion", "Master Ball", "HM01"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"Mach Bike", "Jade Orb", "Light Stone", "Aqua Suit"}
        );

        // Crystal-specific items.
        if(game.Equals("Crystal")) {
            foreach(string crystalItem in CrystalItemNames) {
                keyItemPocket.Add(crystalItem, 1);
                keyItemPocket.Remove(crystalItem, 1);
            }

            Assert.AreEqual(keyItemPocket.GetNumItems(), 0);
        } else {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                CrystalItemNames
            );
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            keyItemPocket,
            new string[]{"Bicycle", "Basement Key", "SecretPotion", "Mystery Egg",
                         "Silver Wing", "Lost Item", "SquirtBottle", "Rainbow Wing"}
        );

        PKMN.StringList validItems = keyItemPocket.GetValidItems();
        Assert.Greater(validItems.Count, 0);
    }

    public static void BallPocketTest(
        PKMN.ItemList ballPocket,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(ballPocket.GetName(), "Balls");
        Assert.AreEqual(ballPocket.GetGame(), game);
        Assert.AreEqual(ballPocket.GetCapacity(), 12);
        Assert.AreEqual(ballPocket.GetNumItems(), 0);

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
            new string[]{"Potion", "Bicycle", "HM01"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            ballPocket,
            new string[]{"Premier Ball", "Heal Ball", "Dream Ball"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            ballPocket,
            new string[]{"Great Ball", "Poké Ball", "Park Ball", "Fast Ball",
                         "Master Ball", "Friend Ball", "Love Ball", "Level Ball"}
        );

        PKMN.StringList validItems = ballPocket.GetValidItems();
        Assert.Greater(validItems.Count, 0);
    }

    public static void TMHMPocketTest(
        PKMN.ItemList tmhmPocket,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(tmhmPocket.GetName(), "TM/HM");
        Assert.AreEqual(tmhmPocket.GetGame(), game);
        Assert.AreEqual(tmhmPocket.GetCapacity(), 57);
        Assert.AreEqual(tmhmPocket.GetNumItems(), 0);

        // Make sure item slots start as correctly empty.
        Assert.AreEqual(tmhmPocket.AsList().Count, 57);
        for(int i = 1; i <= 50; ++i) {
            string name = String.Format("TM{0:D2}", i);
            Assert.AreEqual(tmhmPocket[i-1].Item.GetName(), name);
            Assert.AreEqual(tmhmPocket[i-1].Amount, 0);
        }
        for(int i = 1; i <= 7; ++i) {
            string name = String.Format("HM{0:D2}", i);
            Assert.AreEqual(tmhmPocket[50+i-1].Item.GetName(), name);
            Assert.AreEqual(tmhmPocket[50+i-1].Amount, 0);
        }

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            tmhmPocket,
            "TM10"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmhmPocket,
            new string[]{"Bicycle", "Master Ball", "Bicycle"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmhmPocket,
            new string[]{"TM51"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        for(int i = 1; i <= 50; ++i) {
            string name = String.Format("TM{0:D2}", i);
            tmhmPocket.Add(name, 50);
            Assert.AreEqual(tmhmPocket.GetNumItems(), i);
            Assert.AreEqual(tmhmPocket[i-1].Item.GetName(), name);
            Assert.AreEqual(tmhmPocket[i-1].Amount, 50);
        }
        for(int i = 50; i >= 1; --i) {
            string name = String.Format("TM{0:D2}", i);
            tmhmPocket.Remove(name, 50);
            Assert.AreEqual(tmhmPocket.GetNumItems(), i-1);
            Assert.AreEqual(tmhmPocket[i-1].Item.GetName(), name);
            Assert.AreEqual(tmhmPocket[i-1].Amount, 0);
        }

        for(int i = 1; i <= 7; ++i) {
            string name = String.Format("HM{0:D2}", i);
            tmhmPocket.Add(name, 1);
            Assert.AreEqual(tmhmPocket.GetNumItems(), i);
            Assert.AreEqual(tmhmPocket[50+i-1].Item.GetName(), name);
            Assert.AreEqual(tmhmPocket[50+i-1].Amount, 1);
        }
        for(int i = 7; i >= 1; --i) {
            string name = String.Format("HM{0:D2}", i);
            tmhmPocket.Remove(name, 1);
            Assert.AreEqual(tmhmPocket.GetNumItems(), i-1);
            Assert.AreEqual(tmhmPocket[50+i-1].Item.GetName(), name);
            Assert.AreEqual(tmhmPocket[50+i-1].Amount, 0);
        }

        PKMN.StringList validItems = tmhmPocket.GetValidItems();
        Assert.AreEqual(validItems.Count, 57);
    }

    public static void ItemPCTest(
        PKMN.ItemList itemPC,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(itemPC.GetName(), "PC");
        Assert.AreEqual(itemPC.GetGame(), game);
        Assert.AreEqual(itemPC.GetCapacity(), 50);
        Assert.AreEqual(itemPC.GetNumItems(), 0);

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
            WrongGenerationAllPocketItemNames
        );

        // Crystal-specific items.
        if(game.Equals("Crystal")) {
            foreach(string crystalItem in CrystalItemNames) {
                itemPC.Add(crystalItem, 1);
                itemPC.Remove(crystalItem, 1);
            }

            Assert.AreEqual(itemPC.GetNumItems(), 0);
        } else {
            ItemsTestsCommon.TestItemListInvalidItems(
                itemPC,
                CrystalItemNames
            );
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            itemPC,
            AllPocketItemNames
        );

        PKMN.StringList validItems = itemPC.GetValidItems();
        PKMN.StringList fullItemList = PKMN.Database.GetItemList(game);
        Assert.AreEqual(validItems.Count, fullItemList.Count);
    }

    public static void ItemBagTest(
        PKMN.ItemBag itemBag,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(itemBag.GetGame(), game);

        PKMN.ItemPockets itemPockets = itemBag.GetPockets();
        Assert.AreEqual(itemPockets.Count, 4);

        ItemPocketTest(itemPockets["Items"], game);
        KeyItemPocketTest(itemPockets["KeyItems"], game);
        BallPocketTest(itemPockets["Balls"], game);
        TMHMPocketTest(itemPockets["TM/HM"], game);

        // Make sure adding items through the bag adds to the proper pockets.
        Assert.AreEqual(itemPockets["Items"].GetNumItems(), 0);
        Assert.AreEqual(itemPockets["KeyItems"].GetNumItems(), 0);
        Assert.AreEqual(itemPockets["Balls"].GetNumItems(), 0);
        Assert.AreEqual(itemPockets["TM/HM"].GetNumItems(), 0);
        foreach(string itemName in AllPocketItemNames) {
            itemBag.Add(itemName, 5);
        }

        Assert.AreEqual(itemPockets["Items"][0].Item.GetName(), "Potion");
        Assert.AreEqual(itemPockets["Items"][0].Amount, 5);
        Assert.AreEqual(itemPockets["Items"][1].Item.GetName(), "Berry");
        Assert.AreEqual(itemPockets["Items"][1].Amount, 5);
        Assert.AreEqual(itemPockets["Items"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Items"][2].Amount, 0);

        Assert.AreEqual(itemPockets["KeyItems"][0].Item.GetName(), "Bicycle");
        Assert.AreEqual(itemPockets["KeyItems"][0].Amount, 5);
        Assert.AreEqual(itemPockets["KeyItems"][1].Item.GetName(), "SquirtBottle");
        Assert.AreEqual(itemPockets["KeyItems"][1].Amount, 5);
        Assert.AreEqual(itemPockets["KeyItems"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["KeyItems"][2].Amount, 0);

        Assert.AreEqual(itemPockets["Balls"][0].Item.GetName(), "Great Ball");
        Assert.AreEqual(itemPockets["Balls"][0].Amount, 5);
        Assert.AreEqual(itemPockets["Balls"][1].Item.GetName(), "Friend Ball");
        Assert.AreEqual(itemPockets["Balls"][1].Amount, 5);
        Assert.AreEqual(itemPockets["Balls"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Balls"][2].Amount, 0);

        Assert.AreEqual(itemPockets["TM/HM"][0].Item.GetName(), "TM01");
        Assert.AreEqual(itemPockets["TM/HM"][0].Amount, 0);
        Assert.AreEqual(itemPockets["TM/HM"][1].Item.GetName(), "TM02");
        Assert.AreEqual(itemPockets["TM/HM"][1].Amount, 0);
        Assert.AreEqual(itemPockets["TM/HM"][27].Item.GetName(), "TM28");
        Assert.AreEqual(itemPockets["TM/HM"][27].Amount, 5);
        Assert.AreEqual(itemPockets["TM/HM"][50].Item.GetName(), "HM01");
        Assert.AreEqual(itemPockets["TM/HM"][50].Amount, 5);

        // Make sure removing items through the bag removes from the proper pockets.
        foreach(string itemName in AllPocketItemNames) {
            itemBag.Remove(itemName, 5);
        }

        Assert.AreEqual(itemPockets["Items"][0].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Items"][0].Amount, 0);
        Assert.AreEqual(itemPockets["Items"][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Items"][1].Amount, 0);
        Assert.AreEqual(itemPockets["Items"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Items"][2].Amount, 0);

        Assert.AreEqual(itemPockets["KeyItems"][0].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["KeyItems"][0].Amount, 0);
        Assert.AreEqual(itemPockets["KeyItems"][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["KeyItems"][1].Amount, 0);
        Assert.AreEqual(itemPockets["KeyItems"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["KeyItems"][2].Amount, 0);

        Assert.AreEqual(itemPockets["Balls"][0].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Balls"][0].Amount, 0);
        Assert.AreEqual(itemPockets["Balls"][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Balls"][1].Amount, 0);
        Assert.AreEqual(itemPockets["Balls"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Balls"][2].Amount, 0);

        Assert.AreEqual(itemPockets["TM/HM"][0].Item.GetName(), "TM01");
        Assert.AreEqual(itemPockets["TM/HM"][0].Amount, 0);
        Assert.AreEqual(itemPockets["TM/HM"][1].Item.GetName(), "TM02");
        Assert.AreEqual(itemPockets["TM/HM"][1].Amount, 0);
        Assert.AreEqual(itemPockets["TM/HM"][27].Item.GetName(), "TM28");
        Assert.AreEqual(itemPockets["TM/HM"][27].Amount, 0);
        Assert.AreEqual(itemPockets["TM/HM"][50].Item.GetName(), "HM01");
        Assert.AreEqual(itemPockets["TM/HM"][50].Amount, 0);

        // Make sure we can't add Crystal-specific items with a Gold/Silver bag.
        if(game.Equals("Crystal")) {
            for(int i = 0; i < CrystalItemNames.Length; ++i) {
                itemBag.Add(CrystalItemNames[i], 1);
                Assert.AreEqual(itemPockets["KeyItems"][i].Item.GetName(), CrystalItemNames[i]);
                Assert.AreEqual(itemPockets["KeyItems"][i].Amount, 1);
            }
            for(int i = CrystalItemNames.Length-1; i >= 0; --i) {
                itemBag.Remove(CrystalItemNames[i], 1);
                Assert.AreEqual(itemPockets["KeyItems"][i].Item.GetName(), "None");
                Assert.AreEqual(itemPockets["KeyItems"][i].Amount, 0);
            }
        } else {
            ItemsTestsCommon.TestItemBagInvalidItems(
                itemBag,
                CrystalItemNames
            );
        }

        // Make sure we can't add items from later generations.
        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            WrongGenerationAllPocketItemNames
        );
    }
}

}
