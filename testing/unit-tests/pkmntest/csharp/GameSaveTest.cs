/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * DIstributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using System.IO;
using NUnit.Framework;

namespace PKMNTest {

public class GameSaveTest
{

    private static System.Random rng = new System.Random();

    private static string PKSAV_TEST_SAVES = System.Environment.GetEnvironmentVariable("PKSAV_TEST_SAVES");
    private static string LIBPKMN_TEST_FILES = System.Environment.GetEnvironmentVariable("LIBPKMN_TEST_FILES");
    private static string PKMN_TMP_DIR = PKMN.Paths.GetTmpDir();

    private static ushort DEFAULT_TRAINER_PID = 1351;
    private static ushort DEFAULT_TRAINER_SID = 32123;

    private static int MONEY_MAX = 999999;

    private static PKMN.Game[] RIVAL_NAME_SET_GAMES =
    {
        PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD,
        PKMN.Game.COLOSSEUM, PKMN.Game.XD,
        PKMN.Game.BLACK, PKMN.Game.WHITE,
        PKMN.Game.X, PKMN.Game.Y
    };

    private static PKMN.Game[] MALE_ONLY_GAMES =
    {
        PKMN.Game.RED, PKMN.Game.BLUE, PKMN.Game.YELLOW,
        PKMN.Game.GOLD, PKMN.Game.SILVER,
        PKMN.Game.COLOSSEUM, PKMN.Game.XD
    };

    /*
     * Utility functions
     */

    private static bool IsGBGame(
        PKMN.Game game
    )
    {
        return (Util.GameToGeneration(game) <= 2);
    }

    private static bool IsMaleOnly(
        PKMN.Game game
    )
    {
        return (Array.IndexOf(MALE_ONLY_GAMES, game) > -1);
    }

    private static bool IsRivalNameSet(
        PKMN.Game game
    )
    {
        return (Array.IndexOf(RIVAL_NAME_SET_GAMES, game) > -1);
    }

    private static bool IsGameGamecube(PKMN.Game game)
    {
        return (game == PKMN.Game.COLOSSEUM) || (game == PKMN.Game.XD);
    }

    private static void TestTrainerID(
        PKMN.GameSave gameSave
    )
    {
        bool gbGame = IsGBGame(gameSave.Game);

        Assert.AreEqual(
            gameSave.TrainerID,
            gbGame ? DEFAULT_TRAINER_PID : PKMN.Pokemon.DefaultTrainerID
        );
        if(gbGame)
        {
            Assert.AreEqual(gameSave.TrainerSecretID, 0);
        }
        else
        {
            Assert.AreEqual(gameSave.TrainerSecretID, DEFAULT_TRAINER_SID);
        }
    }

    private static void TestTimePlayed(
        PKMN.GameSave gameSave
    )
    {
        PKMN.Game game = gameSave.Game;
        int generation = Util.GameToGeneration(game);

        int milliseconds = 0;
        if(generation > 1)
        {
            // Generation I doesn't record to sub-second precision.
            milliseconds = rng.Next(0, 999);
        }

        System.TimeSpan validTimePlayed = new System.TimeSpan(
                                                  0, // Days
                                                  rng.Next(0, 255),
                                                  rng.Next(0, 59),
                                                  rng.Next(0, 59),
                                                  milliseconds
                                              );


        if(IsGameGamecube(game))
        {
            Assert.Throws<ApplicationException>(
                delegate
                {
                    gameSave.TimePlayed = validTimePlayed;
                }
            );
            Assert.Throws<ApplicationException>(
                delegate
                {
                    System.TimeSpan timePlayed = gameSave.TimePlayed;
                }
            );
        }
        else
        {
            // Test a valid time played.
            gameSave.TimePlayed = validTimePlayed;

            // Internally, time is stored with a precision of 1/60 second.
            Assert.That(
                gameSave.TimePlayed,
                Is.EqualTo(validTimePlayed).Within(
                    System.TimeSpan.FromMilliseconds(1000.0 / 60.0)
                )
            );

            System.TimeSpan[] invalidTimeSpans =
            {
                new System.TimeSpan(0, -1, 0, 0, 0),
                new System.TimeSpan(0, 0, -1, 0, 0),
                new System.TimeSpan(0, 0, 0, -1, 0),

                // Just past the maximum internal storage size
                new System.TimeSpan(0, 65536, 0, 0, 0)
            };
            foreach(System.TimeSpan invalidTimePlayed in invalidTimeSpans)
            {
                Assert.Throws<IndexOutOfRangeException>(
                    delegate
                    {
                        gameSave.TimePlayed = invalidTimePlayed;
                    }
                );
            }
        }
    }

