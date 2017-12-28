/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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
    public void Gen1RedItemListTest()
    {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList2("Items", "Red"),
            "Red"
        );
    }
    [Test]
    public void Gen1RedItemPCTest()
    {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Red"),
            "Red"
        );
    }
    [Test]
    public void Gen1RedItemBagTest()
    {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Red"),
            "Red"
        );
    }

    /*
     * Blue
     */
    [Test]
    public void Gen1BlueItemListTest()
    {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList2("Items", "Blue"),
            "Blue"
        );
    }
    [Test]
    public void Gen1BlueItemPCTest()
    {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Blue"),
            "Blue"
        );
    }
    [Test]
    public void Gen1BlueItemBagTest()
    {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Blue"),
            "Blue"
        );
    }

    /*
     * Yellow
     */
    [Test]
    public void Gen1YellowItemListTest()
    {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList2("Items", "Yellow"),
            "Yellow"
        );
    }
    [Test]
    public void Gen1YellowItemPCTest()
    {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Yellow"),
            "Yellow"
        );
    }
    [Test]
    public void Gen1YellowItemBagTest()
    {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Yellow"),
            "Yellow"
        );
    }

    /*
     * Gold
     */
    [Test]
    public void Gen2GoldItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldKeyItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("KeyItems", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldBallPocketTest()
    {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList2("Balls", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldTMHMPocketTest()
    {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList2("TM/HM", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldItemPCTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Gold"),
            "Gold"
        );
    }
    [Test]
    public void Gen2GoldItemBagTest()
    {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Gold"),
            "Gold"
        );
    }

    /*
     * Silver
     */
    [Test]
    public void Gen2SilverItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverKeyItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("KeyItems", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverBallPocketTest()
    {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList2("Balls", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverTMHMPocketTest()
    {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList2("TM/HM", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverItemPCTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Silver"),
            "Silver"
        );
    }
    [Test]
    public void Gen2SilverItemBagTest()
    {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Silver"),
            "Silver"
        );
    }

    /*
     * Crystal
     */
    [Test]
    public void Gen2CrystalItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalKeyItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("KeyItems", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalBallPocketTest()
    {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList2("Balls", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalTMHMPocketTest()
    {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList2("TM/HM", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalItemPCTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Crystal"),
            "Crystal"
        );
    }
    [Test]
    public void Gen2CrystalItemBagTest()
    {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Crystal"),
            "Crystal"
        );
    }

    /*
     * Ruby
     */
    [Test]
    public void GBARubyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("Key Items", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList2("Poké Balls", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList2("TMs & HMs", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList2("Berries", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Ruby"),
            "Ruby"
        );
    }
    [Test]
    public void GBARubyItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Ruby"),
            "Ruby"
        );
    }

    /*
     * Sapphire
     */
    [Test]
    public void GBASapphireItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("Key Items", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList2("Poké Balls", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList2("TMs & HMs", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList2("Berries", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Sapphire"),
            "Sapphire"
        );
    }
    [Test]
    public void GBASapphireItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Sapphire"),
            "Sapphire"
        );
    }

    /*
     * Emerald
     */
    [Test]
    public void GBAEmeraldItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("Key Items", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList2("Poké Balls", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList2("TMs & HMs", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList2("Berries", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Emerald"),
            "Emerald"
        );
    }
    [Test]
    public void GBAEmeraldItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Emerald"),
            "Emerald"
        );
    }

    /*
     * FireRed
     */
    [Test]
    public void GBAFireRedItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("Key Items", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList2("Poké Balls", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList2("TM Case", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList2("Berry Pouch", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "FireRed"),
            "FireRed"
        );
    }
    [Test]
    public void GBAFireRedItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag2("FireRed"),
            "FireRed"
        );
    }

    /*
     * LeafGreen
     */
    [Test]
    public void GBALeafGreenItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("Key Items", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList2("Poké Balls", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList2("TM Case", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList2("Berry Pouch", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "LeafGreen"),
            "LeafGreen"
        );
    }
    [Test]
    public void GBALeafGreenItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag2("LeafGreen"),
            "LeafGreen"
        );
    }

    /*
     * Colosseum
     */
    [Test]
    public void GCNColosseumItemPocketTest()
    {
        PKMNTest.GCNItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "Colosseum"),
            "Colosseum"
        );
    }
    [Test]
    public void GCNColosseumKeyItemPocketTest()
    {
        PKMNTest.GCNItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("Key Items", "Colosseum"),
            "Colosseum"
        );
    }
    [Test]
    public void GCNColosseumBallPocketTest()
    {
        PKMNTest.GCNItemsTest.BallPocketTest(
            new PKMN.ItemList2("Poké Balls", "Colosseum"),
            "Colosseum"
        );
    }
    [Test]
    public void GCNColosseumTMPocketTest()
    {
        PKMNTest.GCNItemsTest.TMPocketTest(
            new PKMN.ItemList2("TMs", "Colosseum"),
            "Colosseum"
        );
    }
    [Test]
    public void GCNColosseumBerryPocketTest()
    {
        PKMNTest.GCNItemsTest.BerryPocketTest(
            new PKMN.ItemList2("Berries", "Colosseum"),
            "Colosseum"
        );
    }
    [Test]
    public void GCNColosseumColognePocketTest()
    {
        PKMNTest.GCNItemsTest.ColognePocketTest(
            new PKMN.ItemList2("Colognes", "Colosseum"),
            "Colosseum"
        );
    }
    [Test]
    public void GCNColosseumItemPCTest()
    {
        PKMNTest.GCNItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "Colosseum"),
            "Colosseum"
        );
    }
    [Test]
    public void GCNColosseumItemBagTest()
    {
        PKMNTest.GCNItemsTest.ItemBagTest(
            new PKMN.ItemBag2("Colosseum"),
            "Colosseum"
        );
    }

    /*
     * XD
     */
    [Test]
    public void GCNXDItemPocketTest()
    {
        PKMNTest.GCNItemsTest.ItemPocketTest(
            new PKMN.ItemList2("Items", "XD"),
            "XD"
        );
    }
    [Test]
    public void GCNXDKeyItemPocketTest()
    {
        PKMNTest.GCNItemsTest.KeyItemPocketTest(
            new PKMN.ItemList2("Key Items", "XD"),
            "XD"
        );
    }
    [Test]
    public void GCNXDBallPocketTest()
    {
        PKMNTest.GCNItemsTest.BallPocketTest(
            new PKMN.ItemList2("Poké Balls", "XD"),
            "XD"
        );
    }
    [Test]
    public void GCNXDTMPocketTest()
    {
        PKMNTest.GCNItemsTest.TMPocketTest(
            new PKMN.ItemList2("TMs", "XD"),
            "XD"
        );
    }
    [Test]
    public void GCNXDBerryPocketTest()
    {
        PKMNTest.GCNItemsTest.BerryPocketTest(
            new PKMN.ItemList2("Berries", "XD"),
            "XD"
        );
    }
    [Test]
    public void GCNXDColognePocketTest()
    {
        PKMNTest.GCNItemsTest.ColognePocketTest(
            new PKMN.ItemList2("Colognes", "XD"),
            "XD"
        );
    }
    [Test]
    public void GCNXDBattleCDPocketTest()
    {
        PKMNTest.GCNItemsTest.BattleCDPocketTest(
            new PKMN.ItemList2("Battle CDs", "XD"),
            "XD"
        );
    }
    [Test]
    public void GCNXDItemPCTest()
    {
        PKMNTest.GCNItemsTest.ItemPCTest(
            new PKMN.ItemList2("PC", "XD"),
            "XD"
        );
    }
    [Test]
    public void GCNXDItemBagTest()
    {
        PKMNTest.GCNItemsTest.ItemBagTest(
            new PKMN.ItemBag2("XD"),
            "XD"
        );
    }
}
