/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpListsTest {
    [Test]
    public void AbilityListTest() {
        // Make sure trying to create an invalid list results in an exception
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Database.GetAbilityList(0);
            }
        );

        PKMN.StringList abilityList = PKMN.Database.GetAbilityList(6);
        Assert.That(abilityList.Count, Is.EqualTo(191));
        Assert.That(abilityList[0], Is.EqualTo("Adaptability"));
        Assert.That(abilityList[190], Is.EqualTo("Zen Mode"));
    }
}
