/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import org.junit.Ignore;
import org.junit.Test;
import static org.junit.Assert.*;

public class GUITest {

    // Make sure native libraries are loaded.
    static {
        try {
            JARUtils.initialize();
        } catch(Exception e) {}
    }

    @Test
    public void testAbilityListComboBox() {
        // Make sure trying to create an invalid ComboxBox results in an exception.
        try {
            AbilityListComboBox comboBox = new AbilityListComboBox(0);
            fail("Creating an invalid ComboBox should have failed.");
        } catch(Exception ex) {}

        AbilityListComboBox comboBox = new AbilityListComboBox(6);
        assertEquals(191, comboBox.getItemCount());
        assertEquals("Adaptability", comboBox.getItemAt(0));
        assertEquals("Zen Mode", comboBox.getItemAt(190));
    }

    @Test
    public void testGameListComboBox() {
        // Make sure trying to create an invalid ComboBox results in an exception.
        try {
            GameListComboBox comboBox = new GameListComboBox(7, false);
            fail("Creating an invalid ComboBox should have failed.");
        } catch(Exception ex) {}

        GameListComboBox comboBox = new GameListComboBox(6, true);
        assertEquals(26, comboBox.getItemCount());
        assertEquals("Red", comboBox.getItemAt(0));
        assertEquals("Alpha Sapphire", comboBox.getItemAt(25));
    }

    @Test
    public void testItemListComboBox() {
        // Make sure trying to create an invalid ComboBox results in an exception.
        try {
            ItemListComboBox comboBox = new ItemListComboBox("Not a game");
            fail("Creating an invalid ComboBox should have failed.");
        } catch(Exception ex) {}

        ItemListComboBox comboBox = new ItemListComboBox("HeartGold");
        assertEquals(513, comboBox.getItemCount());
        assertEquals("Adamant Orb", comboBox.getItemAt(0));
        assertEquals("Zoom Lens", comboBox.getItemAt(512));
    }

    @Test
    public void testLocationListComboBox() {
        // Make sure trying to create an invalid ComboBox results in an exception.
        try {
            LocationListComboBox comboBox = new LocationListComboBox("Not a game", false);
            fail("Creating an invalid ComboBox should have failed.");
        } catch(Exception ex) {}

        LocationListComboBox comboBox = new LocationListComboBox("Emerald", false);
        assertEquals(106, comboBox.getItemCount());
        assertEquals("Abandoned Ship", comboBox.getItemAt(0));
        assertEquals("Victory Road", comboBox.getItemAt(105));
    }

    @Test
    public void testMoveListComboBox() {
        // Make sure trying to create an invalid ComboBox results in an exception.
        try {
            MoveListComboBox comboBox = new MoveListComboBox("Not a game");
            fail("Creating an invalid ComboBox should have failed.");
        } catch(Exception ex) {}

        MoveListComboBox comboBox = new MoveListComboBox("Red");
        assertEquals(165, comboBox.getItemCount());
        assertEquals("Pound", comboBox.getItemAt(0));
        assertEquals("Struggle", comboBox.getItemAt(164));
    }

    @Test
    public void testNatureListComboBox() {
        NatureListComboBox comboBox = new NatureListComboBox();
        assertEquals(25, comboBox.getItemCount());
        assertEquals("Hardy", comboBox.getItemAt(0));
        assertEquals("Quirky", comboBox.getItemAt(24));
    }

    @Test
    public void testPokemonListComboBox() {
        // Make sure trying to create an invalid ComboBox results in an exception.
        try {
            PokemonListComboBox comboBox = new PokemonListComboBox(0, false);
            fail("Creating an invalid ComboBox should have failed.");
        } catch(Exception ex) {}

        PokemonListComboBox comboBox = new PokemonListComboBox(1, true);
        assertEquals(151, comboBox.getItemCount());
        assertEquals("Bulbasaur", comboBox.getItemAt(0));
        assertEquals("Mew", comboBox.getItemAt(150));
    }

    @Test
    public void testRegionListComboBox() {
        RegionListComboBox comboBox = new RegionListComboBox();
        assertEquals(7, comboBox.getItemCount());
        assertEquals("Kanto", comboBox.getItemAt(0));
        assertEquals("Kalos", comboBox.getItemAt(6));
    }

    @Ignore("Not implemented")
    @Test
    public void testRibbonTest() {
    }

    @Test
    public void testSuperTrainingMedalListComboBox() {
        SuperTrainingMedalListComboBox comboBox = new SuperTrainingMedalListComboBox();
        assertEquals(30, comboBox.getItemCount());
        assertEquals("Sp. Atk Level 1", comboBox.getItemAt(0));
        assertEquals("The Battle for the Best!", comboBox.getItemAt(29));
    }

    @Test
    public void testTypeListComboBox() {
        // Make sure trying to create an invalid ComboBox results in an exception.
        try {
            TypeListComboBox comboBox = new TypeListComboBox("Not a game");
            fail("Creating an invalid ComboBox should have failed.");
        } catch(Exception ex) {}

        TypeListComboBox comboBox = new TypeListComboBox("Alpha Sapphire");
        assertEquals(18, comboBox.getItemCount());
        assertEquals("Normal", comboBox.getItemAt(0));
        assertEquals("Fairy", comboBox.getItemAt(17));
    }
}
