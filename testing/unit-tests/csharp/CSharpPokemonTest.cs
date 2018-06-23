/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokemonTest
{
    [TestCase(PKMN.Game.RED)]
    [TestCase(PKMN.Game.BLUE)]
    [TestCase(PKMN.Game.YELLOW)]
    [TestCase(PKMN.Game.GOLD)]
    [TestCase(PKMN.Game.SILVER)]
    [TestCase(PKMN.Game.CRYSTAL)]
    [TestCase(PKMN.Game.RUBY)]
    [TestCase(PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Game.EMERALD)]
    [TestCase(PKMN.Game.FIRERED)]
    [TestCase(PKMN.Game.LEAFGREEN)]
    [TestCase(PKMN.Game.COLOSSEUM)]
    [TestCase(PKMN.Game.XD)]
    public void FormTest(
        PKMN.Game game
    )
    {
        PKMNTest.PokemonTestCommon.FormTest(game);
    }

    [TestCase(PKMN.Game.GOLD)]
    [TestCase(PKMN.Game.SILVER)]
    [TestCase(PKMN.Game.CRYSTAL)]
    [TestCase(PKMN.Game.RUBY)]
    [TestCase(PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Game.EMERALD)]
    [TestCase(PKMN.Game.FIRERED)]
    [TestCase(PKMN.Game.LEAFGREEN)]
    [TestCase(PKMN.Game.COLOSSEUM)]
    [TestCase(PKMN.Game.XD)]
    public void GenderTest(
        PKMN.Game game
    )
    {
        PKMNTest.PokemonTestCommon.GenderTest(game);
    }

    [TestCase(PKMN.Game.GOLD)]
    [TestCase(PKMN.Game.SILVER)]
    [TestCase(PKMN.Game.CRYSTAL)]
    public void Gen2UnownTest(
        PKMN.Game game
    )
    {
        PKMNTest.UnownTests.Gen2UnownTest(game);
    }

    [TestCase(PKMN.Game.RUBY)]
    [TestCase(PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Game.EMERALD)]
    [TestCase(PKMN.Game.FIRERED)]
    [TestCase(PKMN.Game.LEAFGREEN)]
    [TestCase(PKMN.Game.COLOSSEUM)]
    [TestCase(PKMN.Game.XD)]
    public void Gen3UnownTest(
        PKMN.Game game
    )
    {
        PKMNTest.UnownTests.Gen3UnownTest(game);
    }

    [TestCase(PKMN.Game.RED,    "Charmander")]
    [TestCase(PKMN.Game.BLUE,   "Squirtle")]
    [TestCase(PKMN.Game.YELLOW, "Bulbasaur")]
    public void Gen1PokemonTest(
        PKMN.Game game,
        string species
    )
    {
        PKMNTest.Gen1PokemonTests.PokemonTest(game, species);
    }

    [TestCase(PKMN.Game.GOLD,    "Cyndaquil")]
    [TestCase(PKMN.Game.SILVER,  "Totodile")]
    [TestCase(PKMN.Game.CRYSTAL, "Chikorita")]
    public void Gen2PokemonTest(
        PKMN.Game game,
        string species
    )
    {
        PKMNTest.Gen2PokemonTests.PokemonTest(game, species);
    }

    [TestCase(PKMN.Game.RUBY, "Torchic")]
    [TestCase(PKMN.Game.SAPPHIRE, "Mudkip")]
    [TestCase(PKMN.Game.EMERALD, "Treecko")]
    [TestCase(PKMN.Game.FIRERED, "Charmander")]
    [TestCase(PKMN.Game.LEAFGREEN, "Bulbasaur")]
    [TestCase(PKMN.Game.COLOSSEUM, "Espeon")]
    [TestCase(PKMN.Game.XD, "Umbreon")]
    public void Gen3PokemonTest(
        PKMN.Game game,
        string species
    )
    {
        PKMNTest.Gen3PokemonTests.PokemonTest(game, species);
    }

    /*
     * I/O
     */

    [TestCase(PKMN.Game.RED)]
    [TestCase(PKMN.Game.BLUE)]
    [TestCase(PKMN.Game.YELLOW)]
    public void TestSavingAndLoadingPK1(
        PKMN.Game game
    )
    {
        PKMNTest.PokemonIOTests.TestLoadingAndSavingPK1(game);
    }

    [TestCase(PKMN.Game.GOLD)]
    [TestCase(PKMN.Game.SILVER)]
    [TestCase(PKMN.Game.CRYSTAL)]
    public void TestSavingAndLoadingPK2(
        PKMN.Game game
    )
    {
        PKMNTest.PokemonIOTests.TestLoadingAndSavingPK2(game);
    }

    [TestCase(PKMN.Game.RUBY)]
    [TestCase(PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Game.EMERALD)]
    [TestCase(PKMN.Game.FIRERED)]
    [TestCase(PKMN.Game.LEAFGREEN)]
    public void TestSavingAndLoading3GPKM(
        PKMN.Game game
    )
    {
        PKMNTest.PokemonIOTests.TestLoadingAndSaving3GPKM(game);
    }

    [Test]
    public void TestOutside3GPKM()
    {
        PKMNTest.PokemonIOTests.TestOutside3GPKM();
    }

    /*
     * Conversions
     */

    // Generation I -> Generation I
    [TestCase("Bulbasaur", "", PKMN.Game.RED, PKMN.Game.YELLOW)]

    // Generation I -> Generation II
    [TestCase("Squirtle", "", PKMN.Game.BLUE, PKMN.Game.GOLD)]

    // Generation II -> Generation II
    [TestCase("Cyndaquil", "", PKMN.Game.GOLD, PKMN.Game.CRYSTAL)]
    [TestCase("Totodile", "", PKMN.Game.CRYSTAL, PKMN.Game.GOLD)]

    // Generation II -> Generation I
    [TestCase("Charmander", "", PKMN.Game.SILVER, PKMN.Game.BLUE)]

    // GBA -> GBA
    [TestCase("Torchic", "", PKMN.Game.RUBY, PKMN.Game.SAPPHIRE)]
    [TestCase("Mudkip", "", PKMN.Game.RUBY, PKMN.Game.EMERALD)]
    [TestCase("Treecko", "", PKMN.Game.RUBY, PKMN.Game.FIRERED)]
    [TestCase("Torchic", "", PKMN.Game.EMERALD, PKMN.Game.SAPPHIRE)]
    [TestCase("Mudkip", "", PKMN.Game.EMERALD, PKMN.Game.EMERALD)]
    [TestCase("Treecko", "", PKMN.Game.EMERALD, PKMN.Game.FIRERED)]
    [TestCase("Charmander", "", PKMN.Game.FIRERED, PKMN.Game.SAPPHIRE)]
    [TestCase("Squirtle", "", PKMN.Game.FIRERED, PKMN.Game.EMERALD)]
    [TestCase("Bulbasaur", "", PKMN.Game.FIRERED, PKMN.Game.FIRERED)]

    // GBA -> GCN
    [TestCase("Eevee", "", PKMN.Game.RUBY, PKMN.Game.COLOSSEUM)]
    [TestCase("Espeon", "", PKMN.Game.EMERALD, PKMN.Game.COLOSSEUM)]
    [TestCase("Umbreon", "", PKMN.Game.FIRERED, PKMN.Game.COLOSSEUM)]
    [TestCase("Eevee", "", PKMN.Game.RUBY, PKMN.Game.XD)]
    [TestCase("Espeon", "", PKMN.Game.EMERALD, PKMN.Game.XD)]
    [TestCase("Umbreon", "", PKMN.Game.FIRERED, PKMN.Game.XD)]

    // GCN -> GBA
    [TestCase("Eevee", "", PKMN.Game.COLOSSEUM, PKMN.Game.SAPPHIRE)]
    [TestCase("Espeon", "", PKMN.Game.COLOSSEUM, PKMN.Game.EMERALD)]
    [TestCase("Umbreon", "", PKMN.Game.COLOSSEUM, PKMN.Game.LEAFGREEN)]
    [TestCase("Eevee", "", PKMN.Game.XD, PKMN.Game.SAPPHIRE)]
    [TestCase("Espeon", "", PKMN.Game.XD, PKMN.Game.EMERALD)]
    [TestCase("Umbreon", "", PKMN.Game.XD, PKMN.Game.LEAFGREEN)]

    // GCN -> GCN
    [TestCase("Vaporeon", "", PKMN.Game.COLOSSEUM, PKMN.Game.COLOSSEUM)]
    [TestCase("Jolteon", "", PKMN.Game.COLOSSEUM, PKMN.Game.XD)]
    [TestCase("Vaporeon", "", PKMN.Game.XD, PKMN.Game.XD)]
    [TestCase("Jolteon", "", PKMN.Game.XD, PKMN.Game.COLOSSEUM)]
    public void PokemonConversionsTest(
        string species,
        string form,
        PKMN.Game originGame,
        PKMN.Game destGame
    )
    {
        PKMNTest.PokemonConversionsTest.ConversionsTest(
            species,
            form,
            originGame,
            destGame
        );
    }
}
