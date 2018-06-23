/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpEqualityHashCodeTest
{
    /*
     * LibPKMN classes
     */

    [Test]
    public void HiddenPowerTest()
    {
        PKMN.HiddenPower hiddenPower = new PKMN.HiddenPower("Normal", 30);

        PKMN.HiddenPower hiddenPowerSame = new PKMN.HiddenPower("Normal", 30);
        PKMN.HiddenPower hiddenPowerDifferentType = new PKMN.HiddenPower("Fire", 30);
        PKMN.HiddenPower hiddenPowerDifferentBasePower = new PKMN.HiddenPower("Normal", 40);

        Assert.AreEqual(hiddenPower, hiddenPower);
        Assert.AreEqual(hiddenPower, hiddenPowerSame);
        Assert.AreEqual(hiddenPower.GetHashCode(), hiddenPowerSame.GetHashCode());

        Assert.AreNotEqual(hiddenPower, hiddenPowerDifferentType);
        Assert.AreNotEqual(hiddenPower.GetHashCode(), hiddenPowerDifferentType.GetHashCode());

        Assert.AreNotEqual(hiddenPower, hiddenPowerDifferentBasePower);
        Assert.AreNotEqual(hiddenPower.GetHashCode(), hiddenPowerDifferentBasePower.GetHashCode());
    }

    [Test]
    public void ItemEntryTest()
    {
        PKMN.Database.ItemEntry itemEntry = new PKMN.Database.ItemEntry("Potion", PKMN.Game.RED);

        PKMN.Database.ItemEntry itemEntrySame = new PKMN.Database.ItemEntry("Potion", PKMN.Game.RED);
        PKMN.Database.ItemEntry itemEntryDifferentItem = new PKMN.Database.ItemEntry("Great Ball", PKMN.Game.RED);
        PKMN.Database.ItemEntry itemEntryDifferentGame = new PKMN.Database.ItemEntry("Potion", PKMN.Game.BLUE);

        Assert.AreEqual(itemEntry, itemEntry);
        Assert.AreEqual(itemEntry, itemEntrySame);
        Assert.AreEqual(itemEntry.GetHashCode(), itemEntrySame.GetHashCode());

        Assert.AreNotEqual(itemEntry, itemEntryDifferentItem);
        Assert.AreNotEqual(itemEntry.GetHashCode(), itemEntryDifferentItem.GetHashCode());

        Assert.AreNotEqual(itemEntry, itemEntryDifferentGame);
        Assert.AreNotEqual(itemEntry.GetHashCode(), itemEntryDifferentGame.GetHashCode());
    }

    [Test]
    public void LevelupMoveTest()
    {
        PKMN.Database.LevelupMove levelupMove = new PKMN.Database.LevelupMove(
                                                        new PKMN.Database.MoveEntry("Scratch", PKMN.Game.RED),
                                                        10
                                                    );

        PKMN.Database.LevelupMove levelupMoveSame = new PKMN.Database.LevelupMove(
                                                            new PKMN.Database.MoveEntry("Scratch", PKMN.Game.RED),
                                                            10
                                                        );
        PKMN.Database.LevelupMove levelupMoveDifferentMove = new PKMN.Database.LevelupMove(
                                                                     new PKMN.Database.MoveEntry("Tackle", PKMN.Game.RED),
                                                                     10
                                                                 );
        PKMN.Database.LevelupMove levelupMoveDifferentGame = new PKMN.Database.LevelupMove(
                                                                     new PKMN.Database.MoveEntry("Scratch", PKMN.Game.BLUE),
                                                                     10
                                                                 );
        PKMN.Database.LevelupMove levelupMoveDifferentLevel = new PKMN.Database.LevelupMove(
                                                                      new PKMN.Database.MoveEntry("Scratch", PKMN.Game.RED),
                                                                      5
                                                                  );

        Assert.AreEqual(levelupMove, levelupMove);
        Assert.AreEqual(levelupMove, levelupMoveSame);
        Assert.AreEqual(levelupMove.GetHashCode(), levelupMoveSame.GetHashCode());

        Assert.AreNotEqual(levelupMove, levelupMoveDifferentMove);
        Assert.AreNotEqual(levelupMove.GetHashCode(), levelupMoveDifferentMove.GetHashCode());

        Assert.AreNotEqual(levelupMove, levelupMoveDifferentGame);
        Assert.AreNotEqual(levelupMove.GetHashCode(), levelupMoveDifferentGame.GetHashCode());

        Assert.AreNotEqual(levelupMove, levelupMoveDifferentLevel);
        Assert.AreNotEqual(levelupMove.GetHashCode(), levelupMoveDifferentLevel.GetHashCode());
    }

    [Test]
    public void MoveEntryTest()
    {
        PKMN.Database.MoveEntry moveEntry = new PKMN.Database.MoveEntry("Scratch", PKMN.Game.RED);

        PKMN.Database.MoveEntry moveEntrySame = new PKMN.Database.MoveEntry("Scratch", PKMN.Game.RED);
        PKMN.Database.MoveEntry moveEntryDifferentMove = new PKMN.Database.MoveEntry("Tackle", PKMN.Game.RED);
        PKMN.Database.MoveEntry moveEntryDifferentGame = new PKMN.Database.MoveEntry("Scratch", PKMN.Game.BLUE);

        Assert.AreEqual(moveEntry, moveEntry);
        Assert.AreEqual(moveEntry, moveEntrySame);
        Assert.AreEqual(moveEntry.GetHashCode(), moveEntrySame.GetHashCode());

        Assert.AreNotEqual(moveEntry, moveEntryDifferentMove);
        Assert.AreNotEqual(moveEntry.GetHashCode(), moveEntryDifferentMove.GetHashCode());

        Assert.AreNotEqual(moveEntry, moveEntryDifferentGame);
        Assert.AreNotEqual(moveEntry.GetHashCode(), moveEntryDifferentGame.GetHashCode());
    }

    [Test]
    public void NaturalGiftTest()
    {
        PKMN.NaturalGift naturalGift = new PKMN.NaturalGift("Normal", 30);

        PKMN.NaturalGift naturalGiftSame = new PKMN.NaturalGift("Normal", 30);
        PKMN.NaturalGift naturalGiftDifferentType = new PKMN.NaturalGift("Fire", 30);
        PKMN.NaturalGift naturalGiftDifferentBasePower = new PKMN.NaturalGift("Normal", 40);

        Assert.AreEqual(naturalGift, naturalGift);
        Assert.AreEqual(naturalGift, naturalGiftSame);
        Assert.AreEqual(naturalGift.GetHashCode(), naturalGiftSame.GetHashCode());

        Assert.AreNotEqual(naturalGift, naturalGiftDifferentType);
        Assert.AreNotEqual(naturalGift.GetHashCode(), naturalGiftDifferentType.GetHashCode());

        Assert.AreNotEqual(naturalGift, naturalGiftDifferentBasePower);
        Assert.AreNotEqual(naturalGift.GetHashCode(), naturalGiftDifferentBasePower.GetHashCode());
    }

    [Test]
    public void PokemonEntryTest()
    {
        PKMN.Database.PokemonEntry pokemonEntry = new PKMN.Database.PokemonEntry("Venusaur", PKMN.Game.OMEGA_RUBY, "");

        PKMN.Database.PokemonEntry pokemonEntrySame = new PKMN.Database.PokemonEntry("Venusaur", PKMN.Game.OMEGA_RUBY, "");
        PKMN.Database.PokemonEntry pokemonEntryDifferentPokemon = new PKMN.Database.PokemonEntry("Charizard", PKMN.Game.OMEGA_RUBY, "");
        PKMN.Database.PokemonEntry pokemonEntryDifferentGame = new PKMN.Database.PokemonEntry("Venusaur", PKMN.Game.RUBY, "");
        PKMN.Database.PokemonEntry pokemonEntryDifferentForm = new PKMN.Database.PokemonEntry("Venusaur", PKMN.Game.OMEGA_RUBY, "Mega");

        Assert.AreEqual(pokemonEntry, pokemonEntry);
        Assert.AreEqual(pokemonEntry, pokemonEntrySame);
        Assert.AreEqual(pokemonEntry.GetHashCode(), pokemonEntrySame.GetHashCode());

        Assert.AreNotEqual(pokemonEntry, pokemonEntryDifferentPokemon);
        Assert.AreNotEqual(pokemonEntry.GetHashCode(), pokemonEntryDifferentPokemon.GetHashCode());

        Assert.AreNotEqual(pokemonEntry, pokemonEntryDifferentGame);
        Assert.AreNotEqual(pokemonEntry.GetHashCode(), pokemonEntryDifferentGame.GetHashCode());

        Assert.AreNotEqual(pokemonEntry, pokemonEntryDifferentForm);
        Assert.AreNotEqual(pokemonEntry.GetHashCode(), pokemonEntryDifferentForm.GetHashCode());
    }

    [Test]
    public void SpindaCoordsTest()
    {
        PKMN.SpindaCoords spindaCoords = new PKMN.SpindaCoords(5, 10);

        PKMN.SpindaCoords spindaCoordsSame = new PKMN.SpindaCoords(5, 10);
        PKMN.SpindaCoords spindaCoordsDifferentX = new PKMN.SpindaCoords(6, 10);
        PKMN.SpindaCoords spindaCoordsDifferentY = new PKMN.SpindaCoords(5, 11);
        PKMN.SpindaCoords spindaCoordsReversed = new PKMN.SpindaCoords(10, 5);

        Assert.AreEqual(spindaCoords, spindaCoords);
        Assert.AreEqual(spindaCoords, spindaCoordsSame);
        Assert.AreEqual(spindaCoords.GetHashCode(), spindaCoordsSame.GetHashCode());

        Assert.AreNotEqual(spindaCoords, spindaCoordsDifferentX);
        Assert.AreNotEqual(spindaCoords.GetHashCode(), spindaCoordsDifferentX.GetHashCode());

        Assert.AreNotEqual(spindaCoords, spindaCoordsDifferentY);
        Assert.AreNotEqual(spindaCoords.GetHashCode(), spindaCoordsDifferentY.GetHashCode());

        Assert.AreNotEqual(spindaCoords, spindaCoordsReversed);
        Assert.AreNotEqual(spindaCoords.GetHashCode(), spindaCoordsReversed.GetHashCode());
    }

    [Test]
    public void SpindaSpotsTest()
    {
        PKMN.SpindaSpots spindaSpots = new PKMN.SpindaSpots(
                                               new PKMN.SpindaCoords(1, 2),
                                               new PKMN.SpindaCoords(3, 4),
                                               new PKMN.SpindaCoords(5, 6),
                                               new PKMN.SpindaCoords(7, 8)
                                           );

        PKMN.SpindaSpots spindaSpotsSame = new PKMN.SpindaSpots(
                                                   new PKMN.SpindaCoords(1, 2),
                                                   new PKMN.SpindaCoords(3, 4),
                                                   new PKMN.SpindaCoords(5, 6),
                                                   new PKMN.SpindaCoords(7, 8)
                                               );
        PKMN.SpindaSpots spindaSpotsReversed = new PKMN.SpindaSpots(
                                                       new PKMN.SpindaCoords(2, 1),
                                                       new PKMN.SpindaCoords(4, 3),
                                                       new PKMN.SpindaCoords(6, 5),
                                                       new PKMN.SpindaCoords(8, 7)
                                                   );
        PKMN.SpindaSpots spindaSpotsShuffled = new PKMN.SpindaSpots(
                                                       new PKMN.SpindaCoords(3, 4),
                                                       new PKMN.SpindaCoords(7, 8),
                                                       new PKMN.SpindaCoords(5, 6),
                                                       new PKMN.SpindaCoords(1, 2)
                                                   );

        Assert.AreEqual(spindaSpots, spindaSpots);
        Assert.AreEqual(spindaSpots, spindaSpotsSame);
        Assert.AreEqual(spindaSpots.GetHashCode(), spindaSpotsSame.GetHashCode());

        Assert.AreNotEqual(spindaSpots, spindaSpotsReversed);
        Assert.AreNotEqual(spindaSpots.GetHashCode(), spindaSpotsReversed.GetHashCode());

        Assert.AreNotEqual(spindaSpots, spindaSpotsShuffled);
        Assert.AreNotEqual(spindaSpots.GetHashCode(), spindaSpotsShuffled.GetHashCode());
    }

    [Test]
    public void PokedexHelpersTest()
    {
        PKMN.Pokedex pokedex = new PKMN.Pokedex(PKMN.Game.RUBY);

        // The Pokemon class and its helpers all use the underlying pointer
        // in generating their hash codes. Make sure these aren't equal.

        Assert.AreNotEqual(pokedex.GetHashCode(), pokedex.SeenPokemonMap.GetHashCode());
        Assert.AreNotEqual(pokedex.GetHashCode(), pokedex.CaughtPokemonMap.GetHashCode());
        Assert.AreNotEqual(pokedex.SeenPokemonMap.GetHashCode(), pokedex.CaughtPokemonMap.GetHashCode());
    }

    [Test]
    public void PokemonHelpersTest()
    {
        PKMN.Pokemon pokemon = new PKMN.Pokemon("Mew", PKMN.Game.RUBY, "", 70);

        // The Pokemon class and its helpers all use the underlying pointer
        // in generating their hash codes. Make sure these aren't equal.

        Assert.AreNotEqual(pokemon.GetHashCode(), pokemon.EVs.GetHashCode());
        Assert.AreNotEqual(pokemon.GetHashCode(), pokemon.IVs.GetHashCode());
        Assert.AreNotEqual(pokemon.GetHashCode(), pokemon.ContestStats.GetHashCode());
        Assert.AreNotEqual(pokemon.GetHashCode(), pokemon.Markings.GetHashCode());
        Assert.AreNotEqual(pokemon.GetHashCode(), pokemon.Ribbons.GetHashCode());
        Assert.AreNotEqual(pokemon.GetHashCode(), pokemon.Moves.GetHashCode());
        Assert.AreNotEqual(pokemon.GetHashCode(), pokemon.NumericAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.GetHashCode(), pokemon.StringAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.GetHashCode(), pokemon.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(pokemon.EVs.GetHashCode(), pokemon.IVs.GetHashCode());
        Assert.AreNotEqual(pokemon.EVs.GetHashCode(), pokemon.ContestStats.GetHashCode());
        Assert.AreNotEqual(pokemon.EVs.GetHashCode(), pokemon.Markings.GetHashCode());
        Assert.AreNotEqual(pokemon.EVs.GetHashCode(), pokemon.Ribbons.GetHashCode());
        Assert.AreNotEqual(pokemon.EVs.GetHashCode(), pokemon.Moves.GetHashCode());
        Assert.AreNotEqual(pokemon.EVs.GetHashCode(), pokemon.NumericAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.EVs.GetHashCode(), pokemon.StringAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.EVs.GetHashCode(), pokemon.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(pokemon.IVs.GetHashCode(), pokemon.ContestStats.GetHashCode());
        Assert.AreNotEqual(pokemon.IVs.GetHashCode(), pokemon.Markings.GetHashCode());
        Assert.AreNotEqual(pokemon.IVs.GetHashCode(), pokemon.Ribbons.GetHashCode());
        Assert.AreNotEqual(pokemon.IVs.GetHashCode(), pokemon.Moves.GetHashCode());
        Assert.AreNotEqual(pokemon.IVs.GetHashCode(), pokemon.NumericAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.IVs.GetHashCode(), pokemon.StringAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.IVs.GetHashCode(), pokemon.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(pokemon.ContestStats.GetHashCode(), pokemon.Markings.GetHashCode());
        Assert.AreNotEqual(pokemon.ContestStats.GetHashCode(), pokemon.Ribbons.GetHashCode());
        Assert.AreNotEqual(pokemon.ContestStats.GetHashCode(), pokemon.Moves.GetHashCode());
        Assert.AreNotEqual(pokemon.ContestStats.GetHashCode(), pokemon.NumericAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.ContestStats.GetHashCode(), pokemon.StringAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.ContestStats.GetHashCode(), pokemon.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(pokemon.Markings.GetHashCode(), pokemon.Ribbons.GetHashCode());
        Assert.AreNotEqual(pokemon.Markings.GetHashCode(), pokemon.Moves.GetHashCode());
        Assert.AreNotEqual(pokemon.Markings.GetHashCode(), pokemon.NumericAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.Markings.GetHashCode(), pokemon.StringAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.Markings.GetHashCode(), pokemon.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(pokemon.Ribbons.GetHashCode(), pokemon.Moves.GetHashCode());
        Assert.AreNotEqual(pokemon.Ribbons.GetHashCode(), pokemon.NumericAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.Ribbons.GetHashCode(), pokemon.StringAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.Ribbons.GetHashCode(), pokemon.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(pokemon.Moves.GetHashCode(), pokemon.NumericAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.Moves.GetHashCode(), pokemon.StringAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.Moves.GetHashCode(), pokemon.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(pokemon.NumericAttributes.GetHashCode(), pokemon.StringAttributes.GetHashCode());
        Assert.AreNotEqual(pokemon.NumericAttributes.GetHashCode(), pokemon.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(pokemon.StringAttributes.GetHashCode(), pokemon.BooleanAttributes.GetHashCode());

        for(int moveIndex = 0; moveIndex < pokemon.Moves.Count; ++moveIndex)
        {
            Assert.AreNotEqual(pokemon.Moves.GetHashCode(), pokemon.Moves[moveIndex].GetHashCode());
        }
    }

    [Test]
    public void DaycareHelpersTest()
    {
        PKMN.Daycare daycare = new PKMN.Daycare(PKMN.Game.GOLD);

        Assert.AreNotEqual(daycare.GetHashCode(), daycare.LevelupPokemon.GetHashCode());
        Assert.AreNotEqual(daycare.GetHashCode(), daycare.BreedingPokemon.GetHashCode());

        Assert.AreNotEqual(daycare.LevelupPokemon.GetHashCode(), daycare.BreedingPokemon.GetHashCode());
    }

    [Test]
    public void GameSaveHelpersTest()
    {
        string pksavTestSaves = System.Environment.GetEnvironmentVariable("PKSAV_TEST_SAVES");
        string saveFilepath = System.IO.Path.GetFullPath(System.IO.Path.Combine(pksavTestSaves, "gold_silver/pokemon_gold.sav"));

        PKMN.GameSave gameSave = new PKMN.GameSave(saveFilepath);

        Assert.AreNotEqual(gameSave.GetHashCode(), gameSave.NumericAttributes.GetHashCode());
        Assert.AreNotEqual(gameSave.GetHashCode(), gameSave.StringAttributes.GetHashCode());
        Assert.AreNotEqual(gameSave.GetHashCode(), gameSave.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(gameSave.NumericAttributes.GetHashCode(), gameSave.StringAttributes.GetHashCode());
        Assert.AreNotEqual(gameSave.NumericAttributes.GetHashCode(), gameSave.BooleanAttributes.GetHashCode());

        Assert.AreNotEqual(gameSave.StringAttributes.GetHashCode(), gameSave.BooleanAttributes.GetHashCode());
    }

    /*
     * STL containers
     */

    [Test]
    public void StatIntDictTest()
    {
        PKMN.StatDict statDict = new PKMN.StatDict();
        statDict[PKMN.Stat.HP] = 12;
        statDict[PKMN.Stat.ATTACK] = 34;
        statDict[PKMN.Stat.DEFENSE] = 56;
        statDict[PKMN.Stat.SPEED] = 78;
        statDict[PKMN.Stat.SPECIAL] = 90;

        PKMN.StatDict statDictSame = new PKMN.StatDict();
        statDictSame[PKMN.Stat.SPECIAL] = 90;
        statDictSame[PKMN.Stat.SPEED] = 78;
        statDictSame[PKMN.Stat.DEFENSE] = 56;
        statDictSame[PKMN.Stat.ATTACK] = 34;
        statDictSame[PKMN.Stat.HP] = 12;

        PKMN.StatDict statDictShuffled = new PKMN.StatDict();
        statDictShuffled[PKMN.Stat.HP] = 78;
        statDictShuffled[PKMN.Stat.ATTACK] = 12;
        statDictShuffled[PKMN.Stat.DEFENSE] = 90;
        statDictShuffled[PKMN.Stat.SPEED] = 34;
        statDictShuffled[PKMN.Stat.SPECIAL] = 56;

        Assert.AreEqual(statDict, statDict);
        Assert.AreEqual(statDict, statDictSame);
        Assert.AreEqual(statDict.GetHashCode(), statDictSame.GetHashCode());

        Assert.AreNotEqual(statDict, statDictShuffled);
        Assert.AreNotEqual(statDict.GetHashCode(), statDictShuffled.GetHashCode());
    }

    [Test]
    public void IntPairTest()
    {
        PKMN.IntPair intPair = new PKMN.IntPair(123, 456);

        PKMN.IntPair intPairSame = new PKMN.IntPair(123, 456);
        PKMN.IntPair intPairDifferentFirst = new PKMN.IntPair(789, 456);
        PKMN.IntPair intPairDifferentSecond = new PKMN.IntPair(123, 789);
        PKMN.IntPair intPairReversed = new PKMN.IntPair(456, 123);

        Assert.AreEqual(intPair, intPair);
        Assert.AreEqual(intPair, intPairSame);
        Assert.AreEqual(intPair.GetHashCode(), intPairSame.GetHashCode());

        Assert.AreNotEqual(intPair, intPairDifferentFirst);
        Assert.AreNotEqual(intPair.GetHashCode(), intPairDifferentFirst.GetHashCode());

        Assert.AreNotEqual(intPair, intPairDifferentSecond);
        Assert.AreNotEqual(intPair.GetHashCode(), intPairDifferentSecond.GetHashCode());

        Assert.AreNotEqual(intPair, intPairReversed);
        Assert.AreNotEqual(intPair.GetHashCode(), intPairReversed.GetHashCode());
    }

    [Test]
    public void StringPairTest()
    {
        PKMN.StringPair stringPair = new PKMN.StringPair("abc", "def");

        PKMN.StringPair stringPairSame = new PKMN.StringPair("abc", "def");
        PKMN.StringPair stringPairDifferentFirst = new PKMN.StringPair("ghi", "def");
        PKMN.StringPair stringPairDifferentSecond = new PKMN.StringPair("abc", "ghi");
        PKMN.StringPair stringPairReversed = new PKMN.StringPair("def", "abc");

        Assert.AreEqual(stringPair, stringPair);
        Assert.AreEqual(stringPair, stringPairSame);
        Assert.AreEqual(stringPair.GetHashCode(), stringPairSame.GetHashCode());

        Assert.AreNotEqual(stringPair, stringPairDifferentFirst);
        Assert.AreNotEqual(stringPair.GetHashCode(), stringPairDifferentFirst.GetHashCode());

        Assert.AreNotEqual(stringPair, stringPairDifferentSecond);
        Assert.AreNotEqual(stringPair.GetHashCode(), stringPairDifferentSecond.GetHashCode());

        Assert.AreNotEqual(stringPair, stringPairReversed);
        Assert.AreNotEqual(stringPair.GetHashCode(), stringPairReversed.GetHashCode());
    }

    [Test]
    public void StringListTest()
    {
        PKMN.StringList stringList = new PKMN.StringList();
        stringList.Add("abc");
        stringList.Add("def");
        stringList.Add("ghi");
        stringList.Add("jkl");
        stringList.Add("mno");

        PKMN.StringList stringListSame = new PKMN.StringList();
        stringListSame.Add("abc");
        stringListSame.Add("def");
        stringListSame.Add("ghi");
        stringListSame.Add("jkl");
        stringListSame.Add("mno");

        PKMN.StringList stringListReversed = new PKMN.StringList();
        stringListReversed.Add("mno");
        stringListReversed.Add("jkl");
        stringListReversed.Add("ghi");
        stringListReversed.Add("def");
        stringListReversed.Add("abc");

        Assert.AreEqual(stringList, stringList);
        Assert.AreEqual(stringList, stringListSame);
        Assert.AreEqual(stringList.GetHashCode(), stringListSame.GetHashCode());

        Assert.AreNotEqual(stringList, stringListReversed);
        Assert.AreNotEqual(stringList.GetHashCode(), stringListReversed.GetHashCode());
    }

    [Test]
    public void MoveEntryListTest()
    {
        PKMN.Database.MoveEntryList moveEntryList = new PKMN.Database.MoveEntryList();
        moveEntryList.Add(
            new PKMN.Database.MoveEntry("Scratch", PKMN.Game.RED)
        );
        moveEntryList.Add(
            new PKMN.Database.MoveEntry("Synthesis", PKMN.Game.GOLD)
        );
        moveEntryList.Add(
            new PKMN.Database.MoveEntry("Swallow", PKMN.Game.RUBY)
        );

        PKMN.Database.MoveEntryList moveEntryListSame = new PKMN.Database.MoveEntryList();
        moveEntryListSame.Add(
            new PKMN.Database.MoveEntry("Scratch", PKMN.Game.RED)
        );
        moveEntryListSame.Add(
            new PKMN.Database.MoveEntry("Synthesis", PKMN.Game.GOLD)
        );
        moveEntryListSame.Add(
            new PKMN.Database.MoveEntry("Swallow", PKMN.Game.RUBY)
        );

        PKMN.Database.MoveEntryList moveEntryListReversed = new PKMN.Database.MoveEntryList();
        moveEntryListReversed.Add(
            new PKMN.Database.MoveEntry("Swallow", PKMN.Game.RUBY)
        );
        moveEntryListReversed.Add(
            new PKMN.Database.MoveEntry("Synthesis", PKMN.Game.GOLD)
        );
        moveEntryListReversed.Add(
            new PKMN.Database.MoveEntry("Scratch", PKMN.Game.RED)
        );

        Assert.AreEqual(moveEntryList, moveEntryList);
        Assert.AreEqual(moveEntryList, moveEntryListSame);
        Assert.AreEqual(moveEntryList.GetHashCode(), moveEntryListSame.GetHashCode());

        Assert.AreNotEqual(moveEntryList, moveEntryListReversed);
        Assert.AreNotEqual(moveEntryList.GetHashCode(), moveEntryListReversed.GetHashCode());
    }

    [Test]
    public void PokemonEntryListTest()
    {
        PKMN.Database.PokemonEntryList pokemonEntryList = new PKMN.Database.PokemonEntryList();
        pokemonEntryList.Add(
            new PKMN.Database.PokemonEntry("Squirtle", PKMN.Game.BLUE, "")
        );
        pokemonEntryList.Add(
            new PKMN.Database.PokemonEntry("Totodile", PKMN.Game.SILVER, "")
        );
        pokemonEntryList.Add(
            new PKMN.Database.PokemonEntry("Mudkip", PKMN.Game.SAPPHIRE, "")
        );

        PKMN.Database.PokemonEntryList pokemonEntryListSame = new PKMN.Database.PokemonEntryList();
        pokemonEntryListSame.Add(
            new PKMN.Database.PokemonEntry("Squirtle", PKMN.Game.BLUE, "")
        );
        pokemonEntryListSame.Add(
            new PKMN.Database.PokemonEntry("Totodile", PKMN.Game.SILVER, "")
        );
        pokemonEntryListSame.Add(
            new PKMN.Database.PokemonEntry("Mudkip", PKMN.Game.SAPPHIRE, "")
        );

        PKMN.Database.PokemonEntryList pokemonEntryListReversed = new PKMN.Database.PokemonEntryList();
        pokemonEntryListReversed.Add(
            new PKMN.Database.PokemonEntry("Mudkip", PKMN.Game.SAPPHIRE, "")
        );
        pokemonEntryListReversed.Add(
            new PKMN.Database.PokemonEntry("Totodile", PKMN.Game.SILVER, "")
        );
        pokemonEntryListReversed.Add(
            new PKMN.Database.PokemonEntry("Squirtle", PKMN.Game.BLUE, "")
        );

        Assert.AreEqual(pokemonEntryList, pokemonEntryList);
        Assert.AreEqual(pokemonEntryList, pokemonEntryListSame);
        Assert.AreEqual(pokemonEntryList.GetHashCode(), pokemonEntryListSame.GetHashCode());

        Assert.AreNotEqual(pokemonEntryList, pokemonEntryListReversed);
        Assert.AreNotEqual(pokemonEntryList.GetHashCode(), pokemonEntryListReversed.GetHashCode());
    }
}
