/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class Gen3PokemonTests {

    private static string[] Markings = {"Circle", "Triangle", "Square", "Heart"};
    private static string[] ContestTypes = {"Cool", "Beauty", "Cute", "Smart", "Tough"};
    private static string[] ContestLevels = {"", " Super", " Hyper", " Master"};
    private static string[] Ribbons = {"Champion", "Winning", "Victory", "Artist",
                                       "Effort", "Marine", "Land", "Sky",
                                       "Country", "National", "Earth", "World"};
    private static string[] Stats = {"HP", "Attack", "Defense", "Speed",
                                     "Special Attack", "Special Defense"};
    private static Random rng = new Random();

    public static void UnownFormTest(
        string game
    ) {
        PKMN.Database.PokemonEntry unownEntry = new PKMN.Database.PokemonEntry("Unown", game, "");
        foreach(string form in unownEntry.Forms) {
            PKMN.Pokemon unownFromLetter = new PKMN.Pokemon("Unown", game, form, 5);
            Assert.AreEqual(unownFromLetter.Form, form);
        }

        // Make sure setting the personality properly sets the form.
        PKMN.Pokemon unown = new PKMN.Pokemon("Unown", game, "A", 5);
        unown.Personality = 0x4C07DE71;
        Assert.AreEqual(unown.Form, "B");

        // Make sure setting the form properly sets the personality.
        foreach(string form in unownEntry.Forms) {
            unown.Form = form;
            Assert.AreEqual(unown.Form, form);
            string formFromPersonality = PKMN.Calculations.Gen3UnownForm(
                                             unown.Personality
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
            Assert.IsFalse(pokemon.Markings[marking]);
            pokemon.SetMarking(marking, true);
            Assert.IsTrue(pokemon.Markings[marking]);
        }
    }

    private static void RibbonsTest(
        PKMN.Pokemon pokemon
    ) {
        // Check contest ribbons.
        foreach(string contestType in ContestTypes) {
            foreach(string contestLevel in ContestLevels) {
                string ribbonName = contestType + contestLevel;
                Assert.IsFalse(pokemon.Ribbons[ribbonName]);
                pokemon.SetRibbon(ribbonName, true);
                Assert.IsTrue(pokemon.Ribbons[ribbonName]);
            }
        }

        // Check other ribbons.
        foreach(string ribbon in Ribbons) {
            Assert.IsFalse(pokemon.Ribbons[ribbon]);
            pokemon.SetRibbon(ribbon, true);
            Assert.IsTrue(pokemon.Ribbons[ribbon]);
        }
    }

    private static void ContestStatsTest(
        PKMN.Pokemon pokemon
    ) {
        foreach(string contestType in ContestTypes) {
            Assert.AreEqual(pokemon.ContestStats[contestType], 0);
            int val = rng.Next(0,256);
            pokemon.SetContestStat(contestType, val);
            Assert.AreEqual(pokemon.ContestStats[contestType], val);
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
            Assert.AreEqual(pokemon.EVs[stat], val);
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
            Assert.AreEqual(pokemon.IVs[stat], val);
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
        Assert.AreEqual(pokemon.Species, species);
        Assert.AreEqual(pokemon.Form, "Standard");
        Assert.AreEqual(pokemon.Game, game);
        Assert.AreEqual(pokemon.Nickname, species.ToUpper());
        Assert.AreEqual(pokemon.GetHeldItem().Name, "None");
        Assert.AreEqual(pokemon.TrainerName, PKMN.Pokemon.LIBPKMN_OT_NAME);
        Assert.AreEqual(pokemon.TrainerPublicID, (PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF));
        Assert.AreEqual(pokemon.TrainerSecretID, ((PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF0000) >> 16));
        Assert.AreEqual(pokemon.TrainerID, PKMN.Pokemon.LIBPKMN_OT_ID);
        Assert.AreEqual(pokemon.TrainerGender, "Male");
        Assert.AreEqual(pokemon.Friendship, pokemon.DatabaseEntry.BaseFriendship);
        Assert.AreEqual(pokemon.Ability, "Blaze");
        Assert.AreEqual(pokemon.Ball, "Premier Ball");
        Assert.AreEqual(pokemon.LevelMet, pokemon.Level);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetLocationMet(true);
            }
        );

        Assert.AreEqual(pokemon.GetLocationMet(false), "Fateful encounter");
        Assert.AreEqual(pokemon.OriginalGame, pokemon.Game);
        Assert.AreEqual(
            pokemon.Experience,
            pokemon.DatabaseEntry.GetExperienceAtLevel(30)
        );
        Assert.AreEqual(pokemon.Level, 30);

        CheckMarkingsMap(pokemon.Markings);
        CheckRibbonsMap(pokemon.Ribbons);
        CheckContestStatsMap(pokemon.ContestStats);

        foreach(PKMN.MoveSlot moveSlot in pokemon.Moves) {
            Assert.AreEqual(moveSlot.Move.Name, "None");
            Assert.AreEqual(moveSlot.PP, 0);
        }

        CheckStatsMap(pokemon.EVs);
        CheckStatsMap(pokemon.IVs);
        CheckStatsMap(pokemon.Stats);

        Assert.IsTrue(System.IO.File.Exists(pokemon.IconFilepath));
        Assert.IsTrue(System.IO.File.Exists(pokemon.SpriteFilepath));

        /*
         * Make sure the getters and setters agree. Also make sure it fails when
         * expected.
         */
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.Nickname = "";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.Nickname = "Too long nickname";
            }
        );

        pokemon.Nickname = "foobarbaz";
        Assert.AreEqual(pokemon.Nickname, "foobarbaz");

        // Gender and personality are tied, so make sure they affect each other.
        pokemon.Gender = "Female";
        Assert.Less((pokemon.Personality & 0xFF), 0xFF);
        pokemon.Gender = "Male";
        Assert.AreEqual((pokemon.Personality & 0xFF), 0xFF);

        pokemon.Personality = 0x1234AB00;
        Assert.AreEqual(pokemon.Gender, "Female");
        pokemon.Personality = 0xCD5678FF;
        Assert.AreEqual(pokemon.Gender, "Male");

        // Setting shininess should affect personality.
        pokemon.IsShiny = false;
        Assert.IsFalse(pokemon.IsShiny);
        uint personality = pokemon.Personality;
        Assert.IsTrue(System.IO.File.Exists(pokemon.SpriteFilepath));

        // This will fail if "shiny" is anywhere in the filepath.
        Assert.AreEqual(pokemon.SpriteFilepath.IndexOf("shiny"), -1);

        pokemon.IsShiny = true;
        Assert.IsTrue(pokemon.IsShiny);
        Assert.AreNotEqual(pokemon.Personality, personality);
        Assert.IsTrue(System.IO.File.Exists(pokemon.SpriteFilepath));
        Assert.AreNotEqual(pokemon.SpriteFilepath.IndexOf("shiny"), -1);

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

        Assert.AreEqual(pokemon.GetHeldItem().Name, "None");

        pokemon.SetHeldItem("Razz Berry");
        Assert.AreEqual(pokemon.GetHeldItem().Name, "Razz Berry");

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.TrainerName = "";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.TrainerName = "Too long trainer name";
            }
        );

        pokemon.TrainerName = "foobar";
        Assert.AreEqual(pokemon.TrainerName, "foobar");

        pokemon.TrainerID = 0x1234ABCD;
        Assert.AreEqual(pokemon.TrainerID, 0x1234ABCD);
        Assert.AreEqual(pokemon.TrainerPublicID, 0xABCD);
        Assert.AreEqual(pokemon.TrainerSecretID, 0x1234);

        pokemon.TrainerPublicID = 0x1A2B;
        Assert.AreEqual(pokemon.TrainerID, 0x12341A2B);
        Assert.AreEqual(pokemon.TrainerPublicID, 0x1A2B);
        Assert.AreEqual(pokemon.TrainerSecretID, 0x1234);

        pokemon.TrainerSecretID = 0x3C4D;
        Assert.AreEqual(pokemon.TrainerID, 0x3C4D1A2B);
        Assert.AreEqual(pokemon.TrainerPublicID, 0x1A2B);
        Assert.AreEqual(pokemon.TrainerSecretID, 0x3C4D);

        pokemon.TrainerGender = "Male";
        Assert.AreEqual(pokemon.TrainerGender, "Male");
        pokemon.TrainerGender = "Female";
        Assert.AreEqual(pokemon.TrainerGender, "Female");
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.TrainerGender = "Genderless";
            }
        );

        pokemon.Friendship = 123;
        Assert.AreEqual(pokemon.Friendship, 123);

        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.Friendship = -1;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.Friendship = 256;
            }
        );

        pokemon.Ability = "Blaze";
        Assert.AreEqual(pokemon.Ability, "Blaze");
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.Ability = "None";
            }
        );

        // Invalid
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.Ability = "Torrent";
            }
        );

        // Hidden ability
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.Ability = "Speed Boost";
            }
        );

        Assert.AreEqual(pokemon.Ability, "Blaze");

        pokemon.Ball = "Great Ball";
        Assert.AreEqual(pokemon.Ball, "Great Ball");

        // Not in Generation III
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.Ball = "Friend Ball";
            }
        );
        Assert.AreEqual(pokemon.Ball, "Great Ball");

        pokemon.LevelMet = 67;
        Assert.AreEqual(pokemon.LevelMet, 67);

        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.LevelMet = -1;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                pokemon.LevelMet = 101;
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

        pokemon.OriginalGame = "Ruby";
        Assert.AreEqual(pokemon.OriginalGame, "Ruby");

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.OriginalGame = "Not a game";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.OriginalGame = "Red"; // Impossible
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.OriginalGame = "HeartGold"; // From a later game
            }
        );
        Assert.AreEqual(pokemon.OriginalGame, "Ruby");

        pokemon.Personality = 0x7F3AB3A8;
        Assert.AreEqual(pokemon.Personality, 0x7F3AB3A8);

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
