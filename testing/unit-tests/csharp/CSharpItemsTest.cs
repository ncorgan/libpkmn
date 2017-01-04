/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpItemsTest {
    /*
     * Red
     */
    [Test]
    public void Gen1RedItemListTest() {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList("Items", "Red"),
            "Red"
        );
    }
    [Test]
    public void Gen1RedItemPCTest() {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Red"),
            "Red"
        );
    }
    [Test]
    public void Gen1RedItemBagTest() {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Red"),
            "Red"
        );
    }

    /*
     * Blue
     */
    [Test]
    public void Gen1BlueItemListTest() {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList("Items", "Blue"),
            "Blue"
        );
    }
    [Test]
    public void Gen1BlueItemPCTest() {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Blue"),
            "Blue"
        );
    }
    [Test]
    public void Gen1BlueItemBagTest() {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Blue"),
            "Blue"
        );
    }

    /*
     * Yellow
     */
    [Test]
    public void Gen1YellowItemListTest() {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList("Items", "Yellow"),
            "Yellow"
        );
    }
    [Test]
    public void Gen1YellowItemPCTest() {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Yellow"),
            "Yellow"
        );
    }
    [Test]
    public void Gen1YellowItemBagTest() {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Yellow"),
            "Yellow"
        );
    }

    /*
     * Gold
     */
    [Test]
    public void Gen2GoldItemPocketTest() {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldKeyItemPocketTest() {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("KeyItems", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldBallPocketTest() {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList("Balls", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldTMHMPocketTest() {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM/HM", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldItemPCTest() {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldItemBagTest() {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Gold"),
            "Gold"
        );
    }

    /*
     * Silver
     */
    [Test]
    public void Gen2SilverItemPocketTest() {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverKeyItemPocketTest() {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("KeyItems", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverBallPocketTest() {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList("Balls", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverTMHMPocketTest() {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM/HM", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverItemPCTest() {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverItemBagTest() {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Silver"),
            "Silver"
        );
    }

    /*
     * Crystal
     */
    [Test]
    public void Gen2CrystalItemPocketTest() {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalKeyItemPocketTest() {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("KeyItems", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalBallPocketTest() {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList("Balls", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalTMHMPocketTest() {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM/HM", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalItemPCTest() {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalItemBagTest() {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag("Crystal"),
            "Crystal"
        );
    }

    /*
     * Ruby
     */
    [Test]
    public void GBARubyItemPocketTest() {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyKeyItemPocketTest() {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyBallPocketTest() {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyTMHMPocketTest() {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TMs & HMs", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyBerryPocketTest() {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berries", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyItemPCTest() {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyItemBagTest() {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag("Ruby"),
            "Ruby"
        );
    }

    /*
     * Sapphire
     */
    [Test]
    public void GBASapphireItemPocketTest() {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireKeyItemPocketTest() {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireBallPocketTest() {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireTMHMPocketTest() {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TMs & HMs", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireBerryPocketTest() {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berries", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireItemPCTest() {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireItemBagTest() {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag("Sapphire"),
            "Sapphire"
        );
    }

    /*
     * Emerald
     */
    [Test]
    public void GBAEmeraldItemPocketTest() {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldKeyItemPocketTest() {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldBallPocketTest() {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldTMHMPocketTest() {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TMs & HMs", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldBerryPocketTest() {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berries", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldItemPCTest() {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldItemBagTest() {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag("Emerald"),
            "Emerald"
        );
    }

    /*
     * FireRed
     */
    [Test]
    public void GBAFireRedItemPocketTest() {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedKeyItemPocketTest() {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedBallPocketTest() {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedTMHMPocketTest() {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM Case", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedBerryPocketTest() {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berry Pouch", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedItemPCTest() {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedItemBagTest() {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag("FireRed"),
            "FireRed"
        );
    }

    /*
     * LeafGreen
     */
    [Test]
    public void GBALeafGreenItemPocketTest() {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenKeyItemPocketTest() {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenBallPocketTest() {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenTMHMPocketTest() {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM Case", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenBerryPocketTest() {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berry Pouch", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenItemPCTest() {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenItemBagTest() {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag("LeafGreen"),
            "LeafGreen"
        );
    }
}
