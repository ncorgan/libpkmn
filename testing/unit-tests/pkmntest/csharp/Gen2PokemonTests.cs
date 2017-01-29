/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class Gen2PokemonTests {

    public static void InvalidPokemonTest(
        string game
    ) {
        PokemonTestsCommon.TestInvalidPokemon(game);
    }

    public static void UnownFormTest(
        string game
    ) {
        foreach(char letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ") {
            string form = letter.ToString();
            PKMN.Pokemon unownFromLetter = new PKMN.Pokemon("Unown", game, form, 5);
            Assert.AreEqual(unownFromLetter.GetForm(), form);

            // Make sure IVs are properly set.
            PKMN.StringIntDict IVs = unownFromLetter.GetIVs();
            string formFromIVs = PKMN.Calculations.Gen2UnownForm(
                                     IVs["Attack"],
                                     IVs["Defense"],
                                     IVs["Speed"],
                                     IVs["Special"]
                                 );
            Assert.AreEqual(unownFromLetter.GetForm(), formFromIVs);
        }

        // Make sure setting IVs properly changes the form.
        PKMN.Pokemon unown = new PKMN.Pokemon("Unown", game, "A", 5);
        unown.SetIV("Attack", 10);
        unown.SetIV("Defense", 9);
        unown.SetIV("Speed", 1);
        unown.SetIV("Special", 14);
        Assert.AreEqual(unown.GetForm(), "G");

        // Make sure setting the form properly changes IVs.
        foreach(char letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ") {
            string form = letter.ToString();
            unown.SetForm(form);
            Assert.AreEqual(unown.GetForm(), form);

            // Make sure IVs are properly set.
            PKMN.StringIntDict IVs = unown.GetIVs();
            string formFromIVs = PKMN.Calculations.Gen2UnownForm(
                                     IVs["Attack"],
                                     IVs["Defense"],
                                     IVs["Speed"],
                                     IVs["Special"]
                                 );
            Assert.AreEqual(unown.GetForm(), formFromIVs);
        }
    }

    private static void CheckStatMap(
        PKMN.StringIntDict statMap,
        bool isStats
    ) {
        Assert.IsTrue(statMap.ContainsKey("HP"));
        Assert.IsTrue(statMap.ContainsKey("Attack"));
        Assert.IsTrue(statMap.ContainsKey("Defense"));
        Assert.IsTrue(statMap.ContainsKey("Speed"));
        if(isStats) {
            Assert.IsFalse(statMap.ContainsKey("Special"));
            Assert.IsTrue(statMap.ContainsKey("Special Attack"));
            Assert.IsTrue(statMap.ContainsKey("Special Defense"));
        } else {
            Assert.IsTrue(statMap.ContainsKey("Special"));
            Assert.IsFalse(statMap.ContainsKey("Special Attack"));
            Assert.IsFalse(statMap.ContainsKey("Special Defense"));
        }
    }

    public static void PokemonTest(
        string game
    ) {
        string species = "Cyndaquil";
        PKMN.Pokemon pokemon = new PKMN.Pokemon(species, game, "", 30);

        /*
         * Check starting values, and confirm that we can't query values
         * that didn't exist in Generation II.
         */
        Assert.AreEqual(pokemon.GetSpecies(), species);
        Assert.AreEqual(pokemon.GetForm(), "Standard");
        Assert.AreEqual(pokemon.GetGame(), game);
        Assert.AreEqual(pokemon.GetNickname(), species.ToUpper());
        Assert.AreEqual(pokemon.GetHeldItem().GetName(), "None");
        Assert.AreEqual(pokemon.GetTrainerName(), PKMN.Pokemon.LIBPKMN_OT_NAME);
        Assert.AreEqual(pokemon.GetTrainerID(), (PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF));
        Assert.AreEqual(pokemon.GetTrainerPublicID(), (PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF));

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetTrainerSecretID();
            }
        );

        Assert.AreEqual(pokemon.GetTrainerGender(), "Male");
        Assert.AreEqual(pokemon.GetFriendship(), pokemon.GetDatabaseEntry().GetBaseFriendship());

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetAbility();
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetBall();
            }
        );

        Assert.AreEqual(pokemon.GetLevelMet(), pokemon.GetLevel());

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetLocationMet(true);
            }
        );

        Assert.AreEqual(pokemon.GetLocationMet(false), "Special");

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetOriginalGame();
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetPersonality();
            }
        );

        Assert.AreEqual(
            pokemon.GetExperience(),
            pokemon.GetDatabaseEntry().GetExperienceAtLevel(30)
        );
        Assert.AreEqual(pokemon.GetLevel(), 30);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetMarkings();
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetRibbons();
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetContestStats();
            }
        );

        PKMN.MoveSlotList moveSlots = pokemon.GetMoves();
        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(moveSlots[i].Move.GetName(), "None");
            Assert.AreEqual(moveSlots[i].PP, 0);
        }

        CheckStatMap(pokemon.GetEVs(), false);
        CheckStatMap(pokemon.GetIVs(), false);
        CheckStatMap(pokemon.GetStats(), true);

        /*
         * Make sure the getters and setters agree. Also make sure it fails when
         * expected.
         */
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetNickname("");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetNickname("Too long nickname");
            }
        );

        pokemon.SetNickname("foobarbaz");
        Assert.AreEqual(pokemon.GetNickname(), "foobarbaz");

        // Shininess affects IVs, so make sure the abstraction reflects that.
        pokemon.SetShininess(false);
        Assert.IsFalse(pokemon.IsShiny());
        Assert.AreEqual(pokemon.GetIVs()["Attack"], 13);

        pokemon.SetShininess(true);
        Assert.IsTrue(pokemon.IsShiny());

        PKMN.StringIntDict IVs = pokemon.GetIVs();
        Assert.AreEqual(IVs["Attack"], 15);
        Assert.AreEqual(IVs["Defense"], 10);
        Assert.AreEqual(IVs["Speed"], 10);
        Assert.AreEqual(IVs["Special"], 10);

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetHeldItem("Not an item");
            }
        );
        Assert.AreEqual(pokemon.GetHeldItem().GetName(), "None");

        // Valid item, but not holdable
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetHeldItem("Bicycle");
            }
        );
        Assert.AreEqual(pokemon.GetHeldItem().GetName(), "None");

        pokemon.SetHeldItem("Berry");
        Assert.AreEqual(pokemon.GetHeldItem().GetName(), "Berry");

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetTrainerName("");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetTrainerName("Too long trainer name");
            }
        );

        pokemon.SetTrainerName("foobar");
        Assert.AreEqual(pokemon.GetTrainerName(), "foobar");

        pokemon.SetTrainerID(10001);
        Assert.AreEqual(pokemon.GetTrainerID(), 10001);
        Assert.AreEqual(pokemon.GetTrainerPublicID(), 10001);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetTrainerSecretID(54321);
            }
        );
        Assert.AreEqual(pokemon.GetTrainerID(), 10001);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetAbility("");
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetBall("Great Ball");
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetLocationMet("Pallet Town", true);
            }
        );

        pokemon.SetLocationMet("Pallet Town", false);
        Assert.AreEqual(pokemon.GetLocationMet(false), "Pallet Town");

        pokemon.SetTrainerGender("Female");
        Assert.AreEqual(pokemon.GetTrainerGender(), "Female");
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetTrainerGender("Genderless");
            }
        );

        pokemon.SetLevelMet(3);
        Assert.AreEqual(pokemon.GetLevelMet(), 3);

        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetFriendship(-1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetFriendship(256);
            }
        );

        pokemon.SetFriendship(123);
        Assert.AreEqual(pokemon.GetFriendship(), 123);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetOriginalGame("Gold");
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetMarking("Circle", true);
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetRibbon("Cool", false);
            }
        );

        pokemon.SetExperience(12345);
        Assert.AreEqual(pokemon.GetExperience(), 12345);

        pokemon.SetLevel(45);
        Assert.AreEqual(pokemon.GetLevel(), 45);

        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetMove("Ember", -1);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetMove("Bounce", 0);
            }
        );

        Assert.AreEqual(pokemon.GetMoves()[0].Move.GetName(), "None");

        string[] moveNames = {"Ember", "Flamethrower", "Slash", "Fire Blast"};
        for(int i = 0; i < 4; ++i) {
            pokemon.SetMove(moveNames[i], i);
        }

        moveSlots = pokemon.GetMoves();
        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(moveSlots[i].Move.GetName(), moveNames[i]);
            Assert.AreEqual(moveSlots[i].PP, moveSlots[i].Move.GetPP(0));
        }

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetEV("Not a stat", 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetEV("Attack", -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetEV("Attack", 65536);
            }
        );

        pokemon.SetEV("Attack", 12345);
        Assert.AreEqual(pokemon.GetEVs()["Attack"], 12345);

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetIV("Not a stat", 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetIV("Attack", -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetIV("Attack", 16);
            }
        );

        pokemon.SetIV("Attack", 12);
        Assert.AreEqual(pokemon.GetIVs()["Attack"], 12);
    }
}

}
