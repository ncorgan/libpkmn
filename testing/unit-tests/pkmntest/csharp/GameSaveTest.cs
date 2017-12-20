/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using System.IO;
using NUnit.Framework;

namespace PKMNTest {

public class GameSaveTest {

    private static System.Random rng = new System.Random();

    private static string PKSAV_TEST_SAVES = System.Environment.GetEnvironmentVariable("PKSAV_TEST_SAVES");
    private static string LIBPKMN_TEST_FILES = System.Environment.GetEnvironmentVariable("LIBPKMN_TEST_FILES");
    private static string PKMN_TMP_DIR = PKMN.Paths.GetTmpDir();

    private static ushort DEFAULT_TRAINER_PID = 1351;
    private static ushort DEFAULT_TRAINER_SID = 32123;

    private static int MONEY_MAX = 999999;

    private static string[] RIVAL_NAME_SET_GAMES = {
        "Ruby", "Sapphire", "Emerald",
        "Colosseum", "XD",
        "Black", "White",
        "X", "Y"
    };

    private static string[] MALE_ONLY_GAMES = {
        "Red", "Blue", "Yellow",
        "Gold", "Silver",
        "Colosseum", "XD"
    };

    /*
     * Utility functions
     */

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

    private static bool isGBGame(
        string game
    ) {
        return (GameToGeneration(game) <= 2);
    }

    private static bool isMaleOnly(
        string game
    ) {
        return (Array.IndexOf(MALE_ONLY_GAMES, game) > -1);
    }

    private static bool isRivalNameSet(
        string game
    ) {
        return (Array.IndexOf(RIVAL_NAME_SET_GAMES, game) > -1);
    }

    private static void TestTrainerID(
        PKMN.GameSave gameSave
    ) {
        bool gbGame = isGBGame(gameSave.Game);

        Assert.AreEqual(
            gameSave.TrainerID,
            gbGame ? DEFAULT_TRAINER_PID : PKMN.Pokemon.DEFAULT_TRAINER_ID
        );
        if(gbGame) {
            Assert.Throws<ApplicationException>(
                delegate {
                    ushort SID = gameSave.TrainerSecretID;
                }
            );
        } else {
            Assert.AreEqual(gameSave.TrainerSecretID, DEFAULT_TRAINER_SID);
        }
    }

    private static void TestCommonFields(
        PKMN.GameSave gameSave
    ) {
        String game = gameSave.Game;

        // Trainer name
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                gameSave.TrainerName = "";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                gameSave.TrainerName = "LibPKMNLibPKMN";
            }
        );
        gameSave.TrainerName = "LibPKMN";
        Assert.AreEqual(gameSave.TrainerName, "LibPKMN");

        // Trainer ID
        gameSave.TrainerID = isGBGame(game) ? DEFAULT_TRAINER_PID : PKMN.Pokemon.DEFAULT_TRAINER_ID;
        TestTrainerID(gameSave);
        gameSave.TrainerPublicID = DEFAULT_TRAINER_PID;
        TestTrainerID(gameSave);
        if(isGBGame(game)) {
            Assert.Throws<ApplicationException>(
                delegate {
                    gameSave.TrainerSecretID = DEFAULT_TRAINER_SID;
                }
            );
        } else {
            gameSave.TrainerSecretID = DEFAULT_TRAINER_SID;
            TestTrainerID(gameSave);
        }

        // Rival Name
        if(isRivalNameSet(game)) {
            Assert.Throws<ApplicationException>(
                delegate {
                   gameSave.RivalName = PKMN.Pokemon.DEFAULT_TRAINER_NAME;
                }
            );
        } else {
           gameSave.RivalName = PKMN.Pokemon.DEFAULT_TRAINER_NAME;
           Assert.AreEqual(gameSave.RivalName, PKMN.Pokemon.DEFAULT_TRAINER_NAME);
        }

