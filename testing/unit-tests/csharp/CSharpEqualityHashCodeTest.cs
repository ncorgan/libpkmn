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
        PKMN.HiddenPower hiddenPower = new PKMN.HiddenPower(
                                               PKMN.Type.NORMAL,
                                               30
                                           );

        PKMN.HiddenPower hiddenPowerSame = new PKMN.HiddenPower(
                                                   PKMN.Type.NORMAL,
                                                   30
                                               );
        PKMN.HiddenPower hiddenPowerDifferentType = new PKMN.HiddenPower(
                                                            PKMN.Type.FIRE,
                                                            30
                                                        );
        PKMN.HiddenPower hiddenPowerDifferentBasePower = new PKMN.HiddenPower(
                                                                 PKMN.Type.NORMAL,
                                                                 40
                                                             );

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
        PKMN.Database.ItemEntry itemEntry = new PKMN.Database.ItemEntry(PKMN.Item.POTION, PKMN.Game.RED);

        PKMN.Database.ItemEntry itemEntrySame = new PKMN.Database.ItemEntry(PKMN.Item.POTION, PKMN.Game.RED);
        PKMN.Database.ItemEntry itemEntryDifferentItem = new PKMN.Database.ItemEntry(PKMN.Item.GREAT_BALL, PKMN.Game.RED);
        PKMN.Database.ItemEntry itemEntryDifferentGame = new PKMN.Database.ItemEntry(PKMN.Item.POTION, PKMN.Game.BLUE);

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
                                                        new PKMN.Database.MoveEntry(
                                                                PKMN.Move.SCRATCH,
                                                                PKMN.Game.RED
                                                            ),
                                                        10
                                                    );

        PKMN.Database.LevelupMove levelupMoveSame = new PKMN.Database.LevelupMove(
                                                            new PKMN.Database.MoveEntry(
                                                                    PKMN.Move.SCRATCH,
                                                                    PKMN.Game.RED
                                                                ),
                                                            10
                                                        );
        PKMN.Database.LevelupMove levelupMoveDifferentMove = new PKMN.Database.LevelupMove(
                                                                     new PKMN.Database.MoveEntry(
                                                                             PKMN.Move.TACKLE,
                                                                             PKMN.Game.RED
                                                                         ),
                                                                     10
                                                                 );
        PKMN.Database.LevelupMove levelupMoveDifferentGame = new PKMN.Database.LevelupMove(
                                                                     new PKMN.Database.MoveEntry(
                                                                             PKMN.Move.SCRATCH,
                                                                             PKMN.Game.BLUE
                                                                         ),
                                                                     10
                                                                 );
        PKMN.Database.LevelupMove levelupMoveDifferentLevel = new PKMN.Database.LevelupMove(
                                                                      new PKMN.Database.MoveEntry(
                                                                              PKMN.Move.SCRATCH,
                                                                              PKMN.Game.RED
                                                                          ),
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
        PKMN.Database.MoveEntry moveEntry = new PKMN.Database.MoveEntry(
                                                    PKMN.Move.SCRATCH,
                                                    PKMN.Game.RED
                                                );

        PKMN.Database.MoveEntry moveEntrySame = new PKMN.Database.MoveEntry(
                                                        PKMN.Move.SCRATCH,
                                                        PKMN.Game.RED
                                                    );
        PKMN.Database.MoveEntry moveEntryDifferentMove = new PKMN.Database.MoveEntry(
                                                                 PKMN.Move.TACKLE,
                                                                 PKMN.Game.RED
                                                             );
        PKMN.Database.MoveEntry moveEntryDifferentGame = new PKMN.Database.MoveEntry(
                                                                 PKMN.Move.SCRATCH,
                                                                 PKMN.Game.BLUE
                                                             );

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
        PKMN.NaturalGift naturalGift = new PKMN.NaturalGift(
                                               PKMN.Type.NORMAL,
                                               30
                                           );

        PKMN.NaturalGift naturalGiftSame = new PKMN.NaturalGift(
                                                   PKMN.Type.NORMAL,
                                                   30
                                               );
        PKMN.NaturalGift naturalGiftDifferentType = new PKMN.NaturalGift(
                                                            PKMN.Type.FIRE,
                                                            30
                                                        );
        PKMN.NaturalGift naturalGiftDifferentBasePower = new PKMN.NaturalGift(
                                                                 PKMN.Type.NORMAL,
                                                                 40
                                                             );

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
        PKMN.Database.PokemonEntry pokemonEntry = new PKMN.Database.PokemonEntry(
                                                          PKMN.Species.VENUSAUR,
                                                          PKMN.Game.OMEGA_RUBY,
                                                          ""
                                                      );

        PKMN.Database.PokemonEntry pokemonEntrySame = new PKMN.Database.PokemonEntry(
                                                              PKMN.Species.VENUSAUR,
                                                              PKMN.Game.OMEGA_RUBY,
                                                              ""
                                                          );
        PKMN.Database.PokemonEntry pokemonEntryDifferentPokemon = new PKMN.Database.PokemonEntry(
                                                                          PKMN.Species.CHARIZARD,
                                                                          PKMN.Game.OMEGA_RUBY,
                                                                          ""
                                                                      );
        PKMN.Database.PokemonEntry pokemonEntryDifferentGame = new PKMN.Database.PokemonEntry(
                                                                       PKMN.Species.VENUSAUR,
                                                                       PKMN.Game.RUBY,
                                                                       ""
                                                                   );
        PKMN.Database.PokemonEntry pokemonEntryDifferentForm = new PKMN.Database.PokemonEntry(
                                                                       PKMN.Species.VENUSAUR,
                                                                       PKMN.Game.OMEGA_RUBY,
                                                                       "Mega"
                                                                   );

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
        PKMN.Pokemon pokemon = new PKMN.Pokemon(PKMN.Species.MEW, PKMN.Game.RUBY, "", 70);

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
            new PKMN.Database.MoveEntry(PKMN.Move.SCRATCH, PKMN.Game.RED)
        );
        moveEntryList.Add(
            new PKMN.Database.MoveEntry(PKMN.Move.SYNTHESIS, PKMN.Game.GOLD)
        );
        moveEntryList.Add(
            new PKMN.Database.MoveEntry(PKMN.Move.SWALLOW, PKMN.Game.RUBY)
        );

        PKMN.Database.MoveEntryList moveEntryListSame = new PKMN.Database.MoveEntryList();
        moveEntryListSame.Add(
            new PKMN.Database.MoveEntry(PKMN.Move.SCRATCH, PKMN.Game.RED)
        );
        moveEntryListSame.Add(
            new PKMN.Database.MoveEntry(PKMN.Move.SYNTHESIS, PKMN.Game.GOLD)
        );
        moveEntryListSame.Add(
            new PKMN.Database.MoveEntry(PKMN.Move.SWALLOW, PKMN.Game.RUBY)
        );

        PKMN.Database.MoveEntryList moveEntryListReversed = new PKMN.Database.MoveEntryList();
        moveEntryListReversed.Add(
            new PKMN.Database.MoveEntry(PKMN.Move.SWALLOW, PKMN.Game.RUBY)
        );
        moveEntryListReversed.Add(
            new PKMN.Database.MoveEntry(PKMN.Move.SYNTHESIS, PKMN.Game.GOLD)
        );
        moveEntryListReversed.Add(
            new PKMN.Database.MoveEntry(PKMN.Move.SCRATCH, PKMN.Game.RED)
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
            new PKMN.Database.PokemonEntry(PKMN.Species.SQUIRTLE, PKMN.Game.BLUE, "")
        );
        pokemonEntryList.Add(
            new PKMN.Database.PokemonEntry(PKMN.Species.TOTODILE, PKMN.Game.SILVER, "")
        );
        pokemonEntryList.Add(
            new PKMN.Database.PokemonEntry(PKMN.Species.MUDKIP, PKMN.Game.SAPPHIRE, "")
        );

        PKMN.Database.PokemonEntryList pokemonEntryListSame = new PKMN.Database.PokemonEntryList();
        pokemonEntryListSame.Add(
            new PKMN.Database.PokemonEntry(PKMN.Species.SQUIRTLE, PKMN.Game.BLUE, "")
        );
        pokemonEntryListSame.Add(
            new PKMN.Database.PokemonEntry(PKMN.Species.TOTODILE, PKMN.Game.SILVER, "")
        );
        pokemonEntryListSame.Add(
            new PKMN.Database.PokemonEntry(PKMN.Species.MUDKIP, PKMN.Game.SAPPHIRE, "")
        );

        PKMN.Database.PokemonEntryList pokemonEntryListReversed = new PKMN.Database.PokemonEntryList();
        pokemonEntryListReversed.Add(
            new PKMN.Database.PokemonEntry(PKMN.Species.MUDKIP, PKMN.Game.SAPPHIRE, "")
        );
        pokemonEntryListReversed.Add(
            new PKMN.Database.PokemonEntry(PKMN.Species.TOTODILE, PKMN.Game.SILVER, "")
        );
        pokemonEntryListReversed.Add(
            new PKMN.Database.PokemonEntry(PKMN.Species.SQUIRTLE, PKMN.Game.BLUE, "")
        );

        Assert.AreEqual(pokemonEntryList, pokemonEntryList);
        Assert.AreEqual(pokemonEntryList, pokemonEntryListSame);
        Assert.AreEqual(pokemonEntryList.GetHashCode(), pokemonEntryListSame.GetHashCode());

        Assert.AreNotEqual(pokemonEntryList, pokemonEntryListReversed);
        Assert.AreNotEqual(pokemonEntryList.GetHashCode(), pokemonEntryListReversed.GetHashCode());
    }

    [Test]
    public void AbilityEnumListTest()
    {
        PKMN.AbilityEnumList abilityEnumList = new PKMN.AbilityEnumList();
        abilityEnumList.Add(PKMN.Ability.STENCH);
        abilityEnumList.Add(PKMN.Ability.TANGLED_FEET);
        abilityEnumList.Add(PKMN.Ability.SAP_SIPPER);
        abilityEnumList.Add(PKMN.Ability.BULLETPROOF);
        abilityEnumList.Add(PKMN.Ability.DELTA_STREAM);

        PKMN.AbilityEnumList abilityEnumListSame = new PKMN.AbilityEnumList();
        abilityEnumListSame.Add(PKMN.Ability.STENCH);
        abilityEnumListSame.Add(PKMN.Ability.TANGLED_FEET);
        abilityEnumListSame.Add(PKMN.Ability.SAP_SIPPER);
        abilityEnumListSame.Add(PKMN.Ability.BULLETPROOF);
        abilityEnumListSame.Add(PKMN.Ability.DELTA_STREAM);

        PKMN.AbilityEnumList abilityEnumListReversed = new PKMN.AbilityEnumList();
        abilityEnumListReversed.Add(PKMN.Ability.DELTA_STREAM);
        abilityEnumListReversed.Add(PKMN.Ability.BULLETPROOF);
        abilityEnumListReversed.Add(PKMN.Ability.SAP_SIPPER);
        abilityEnumListReversed.Add(PKMN.Ability.TANGLED_FEET);
        abilityEnumListReversed.Add(PKMN.Ability.STENCH);

        Assert.AreEqual(abilityEnumList, abilityEnumList);
        Assert.AreEqual(abilityEnumList, abilityEnumListSame);
        Assert.AreEqual(abilityEnumList.GetHashCode(), abilityEnumListSame.GetHashCode());

        Assert.AreNotEqual(abilityEnumList, abilityEnumListReversed);
        Assert.AreNotEqual(abilityEnumList.GetHashCode(), abilityEnumListReversed.GetHashCode());
    }

    [Test]
    public void BallEnumListTest()
    {
        PKMN.BallEnumList ballEnumList = new PKMN.BallEnumList();
        ballEnumList.Add(PKMN.Ball.POKE_BALL);
        ballEnumList.Add(PKMN.Ball.NET_BALL);
        ballEnumList.Add(PKMN.Ball.LUXURY_BALL);
        ballEnumList.Add(PKMN.Ball.HEAVY_BALL);
        ballEnumList.Add(PKMN.Ball.DREAM_BALL);

        PKMN.BallEnumList ballEnumListSame = new PKMN.BallEnumList();
        ballEnumListSame.Add(PKMN.Ball.POKE_BALL);
        ballEnumListSame.Add(PKMN.Ball.NET_BALL);
        ballEnumListSame.Add(PKMN.Ball.LUXURY_BALL);
        ballEnumListSame.Add(PKMN.Ball.HEAVY_BALL);
        ballEnumListSame.Add(PKMN.Ball.DREAM_BALL);

        PKMN.BallEnumList ballEnumListReversed = new PKMN.BallEnumList();
        ballEnumListReversed.Add(PKMN.Ball.DREAM_BALL);
        ballEnumListReversed.Add(PKMN.Ball.HEAVY_BALL);
        ballEnumListReversed.Add(PKMN.Ball.LUXURY_BALL);
        ballEnumListReversed.Add(PKMN.Ball.NET_BALL);
        ballEnumListReversed.Add(PKMN.Ball.POKE_BALL);

        Assert.AreEqual(ballEnumList, ballEnumList);
        Assert.AreEqual(ballEnumList, ballEnumListSame);
        Assert.AreEqual(ballEnumList.GetHashCode(), ballEnumListSame.GetHashCode());

        Assert.AreNotEqual(ballEnumList, ballEnumListReversed);
        Assert.AreNotEqual(ballEnumList.GetHashCode(), ballEnumListReversed.GetHashCode());
    }

    [Test]
    public void ConditionEnumListTest()
    {
        PKMN.ConditionEnumList conditionEnumList = new PKMN.ConditionEnumList();
        conditionEnumList.Add(PKMN.Condition.NONE);
        conditionEnumList.Add(PKMN.Condition.ASLEEP);
        conditionEnumList.Add(PKMN.Condition.POISON);
        conditionEnumList.Add(PKMN.Condition.BURN);
        conditionEnumList.Add(PKMN.Condition.FROZEN);

        PKMN.ConditionEnumList conditionEnumListSame = new PKMN.ConditionEnumList();
        conditionEnumListSame.Add(PKMN.Condition.NONE);
        conditionEnumListSame.Add(PKMN.Condition.ASLEEP);
        conditionEnumListSame.Add(PKMN.Condition.POISON);
        conditionEnumListSame.Add(PKMN.Condition.BURN);
        conditionEnumListSame.Add(PKMN.Condition.FROZEN);

        PKMN.ConditionEnumList conditionEnumListReversed = new PKMN.ConditionEnumList();
        conditionEnumListReversed.Add(PKMN.Condition.FROZEN);
        conditionEnumListReversed.Add(PKMN.Condition.BURN);
        conditionEnumListReversed.Add(PKMN.Condition.POISON);
        conditionEnumListReversed.Add(PKMN.Condition.ASLEEP);
        conditionEnumListReversed.Add(PKMN.Condition.NONE);

        Assert.AreEqual(conditionEnumList, conditionEnumList);
        Assert.AreEqual(conditionEnumList, conditionEnumListSame);
        Assert.AreEqual(conditionEnumList.GetHashCode(), conditionEnumListSame.GetHashCode());

        Assert.AreNotEqual(conditionEnumList, conditionEnumListReversed);
        Assert.AreNotEqual(conditionEnumList.GetHashCode(), conditionEnumListReversed.GetHashCode());
    }

    [Test]
    public void EggGroupEnumListTest()
    {
        PKMN.EggGroupEnumList eggGroupEnumList = new PKMN.EggGroupEnumList();
        eggGroupEnumList.Add(PKMN.EggGroup.NONE);
        eggGroupEnumList.Add(PKMN.EggGroup.MONSTER);
        eggGroupEnumList.Add(PKMN.EggGroup.BUG);
        eggGroupEnumList.Add(PKMN.EggGroup.GROUND);
        eggGroupEnumList.Add(PKMN.EggGroup.PLANT);

        PKMN.EggGroupEnumList eggGroupEnumListSame = new PKMN.EggGroupEnumList();
        eggGroupEnumListSame.Add(PKMN.EggGroup.NONE);
        eggGroupEnumListSame.Add(PKMN.EggGroup.MONSTER);
        eggGroupEnumListSame.Add(PKMN.EggGroup.BUG);
        eggGroupEnumListSame.Add(PKMN.EggGroup.GROUND);
        eggGroupEnumListSame.Add(PKMN.EggGroup.PLANT);

        PKMN.EggGroupEnumList eggGroupEnumListReversed = new PKMN.EggGroupEnumList();
        eggGroupEnumListReversed.Add(PKMN.EggGroup.PLANT);
        eggGroupEnumListReversed.Add(PKMN.EggGroup.GROUND);
        eggGroupEnumListReversed.Add(PKMN.EggGroup.BUG);
        eggGroupEnumListReversed.Add(PKMN.EggGroup.MONSTER);
        eggGroupEnumListReversed.Add(PKMN.EggGroup.NONE);

        Assert.AreEqual(eggGroupEnumList, eggGroupEnumList);
        Assert.AreEqual(eggGroupEnumList, eggGroupEnumListSame);
        Assert.AreEqual(eggGroupEnumList.GetHashCode(), eggGroupEnumListSame.GetHashCode());

        Assert.AreNotEqual(eggGroupEnumList, eggGroupEnumListReversed);
        Assert.AreNotEqual(eggGroupEnumList.GetHashCode(), eggGroupEnumListReversed.GetHashCode());
    }

    [Test]
    public void GameEnumListTest()
    {
        PKMN.GameEnumList gameEnumList = new PKMN.GameEnumList();
        gameEnumList.Add(PKMN.Game.RED);
        gameEnumList.Add(PKMN.Game.GOLD);
        gameEnumList.Add(PKMN.Game.RUBY);
        gameEnumList.Add(PKMN.Game.DIAMOND);
        gameEnumList.Add(PKMN.Game.BLACK);

        PKMN.GameEnumList gameEnumListSame = new PKMN.GameEnumList();
        gameEnumListSame.Add(PKMN.Game.RED);
        gameEnumListSame.Add(PKMN.Game.GOLD);
        gameEnumListSame.Add(PKMN.Game.RUBY);
        gameEnumListSame.Add(PKMN.Game.DIAMOND);
        gameEnumListSame.Add(PKMN.Game.BLACK);

        PKMN.GameEnumList gameEnumListReversed = new PKMN.GameEnumList();
        gameEnumListReversed.Add(PKMN.Game.BLACK);
        gameEnumListReversed.Add(PKMN.Game.DIAMOND);
        gameEnumListReversed.Add(PKMN.Game.RUBY);
        gameEnumListReversed.Add(PKMN.Game.GOLD);
        gameEnumListReversed.Add(PKMN.Game.RED);

        Assert.AreEqual(gameEnumList, gameEnumList);
        Assert.AreEqual(gameEnumList, gameEnumListSame);
        Assert.AreEqual(gameEnumList.GetHashCode(), gameEnumListSame.GetHashCode());

        Assert.AreNotEqual(gameEnumList, gameEnumListReversed);
        Assert.AreNotEqual(gameEnumList.GetHashCode(), gameEnumListReversed.GetHashCode());
    }

    [Test]
    public void GenderEnumListTest()
    {
        PKMN.GenderEnumList genderEnumList = new PKMN.GenderEnumList();
        genderEnumList.Add(PKMN.Gender.MALE);
        genderEnumList.Add(PKMN.Gender.FEMALE);
        genderEnumList.Add(PKMN.Gender.GENDERLESS);

        PKMN.GenderEnumList genderEnumListSender = new PKMN.GenderEnumList();
        genderEnumListSender.Add(PKMN.Gender.MALE);
        genderEnumListSender.Add(PKMN.Gender.FEMALE);
        genderEnumListSender.Add(PKMN.Gender.GENDERLESS);

        PKMN.GenderEnumList genderEnumListReversed = new PKMN.GenderEnumList();
        genderEnumListReversed.Add(PKMN.Gender.GENDERLESS);
        genderEnumListReversed.Add(PKMN.Gender.FEMALE);
        genderEnumListReversed.Add(PKMN.Gender.MALE);

        Assert.AreEqual(genderEnumList, genderEnumList);
        Assert.AreEqual(genderEnumList, genderEnumListSender);
        Assert.AreEqual(genderEnumList.GetHashCode(), genderEnumListSender.GetHashCode());

        Assert.AreNotEqual(genderEnumList, genderEnumListReversed);
        Assert.AreNotEqual(genderEnumList.GetHashCode(), genderEnumListReversed.GetHashCode());
    }

    [Test]
    public void ItemEnumListTest()
    {
        PKMN.ItemEnumList itemEnumList = new PKMN.ItemEnumList();
        itemEnumList.Add(PKMN.Item.POKE_BALL);
        itemEnumList.Add(PKMN.Item.NET_BALL);
        itemEnumList.Add(PKMN.Item.LUXURY_BALL);
        itemEnumList.Add(PKMN.Item.HEAVY_BALL);
        itemEnumList.Add(PKMN.Item.DREAM_BALL);

        PKMN.ItemEnumList itemEnumListSame = new PKMN.ItemEnumList();
        itemEnumListSame.Add(PKMN.Item.POKE_BALL);
        itemEnumListSame.Add(PKMN.Item.NET_BALL);
        itemEnumListSame.Add(PKMN.Item.LUXURY_BALL);
        itemEnumListSame.Add(PKMN.Item.HEAVY_BALL);
        itemEnumListSame.Add(PKMN.Item.DREAM_BALL);

        PKMN.ItemEnumList itemEnumListReversed = new PKMN.ItemEnumList();
        itemEnumListReversed.Add(PKMN.Item.DREAM_BALL);
        itemEnumListReversed.Add(PKMN.Item.HEAVY_BALL);
        itemEnumListReversed.Add(PKMN.Item.LUXURY_BALL);
        itemEnumListReversed.Add(PKMN.Item.NET_BALL);
        itemEnumListReversed.Add(PKMN.Item.POKE_BALL);

        Assert.AreEqual(itemEnumList, itemEnumList);
        Assert.AreEqual(itemEnumList, itemEnumListSame);
        Assert.AreEqual(itemEnumList.GetHashCode(), itemEnumListSame.GetHashCode());

        Assert.AreNotEqual(itemEnumList, itemEnumListReversed);
        Assert.AreNotEqual(itemEnumList.GetHashCode(), itemEnumListReversed.GetHashCode());
    }

    [Test]
    public void LanguageEnumListTest()
    {
        PKMN.LanguageEnumList languageEnumList = new PKMN.LanguageEnumList();
        languageEnumList.Add(PKMN.Language.JAPANESE);
        languageEnumList.Add(PKMN.Language.ENGLISH);
        languageEnumList.Add(PKMN.Language.GERMAN);
        languageEnumList.Add(PKMN.Language.FRENCH);
        languageEnumList.Add(PKMN.Language.ITALIAN);

        PKMN.LanguageEnumList languageEnumListSame = new PKMN.LanguageEnumList();
        languageEnumListSame.Add(PKMN.Language.JAPANESE);
        languageEnumListSame.Add(PKMN.Language.ENGLISH);
        languageEnumListSame.Add(PKMN.Language.GERMAN);
        languageEnumListSame.Add(PKMN.Language.FRENCH);
        languageEnumListSame.Add(PKMN.Language.ITALIAN);

        PKMN.LanguageEnumList languageEnumListReversed = new PKMN.LanguageEnumList();
        languageEnumListReversed.Add(PKMN.Language.ITALIAN);
        languageEnumListReversed.Add(PKMN.Language.FRENCH);
        languageEnumListReversed.Add(PKMN.Language.GERMAN);
        languageEnumListReversed.Add(PKMN.Language.ENGLISH);
        languageEnumListReversed.Add(PKMN.Language.JAPANESE);

        Assert.AreEqual(languageEnumList, languageEnumList);
        Assert.AreEqual(languageEnumList, languageEnumListSame);
        Assert.AreEqual(languageEnumList.GetHashCode(), languageEnumListSame.GetHashCode());

        Assert.AreNotEqual(languageEnumList, languageEnumListReversed);
        Assert.AreNotEqual(languageEnumList.GetHashCode(), languageEnumListReversed.GetHashCode());
    }

    [Test]
    public void MoveDamageClassEnumListTest()
    {
        PKMN.MoveDamageClassEnumList moveDamageClassEnumList = new PKMN.MoveDamageClassEnumList();
        moveDamageClassEnumList.Add(PKMN.MoveDamageClass.PHYSICAL);
        moveDamageClassEnumList.Add(PKMN.MoveDamageClass.SPECIAL);
        moveDamageClassEnumList.Add(PKMN.MoveDamageClass.STATUS);

        PKMN.MoveDamageClassEnumList moveDamageClassEnumListSender = new PKMN.MoveDamageClassEnumList();
        moveDamageClassEnumListSender.Add(PKMN.MoveDamageClass.PHYSICAL);
        moveDamageClassEnumListSender.Add(PKMN.MoveDamageClass.SPECIAL);
        moveDamageClassEnumListSender.Add(PKMN.MoveDamageClass.STATUS);

        PKMN.MoveDamageClassEnumList moveDamageClassEnumListReversed = new PKMN.MoveDamageClassEnumList();
        moveDamageClassEnumListReversed.Add(PKMN.MoveDamageClass.STATUS);
        moveDamageClassEnumListReversed.Add(PKMN.MoveDamageClass.SPECIAL);
        moveDamageClassEnumListReversed.Add(PKMN.MoveDamageClass.PHYSICAL);

        Assert.AreEqual(moveDamageClassEnumList, moveDamageClassEnumList);
        Assert.AreEqual(moveDamageClassEnumList, moveDamageClassEnumListSender);
        Assert.AreEqual(moveDamageClassEnumList.GetHashCode(), moveDamageClassEnumListSender.GetHashCode());

        Assert.AreNotEqual(moveDamageClassEnumList, moveDamageClassEnumListReversed);
        Assert.AreNotEqual(moveDamageClassEnumList.GetHashCode(), moveDamageClassEnumListReversed.GetHashCode());
    }

    [Test]
    public void MoveEnumListTest()
    {
        PKMN.MoveEnumList moveEnumList = new PKMN.MoveEnumList();
        moveEnumList.Add(PKMN.Move.CUT);
        moveEnumList.Add(PKMN.Move.FLY);
        moveEnumList.Add(PKMN.Move.SURF);
        moveEnumList.Add(PKMN.Move.STRENGTH);
        moveEnumList.Add(PKMN.Move.FLASH);

        PKMN.MoveEnumList moveEnumListSame = new PKMN.MoveEnumList();
        moveEnumListSame.Add(PKMN.Move.CUT);
        moveEnumListSame.Add(PKMN.Move.FLY);
        moveEnumListSame.Add(PKMN.Move.SURF);
        moveEnumListSame.Add(PKMN.Move.STRENGTH);
        moveEnumListSame.Add(PKMN.Move.FLASH);

        PKMN.MoveEnumList moveEnumListReversed = new PKMN.MoveEnumList();
        moveEnumListReversed.Add(PKMN.Move.FLASH);
        moveEnumListReversed.Add(PKMN.Move.STRENGTH);
        moveEnumListReversed.Add(PKMN.Move.SURF);
        moveEnumListReversed.Add(PKMN.Move.FLY);
        moveEnumListReversed.Add(PKMN.Move.CUT);

        Assert.AreEqual(moveEnumList, moveEnumList);
        Assert.AreEqual(moveEnumList, moveEnumListSame);
        Assert.AreEqual(moveEnumList.GetHashCode(), moveEnumListSame.GetHashCode());

        Assert.AreNotEqual(moveEnumList, moveEnumListReversed);
        Assert.AreNotEqual(moveEnumList.GetHashCode(), moveEnumListReversed.GetHashCode());
    }

    [Test]
    public void NatureEnumListTest()
    {
        PKMN.NatureEnumList natureEnumList = new PKMN.NatureEnumList();
        natureEnumList.Add(PKMN.Nature.HARDY);
        natureEnumList.Add(PKMN.Nature.QUIRKY);
        natureEnumList.Add(PKMN.Nature.LONELY);
        natureEnumList.Add(PKMN.Nature.BASHFUL);
        natureEnumList.Add(PKMN.Nature.QUIET);

        PKMN.NatureEnumList natureEnumListSame = new PKMN.NatureEnumList();
        natureEnumListSame.Add(PKMN.Nature.HARDY);
        natureEnumListSame.Add(PKMN.Nature.QUIRKY);
        natureEnumListSame.Add(PKMN.Nature.LONELY);
        natureEnumListSame.Add(PKMN.Nature.BASHFUL);
        natureEnumListSame.Add(PKMN.Nature.QUIET);

        PKMN.NatureEnumList natureEnumListReversed = new PKMN.NatureEnumList();
        natureEnumListReversed.Add(PKMN.Nature.QUIET);
        natureEnumListReversed.Add(PKMN.Nature.BASHFUL);
        natureEnumListReversed.Add(PKMN.Nature.LONELY);
        natureEnumListReversed.Add(PKMN.Nature.QUIRKY);
        natureEnumListReversed.Add(PKMN.Nature.HARDY);

        Assert.AreEqual(natureEnumList, natureEnumList);
        Assert.AreEqual(natureEnumList, natureEnumListSame);
        Assert.AreEqual(natureEnumList.GetHashCode(), natureEnumListSame.GetHashCode());

        Assert.AreNotEqual(natureEnumList, natureEnumListReversed);
        Assert.AreNotEqual(natureEnumList.GetHashCode(), natureEnumListReversed.GetHashCode());
    }

    [Test]
    public void SpeciesEnumListTest()
    {
        PKMN.SpeciesEnumList speciesEnumList = new PKMN.SpeciesEnumList();
        speciesEnumList.Add(PKMN.Species.SQUIRTLE);
        speciesEnumList.Add(PKMN.Species.TOTODILE);
        speciesEnumList.Add(PKMN.Species.MUDKIP);
        speciesEnumList.Add(PKMN.Species.PIPLUP);
        speciesEnumList.Add(PKMN.Species.OSHAWOTT);

        PKMN.SpeciesEnumList speciesEnumListSame = new PKMN.SpeciesEnumList();
        speciesEnumListSame.Add(PKMN.Species.SQUIRTLE);
        speciesEnumListSame.Add(PKMN.Species.TOTODILE);
        speciesEnumListSame.Add(PKMN.Species.MUDKIP);
        speciesEnumListSame.Add(PKMN.Species.PIPLUP);
        speciesEnumListSame.Add(PKMN.Species.OSHAWOTT);

        PKMN.SpeciesEnumList speciesEnumListReversed = new PKMN.SpeciesEnumList();
        speciesEnumListReversed.Add(PKMN.Species.OSHAWOTT);
        speciesEnumListReversed.Add(PKMN.Species.PIPLUP);
        speciesEnumListReversed.Add(PKMN.Species.MUDKIP);
        speciesEnumListReversed.Add(PKMN.Species.TOTODILE);
        speciesEnumListReversed.Add(PKMN.Species.SQUIRTLE);

        Assert.AreEqual(speciesEnumList, speciesEnumList);
        Assert.AreEqual(speciesEnumList, speciesEnumListSame);
        Assert.AreEqual(speciesEnumList.GetHashCode(), speciesEnumListSame.GetHashCode());

        Assert.AreNotEqual(speciesEnumList, speciesEnumListReversed);
        Assert.AreNotEqual(speciesEnumList.GetHashCode(), speciesEnumListReversed.GetHashCode());
    }

    [Test]
    public void StatEnumListTest()
    {
        PKMN.StatEnumList statEnumList = new PKMN.StatEnumList();
        statEnumList.Add(PKMN.Stat.HP);
        statEnumList.Add(PKMN.Stat.ATTACK);
        statEnumList.Add(PKMN.Stat.DEFENSE);
        statEnumList.Add(PKMN.Stat.SPEED);
        statEnumList.Add(PKMN.Stat.SPECIAL);

        PKMN.StatEnumList statEnumListSame = new PKMN.StatEnumList();
        statEnumListSame.Add(PKMN.Stat.HP);
        statEnumListSame.Add(PKMN.Stat.ATTACK);
        statEnumListSame.Add(PKMN.Stat.DEFENSE);
        statEnumListSame.Add(PKMN.Stat.SPEED);
        statEnumListSame.Add(PKMN.Stat.SPECIAL);

        PKMN.StatEnumList statEnumListReversed = new PKMN.StatEnumList();
        statEnumListReversed.Add(PKMN.Stat.SPECIAL);
        statEnumListReversed.Add(PKMN.Stat.SPEED);
        statEnumListReversed.Add(PKMN.Stat.DEFENSE);
        statEnumListReversed.Add(PKMN.Stat.ATTACK);
        statEnumListReversed.Add(PKMN.Stat.HP);

        Assert.AreEqual(statEnumList, statEnumList);
        Assert.AreEqual(statEnumList, statEnumListSame);
        Assert.AreEqual(statEnumList.GetHashCode(), statEnumListSame.GetHashCode());

        Assert.AreNotEqual(statEnumList, statEnumListReversed);
        Assert.AreNotEqual(statEnumList.GetHashCode(), statEnumListReversed.GetHashCode());
    }

    [Test]
    public void TypeEnumListTest()
    {
        PKMN.TypeEnumList typeEnumList = new PKMN.TypeEnumList();
        typeEnumList.Add(PKMN.Type.NORMAL);
        typeEnumList.Add(PKMN.Type.SHADOW);
        typeEnumList.Add(PKMN.Type.FAIRY);
        typeEnumList.Add(PKMN.Type.QUESTION_MARK);
        typeEnumList.Add(PKMN.Type.STEEL);

        PKMN.TypeEnumList typeEnumListSame = new PKMN.TypeEnumList();
        typeEnumListSame.Add(PKMN.Type.NORMAL);
        typeEnumListSame.Add(PKMN.Type.SHADOW);
        typeEnumListSame.Add(PKMN.Type.FAIRY);
        typeEnumListSame.Add(PKMN.Type.QUESTION_MARK);
        typeEnumListSame.Add(PKMN.Type.STEEL);

        PKMN.TypeEnumList typeEnumListReversed = new PKMN.TypeEnumList();
        typeEnumListReversed.Add(PKMN.Type.STEEL);
        typeEnumListReversed.Add(PKMN.Type.QUESTION_MARK);
        typeEnumListReversed.Add(PKMN.Type.FAIRY);
        typeEnumListReversed.Add(PKMN.Type.SHADOW);
        typeEnumListReversed.Add(PKMN.Type.NORMAL);

        Assert.AreEqual(typeEnumList, typeEnumList);
        Assert.AreEqual(typeEnumList, typeEnumListSame);
        Assert.AreEqual(typeEnumList.GetHashCode(), typeEnumListSame.GetHashCode());

        Assert.AreNotEqual(typeEnumList, typeEnumListReversed);
        Assert.AreNotEqual(typeEnumList.GetHashCode(), typeEnumListReversed.GetHashCode());
    }
}