    private static void TestCommonFields(
        PKMN.GameSave gameSave
    )
    {
        PKMN.Game game = gameSave.Game;

        // Trainer name
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                gameSave.TrainerName = "";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                gameSave.TrainerName = "LibPKMNLibPKMN";
            }
        );
        gameSave.TrainerName = "LibPKMN";
        Assert.AreEqual(gameSave.TrainerName, "LibPKMN");

        // Trainer ID
        gameSave.TrainerID = IsGBGame(game) ? DEFAULT_TRAINER_PID : PKMN.Pokemon.DefaultTrainerID;
        TestTrainerID(gameSave);
        gameSave.TrainerPublicID = DEFAULT_TRAINER_PID;
        TestTrainerID(gameSave);
        if(IsGBGame(game))
        {
            Assert.Throws<ApplicationException>(
                delegate
                {
                    gameSave.TrainerSecretID = DEFAULT_TRAINER_SID;
                }
            );
        }
        else
        {
            gameSave.TrainerSecretID = DEFAULT_TRAINER_SID;
            TestTrainerID(gameSave);
        }

        // Rival Name
        if(IsRivalNameSet(game))
        {
            Assert.Throws<ApplicationException>(
                delegate
                {
                   gameSave.RivalName = PKMN.Pokemon.DefaultTrainerName;
                }
            );
        }
        else
        {
           gameSave.RivalName = PKMN.Pokemon.DefaultTrainerName;
           Assert.AreEqual(gameSave.RivalName, PKMN.Pokemon.DefaultTrainerName);
        }

