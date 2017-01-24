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

    public static void InvalidPokemonTest(
        string game
    ) {
        PokemonTestsCommon.TestInvalidPokemon(game);
    }

    public static void FriendshipTest(
        string game
    ) {
        PKMN.Pokemon pikachu = new PKMN.Pokemon("Pikachu", game, "", 5);

        if(game.Equals("Yellow")) {
            pikachu.SetFriendship(123);
            Assert.AreEqual(pikachu.GetFriendship(), 123);

            // Also check a non-Pikachu.
            PKMN.Pokemon mewtwo = new PKMN.Pokemon("Mewtwo", game, "", 70);
            Assert.Throws<ApplicationException>(
                delegate {
                    mewtwo.SetFriendship(123);
                }
            );
            Assert.Throws<ApplicationException>(
                delegate {
                    mewtwo.GetFriendship();
                }
            );
        } else {
            Assert.Throws<ApplicationException>(
                delegate {
                    pikachu.SetFriendship(123);
                }
            );
            Assert.Throws<ApplicationException>(
                delegate {
                    pikachu.GetFriendship();
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
        Assert.AreEqual(pokemon.GetSpecies(), species);
        Assert.AreEqual(pokemon.GetForm(), "Standard");
        Assert.AreEqual(pokemon.GetGame(), game);
        Assert.AreEqual(pokemon.GetNickname(), species.ToUpper());

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.IsShiny();
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetHeldItem();
            }
        );

        Assert.AreEqual(pokemon.GetTrainerName(), PKMN.Pokemon.LIBPKMN_OT_NAME);
        Assert.AreEqual(pokemon.GetTrainerGender(), "Male");
        Assert.AreEqual(pokemon.GetTrainerID(), (PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF));
        Assert.AreEqual(pokemon.GetTrainerPublicID(), (PKMN.Pokemon.LIBPKMN_OT_ID & 0xFFFF));

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetTrainerSecretID();
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetFriendship();
            }
        );
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
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.GetLevelMet();
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

        foreach(PKMN.MoveSlot moveSlot in pokemon.GetMoves()) {
            Assert.AreEqual(moveSlot.Move.GetName(), "None");
            Assert.AreEqual(moveSlot.PP, 0);
        }

        CheckStatMap(pokemon.GetEVs());
        CheckStatMap(pokemon.GetIVs());
        CheckStatMap(pokemon.GetStats());

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

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetShininess(true);
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetShininess(false);
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetHeldItem("Potion");
            }
        );

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
                pokemon.GetTrainerSecretID();
            }
        );

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetTrainerSecretID(54321);
            }
        );
        Assert.AreEqual(pokemon.GetTrainerID(), 10001);

        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetTrainerGender("Male");
            }
        );
        Assert.Throws<ApplicationException>(
            delegate {
                pokemon.SetFriendship(123);
            }
        );
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
                pokemon.SetLevelMet(78);
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
                pokemon.SetOriginalGame("Blue");
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
                pokemon.SetMove("Synthesis", 0);
            }
        );
        Assert.AreEqual(pokemon.GetMoves()[0].Move.GetName(), "None");

        string[] moves = {"Ember", "Flamethrower", "Slash", "Fire Blast"};
        for(int i = 0; i < 4; ++i) {
            pokemon.SetMove(moves[i], i);
        }
        PKMN.MoveSlotList moveSlots = pokemon.GetMoves();
        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(moveSlots[i].Move.GetName(), moves[i]);
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
