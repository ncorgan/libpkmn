/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokemonPCTest
{
    [TestCase(
        PKMN.Game.RED,
        new PKMN.Game[]{
            PKMN.Game.BLUE, PKMN.Game.YELLOW,
            PKMN.Game.GOLD, PKMN.Game.SILVER, PKMN.Game.CRYSTAL
        },
        PKMN.Game.RUBY
    )]
    [TestCase(
        PKMN.Game.BLUE,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.YELLOW,
            PKMN.Game.GOLD, PKMN.Game.SILVER, PKMN.Game.CRYSTAL
        },
        PKMN.Game.SAPPHIRE
    )]
    [TestCase(
        PKMN.Game.YELLOW,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.BLUE,
            PKMN.Game.GOLD, PKMN.Game.SILVER, PKMN.Game.CRYSTAL
        },
        PKMN.Game.EMERALD
    )]
    [TestCase(
        PKMN.Game.GOLD,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.BLUE, PKMN.Game.YELLOW,
            PKMN.Game.SILVER, PKMN.Game.CRYSTAL
        },
        PKMN.Game.FIRERED
    )]
    [TestCase(
        PKMN.Game.SILVER,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.BLUE, PKMN.Game.YELLOW,
            PKMN.Game.GOLD, PKMN.Game.CRYSTAL
        },
        PKMN.Game.LEAFGREEN
    )]
    [TestCase(
        PKMN.Game.CRYSTAL,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.BLUE, PKMN.Game.YELLOW,
            PKMN.Game.GOLD, PKMN.Game.SILVER
        },
        PKMN.Game.COLOSSEUM
    )]
    [TestCase(
        PKMN.Game.RUBY,
        new PKMN.Game[]{
            PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.RED
    )]
    [TestCase(
        PKMN.Game.SAPPHIRE,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.EMERALD, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.BLUE
    )]
    [TestCase(
        PKMN.Game.EMERALD,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.YELLOW
    )]
    [TestCase(
        PKMN.Game.FIRERED,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.GOLD
    )]
    [TestCase(
        PKMN.Game.LEAFGREEN,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.FIRERED,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.SILVER
    )]
    [TestCase(
        PKMN.Game.COLOSSEUM,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.XD
        },
        PKMN.Game.CRYSTAL
    )]
    [TestCase(
        PKMN.Game.XD,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM
        },
        PKMN.Game.RED
    )]
    public void PokemonBoxTest(
        PKMN.Game game,
        PKMN.Game[] validOtherGames,
        PKMN.Game invalidOtherGame
    )
    {
        PKMNTest.PokemonPCTests.TestPokemonBox(
            new PKMN.PokemonBox(game),
            validOtherGames,
            invalidOtherGame
        );
    }

    [TestCase(
        PKMN.Game.RED,
        new PKMN.Game[]{
            PKMN.Game.BLUE, PKMN.Game.YELLOW,
            PKMN.Game.GOLD, PKMN.Game.SILVER, PKMN.Game.CRYSTAL
        },
        PKMN.Game.RUBY
    )]
    [TestCase(
        PKMN.Game.BLUE,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.YELLOW,
            PKMN.Game.GOLD, PKMN.Game.SILVER, PKMN.Game.CRYSTAL
        },
        PKMN.Game.SAPPHIRE
    )]
    [TestCase(
        PKMN.Game.YELLOW,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.BLUE,
            PKMN.Game.GOLD, PKMN.Game.SILVER, PKMN.Game.CRYSTAL
        },
        PKMN.Game.EMERALD
    )]
    [TestCase(
        PKMN.Game.GOLD,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.BLUE, PKMN.Game.YELLOW,
            PKMN.Game.SILVER, PKMN.Game.CRYSTAL
        },
        PKMN.Game.FIRERED
    )]
    [TestCase(
        PKMN.Game.SILVER,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.BLUE, PKMN.Game.YELLOW,
            PKMN.Game.GOLD, PKMN.Game.CRYSTAL
        },
        PKMN.Game.LEAFGREEN
    )]
    [TestCase(
        PKMN.Game.CRYSTAL,
        new PKMN.Game[]{
            PKMN.Game.RED, PKMN.Game.BLUE, PKMN.Game.YELLOW,
            PKMN.Game.GOLD, PKMN.Game.SILVER
        },
        PKMN.Game.COLOSSEUM
    )]
    [TestCase(
        PKMN.Game.RUBY,
        new PKMN.Game[]{
            PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.RED
    )]
    [TestCase(
        PKMN.Game.SAPPHIRE,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.EMERALD, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.BLUE
    )]
    [TestCase(
        PKMN.Game.EMERALD,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.YELLOW
    )]
    [TestCase(
        PKMN.Game.FIRERED,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.GOLD
    )]
    [TestCase(
        PKMN.Game.LEAFGREEN,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.FIRERED,
            PKMN.Game.COLOSSEUM, PKMN.Game.XD
        },
        PKMN.Game.SILVER
    )]
    [TestCase(
        PKMN.Game.COLOSSEUM,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.XD
        },
        PKMN.Game.CRYSTAL
    )]
    [TestCase(
        PKMN.Game.XD,
        new PKMN.Game[]{
            PKMN.Game.RUBY, PKMN.Game.SAPPHIRE, PKMN.Game.EMERALD, PKMN.Game.FIRERED, PKMN.Game.LEAFGREEN,
            PKMN.Game.COLOSSEUM
        },
        PKMN.Game.RED
    )]
    public void PokemonPCTest(
        PKMN.Game game,
        PKMN.Game[] validOtherGames,
        PKMN.Game invalidOtherGame
    )
    {
        PKMNTest.PokemonPCTests.TestPokemonPC(
            new PKMN.PokemonPC(game),
            validOtherGames,
            invalidOtherGame
        );
    }
}
