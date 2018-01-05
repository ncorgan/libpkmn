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

    public string ValidBall;
    public string[] InvalidBalls;

    public string ValidItem;
    public string[] InvalidItems;

    public string ExpectedOriginalLocation;
    public string[] ValidLocations;
    public string[] InvalidLocations;

    public string[] ValidMoves;
    public string[] InvalidMoves;

    public string[] ValidOriginalGames;
    public string[] InvalidOriginalGames;
}

}
