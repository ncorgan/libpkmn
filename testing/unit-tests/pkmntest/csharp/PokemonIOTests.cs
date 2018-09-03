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

public class PokemonIOTests
{
    private static System.Random rng = new System.Random();

    private static string LibPKMNTestFiles = Environment.GetEnvironmentVariable("LIBPKMN_TEST_FILES");
    private static string TmpDir = PKMN.Paths.GetTmpDir();

    private static void TestLoadingAndSaving(
        PKMN.Game game,
        string extension
    )
    {
        string tmpPath = System.IO.Path.Combine(
                             TmpDir,
                             String.Format("{0}_{1}.{2}",
                                 game,
                                 rng.Next(),
                                 extension
                             )
                          );

        PKMN.ItemEnumList itemList = PKMN.Database.Lists.ItemList(game);
        PKMN.MoveEnumList moveList = PKMN.Database.Lists.MoveList(game);
        PKMN.SpeciesEnumList pokemonList = PKMN.Database.Lists.PokemonList(
                                               Util.GameToGeneration(game),
                                               true
                                           );

        PKMN.Pokemon randomPokemon = Util.GetRandomPokemon(game, itemList, moveList, pokemonList);
        randomPokemon.ExportToFile(tmpPath);

        PKMN.Pokemon importedPokemon = new PKMN.Pokemon(tmpPath);
        Util.ComparePokemon(randomPokemon, importedPokemon);

        System.IO.File.Delete(tmpPath);
    }

    // Generation I

    public static void TestLoadingAndSavingPK1(
        PKMN.Game game
    )
    {
        TestLoadingAndSaving(game, "pk1");
    }

    // Generation III

    public static void TestLoadingAndSavingPK2(
        PKMN.Game game
    )
    {
        TestLoadingAndSaving(game, "pk2");
    }

    // Game Boy Advance

    public static void TestLoadingAndSaving3GPKM(
        PKMN.Game game
    )
    {
        TestLoadingAndSaving(game, "3gpkm");
    }

    public static void TestLoadingAndSavingPK3(
        PKMN.Game game
    )
    {
        TestLoadingAndSaving(game, "pk3");
    }

    // Gamecube

    public static void TestLoadingAndSavingCK3(
        PKMN.Game game
    )
    {
        TestLoadingAndSaving(game, "ck3");
    }

    public static void TestLoadingAndSavingXK3(
        PKMN.Game game
    )
    {
        TestLoadingAndSaving(game, "xk3");
    }

    // Outside files

