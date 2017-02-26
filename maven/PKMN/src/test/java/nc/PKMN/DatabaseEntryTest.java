/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import java.nio.file.*;
import java.nio.file.Paths;

import org.junit.Test;
import static org.junit.Assert.*;

public class DatabaseEntryTest {

    // Make sure native libraries are loaded.
    static {
        try {
            JARUtils.initialize();
        } catch(Exception e) {}
    }

    @Test
    public void testItemEntry() {
        // Make sure trying to create an invalid entry results in an exception.
        try {
            ItemEntry itemEntry = new ItemEntry("Not an item", "SoulSilver");
            fail("Attempting to create an invalid entry should have resulted in an exception.");
        } catch(Exception ex) {}
        try {
            ItemEntry itemEntry = new ItemEntry("Fast Ball", "Not a game");
            fail("Attempting to create an invalid entry should have resulted in an exception.");
        } catch(Exception ex) {}
        try {
            ItemEntry itemEntry = new ItemEntry("Fast Ball", "Red");
            fail("Attempting to create an invalid entry should have resulted in an exception.");
        } catch(Exception ex) {}

        ItemEntry itemEntry = new ItemEntry("Fast Ball", "SoulSilver");

        assertEquals("Fast Ball", itemEntry.getName());
        assertEquals("SoulSilver", itemEntry.getGame());
        assertEquals("Apricorn balls", itemEntry.getCategory());
        assertEquals("Poké Balls", itemEntry.getPocket());

        // Just make sure it works.
        String description = itemEntry.getDescription();

        assertTrue(itemEntry.holdable());
        assertEquals(0, itemEntry.getFlingPower());
        assertEquals("None", itemEntry.getFlingEffect());

        // TODO: equality checks
    }

    @Test
    public void testMoveEntry() {
        // Make sure trying to create an invalid entry results in an exception.
        try {
            MoveEntry moveEntry = new MoveEntry("Not a move", "Silver");
            fail("Attempting to create an invalid entry should have resulted in an exception.");
        } catch(Exception ex) {}
        try {
            MoveEntry moveEntry = new MoveEntry("Octazooka", "Not a game");
            fail("Attempting to create an invalid entry should have resulted in an exception.");
        } catch(Exception ex) {}
        try {
            MoveEntry moveEntry = new MoveEntry("Octazooka", "Red");
            fail("Attempting to create an invalid entry should have resulted in an exception.");
        } catch(Exception ex) {}

        MoveEntry moveEntry = new MoveEntry("Octazooka", "Silver");

        assertEquals("Octazooka", moveEntry.getName());
        assertEquals("Silver", moveEntry.getGame());
        assertEquals("Water", moveEntry.getType());

        // Just make sure it works.
        String description = moveEntry.getDescription();

        assertEquals("Selected Pokémon", moveEntry.getTarget());
        assertEquals("Special", moveEntry.getDamageClass());
        assertEquals(65, moveEntry.getBasePower());

        int[] PPs = new int[]{10, 12, 14, 16};
        for(int i = 0; i < 4; ++i) {
            assertEquals(PPs[i], moveEntry.getPP(i));
        }

        assertEquals(0.85, moveEntry.getAccuracy(), 0.0001);
        assertEquals(0, moveEntry.getPriority());

        // Just make sure it works.
        String effect = moveEntry.getEffect();

        assertEquals("None", moveEntry.getContestType());
        assertEquals("None", moveEntry.getContestEffect());
        assertEquals("None", moveEntry.getSuperContestEffect());

        // TODO: equality checks
    }

