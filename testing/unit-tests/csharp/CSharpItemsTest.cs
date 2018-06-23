/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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
            new PKMN.ItemList("Items", PKMN.Game.RED),
            PKMN.Game.RED
        );
    }
    [Test]
    public void Gen1RedItemPCTest()
    {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.RED),
            PKMN.Game.RED
        );
    }
    [Test]
    public void Gen1RedItemBagTest()
    {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.RED),
            PKMN.Game.RED
        );
    }

    /*
     * Blue
     */
    [Test]
    public void Gen1BlueItemListTest()
    {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList("Items", PKMN.Game.BLUE),
            PKMN.Game.BLUE
        );
    }
    [Test]
    public void Gen1BlueItemPCTest()
    {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.BLUE),
            PKMN.Game.BLUE
        );
    }
    [Test]
    public void Gen1BlueItemBagTest()
    {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.BLUE),
            PKMN.Game.BLUE
        );
    }

    /*
     * Yellow
     */
    [Test]
    public void Gen1YellowItemListTest()
    {
        PKMNTest.Gen1ItemsTest.ItemListTest(
            new PKMN.ItemList("Items", PKMN.Game.YELLOW),
            PKMN.Game.YELLOW
        );
    }
    [Test]
    public void Gen1YellowItemPCTest()
    {
        PKMNTest.Gen1ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.YELLOW),
            PKMN.Game.YELLOW
        );
    }
    [Test]
    public void Gen1YellowItemBagTest()
    {
        PKMNTest.Gen1ItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.YELLOW),
            PKMN.Game.YELLOW
        );
    }

    /*
     * Gold
     */
    [Test]
    public void Gen2GoldItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.GOLD),
            PKMN.Game.GOLD
        );
    }
    [Test]
    public void Gen2GoldKeyItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("KeyItems", PKMN.Game.GOLD),
            PKMN.Game.GOLD
        );
    }
    [Test]
    public void Gen2GoldBallPocketTest()
    {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList("Balls", PKMN.Game.GOLD),
            PKMN.Game.GOLD
        );
    }
    [Test]
    public void Gen2GoldTMHMPocketTest()
    {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM/HM", PKMN.Game.GOLD),
            PKMN.Game.GOLD
        );
    }
    [Test]
    public void Gen2GoldItemPCTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.GOLD),
            PKMN.Game.GOLD
        );
    }
    [Test]
    public void Gen2GoldItemBagTest()
    {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.GOLD),
            PKMN.Game.GOLD
        );
    }

    /*
     * Silver
     */
    [Test]
    public void Gen2SilverItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.SILVER),
            PKMN.Game.SILVER
        );
    }
    [Test]
    public void Gen2SilverKeyItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("KeyItems", PKMN.Game.SILVER),
            PKMN.Game.SILVER
        );
    }
    [Test]
    public void Gen2SilverBallPocketTest()
    {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList("Balls", PKMN.Game.SILVER),
            PKMN.Game.SILVER
        );
    }
    [Test]
    public void Gen2SilverTMHMPocketTest()
    {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM/HM", PKMN.Game.SILVER),
            PKMN.Game.SILVER
        );
    }
    [Test]
    public void Gen2SilverItemPCTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.SILVER),
            PKMN.Game.SILVER
        );
    }
    [Test]
    public void Gen2SilverItemBagTest()
    {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.SILVER),
            PKMN.Game.SILVER
        );
    }

    /*
     * Crystal
     */
    [Test]
    public void Gen2CrystalItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.CRYSTAL),
            PKMN.Game.CRYSTAL
        );
    }
    [Test]
    public void Gen2CrystalKeyItemPocketTest()
    {
        PKMNTest.Gen2ItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("KeyItems", PKMN.Game.CRYSTAL),
            PKMN.Game.CRYSTAL
        );
    }
    [Test]
    public void Gen2CrystalBallPocketTest()
    {
        PKMNTest.Gen2ItemsTest.BallPocketTest(
            new PKMN.ItemList("Balls", PKMN.Game.CRYSTAL),
            PKMN.Game.CRYSTAL
        );
    }
    [Test]
    public void Gen2CrystalTMHMPocketTest()
    {
        PKMNTest.Gen2ItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM/HM", PKMN.Game.CRYSTAL),
            PKMN.Game.CRYSTAL
        );
    }
    [Test]
    public void Gen2CrystalItemPCTest()
    {
        PKMNTest.Gen2ItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.CRYSTAL),
            PKMN.Game.CRYSTAL
        );
    }
    [Test]
    public void Gen2CrystalItemBagTest()
    {
        PKMNTest.Gen2ItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.CRYSTAL),
            PKMN.Game.CRYSTAL
        );
    }

    /*
     * Ruby
     */
    [Test]
    public void GBARubyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.RUBY),
            PKMN.Game.RUBY
        );
    }
    [Test]
    public void GBARubyKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", PKMN.Game.RUBY),
            PKMN.Game.RUBY
        );
    }
    [Test]
    public void GBARubyBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", PKMN.Game.RUBY),
            PKMN.Game.RUBY
        );
    }
    [Test]
    public void GBARubyTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TMs & HMs", PKMN.Game.RUBY),
            PKMN.Game.RUBY
        );
    }
    [Test]
    public void GBARubyBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berries", PKMN.Game.RUBY),
            PKMN.Game.RUBY
        );
    }
    [Test]
    public void GBARubyItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.RUBY),
            PKMN.Game.RUBY
        );
    }
    [Test]
    public void GBARubyItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.RUBY),
            PKMN.Game.RUBY
        );
    }

    /*
     * Sapphire
     */
    [Test]
    public void GBASapphireItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.SAPPHIRE),
            PKMN.Game.SAPPHIRE
        );
    }
    [Test]
    public void GBASapphireKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", PKMN.Game.SAPPHIRE),
            PKMN.Game.SAPPHIRE
        );
    }
    [Test]
    public void GBASapphireBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", PKMN.Game.SAPPHIRE),
            PKMN.Game.SAPPHIRE
        );
    }
    [Test]
    public void GBASapphireTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TMs & HMs", PKMN.Game.SAPPHIRE),
            PKMN.Game.SAPPHIRE
        );
    }
    [Test]
    public void GBASapphireBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berries", PKMN.Game.SAPPHIRE),
            PKMN.Game.SAPPHIRE
        );
    }
    [Test]
    public void GBASapphireItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.SAPPHIRE),
            PKMN.Game.SAPPHIRE
        );
    }
    [Test]
    public void GBASapphireItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.SAPPHIRE),
            PKMN.Game.SAPPHIRE
        );
    }

    /*
     * Emerald
     */
    [Test]
    public void GBAEmeraldItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.EMERALD),
            PKMN.Game.EMERALD
        );
    }
    [Test]
    public void GBAEmeraldKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", PKMN.Game.EMERALD),
            PKMN.Game.EMERALD
        );
    }
    [Test]
    public void GBAEmeraldBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", PKMN.Game.EMERALD),
            PKMN.Game.EMERALD
        );
    }
    [Test]
    public void GBAEmeraldTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TMs & HMs", PKMN.Game.EMERALD),
            PKMN.Game.EMERALD
        );
    }
    [Test]
    public void GBAEmeraldBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berries", PKMN.Game.EMERALD),
            PKMN.Game.EMERALD
        );
    }
    [Test]
    public void GBAEmeraldItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.EMERALD),
            PKMN.Game.EMERALD
        );
    }
    [Test]
    public void GBAEmeraldItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.EMERALD),
            PKMN.Game.EMERALD
        );
    }

    /*
     * FireRed
     */
    [Test]
    public void GBAFireRedItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.FIRERED),
            PKMN.Game.FIRERED
        );
    }
    [Test]
    public void GBAFireRedKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", PKMN.Game.FIRERED),
            PKMN.Game.FIRERED
        );
    }
    [Test]
    public void GBAFireRedBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", PKMN.Game.FIRERED),
            PKMN.Game.FIRERED
        );
    }
    [Test]
    public void GBAFireRedTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM Case", PKMN.Game.FIRERED),
            PKMN.Game.FIRERED
        );
    }
    [Test]
    public void GBAFireRedBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berry Pouch", PKMN.Game.FIRERED),
            PKMN.Game.FIRERED
        );
    }
    [Test]
    public void GBAFireRedItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.FIRERED),
            PKMN.Game.FIRERED
        );
    }
    [Test]
    public void GBAFireRedItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.FIRERED),
            PKMN.Game.FIRERED
        );
    }

    /*
     * LeafGreen
     */
    [Test]
    public void GBALeafGreenItemPocketTest()
    {
        PKMNTest.GBAItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.LEAFGREEN),
            PKMN.Game.LEAFGREEN
        );
    }
    [Test]
    public void GBALeafGreenKeyItemPocketTest()
    {
        PKMNTest.GBAItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", PKMN.Game.LEAFGREEN),
            PKMN.Game.LEAFGREEN
        );
    }
    [Test]
    public void GBALeafGreenBallPocketTest()
    {
        PKMNTest.GBAItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", PKMN.Game.LEAFGREEN),
            PKMN.Game.LEAFGREEN
        );
    }
    [Test]
    public void GBALeafGreenTMHMPocketTest()
    {
        PKMNTest.GBAItemsTest.TMHMPocketTest(
            new PKMN.ItemList("TM Case", PKMN.Game.LEAFGREEN),
            PKMN.Game.LEAFGREEN
        );
    }
    [Test]
    public void GBALeafGreenBerryPocketTest()
    {
        PKMNTest.GBAItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berry Pouch", PKMN.Game.LEAFGREEN),
            PKMN.Game.LEAFGREEN
        );
    }
    [Test]
    public void GBALeafGreenItemPCTest()
    {
        PKMNTest.GBAItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.LEAFGREEN),
            PKMN.Game.LEAFGREEN
        );
    }
    [Test]
    public void GBALeafGreenItemBagTest()
    {
        PKMNTest.GBAItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.LEAFGREEN),
            PKMN.Game.LEAFGREEN
        );
    }

    /*
     * Colosseum
     */
    [Test]
    public void GCNColosseumItemPocketTest()
    {
        PKMNTest.GCNItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.COLOSSEUM),
            PKMN.Game.COLOSSEUM
        );
    }
    [Test]
    public void GCNColosseumKeyItemPocketTest()
    {
        PKMNTest.GCNItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", PKMN.Game.COLOSSEUM),
            PKMN.Game.COLOSSEUM
        );
    }
    [Test]
    public void GCNColosseumBallPocketTest()
    {
        PKMNTest.GCNItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", PKMN.Game.COLOSSEUM),
            PKMN.Game.COLOSSEUM
        );
    }
    [Test]
    public void GCNColosseumTMPocketTest()
    {
        PKMNTest.GCNItemsTest.TMPocketTest(
            new PKMN.ItemList("TMs", PKMN.Game.COLOSSEUM),
            PKMN.Game.COLOSSEUM
        );
    }
    [Test]
    public void GCNColosseumBerryPocketTest()
    {
        PKMNTest.GCNItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berries", PKMN.Game.COLOSSEUM),
            PKMN.Game.COLOSSEUM
        );
    }
    [Test]
    public void GCNColosseumColognePocketTest()
    {
        PKMNTest.GCNItemsTest.ColognePocketTest(
            new PKMN.ItemList("Colognes", PKMN.Game.COLOSSEUM),
            PKMN.Game.COLOSSEUM
        );
    }
    [Test]
    public void GCNColosseumItemPCTest()
    {
        PKMNTest.GCNItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.COLOSSEUM),
            PKMN.Game.COLOSSEUM
        );
    }
    [Test]
    public void GCNColosseumItemBagTest()
    {
        PKMNTest.GCNItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.COLOSSEUM),
            PKMN.Game.COLOSSEUM
        );
    }

    /*
     * XD
     */
    [Test]
    public void GCNXDItemPocketTest()
    {
        PKMNTest.GCNItemsTest.ItemPocketTest(
            new PKMN.ItemList("Items", PKMN.Game.XD),
            PKMN.Game.XD
        );
    }
    [Test]
    public void GCNXDKeyItemPocketTest()
    {
        PKMNTest.GCNItemsTest.KeyItemPocketTest(
            new PKMN.ItemList("Key Items", PKMN.Game.XD),
            PKMN.Game.XD
        );
    }
    [Test]
    public void GCNXDBallPocketTest()
    {
        PKMNTest.GCNItemsTest.BallPocketTest(
            new PKMN.ItemList("Poké Balls", PKMN.Game.XD),
            PKMN.Game.XD
        );
    }
    [Test]
    public void GCNXDTMPocketTest()
    {
        PKMNTest.GCNItemsTest.TMPocketTest(
            new PKMN.ItemList("TMs", PKMN.Game.XD),
            PKMN.Game.XD
        );
    }
    [Test]
    public void GCNXDBerryPocketTest()
    {
        PKMNTest.GCNItemsTest.BerryPocketTest(
            new PKMN.ItemList("Berries", PKMN.Game.XD),
            PKMN.Game.XD
        );
    }
    [Test]
    public void GCNXDColognePocketTest()
    {
        PKMNTest.GCNItemsTest.ColognePocketTest(
            new PKMN.ItemList("Colognes", PKMN.Game.XD),
            PKMN.Game.XD
        );
    }
    [Test]
    public void GCNXDBattleCDPocketTest()
    {
        PKMNTest.GCNItemsTest.BattleCDPocketTest(
            new PKMN.ItemList("Battle CDs", PKMN.Game.XD),
            PKMN.Game.XD
        );
    }
    [Test]
    public void GCNXDItemPCTest()
    {
        PKMNTest.GCNItemsTest.ItemPCTest(
            new PKMN.ItemList("PC", PKMN.Game.XD),
            PKMN.Game.XD
        );
    }
    [Test]
    public void GCNXDItemBagTest()
    {
        PKMNTest.GCNItemsTest.ItemBagTest(
            new PKMN.ItemBag(PKMN.Game.XD),
            PKMN.Game.XD
        );
    }
}
