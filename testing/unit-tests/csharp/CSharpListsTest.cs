/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpListsTest {
    [Test]
    public void AbilityListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Database.Lists.AbilityList(0);
            }
        );

        PKMN.StringList abilityList = PKMN.Database.Lists.AbilityList(6);
        Assert.AreEqual(abilityList.Count, 191);
        Assert.AreEqual(abilityList[0], "Adaptability");
        Assert.AreEqual(abilityList[190], "Zen Mode");
    }

    [Test]
    public void GameListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Database.Lists.GameList(7, false);
            }
        );

        PKMN.StringList gameList = PKMN.Database.Lists.GameList(6, true);
        Assert.AreEqual(gameList.Count, 26);
        Assert.AreEqual(gameList[0], "Red");
        Assert.AreEqual(gameList[25], "Alpha Sapphire");
    }

    [Test]
    public void GamecubeShadowPokemonListTest() {
        PKMN.StringList colosseumShadowPokemonList = PKMN.Database.Lists.GamecubeShadowPokemonList(true);
        Assert.AreEqual(colosseumShadowPokemonList.Count, 48);

        PKMN.StringList xdShadowPokemonList = PKMN.Database.Lists.GamecubeShadowPokemonList(false);
        Assert.AreEqual(xdShadowPokemonList.Count, 83);
    }

    [Test]
    public void ItemListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Database.Lists.ItemList("Not a game");
            }
        );

        PKMN.StringList itemList = PKMN.Database.Lists.ItemList("HeartGold");
        Assert.AreEqual(itemList.Count, 513);
        Assert.AreEqual(itemList[0], "Adamant Orb");
        Assert.AreEqual(itemList[512], "Zoom Lens");
    }

    [Test]
    public void LocationListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Database.Lists.LocationList("Not a game", false);
            }
        );

        PKMN.StringList locationList = PKMN.Database.Lists.LocationList("Emerald", false);
        Assert.AreEqual(locationList.Count, 106);
        Assert.AreEqual(locationList[0], "Abandoned Ship");
        Assert.AreEqual(locationList[105], "Victory Road");
    }

    [Test]
    public void MoveListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Database.Lists.MoveList("Not a game");
            }
        );

        PKMN.StringList moveList = PKMN.Database.Lists.MoveList("Red");
        Assert.AreEqual(moveList.Count, 165);
        Assert.AreEqual(moveList[0], "Pound");
        Assert.AreEqual(moveList[164], "Struggle");
    }

    [Test]
    public void NatureListTest() {
        PKMN.StringList natureList = PKMN.Database.Lists.NatureList();
        Assert.AreEqual(natureList.Count, 25);
        Assert.AreEqual(natureList[0], "Hardy");
        Assert.AreEqual(natureList[24], "Quirky");
    }

    [Test]
    public void PokemonListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Database.Lists.PokemonList(0, true);
            }
        );

        PKMN.StringList pokemonList = PKMN.Database.Lists.PokemonList(1, true);
        Assert.AreEqual(pokemonList.Count, 151);
        Assert.AreEqual(pokemonList[0], "Bulbasaur");
        Assert.AreEqual(pokemonList[150], "Mew");
    }

    [Test]
    public void RegionListTest() {
        PKMN.StringList regionList = PKMN.Database.Lists.RegionList();
        Assert.AreEqual(regionList.Count, 7);
        Assert.AreEqual(regionList[0], "Kanto");
        Assert.AreEqual(regionList[6], "Kalos");
    }

    [Test]
    [Ignore("Feature not implemented")]
    public void RibbonListTest() {
    }

    [Test]
    public void SuperTrainingMedalListTest() {
        PKMN.StringList superTrainingMedalList = PKMN.Database.Lists.SuperTrainingMedalList();
        Assert.AreEqual(superTrainingMedalList.Count, 30);
        Assert.AreEqual(superTrainingMedalList[0], "Sp. Atk Level 1");
        Assert.AreEqual(superTrainingMedalList[29], "The Battle for the Best!");
    }

    [Test]
    public void TypeListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Database.Lists.TypeList("Not a game");
            }
        );

        PKMN.StringList typeList = PKMN.Database.Lists.TypeList("Alpha Sapphire");
        Assert.AreEqual(typeList.Count, 18);
        Assert.AreEqual(typeList[0], "Normal");
        Assert.AreEqual(typeList[17], "Fairy");
    }
}
