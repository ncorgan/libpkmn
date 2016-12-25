/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpWinFormsTest {
    [Test]
    public void AbilityListComboBoxTest() {
        PKMN.AbilityListComboBox abilityListComboBox = new PKMN.AbilityListComboBox(6);
        Assert.AreEqual(abilityListComboBox.Items.Count, 191);
        Assert.AreEqual(abilityListComboBox.Items[0], "Adaptability");
        Assert.AreEqual(abilityListComboBox.Items[190], "Zen Mode");
    }

    [Test]
    public void GameListComboBoxTest() {
        PKMN.GameListComboBox gameListComboBox = new PKMN.GameListComboBox(6, true);
        Assert.AreEqual(gameListComboBox.Items.Count, 26);
        Assert.AreEqual(gameListComboBox.Items[0], "Red");
        Assert.AreEqual(gameListComboBox.Items[25], "Alpha Sapphire");
    }

    [Test]
    public void ItemListComboBoxTest() {
        PKMN.ItemListComboBox itemListComboBox = new PKMN.ItemListComboBox("HeartGold");
        Assert.AreEqual(itemListComboBox.Items.Count, 513);
        Assert.AreEqual(itemListComboBox.Items[0], "Adamant Orb");
        Assert.AreEqual(itemListComboBox.Items[512], "Zoom Lens");
    }

    [Test]
    public void LocationListComboBoxTest() {
        PKMN.LocationListComboBox locationListComboBox = new PKMN.LocationListComboBox("Emerald", false);
        Assert.AreEqual(locationListComboBox.Items.Count, 106);
        Assert.AreEqual(locationListComboBox.Items[0], "Abandoned Ship");
        Assert.AreEqual(locationListComboBox.Items[105], "Victory Road");
    }

    [Test]
    public void MoveListComboBoxTest() {
        PKMN.MoveListComboBox moveListComboBox = new PKMN.MoveListComboBox("Red");
        Assert.AreEqual(moveListComboBox.Items.Count, 165);
        Assert.AreEqual(moveListComboBox.Items[0], "Pound");
        Assert.AreEqual(moveListComboBox.Items[164], "Struggle");
    }

    [Test]
    public void NatureListComboBoxTest() {
        PKMN.NatureListComboBox natureListComboBox = new PKMN.NatureListComboBox();
        Assert.AreEqual(natureListComboBox.Items.Count, 25);
        Assert.AreEqual(natureListComboBox.Items[0], "Adamant");
        Assert.AreEqual(natureListComboBox.Items[24], "Timid");
    }

    [Test]
    public void PokemonListComboBoxTest() {
        PKMN.PokemonListComboBox pokemonListComboBox = new PKMN.PokemonListComboBox(1, true);
        Assert.AreEqual(pokemonListComboBox.Items.Count, 151);
        Assert.AreEqual(pokemonListComboBox.Items[0], "Bulbasaur");
        Assert.AreEqual(pokemonListComboBox.Items[150], "Mew");
    }

    [Test]
    public void RegionListComboBoxTest() {
        PKMN.RegionListComboBox regionListComboBox = new PKMN.RegionListComboBox();
        Assert.AreEqual(regionListComboBox.Items.Count, 7);
        Assert.AreEqual(regionListComboBox.Items[0], "Kanto");
        Assert.AreEqual(regionListComboBox.Items[6], "Kalos");
    }

    [Test]
    [Ignore("Not implemented yet")]
    public void RibbonListComboBoxTest() {
    }

    [Test]
    public void SuperTrainingMedalListComboBoxTest() {
        PKMN.SuperTrainingMedalListComboBox superTrainingMedalListComboBox = new PKMN.SuperTrainingMedalListComboBox();
        Assert.AreEqual(superTrainingMedalListComboBox.Items.Count, 30);
        Assert.AreEqual(superTrainingMedalListComboBox.Items[0], "Sp. Atk Level 1");
        Assert.AreEqual(superTrainingMedalListComboBox.Items[29], "The Battle for the Best!");
    }

    [Test]
    public void TypeListComboBoxTest() {
        PKMN.TypeListComboBox typeListComboBox = new PKMN.TypeListComboBox("Alpha Sapphire");
        Assert.AreEqual(typeListComboBox.Items.Count, 18);
        Assert.AreEqual(typeListComboBox.Items[0], "Normal");
        Assert.AreEqual(typeListComboBox.Items[17], "Fairy");
    }
}
