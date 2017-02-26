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
public class Gen1ItemTest extends ItemTest {

    @Parameters
    public static Object[] data() {
        return new Object[] {"Red", "Blue", "Yellow"};
    }

    private static String[] itemNames = {
        "Potion", "Great Ball", "Ether", "PP Up",
        "TM34", "Moon Stone", "Bicycle", "Full Heal"
    };
    private static String[] wrongGenerationItemNames = {
        "Amulet Coin", "Apicot Berry", "Air Mail", "Air Balloon", "Aqua Suit"
    };

    public Gen1ItemTest(
        String game
    ) {
        this.testGame = game;
    }

    private void testItemListCommon(
        ItemList itemList
    ) {
        // Make sure item slots start as correctly empty.
        this.testItemListEmptySlots(itemList);

        // Confirm exceptions are thrown when expected.
        this.testItemListOutOfRangeError(
            itemList,
            "Potion"
        );

        // Confirm items from later generations can't be added.
        this.testItemListInvalidItems(
            itemList,
            wrongGenerationItemNames
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        this.testItemListAddRemove(
            itemList,
            itemNames
        );

        StringVector validItems = itemList.getValidItems();
        StringVector fullItemList = Database.getItemList(this.testGame);
        assertEquals(validItems.size(), fullItemList.size());
    }

    private void testItemPocket(
        ItemList itemPocket
    ) {
        assertEquals("Items", itemPocket.getName());
        assertEquals(this.testGame, itemPocket.getGame());
        assertEquals(20, itemPocket.getCapacity());
        assertEquals(20, itemPocket.asVector().size());

        this.testItemListCommon(itemPocket);
    }

    private void testItemPC(
        ItemList itemPC
    ) {
        assertEquals("PC", itemPC.getName());
        assertEquals(this.testGame, itemPC.getGame());
        assertEquals(50, itemPC.getCapacity());
        assertEquals(50, itemPC.asVector().size());

        this.testItemListCommon(itemPC);
    }

    @Test
    public void testItemPocket() {
        ItemList itemPocket = new ItemList("Items", this.testGame);
        testItemPocket(itemPocket);
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
        assertEquals(1, itemPockets.size());
        assertEquals("Items", itemPockets.get("Items").getName());
        assertEquals(this.testGame, itemPockets.get("Items").getGame());

        StringVector pocketNames = itemBag.getPocketNames();
        assertEquals(1, pocketNames.size());
        assertEquals("Items", pocketNames.get(0));

        this.testItemPocket(itemPockets.get("Items"));

        // Make sure attempting to add invalid items through the bag fails.
        this.testItemBagInvalidItems(
            itemBag,
            wrongGenerationItemNames
        );

        ItemList itemPocket = itemBag.getPocket("Items");

        // Make sure adding items through the bag adds to the pocket.
        assertEquals(0, itemPocket.getNumItems());
        for(int i = 0; i < 8; ++i) {
            itemBag.add(
                itemNames[i],
                i+1
            );
        }
        ItemSlotVector itemSlots = itemPocket.asVector();
        for(int i = 0; i < 8; ++i) {
            assertEquals(itemNames[i], itemSlots.get(i).getItem().getName());
            assertEquals(i+1, itemSlots.get(i).getAmount());
        }
        assertEquals("None", itemSlots.get(8).getItem().getName());
        assertEquals(0, itemSlots.get(8).getAmount());

        // Make sure removing items through the bag removes from the pocket.
        for(int i = 0; i < 8; ++i) {
            itemBag.remove(
                itemNames[i],
                i+1
            );
        }
        itemSlots = itemPocket.asVector();
        for(int i = 0; i < 9; ++i) {
            assertEquals("None", itemSlots.get(i).getItem().getName());
            assertEquals(0, itemSlots.get(i).getAmount());
        }
    }
}
