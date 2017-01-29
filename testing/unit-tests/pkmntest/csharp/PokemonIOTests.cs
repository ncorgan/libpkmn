/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class PokemonIOTests {

    private static string LibPKMNTestFiles = "@LIBPKMN_TEST_FILES@";

    public static void Test3GPKM() {
        /*
         * Test files in repo and compare to known values.
         */
        string _3GPKMDir = System.IO.Path.Combine(LibPKMNTestFiles, "3gpkm");

        PKMN.Pokemon mightyena = new PKMN.Pokemon(
                                         System.IO.Path.Combine(_3GPKMDir, "MIGHTYENA.3gpkm")
                                     );
        Assert.AreEqual(mightyena.GetSpecies(), "Mightyena");
        Assert.AreEqual(mightyena.GetGame(), "Emerald");
        Assert.AreEqual(mightyena.GetForm(), "Standard");
        Assert.AreEqual(mightyena.GetNickname(), "MIGHTYENA");
        Assert.IsFalse(mightyena.IsShiny());
        Assert.AreEqual(mightyena.GetHeldItem().GetName(), "Heart Scale");
        Assert.AreEqual(mightyena.GetTrainerName(), "A");
        Assert.AreEqual(mightyena.GetTrainerPublicID(), 61415);
        Assert.AreEqual(mightyena.GetTrainerSecretID(), 3417);
        Assert.AreEqual(mightyena.GetTrainerID(), 223997927);
        Assert.AreEqual(mightyena.GetTrainerGender(), "Female");
        Assert.AreEqual(mightyena.GetFriendship(), 254);
        Assert.AreEqual(mightyena.GetAbility(), "Intimidate");
        Assert.AreEqual(mightyena.GetBall(), "Great Ball");
        Assert.AreEqual(mightyena.GetLevelMet(), 25);
        Assert.AreEqual(mightyena.GetLocationMet(false), "Route 120");
        Assert.AreEqual(mightyena.GetOriginalGame(), "Emerald");
        Assert.AreEqual(mightyena.GetPersonality(), 3557601241);
        Assert.AreEqual(mightyena.GetExperience(), 128734);
        Assert.AreEqual(mightyena.GetLevel(), 50);

        PKMN.StringBoolDict mightyenaMarkings = mightyena.GetMarkings();
        Assert.AreEqual(mightyenaMarkings.Count, 4);
        foreach(string marking in mightyenaMarkings.Keys) {
            Assert.IsFalse(mightyenaMarkings[marking]);
        }

        PKMN.StringBoolDict mightyenaRibbons = mightyena.GetRibbons();
        Assert.AreEqual(mightyenaRibbons.Count, 32);
        foreach(string ribbon in mightyenaRibbons.Keys) {
            if(ribbon.Equals("Champion")) {
                Assert.IsTrue(mightyenaRibbons[ribbon]);
            } else {
                Assert.IsFalse(mightyenaRibbons[ribbon]);
            }
        }

        PKMN.StringIntDict mightyenaContestStats = mightyena.GetContestStats();
        Assert.AreEqual(mightyenaContestStats.Count, 6);
        foreach(string contestStat in mightyenaContestStats.Keys) {
            Assert.AreEqual(mightyenaContestStats[contestStat], 0);
        }

        string[] expectedMightyenaMoves = {
            "Crunch", "Strength", "Shadow Ball", "Double-Edge"
        };
        PKMN.MoveSlotList mightyenaMoves = mightyena.GetMoves();
        Assert.AreEqual(mightyenaMoves.Count, 4);
        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(mightyenaMoves[i].Move.GetName(), expectedMightyenaMoves[i]);
        }

        PKMN.StringIntDict mightyenaEVs = mightyena.GetEVs();
        Assert.AreEqual(mightyenaEVs.Count, 6);
        Assert.AreEqual(mightyenaEVs["HP"], 30);
        Assert.AreEqual(mightyenaEVs["Attack"], 110);
        Assert.AreEqual(mightyenaEVs["Defense"], 32);
        Assert.AreEqual(mightyenaEVs["Speed"], 48);
        Assert.AreEqual(mightyenaEVs["Special Attack"], 17);
        Assert.AreEqual(mightyenaEVs["Special Defense"], 83);

        PKMN.StringIntDict mightyenaIVs = mightyena.GetIVs();
        Assert.AreEqual(mightyenaIVs.Count, 6);
        Assert.AreEqual(mightyenaIVs["HP"], 26);
        Assert.AreEqual(mightyenaIVs["Attack"], 28);
        Assert.AreEqual(mightyenaIVs["Defense"], 4);
        Assert.AreEqual(mightyenaIVs["Speed"], 13);
        Assert.AreEqual(mightyenaIVs["Special Attack"], 25);
        Assert.AreEqual(mightyenaIVs["Special Defense"], 26);

        PKMN.StringIntDict mightyenaStats = mightyena.GetStats();
        Assert.AreEqual(mightyenaStats.Count, 6);
        Assert.AreEqual(mightyenaStats["HP"], 146);
        Assert.AreEqual(mightyenaStats["Attack"], 122);
        Assert.AreEqual(mightyenaStats["Defense"], 81);
        Assert.AreEqual(mightyenaStats["Speed"], 87);
        Assert.AreEqual(mightyenaStats["Special Attack"], 79);
        Assert.AreEqual(mightyenaStats["Special Defense"], 88);
    }
}

}
