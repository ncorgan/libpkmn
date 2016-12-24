/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
                PKMN.Database.GetAbilityList(0);
            }
        );

        PKMN.StringList abilityList = PKMN.Database.GetAbilityList(6);
        Assert.That(abilityList.Count, Is.EqualTo(191));
        Assert.That(abilityList[0], Is.EqualTo("Adaptability"));
        Assert.That(abilityList[190], Is.EqualTo("Zen Mode"));
    }

    [Test]
    public void GameListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Database.GetGameList(7, false);
            }
        );

        PKMN.StringList gameList = PKMN.Database.GetGameList(6, true);
        Assert.That(gameList.Count, Is.EqualTo(26));
        Assert.That(gameList[0], Is.EqualTo("Red"));
        Assert.That(gameList[25], Is.EqualTo("Alpha Sapphire"));
    }

    [Test]
    public void ItemListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Database.GetItemList("Not a game");
            }
        );

        PKMN.StringList itemList = PKMN.Database.GetItemList("HeartGold");
        Assert.That(itemList.Count, Is.EqualTo(513));
        Assert.That(itemList[0], Is.EqualTo("Adamant Orb"));
        Assert.That(itemList[512], Is.EqualTo("Zoom Lens"));
    }

    [Test]
    public void LocationListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Database.GetLocationList("Not a game", false);
            }
        );

        PKMN.StringList locationList = PKMN.Database.GetLocationList("Emerald", false);
        Assert.That(locationList.Count, Is.EqualTo(106));
        Assert.That(locationList[0], Is.EqualTo("Abandoned Ship"));
        Assert.That(locationList[105], Is.EqualTo("Victory Road"));
    }

    [Test]
    public void MoveListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Database.GetMoveList("Not a game");
            }
        );

        PKMN.StringList moveList = PKMN.Database.GetMoveList("Red");
        Assert.That(moveList.Count, Is.EqualTo(165));
        Assert.That(moveList[0], Is.EqualTo("Pound"));
        Assert.That(moveList[164], Is.EqualTo("Struggle"));
    }

    [Test]
    public void NatureListTest() {
        PKMN.StringList natureList = PKMN.Database.GetNatureList();
        Assert.That(natureList.Count, Is.EqualTo(25));
        Assert.That(natureList[0], Is.EqualTo("Adamant"));
        Assert.That(natureList[24], Is.EqualTo("Timid"));
    }

    [Test]
    public void PokemonListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Database.GetPokemonList(0, true);
            }
        );

        PKMN.StringList pokemonList = PKMN.Database.GetPokemonList(1, true);
        Assert.That(pokemonList.Count, Is.EqualTo(151));
        Assert.That(pokemonList[0], Is.EqualTo("Bulbasaur"));
        Assert.That(pokemonList[150], Is.EqualTo("Mew"));
    }

    [Test]
    public void RegionListTest() {
        PKMN.StringList regionList = PKMN.Database.GetRegionList();
        Assert.That(regionList.Count, Is.EqualTo(7));
        Assert.That(regionList[0], Is.EqualTo("Kanto"));
        Assert.That(regionList[6], Is.EqualTo("Kalos"));
    }

    [Test]
    [Ignore("Feature not implemented")]
    public void RibbonListTest() {
    }

    [Test]
    public void SuperTrainingMedalListTest() {
        PKMN.StringList superTrainingMedalList = PKMN.Database.GetSuperTrainingMedalList();
        Assert.That(superTrainingMedalList.Count, Is.EqualTo(30));
        Assert.That(superTrainingMedalList[0], Is.EqualTo("Sp. Atk Level 1"));
        Assert.That(superTrainingMedalList[29], Is.EqualTo("The Battle for the Best!"));
    }

    [Test]
    public void TypeListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Database.GetTypeList("Not a game");
            }
        );

        PKMN.StringList typeList = PKMN.Database.GetTypeList("Alpha Sapphire");
        Assert.That(typeList.Count, Is.EqualTo(18));
        Assert.That(typeList[0], Is.EqualTo("Normal"));
        Assert.That(typeList[17], Is.EqualTo("Fairy"));
    }
}