    @Test
    public void testPokemonEntry() throws java.io.IOException {
        // Make sure trying to create an invalid entry results in an exception.
        try {
            PokemonEntry pokemonEntry = new PokemonEntry("Invalid", "Black 2", "Sunny");
            fail("Attempting to create an invalid entry should have resulted in an exception.");
        } catch(Exception ex) {}
        try {
            PokemonEntry pokemonEntry = new PokemonEntry("Castform", "Not a game", "Sunny");
            fail("Attempting to create an invalid entry should have resulted in an exception.");
        } catch(Exception ex) {}
        try {
            PokemonEntry pokemonEntry = new PokemonEntry("Castform", "Black 2", "Not a form");
            fail("Attempting to create an invalid entry should have resulted in an exception.");
        } catch(Exception ex) {}

        PokemonEntry pokemonEntry = new PokemonEntry("Stunfisk", "Black 2", "");

        assertEquals("Stunfisk", pokemonEntry.getName());
        assertEquals("Black 2", pokemonEntry.getGame());
        assertEquals("Trap", pokemonEntry.getSpecies());
        assertEquals("Standard", pokemonEntry.getForm());

        // Just make sure this works.
        String pokedexEntry = pokemonEntry.getPokedexEntry();

        assertEquals(0.7, pokemonEntry.getHeight(), 0.0001);
        assertEquals(11.0, pokemonEntry.getWeight(), 0.0001);
        assertEquals(0.5, pokemonEntry.getChanceMale(), 0.0001);
        assertEquals(0.5, pokemonEntry.getChanceFemale(), 0.0001);
        assertFalse(pokemonEntry.hasGenderDifferences());
        assertEquals(70, pokemonEntry.getBaseFriendship());

        StringPair types = pokemonEntry.getTypes();
        assertEquals("Ground", types.getFirst());
        assertEquals("Electric", types.getSecond());

        StringPair abilities = pokemonEntry.getAbilities();
        assertEquals("Static", abilities.getFirst());
        assertEquals("Limber", abilities.getSecond());

        assertEquals("Sand Veil", pokemonEntry.getHiddenAbility());

        StringPair eggGroups = pokemonEntry.getEggGroups();
        assertEquals("Water 1", eggGroups.getFirst());
        assertEquals("Amorphous", eggGroups.getSecond());

        StringIntMap baseStats = pokemonEntry.getBaseStats();
        assertEquals(109, baseStats.get("HP"));
        assertEquals(66, baseStats.get("Attack"));
        assertEquals(84, baseStats.get("Defense"));
        assertEquals(32, baseStats.get("Speed"));
        assertEquals(81, baseStats.get("Special Attack"));
        assertEquals(99, baseStats.get("Special Defense"));

        StringIntMap evYields = pokemonEntry.getEVYields();
        assertEquals(2, evYields.get("HP"));
        assertEquals(0, evYields.get("Attack"));
        assertEquals(0, evYields.get("Defense"));
        assertEquals(0, evYields.get("Speed"));
        assertEquals(0, evYields.get("Special Attack"));
        assertEquals(0, evYields.get("Special Defense"));

        assertEquals(165, pokemonEntry.getExperienceYield());
        assertEquals(125000, pokemonEntry.getExperienceAtLevel(50));
        assertEquals(58, pokemonEntry.getLevelAtExperience(200000));
        assertTrue(pokemonEntry.getLevelupMoves().size() > 0);
        assertTrue(pokemonEntry.getTMHMMoves().size() > 0);
        assertTrue(pokemonEntry.getEggMoves().size() > 0);
        assertTrue(pokemonEntry.getTutorMoves().size() > 0);
        assertEquals(1, pokemonEntry.getForms().size());
        assertEquals(0, pokemonEntry.getEvolutions().size());

        assertTrue(Files.exists(Paths.get(pokemonEntry.getIconFilepath(false))));
        assertTrue(Files.exists(Paths.get(pokemonEntry.getIconFilepath(true))));
        assertTrue(Files.exists(Paths.get(pokemonEntry.getSpriteFilepath(false,false))));
        assertTrue(Files.exists(Paths.get(pokemonEntry.getSpriteFilepath(false,true))));
        assertTrue(Files.exists(Paths.get(pokemonEntry.getSpriteFilepath(true,false))));
        assertTrue(Files.exists(Paths.get(pokemonEntry.getSpriteFilepath(true,true))));

        pokemonEntry.getIcon(false);
        pokemonEntry.getIcon(true);
        pokemonEntry.getSprite(false,false);
        pokemonEntry.getSprite(false,true);
        pokemonEntry.getSprite(true,false);
        pokemonEntry.getSprite(true,true);

        // TODO: equality checks
    }
}