        // Trainer Gender
        if(IsMaleOnly(game))
        {
            Assert.AreEqual(gameSave.TrainerGender, PKMN.Gender.MALE);
            Assert.Throws<ApplicationException>(
                delegate
                {
                   gameSave.TrainerGender = PKMN.Gender.MALE;
                }
            );
            Assert.Throws<ApplicationException>(
                delegate
                {
                   gameSave.TrainerGender = PKMN.Gender.FEMALE;
                }
            );
        }
        else
        {
            gameSave.TrainerGender = PKMN.Gender.MALE;
            Assert.AreEqual(gameSave.TrainerGender, PKMN.Gender.MALE);
            gameSave.TrainerGender = PKMN.Gender.FEMALE;
            Assert.AreEqual(gameSave.TrainerGender, PKMN.Gender.FEMALE);
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                   gameSave.TrainerGender = PKMN.Gender.GENDERLESS;
                }
            );
        }

        // Money
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
               gameSave.Money = -1;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
               gameSave.Money = MONEY_MAX+1;
            }
        );
        gameSave.Money = 123456;
        Assert.AreEqual(gameSave.Money, 123456);

        // Pokémon Party
        PKMN.PokemonParty party = gameSave.PokemonParty;
        int numPokemon = party.NumPokemon;
        Assert.AreEqual(party.Length, 6);

        Assert.Greater(numPokemon, 0);
        Assert.LessOrEqual(numPokemon, 6);
        for(int i = 0; i < 6; ++i)
        {
            if(i < numPokemon)
            {
                Assert.AreNotEqual(party[i].Species, PKMN.Species.NONE);
            }
            else
            {
                Assert.AreEqual(party[i].Species, PKMN.Species.NONE);
            }
        }

        // Pokémon PC
        PKMN.PokemonPC pokemonPC = gameSave.PokemonPC;
        for(int i = 0; i < pokemonPC.Length; ++i)
        {
            PKMN.PokemonBox box = pokemonPC[i];

            Assert.LessOrEqual(box.NumPokemon, box.Length);

            // Boxes are only contiguous in Game Boy games.
            if(IsGBGame(game))
            {
                numPokemon = box.NumPokemon;
                for(int j = 0; j < box.Length; ++j)
                {
                    if(j < numPokemon)
                    {
                        Assert.AreNotEqual(box[j].Species, PKMN.Species.NONE);
                    }
                    else
                    {
                        Assert.AreEqual(box[j].Species, PKMN.Species.NONE);
                    }
                }
            }
        }

        // Pokédex
        if(!IsGameGamecube(game))
        {
            PKMN.Pokedex pokedex = gameSave.Pokedex;
            Assert.GreaterOrEqual(pokedex.NumSeen, pokedex.NumCaught);

            for(int partyIndex = 0; partyIndex < party.Length; ++partyIndex)
            {
                PKMN.Species species = party[partyIndex].Species;
                if((species != PKMN.Species.NONE) && !party[partyIndex].IsEgg)
                {
                    Assert.IsTrue(pokedex.SeenPokemonMap[species]);
                    Assert.IsTrue(pokedex.CaughtPokemonMap[species]);
                }
            }

            for(int PCIndex = 0; PCIndex < pokemonPC.Length; ++PCIndex)
            {
                PKMN.PokemonBox box = pokemonPC[PCIndex];
                for(int boxIndex = 0; boxIndex < box.Length; ++boxIndex)
                {
                    PKMN.Species species = box[boxIndex].Species;
                    if((species != PKMN.Species.NONE) && !box[boxIndex].IsEgg)
                    {
                        Assert.IsTrue(pokedex.SeenPokemonMap[species]);
                        Assert.IsTrue(pokedex.CaughtPokemonMap[species]);
                    }
                }
            }

            // Make sure that when a Pokémon is added to the party or PC, it's
            // added to the Pokédex. Manually remove the test species from the
            // Pokédex to confirm this behavior.

            PKMN.Species testSpecies1 = PKMN.Species.BULBASAUR;
            PKMN.Species testSpecies2 = PKMN.Species.CHARMANDER;

            pokedex.SeenPokemonMap[testSpecies1] = false;
            Assert.IsFalse(pokedex.SeenPokemonMap[testSpecies1]);
            Assert.IsFalse(pokedex.CaughtPokemonMap[testSpecies1]);

            pokedex.SeenPokemonMap[testSpecies2] = false;
            Assert.IsFalse(pokedex.SeenPokemonMap[testSpecies1]);
            Assert.IsFalse(pokedex.CaughtPokemonMap[testSpecies1]);

            PKMN.Pokemon testPokemon1 = new PKMN.Pokemon(
                                                testSpecies1,
                                                game,
                                                "",
                                                5
                                            );
            PKMN.Pokemon testPokemon2 = new PKMN.Pokemon(
                                                testSpecies2,
                                                game,
                                                "",
                                                5
                                            );


            party[0] = testPokemon1;
            Assert.IsTrue(pokedex.SeenPokemonMap[testSpecies1]);
            Assert.IsTrue(pokedex.CaughtPokemonMap[testSpecies1]);

            pokemonPC[0][0] = testPokemon2;
            Assert.IsTrue(pokedex.SeenPokemonMap[testSpecies2]);
            Assert.IsTrue(pokedex.CaughtPokemonMap[testSpecies2]);
        }

        TestTimePlayed(gameSave);
    }

    private static void TestAttributes(
        PKMN.GameSave gameSave
    )
    {
        PKMN.Game game = gameSave.Game;
        int generation = Util.GameToGeneration(game);
        switch(generation)
        {
            case 1:
                Assert.IsTrue(gameSave.NumericAttributes.Names.Contains("Casino coins"));
                Assert.GreaterOrEqual(gameSave.NumericAttributes["Casino coins"], 0);
                Assert.LessOrEqual(gameSave.NumericAttributes["Casino coins"], 9999);

                int gen1CasinoCoins = rng.Next(0, 9999);
                gameSave.NumericAttributes["Casino coins"] = gen1CasinoCoins;
                Assert.AreEqual(gameSave.NumericAttributes["Casino coins"], gen1CasinoCoins);

                if(game == PKMN.Game.YELLOW)
                {
                    Assert.IsTrue(gameSave.NumericAttributes.Names.Contains("Pikachu friendship"));
                    Assert.GreaterOrEqual(gameSave.NumericAttributes["Pikachu friendship"], 0);
                    Assert.LessOrEqual(gameSave.NumericAttributes["Pikachu friendship"], 255);

                    int pikachuFriendship = rng.Next(0, 255);
                    gameSave.NumericAttributes["Pikachu friendship"] = pikachuFriendship;
                    Assert.AreEqual(gameSave.NumericAttributes["Pikachu friendship"], pikachuFriendship);
                }
                else
                {
                    Assert.IsFalse(gameSave.NumericAttributes.Names.Contains("Pikachu friendship"));
                }
                break;

            case 3:
                if(!IsGameGamecube(game))
                {
                    Assert.IsTrue(gameSave.NumericAttributes.Names.Contains("Casino coins"));
                    Assert.GreaterOrEqual(gameSave.NumericAttributes["Casino coins"], 0);
                    Assert.LessOrEqual(gameSave.NumericAttributes["Casino coins"], 9999);

                    int gcnCasinoCoins = rng.Next(0, 9999);
                    gameSave.NumericAttributes["Casino coins"] = gcnCasinoCoins;
                    Assert.AreEqual(gameSave.NumericAttributes["Casino coins"], gcnCasinoCoins);
                }
                break;

            default:
                break;
        }
    }

    private static void RandomizePokemon(
        PKMN.GameSave gameSave,
        PKMN.ItemEnumList itemList
    )
    {
        PKMN.Game game = gameSave.Game;

        PKMN.StringList moveNameList = PKMN.Database.Lists.MoveNameList(game);
        PKMN.SpeciesEnumList pokemonList = PKMN.Database.Lists.PokemonList(1, true);

        PKMN.PokemonParty party = gameSave.PokemonParty;
        for(int i = 0; i < 6; ++i)
        {
            party[i] = Util.GetRandomPokemon(game, itemList, moveNameList, pokemonList);
        }

        PKMN.PokemonPC PC = gameSave.PokemonPC;
        for(int i = 0; i < PC.Length; ++i)
        {
            PKMN.PokemonBox box = PC[i];
            for(int j = 0; j < box.Length; ++j)
            {
                box[j] = Util.GetRandomPokemon(game, itemList, moveNameList, pokemonList);
            }
        }
    }

    private static void CompareItemLists(
        PKMN.ItemList itemList1,
        PKMN.ItemList itemList2
    )
    {
        Assert.AreEqual(itemList1.Game, itemList2.Game);
        Assert.AreEqual(itemList1.Name, itemList2.Name);
        Assert.AreEqual(itemList1.Length, itemList2.Length);
        Assert.AreEqual(itemList1.NumItems, itemList2.NumItems);

        for(int i = 0; i < itemList1.Length; ++i)
        {
            Assert.AreEqual(itemList1[i].Item, itemList2[i].Item);
            Assert.AreEqual(itemList1[i].Amount, itemList2[i].Amount);
        }
    }

    private static void ComparePokedexes(
        PKMN.Pokedex pokedex1,
        PKMN.Pokedex pokedex2
    )
    {
        Assert.AreEqual(pokedex1.AllSeen, pokedex2.AllSeen);
        Assert.AreEqual(pokedex1.AllCaught, pokedex2.AllCaught);
    }

    public static void CompareGameSaves(
        PKMN.GameSave save1,
        PKMN.GameSave save2
    )
    {
        PKMN.Game game = save1.Game;
        int generation = Util.GameToGeneration(game);

        Assert.AreEqual(save1.Game, save2.Game);
        Assert.AreEqual(save1.TrainerName, save2.TrainerName);
        Assert.AreEqual(save1.TrainerID, save2.TrainerID);
        Assert.AreEqual(save1.TrainerPublicID, save2.TrainerPublicID);

        if(!IsGBGame(game))
        {
            Assert.AreEqual(save1.TrainerSecretID, save2.TrainerSecretID);
        }
        if(!IsMaleOnly(game))
        {
            Assert.AreEqual(save1.TrainerGender, save2.TrainerGender);
        }
        if(!IsRivalNameSet(game))
        {
            Assert.AreEqual(save1.RivalName, save2.RivalName);
        }

        Assert.AreEqual(save1.Money, save2.Money);

        Assert.AreEqual(save1.ItemBag.Count, save2.ItemBag.Count);
        foreach(string pocketName in save1.ItemBag.PocketNames)
        {
            CompareItemLists(
                save1.ItemBag[pocketName],
                save2.ItemBag[pocketName]
            );
        }

        if(generation <= 3)
        {
            CompareItemLists(
                save1.ItemPC,
                save2.ItemPC
            );
        }

        Assert.AreEqual(save1.PokemonParty.NumPokemon, save2.PokemonParty.NumPokemon);
        for(int i = 0; i < 6; ++i)
        {
            Util.ComparePokemon(
                save1.PokemonParty[i],
                save2.PokemonParty[i]
            );
        }

        PKMN.PokemonPC pokemonPC1 = save1.PokemonPC;
        PKMN.PokemonPC pokemonPC2 = save2.PokemonPC;

        if(generation >= 2)
        {
            Assert.AreEqual(pokemonPC1.BoxNames.Count, pokemonPC2.BoxNames.Count);
            for(int i = 0; i < pokemonPC1.BoxNames.Count; ++i)
            {
                Assert.AreEqual(pokemonPC1.BoxNames[i], pokemonPC2.BoxNames[i]);
            }
        }
        for(int i = 0; i < pokemonPC1.Length; ++i)
        {
            PKMN.PokemonBox pokemonBox1 = pokemonPC1[i];
            PKMN.PokemonBox pokemonBox2 = pokemonPC2[i];

            if(generation >= 2)
            {
                Assert.AreEqual(pokemonBox1.Name, pokemonBox2.Name);
            }
            Assert.AreEqual(pokemonBox1.NumPokemon, pokemonBox2.NumPokemon);
            for(int j = 0; j < pokemonBox1.NumPokemon; ++j)
            {
                Util.ComparePokemon(
                    pokemonBox1[j],
                    pokemonBox2[j]
                );
            }
        }
        if(!IsGameGamecube(game))
        {
            ComparePokedexes(
                save1.Pokedex,
                save2.Pokedex
            );
        }

        Assert.AreEqual(save1.NumericAttributes.Names, save2.NumericAttributes.Names);
        foreach(string attributeName in save1.NumericAttributes.Names)
        {
            Assert.AreEqual(
                save1.NumericAttributes[attributeName],
                save2.NumericAttributes[attributeName]
            );
        }
        Assert.AreEqual(save1.StringAttributes.Names, save2.StringAttributes.Names);
        foreach(string attributeName in save1.StringAttributes.Names)
        {
            Assert.AreEqual(
                save1.StringAttributes[attributeName],
                save2.StringAttributes[attributeName]
            );
        }
        Assert.AreEqual(save1.BooleanAttributes.Names, save2.BooleanAttributes.Names);
        foreach(string attributeName in save1.BooleanAttributes.Names)
        {
            Assert.AreEqual(
                save1.BooleanAttributes[attributeName],
                save2.BooleanAttributes[attributeName]
            );
        }
    }

    public static void TestGameSave(
        PKMN.GameSaveType type,
        PKMN.Game game,
        string subPath
    )
    {
        string saveFilepath = "";
        if(IsGameGamecube(game))
        {
            saveFilepath = Path.GetFullPath(Path.Combine(LIBPKMN_TEST_FILES, subPath));
        }
        else
        {
            saveFilepath = Path.GetFullPath(Path.Combine(PKSAV_TEST_SAVES, subPath));
        }
        Assert.AreEqual(PKMN.GameSave.DetectType(saveFilepath), type);

        PKMN.GameSave gameSave = new PKMN.GameSave(saveFilepath);
        Assert.AreEqual(gameSave.Filepath, saveFilepath);
        Assert.AreEqual(gameSave.Game, game);

        PKMN.ItemEnumList itemList = PKMN.Database.Lists.ItemList(game);

        TestCommonFields(gameSave);
        TestAttributes(gameSave);

        // TODO: RandomizeItems
        RandomizePokemon(
            gameSave,
            itemList
        );

        string tempSaveFilepath = Path.GetFullPath(
                                      Path.Combine(
                                          PKMN_TMP_DIR,
                                          String.Format("{0}_{1}.sav", game, rng.Next())
                                      )
                                  );
        gameSave.SaveAs(tempSaveFilepath);
        PKMN.GameSave gameSave2 = new PKMN.GameSave(tempSaveFilepath);
        Assert.AreEqual(gameSave2.Filepath, tempSaveFilepath);

        CompareGameSaves(
            gameSave,
            gameSave2
        );

        File.Delete(tempSaveFilepath);
    }

}

}
