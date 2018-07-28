/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class PokemonConversionsTest
{
    private static System.Random rng = new System.Random();

    public static void ConversionsTest(
        PKMN.Species species,
        string form,
        PKMN.Game originGame,
        PKMN.Game destGame
    )
    {
        PKMN.Pokemon firstPokemon = new PKMN.Pokemon(species, originGame, form, 50);

        int originGeneration = Util.GameToGeneration(originGame);
        int destGeneration = Util.GameToGeneration(destGame);
        int minGeneration = System.Math.Min(originGeneration, destGeneration);
        PKMN.Game gameForLists = (minGeneration == originGeneration) ? originGame : destGame;

        PKMN.ItemEnumList items = PKMN.Database.Lists.ItemList(gameForLists);
        PKMN.MoveEnumList moves = PKMN.Database.Lists.MoveList(gameForLists);

        for(int i = 0; i < 4; ++i)
        {
            PKMN.Move move = PKMN.Move.NONE;
            do
            {
                move = moves[rng.Next(0, moves.Count-1)];
            }
            while(move >= PKMN.Move.SHADOW_RUSH);

            firstPokemon.Moves[i].Move = move;
        }

        if(originGeneration >= 3)
        {
            firstPokemon.OriginalTrainerSecretID = (ushort)rng.Next(0, 0xFFFF);

            if(firstPokemon.DatabaseEntry.Abilities.Second != PKMN.Ability.NONE)
            {
                firstPokemon.Ability = Util.RandomBool() ? firstPokemon.DatabaseEntry.Abilities.First
                                                         : firstPokemon.DatabaseEntry.Abilities.Second;
            }
        }
        firstPokemon.OriginalTrainerPublicID = (ushort)rng.Next(0, 0xFFFF);

        if(minGeneration >= 2)
        {
            PKMN.Item heldItem = PKMN.Item.NONE;
            do
            {
                heldItem = items[rng.Next(0, items.Count-1)];
            } while(!(new PKMN.Database.ItemEntry(heldItem, originGame).IsHoldable) ||
                    ((heldItem >= PKMN.Item.JOY_SCENT) && (heldItem <= PKMN.Item.VIVID_SCENT)));

            firstPokemon.HeldItem = heldItem;
        }
        if(originGeneration >= 2)
        {
            firstPokemon.Gender = Util.RandomBool() ? PKMN.Gender.MALE : PKMN.Gender.FEMALE;
            firstPokemon.IsShiny = Util.RandomBool();
            firstPokemon.CurrentTrainerFriendship = rng.Next(0, 255);

            if((originGame == PKMN.Game.GOLD) || (originGame == PKMN.Game.CRYSTAL))
            {
                firstPokemon.OriginalTrainerGender = Util.RandomBool() ? PKMN.Gender.MALE : PKMN.Gender.FEMALE;
            }

            // The max level met value in Generation II is 63.
            firstPokemon.LevelMet = rng.Next(2, (originGeneration == 2) ? 63 : 100);
        }
        if(originGeneration >= 3)
        {
            // Randomize ribbons, markings, and contest stats.
            foreach(string marking in firstPokemon.Markings.Keys)
            {
                firstPokemon.Markings[marking] = Util.RandomBool();
            }
            foreach(string ribbon in firstPokemon.Ribbons.Keys)
            {
                firstPokemon.Ribbons[ribbon] = Util.RandomBool();
            }
            foreach(string contestStat in firstPokemon.ContestStats.Keys)
            {
                firstPokemon.ContestStats[contestStat] = rng.Next(0, 255);
            }
        }

        firstPokemon.Nickname = Util.RandomString(10);
        firstPokemon.OriginalTrainerName = Util.RandomString(7);

        // The max level met value in Generation II is 63, which restricts this as well.
        firstPokemon.Level = rng.Next(2, (destGeneration == 2) ? 63 : 100);

        // Convert to the second game and compare.
        PKMN.Pokemon secondPokemon = firstPokemon.ToGame(destGame);

        Assert.AreEqual(firstPokemon.Species, secondPokemon.Species);
        Assert.AreEqual(destGame, secondPokemon.Game);
        Assert.AreEqual(firstPokemon.Form, secondPokemon.Form);
        Assert.AreEqual(firstPokemon.Nickname, secondPokemon.Nickname);
        Assert.AreEqual(firstPokemon.OriginalTrainerName, secondPokemon.OriginalTrainerName);
        Assert.AreEqual(firstPokemon.OriginalTrainerID, secondPokemon.OriginalTrainerID);
        Assert.AreEqual(firstPokemon.OriginalTrainerPublicID, secondPokemon.OriginalTrainerPublicID);
        Assert.AreEqual(firstPokemon.Experience, secondPokemon.Experience);
        Assert.AreEqual(firstPokemon.Level, secondPokemon.Level);

        for(int i = 0; i < 4; ++i)
        {
            Assert.AreEqual(firstPokemon.Moves[i].Move, secondPokemon.Moves[i].Move);
            Assert.AreEqual(firstPokemon.Moves[i].PP, secondPokemon.Moves[i].PP);
        }

        if(minGeneration >= 3)
        {
            Assert.AreEqual(firstPokemon.OriginalTrainerSecretID, secondPokemon.OriginalTrainerSecretID);
            Assert.AreEqual(firstPokemon.Ability, secondPokemon.Ability);
            Assert.AreEqual(firstPokemon.Ball, secondPokemon.Ball);
            Assert.AreEqual(firstPokemon.OriginalGame, secondPokemon.OriginalGame);
            Assert.AreEqual(firstPokemon.Personality, secondPokemon.Personality);

            if(originGeneration == destGeneration)
            {
                foreach(string marking in firstPokemon.Markings.Keys)
                {
                    Assert.AreEqual(firstPokemon.Markings[marking], secondPokemon.Markings[marking]);
                }
                foreach(string ribbon in firstPokemon.Ribbons.Keys)
                {
                    Assert.AreEqual(firstPokemon.Ribbons[ribbon], secondPokemon.Ribbons[ribbon]);
                }
                foreach(string contestStat in firstPokemon.ContestStats.Keys)
                {
                    Assert.AreEqual(firstPokemon.ContestStats[contestStat], secondPokemon.ContestStats[contestStat]);
                }
            }
        }
        if(minGeneration >= 2)
        {
            Assert.AreEqual(firstPokemon.OriginalTrainerGender, secondPokemon.OriginalTrainerGender);
            Assert.AreEqual(firstPokemon.Gender, secondPokemon.Gender);
            Assert.AreEqual(firstPokemon.IsShiny, secondPokemon.IsShiny);
            Assert.AreEqual(firstPokemon.HeldItem, secondPokemon.HeldItem);
            Assert.AreEqual(firstPokemon.CurrentTrainerFriendship, secondPokemon.CurrentTrainerFriendship);
            Assert.AreEqual(firstPokemon.Level, secondPokemon.LevelMet);
        }
    }
}

}
