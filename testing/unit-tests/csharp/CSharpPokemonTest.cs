/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokemonTest
{
    [TestCase("Red")]
    [TestCase("Blue")]
    [TestCase("Yellow")]
    [TestCase("Gold")]
    [TestCase("Silver")]
    [TestCase("Crystal")]
    [TestCase("Ruby")]
    [TestCase("Sapphire")]
    [TestCase("Emerald")]
    [TestCase("FireRed")]
    [TestCase("LeafGreen")]
    [TestCase("Colosseum")]
    [TestCase("XD")]
    public void FormTest(
        string game
    )
    {
        PKMNTest.PokemonTestCommon.FormTest(game);
    }

    [TestCase("Gold")]
    [TestCase("Silver")]
    [TestCase("Crystal")]
    [TestCase("Ruby")]
    [TestCase("Sapphire")]
    [TestCase("Emerald")]
    [TestCase("FireRed")]
    [TestCase("LeafGreen")]
    [TestCase("Colosseum")]
    [TestCase("XD")]
    public void GenderTest(
        string game
    )
    {
        PKMNTest.PokemonTestCommon.GenderTest(game);
    }

    [TestCase("Gold")]
    [TestCase("Silver")]
    [TestCase("Crystal")]
    public void Gen2UnownTest(
        string game
    )
    {
        PKMNTest.UnownTests.Gen2UnownTest(game);
    }

    [TestCase("Ruby")]
    [TestCase("Sapphire")]
    [TestCase("Emerald")]
    [TestCase("FireRed")]
    [TestCase("LeafGreen")]
    [TestCase("Colosseum")]
    [TestCase("XD")]
    public void Gen3UnownTest(
        string game
    )
    {
        PKMNTest.UnownTests.Gen3UnownTest(game);
    }

    [TestCase("Red",    "Charmander")]
    [TestCase("Blue",   "Squirtle")]
    [TestCase("Yellow", "Bulbasaur")]
    public void Gen1PokemonTest(
        string game,
        string species
    )
    {
        PKMNTest.Gen1PokemonTests.PokemonTest(game, species);
    }

    [TestCase("Gold",    "Cyndaquil")]
    [TestCase("Silver",  "Totodile")]
    [TestCase("Crystal", "Chikorita")]
    public void Gen2PokemonTest(
        string game,
        string species
    )
    {
        PKMNTest.Gen2PokemonTests.PokemonTest(game, species);
    }

    [TestCase("Ruby", "Torchic")]
    [TestCase("Sapphire", "Mudkip")]
    [TestCase("Emerald", "Treecko")]
    [TestCase("FireRed", "Charmander")]
    [TestCase("LeafGreen", "Bulbasaur")]
    [TestCase("Colosseum", "Espeon")]
    [TestCase("XD", "Umbreon")]
    public void Gen3PokemonTest(
        string game,
        string species
    )
    {
        PKMNTest.Gen3PokemonTests.PokemonTest(game, species);
    }

    /*
     * I/O
     */

    [TestCase("Red")]
    [TestCase("Blue")]
    [TestCase("Yellow")]
    public void TestSavingAndLoadingPK1(
        string game
    )
    {
        PKMNTest.PokemonIOTests.TestLoadingAndSavingPK1(game);
    }

    [TestCase("Gold")]
    [TestCase("Silver")]
    [TestCase("Crystal")]
    public void TestSavingAndLoadingPK2(
        string game
    )
    {
        PKMNTest.PokemonIOTests.TestLoadingAndSavingPK2(game);
    }

    [TestCase("Ruby")]
    [TestCase("Sapphire")]
    [TestCase("Emerald")]
    [TestCase("FireRed")]
    [TestCase("LeafGreen")]
    public void TestSavingAndLoading3GPKM(
        string game
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
    [TestCase("Bulbasaur", "", "Red", "Yellow")]

    // Generation I -> Generation II
    [TestCase("Squirtle", "", "Blue", "Gold")]

    // Generation II -> Generation II
    [TestCase("Cyndaquil", "", "Gold", "Crystal")]
    [TestCase("Totodile", "", "Crystal", "Gold")]

    // Generation II -> Generation I
    [TestCase("Charmander", "", "Silver", "Blue")]

    // GBA -> GBA
    [TestCase("Torchic", "", "Ruby", "Sapphire")]
    [TestCase("Mudkip", "", "Ruby", "Emerald")]
    [TestCase("Treecko", "", "Ruby", "FireRed")]
    [TestCase("Torchic", "", "Emerald", "Sapphire")]
    [TestCase("Mudkip", "", "Emerald", "Emerald")]
    [TestCase("Treecko", "", "Emerald", "FireRed")]
    [TestCase("Charmander", "", "FireRed", "Sapphire")]
    [TestCase("Squirtle", "", "FireRed", "Emerald")]
    [TestCase("Bulbasaur", "", "FireRed", "FireRed")]

    // GBA -> GCN
    [TestCase("Eevee", "", "Ruby", "Colosseum")]
    [TestCase("Espeon", "", "Emerald", "Colosseum")]
    [TestCase("Umbreon", "", "FireRed", "Colosseum")]
    [TestCase("Eevee", "", "Ruby", "XD")]
    [TestCase("Espeon", "", "Emerald", "XD")]
    [TestCase("Umbreon", "", "FireRed", "XD")]

    // GCN -> GBA
    [TestCase("Eevee", "", "Colosseum", "Sapphire")]
    [TestCase("Espeon", "", "Colosseum", "Emerald")]
    [TestCase("Umbreon", "", "Colosseum", "LeafGreen")]
    [TestCase("Eevee", "", "XD", "Sapphire")]
    [TestCase("Espeon", "", "XD", "Emerald")]
    [TestCase("Umbreon", "", "XD", "LeafGreen")]

    // GCN -> GCN
    [TestCase("Vaporeon", "", "Colosseum", "Colosseum")]
    [TestCase("Jolteon", "", "Colosseum", "XD")]
    [TestCase("Vaporeon", "", "XD", "XD")]
    [TestCase("Jolteon", "", "XD", "Colosseum")]
    public void PokemonConversionsTest(
        string species,
        string form,
        string originGame,
        string destGame
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
