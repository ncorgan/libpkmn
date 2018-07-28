/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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
        PKMN.Ball validBall,
        PKMN.Ball[] invalidBalls,
        PKMN.Item validItem,
        PKMN.Item[] invalidItems,
        string expectedOriginalLocation,
        string[] validLocations,
        string[] invalidLocations,
        PKMN.Move[] validMoves,
        PKMN.Move[] invalidMoves,
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

    public PKMN.Ball ValidBall;
    public PKMN.Ball[] InvalidBalls;

    public PKMN.Item ValidItem;
    public PKMN.Item[] InvalidItems;

    public string ExpectedOriginalLocation;
    public string[] ValidLocations;
    public string[] InvalidLocations;

    public PKMN.Move[] ValidMoves;
    public PKMN.Move[] InvalidMoves;

    public PKMN.Game[] ValidOriginalGames;
    public PKMN.Game[] InvalidOriginalGames;
}

}
