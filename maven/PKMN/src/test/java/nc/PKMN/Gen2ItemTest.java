/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import static org.junit.Assert.*;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

@RunWith(Parameterized.class)
public class Gen2ItemTest extends ItemTest {

    @Parameters
    public static Object[] data() {
        return new Object[] {"Gold", "Silver", "Crystal"};
    }

    private static String[] allPocketItemNames = {
        "Potion", "Bicycle", "Great Ball", "TM28",
        "Berry", "SquirtBottle", "Friend Ball", "HM01"
    };
    private static String[] crystalItems = {
        "Clear Bell", "GS Ball", "Blue Card", "Egg Ticket"
    };
    private static String[] wrongGenerationAllPocketItemNames = {
        "Black Sludge", "Poffin Items", "TM51", "Occa Berry"
    };

    public Gen2ItemTest(
        String game
    ) {
        this.testGame = game;
    }

    private void testItemPocket(
        ItemList itemPocket
    ) {
        assertEquals("Items", itemPocket.getName());
        assertEquals(this.testGame, itemPocket.getGame());
        assertEquals(20, itemPocket.getCapacity());
        assertEquals(20, itemPocket.asVector().size());

        // Make sure item slots start as correctly empty.
        this.testItemListEmptySlots(itemPocket);

        // Confirm exceptions are thrown when expected.
        this.testItemListOutOfRangeError(
            itemPocket,
            "Potion"
        );

        // Confirm items from other pockets can't be added.
        this.testItemListInvalidItems(
            itemPocket,
            new String[] {"Bicycle", "Master Ball", "HM01"}
        );

        // Confirm items from later generations can't be added.
        this.testItemListInvalidItems(
            itemPocket,
            new String[] {"Black Flute", "Black Sludge", "Binding Band", "Beedrillite"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            itemPocket,
            new String[] {
                "Potion", "HP Up", "Wht Apricorn", "Lucky Egg",
                "Flower Mail", "Burn Heal", "PSNCureBerry", "Stick"
            }
        );

        StringVector validItems = itemPocket.getValidItems();
        assertTrue(validItems.size() > 0);
    }

    private void testKeyItemPocket(
        ItemList keyItemPocket
    ) {
        assertEquals("KeyItems", keyItemPocket.getName());
        assertEquals(this.testGame, keyItemPocket.getGame());
        assertEquals(26, keyItemPocket.getCapacity());
        assertEquals(26, keyItemPocket.asVector().size());

        // Make sure item slots start as correctly empty.
        this.testItemListEmptySlots(keyItemPocket);

        // Confirm exceptions are thrown when expected.
        this.testItemListOutOfRangeError(
            keyItemPocket,
            "Bicycle"
        );

        // Confirm items from other pockets can't be added.
        this.testItemListInvalidItems(
            keyItemPocket,
            new String[] {"Potion", "Master Ball", "HM01"}
        );

        // Confirm items from later generations can't be added.
        this.testItemListInvalidItems(
            keyItemPocket,
            new String[] {"Mach Bike", "Jade Orb", "Light Stone", "Aqua Suit"}
        );

        // Crystal-specific items.
        if(this.testGame.equals("Crystal")) {
            for(int i = 0; i < crystalItems.length; ++i) {
                keyItemPocket.add(
                    crystalItems[i],
                    1
                );
                keyItemPocket.remove(
                    crystalItems[i],
                    1
                );
                assertEquals(0, keyItemPocket.getNumItems());
            }
        } else {
            for(int i = 0; i < crystalItems.length; ++i) {
                this.testItemListInvalidItems(
                    keyItemPocket,
                    crystalItems
                );
            }
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            keyItemPocket,
            new String[] {
                "Bicycle", "Basement Key", "SecretPotion", "Mystery Egg",
                "Silver Wing", "Lost Item", "SquirtBottle", "Rainbow Wing"
            }
        );

        StringVector validItems = keyItemPocket.getValidItems();
        assertTrue(validItems.size() > 0);
    }

    private void testBallPocket(
        ItemList ballPocket
    ) {
        assertEquals("Balls", ballPocket.getName());
        assertEquals(this.testGame, ballPocket.getGame());
        assertEquals(12, ballPocket.getCapacity());
        assertEquals(12, ballPocket.asVector().size());

        // Make sure item slots start as correctly empty.
        this.testItemListEmptySlots(ballPocket);

        // Confirm exceptions are thrown when expected.
        this.testItemListOutOfRangeError(
            ballPocket,
            "Master Ball"
        );

        // Confirm items from other pockets can't be added.
        this.testItemListInvalidItems(
            ballPocket,
            new String[] {"Potion", "Bicycle", "HM01"}
        );

        // Confirm items from later generations can't be added.
        this.testItemListInvalidItems(
            ballPocket,
            new String[] {"Premier Ball", "Heal Ball", "Dream Ball"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            ballPocket,
            new String[] {
                "Great Ball", "Poké Ball", "Park Ball", "Fast Ball",
                "Master Ball", "Friend Ball", "Love Ball", "Level Ball"
            }
        );

        StringVector validItems = ballPocket.getValidItems();
        assertTrue(validItems.size() > 0);
    }

    private void testTMHMPocket(
        ItemList tmhmPocket
    ) {
        assertEquals("TM/HM", tmhmPocket.getName());
        assertEquals(this.testGame, tmhmPocket.getGame());
        assertEquals(57, tmhmPocket.getCapacity());
        assertEquals(57, tmhmPocket.asVector().size());

        // Make sure item slots start as correctly empty.
        ItemSlotVector itemSlots = tmhmPocket.asVector();
        for(int i = 1; i <= 50; ++i) {
            assertEquals(String.format("TM%1$02d", i), itemSlots.get(i-1).getItem().getName());
            assertEquals(0, itemSlots.get(i-1).getAmount());
        }
        for(int i = 1; i <= 7; ++i) {
            assertEquals(String.format("HM%1$02d", i), itemSlots.get(50+i-1).getItem().getName());
            assertEquals(0, itemSlots.get(50+i-1).getAmount());
        }

        // Confirm exceptions are thrown when expected.
        this.testItemListOutOfRangeError(
            tmhmPocket,
            "TM10"
        );

        // Confirm items from other pockets can't be added.
        this.testItemListInvalidItems(
            tmhmPocket,
            new String[] {"Potion", "Master Ball", "Bicycle"}
        );

        // Confirm items from later generations can't be added.
        this.testItemListInvalidItems(
            tmhmPocket,
            new String[] {"TM51"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        for(int i = 1; i <= 50; ++i) {
            String itemName = String.format("TM%1$02d", i);
            tmhmPocket.add(itemName, 50);
            assertEquals(i, tmhmPocket.getNumItems());
            assertEquals(itemName, tmhmPocket.at(i-1).getItem().getName());
            assertEquals(50, tmhmPocket.at(i-1).getAmount());
        }
        for(int i = 50; i >= 1; --i) {
            String itemName = String.format("TM%1$02d", i);
            tmhmPocket.remove(itemName, 50);
            assertEquals(i-1, tmhmPocket.getNumItems());
            assertEquals(itemName, tmhmPocket.at(i-1).getItem().getName());
            assertEquals(0, tmhmPocket.at(i-1).getAmount());
        }
        for(int i = 1; i <= 7; ++i) {
            String itemName = String.format("HM%1$02d", i);
            tmhmPocket.add(itemName, 1);
            assertEquals(i, tmhmPocket.getNumItems());
            assertEquals(itemName, tmhmPocket.at(50+i-1).getItem().getName());
            assertEquals(1, tmhmPocket.at(50+i-1).getAmount());
        }
        for(int i = 7; i >= 1; --i) {
            String itemName = String.format("HM%1$02d", i);
            tmhmPocket.remove(itemName, 1);
            assertEquals(i-1, tmhmPocket.getNumItems());
            assertEquals(itemName, tmhmPocket.at(50+i-1).getItem().getName());
            assertEquals(0, tmhmPocket.at(50+i-1).getAmount());
        }

        StringVector validItems = tmhmPocket.getValidItems();
        assertEquals(57, validItems.size());
    }

    public void testItemPC(
        ItemList itemPC
    ) {
        assertEquals("PC", itemPC.getName());
        assertEquals(this.testGame, itemPC.getGame());
        assertEquals(50, itemPC.getCapacity());
        assertEquals(50, itemPC.asVector().size());

        // Make sure item slots start as correctly empty.
        this.testItemListEmptySlots(itemPC);

        // Confirm exceptions are thrown when expected.
        this.testItemListOutOfRangeError(
            itemPC,
            "Potion"
        );

        // Crystal-specific items.
        if(this.testGame.equals("Crystal")) {
            for(int i = 0; i < crystalItems.length; ++i) {
                itemPC.add(
                    crystalItems[i],
                    1
                );
                itemPC.remove(
                    crystalItems[i],
                    1
                );
                assertEquals(0, itemPC.getNumItems());
            }
        } else {
            for(int i = 0; i < crystalItems.length; ++i) {
                this.testItemListInvalidItems(
                    itemPC,
                    crystalItems
                );
            }
        }

        // Confirm items from later generations can't be added.
        this.testItemListInvalidItems(
            itemPC,
            wrongGenerationAllPocketItemNames
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            itemPC,
            allPocketItemNames
        );

        StringVector validItems = itemPC.getValidItems();
        StringVector fullItemList = Database.getItemList(this.testGame);
        assertEquals(validItems.size(), fullItemList.size());
    }

    @Test
    public void testItemPocket() {
        ItemList itemPocket = new ItemList("Items", this.testGame);
        testItemPocket(itemPocket);
    }

    @Test
    public void testKeyItemPocket() {
        ItemList keyItemPocket = new ItemList("KeyItems", this.testGame);
        testKeyItemPocket(keyItemPocket);
    }

    @Test
    public void testBallPocket() {
        ItemList ballPocket = new ItemList("Balls", this.testGame);
        testBallPocket(ballPocket);
    }

    @Test
    public void testTMHMPocket() {
        ItemList tmhmPocket = new ItemList("TM/HM", this.testGame);
        testTMHMPocket(tmhmPocket);
    }

    @Test
    public void testItemPC() {
        ItemList itemPC = new ItemList("PC", this.testGame);
        testItemPC(itemPC);
    }

    @Test
    public void testItemBag() {
        ItemBag itemBag = new ItemBag(this.testGame);
        assertEquals(this.testGame, itemBag.getGame());

        StringItemListMap itemPockets = itemBag.getPockets();
        assertEquals(4, itemPockets.size());
        assertTrue(itemPockets.containsKey("Items"));
        assertTrue(itemPockets.containsKey("KeyItems"));
        assertTrue(itemPockets.containsKey("Balls"));
        assertTrue(itemPockets.containsKey("TM/HM"));

        StringVector pocketNames = itemBag.getPocketNames();
        assertEquals(4, pocketNames.size());
        assertTrue(pocketNames.contains("Items"));
        assertTrue(pocketNames.contains("KeyItems"));
        assertTrue(pocketNames.contains("Balls"));
        assertTrue(pocketNames.contains("TM/HM"));

        this.testItemPocket(itemPockets.get("Items"));
        this.testKeyItemPocket(itemPockets.get("KeyItems"));
        this.testBallPocket(itemPockets.get("Balls"));
        this.testTMHMPocket(itemPockets.get("TM/HM"));

        // Make sure adding items through the bag adds to the pocket.
        assertEquals(0, itemPockets.get("Items").getNumItems());
        assertEquals(0, itemPockets.get("KeyItems").getNumItems());
        assertEquals(0, itemPockets.get("Balls").getNumItems());
        assertEquals(0, itemPockets.get("TM/HM").getNumItems());
        for(int i = 0; i < 8; ++i) {
            itemBag.add(
                allPocketItemNames[i],
                5
            );
        }

        ItemSlotVector itemSlots = itemPockets.get("Items").asVector();
        ItemSlotVector keyItemSlots = itemPockets.get("KeyItems").asVector();
        ItemSlotVector ballSlots = itemPockets.get("Balls").asVector();
        ItemSlotVector tmhmSlots = itemPockets.get("TM/HM").asVector();

        assertEquals("Potion", itemSlots.get(0).getItem().getName());
        assertEquals(5, itemSlots.get(0).getAmount());
        assertEquals("Berry", itemSlots.get(1).getItem().getName());
        assertEquals(5, itemSlots.get(1).getAmount());
        assertEquals("None", itemSlots.get(2).getItem().getName());
        assertEquals(0, itemSlots.get(2).getAmount());

        assertEquals("Bicycle", keyItemSlots.get(0).getItem().getName());
        assertEquals(5, keyItemSlots.get(0).getAmount());
        assertEquals("SquirtBottle", keyItemSlots.get(1).getItem().getName());
        assertEquals(5, keyItemSlots.get(1).getAmount());
        assertEquals("None", keyItemSlots.get(2).getItem().getName());
        assertEquals(0, keyItemSlots.get(2).getAmount());

        assertEquals("Great Ball", ballSlots.get(0).getItem().getName());
        assertEquals(5, ballSlots.get(0).getAmount());
        assertEquals("Friend Ball", ballSlots.get(1).getItem().getName());
        assertEquals(5, ballSlots.get(1).getAmount());
        assertEquals("None", ballSlots.get(2).getItem().getName());
        assertEquals(0, ballSlots.get(2).getAmount());

        assertEquals("TM01", tmhmSlots.get(0).getItem().getName());
        assertEquals(0, tmhmSlots.get(0).getAmount());
        assertEquals("TM02", tmhmSlots.get(1).getItem().getName());
        assertEquals(0, tmhmSlots.get(1).getAmount());
        assertEquals("TM28", tmhmSlots.get(27).getItem().getName());
        assertEquals(5, tmhmSlots.get(27).getAmount());
        assertEquals("HM01", tmhmSlots.get(50).getItem().getName());
        assertEquals(5, tmhmSlots.get(50).getAmount());
    }
}
