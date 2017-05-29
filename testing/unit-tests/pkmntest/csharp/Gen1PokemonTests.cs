/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class Gen1PokemonTests {

    public static void FriendshipTest(
        string game
    ) {
        PKMN.Pokemon pikachu = new PKMN.Pokemon("Pikachu", game, "", 5);

        if(game.Equals("Yellow")) {
            pikachu.Friendship = 123;
            Assert.AreEqual(pikachu.Friendship, 123);

            // Also check a non-Pikachu.
            PKMN.Pokemon mewtwo = new PKMN.Pokemon("Mewtwo", game, "", 70);
            Assert.Throws<ApplicationException>(
                delegate {
                    mewtwo.Friendship = 123;
                }
            );
            Assert.Throws<ApplicationException>(
                delegate {
                    int friendship = mewtwo.Friendship;
                }
            );
        } else {
            Assert.Throws<ApplicationException>(
                delegate {
                    pikachu.Friendship = 123;
                }
            );
            Assert.Throws<ApplicationException>(
                delegate {
                   int friendship = pikachu.Friendship;
                }
            );
        }
    }

    private static void CheckStatMap(
        PKMN.StringIntDict statMap
    ) {
        Assert.IsTrue(statMap.ContainsKey("HP"));
        Assert.IsTrue(statMap.ContainsKey("Attack"));
        Assert.IsTrue(statMap.ContainsKey("Defense"));
        Assert.IsTrue(statMap.ContainsKey("Speed"));
        Assert.IsTrue(statMap.ContainsKey("Special"));
        Assert.IsFalse(statMap.ContainsKey("Special Attack"));
        Assert.IsFalse(statMap.ContainsKey("Special Defense"));
    }

    public static void PokemonTest(
        string game
    ) {
        string species = "Charmander";
        PKMN.Pokemon pokemon = new PKMN.Pokemon(species, game, "", 30);

        /*
         * Check starting values, and confirm that we can't query values
         * that didn't exist in Generation I.
         */
        Assert.AreEqual(pokemon.Species, species);
        Assert.AreEqual(pokemon.Form, "Standard");
        Assert.AreEqual(pokemon.Game, game);
        Assert.AreEqual(pokemon.Nickname, species.ToUpper());

        Assert.Throws<ApplicationException>(
            delegate {
                string gender = pokemon.Gender;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                bool isShiny = pokemon.IsShiny;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.HeldItem = "Potion";
            }
        );

        Assert.AreEqual(pokemon.TrainerName, PKMN.Pokemon.LIBPKMN_OT_NAME);
        Assert.AreEqual(pokemon.TrainerGender, "Male");
        Assert.AreEqual(pokemon.TrainerID, (PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF));
        Assert.AreEqual(pokemon.TrainerPublicID, (PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF));

        Assert.Throws<ApplicationException>(
            delegate {
                ushort trainerSecretID = pokemon.TrainerSecretID;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                int friendship = pokemon.Friendship;
            }
        );
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
        Assert.Throws<ApplicationException>(
            delegate {
                int levelMet = pokemon.LevelMet;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetLocationMet(true);
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetLocationMet(false);
            }
        );
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

        foreach(PKMN.MoveSlot moveSlot in pokemon.Moves) {
            Assert.AreEqual(moveSlot.Move, "None");
            Assert.AreEqual(moveSlot.PP, 0);
        }

        CheckStatMap(pokemon.EVs);
        CheckStatMap(pokemon.IVs);
        CheckStatMap(pokemon.Stats);

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

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.Gender = "Male";
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.IsShiny = true;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.IsShiny = false;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.HeldItem = "Potion";
            }
        );

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
                ushort trainerSecretID = pokemon.TrainerSecretID;
            }
        );

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.TrainerSecretID = 54321;
            }
        );
        Assert.AreEqual(pokemon.TrainerID, 10001);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.TrainerGender = "Male";
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.Friendship = 123;
            }
        );
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
                pokemon.LevelMet = 78;
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetLocationMet("Victory Road", true);
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetLocationMet("Victory Road", false);
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.OriginalGame = "Blue";
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
                pokemon.SetMove("Synthesis", 0);
            }
        );
        Assert.AreEqual(pokemon.Moves[0].Move, "None");

        string[] moves = {"Ember", "Flamethrower", "Slash", "Fire Blast"};
        for(int i = 0; i < 4; ++i) {
            pokemon.SetMove(moves[i], i);
        }
        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(pokemon.Moves[i].Move, moves[i]);
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
