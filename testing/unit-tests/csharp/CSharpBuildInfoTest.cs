/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpBuildInfoTest
{
    [Test]
    public void PKMNVersionTest()
    {
        Assert.Greater(PKMN.BuildInfo.PKMNVersion.Length, 0);
    }

    [Test]
    public void BoostVersionTest()
    {
        Assert.Greater(PKMN.BuildInfo.BoostVersion.Length, 0);
    }

    [Test]
    public void LibPkmGCVersionTest()
    {
        Assert.Greater(PKMN.BuildInfo.LibPkmGCVersion.Length, 0);
    }

    [Test]
    public void PKSavVersionTest()
    {
        Assert.Greater(PKMN.BuildInfo.PKSavVersion.Length, 0);
    }

    [Test]
    public void QtVersionTest()
    {
        Assert.Greater(PKMN.BuildInfo.QtVersion.Length, 0);
    }

    [Test]
    public void SQLite3VersionTest()
    {
        Assert.Greater(PKMN.BuildInfo.SQLite3Version.Length, 0);
    }

    [Test]
    public void SQLiteCppVersionTest()
    {
        Assert.Greater(PKMN.BuildInfo.SQLiteCppVersion.Length, 0);
    }

    [Test]
    public void SWIGVersionTest()
    {
        Assert.Greater(PKMN.BuildInfo.SWIGVersion.Length, 0);
    }
}
