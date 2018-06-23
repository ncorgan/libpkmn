/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class PokemonTestParams
{
    public PokemonTestParams() {}

    public PokemonTestParams(
        string validBall,
        string[] invalidBalls,
        string validItem,
        string[] invalidItems,
        string expectedOriginalLocation,
        string[] validLocations,
        string[] invalidLocations,
        string[] validMoves,
        string[] invalidMoves,
        PKMN.Game[] validOriginalGames,
        PKMN.Game[] invalidOriginalGames
    )
    {
        ValidBall = validBall;
        InvalidBalls = invalidBalls;
        ValidItem = validItem;
        InvalidItems = invalidItems;
        ExpectedOriginalLocation = expectedOriginalLocation;
        ValidLocations = validLocations;
        InvalidLocations = invalidLocations;
        ValidMoves = validMoves;
        InvalidMoves = invalidMoves;
        ValidOriginalGames = validOriginalGames;
        InvalidOriginalGames = invalidOriginalGames;
    }

    public string ValidBall;
    public string[] InvalidBalls;

    public string ValidItem;
    public string[] InvalidItems;

    public string ExpectedOriginalLocation;
    public string[] ValidLocations;
    public string[] InvalidLocations;

    public string[] ValidMoves;
    public string[] InvalidMoves;

    public PKMN.Game[] ValidOriginalGames;
    public PKMN.Game[] InvalidOriginalGames;
}

}
