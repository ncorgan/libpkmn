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

    public static void UnownFormTest(
        string game
    ) {
        foreach(char letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ") {
            string form = letter.ToString();
            PKMN.Pokemon unownFromLetter = new PKMN.Pokemon("Unown", game, form, 5);
            Assert.AreEqual(unownFromLetter.Form, form);

            // Make sure IVs are properly set.
            string formFromIVs = PKMN.Calculations.Gen2UnownForm(
                                     unownFromLetter.IVs["Attack"],
                                     unownFromLetter.IVs["Defense"],
                                     unownFromLetter.IVs["Speed"],
                                     unownFromLetter.IVs["Special"]
                                 );
            Assert.AreEqual(unownFromLetter.Form, formFromIVs);
        }

        // Make sure setting IVs properly changes the form.
        PKMN.Pokemon unown = new PKMN.Pokemon("Unown", game, "A", 5);
        unown.SetIV("Attack", 10);
        unown.SetIV("Defense", 9);
        unown.SetIV("Speed", 1);
        unown.SetIV("Special", 14);
        Assert.AreEqual(unown.Form, "G");

        // Make sure setting the form properly changes IVs.
        foreach(char letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ") {
            string form = letter.ToString();
            unown.Form = form;
            Assert.AreEqual(unown.Form, form);

            // Make sure IVs are properly set.
            string formFromIVs = PKMN.Calculations.Gen2UnownForm(
                                     unown.IVs["Attack"],
                                     unown.IVs["Defense"],
                                     unown.IVs["Speed"],
                                     unown.IVs["Special"]
                                 );
            Assert.AreEqual(unown.Form, formFromIVs);
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
        Assert.AreEqual(pokemon.Species, species);
        Assert.AreEqual(pokemon.Form, "Standard");
        Assert.AreEqual(pokemon.Game, game);
        Assert.AreEqual(pokemon.Nickname, species.ToUpper());
        Assert.AreEqual(pokemon.HeldItem, "None");
        Assert.AreEqual(pokemon.TrainerName, PKMN.Pokemon.DEFAULT_TRAINER_NAME);
        Assert.AreEqual(pokemon.TrainerID, (PKMN.Pokemon.DEFAULT_TRAINER_ID & 0xFFFF));
        Assert.AreEqual(pokemon.TrainerPublicID, (PKMN.Pokemon.DEFAULT_TRAINER_ID & 0xFFFF));

        Assert.Throws<ApplicationException>(
            delegate {
                ushort trainerSecretID = pokemon.TrainerSecretID;
            }
        );

        Assert.AreEqual(pokemon.TrainerGender, "Male");
        Assert.AreEqual(pokemon.Friendship, pokemon.DatabaseEntry.GetBaseFriendship());

        Assert.Throws<ApplicationException>(
            delegate {
                string ability = pokemon.Ability;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                string ball = pokemon.Ball;
            }
        );

        Assert.AreEqual(pokemon.LevelMet, pokemon.Level);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetLocationMet(true);
            }
        );

        Assert.AreEqual(pokemon.GetLocationMet(false), "Special");

        Assert.Throws<ApplicationException>(
            delegate {
                string originalGame = pokemon.OriginalGame;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                uint personality = pokemon.Personality;
            }
        );

        Assert.AreEqual(
            pokemon.Experience,
            pokemon.DatabaseEntry.GetExperienceAtLevel(30)
        );
        Assert.AreEqual(pokemon.Level, 30);

        Assert.Throws<ApplicationException>(
            delegate {
                PKMN.StringBoolDict markings = pokemon.Markings;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                PKMN.StringBoolDict ribbons = pokemon.Ribbons;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                PKMN.StringIntDict contestStats = pokemon.ContestStats;
            }
        );

        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(pokemon.Moves[i].Move, "None");
            Assert.AreEqual(pokemon.Moves[i].PP, 0);
        }

        CheckStatMap(pokemon.EVs, false);
        CheckStatMap(pokemon.IVs, false);
        CheckStatMap(pokemon.Stats, true);

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

        // Gender affects IVs, so make sure the abstraction reflects that.
        pokemon.Gender = "Male";
        Assert.AreEqual(pokemon.IVs["Attack"], 15);
        pokemon.Gender = "Female";
        Assert.Less(pokemon.IVs["Attack"], 15);

        pokemon.SetIV("Attack", 0);
        Assert.AreEqual(pokemon.Gender, "Female");
        pokemon.SetIV("Attack", 15);
        Assert.AreEqual(pokemon.Gender, "Male");

        // Shininess affects IVs, so make sure the abstraction reflects that.
        pokemon.IsShiny = false;
        Assert.IsFalse(pokemon.IsShiny);
        Assert.AreEqual(pokemon.IVs["Attack"], 13);
        Assert.IsTrue(System.IO.File.Exists(pokemon.SpriteFilepath));

        // This will fail if "shiny" is anywhere in the filepath.
        Assert.AreEqual(pokemon.SpriteFilepath.IndexOf("shiny"), -1);

        pokemon.IsShiny = true;
        Assert.IsTrue(pokemon.IsShiny);

        Assert.AreEqual(pokemon.IVs["Attack"], 15);
        Assert.AreEqual(pokemon.IVs["Defense"], 10);
        Assert.AreEqual(pokemon.IVs["Speed"], 10);
        Assert.AreEqual(pokemon.IVs["Special"], 10);
        Assert.IsTrue(System.IO.File.Exists(pokemon.SpriteFilepath));
        Assert.AreNotEqual(pokemon.SpriteFilepath.IndexOf("shiny"), -1);

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.HeldItem = "Not an item";
            }
        );
        Assert.AreEqual(pokemon.HeldItem, "None");

        // Valid item, but not holdable
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.HeldItem = "Bicycle";
            }
        );
        Assert.AreEqual(pokemon.HeldItem, "None");

        pokemon.HeldItem = "Berry";
        Assert.AreEqual(pokemon.HeldItem, "Berry");

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

        pokemon.TrainerID = 10001;
        Assert.AreEqual(pokemon.TrainerID, 10001);
        Assert.AreEqual(pokemon.TrainerPublicID, 10001);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.TrainerSecretID = 54321;
            }
        );
        Assert.AreEqual(pokemon.TrainerID, 10001);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.Ability = "";
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.Ball = "Great Ball";
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetLocationMet("Pallet Town", true);
            }
        );

        pokemon.SetLocationMet("Pallet Town", false);
        Assert.AreEqual(pokemon.GetLocationMet(false), "Pallet Town");

        pokemon.TrainerGender = "Female";
        Assert.AreEqual(pokemon.TrainerGender, "Female");
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                pokemon.TrainerGender = "Genderless";
            }
        );

        pokemon.LevelMet = 3;
        Assert.AreEqual(pokemon.LevelMet, 3);

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

        pokemon.Friendship = 123;
        Assert.AreEqual(pokemon.Friendship, 123);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.OriginalGame = "Gold";
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

        pokemon.Experience = 12345;
        Assert.AreEqual(pokemon.Experience, 12345);

        pokemon.Level = 45;
        Assert.AreEqual(pokemon.Level, 45);

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

        Assert.AreEqual(pokemon.Moves[0].Move, "None");

        string[] moveNames = {"Ember", "Flamethrower", "Slash", "Fire Blast"};
        for(int i = 0; i < 4; ++i) {
            pokemon.SetMove(moveNames[i], i);
        }

        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(pokemon.Moves[i].Move, moveNames[i]);
            Assert.AreEqual(
                pokemon.Moves[i].PP,
                new PKMN.Database.MoveEntry(pokemon.Moves[i].Move, game).GetPP(0)
            );
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
        Assert.AreEqual(pokemon.EVs["Attack"], 12345);

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
        Assert.AreEqual(pokemon.IVs["Attack"], 12);
    }
}

}
