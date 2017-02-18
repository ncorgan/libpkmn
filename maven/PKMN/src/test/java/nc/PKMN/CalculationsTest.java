/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import java.util.concurrent.ThreadLocalRandom;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class CalculationsTest extends TestCase {

    // Putting L in a hex literal makes me sad.
    private static long UINT32_MAX = 0xFFFFFFFFL;

    protected void setUp() {
        try {
            InitPKMN.initialize();
        } catch(Exception e) {
        }
    }

    public CalculationsTest(String testName) {
        super(testName);
    }

    public static Test suite() {
        return new TestSuite(CalculationsTest.class);
    }

    public void testGen2UnownForm() {
        // Make sure using invalid parameters results in an error.
        try {
            String form = nc.PKMN.Calculations.gen2UnownForm(-1, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen2UnownForm(16, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen2UnownForm(0, -1, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen2UnownForm(0, 16, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen2UnownForm(0, 0, -1, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen2UnownForm(0, 0, 16, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen2UnownForm(0, 0, 0, -1);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen2UnownForm(0, 0, 0, 16);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}

        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
         */
        String form1 = nc.PKMN.Calculations.gen2UnownForm(10, 9, 1, 14);
        assertEquals("G", form1);
        String form2 = nc.PKMN.Calculations.gen2UnownForm(5, 15, 10, 5);
        assertEquals("S", form2);
    }

    public void testGen3UnownForm() {
        // Make sure SWIG+Java catches invalid input parameters.
        try {
            String form = nc.PKMN.Calculations.gen3UnownForm(-1);
            fail("SWIG+Java did not catch an invalid uint32_t value.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen3UnownForm(UINT32_MAX+1);
            fail("SWIG+Java did not catch an invalid uint32_t value.");
        } catch(Exception ex) {}

        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
         */
        String form = nc.PKMN.Calculations.gen3UnownForm(0x4C07DE71);
        assertEquals("B", form);
    }

    public void testWurmpleBecomesSilcoon() {
        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
         */
        assertTrue(nc.PKMN.Calculations.wurmpleBecomesSilcoon(0x5CF4091CL, true));
        assertFalse(nc.PKMN.Calculations.wurmpleBecomesSilcoon(0x091C5CF4L, true));
        assertTrue(nc.PKMN.Calculations.wurmpleBecomesSilcoon(0x091C5CF4L, false));
        assertFalse(nc.PKMN.Calculations.wurmpleBecomesSilcoon(0x5CF4091CL, false));
    }

    public void testGen2Gender() {
        /*
         * Make sure expected exceptions are thrown.
         */
        try {
            String form = nc.PKMN.Calculations.gen2PokemonGender("Not a species", 0);
            fail("Using an invalid species should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen2PokemonGender("Bulbasaur", -1);
            fail("Using an invalid stat value should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.gen2PokemonGender("Bulbasaur", 16);
            fail("Using an invalid stat value should have failed.");
        } catch(Exception ex) {}

        /*
         * Make sure known good inputs result in expected results.
         */

        // All male
        assertEquals("Male", nc.PKMN.Calculations.gen2PokemonGender("Nidorino", 0));
        assertEquals("Male", nc.PKMN.Calculations.gen2PokemonGender("Nidorino", 15));

        // 87.5% male, 12.5% female
        assertEquals("Female", nc.PKMN.Calculations.gen2PokemonGender("Charmander", 1));
        assertEquals("Male", nc.PKMN.Calculations.gen2PokemonGender("Charmander", 2));

        // 75% male, 25% female
        assertEquals("Female", nc.PKMN.Calculations.gen2PokemonGender("Growlithe", 3));
        assertEquals("Male", nc.PKMN.Calculations.gen2PokemonGender("Growlithe", 4));

        // 50% male, 50% female
        assertEquals("Female", nc.PKMN.Calculations.gen2PokemonGender("Pidgey", 6));
        assertEquals("Male", nc.PKMN.Calculations.gen2PokemonGender("Pidgey", 7));

        // 25% male, 75% female
        assertEquals("Female", nc.PKMN.Calculations.gen2PokemonGender("Vulpix", 11));
        assertEquals("Male", nc.PKMN.Calculations.gen2PokemonGender("Vulpix", 12));

        // All female
        assertEquals("Female", nc.PKMN.Calculations.gen2PokemonGender("Nidorina", 0));
        assertEquals("Female", nc.PKMN.Calculations.gen2PokemonGender("Nidorina", 15));

        // Genderless
        assertEquals("Genderless", nc.PKMN.Calculations.gen2PokemonGender("Magnemite", 0));
        assertEquals("Genderless", nc.PKMN.Calculations.gen2PokemonGender("Magnemite", 15));
    }

    public void testModernGender() {
        /*
         * Make sure expected exceptions are thrown.
         */
        try {
            String form = nc.PKMN.Calculations.modernPokemonGender("Not a species", 0);
            fail("Using an invalid species should have failed.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.modernPokemonGender("Bulbasaur", -1);
            fail("SWIG+Java did not catch an invalid uint32_t value.");
        } catch(Exception ex) {}
        try {
            String form = nc.PKMN.Calculations.modernPokemonGender("Bulbasaur", UINT32_MAX+1);
            fail("SWIG+Java did not catch an invalid uint32_t value.");
        } catch(Exception ex) {}

        /*
         * Make sure known good inputs result in expected results.
         */

        // All male
        assertEquals("Male", nc.PKMN.Calculations.modernPokemonGender("Nidorino", 0));
        assertEquals("Male", nc.PKMN.Calculations.modernPokemonGender("Nidorino", UINT32_MAX));

        // 87.5% male, 12.5% female
        assertEquals("Female", nc.PKMN.Calculations.modernPokemonGender("Charmander", 30));
        assertEquals("Male", nc.PKMN.Calculations.modernPokemonGender("Charmander", 31));

        // 75% male, 25% female
        assertEquals("Female", nc.PKMN.Calculations.modernPokemonGender("Growlithe", 63));
        assertEquals("Male", nc.PKMN.Calculations.modernPokemonGender("Growlithe", 64));

        // 50% male, 50% female
        assertEquals("Female", nc.PKMN.Calculations.modernPokemonGender("Pidgey", 126));
        assertEquals("Male", nc.PKMN.Calculations.modernPokemonGender("Pidgey", 127));

        // 25% male, 75% female
        assertEquals("Female", nc.PKMN.Calculations.modernPokemonGender("Vulpix", 190));
        assertEquals("Male", nc.PKMN.Calculations.modernPokemonGender("Vulpix", 191));

        // All female
        assertEquals("Female", nc.PKMN.Calculations.modernPokemonGender("Nidorina", 0));
        assertEquals("Female", nc.PKMN.Calculations.modernPokemonGender("Nidorina", UINT32_MAX));

        // Genderless
        assertEquals("Genderless", nc.PKMN.Calculations.modernPokemonGender("Magnemite", 0));
        assertEquals("Genderless", nc.PKMN.Calculations.modernPokemonGender("Magnemite", UINT32_MAX));
    }

    public void testGen2HiddenPower() {
        // Make sure using invalid parameters results in an error.
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.gen2HiddenPower(-1, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.gen2HiddenPower(16, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.gen2HiddenPower(0, -1, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.gen2HiddenPower(0, 16, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.gen2HiddenPower(0, 0, -1, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.gen2HiddenPower(0, 0, 16, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.gen2HiddenPower(0, 0, 0, -1);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.gen2HiddenPower(0, 0, 0, 16);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}

        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
         */
        nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.gen2HiddenPower(15, 15, 15, 14);
        assertEquals("Dark", hiddenPower.getType());
        assertEquals(69, hiddenPower.getBasePower());
    }

    public void testModernHiddenPower() {
        // Make sure using invalid parameters results in an error.
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(-1, 0, 0, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(32, 0, 0, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, -1, 0, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, 32, 0, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, 0, -1, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, 0, 32, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, 0, 0, -1, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, 0, 0, 32, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, 0, 0, 0, -1, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, 0, 0, 0, 32, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, 0, 0, 0, 0, -1);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(0, 0, 0, 0, 0, 32);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}

        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
         */
        nc.PKMN.HiddenPower hiddenPower = nc.PKMN.Calculations.modernHiddenPower(30, 31, 31, 31, 30, 31);
        assertEquals("Grass", hiddenPower.getType());
        assertEquals(70, hiddenPower.getBasePower());
    }

    public void testGen3Gen4Nature() {
        String[] natures = {
            "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
            "Bold", "Docile", "Relaxed", "Impish", "Lax",
            "Timid", "Hasty", "Serious", "Jolly", "Naive",
            "Modest", "Mild", "Quiet", "Bashful", "Rash",
            "Calm", "Gentle", "Sassy", "Careful", "Quirky",
        };

        for(int i = 0; i < natures.length; ++i) {
            assertEquals(
                natures[i],
                nc.PKMN.Calculations.gen3Gen4Nature(
                    (ThreadLocalRandom.current().nextInt(0, 50000) * 1000) + i
                )
            );
        }
    }

    public void testGen2Shiny() {
        // Make sure using invalid parameters results in an error.
        try {
            boolean isShiny = nc.PKMN.Calculations.gen2Shiny(-1, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.gen2Shiny(16, 0, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.gen2Shiny(0, -1, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.gen2Shiny(0, 16, 0, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.gen2Shiny(0, 0, -1, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.gen2Shiny(0, 0, 16, 0);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.gen2Shiny(0, 0, 0, -1);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.gen2Shiny(0, 0, 0, 16);
            fail("Using an invalid parameter should have failed.");
        } catch(Exception ex) {}

        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
         */
        assertTrue(nc.PKMN.Calculations.gen2Shiny(7, 10, 10, 10));
        assertFalse(nc.PKMN.Calculations.gen2Shiny(6, 15, 7, 5));
    }

    public void testModernShiny() {
        // Make sure SWIg+Java catch invalid parameters.
        try {
            boolean isShiny = nc.PKMN.Calculations.modernShiny(-1, 0);
            fail("SWIG+Java did not catch an invalid uint32_t value.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.modernShiny(UINT32_MAX+1, 0);
            fail("SWIG+Java did not catch an invalid uint32_t value.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.modernShiny(0, -1);
            fail("SWIG+Java did not catch an invalid uint32_t value.");
        } catch(Exception ex) {}
        try {
            boolean isShiny = nc.PKMN.Calculations.modernShiny(0, UINT32_MAX+1);
            fail("SWIG+Java did not catch an invalid uint32_t value.");
        } catch(Exception ex) {}

        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
         *         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
         */
        assertTrue(nc.PKMN.Calculations.modernShiny(2814471828L, 2545049318L));
        assertTrue(nc.PKMN.Calculations.modernShiny(0xB58F0B2AL, 398174488L));
    }
}
