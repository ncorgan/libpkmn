/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using System.Drawing;
using System.IO;
using NUnit.Framework;

[TestFixture]
public class CSharpGUITest
{
    private static string LIBPKMN_TEST_FILES = Environment.GetEnvironmentVariable("LIBPKMN_TEST_FILES");
    private static string PKMN_TMP_DIR       = PKMN.Paths.GetTmpDir();

    [Test]
    public void AbilityListComboBoxTest()
    {
        PKMN.GUI.AbilityListComboBox abilityListComboBox = new PKMN.GUI.AbilityListComboBox(6);
        Assert.AreEqual(abilityListComboBox.Items.Count, 191);
        Assert.AreEqual(abilityListComboBox.Items[0], "Adaptability");
        Assert.AreEqual(abilityListComboBox.Items[190], "Zen Mode");
    }

    [Test]
    public void GameListComboBoxTest()
    {
        PKMN.GUI.GameListComboBox gameListComboBox = new PKMN.GUI.GameListComboBox(6, true);
        Assert.AreEqual(gameListComboBox.Items.Count, 26);
        Assert.AreEqual(gameListComboBox.Items[0], "Red");
        Assert.AreEqual(gameListComboBox.Items[25], "Alpha Sapphire");
    }

    [Test]
    public void ItemListComboBoxTest()
    {
        PKMN.GUI.ItemListComboBox itemListComboBox = new PKMN.GUI.ItemListComboBox("HeartGold");
        Assert.AreEqual(itemListComboBox.Items.Count, 513);
        Assert.AreEqual(itemListComboBox.Items[0], "Adamant Orb");
        Assert.AreEqual(itemListComboBox.Items[512], "Zoom Lens");
    }

    [Test]
    public void LocationListComboBoxTest()
    {
        PKMN.GUI.LocationListComboBox locationListComboBox = new PKMN.GUI.LocationListComboBox("Emerald", false);
        Assert.AreEqual(locationListComboBox.Items.Count, 106);
        Assert.AreEqual(locationListComboBox.Items[0], "Abandoned Ship");
        Assert.AreEqual(locationListComboBox.Items[105], "Victory Road");
    }

    [Test]
    public void MoveListComboBoxTest()
    {
        PKMN.GUI.MoveListComboBox moveListComboBox = new PKMN.GUI.MoveListComboBox("Red");
        Assert.AreEqual(moveListComboBox.Items.Count, 165);
        Assert.AreEqual(moveListComboBox.Items[0], "Pound");
        Assert.AreEqual(moveListComboBox.Items[164], "Struggle");
    }

    [Test]
    public void NatureListComboBoxTest()
    {
        PKMN.GUI.NatureListComboBox natureListComboBox = new PKMN.GUI.NatureListComboBox();
        Assert.AreEqual(natureListComboBox.Items.Count, 25);
        Assert.AreEqual(natureListComboBox.Items[0], "Hardy");
        Assert.AreEqual(natureListComboBox.Items[24], "Quirky");
    }

    [Test]
    public void PokemonListComboBoxTest()
    {
        PKMN.GUI.PokemonListComboBox pokemonListComboBox = new PKMN.GUI.PokemonListComboBox(1, true);
        Assert.AreEqual(pokemonListComboBox.Items.Count, 151);
        Assert.AreEqual(pokemonListComboBox.Items[0], "Bulbasaur");
        Assert.AreEqual(pokemonListComboBox.Items[150], "Mew");
    }

    [Test]
    public void RegionListComboBoxTest()
    {
        PKMN.GUI.RegionListComboBox regionListComboBox = new PKMN.GUI.RegionListComboBox();
        Assert.AreEqual(regionListComboBox.Items.Count, 7);
        Assert.AreEqual(regionListComboBox.Items[0], "Kanto");
        Assert.AreEqual(regionListComboBox.Items[6], "Kalos");
    }

    [Test]
    [Ignore("Not implemented yet")]
    public void RibbonListComboBoxTest()
    {
    }

