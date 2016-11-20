/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using System.Collections.Generic;

public class CSharpEqualityTest {
    public static bool AbilityListTest() {
        System.Console.Write("Starting AbilityListTest...");

        try {
            try {
                PKMN.Database.GetAbilityList(0);
            } catch(System.ArgumentOutOfRangeException) {
                // We want this
            } catch(System.Exception ex) {
                System.Console.WriteLine(ex.GetType().Name);
                System.Console.WriteLine(ex.Message);
                System.Console.WriteLine(ex.StackTrace);
                System.Console.WriteLine(ex.InnerException.GetType().Name);
                System.Console.WriteLine(ex.InnerException.Message);
                System.Console.WriteLine(ex.InnerException.StackTrace);
                throw new System.Exception("This should have thrown an ArgumentOutOfRangeException.");
            }

            PKMN.StringList abilityList = PKMN.Database.GetAbilityList(6);
            if(abilityList.Count != 191) {
                throw new System.Exception("abilityList.Count != 191");
            }
            if(!abilityList[0].Equals("Adaptability")) {
                throw new System.Exception("!abilityList[0].Equals(\"Adaptability\"), is " + abilityList[0]);
            }
            if(!abilityList[abilityList.Count-1].Equals("Zen Mode")) {
                throw new System.Exception("!abilityList[abilityList.Count-1].Equals(\"Zen Mode\"), is \"" + abilityList[abilityList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static bool GameListTest() {
        System.Console.Write("Starting GameListTest...");
        try {
            try {
                PKMN.Database.GetGameList(7, false);
            } catch(System.ArgumentOutOfRangeException) {
                // We want this
            } catch(System.Exception) {
                throw new System.Exception("This should have thrown an ArgumentOutOfRangeException.");
            }

            PKMN.StringList gameList = PKMN.Database.GetGameList(6, true);
            if(gameList.Count != 24) {
                throw new System.Exception("gameList.Count != 24");
            }
            if(!gameList[0].Equals("Red")) {
                throw new System.Exception("!gameList[0].Equals(\"Red\"), is " + gameList[0]);
            }
            if(!gameList[gameList.Count-1].Equals("Alpha Sapphire")) {
                throw new System.Exception("!gameList[gameList.Count-1].Equals(\"Alpha Sapphire\"), is \"" + gameList[gameList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static bool ItemListTest() {
        System.Console.Write("Starting ItemListTest...");
        try {
            try {
                PKMN.Database.GetItemList("Not a game");
            } catch(System.ArgumentException) {
                // We want this
            } catch(System.Exception) {
                throw new System.Exception("This should have thrown an ArgumentException.");
            }

            PKMN.StringList itemList = PKMN.Database.GetItemList("HeartGold");
            if(itemList.Count != 513) {
                throw new System.Exception("itemList.Count != 513");
            }
            if(!itemList[0].Equals("Adamant Orb")) {
                throw new System.Exception("!itemList[0].Equals(\"Adamant Orb\"), is " + itemList[0]);
            }
            if(!itemList[itemList.Count-1].Equals("Zoom Lens")) {
                throw new System.Exception("!itemList[itemList.Count-1].Equals(\"Zoom Lens\"), is \"" + itemList[itemList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static bool LocationListTest() {
        System.Console.Write("Starting LocationListTest...");
        try {
            try {
                PKMN.Database.GetLocationList("Not a game", false);
            } catch(System.ArgumentException) {
                // We want this
            } catch(System.Exception) {
                throw new System.Exception("This should have thrown an ArgumentException.");
            }

            PKMN.StringList locationList = PKMN.Database.GetLocationList("Emerald", false);
            if(locationList.Count != 165) {
                throw new System.Exception("locationList.Count != 165");
            }
            if(!locationList[0].Equals("Abandoned Ship")) {
                throw new System.Exception("!locationList[0].Equals(\"Abandoned Ship\"), is " + locationList[0]);
            }
            if(!locationList[locationList.Count-1].Equals("Victory Road")) {
                throw new System.Exception("!locationList[locationList.Count-1].Equals(\"Victory Road\"), is \"" + locationList[locationList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static bool MoveListTest() {
        System.Console.Write("Starting MoveListTest...");
        try {
            try {
                PKMN.Database.GetMoveList("Not a game");
            } catch(System.ArgumentException) {
                // We want this
            } catch(System.Exception) {
                throw new System.Exception("This should have thrown an ArgumentException.");
            }

            PKMN.StringList moveList = PKMN.Database.GetMoveList("Red");
            if(moveList.Count != 165) {
                throw new System.Exception("moveList.Count != 165");
            }
            if(!moveList[0].Equals("Pound")) {
                throw new System.Exception("!moveList[0].Equals(\"Pound\"), is " + moveList[0]);
            }
            if(!moveList[moveList.Count-1].Equals("Struggle")) {
                throw new System.Exception("!moveList[moveList.Count-1].Equals(\"Struggle\"), is \"" + moveList[moveList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static bool NatureListTest() {
        System.Console.Write("Starting NatureListTest...");
        try {
            PKMN.StringList natureList = PKMN.Database.GetNatureList();
            if(natureList.Count != 25) {
                throw new System.Exception("natureList.Count != 25");
            }
            if(!natureList[0].Equals("Adamant")) {
                throw new System.Exception("!natureList[0].Equals(\"Adamant\"), is " + natureList[0]);
            }
            if(!natureList[natureList.Count-1].Equals("Timid")) {
                throw new System.Exception("!natureList[natureList.Count-1].Equals(\"Timid\"), is \"" + natureList[natureList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static bool PokemonListTest() {
        System.Console.Write("Starting PokemonListTest...");
        try {
            try {
                PKMN.Database.GetPokemonList(0, true);
            } catch(System.ArgumentOutOfRangeException) {
                // We want this
            } catch(System.Exception) {
                throw new System.Exception("This should have thrown an ArgumentOutOfRangeException.");
            }

            PKMN.StringList pokemonList = PKMN.Database.GetPokemonList(1, true);
            if(pokemonList.Count != 151) {
                throw new System.Exception("pokemonList.Count != 151");
            }
            if(!pokemonList[0].Equals("Bulbasaur")) {
                throw new System.Exception("!pokemonList[0].Equals(\"Bulbasaur\"), is " + pokemonList[0]);
            }
            if(!pokemonList[pokemonList.Count-1].Equals("Mew")) {
                throw new System.Exception("!pokemonList[pokemonList.Count-1].Equals(\"Mew\"), is \"" + pokemonList[pokemonList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static bool RegionListTest() {
        System.Console.Write("Starting RegionListTest...");
        try {
            PKMN.StringList regionList = PKMN.Database.GetRegionList();
            if(regionList.Count != 7) {
                throw new System.Exception("regionList.Count != 7");
            }
            if(!regionList[0].Equals("Kanto")) {
                throw new System.Exception("!regionList[0].Equals(\"Kanto\"), is " + regionList[0]);
            }
            if(!regionList[regionList.Count-1].Equals("Kalos")) {
                throw new System.Exception("!regionList[regionList.Count-1].Equals(\"Kalos\"), is \"" + regionList[regionList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    // TODO
    public static bool RibbonListTest() {
        return true;
    }

    public static bool SuperTrainingMedalListTest() {
        System.Console.Write("Starting SuperTrainingMedalListTest...");
        try {
            PKMN.StringList superTrainingMedalList = PKMN.Database.GetSuperTrainingMedalList();
            if(superTrainingMedalList.Count != 30) {
                throw new System.Exception("superTrainingMedalList.Count != 30");
            }
            if(!superTrainingMedalList[0].Equals("Sp. Atk Level 1")) {
                throw new System.Exception("!superTrainingMedalList[0].Equals(\"Sp. Atk Level 1\"), is " + superTrainingMedalList[0]);
            }
            if(!superTrainingMedalList[superTrainingMedalList.Count-1].Equals("The Battle for the Best!")) {
                throw new System.Exception("!superTrainingMedalList[superTrainingMedalList.Count-1].Equals(\"The Battle for the Best!\"), is \"" + superTrainingMedalList[superTrainingMedalList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static bool TypeListTest() {
        System.Console.Write("Starting TypeListTest...");
        try {
            try {
                PKMN.Database.GetTypeList("Not a game");
            } catch(System.ArgumentException) {
                // We want this
            } catch(System.Exception) {
                throw new System.Exception("This should have thrown an ArgumentException.");
            }

            PKMN.StringList typeList = PKMN.Database.GetTypeList("Alpha Sapphire");
            if(typeList.Count != 18) {
                throw new System.Exception("typeList.Count != 18");
            }
            if(!typeList[0].Equals("Normal")) {
                throw new System.Exception("!typeList[0].Equals(\"Normal\"), is " + typeList[0]);
            }
            if(!typeList[typeList.Count-1].Equals("Fairy")) {
                throw new System.Exception("!typeList[typeList.Count-1].Equals(\"Fairy\"), is \"" + typeList[typeList.Count-1] + "\"");
            }

            System.Console.WriteLine("success.\n");
        } catch (System.Exception ex) {
            System.Console.WriteLine("failed.\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }

        return true;
    }

    public static int Main(string[] args) {
        bool successful = AbilityListTest() && GameListTest() && ItemListTest() &&
                          LocationListTest() && MoveListTest() && NatureListTest() &&
                          PokemonListTest() && RegionListTest() && RibbonListTest() &&
                          SuperTrainingMedalListTest() && TypeListTest();

        return successful ? 0 : 1;
    }
}