    public static void TestOutside3GPKM()
    {
        /*
         * Test files in repo and compare to known values.
         */
        string _3GPKMDir = System.IO.Path.Combine(LibPKMNTestFiles, "3gpkm");

        PKMN.Pokemon mightyena = new PKMN.Pokemon(
                                          System.IO.Path.Combine(_3GPKMDir, "MIGHTYENA.3gpkm")
                                      );
        Assert.AreEqual(mightyena.Species, PKMN.Species.MIGHTYENA);
        Assert.AreEqual(mightyena.Game, PKMN.Game.EMERALD);
        Assert.AreEqual(mightyena.Form, "Standard");
        Assert.AreEqual(mightyena.Nickname, "MIGHTYENA");
        Assert.IsFalse(mightyena.IsShiny);
        Assert.AreEqual(mightyena.HeldItem, PKMN.Item.HEART_SCALE);
        Assert.AreEqual(mightyena.Condition, PKMN.Condition.NONE);
        Assert.AreEqual(mightyena.OriginalTrainerName, "A");
        Assert.AreEqual(mightyena.OriginalTrainerPublicID, 61415);
        Assert.AreEqual(mightyena.OriginalTrainerSecretID, 3417);
        Assert.AreEqual(mightyena.OriginalTrainerID, 223997927);
        Assert.AreEqual(mightyena.OriginalTrainerGender, PKMN.Gender.FEMALE);
        Assert.AreEqual(mightyena.CurrentTrainerFriendship, 254);
        Assert.AreEqual(mightyena.Ability, PKMN.Ability.INTIMIDATE);
        Assert.AreEqual(mightyena.Ball, PKMN.Ball.GREAT_BALL);
        Assert.AreEqual(mightyena.LevelMet, 25);
        Assert.AreEqual(mightyena.LocationMet, "Route 120");
        Assert.AreEqual(mightyena.OriginalGame, PKMN.Game.EMERALD);
        Assert.AreEqual(mightyena.Personality, 3557601241);
        Assert.AreEqual(mightyena.Experience, 128734);
        Assert.AreEqual(mightyena.Level, 50);

        Assert.AreEqual(mightyena.Markings.Count, 4);
        foreach(PKMN.Marking marking in mightyena.Markings.Keys)
        {
            Assert.IsFalse(mightyena.Markings[marking]);
        }

        Assert.AreEqual(mightyena.Ribbons.Count, 32);
        foreach(string ribbon in mightyena.Ribbons.Keys)
        {
            if(ribbon.Equals("Champion")) {
                Assert.IsTrue(mightyena.Ribbons[ribbon]);
            } else {
                Assert.IsFalse(mightyena.Ribbons[ribbon]);
            }
        }

        Assert.AreEqual(mightyena.ContestStats.Count, 6);
        foreach(PKMN.ContestStat contestStat in mightyena.ContestStats.Keys)
        {
            Assert.AreEqual(mightyena.ContestStats[contestStat], 0);
        }

        PKMN.Move[] expectedMightyenaMoves =
        {
            PKMN.Move.CRUNCH,
            PKMN.Move.STRENGTH,
            PKMN.Move.SHADOW_BALL,
            PKMN.Move.DOUBLE_EDGE
        };
        Assert.AreEqual(mightyena.Moves.Count, 4);
        for(int i = 0; i < 4; ++i)
        {
            Assert.AreEqual(mightyena.Moves[i].Move, expectedMightyenaMoves[i]);
        }

        Assert.AreEqual(mightyena.EVs.Count, 6);
        Assert.AreEqual(mightyena.EVs[PKMN.Stat.HP], 30);
        Assert.AreEqual(mightyena.EVs[PKMN.Stat.ATTACK], 110);
        Assert.AreEqual(mightyena.EVs[PKMN.Stat.DEFENSE], 32);
        Assert.AreEqual(mightyena.EVs[PKMN.Stat.SPEED], 48);
        Assert.AreEqual(mightyena.EVs[PKMN.Stat.SPECIAL_ATTACK], 17);
        Assert.AreEqual(mightyena.EVs[PKMN.Stat.SPECIAL_DEFENSE], 83);

        Assert.AreEqual(mightyena.IVs.Count, 6);
        Assert.AreEqual(mightyena.IVs[PKMN.Stat.HP], 26);
        Assert.AreEqual(mightyena.IVs[PKMN.Stat.ATTACK], 28);
        Assert.AreEqual(mightyena.IVs[PKMN.Stat.DEFENSE], 4);
        Assert.AreEqual(mightyena.IVs[PKMN.Stat.SPEED], 13);
        Assert.AreEqual(mightyena.IVs[PKMN.Stat.SPECIAL_ATTACK], 25);
        Assert.AreEqual(mightyena.IVs[PKMN.Stat.SPECIAL_DEFENSE], 26);

        Assert.AreEqual(mightyena.Stats.Count, 6);
        Assert.AreEqual(mightyena.Stats[PKMN.Stat.HP], 146);
        Assert.AreEqual(mightyena.Stats[PKMN.Stat.ATTACK], 122);
        Assert.AreEqual(mightyena.Stats[PKMN.Stat.DEFENSE], 81);
        Assert.AreEqual(mightyena.Stats[PKMN.Stat.SPEED], 87);
        Assert.AreEqual(mightyena.Stats[PKMN.Stat.SPECIAL_ATTACK], 79);
        Assert.AreEqual(mightyena.Stats[PKMN.Stat.SPECIAL_DEFENSE], 88);
    }
}

}
