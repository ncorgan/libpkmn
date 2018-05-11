/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokemonPCTest
{
    [TestCase("Red", new string[]{"Blue", "Yellow", "Gold", "Silver", "Crystal"}, "Ruby")]
    [TestCase("Blue", new string[]{"Red", "Yellow", "Gold", "Silver", "Crystal"}, "Sapphire")]
    [TestCase("Yellow", new string[]{"Red", "Blue", "Gold", "Silver", "Crystal"}, "Emerald")]
    [TestCase("Gold", new string[]{"Red", "Blue", "Yellow", "Silver", "Crystal"}, "FireRed")]
    [TestCase("Silver", new string[]{"Red", "Blue", "Yellow", "Gold", "Crystal"}, "LeafGreen")]
    [TestCase("Crystal", new string[]{"Red", "Blue", "Yellow", "Gold", "Silver"}, "Colosseum")]
    [TestCase("Ruby", new string[]{"Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum", "XD"}, "Red")]
    [TestCase("Sapphire", new string[]{"Ruby", "Emerald", "FireRed", "LeafGreen", "Colosseum", "XD"}, "Blue")]
    [TestCase("Emerald", new string[]{"Ruby", "Sapphire", "FireRed", "LeafGreen", "Colosseum", "XD"}, "Yellow")]
    [TestCase("FireRed", new string[]{"Ruby", "Sapphire", "Emerald", "LeafGreen", "Colosseum", "XD"}, "Gold")]
    [TestCase("LeafGreen", new string[]{"Ruby", "Sapphire", "Emerald", "FireRed", "Colosseum", "XD"}, "Silver")]
    [TestCase("Colosseum", new string[]{"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "XD"}, "Crystal")]
    [TestCase("XD", new string[]{"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum"}, "Red")]
    public void PokemonBoxTest(
        string game,
        string[] validOtherGames,
        string invalidOtherGame
    )
    {
        PKMNTest.PokemonPCTests.TestPokemonBox(
            new PKMN.PokemonBox(game),
            validOtherGames,
            invalidOtherGame
        );
    }

    [TestCase("Red", new string[]{"Blue", "Yellow", "Gold", "Silver", "Crystal"}, "Ruby")]
    [TestCase("Blue", new string[]{"Red", "Yellow", "Gold", "Silver", "Crystal"}, "Sapphire")]
    [TestCase("Yellow", new string[]{"Red", "Blue", "Gold", "Silver", "Crystal"}, "Emerald")]
    [TestCase("Gold", new string[]{"Red", "Blue", "Yellow", "Silver", "Crystal"}, "FireRed")]
    [TestCase("Silver", new string[]{"Red", "Blue", "Yellow", "Gold", "Crystal"}, "LeafGreen")]
    [TestCase("Crystal", new string[]{"Red", "Blue", "Yellow", "Gold", "Silver"}, "Colosseum")]
    [TestCase("Ruby", new string[]{"Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum", "XD"}, "Red")]
    [TestCase("Sapphire", new string[]{"Ruby", "Emerald", "FireRed", "LeafGreen", "Colosseum", "XD"}, "Blue")]
    [TestCase("Emerald", new string[]{"Ruby", "Sapphire", "FireRed", "LeafGreen", "Colosseum", "XD"}, "Yellow")]
    [TestCase("FireRed", new string[]{"Ruby", "Sapphire", "Emerald", "LeafGreen", "Colosseum", "XD"}, "Gold")]
    [TestCase("LeafGreen", new string[]{"Ruby", "Sapphire", "Emerald", "FireRed", "Colosseum", "XD"}, "Silver")]
    [TestCase("Colosseum", new string[]{"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "XD"}, "Crystal")]
    [TestCase("XD", new string[]{"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum"}, "Red")]
    public void PokemonPCTest(
        string game,
        string[] validOtherGames,
        string invalidOtherGame
    )
    {
        PKMNTest.PokemonPCTests.TestPokemonPC(
            new PKMN.PokemonPC(game),
            validOtherGames,
            invalidOtherGame
        );
    }
}
