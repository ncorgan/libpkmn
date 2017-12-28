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

internal class Util
{
    private static System.Random rng = new System.Random();

    internal static int GameToGeneration(
        string game
    )
    {
        if(game.Equals("Red") || game.Equals("Blue") || game.Equals("Yellow"))
        {
            return 1;
        }
        else if(game.Equals("Gold") || game.Equals("Silver") || game.Equals("Crystal"))
        {
            return 2;
        }
        else if(game.Equals("Ruby") || game.Equals("Sapphire") || game.Equals("Emerald") ||
                game.Equals("FireRed") || game.Equals("LeafGreen") ||
                game.Equals("Colosseum") || game.Equals("XD")
        )
        {
            return 3;
        }
        else if(game.Equals("Diamond") || game.Equals("Pearl") || game.Equals("Platinum") ||
                  game.Equals("HeartGold") || game.Equals("SoulSilver")
        )
        {
            return 4;
        }
        else if(game.Equals("Black") || game.Equals("White") ||
                game.Equals("Black 2") || game.Equals("White 2")
        )
        {
            return 5;
        }
        else if(game.Equals("X") || game.Equals("Y") ||
                game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire")) {
            return 6;
        }
        else
        {
            return 0;
        }
    }

    internal static bool RandomBool()
    {
        return (rng.Next() % 2) == 0;
    }

    // https://stackoverflow.com/a/1344295
    internal static string RandomString(
        int length
    )
    {
        return System.IO.Path.GetRandomFileName().Replace(".","").Substring(0,length);
    }


}
}