        // Trainer Gender
        if(isMaleOnly(game)) {
            Assert.AreEqual(gameSave.TrainerGender, "Male");
            Assert.Throws<ApplicationException>(
                delegate {
                   gameSave.TrainerGender = "Male";
                }
            );
            Assert.Throws<ApplicationException>(
                delegate {
                   gameSave.TrainerGender = "Female";
                }
            );
        } else {
            gameSave.TrainerGender = "Male";
            Assert.AreEqual(gameSave.TrainerGender, "Male");
            gameSave.TrainerGender = "Female";
            Assert.AreEqual(gameSave.TrainerGender, "Female");
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                   gameSave.TrainerGender = "Genderless";
                }
            );
        }

        // Money
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
               gameSave.Money = -1;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
               gameSave.Money = MONEY_MAX+1;
            }
        );
        gameSave.Money = 123456;
        Assert.AreEqual(gameSave.Money, 123456);

        // Pokémon Party
        PKMN.PokemonParty party = gameSave.PokemonParty;
        int numPokemon = party.NumPokemon;
        Assert.AreEqual(party.AsList().Count, 6);

        Assert.Greater(numPokemon, 0);
        Assert.LessOrEqual(numPokemon, 6);
        for(int i = 0; i < 6; ++i) {
            if(i < numPokemon) {
                Assert.AreNotEqual(party[i].Species, "None");
            } else {
                Assert.AreEqual(party[i].Species, "None");
            }
        }

        // Pokémon PC
        PKMN.PokemonPC pokemonPC = gameSave.PokemonPC;
        Assert.AreEqual(pokemonPC.Count, pokemonPC.AsList().Count);
        for(int i = 0; i < pokemonPC.Count; ++i) {
            PKMN.PokemonBox box = pokemonPC[i];

            Assert.LessOrEqual(box.NumPokemon, box.Count);
            Assert.AreEqual(box.Count, box.AsList().Count);

            // Boxes are only contiguous in Game Boy games.
            if(isGBGame(game)) {
                numPokemon = box.NumPokemon;
                for(int j = 0; j < box.Count; ++j) {
                    if(j < numPokemon) {
                        Assert.AreNotEqual(box[j].Species, "None");
                    } else {
                        Assert.AreEqual(box[j].Species, "None");
                    }
                }
            }
        }
    }

    private static PKMN.Pokemon GetRandomPokemon(
        string game,
        PKMN.StringList pokemonList,
        PKMN.StringList moveList,
        PKMN.StringList itemList
    ) {
        int generation = GameToGeneration(game);

        // Don't deal with Deoxys issues here.
        string species = "";
        do {
            species = pokemonList[rng.Next(0, pokemonList.Count)];
        } while(generation == 3 && species.Equals("Deoxys"));

        PKMN.Pokemon ret = new PKMN.Pokemon(
                                   species,
                                   game,
                                   "",
                                   rng.Next(2, 101)
                               );

        for(int i = 0; i < 4; ++i) {
            string move = "";
            do
            {
                move = moveList[rng.Next(0, moveList.Count)];
            } while(move.StartsWith("Shadow"));
            ret.SetMove(move, i);
        }

        if(generation >= 2) {
            // Keep going until one is holdable
            PKMN.Database.ItemEntry itemEntry;
            do {
                itemEntry = new PKMN.Database.ItemEntry(itemList[rng.Next(0, itemList.Count)], game);
            } while(!itemEntry.IsHoldable);

            ret.HeldItem = itemEntry.Name;
        }

        return ret;
    }

    private static void RandomizePokemon(
        PKMN.GameSave gameSave,
        PKMN.StringList itemList
    ) {
        string game = gameSave.Game;
        int generation = GameToGeneration(game);
        PKMN.StringList pokemonList = PKMN.Database.Lists.PokemonList(generation, true);
        PKMN.StringList moveList = PKMN.Database.Lists.MoveList(game);

        PKMN.PokemonParty party = gameSave.PokemonParty;
        for(int i = 0; i < 6; ++i) {
            party[i] = GetRandomPokemon(
                           game,
                           pokemonList,
                           moveList,
                           itemList
                       );
        }

        PKMN.PokemonPC PC = gameSave.PokemonPC;
        for(int i = 0; i < PC.Count; ++i) {
            PKMN.PokemonBox box = PC[i];
            for(int j = 0; j < box.Count; ++j) {
                box[j] = GetRandomPokemon(
                             game,
                             pokemonList,
                             moveList,
                             itemList
                         );
            }
        }
    }

    public static void CompareItemLists(
        PKMN.ItemList itemList1,
        PKMN.ItemList itemList2
    ) {
        Assert.AreEqual(itemList1.Game, itemList2.Game);
        Assert.AreEqual(itemList1.Name, itemList2.Name);
        Assert.AreEqual(itemList1.Capacity, itemList2.Capacity);
        Assert.AreEqual(itemList1.NumItems, itemList2.NumItems);

        for(int i = 0; i < itemList1.Capacity; ++i) {
            Assert.AreEqual(itemList1[i].Item, itemList2[i].Item);
            Assert.AreEqual(itemList1[i].Amount, itemList2[i].Amount);
        }
    }

    public static void ComparePokemon(
        PKMN.Pokemon pokemon1,
        PKMN.Pokemon pokemon2
    ) {
        Assert.AreEqual(pokemon1.Game, pokemon2.Game);
        Assert.AreEqual(pokemon1.Species, pokemon2.Species);
        Assert.AreEqual(pokemon1.Form, pokemon2.Form);
        Assert.AreEqual(pokemon1.Nickname, pokemon2.Nickname);
        Assert.AreEqual(pokemon1.OriginalTrainerName, pokemon2.OriginalTrainerName);

        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(pokemon1.Moves[i].Move, pokemon2.Moves[i].Move);
            Assert.AreEqual(pokemon1.Moves[i].PP, pokemon2.Moves[i].PP);
        }

        if(GameToGeneration(pokemon1.Game) >= 2) {
            Assert.AreEqual(pokemon1.HeldItem, pokemon2.HeldItem);
        }
    }

    public static void CompareGameSaves(
        PKMN.GameSave save1,
        PKMN.GameSave save2
    ) {
        string game = save1.Game;
        int generation = GameToGeneration(game);

        Assert.AreEqual(save1.Game, save2.Game);
        Assert.AreEqual(save1.TrainerName, save2.TrainerName);
        Assert.AreEqual(save1.TrainerID, save2.TrainerID);
        Assert.AreEqual(save1.TrainerPublicID, save2.TrainerPublicID);

        if(!isGBGame(game)) {
            Assert.AreEqual(save1.TrainerSecretID, save2.TrainerSecretID);
        }
        if(!isMaleOnly(game)) {
            Assert.AreEqual(save1.TrainerGender, save2.TrainerGender);
        }
        if(!isRivalNameSet(game)) {
            Assert.AreEqual(save1.RivalName, save2.RivalName);
        }

        Assert.AreEqual(save1.Money, save2.Money);

        Assert.AreEqual(save1.ItemBag.Pockets.Count, save2.ItemBag.Pockets.Count);
        foreach(string pocketName in save1.ItemBag.PocketNames) {
            CompareItemLists(
                save1.ItemBag[pocketName],
                save2.ItemBag[pocketName]
            );
        }

        if(generation <= 3) {
            CompareItemLists(
                save1.ItemPC,
                save2.ItemPC
            );
        }

        Assert.AreEqual(save1.PokemonParty.NumPokemon, save2.PokemonParty.NumPokemon);
        for(int i = 0; i < 6; ++i) {
            ComparePokemon(
                save1.PokemonParty[i],
                save2.PokemonParty[i]
            );
        }

        PKMN.PokemonPC pokemonPC1 = save1.PokemonPC;
        PKMN.PokemonPC pokemonPC2 = save2.PokemonPC;

        if(generation >= 2) {
            Assert.AreEqual(pokemonPC1.BoxNames.Count, pokemonPC2.BoxNames.Count);
            for(int i = 0; i < pokemonPC1.BoxNames.Count; ++i) {
                Assert.AreEqual(pokemonPC1.BoxNames[i], pokemonPC2.BoxNames[i]);
            }
        }
        for(int i = 0; i < pokemonPC1.Count; ++i) {
            PKMN.PokemonBox pokemonBox1 = pokemonPC1[i];
            PKMN.PokemonBox pokemonBox2 = pokemonPC2[i];

            if(generation >= 2) {
                Assert.AreEqual(pokemonBox1.Name, pokemonBox2.Name);
            }
            Assert.AreEqual(pokemonBox1.NumPokemon, pokemonBox2.NumPokemon);
            for(int j = 0; j < pokemonBox1.NumPokemon; ++j) {
                ComparePokemon(
                    pokemonBox1[j],
                    pokemonBox2[j]
                );
            }
        }
    }

    public static void TestGameSave(
        string type,
        string game,
        string subPath
    ) {
        string saveFilepath = "";
        if(game.Equals("Colosseum") || game.Equals("XD"))
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

        PKMN.StringList itemList = PKMN.Database.Lists.ItemList(game);

        TestCommonFields(gameSave);
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
