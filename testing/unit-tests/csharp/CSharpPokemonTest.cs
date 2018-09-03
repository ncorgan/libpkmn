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

    [TestCase(PKMN.Game.RED,    PKMN.Species.CHARMANDER)]
    [TestCase(PKMN.Game.BLUE,   PKMN.Species.SQUIRTLE)]
    [TestCase(PKMN.Game.YELLOW, PKMN.Species.BULBASAUR)]
    public void Gen1PokemonTest(
        PKMN.Game game,
        PKMN.Species species
    )
    {
        PKMNTest.Gen1PokemonTests.PokemonTest(game, species);
    }

    [TestCase(PKMN.Game.GOLD,    PKMN.Species.CYNDAQUIL)]
    [TestCase(PKMN.Game.SILVER,  PKMN.Species.TOTODILE)]
    [TestCase(PKMN.Game.CRYSTAL, PKMN.Species.CHIKORITA)]
    public void Gen2PokemonTest(
        PKMN.Game game,
        PKMN.Species species
    )
    {
        PKMNTest.Gen2PokemonTests.PokemonTest(game, species);
    }

    [TestCase(PKMN.Game.RUBY, PKMN.Species.TORCHIC)]
    [TestCase(PKMN.Game.SAPPHIRE, PKMN.Species.MUDKIP)]
    [TestCase(PKMN.Game.EMERALD, PKMN.Species.TREECKO)]
    [TestCase(PKMN.Game.FIRERED, PKMN.Species.CHARMANDER)]
    [TestCase(PKMN.Game.LEAFGREEN, PKMN.Species.BULBASAUR)]
    [TestCase(PKMN.Game.COLOSSEUM, PKMN.Species.ESPEON)]
    [TestCase(PKMN.Game.XD, PKMN.Species.UMBREON)]
    public void Gen3PokemonTest(
        PKMN.Game game,
        PKMN.Species species
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

    [TestCase(PKMN.Game.RUBY)]
    [TestCase(PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Game.EMERALD)]
    [TestCase(PKMN.Game.FIRERED)]
    [TestCase(PKMN.Game.LEAFGREEN)]
    public void TestSavingAndLoadingPK3(
        PKMN.Game game
    )
    {
        PKMNTest.PokemonIOTests.TestLoadingAndSavingPK3(game);
    }

    [Test]
    public void TestSavingAndLoadingCK3()
    {
        PKMNTest.PokemonIOTests.TestLoadingAndSavingCK3(
            PKMN.Game.COLOSSEUM
        );
    }

    [Test]
    public void TestSavingAndLoadingXK3()
    {
        PKMNTest.PokemonIOTests.TestLoadingAndSavingXK3(
            PKMN.Game.XD
        );
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
    [TestCase(PKMN.Species.BULBASAUR, "", PKMN.Game.RED, PKMN.Game.YELLOW)]

    // Generation I -> Generation II
    [TestCase(PKMN.Species.SQUIRTLE, "", PKMN.Game.BLUE, PKMN.Game.GOLD)]

    // Generation II -> Generation II
    [TestCase(PKMN.Species.CYNDAQUIL, "", PKMN.Game.GOLD, PKMN.Game.CRYSTAL)]
    [TestCase(PKMN.Species.TOTODILE, "", PKMN.Game.CRYSTAL, PKMN.Game.GOLD)]

    // Generation II -> Generation I
    [TestCase(PKMN.Species.CHARMANDER, "", PKMN.Game.SILVER, PKMN.Game.BLUE)]

    // GBA -> GBA
    [TestCase(PKMN.Species.TORCHIC, "", PKMN.Game.RUBY, PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Species.MUDKIP, "", PKMN.Game.RUBY, PKMN.Game.EMERALD)]
    [TestCase(PKMN.Species.TREECKO, "", PKMN.Game.RUBY, PKMN.Game.FIRERED)]
    [TestCase(PKMN.Species.TORCHIC, "", PKMN.Game.EMERALD, PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Species.MUDKIP, "", PKMN.Game.EMERALD, PKMN.Game.EMERALD)]
    [TestCase(PKMN.Species.TREECKO, "", PKMN.Game.EMERALD, PKMN.Game.FIRERED)]
    [TestCase(PKMN.Species.CHARMANDER, "", PKMN.Game.FIRERED, PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Species.SQUIRTLE, "", PKMN.Game.FIRERED, PKMN.Game.EMERALD)]
    [TestCase(PKMN.Species.BULBASAUR, "", PKMN.Game.FIRERED, PKMN.Game.FIRERED)]

    // GBA -> GCN
    [TestCase(PKMN.Species.EEVEE, "", PKMN.Game.RUBY, PKMN.Game.COLOSSEUM)]
    [TestCase(PKMN.Species.ESPEON, "", PKMN.Game.EMERALD, PKMN.Game.COLOSSEUM)]
    [TestCase(PKMN.Species.UMBREON, "", PKMN.Game.FIRERED, PKMN.Game.COLOSSEUM)]
    [TestCase(PKMN.Species.EEVEE, "", PKMN.Game.RUBY, PKMN.Game.XD)]
    [TestCase(PKMN.Species.ESPEON, "", PKMN.Game.EMERALD, PKMN.Game.XD)]
    [TestCase(PKMN.Species.UMBREON, "", PKMN.Game.FIRERED, PKMN.Game.XD)]

    // GCN -> GBA
    [TestCase(PKMN.Species.EEVEE, "", PKMN.Game.COLOSSEUM, PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Species.ESPEON, "", PKMN.Game.COLOSSEUM, PKMN.Game.EMERALD)]
    [TestCase(PKMN.Species.UMBREON, "", PKMN.Game.COLOSSEUM, PKMN.Game.LEAFGREEN)]
    [TestCase(PKMN.Species.EEVEE, "", PKMN.Game.XD, PKMN.Game.SAPPHIRE)]
    [TestCase(PKMN.Species.ESPEON, "", PKMN.Game.XD, PKMN.Game.EMERALD)]
    [TestCase(PKMN.Species.UMBREON, "", PKMN.Game.XD, PKMN.Game.LEAFGREEN)]

    // GCN -> GCN
    [TestCase(PKMN.Species.VAPOREON, "", PKMN.Game.COLOSSEUM, PKMN.Game.COLOSSEUM)]
    [TestCase(PKMN.Species.JOLTEON, "", PKMN.Game.COLOSSEUM, PKMN.Game.XD)]
    [TestCase(PKMN.Species.VAPOREON, "", PKMN.Game.XD, PKMN.Game.XD)]
    [TestCase(PKMN.Species.JOLTEON, "", PKMN.Game.XD, PKMN.Game.COLOSSEUM)]
    public void PokemonConversionsTest(
        PKMN.Species species,
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
