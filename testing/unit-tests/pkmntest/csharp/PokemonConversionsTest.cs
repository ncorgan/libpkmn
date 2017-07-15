/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class PokemonConversionsTest {

    private static System.Random rng = new System.Random();

    private static int GameToGeneration(
        string game
    ) {
        if(game.Equals("Red") || game.Equals("Blue") || game.Equals("Yellow")) {
            return 1;
        } else if(game.Equals("Gold") || game.Equals("Silver") || game.Equals("Crystal")) {
            return 2;
        } else if(game.Equals("Ruby") || game.Equals("Sapphire") || game.Equals("Emerald") ||
                  game.Equals("FireRed") || game.Equals("LeafGreen") ||
                  game.Equals("Colosseum") || game.Equals("XD")
        ) {
            return 3;
        } else if(game.Equals("Diamond") || game.Equals("Pearl") || game.Equals("Platinum") ||
                  game.Equals("HeartGold") || game.Equals("SoulSilver")
        ) {
            return 4;
        } else if(game.Equals("Black") || game.Equals("White") ||
                  game.Equals("Black 2") || game.Equals("White 2")
        ) {
            return 5;
        } else if(game.Equals("X") || game.Equals("Y") ||
                  game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire")) {
            return 6;
        } else {
            return 0;
        }
    }

    private static bool RandomBool()
    {
        return (rng.Next() % 2) == 0;
    }

    // https://stackoverflow.com/a/1344295
    private static string RandomString(
        int length
    )
    {
        return System.IO.Path.GetRandomFileName().Replace(".","").Substring(0,length);
    }

    public static void ConversionsTest(
        string species,
        string form,
        string originGame,
        string destGame
    )
    {
        PKMN.Pokemon firstPokemon = new PKMN.Pokemon(species, originGame, "", 50);

        int originGeneration = GameToGeneration(originGame);
        int destGeneration = GameToGeneration(destGame);
        int minGeneration = System.Math.Min(originGeneration, destGeneration);
        string gameForLists = (minGeneration == originGeneration) ? originGame : destGame;

        PKMN.StringList items = PKMN.Database.Lists.ItemList(gameForLists);
        PKMN.StringList moves = PKMN.Database.Lists.MoveList(gameForLists);

        for(int i = 0; i < 4; ++i)
        {
            /*
             * This will get rid of legitimate moves, like Shadow Ball, but not
             * enough to cause an issue.
             */
            string moveName = "";
            do
            {
                moveName = moves[rng.Next(0, moves.Count-1)];
            } while(moveName.IndexOf("Shadow") == 0);

            firstPokemon.SetMove(moveName, i);
        }

        if(originGeneration >= 3)
        {
            firstPokemon.TrainerSecretID = (ushort)rng.Next(0, 0xFFFF);

            if(!firstPokemon.DatabaseEntry.Abilities.Second.Equals("None"))
            {
                firstPokemon.Ability = RandomBool() ? firstPokemon.DatabaseEntry.Abilities.First
                                                    : firstPokemon.DatabaseEntry.Abilities.Second;
            }
        }
        firstPokemon.TrainerPublicID = (ushort)rng.Next(0, 0xFFFF);

        if(minGeneration >= 2)
        {
            string heldItem = "";
            do
            {
                heldItem = items[rng.Next(0, items.Count-1)];
            } while(!(new PKMN.Database.ItemEntry(heldItem, originGame).IsHoldable) ||
                    (heldItem.IndexOf("Scent") != -1));

            firstPokemon.HeldItem = heldItem;
        }
        if(originGeneration >= 2)
        {
            firstPokemon.Gender = RandomBool() ? "Male" : "Female";
            firstPokemon.IsShiny = RandomBool();
            firstPokemon.Friendship = rng.Next(0, 255);

            if(!originGame.Equals("Gold") && !originGame.Equals("Silver"))
            {
                firstPokemon.TrainerGender = RandomBool() ? "Male" : "Female";
            }

            // The max level met value in Generation II is 63.
            firstPokemon.LevelMet = rng.Next(2, (originGeneration == 2) ? 63 : 100);
        }
        if(originGeneration >= 3)
        {
            // Randomize ribbons, markings, and contest stats.
            foreach(string marking in firstPokemon.Markings.Keys)
            {
                firstPokemon.SetMarking(marking, RandomBool());
            }
            foreach(string ribbon in firstPokemon.Ribbons.Keys)
            {
                firstPokemon.SetRibbon(ribbon, RandomBool());
            }
            foreach(string contestStat in firstPokemon.ContestStats.Keys)
            {
                firstPokemon.SetContestStat(contestStat, rng.Next(0, 255));
            }
        }

        firstPokemon.Nickname = RandomString(10);
        firstPokemon.TrainerName = RandomString(7);

        // The max level met value in Generation II is 63, which restricts this as well.
        firstPokemon.Level = rng.Next(2, (destGeneration == 2) ? 63 : 100);

        // Convert to the second game and compare.
        PKMN.Pokemon secondPokemon = firstPokemon.ToGame(destGame);

        Assert.AreEqual(firstPokemon.Species, secondPokemon.Species);
        Assert.AreEqual(destGame, secondPokemon.Game);
        Assert.AreEqual(firstPokemon.Form, secondPokemon.Form);
        Assert.AreEqual(firstPokemon.Nickname, secondPokemon.Nickname);
        Assert.AreEqual(firstPokemon.TrainerName, secondPokemon.TrainerName);
        Assert.AreEqual(firstPokemon.TrainerID, secondPokemon.TrainerID);
        Assert.AreEqual(firstPokemon.TrainerPublicID, secondPokemon.TrainerPublicID);
        Assert.AreEqual(firstPokemon.Experience, secondPokemon.Experience);
        Assert.AreEqual(firstPokemon.Level, secondPokemon.Level);

        for(int i = 0; i < 4; ++i)
        {
            Assert.AreEqual(firstPokemon.Moves[i].Move, secondPokemon.Moves[i].Move);
            Assert.AreEqual(firstPokemon.Moves[i].PP, secondPokemon.Moves[i].PP);
        }

        if(minGeneration >= 3)
        {
            Assert.AreEqual(firstPokemon.TrainerSecretID, secondPokemon.TrainerSecretID);
            Assert.AreEqual(firstPokemon.Ability, secondPokemon.Ability);
            Assert.AreEqual(firstPokemon.Ball, secondPokemon.Ball);
            Assert.AreEqual(firstPokemon.OriginalGame, secondPokemon.OriginalGame);
            Assert.AreEqual(firstPokemon.Personality, secondPokemon.Personality);

            if(originGeneration == destGeneration)
            {
                Assert.AreEqual(firstPokemon.Markings, secondPokemon.Markings);
                Assert.AreEqual(firstPokemon.Ribbons, secondPokemon.Ribbons);
                Assert.AreEqual(firstPokemon.ContestStats, secondPokemon.ContestStats);
            }
        }
        if(minGeneration >= 2)
        {
            Assert.AreEqual(firstPokemon.TrainerGender, secondPokemon.TrainerGender);
            Assert.AreEqual(firstPokemon.Gender, secondPokemon.Gender);
            Assert.AreEqual(firstPokemon.IsShiny, secondPokemon.IsShiny);
            Assert.AreEqual(firstPokemon.HeldItem, secondPokemon.HeldItem);
            Assert.AreEqual(firstPokemon.Friendship, secondPokemon.Friendship);
            Assert.AreEqual(firstPokemon.Level, secondPokemon.LevelMet);
        }
    }
}

}
