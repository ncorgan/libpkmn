/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import static org.junit.Assert.*;

public class ItemTest {

    // Make sure native libraries are loaded.
    static {
        try {
            JARUtils.initialize();
        } catch(Exception e) {}
    }

    protected String testGame;

    protected void testItemListEmptySlots(
        ItemList itemList
    ) {
        assertEquals(0, itemList.getNumItems());
        String game = itemList.getGame();
        ItemSlotVector itemSlots = itemList.asVector();
        assertEquals(itemList.getCapacity(), itemSlots.size());

        int size = itemSlots.size();
        for(int i = 0; i < size; ++i) {
            ItemSlot itemSlot = itemSlots.get(i);
            assertEquals("None", itemSlot.getItem().getName());
            assertEquals(game, itemSlot.getItem().getGame());
            assertEquals(0, itemSlot.getAmount());
        }
    }

    protected void testItemListOutOfRangeError(
        ItemList itemList,
        String itemName
    ) {
        int numItems = itemList.getNumItems();

        try {
            itemList.add(itemName, 0);
            fail("Attempting to add 0 items should have failed.");
        } catch(Exception e) {}
        try {
            itemList.add(itemName, 100);
            fail("Attempting to add 100 items should have failed.");
        } catch(Exception e) {}
        try {
            itemList.remove(itemName, 0);
            fail("Attempting to remove 0 items should have failed.");
        } catch(Exception e) {}
        try {
            itemList.remove(itemName, 100);
            fail("Attempting to remove 100 items should have failed.");
        } catch(Exception e) {}

        // This shouldn't have added any items.
        assertEquals(numItems, itemList.getNumItems());
    }

    protected void testItemBagInvalidItems(
        ItemBag itemBag,
        String[] itemNames
    ) {
        for(int i = 0; i < itemNames.length; ++i) {
            try {
                itemBag.add(itemNames[i], 1);
                fail("Attempting to add " + itemNames[i] + " should have failed.");
            } catch(Exception e) {}
        }
    }

    protected void testItemListInvalidItems(
        ItemList itemList,
        String[] itemNames
    ) {
        for(int i = 0; i < itemNames.length; ++i) {
            try {
                itemList.add(itemNames[i], 1);
                fail("Attempting to add " + itemNames[i] + " should have failed.");
            } catch(Exception e) {}
        }
    }

