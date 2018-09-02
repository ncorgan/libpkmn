/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpListsTest
{
    [Test]
    public void AbilityListTest()
    {
        // Make sure trying to create invalid lists results in an exception.
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.AbilityList(0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.AbilityNameList(0);
            }
        );

        PKMN.AbilityEnumList abilityList = PKMN.Database.Lists.AbilityList(6);
        Assert.AreEqual(abilityList.Count, 191);
        Assert.AreEqual(abilityList[0], PKMN.Ability.STENCH);
        Assert.AreEqual(abilityList[190], PKMN.Ability.DELTA_STREAM);

        PKMN.StringList abilityNameList = PKMN.Database.Lists.AbilityNameList(6);
        Assert.AreEqual(abilityNameList.Count, 191);
        Assert.AreEqual(abilityNameList[0], "Stench");
        Assert.AreEqual(abilityNameList[190], "Delta Stream");

        // Make sure lists match.
        Assert.AreEqual(abilityList.Count, abilityNameList.Count);
        for(int abilityIndex = 0; abilityIndex < abilityList.Count; ++abilityIndex)
        {
            string abilityName = PKMN.PKMN.AbilityToString(abilityList[abilityIndex]);
            Assert.AreEqual(abilityName, abilityNameList[abilityIndex]);
        }
    }

    [Test]
    public void GameListTest()
    {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.GameList(7, false);
            }
        );

        PKMN.GameEnumList gameList = PKMN.Database.Lists.GameList(6, true);
        Assert.AreEqual(gameList.Count, 26);
        Assert.AreEqual(gameList[0], PKMN.Game.RED);
        Assert.AreEqual(gameList[25], PKMN.Game.ALPHA_SAPPHIRE);

        PKMN.StringList gameNameList = PKMN.Database.Lists.GameNameList(6, true);
        Assert.AreEqual(gameNameList.Count, 26);
        Assert.AreEqual(gameNameList[0], "Red");
        Assert.AreEqual(gameNameList[25], "Alpha Sapphire");

        // Make sure lists match.
        Assert.AreEqual(gameList.Count, gameNameList.Count);
        for(int gameIndex = 0; gameIndex < gameList.Count; ++gameIndex)
        {
            string gameName = PKMN.PKMN.GameToString(gameList[gameIndex]);
            Assert.AreEqual(gameName, gameNameList[gameIndex]);
        }
    }

    [Test]
    public void GamecubeShadowPokemonListTest()
    {
        //
        // Colosseum
        //

        PKMN.SpeciesEnumList colosseumShadowPokemonList = PKMN.Database.Lists.GamecubeShadowPokemonList(true);
        Assert.AreEqual(colosseumShadowPokemonList.Count, 48);

        PKMN.StringList colosseumShadowPokemonNameList = PKMN.Database.Lists.GamecubeShadowPokemonNameList(true);
        Assert.AreEqual(colosseumShadowPokemonNameList.Count, 48);

        // Make sure lists match.
        for(int pokemonIndex = 0; pokemonIndex < colosseumShadowPokemonList.Count; ++pokemonIndex)
        {
            string pokemonName = PKMN.PKMN.SpeciesToString(colosseumShadowPokemonList[pokemonIndex]);
            Assert.AreEqual(pokemonName, colosseumShadowPokemonNameList[pokemonIndex]);
        }

        //
        // XD
        //

        PKMN.SpeciesEnumList xdShadowPokemonList = PKMN.Database.Lists.GamecubeShadowPokemonList(false);
        Assert.AreEqual(xdShadowPokemonList.Count, 83);
        PKMN.StringList xdShadowPokemonNameList = PKMN.Database.Lists.GamecubeShadowPokemonNameList(false);
        Assert.AreEqual(xdShadowPokemonNameList.Count, 83);

        // Make sure lists match.
        for(int pokemonIndex = 0; pokemonIndex < xdShadowPokemonList.Count; ++pokemonIndex)
        {
            string pokemonName = PKMN.PKMN.SpeciesToString(xdShadowPokemonList[pokemonIndex]);
            Assert.AreEqual(pokemonName, xdShadowPokemonNameList[pokemonIndex]);
        }
    }

    [Test]
    public void HMMoveListTest()
    {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.HMMoveList(PKMN.Game.NONE);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.HMMoveNameList(PKMN.Game.NONE);
            }
        );

        PKMN.MoveEnumList hmMoveList = PKMN.Database.Lists.HMMoveList(PKMN.Game.RED);
        Assert.AreEqual(hmMoveList.Count, 5);
        Assert.AreEqual(hmMoveList[0], PKMN.Move.CUT);
        Assert.AreEqual(hmMoveList[4], PKMN.Move.FLASH);

        PKMN.StringList hmMoveNameList = PKMN.Database.Lists.HMMoveNameList(PKMN.Game.RED);
        Assert.AreEqual(hmMoveNameList.Count, 5);
        Assert.AreEqual(hmMoveNameList[0], "Cut");
        Assert.AreEqual(hmMoveNameList[4], "Flash");

        // Make sure lists match.
        Assert.AreEqual(hmMoveList.Count, hmMoveNameList.Count);
        for(int hmMoveIndex = 0; hmMoveIndex < hmMoveList.Count; ++hmMoveIndex)
        {
            string hmMoveName = PKMN.PKMN.MoveToString(hmMoveList[hmMoveIndex]);
            Assert.AreEqual(hmMoveName, hmMoveNameList[hmMoveIndex]);
        }
    }

    [Test]
    public void ItemListTest()
    {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.ItemList(PKMN.Game.NONE);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.ItemNameList(PKMN.Game.NONE);
            }
        );

        PKMN.ItemEnumList itemList = PKMN.Database.Lists.ItemList(PKMN.Game.HEARTGOLD);
        Assert.AreEqual(itemList.Count, 513);
        Assert.AreEqual(itemList[0], PKMN.Item.MASTER_BALL);
        Assert.AreEqual(itemList[512], PKMN.Item.PHOTO_ALBUM);

        PKMN.StringList itemNameList = PKMN.Database.Lists.ItemNameList(PKMN.Game.HEARTGOLD);
        Assert.AreEqual(itemNameList.Count, 513);
        Assert.AreEqual(itemNameList[0], "Master Ball");
        Assert.AreEqual(itemNameList[512], "Photo Album");

        // Make sure lists match. Note that for items, we need to check from
        // name to enum, as there are multiple names for some items but always
        // a single enum.
        Assert.AreEqual(itemList.Count, itemNameList.Count);
        for(int itemIndex = 0; itemIndex < itemList.Count; ++itemIndex)
        {
            PKMN.Item item = PKMN.PKMN.StringToItem(itemNameList[itemIndex]);
            Assert.AreEqual(item, itemList[itemIndex]);
        }
    }

    [Test]
    public void LocationListTest()
    {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.LocationNameList(PKMN.Game.NONE, false);
            }
        );

        PKMN.StringList locationNameList = PKMN.Database.Lists.LocationNameList(PKMN.Game.EMERALD, false);
        Assert.AreEqual(locationNameList.Count, 106);
        Assert.AreEqual(locationNameList[0], "Abandoned Ship");
        Assert.AreEqual(locationNameList[105], "Victory Road");
    }

    [Test]
    public void MoveListTest()
    {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.MoveList(PKMN.Game.NONE);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.MoveNameList(PKMN.Game.NONE);
            }
        );

        PKMN.MoveEnumList moveList = PKMN.Database.Lists.MoveList(PKMN.Game.RED);
        Assert.AreEqual(moveList.Count, 165);
        Assert.AreEqual(moveList[0], PKMN.Move.POUND);
        Assert.AreEqual(moveList[164], PKMN.Move.STRUGGLE);

        PKMN.StringList moveNameList = PKMN.Database.Lists.MoveNameList(PKMN.Game.RED);
        Assert.AreEqual(moveNameList.Count, 165);
        Assert.AreEqual(moveNameList[0], "Pound");
        Assert.AreEqual(moveNameList[164], "Struggle");

        // Make sure lists match. Note that for moves, we need to check from
        // name to enum, as there are multiple names for some moves but always
        // a single enum.
        Assert.AreEqual(moveList.Count, moveNameList.Count);
        for(int moveIndex = 0; moveIndex < moveList.Count; ++moveIndex)
        {
            PKMN.Move move = PKMN.PKMN.StringToMove(moveNameList[moveIndex]);
            Assert.AreEqual(move, moveList[moveIndex]);
        }
    }

    [Test]
    public void NatureListTest()
    {
        PKMN.NatureEnumList natureList = PKMN.Database.Lists.NatureList();
        Assert.AreEqual(natureList.Count, 25);
        Assert.AreEqual(natureList[0], PKMN.Nature.HARDY);
        Assert.AreEqual(natureList[24], PKMN.Nature.QUIRKY);

        PKMN.StringList natureNameList = PKMN.Database.Lists.NatureNameList();
        Assert.AreEqual(natureNameList.Count, 25);
        Assert.AreEqual(natureNameList[0], "Hardy");
        Assert.AreEqual(natureNameList[24], "Quirky");

        // Make sure lists match.
        Assert.AreEqual(natureList.Count, natureNameList.Count);
        for(int natureIndex = 0; natureIndex < natureList.Count; ++natureIndex)
        {
            string natureName = PKMN.PKMN.NatureToString(natureList[natureIndex]);
            Assert.AreEqual(natureName, natureNameList[natureIndex]);
        }
    }

    [Test]
    public void PokemonListTest()
    {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.PokemonList(0, true);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.PokemonNameList(0, true);
            }
        );

        PKMN.SpeciesEnumList speciesList = PKMN.Database.Lists.PokemonList(1, true);
        Assert.AreEqual(speciesList.Count, 151);
        Assert.AreEqual(speciesList[0], PKMN.Species.BULBASAUR);
        Assert.AreEqual(speciesList[150], PKMN.Species.MEW);

        PKMN.StringList speciesNameList = PKMN.Database.Lists.PokemonNameList(1, true);
        Assert.AreEqual(speciesNameList.Count, 151);
        Assert.AreEqual(speciesNameList[0], "Bulbasaur");
        Assert.AreEqual(speciesNameList[150], "Mew");

        // Make sure lists match.
        Assert.AreEqual(speciesList.Count, speciesNameList.Count);
        for(int speciesIndex = 0; speciesIndex < speciesList.Count; ++speciesIndex)
        {
            string speciesName = PKMN.PKMN.SpeciesToString(speciesList[speciesIndex]);
            Assert.AreEqual(speciesName, speciesNameList[speciesIndex]);
        }
    }

    [Test]
    public void RegionListTest()
    {
        PKMN.StringList regionNameList = PKMN.Database.Lists.RegionNameList();
        Assert.AreEqual(regionNameList.Count, 7);
        Assert.AreEqual(regionNameList[0], "Kanto");
        Assert.AreEqual(regionNameList[6], "Kalos");
    }

    [Test]
    [Ignore("Feature not implemented")]
    public void RibbonListTest()
    {
    }

    [Test]
    public void SuperTrainingMedalListTest()
    {
        PKMN.StringList superTrainingMedalNameList = PKMN.Database.Lists.SuperTrainingMedalNameList();
        Assert.AreEqual(superTrainingMedalNameList.Count, 30);
        Assert.AreEqual(superTrainingMedalNameList[0], "Sp. Atk Level 1");
        Assert.AreEqual(superTrainingMedalNameList[29], "The Battle for the Best!");
    }

    [Test]
    public void TMMoveListTest()
    {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.TMMoveList(PKMN.Game.NONE);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.TMMoveNameList(PKMN.Game.NONE);
            }
        );

        PKMN.MoveEnumList tmMoveList = PKMN.Database.Lists.TMMoveList(PKMN.Game.RED);
        Assert.AreEqual(tmMoveList.Count, 50);
        Assert.AreEqual(tmMoveList[0], PKMN.Move.MEGA_PUNCH);
        Assert.AreEqual(tmMoveList[49], PKMN.Move.SUBSTITUTE);

        PKMN.StringList tmMoveNameList = PKMN.Database.Lists.TMMoveNameList(PKMN.Game.RED);
        Assert.AreEqual(tmMoveNameList.Count, 50);
        Assert.AreEqual(tmMoveNameList[0], "Mega Punch");
        Assert.AreEqual(tmMoveNameList[49], "Substitute");

        // Make sure lists match.
        Assert.AreEqual(tmMoveList.Count, tmMoveNameList.Count);
        for(int tmMoveIndex = 0; tmMoveIndex < tmMoveList.Count; ++tmMoveIndex)
        {
            string tmMoveName = PKMN.PKMN.MoveToString(tmMoveList[tmMoveIndex]);
            Assert.AreEqual(tmMoveName, tmMoveNameList[tmMoveIndex]);
        }
    }

    [Test]
    public void TypeListTest()
    {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.TypeList(PKMN.Game.NONE);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Database.Lists.TypeNameList(PKMN.Game.NONE);
            }
        );

        PKMN.TypeEnumList typeList = PKMN.Database.Lists.TypeList(PKMN.Game.ALPHA_SAPPHIRE);
        Assert.AreEqual(typeList.Count, 18);
        Assert.AreEqual(typeList[0], PKMN.Type.NORMAL);
        Assert.AreEqual(typeList[17], PKMN.Type.FAIRY);

        PKMN.StringList typeNameList = PKMN.Database.Lists.TypeNameList(PKMN.Game.ALPHA_SAPPHIRE);
        Assert.AreEqual(typeNameList.Count, 18);
        Assert.AreEqual(typeNameList[0], "Normal");
        Assert.AreEqual(typeNameList[17], "Fairy");

        // Make sure lists match.
        Assert.AreEqual(typeList.Count, typeNameList.Count);
        for(int typeIndex = 0; typeIndex < typeList.Count; ++typeIndex)
        {
            string typeName = PKMN.PKMN.TypeToString(typeList[typeIndex]);
            Assert.AreEqual(typeName, typeNameList[typeIndex]);
        }
    }
}