    private void CompareImages(
        Image image1,
        Image image2
    )
    {
        Bitmap image1Bitmap = (Bitmap)image1;
        Bitmap image2Bitmap = (Bitmap)image2;
        Assert.AreNotEqual(image1Bitmap, null);
        Assert.AreNotEqual(image2Bitmap, null);

        Assert.AreEqual(image1Bitmap.Size, image2Bitmap.Size);
        for(int i = 0; i < image1Bitmap.Width; ++i)
        {
            for(int j = 0; j < image1Bitmap.Height; ++j)
            {
                Assert.AreEqual(
                    image1Bitmap.GetPixel(i, j),
                    image2Bitmap.GetPixel(i, j)
                );
            }
        }
    }

    [Test]
    public void SpindaTest()
    {
        string spindaFormat = "spinda_{0}_{1}_{2}.png";

        uint personality      = 0x88888888;
        uint personalityShiny = 0xF81C8021;

        for(int generation = 3; generation <= 5; ++generation)
        {
            string spindaFilename = String.Format(spindaFormat, generation, 0, personality);

            string testFilesSpindaPath = Path.GetFullPath(
                                             Path.Combine(
                                                 LIBPKMN_TEST_FILES,
                                                 "spinda",
                                                 spindaFilename
                                             )
                                         );
            Assert.IsTrue(File.Exists(testFilesSpindaPath));
            Image testFilesSpinda = Image.FromFile(testFilesSpindaPath);

            string testSpindaPath = Path.GetFullPath(
                                        Path.Combine(
                                            PKMN_TMP_DIR,
                                            spindaFilename
                                        )
                                    );
            PKMN.GUI.GenerateSpindaSpriteAtFilepath(
                generation,
                personality,
                false,
                testSpindaPath
            );
            Assert.IsTrue(File.Exists(testSpindaPath));
            Image testSpindaFromFilepath = Image.FromFile(testSpindaPath);
            CompareImages(testFilesSpinda, testSpindaFromFilepath);
            File.Delete(testSpindaPath);

            Image testSpinda = PKMN.GUI.GenerateSpindaSprite(
                                   generation,
                                   personality,
                                   false
                               );
            CompareImages(testFilesSpinda, testSpinda);

            spindaFilename = String.Format(spindaFormat, generation, 1, personalityShiny);

            testFilesSpindaPath = Path.GetFullPath(
                Path.Combine(
                    LIBPKMN_TEST_FILES,
                    "spinda",
                    spindaFilename
                )
            );
            Assert.IsTrue(File.Exists(testFilesSpindaPath));
            testFilesSpinda = Image.FromFile(testFilesSpindaPath);

            testSpindaPath = Path.GetFullPath(
                Path.Combine(
                    PKMN_TMP_DIR,
                    spindaFilename
                )
            );
            PKMN.GUI.GenerateSpindaSpriteAtFilepath(
                generation,
                personalityShiny,
                true,
                testSpindaPath
            );
            Assert.IsTrue(File.Exists(testSpindaPath));
            testSpindaFromFilepath = Image.FromFile(testSpindaPath);
            CompareImages(testFilesSpinda, testSpindaFromFilepath);
            File.Delete(testSpindaPath);

            testSpinda = PKMN.GUI.GenerateSpindaSprite(
                             generation,
                             personalityShiny,
                             true
                         );
            CompareImages(testFilesSpinda, testSpinda);
        }
    }

    [Test]
    public void SuperTrainingMedalListComboBoxTest()
    {
        PKMN.GUI.SuperTrainingMedalListComboBox superTrainingMedalListComboBox = new PKMN.GUI.SuperTrainingMedalListComboBox();
        Assert.AreEqual(superTrainingMedalListComboBox.Items.Count, 30);
        Assert.AreEqual(superTrainingMedalListComboBox.Items[0], "Sp. Atk Level 1");
        Assert.AreEqual(superTrainingMedalListComboBox.Items[29], "The Battle for the Best!");
    }

    [Test]
    public void TypeListComboBoxTest()
    {
        PKMN.GUI.TypeListComboBox typeListComboBox = new PKMN.GUI.TypeListComboBox("Alpha Sapphire");
        Assert.AreEqual(typeListComboBox.Items.Count, 18);
        Assert.AreEqual(typeListComboBox.Items[0], "Normal");
        Assert.AreEqual(typeListComboBox.Items[17], "Fairy");
    }
}
