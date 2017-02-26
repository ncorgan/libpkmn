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
public class GBAItemTest extends ItemTest {

    @Parameters
    public static Object[] data() {
        return new Object[] {
            "Ruby", "Sapphire",
            "Emerald",
            "FireRed", "LeafGreen"
        };
    }

    private static String[] allPocketItemNames = {
        "Potion", "Mach Bike", "Great Ball", "TM01",
        "Aspear Berry", "Wailmer Pail", "Master Ball", "HM04"
    };
    private static String[] wrongGameAllPocketItemNames = {
        "Pink Bow", "Black Sludge",
        "Ein File S", "Gonzap's Key",
        "GS Ball", "Poffin Items",
        "TM51",
        "Berry", "Occa Berry"
    };

    public GBAItemTest(
        String game
    ) {
        this.testGame = game;
    }

    private void testItemPocket(
        ItemList itemPocket
    ) {
        assertEquals("Items", itemPocket.getName());
        assertEquals(this.testGame, itemPocket.getGame());

        int capacity = 0;
        if(this.testGame.equals("Ruby") || this.testGame.equals("Sapphire")) {
            capacity = 20;
        } else if(this.testGame.equals("Emerald")) {
            capacity = 30;
        } else {
            capacity = 42;
        }

        assertEquals(capacity, itemPocket.getCapacity());
        assertEquals(capacity, itemPocket.asVector().size());

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
            new String[] {"Bicycle", "Master Ball", "HM01", "Razz Berry"}
        );

        // Confirm items from other generations can't be added.
        this.testItemListInvalidItems(
            itemPocket,
            new String[] {"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"}
        );

        // Confirm items from Gamecube games can't be added.
        this.testItemListInvalidItems(
            itemPocket,
            new String[] {"Time Flute", "Poké Snack"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            itemPocket,
            new String[] {
                "Potion", "Orange Mail", "Lava Cookie", "Stardust",
                "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"
            }
        );

        StringVector validItems = itemPocket.getValidItems();
        assertTrue(validItems.size() > 0);
    }

    private void testKeyItemPocket(
        ItemList keyItemPocket
    ) {
        assertEquals("Key Items", keyItemPocket.getName());
        assertEquals(this.testGame, keyItemPocket.getGame());

        int capacity = 0;
        if(this.testGame.equals("Ruby") || this.testGame.equals("Sapphire")) {
            capacity = 20;
        } else {
            capacity = 30;
        }

        assertEquals(capacity, keyItemPocket.getCapacity());
        assertEquals(capacity, keyItemPocket.asVector().size());

        // Make sure item slots start as correctly empty.
        this.testItemListEmptySlots(keyItemPocket);

        // Confirm exceptions are thrown when expected.
        this.testItemListOutOfRangeError(
            keyItemPocket,
            "Basement Key"
        );

        // Confirm items from other pockets can't be added.
        this.testItemListInvalidItems(
            keyItemPocket,
            new String[] {"Potion", "Master Ball", "HM01", "Razz Berry"}
        );

        // Confirm items from other generations can't be added.
        this.testItemListInvalidItems(
            keyItemPocket,
            new String[] {"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"}
        );

        // Confirm items from incompatible Generation IIIgames can't be added.
        this.testItemListInvalidItems(
            keyItemPocket,
            new String[] {
                // Gamecube
                "Ein File S", "Powerup Part",
                "Gonzap's Key", "Krane Memo 1"
            }
        );
        if(this.testGame.equals("Ruby") || this.testGame.equals("Sapphire")) {
            this.testItemListInvalidItems(
                keyItemPocket,
                new String[] {
                    // FireRed/LeafGreen
                    "Helix Fossil", "Tea", "Ruby"
                }
            );
        }
        if(!this.testGame.equals("Emerald")) {
            this.testItemListInvalidItems(
                keyItemPocket,
                new String[] {
                    // Emerald
                    "Magma Emblem", "Old Sea Map"
                }
            );
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            keyItemPocket,
            new String[] {
                "Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
                "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"
            }
        );

        StringVector validItems = keyItemPocket.getValidItems();
        assertTrue(validItems.size() > 0);
    }

    private void testBallPocket(
        ItemList ballPocket
    ) {
        assertEquals("Poké Balls", ballPocket.getName());
        assertEquals(this.testGame, ballPocket.getGame());

        int capacity = 0;
        if(this.testGame.equals("FireRed") || this.testGame.equals("LeafGreen")) {
            capacity = 13;
        } else {
            capacity = 16;
        }

        assertEquals(capacity, ballPocket.getCapacity());
        assertEquals(capacity, ballPocket.asVector().size());

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
            new String[] {"Potion", "Bicycle", "HM01", "Razz Berry"}
        );

