/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class Gen2PokemonTests {

    public static void TestInvalidPokemon(
        string game
    ) {
        PokemonTestsCommon.TestInvalidPokemon(game);
    }
}

}
