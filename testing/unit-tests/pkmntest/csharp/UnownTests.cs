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

public class UnownTests 
{
    public static void Gen2UnownTest(
        string game
    )
    {
        PKMN.Database.PokemonEntry unownEntry = new PKMN.Database.PokemonEntry(
                                                        "Unown", game, ""
                                                    );
        PKMN.Pokemon unown;

        foreach(string form in unownEntry.Forms)
        {
            unown = new PKMN.Pokemon("Unown", game, form, 5);
            Assert.AreEqual(unown.Form, form);

            // Make sure IVs are properly set.
            string formFromIVs = PKMN.Calculations.Gen2UnownForm(
                                     unown.IVs["Attack"],
                                     unown.IVs["Defense"],
                                     unown.IVs["Speed"],
                                     unown.IVs["Special"]
                                 );
            Assert.AreEqual(unown.Form, formFromIVs);

            Assert.IsTrue(System.IO.File.Exists(unown.IconFilepath));
            Assert.IsTrue(System.IO.File.Exists(unown.SpriteFilepath));
        }

        unown = new PKMN.Pokemon("Unown", game, "A", 5);

        // Make sure setting the form properly changes the IVs.
        foreach(string form in unownEntry.Forms)
        {
            unown.Form = form;
            Assert.AreEqual(unown.Form, form);

            string formFromIVs = PKMN.Calculations.Gen2UnownForm(
                                     unown.IVs["Attack"],
                                     unown.IVs["Defense"],
                                     unown.IVs["Speed"],
                                     unown.IVs["Special"]
                                 );
            Assert.AreEqual(unown.Form, formFromIVs);

            Assert.IsTrue(System.IO.File.Exists(unown.IconFilepath));
            Assert.IsTrue(System.IO.File.Exists(unown.SpriteFilepath));
        }

        // Make sure setting IVs properly changes the form.
        unown.IVs["Attack"] = 10;
        unown.IVs["Defense"] = 9;
        unown.IVs["Speed"] = 1;
        unown.IVs["Special"] = 14;
        Assert.AreEqual(unown.Form, "G");
    }

    public static void Gen3UnownTest(
        string game
    )
    {
        PKMN.Database.PokemonEntry unownEntry = new PKMN.Database.PokemonEntry(
                                                        "Unown", game, ""
                                                    );
        PKMN.Pokemon unown;

        foreach(string form in unownEntry.Forms)
        {
            unown = new PKMN.Pokemon("Unown", game, form, 5);
            Assert.AreEqual(unown.Form, form);

            // Make sure the personality is properly set.
            string formFromPersonality = PKMN.Calculations.Gen3UnownForm(unown.Personality);
            Assert.AreEqual(unown.Form, formFromPersonality);

            Assert.IsTrue(System.IO.File.Exists(unown.IconFilepath));
            Assert.IsTrue(System.IO.File.Exists(unown.SpriteFilepath));
        }

        unown = new PKMN.Pokemon("Unown", game, "A", 5);

        foreach(string form in unownEntry.Forms)
        {
            unown.Form = form;
            Assert.AreEqual(unown.Form, form);

            // Make sure the personality is properly set.
            string formFromPersonality = PKMN.Calculations.Gen3UnownForm(unown.Personality);
            Assert.AreEqual(unown.Form, formFromPersonality);

            Assert.IsTrue(System.IO.File.Exists(unown.IconFilepath));
            Assert.IsTrue(System.IO.File.Exists(unown.SpriteFilepath));
        }

        // Make sure setting the personality properly sets the form.
        unown.Personality = 0x4C07DE71;
        Assert.AreEqual(unown.Form, "B");
    }
}

}
