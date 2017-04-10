/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpDateTimeTest {

    [Test]
    public void CurrentDateTimeTest() {
        // TODO: fix PKMN.PKMN, eww
        PKMN.DateTime now = PKMN.PKMN.CurrentDateTime();
        Assert.GreaterOrEqual(now.Year, 1900);
        Assert.That(now.Month, Is.InRange(1, 12));
        Assert.That(now.Day, Is.InRange(1, 31));
        Assert.That(now.Hour, Is.InRange(0, 23));
        Assert.That(now.Minute, Is.InRange(0, 59));
        Assert.That(now.Second, Is.InRange(0, 59));
        Assert.AreEqual(0, now.Frames);
    }

}
