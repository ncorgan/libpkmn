/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class GameSaveTest {

    private static string PKSAV_TEST_SAVES = System.Environment.GetEnvironmentVariable("PKSAV_TEST_SAVES");

    public static void TestGameSave(
        string expectedType,
        string expectedGame,
        string subPath
    ) {
        System.Console.WriteLine(System.IO.Path.GetFullPath(System.IO.Path.Combine(PKSAV_TEST_SAVES, subPath)));
    }

}

}