    protected void testItemListAddRemove(
        ItemList itemList,
        String[] itemNames
    ) {
        assertEquals(0, itemList.getNumItems());
        assertEquals(8, itemNames.length);

        itemList.add(itemNames[0], 30);
        itemList.add(itemNames[1], 99);
        itemList.add(itemNames[2], 1);

        assertEquals(itemNames[0], itemList.at(0).getItem().getName());
        assertEquals(30, itemList.at(0).getAmount());
        assertEquals(itemNames[1], itemList.at(1).getItem().getName());
        assertEquals(99, itemList.at(1).getAmount());
        assertEquals(itemNames[2], itemList.at(2).getItem().getName());
        assertEquals(1, itemList.at(2).getAmount());
        assertEquals(3, itemList.getNumItems());

        itemList.add(itemNames[2], 15);
        assertEquals(itemNames[0], itemList.at(0).getItem().getName());
        assertEquals(30, itemList.at(0).getAmount());
        assertEquals(itemNames[1], itemList.at(1).getItem().getName());
        assertEquals(99, itemList.at(1).getAmount());
        assertEquals(itemNames[2], itemList.at(2).getItem().getName());
        assertEquals(16, itemList.at(2).getAmount());
        assertEquals(3, itemList.getNumItems());

        itemList.remove(itemNames[1], 20);
        assertEquals(itemNames[0], itemList.at(0).getItem().getName());
        assertEquals(30, itemList.at(0).getAmount());
        assertEquals(itemNames[1], itemList.at(1).getItem().getName());
        assertEquals(79, itemList.at(1).getAmount());
        assertEquals(itemNames[2], itemList.at(2).getItem().getName());
        assertEquals(16, itemList.at(2).getAmount());
        assertEquals(3, itemList.getNumItems());

        itemList.move(0, 1);
        assertEquals(itemNames[1], itemList.at(0).getItem().getName());
        assertEquals(79, itemList.at(0).getAmount());
        assertEquals(itemNames[0], itemList.at(1).getItem().getName());
        assertEquals(30, itemList.at(1).getAmount());
        assertEquals(itemNames[2], itemList.at(2).getItem().getName());
        assertEquals(16, itemList.at(2).getAmount());
        assertEquals(3, itemList.getNumItems());

        itemList.remove(itemNames[0], 30);
        assertEquals(itemNames[1], itemList.at(0).getItem().getName());
        assertEquals(79, itemList.at(0).getAmount());
        assertEquals(itemNames[2], itemList.at(1).getItem().getName());
        assertEquals(16, itemList.at(1).getAmount());
        assertEquals("None", itemList.at(2).getItem().getName());
        assertEquals(0, itemList.at(2).getAmount());
        assertEquals(2, itemList.getNumItems());

        itemList.add(itemNames[3], 90);
        assertEquals(itemNames[1], itemList.at(0).getItem().getName());
        assertEquals(79, itemList.at(0).getAmount());
        assertEquals(itemNames[2], itemList.at(1).getItem().getName());
        assertEquals(16, itemList.at(1).getAmount());
        assertEquals(itemNames[3], itemList.at(2).getItem().getName());
        assertEquals(90, itemList.at(2).getAmount());
        assertEquals(3, itemList.getNumItems());

        itemList.add(itemNames[4], 2);
        assertEquals(itemNames[1], itemList.at(0).getItem().getName());
        assertEquals(79, itemList.at(0).getAmount());
        assertEquals(itemNames[2], itemList.at(1).getItem().getName());
        assertEquals(16, itemList.at(1).getAmount());
        assertEquals(itemNames[3], itemList.at(2).getItem().getName());
        assertEquals(90, itemList.at(2).getAmount());
        assertEquals(itemNames[4], itemList.at(3).getItem().getName());
        assertEquals(2, itemList.at(3).getAmount());
        assertEquals(4, itemList.getNumItems());

        itemList.remove(itemNames[1], 30);
        assertEquals(itemNames[1], itemList.at(0).getItem().getName());
        assertEquals(49, itemList.at(0).getAmount());
        assertEquals(itemNames[2], itemList.at(1).getItem().getName());
        assertEquals(16, itemList.at(1).getAmount());
        assertEquals(itemNames[3], itemList.at(2).getItem().getName());
        assertEquals(90, itemList.at(2).getAmount());
        assertEquals(itemNames[4], itemList.at(3).getItem().getName());
        assertEquals(2, itemList.at(3).getAmount());
        assertEquals(4, itemList.getNumItems());

        itemList.add(itemNames[5], 12);
        assertEquals(itemNames[1], itemList.at(0).getItem().getName());
        assertEquals(49, itemList.at(0).getAmount());
        assertEquals(itemNames[2], itemList.at(1).getItem().getName());
        assertEquals(16, itemList.at(1).getAmount());
        assertEquals(itemNames[3], itemList.at(2).getItem().getName());
        assertEquals(90, itemList.at(2).getAmount());
        assertEquals(itemNames[4], itemList.at(3).getItem().getName());
        assertEquals(2, itemList.at(3).getAmount());
        assertEquals(itemNames[5], itemList.at(4).getItem().getName());
        assertEquals(12, itemList.at(4).getAmount());
        assertEquals(5, itemList.getNumItems());

        itemList.remove(itemNames[2], 16);
        assertEquals(itemNames[1], itemList.at(0).getItem().getName());
        assertEquals(49, itemList.at(0).getAmount());
        assertEquals(itemNames[3], itemList.at(1).getItem().getName());
        assertEquals(90, itemList.at(1).getAmount());
        assertEquals(itemNames[4], itemList.at(2).getItem().getName());
        assertEquals(2, itemList.at(2).getAmount());
        assertEquals(itemNames[5], itemList.at(3).getItem().getName());
        assertEquals(12, itemList.at(3).getAmount());
        assertEquals(4, itemList.getNumItems());

        itemList.add(itemNames[6], 65);
        assertEquals(itemNames[1], itemList.at(0).getItem().getName());
        assertEquals(49, itemList.at(0).getAmount());
        assertEquals(itemNames[3], itemList.at(1).getItem().getName());
        assertEquals(90, itemList.at(1).getAmount());
        assertEquals(itemNames[4], itemList.at(2).getItem().getName());
        assertEquals(2, itemList.at(2).getAmount());
        assertEquals(itemNames[5], itemList.at(3).getItem().getName());
        assertEquals(12, itemList.at(3).getAmount());
        assertEquals(itemNames[6], itemList.at(4).getItem().getName());
        assertEquals(65, itemList.at(4).getAmount());
        assertEquals(5, itemList.getNumItems());

        itemList.add(itemNames[7], 6);
        assertEquals(itemNames[1], itemList.at(0).getItem().getName());
        assertEquals(49, itemList.at(0).getAmount());
        assertEquals(itemNames[3], itemList.at(1).getItem().getName());
        assertEquals(90, itemList.at(1).getAmount());
        assertEquals(itemNames[4], itemList.at(2).getItem().getName());
        assertEquals(2, itemList.at(2).getAmount());
        assertEquals(itemNames[5], itemList.at(3).getItem().getName());
        assertEquals(12, itemList.at(3).getAmount());
        assertEquals(itemNames[6], itemList.at(4).getItem().getName());
        assertEquals(65, itemList.at(4).getAmount());
        assertEquals(itemNames[7], itemList.at(5).getItem().getName());
        assertEquals(6, itemList.at(5).getAmount());
        assertEquals(6, itemList.getNumItems());

        // Now remove everything.
        itemList.remove(itemNames[1], 49);
        itemList.remove(itemNames[3], 90);
        itemList.remove(itemNames[4], 2);
        itemList.remove(itemNames[5], 12);
        itemList.remove(itemNames[6], 65);
        itemList.remove(itemNames[7], 6);
        for(int i = 0; i < 6; ++i) {
            assertEquals("None", itemList.at(i).getItem().getName());
            assertEquals(0, itemList.at(i).getAmount());
        }
        assertEquals(0, itemList.getNumItems());
    }
}