        // Confirm items from other generations can't be added.
        this.testItemListInvalidItems(
            ballPocket,
            new String[] {"Moon Ball", "Heal Ball", "Dream Ball"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            ballPocket,
            new String[] {
                "Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
                "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"
            }
        );

        StringVector validItems = ballPocket.getValidItems();
        assertTrue(validItems.size() > 0);
    }

    private void testTMHMPocket(
        ItemList tmhmPocket
    ) {
        assertEquals(this.testGame, tmhmPocket.getGame());

        String pocketName = "";
        int capacity = 0;
        if(this.testGame.equals("FireRed") || this.testGame.equals("LeafGreen")) {
            pocketName = "TM Case";
            capacity = 58;
        } else {
            pocketName = "TMs & HMs";
            capacity = 64;
        }

        assertEquals(pocketName, tmhmPocket.getName());
        assertEquals(capacity, tmhmPocket.getCapacity());
        assertEquals(capacity, tmhmPocket.asVector().size());

        // Make sure item slots start as correctly empty.
        this.testItemListEmptySlots(tmhmPocket);

        // Confirm exceptions are thrown when expected.
        this.testItemListOutOfRangeError(
            tmhmPocket,
            "TM01"
        );

        // Confirm items from other pockets can't be added.
        this.testItemListInvalidItems(
            tmhmPocket,
            new String[] {"Potion", "Bicycle", "Great Ball", "Razz Berry"}
        );

        // Confirm items from other generations can't be added.
        this.testItemListInvalidItems(
            tmhmPocket,
            new String[] {"TM51"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            tmhmPocket,
            new String[] {
                "TM01", "TM02", "TM03", "TM04",
                "HM01", "HM02", "HM03", "HM04"
            }
        );

        StringVector validItems = tmhmPocket.getValidItems();
        assertTrue(validItems.size() > 0);
    }

    private void testBerryPocket(
        ItemList berryPocket
    ) {
        assertEquals(this.testGame, berryPocket.getGame());

        String pocketName = "";
        int capacity = 0;
        if(this.testGame.equals("FireRed") || this.testGame.equals("LeafGreen")) {
            pocketName = "Berry Pouch";
            capacity = 43;
        } else {
            pocketName = "Berries";
            capacity = 46;
        }

        assertEquals(pocketName, berryPocket.getName());
        assertEquals(capacity, berryPocket.getCapacity());
        assertEquals(capacity, berryPocket.asVector().size());

        // Make sure item slots start as correctly empty.
        this.testItemListEmptySlots(berryPocket);

        // Confirm exceptions are thrown when expected.
        this.testItemListOutOfRangeError(
            berryPocket,
            "Razz Berry"
        );

        // Confirm items from other pockets can't be added.
        this.testItemListInvalidItems(
            berryPocket,
            new String[] {"Potion", "Bicycle", "Great Ball", "HM02"}
        );

        // Confirm items from other generations can't be added.
        this.testItemListInvalidItems(
            berryPocket,
            new String[] {"Berry", "Occa Berry", "Roseli Berry"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            berryPocket,
            new String[] {
                "Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
                "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"
            }
        );

        StringVector validItems = berryPocket.getValidItems();
        assertTrue(validItems.size() > 0);
    }

    private void testItemPC(
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

        // Confirm items from other generations or invalid Generation III games can't be added.
        this.testItemListInvalidItems(
            itemPC,
            wrongGameAllPocketItemNames
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            itemPC,
            allPocketItemNames
        );

        StringVector validItems = itemPC.getValidItems();
        assertTrue(validItems.size() > 0);
    }

    @Test
    public void testItemPocket() {
        ItemList itemPocket = new ItemList("Items", this.testGame);
        testItemPocket(itemPocket);
    }

    @Test
    public void testKeyItemPocket() {
        ItemList keyItemPocket = new ItemList("Key Items", this.testGame);
        testKeyItemPocket(keyItemPocket);
    }

    @Test
    public void testBallPocket() {
        ItemList ballPocket = new ItemList("Poké Balls", this.testGame);
        testBallPocket(ballPocket);
    }

    @Test
    public void testTMHMPocket() {
        ItemList tmhmPocket;

        if(this.testGame.equals("FireRed") || this.testGame.equals("LeafGreen")) {
            tmhmPocket = new ItemList("TM Case", this.testGame);
        } else {
            tmhmPocket = new ItemList("TMs & HMs", this.testGame);
        }
       
        testTMHMPocket(tmhmPocket);
    }

    @Test
    public void testBerryPocket() {
        ItemList berryPocket;

        if(this.testGame.equals("FireRed") || this.testGame.equals("LeafGreen")) {
            berryPocket = new ItemList("Berry Pouch", this.testGame);
        } else {
            berryPocket = new ItemList("Berries", this.testGame);
        }
       
        testBerryPocket(berryPocket);
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

        String tmhmPocketName = "";
        String berryPocketName = "";
        if(this.testGame.equals("FireRed") || this.testGame.equals("LeafGreen")) {
            tmhmPocketName = "TM Case";
            berryPocketName = "Berry Pouch";
        } else {
            tmhmPocketName = "TMs & HMs";
            berryPocketName = "Berries";
        }

        StringItemListMap itemPockets = itemBag.getPockets();
        assertEquals(5, itemPockets.size());
        assertTrue(itemPockets.containsKey("Items"));
        assertTrue(itemPockets.containsKey("Key Items"));
        assertTrue(itemPockets.containsKey("Poké Balls"));
        assertTrue(itemPockets.containsKey(tmhmPocketName));
        assertTrue(itemPockets.containsKey(berryPocketName));

        StringVector pocketNames = itemBag.getPocketNames();
        assertEquals(5, pocketNames.size());
        assertTrue(pocketNames.contains("Items"));
        assertTrue(pocketNames.contains("Key Items"));
        assertTrue(pocketNames.contains("Poké Balls"));
        assertTrue(pocketNames.contains(tmhmPocketName));
        assertTrue(pocketNames.contains(berryPocketName));

        this.testItemPocket(itemPockets.get("Items"));
        this.testKeyItemPocket(itemPockets.get("Key Items"));
        this.testBallPocket(itemPockets.get("Poké Balls"));
        this.testTMHMPocket(itemPockets.get(tmhmPocketName));
        this.testBerryPocket(itemPockets.get(berryPocketName));

        // Make sure adding items through the bag adds to the pocket.
        assertEquals(0, itemPockets.get("Items").getNumItems());
        assertEquals(0, itemPockets.get("Key Items").getNumItems());
        assertEquals(0, itemPockets.get("Poké Balls").getNumItems());
        assertEquals(0, itemPockets.get(tmhmPocketName).getNumItems());
        assertEquals(0, itemPockets.get(berryPocketName).getNumItems());
        for(int i = 0; i < 8; ++i) {
            itemBag.add(
                allPocketItemNames[i],
                5
            );
        }

        ItemSlotVector itemSlots = itemPockets.get("Items").asVector();
        ItemSlotVector keyItemSlots = itemPockets.get("Key Items").asVector();
        ItemSlotVector ballSlots = itemPockets.get("Poké Balls").asVector();
        ItemSlotVector tmhmSlots = itemPockets.get(tmhmPocketName).asVector();
        ItemSlotVector berrySlots = itemPockets.get(berryPocketName).asVector();

        assertEquals("Potion", itemSlots.get(0).getItem().getName());
        assertEquals(5, itemSlots.get(0).getAmount());
        assertEquals("None", itemSlots.get(1).getItem().getName());
        assertEquals(0, itemSlots.get(1).getAmount());

        assertEquals("Mach Bike", keyItemSlots.get(0).getItem().getName());
        assertEquals(5, keyItemSlots.get(0).getAmount());
        assertEquals("Wailmer Pail", keyItemSlots.get(1).getItem().getName());
        assertEquals(5, keyItemSlots.get(1).getAmount());
        assertEquals("None", keyItemSlots.get(2).getItem().getName());
        assertEquals(0, keyItemSlots.get(2).getAmount());

        assertEquals("Great Ball", ballSlots.get(0).getItem().getName());
        assertEquals(5, ballSlots.get(0).getAmount());
        assertEquals("Master Ball", ballSlots.get(1).getItem().getName());
        assertEquals(5, ballSlots.get(1).getAmount());
        assertEquals("None", ballSlots.get(2).getItem().getName());
        assertEquals(0, ballSlots.get(2).getAmount());

        assertEquals("TM01", tmhmSlots.get(0).getItem().getName());
        assertEquals(5, tmhmSlots.get(0).getAmount());
        assertEquals("HM04", tmhmSlots.get(1).getItem().getName());
        assertEquals(5, tmhmSlots.get(1).getAmount());
        assertEquals("None", tmhmSlots.get(2).getItem().getName());
        assertEquals(0, tmhmSlots.get(2).getAmount());

        assertEquals("Aspear Berry", berrySlots.get(0).getItem().getName());
        assertEquals(5, berrySlots.get(0).getAmount());
        assertEquals("None", berrySlots.get(1).getItem().getName());
        assertEquals(0, berrySlots.get(1).getAmount());
    }
}
