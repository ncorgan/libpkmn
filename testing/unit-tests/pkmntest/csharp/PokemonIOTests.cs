/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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

    private static PKMN.Pokemon2 GetRandomPokemon(
        string game
    )
    {
        int generation = Util.GameToGeneration(game);

        PKMN.StringList itemList = PKMN.Database.Lists.ItemList(game);
        PKMN.StringList moveList = PKMN.Database.Lists.MoveList(game);
        PKMN.StringList pokemonList = PKMN.Database.Lists.PokemonList(generation, true);

        // Don't deal with Deoxys issues here.
        string species;
        if(generation == 3)
        {
            do
            {
                species = pokemonList[rng.Next(0, pokemonList.Count-1)];
            }
            while(species == "Deoxys");
        }
        else
        {
            species = pokemonList[rng.Next(0, pokemonList.Count-1)];
        }

        PKMN.Pokemon2 ret = new PKMN.Pokemon2(
                                    species,
                                    game,
                                    "",
                                    rng.Next(2, 100)
                                );

        for(int moveIndex = 0; moveIndex < 4; ++moveIndex)
        {
            string move;
            do
            {
                move = moveList[rng.Next(0, moveList.Count-1)];
            }
            while(move.IndexOf("Shadow") != -1);

            ret.Moves[moveIndex].Move = move;
        }

        foreach(string EV in ret.EVs.Keys)
        {
            ret.EVs[EV] = rng.Next(0, 255);
        }
        foreach(string IV in ret.IVs.Keys)
        {
            ret.IVs[IV] = rng.Next(0, 15);
        }

        if(generation >= 2)
        {
            // Keep going until one is holdable.
            while(ret.HeldItem == "None")
            {
                try
                {
                    ret.HeldItem = itemList[rng.Next(0, itemList.Count-1)];
                }
                catch(ArgumentOutOfRangeException)
                {
                }
            }

            ret.PokerusDuration = rng.Next(0, 15);
        }
        if(generation >= 3)
        {
            foreach(string marking in ret.Markings.Keys)
            {
                ret.Markings[marking] = Util.RandomBool();
            }
            foreach(string ribbon in ret.Ribbons.Keys)
            {
                ret.Ribbons[ribbon] = Util.RandomBool();
            }
            foreach(string contestStat in ret.ContestStats.Keys)
            {
                ret.ContestStats[contestStat] = rng.Next(0, 255);
            }
        }

        return ret;
    }

    public static void ComparePokemon(
        PKMN.Pokemon2 pokemon1,
        PKMN.Pokemon2 pokemon2
    )
    {
        string game = pokemon1.Game;
        int generation = Util.GameToGeneration(game);

        // There is no way to tell what game an imported Generation I-II
        // Pokémon comes from, so LibPKMN defaults to a default valid game.
        if(generation >= 3)
        {
            Assert.AreEqual(pokemon2.Game, game);
        }

        Assert.AreEqual(pokemon1.Species, pokemon2.Species);
        Assert.AreEqual(pokemon1.Form, pokemon2.Form);
        Assert.AreEqual(pokemon1.OriginalTrainerID, pokemon2.OriginalTrainerID);
        Assert.AreEqual(pokemon1.Experience, pokemon2.Experience);
        Assert.AreEqual(pokemon1.Level, pokemon2.Level);
        Assert.AreEqual(pokemon1.Nickname, pokemon2.Nickname);
        Assert.AreEqual(pokemon1.OriginalTrainerName, pokemon2.OriginalTrainerName);

        Assert.AreEqual(pokemon1.EVs.Keys, pokemon2.EVs.Keys);
        foreach(string EV in pokemon1.EVs.Keys)
        {
            Assert.AreEqual(pokemon1.EVs[EV], pokemon2.EVs[EV]);
        }

        Assert.AreEqual(pokemon1.IVs.Keys, pokemon2.IVs.Keys);
        foreach(string IV in pokemon1.IVs.Keys)
        {
            Assert.AreEqual(pokemon1.IVs[IV], pokemon2.IVs[IV]);
        }

        Assert.AreEqual(pokemon1.Stats.Keys, pokemon2.Stats.Keys);
        foreach(string stat in pokemon1.Stats.Keys)
        {
            Assert.AreEqual(pokemon1.Stats[stat], pokemon2.Stats[stat]);
        }

        if(pokemon1.Game.Equals(pokemon2.Game))
        {
            Assert.AreEqual(pokemon1.IconFilepath, pokemon2.IconFilepath);
            Assert.AreEqual(pokemon1.SpriteFilepath, pokemon2.SpriteFilepath);
        }

        if(generation >= 2)
        {
            Assert.AreEqual(pokemon1.OriginalTrainerGender, pokemon2.OriginalTrainerGender);
            Assert.AreEqual(pokemon1.CurrentTrainerFriendship, pokemon2.CurrentTrainerFriendship);
            Assert.AreEqual(pokemon1.Gender, pokemon2.Gender);
            Assert.AreEqual(pokemon1.IsShiny, pokemon2.IsShiny);
            Assert.AreEqual(pokemon1.HeldItem, pokemon2.HeldItem);
            Assert.AreEqual(pokemon1.LevelMet, pokemon2.LevelMet);
            Assert.AreEqual(pokemon1.LocationMet, pokemon2.LocationMet);
            Assert.AreEqual(pokemon1.PokerusDuration, pokemon2.PokerusDuration);
        }
        if(generation >= 3)
        {
            Assert.AreEqual(pokemon1.Ability, pokemon2.Ability);
            Assert.AreEqual(pokemon1.Ball, pokemon2.Ball);
            Assert.AreEqual(pokemon1.Personality, pokemon2.Personality);
            Assert.AreEqual(pokemon1.Gender, pokemon2.Gender);

            Assert.AreEqual(pokemon1.Markings.Keys, pokemon2.Markings.Keys);
            foreach(string marking in pokemon1.Markings.Keys)
            {
                Assert.AreEqual(pokemon1.Markings[marking], pokemon2.Markings[marking]);
            }

            Assert.AreEqual(pokemon1.Ribbons.Keys, pokemon2.Ribbons.Keys);
            foreach(string ribbon in pokemon1.Ribbons.Keys)
            {
                Assert.AreEqual(pokemon1.Ribbons[ribbon], pokemon2.Ribbons[ribbon]);
            }

            Assert.AreEqual(pokemon1.ContestStats.Keys, pokemon2.ContestStats.Keys);
            foreach(string contestStat in pokemon1.ContestStats.Keys)
            {
                Assert.AreEqual(pokemon1.ContestStats[contestStat], pokemon2.ContestStats[contestStat]);
            }
        }
        if(generation >= 4)
        {
            Assert.AreEqual(pokemon1.LocationMetAsEgg, pokemon2.LocationMetAsEgg);
        }
    }

    // Tests

    public static void TestLoadingAndSavingPK1(
        string game
    )
    {
        string pk1Path = System.IO.Path.Combine(TmpDir, String.Format("{0}_{1}.pk1", game, rng.Next()));

        PKMN.Pokemon2 randomPokemon = GetRandomPokemon(game);
        randomPokemon.ExportToFile(pk1Path);

        PKMN.Pokemon2 importedPokemon = new PKMN.Pokemon2(pk1Path);
        ComparePokemon(randomPokemon, importedPokemon);

        System.IO.File.Delete(pk1Path);
    }

    public static void TestLoadingAndSavingPK2(
        string game
    )
    {
        string pk2Path = System.IO.Path.Combine(TmpDir, String.Format("{0}_{1}.pk2", game, rng.Next()));

        PKMN.Pokemon2 randomPokemon = GetRandomPokemon(game);
        randomPokemon.ExportToFile(pk2Path);

        PKMN.Pokemon2 importedPokemon = new PKMN.Pokemon2(pk2Path);
        ComparePokemon(randomPokemon, importedPokemon);

        System.IO.File.Delete(pk2Path);
    }

    public static void TestLoadingAndSaving3GPKM(
        string game
    )
    {
        string _3gpkmPath = System.IO.Path.Combine(TmpDir, String.Format("{0}_{1}.3gpkm", game, rng.Next()));

        PKMN.Pokemon2 randomPokemon = GetRandomPokemon(game);
        randomPokemon.ExportToFile(_3gpkmPath);

        PKMN.Pokemon2 importedPokemon = new PKMN.Pokemon2(_3gpkmPath);
        ComparePokemon(randomPokemon, importedPokemon);

        System.IO.File.Delete(_3gpkmPath);
    }

    public static void TestOutside3GPKM()
    {
        /*
         * Test files in repo and compare to known values.
         */
        string _3GPKMDir = System.IO.Path.Combine(LibPKMNTestFiles, "3gpkm");

        PKMN.Pokemon2 mightyena = new PKMN.Pokemon2(
                                          System.IO.Path.Combine(_3GPKMDir, "MIGHTYENA.3gpkm")
                                      );
        Assert.AreEqual(mightyena.Species, "Mightyena");
        Assert.AreEqual(mightyena.Game, "Emerald");
        Assert.AreEqual(mightyena.Form, "Standard");
        Assert.AreEqual(mightyena.Nickname, "MIGHTYENA");
        Assert.IsFalse(mightyena.IsShiny);
        Assert.AreEqual(mightyena.HeldItem, "Heart Scale");
        Assert.AreEqual(mightyena.OriginalTrainerName, "A");
        Assert.AreEqual(mightyena.OriginalTrainerPublicID, 61415);
        Assert.AreEqual(mightyena.OriginalTrainerSecretID, 3417);
        Assert.AreEqual(mightyena.OriginalTrainerID, 223997927);
        Assert.AreEqual(mightyena.OriginalTrainerGender, "Female");
        Assert.AreEqual(mightyena.CurrentTrainerFriendship, 254);
        Assert.AreEqual(mightyena.Ability, "Intimidate");
        Assert.AreEqual(mightyena.Ball, "Great Ball");
        Assert.AreEqual(mightyena.LevelMet, 25);
        Assert.AreEqual(mightyena.LocationMet, "Route 120");
        Assert.AreEqual(mightyena.OriginalGame, "Emerald");
        Assert.AreEqual(mightyena.Personality, 3557601241);
        Assert.AreEqual(mightyena.Experience, 128734);
        Assert.AreEqual(mightyena.Level, 50);

        Assert.AreEqual(mightyena.Markings.Count, 4);
        foreach(string marking in mightyena.Markings.Keys)
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
        foreach(string contestStat in mightyena.ContestStats.Keys)
        {
            Assert.AreEqual(mightyena.ContestStats[contestStat], 0);
        }

        string[] expectedMightyenaMoves =
        {
            "Crunch", "Strength", "Shadow Ball", "Double-Edge"
        };
        Assert.AreEqual(mightyena.Moves.Count, 4);
        for(int i = 0; i < 4; ++i)
        {
            Assert.AreEqual(mightyena.Moves[i].Move, expectedMightyenaMoves[i]);
        }

        Assert.AreEqual(mightyena.EVs.Count, 6);
        Assert.AreEqual(mightyena.EVs["HP"], 30);
        Assert.AreEqual(mightyena.EVs["Attack"], 110);
        Assert.AreEqual(mightyena.EVs["Defense"], 32);
        Assert.AreEqual(mightyena.EVs["Speed"], 48);
        Assert.AreEqual(mightyena.EVs["Special Attack"], 17);
        Assert.AreEqual(mightyena.EVs["Special Defense"], 83);

        Assert.AreEqual(mightyena.IVs.Count, 6);
        Assert.AreEqual(mightyena.IVs["HP"], 26);
        Assert.AreEqual(mightyena.IVs["Attack"], 28);
        Assert.AreEqual(mightyena.IVs["Defense"], 4);
        Assert.AreEqual(mightyena.IVs["Speed"], 13);
        Assert.AreEqual(mightyena.IVs["Special Attack"], 25);
        Assert.AreEqual(mightyena.IVs["Special Defense"], 26);

        Assert.AreEqual(mightyena.Stats.Count, 6);
        Assert.AreEqual(mightyena.Stats["HP"], 146);
        Assert.AreEqual(mightyena.Stats["Attack"], 122);
        Assert.AreEqual(mightyena.Stats["Defense"], 81);
        Assert.AreEqual(mightyena.Stats["Speed"], 87);
        Assert.AreEqual(mightyena.Stats["Special Attack"], 79);
        Assert.AreEqual(mightyena.Stats["Special Defense"], 88);
    }
}

}
