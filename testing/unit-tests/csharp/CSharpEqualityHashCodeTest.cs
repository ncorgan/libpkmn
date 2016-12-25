/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpEqualityHashCodeTest {
    /*
     * LibPKMN classes
     */
    [Test]
    public void HiddenPowerTest() {
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
    public void ItemEntryTest() {
        PKMN.ItemEntry itemEntry = new PKMN.ItemEntry("Potion", "Red");

        PKMN.ItemEntry itemEntrySame = new PKMN.ItemEntry("Potion", "Red");
        PKMN.ItemEntry itemEntryDifferentItem = new PKMN.ItemEntry("Great Ball", "Red");
        PKMN.ItemEntry itemEntryDifferentGame = new PKMN.ItemEntry("Potion", "Blue");

        Assert.AreEqual(itemEntry, itemEntry);
        Assert.AreEqual(itemEntry, itemEntrySame);
        Assert.AreEqual(itemEntry.GetHashCode(), itemEntrySame.GetHashCode());

        Assert.AreNotEqual(itemEntry, itemEntryDifferentItem);
        Assert.AreNotEqual(itemEntry.GetHashCode(), itemEntryDifferentItem.GetHashCode());

        Assert.AreNotEqual(itemEntry, itemEntryDifferentGame);
        Assert.AreNotEqual(itemEntry.GetHashCode(), itemEntryDifferentGame.GetHashCode());
    }

    [Test]
    public void ItemSlotTest() {
        PKMN.ItemSlot itemSlot = new PKMN.ItemSlot(
                                         new PKMN.ItemEntry("Potion", "Red"),
                                         10
                                     );

        PKMN.ItemSlot itemSlotSame = new PKMN.ItemSlot(
                                             new PKMN.ItemEntry("Potion", "Red"),
                                             10
                                         );
        PKMN.ItemSlot itemSlotDifferentItem = new PKMN.ItemSlot(
                                                      new PKMN.ItemEntry("Great Ball", "Red"),
                                                      10
                                                  );
        PKMN.ItemSlot itemSlotDifferentGame = new PKMN.ItemSlot(
                                                      new PKMN.ItemEntry("Potion", "Blue"),
                                                      10
                                                  );
        PKMN.ItemSlot itemSlotDifferentAmount = new PKMN.ItemSlot(
                                                        new PKMN.ItemEntry("Potion", "Red"),
                                                        5
                                                    );

        Assert.AreEqual(itemSlot, itemSlot);
        Assert.AreEqual(itemSlot, itemSlotSame);
        Assert.AreEqual(itemSlot.GetHashCode(), itemSlotSame.GetHashCode());

        Assert.AreNotEqual(itemSlot, itemSlotDifferentItem);
        Assert.AreNotEqual(itemSlot.GetHashCode(), itemSlotDifferentItem.GetHashCode());

        Assert.AreNotEqual(itemSlot, itemSlotDifferentGame);
        Assert.AreNotEqual(itemSlot.GetHashCode(), itemSlotDifferentGame.GetHashCode());

        Assert.AreNotEqual(itemSlot, itemSlotDifferentAmount);
        Assert.AreNotEqual(itemSlot.GetHashCode(), itemSlotDifferentAmount.GetHashCode());
    }

    [Test]
    public void LevelupMoveTest() {
        PKMN.LevelupMove levelupMove = new PKMN.LevelupMove(
                                               new PKMN.MoveEntry("Scratch", "Red"),
                                               10
                                           );

        PKMN.LevelupMove levelupMoveSame = new PKMN.LevelupMove(
                                                   new PKMN.MoveEntry("Scratch", "Red"),
                                                   10
                                               );
        PKMN.LevelupMove levelupMoveDifferentMove = new PKMN.LevelupMove(
                                                            new PKMN.MoveEntry("Tackle", "Red"),
                                                            10
                                                        );
        PKMN.LevelupMove levelupMoveDifferentGame = new PKMN.LevelupMove(
                                                            new PKMN.MoveEntry("Scratch", "Blue"),
                                                            10
                                                        );
        PKMN.LevelupMove levelupMoveDifferentLevel = new PKMN.LevelupMove(
                                                             new PKMN.MoveEntry("Scratch", "Red"),
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
    public void MoveEntryTest() {
        PKMN.MoveEntry moveEntry = new PKMN.MoveEntry("Scratch", "Red");

        PKMN.MoveEntry moveEntrySame = new PKMN.MoveEntry("Scratch", "Red");
        PKMN.MoveEntry moveEntryDifferentMove = new PKMN.MoveEntry("Tackle", "Red");
        PKMN.MoveEntry moveEntryDifferentGame = new PKMN.MoveEntry("Scratch", "Blue");

        Assert.AreEqual(moveEntry, moveEntry);
        Assert.AreEqual(moveEntry, moveEntrySame);
        Assert.AreEqual(moveEntry.GetHashCode(), moveEntrySame.GetHashCode());

        Assert.AreNotEqual(moveEntry, moveEntryDifferentMove);
        Assert.AreNotEqual(moveEntry.GetHashCode(), moveEntryDifferentMove.GetHashCode());

        Assert.AreNotEqual(moveEntry, moveEntryDifferentGame);
        Assert.AreNotEqual(moveEntry.GetHashCode(), moveEntryDifferentGame.GetHashCode());
    }

    [Test]
    public void PokemonEntryTest() {
        PKMN.PokemonEntry pokemonEntry = new PKMN.PokemonEntry("Venusaur", "Omega Ruby", "");

        PKMN.PokemonEntry pokemonEntrySame = new PKMN.PokemonEntry("Venusaur", "Omega Ruby", "");
        PKMN.PokemonEntry pokemonEntryDifferentPokemon = new PKMN.PokemonEntry("Charizard", "Omega Ruby", "");
        PKMN.PokemonEntry pokemonEntryDifferentGame = new PKMN.PokemonEntry("Venusaur", "Ruby", "");
        PKMN.PokemonEntry pokemonEntryDifferentForm = new PKMN.PokemonEntry("Venusaur", "Omega Ruby", "Mega");

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
    public void SpindaCoordsTest() {
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
    public void SpindaSpotsTest() {
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

    /*
     * STL containers
     */
    [Test]
    public void StringBoolDictTest() {
        PKMN.StringBoolDict stringBoolDict = new PKMN.StringBoolDict();
        stringBoolDict["key1"] = true;
        stringBoolDict["key2"] = false;
        stringBoolDict["key3"] = true;
        stringBoolDict["key4"] = false;
        stringBoolDict["key5"] = true;

        PKMN.StringBoolDict stringBoolDictSame = new PKMN.StringBoolDict();
        stringBoolDictSame["key5"] = true;
        stringBoolDictSame["key4"] = false;
        stringBoolDictSame["key3"] = true;
        stringBoolDictSame["key2"] = false;
        stringBoolDictSame["key1"] = true;

        PKMN.StringBoolDict stringBoolDictReversed = new PKMN.StringBoolDict();
        stringBoolDictReversed["key1"] = false;
        stringBoolDictReversed["key2"] = true;
        stringBoolDictReversed["key3"] = false;
        stringBoolDictReversed["key4"] = true;
        stringBoolDictReversed["key5"] = false;

        PKMN.StringBoolDict stringBoolDictShuffled = new PKMN.StringBoolDict();
        stringBoolDictShuffled["key1"] = false;
        stringBoolDictShuffled["key2"] = true;
        stringBoolDictShuffled["key3"] = false;
        stringBoolDictShuffled["key4"] = true;
        stringBoolDictShuffled["key5"] = true;

        Assert.AreEqual(stringBoolDict, stringBoolDict);
        Assert.AreEqual(stringBoolDict, stringBoolDictSame);
        Assert.AreEqual(stringBoolDict.GetHashCode(), stringBoolDictSame.GetHashCode());

        Assert.AreNotEqual(stringBoolDict, stringBoolDictReversed);
        Assert.AreNotEqual(stringBoolDict.GetHashCode(), stringBoolDictReversed.GetHashCode());

        Assert.AreNotEqual(stringBoolDict, stringBoolDictShuffled);
        Assert.AreNotEqual(stringBoolDict.GetHashCode(), stringBoolDictShuffled.GetHashCode());
    }

    [Test]
    public void StringStringDictTest() {
        PKMN.StringStringDict stringStringDict = new PKMN.StringStringDict();
        stringStringDict["key1"] = "abc";
        stringStringDict["key2"] = "def";
        stringStringDict["key3"] = "ghi";
        stringStringDict["key4"] = "jkl";
        stringStringDict["key5"] = "mno";

        PKMN.StringStringDict stringStringDictSame = new PKMN.StringStringDict();
        stringStringDictSame["key5"] = "mno";
        stringStringDictSame["key4"] = "jkl";
        stringStringDictSame["key3"] = "ghi";
        stringStringDictSame["key2"] = "def";
        stringStringDictSame["key1"] = "abc";

        PKMN.StringStringDict stringStringDictShuffled = new PKMN.StringStringDict();
        stringStringDictShuffled["key1"] = "jkl";
        stringStringDictShuffled["key2"] = "abc";
        stringStringDictShuffled["key3"] = "mno";
        stringStringDictShuffled["key4"] = "def";
        stringStringDictShuffled["key5"] = "ghi";

        Assert.AreEqual(stringStringDict, stringStringDict);
        Assert.AreEqual(stringStringDict, stringStringDictSame);
        Assert.AreEqual(stringStringDict.GetHashCode(), stringStringDictSame.GetHashCode());

        Assert.AreNotEqual(stringStringDict, stringStringDictShuffled);
        Assert.AreNotEqual(stringStringDict.GetHashCode(), stringStringDictShuffled.GetHashCode());
    }

    [Test]
    public void StringIntDictTest() {
        PKMN.StringIntDict stringIntDict = new PKMN.StringIntDict();
        stringIntDict["key1"] = 12;
        stringIntDict["key2"] = 34;
        stringIntDict["key3"] = 56;
        stringIntDict["key4"] = 78;
        stringIntDict["key5"] = 90;

        PKMN.StringIntDict stringIntDictSame = new PKMN.StringIntDict();
        stringIntDictSame["key5"] = 90;
        stringIntDictSame["key4"] = 78;
        stringIntDictSame["key3"] = 56;
        stringIntDictSame["key2"] = 34;
        stringIntDictSame["key1"] = 12;

        PKMN.StringIntDict stringIntDictShuffled = new PKMN.StringIntDict();
        stringIntDictShuffled["key1"] = 78;
        stringIntDictShuffled["key2"] = 12;
        stringIntDictShuffled["key3"] = 90;
        stringIntDictShuffled["key4"] = 34;
        stringIntDictShuffled["key5"] = 56;

        Assert.AreEqual(stringIntDict, stringIntDict);
        Assert.AreEqual(stringIntDict, stringIntDictSame);
        Assert.AreEqual(stringIntDict.GetHashCode(), stringIntDictSame.GetHashCode());

        Assert.AreNotEqual(stringIntDict, stringIntDictShuffled);
        Assert.AreNotEqual(stringIntDict.GetHashCode(), stringIntDictShuffled.GetHashCode());
    }

    [Test]
    public void IntPairTest() {
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
    public void StringPairTest() {
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
    public void StringListTest() {
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
    public void ItemPocketsTest() {
        // Test values
        PKMN.ItemList itemList1 = new PKMN.ItemList("Items", "Red");
        PKMN.ItemList itemList2 = new PKMN.ItemList("Balls", "Gold");
        PKMN.ItemList itemList3 = new PKMN.ItemList("Berries", "Ruby");

        // Test values with same data, different underlying memory
        PKMN.ItemList itemList4 = new PKMN.ItemList("Items", "Red");
        PKMN.ItemList itemList5 = new PKMN.ItemList("Balls", "Gold");
        PKMN.ItemList itemList6 = new PKMN.ItemList("Berries", "Ruby");

        PKMN.ItemPockets itemPockets = new PKMN.ItemPockets();
        itemPockets["key1"] = itemList1;
        itemPockets["key2"] = itemList2;
        itemPockets["key3"] = itemList3;

        PKMN.ItemPockets itemPocketsSame = new PKMN.ItemPockets();
        itemPocketsSame["key3"] = itemList3;
        itemPocketsSame["key2"] = itemList2;
        itemPocketsSame["key1"] = itemList1;

        PKMN.ItemPockets itemPocketsDifferentMemory = new PKMN.ItemPockets();
        itemPocketsDifferentMemory["key1"] = itemList4;
        itemPocketsDifferentMemory["key2"] = itemList5;
        itemPocketsDifferentMemory["key3"] = itemList6;

        Assert.AreEqual(itemPockets, itemPockets);
        Assert.AreEqual(itemPockets, itemPocketsSame);
        Assert.AreEqual(itemPockets.GetHashCode(), itemPocketsSame.GetHashCode());

        Assert.AreNotEqual(itemPockets, itemPocketsDifferentMemory);
        Assert.AreNotEqual(itemPockets.GetHashCode(), itemPocketsDifferentMemory.GetHashCode());
    }

    [Test]
    public void ItemSlotListTest() {
        PKMN.ItemSlotList itemSlotList = new PKMN.ItemSlotList();
        itemSlotList.Add(
            new PKMN.ItemSlot(
                    new PKMN.ItemEntry("Potion", "Red"),
                    10
                )
        );
        itemSlotList.Add(
            new PKMN.ItemSlot(
                    new PKMN.ItemEntry("Great Ball", "Gold"),
                    3
                )
        );
        itemSlotList.Add(
            new PKMN.ItemSlot(
                    new PKMN.ItemEntry("Razz Berry", "Ruby"),
                    5
                )
        );

        PKMN.ItemSlotList itemSlotListSame = new PKMN.ItemSlotList();
        itemSlotListSame.Add(
            new PKMN.ItemSlot(
                    new PKMN.ItemEntry("Potion", "Red"),
                    10
                )
        );
        itemSlotListSame.Add(
            new PKMN.ItemSlot(
                    new PKMN.ItemEntry("Great Ball", "Gold"),
                    3
                )
        );
        itemSlotListSame.Add(
            new PKMN.ItemSlot(
                    new PKMN.ItemEntry("Razz Berry", "Ruby"),
                    5
                )
        );

        PKMN.ItemSlotList itemSlotListReversed = new PKMN.ItemSlotList();
        itemSlotListReversed.Add(
            new PKMN.ItemSlot(
                    new PKMN.ItemEntry("Razz Berry", "Ruby"),
                    5
                )
        );
        itemSlotListReversed.Add(
            new PKMN.ItemSlot(
                    new PKMN.ItemEntry("Great Ball", "Gold"),
                    3
                )
        );
        itemSlotListReversed.Add(
            new PKMN.ItemSlot(
                    new PKMN.ItemEntry("Potion", "Red"),
                    10
                )
        );

        Assert.AreEqual(itemSlotList, itemSlotList);
        Assert.AreEqual(itemSlotList, itemSlotListSame);
        Assert.AreEqual(itemSlotList.GetHashCode(), itemSlotListSame.GetHashCode());

        Assert.AreNotEqual(itemSlotList, itemSlotListReversed);
        Assert.AreNotEqual(itemSlotList.GetHashCode(), itemSlotListReversed.GetHashCode());
    }

    [Test]
    public void MoveEntryListTest() {
        PKMN.MoveEntryList moveEntryList = new PKMN.MoveEntryList();
        moveEntryList.Add(
            new PKMN.MoveEntry("Scratch", "Red")
        );
        moveEntryList.Add(
            new PKMN.MoveEntry("Synthesis", "Gold")
        );
        moveEntryList.Add(
            new PKMN.MoveEntry("Swallow", "Ruby")
        );

        PKMN.MoveEntryList moveEntryListSame = new PKMN.MoveEntryList();
        moveEntryListSame.Add(
            new PKMN.MoveEntry("Scratch", "Red")
        );
        moveEntryListSame.Add(
            new PKMN.MoveEntry("Synthesis", "Gold")
        );
        moveEntryListSame.Add(
            new PKMN.MoveEntry("Swallow", "Ruby")
        );

        PKMN.MoveEntryList moveEntryListReversed = new PKMN.MoveEntryList();
        moveEntryListReversed.Add(
            new PKMN.MoveEntry("Swallow", "Ruby")
        );
        moveEntryListReversed.Add(
            new PKMN.MoveEntry("Synthesis", "Gold")
        );
        moveEntryListReversed.Add(
            new PKMN.MoveEntry("Scratch", "Red")
        );

        Assert.AreEqual(moveEntryList, moveEntryList);
        Assert.AreEqual(moveEntryList, moveEntryListSame);
        Assert.AreEqual(moveEntryList.GetHashCode(), moveEntryListSame.GetHashCode());

        Assert.AreNotEqual(moveEntryList, moveEntryListReversed);
        Assert.AreNotEqual(moveEntryList.GetHashCode(), moveEntryListReversed.GetHashCode());
    }

    [Test]
    public void PokemonEntryListTest() {
        PKMN.PokemonEntryList pokemonEntryList = new PKMN.PokemonEntryList();
        pokemonEntryList.Add(
            new PKMN.PokemonEntry("Squirtle", "Blue", "")
        );
        pokemonEntryList.Add(
            new PKMN.PokemonEntry("Totodile", "Silver", "")
        );
        pokemonEntryList.Add(
            new PKMN.PokemonEntry("Mudkip", "Sapphire", "")
        );

        PKMN.PokemonEntryList pokemonEntryListSame = new PKMN.PokemonEntryList();
        pokemonEntryListSame.Add(
            new PKMN.PokemonEntry("Squirtle", "Blue", "")
        );
        pokemonEntryListSame.Add(
            new PKMN.PokemonEntry("Totodile", "Silver", "")
        );
        pokemonEntryListSame.Add(
            new PKMN.PokemonEntry("Mudkip", "Sapphire", "")
        );

        PKMN.PokemonEntryList pokemonEntryListReversed = new PKMN.PokemonEntryList();
        pokemonEntryListReversed.Add(
            new PKMN.PokemonEntry("Mudkip", "Sapphire", "")
        );
        pokemonEntryListReversed.Add(
            new PKMN.PokemonEntry("Totodile", "Silver", "")
        );
        pokemonEntryListReversed.Add(
            new PKMN.PokemonEntry("Squirtle", "Blue", "")
        );

        Assert.AreEqual(pokemonEntryList, pokemonEntryList);
        Assert.AreEqual(pokemonEntryList, pokemonEntryListSame);
        Assert.AreEqual(pokemonEntryList.GetHashCode(), pokemonEntryListSame.GetHashCode());

        Assert.AreNotEqual(pokemonEntryList, pokemonEntryListReversed);
        Assert.AreNotEqual(pokemonEntryList.GetHashCode(), pokemonEntryListReversed.GetHashCode());
    }
}
