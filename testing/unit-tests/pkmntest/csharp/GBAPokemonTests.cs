/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class GBAPokemonTests {

    private static string[] Markings = {"Circle", "Triangle", "Square", "Heart"};
    private static string[] ContestTypes = {"Cool", "Beauty", "Cute", "Smart", "Tough"};
    private static string[] ContestLevels = {"", " Super", " Hyper", " Master"};
    private static string[] Ribbons = {"Champion", "Winning", "Victory", "Artist",
                                       "Effort", "Marine", "Land", "Sky",
                                       "Country", "National", "Earth", "World"};
    private static string[] Stats = {"HP", "Attack", "Defense", "Speed",
                                     "Special Attack", "Special Defense"};
    private static Random rng = new Random();

    public static void InvalidPokemonTest(
        string game
    ) {
        PokemonTestsCommon.TestInvalidPokemon(game);
    }

    public static void UnownFormTest(
        string game
    ) {
        PKMN.PokemonEntry unownEntry = new PKMN.PokemonEntry("Unown", game, "");
        foreach(string form in unownEntry.GetForms()) {
            PKMN.Pokemon unownFromLetter = new PKMN.Pokemon("Unown", game, form, 5);
            Assert.AreEqual(unownFromLetter.GetForm(), form);
        }

        // Make sure setting the personality properly sets the form.
        PKMN.Pokemon unown = new PKMN.Pokemon("Unown", game, "A", 5);
        unown.SetPersonality(0x4C07DE71);
        Assert.AreEqual(unown.GetForm(), "B");

        // Make sure setting the form properly sets the personality.
        foreach(string form in unownEntry.GetForms()) {
            unown.SetForm(form);
            Assert.AreEqual(unown.GetForm(), form);
            string formFromPersonality = PKMN.Calculations.Gen3UnownForm(
                                             unown.GetPersonality()
                                         );
            Assert.AreEqual(formFromPersonality, form);
        }
    }

    private static void CheckMarkingsMap(
        PKMN.StringBoolDict markingsMap
    ) {
        Assert.AreEqual(markingsMap.Count, 4);
        foreach(string marking in Markings) {
            Assert.IsTrue(markingsMap.ContainsKey(marking));
            Assert.IsFalse(markingsMap[marking]);
        }

        Assert.IsFalse(markingsMap.ContainsKey("Star"));
        Assert.IsFalse(markingsMap.ContainsKey("Diamond"));
    }

    private static void CheckRibbonsMap(
        PKMN.StringBoolDict ribbonsMap
    ) {
        Assert.AreEqual(ribbonsMap.Count, 32);

        // Check contest ribbons.
        foreach(string contestType in ContestTypes) {
            foreach(string contestLevel in ContestLevels) {
                string ribbonName = contestType + contestLevel;
                Assert.IsTrue(ribbonsMap.ContainsKey(ribbonName));
                Assert.IsFalse(ribbonsMap[ribbonName]);
            }
        }

        // Check other ribbons.
        foreach(string ribbon in Ribbons) {
            Assert.IsTrue(ribbonsMap.ContainsKey(ribbon));
            Assert.IsFalse(ribbonsMap[ribbon]);
        }
    }

    private static void CheckContestStatsMap(
        PKMN.StringIntDict contestStatsMap
    ) {
        Assert.AreEqual(contestStatsMap.Count, 6);

        foreach(string stat in ContestTypes) {
            Assert.IsTrue(contestStatsMap.ContainsKey(stat));
            Assert.AreEqual(contestStatsMap[stat], 0);
        }

        Assert.IsTrue(contestStatsMap.ContainsKey("Feel"));
        Assert.AreEqual(contestStatsMap["Feel"], 0);

        Assert.IsFalse(contestStatsMap.ContainsKey("Sheen"));
    }

    private static void CheckStatsMap(
        PKMN.StringIntDict statsMap
    ) {
        Assert.AreEqual(statsMap.Count, 6);

        foreach(string stat in Stats) {
            Assert.IsTrue(statsMap.ContainsKey(stat));
        }

        Assert.IsFalse(statsMap.ContainsKey("Special"));
    }

    private static void MarkingsTest(
        PKMN.Pokemon pokemon
    ) {
        foreach(string marking in Markings) {
            Assert.IsFalse(pokemon.GetMarkings()[marking]);
            pokemon.SetMarking(marking, true);
            Assert.IsTrue(pokemon.GetMarkings()[marking]);
        }
    }

    private static void RibbonsTest(
        PKMN.Pokemon pokemon
    ) {
        // Check contest ribbons.
        foreach(string contestType in ContestTypes) {
            foreach(string contestLevel in ContestLevels) {
                string ribbonName = contestType + contestLevel;
                Assert.IsFalse(pokemon.GetRibbons()[ribbonName]);
                pokemon.SetRibbon(ribbonName, true);
                Assert.IsTrue(pokemon.GetRibbons()[ribbonName]);
            }
        }

        // Check other ribbons.
        foreach(string ribbon in Ribbons) {
            Assert.IsFalse(pokemon.GetRibbons()[ribbon]);
            pokemon.SetRibbon(ribbon, true);
            Assert.IsTrue(pokemon.GetRibbons()[ribbon]);
        }
    }

    private static void ContestStatsTest(
        PKMN.Pokemon pokemon
    ) {
        foreach(string contestType in ContestTypes) {
            Assert.AreEqual(pokemon.GetContestStats()[contestType], 0);
            int val = rng.Next(0,256);
            pokemon.SetContestStat(contestType, val);
            Assert.AreEqual(pokemon.GetContestStats()[contestType], val);
        }
    }

    private static void EVsTest(
        PKMN.Pokemon pokemon
    ) {
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetEV("Not a stat", 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetEV("HP", -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetEV("HP", 256);
            }
        );

        foreach(string stat in Stats) {
            int val = rng.Next(0,256);
            pokemon.SetEV(stat, val);
            Assert.AreEqual(pokemon.GetEVs()[stat], val);
        }
    }

    private static void IVsTest(
        PKMN.Pokemon pokemon
    ) {
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetIV("Not a stat", 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetIV("HP", -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetIV("HP", 32);
            }
        );

        foreach(string stat in Stats) {
            int val = rng.Next(0,32);
            pokemon.SetIV(stat, val);
            Assert.AreEqual(pokemon.GetIVs()[stat], val);
        }
    }

    public static void PokemonTest(
        string game
    ) {
        string species = "Torchic";
        PKMN.Pokemon pokemon = new PKMN.Pokemon(species, game, "", 30);

        /*
         * Check known starting values, and confirm that we can't query values
         * that didn't exist in Generation III.
         */
        Assert.AreEqual(pokemon.GetSpecies(), species);
        Assert.AreEqual(pokemon.GetForm(), "Standard");
        Assert.AreEqual(pokemon.GetGame(), game);
        Assert.AreEqual(pokemon.GetNickname(), species.ToUpper());
        Assert.AreEqual(pokemon.GetHeldItem().GetName(), "None");
        Assert.AreEqual(pokemon.GetTrainerName(), PKMN.Pokemon.LIBPKMN_OT_NAME);
        Assert.AreEqual(pokemon.GetTrainerPublicID(), (PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF));
        Assert.AreEqual(pokemon.GetTrainerSecretID(), ((PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF0000) >> 16));
        Assert.AreEqual(pokemon.GetTrainerID(), PKMN.Pokemon.LIBPKMN_OT_ID);
        Assert.AreEqual(pokemon.GetTrainerGender(), "Male");
        Assert.AreEqual(pokemon.GetFriendship(), pokemon.GetDatabaseEntry().GetBaseFriendship());
        Assert.AreEqual(pokemon.GetAbility(), "Blaze");
        Assert.AreEqual(pokemon.GetBall(), "Premier Ball");
        Assert.AreEqual(pokemon.GetLevelMet(), pokemon.GetLevel());

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetLocationMet(true);
            }
        );

        Assert.AreEqual(pokemon.GetLocationMet(false), "Fateful encounter");
        Assert.AreEqual(pokemon.GetOriginalGame(), pokemon.GetGame());
        Assert.AreEqual(
            pokemon.GetExperience(),
            pokemon.GetDatabaseEntry().GetExperienceAtLevel(30)
        );
        Assert.AreEqual(pokemon.GetLevel(), 30);

        CheckMarkingsMap(pokemon.GetMarkings());
        CheckRibbonsMap(pokemon.GetRibbons());
        CheckContestStatsMap(pokemon.GetContestStats());

        PKMN.MoveSlotList moveSlots = pokemon.GetMoves();
        foreach(PKMN.MoveSlot moveSlot in moveSlots) {
            Assert.AreEqual(moveSlot.Move.GetName(), "None");
            Assert.AreEqual(moveSlot.PP, 0);
        }

        CheckStatsMap(pokemon.GetEVs());
        CheckStatsMap(pokemon.GetIVs());
        CheckStatsMap(pokemon.GetStats());

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

        // Setting shininess should affect personality.
        pokemon.SetShininess(false);
        Assert.IsFalse(pokemon.IsShiny());
        uint personality = pokemon.GetPersonality();
        pokemon.SetShininess(true);
        Assert.IsTrue(pokemon.IsShiny());
        Assert.AreNotEqual(pokemon.GetPersonality(), personality);

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetHeldItem("Not an item");
            }
        );

        // Not in this game.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetHeldItem("Berry");
            }
        );

        // Not holdable.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetHeldItem("Mach Bike");
            }
        );

        Assert.AreEqual(pokemon.GetHeldItem().GetName(), "None");

        pokemon.SetHeldItem("Razz Berry");
        Assert.AreEqual(pokemon.GetHeldItem().GetName(), "Razz Berry");

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

        pokemon.SetTrainerID(0x1234ABCD);
        Assert.AreEqual(pokemon.GetTrainerID(), 0x1234ABCD);
        Assert.AreEqual(pokemon.GetTrainerPublicID(), 0xABCD);
        Assert.AreEqual(pokemon.GetTrainerSecretID(), 0x1234);

        pokemon.SetTrainerPublicID(0x1A2B);
        Assert.AreEqual(pokemon.GetTrainerID(), 0x12341A2B);
        Assert.AreEqual(pokemon.GetTrainerPublicID(), 0x1A2B);
        Assert.AreEqual(pokemon.GetTrainerSecretID(), 0x1234);

        pokemon.SetTrainerSecretID(0x3C4D);
        Assert.AreEqual(pokemon.GetTrainerID(), 0x3C4D1A2B);
        Assert.AreEqual(pokemon.GetTrainerPublicID(), 0x1A2B);
        Assert.AreEqual(pokemon.GetTrainerSecretID(), 0x3C4D);

        pokemon.SetTrainerGender("Male");
        Assert.AreEqual(pokemon.GetTrainerGender(), "Male");
        pokemon.SetTrainerGender("Female");
        Assert.AreEqual(pokemon.GetTrainerGender(), "Female");
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetTrainerGender("Genderless");
            }
        );

        pokemon.SetFriendship(123);
        Assert.AreEqual(pokemon.GetFriendship(), 123);

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

        pokemon.SetAbility("Blaze");
        Assert.AreEqual(pokemon.GetAbility(), "Blaze");
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetAbility("None");
            }
        );

        // Invalid
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetAbility("Torrent");
            }
        );

        // Hidden ability
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetAbility("Speed Boost");
            }
        );

        Assert.AreEqual(pokemon.GetAbility(), "Blaze");

        pokemon.SetBall("Great Ball");
        Assert.AreEqual(pokemon.GetBall(), "Great Ball");

        // Not in Generation III
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetBall("Friend Ball");
            }
        );
        Assert.AreEqual(pokemon.GetBall(), "Great Ball");

        pokemon.SetLevelMet(67);
        Assert.AreEqual(pokemon.GetLevelMet(), 67);

        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetLevelMet(-1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetLevelMet(101);
            }
        );

        string location;
        if(game.Equals("FireRed") || game.Equals("LeafGreen")) {
            location = "Viridian Forest";
        } else {
            location = "Petalburg Woods";
        }
        pokemon.SetLocationMet(location, false);
        Assert.AreEqual(pokemon.GetLocationMet(false), location);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetLocationMet(location, true);
            }
        );

        pokemon.SetOriginalGame("Ruby");
        Assert.AreEqual(pokemon.GetOriginalGame(), "Ruby");

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetOriginalGame("Not a game");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetOriginalGame("Red"); // Impossible
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetOriginalGame("HeartGold"); // From a later game
            }
        );
        Assert.AreEqual(pokemon.GetOriginalGame(), "Ruby");

        pokemon.SetPersonality(0x7F3AB3A8);
        Assert.AreEqual(pokemon.GetPersonality(), 0x7F3AB3A8);

        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetContestStat("Cool", -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.SetContestStat("Cool", 256);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.SetContestStat("Not a stat", 1);
            }
        );

        MarkingsTest(pokemon);
        RibbonsTest(pokemon);
        ContestStatsTest(pokemon);
        EVsTest(pokemon);
        IVsTest(pokemon);
    }
}

}
