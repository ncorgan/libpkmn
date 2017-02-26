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

public class ListsTest {

    // Make sure native libraries are loaded.
    static {
        try {
            JARUtils.initialize();
        } catch(Exception e) {}
    }

    @Test
    public void testAbilityList() {
        // Make sure trying to create an invalid list results in an exception.
        try {
            StringVector invalidAbilityList = nc.PKMN.Database.getAbilityList(0);
            fail("Creating an invalid ability list should have failed.");
        } catch(Exception ex) {}

        StringVector abilityList = nc.PKMN.Database.getAbilityList(6);
        assertEquals(191, abilityList.size());
        assertEquals("Adaptability", abilityList.get(0));
        assertEquals("Zen Mode", abilityList.get(190));
    }

    @Test
    public void testGameList() {
        // Make sure trying to create an invalid list results in an exception.
        try {
            StringVector invalidGameList = nc.PKMN.Database.getGameList(7, false);
            fail("Creating an invalid game list should have failed.");
        } catch(Exception ex) {}

        StringVector gameList = nc.PKMN.Database.getGameList(6, true);
        assertEquals(26, gameList.size());
        assertEquals("Red", gameList.get(0));
        assertEquals("Alpha Sapphire", gameList.get(25));
    }

    @Test
    public void testItemList() {
        // Make sure trying to create an invalid list results in an exception.
        try {
            StringVector invalidItemList = nc.PKMN.Database.getItemList("Not a game");
            fail("Creating an invalid item list should have failed.");
        } catch(Exception ex) {}

        StringVector itemList = nc.PKMN.Database.getItemList("HeartGold");
        assertEquals(513, itemList.size());
        assertEquals("Adamant Orb", itemList.get(0));
        assertEquals("Zoom Lens", itemList.get(512));
    }

    @Test
    public void testLocationList() {
        // Make sure trying to create an invalid list results in an exception.
        try {
            StringVector invalidLocationList = nc.PKMN.Database.getLocationList("Not a game", false);
            fail("Creating an invalid location list should have failed.");
        } catch(Exception ex) {}

        StringVector locationList = nc.PKMN.Database.getLocationList("Emerald", false);
        assertEquals(106, locationList.size());
        assertEquals("Abandoned Ship", locationList.get(0));
        assertEquals("Victory Road", locationList.get(105));
    }

    @Test
    public void testMoveList() {
        // Make sure trying to create an invalid list results in an exception.
        try {
            StringVector invalidMoveList = nc.PKMN.Database.getMoveList("Not a game");
            fail("Creating an invalid move list should have failed.");
        } catch(Exception ex) {}

        StringVector moveList = nc.PKMN.Database.getMoveList("Red");
        assertEquals(165, moveList.size());
        assertEquals("Pound", moveList.get(0));
        assertEquals("Struggle", moveList.get(164));
    }

    @Test
    public void testNatureList() {
        StringVector natureList = nc.PKMN.Database.getNatureList();
        assertEquals(25, natureList.size());
        assertEquals("Hardy", natureList.get(0));
        assertEquals("Quirky", natureList.get(24));
    }

    @Test
    public void testPokemonList() {
        // Make sure trying to create an invalid list results in an exception.
        try {
            StringVector invalidPokemonList = nc.PKMN.Database.getPokemonList(0, false);
            fail("Creating an invalid Pokémon list should have failed.");
        } catch(Exception ex) {}

        StringVector pokemonList = nc.PKMN.Database.getPokemonList(1, true);
        assertEquals(151, pokemonList.size());
        assertEquals("Bulbasaur", pokemonList.get(0));
        assertEquals("Mew", pokemonList.get(150));
    }

    @Test
    public void testRegionList() {
        StringVector regionList = nc.PKMN.Database.getRegionList();
        assertEquals(7, regionList.size());
        assertEquals("Kanto", regionList.get(0));
        assertEquals("Kalos", regionList.get(6));
    }

    @Ignore("Not implemented")
    @Test
    public void testRibbonTest() {
    }

    @Test
    public void testSuperTrainingMedalList() {
        StringVector superTrainingMedalList = nc.PKMN.Database.getSuperTrainingMedalList();
        assertEquals(30, superTrainingMedalList.size());
        assertEquals("Sp. Atk Level 1", superTrainingMedalList.get(0));
        assertEquals("The Battle for the Best!", superTrainingMedalList.get(29));
    }

    @Test
    public void testTypeList() {
        // Make sure trying to create an invalid list results in an exception.
        try {
            StringVector invalidTypeList = nc.PKMN.Database.getTypeList("Not a game");
            fail("Creating an invalid type list should have failed.");
        } catch(Exception ex) {}

        StringVector typeList = nc.PKMN.Database.getTypeList("Alpha Sapphire");
        assertEquals(18, typeList.size());
        assertEquals("Normal", typeList.get(0));
        assertEquals("Fairy", typeList.get(17));
    }
}
