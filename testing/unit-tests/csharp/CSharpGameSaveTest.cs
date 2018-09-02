/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpGameSaveTest
{
    /*
     * Red
     */
    [Test]
    public void RedGameSaveTest()
    {
        PKMNTest.GameSaveTest.TestGameSave(
            PKMN.GameSaveType.RED_BLUE,
            PKMN.Game.RED,
            "red_blue/pokemon_red.sav"
        );
    }

    /*
     * Yellow
     */
    [Test]
    public void YellowGameSaveTest()
    {
        PKMNTest.GameSaveTest.TestGameSave(
            PKMN.GameSaveType.YELLOW,
            PKMN.Game.YELLOW,
            "yellow/pokemon_yellow.sav"
        );
    }

    /*
     * Gold
     */
    [Test]
    public void GoldGameSaveTest()
    {
        PKMNTest.GameSaveTest.TestGameSave(
            PKMN.GameSaveType.GOLD_SILVER,
            PKMN.Game.GOLD,
            "gold_silver/pokemon_gold.sav"
        );
    }

    /*
     * Crystal
     */
    [Test]
    public void CrystalGameSaveTest()
    {
        PKMNTest.GameSaveTest.TestGameSave(
            PKMN.GameSaveType.CRYSTAL,
            PKMN.Game.CRYSTAL,
            "crystal/pokemon_crystal.sav"
        );
    }

    /*
     * Ruby
     */
    [Test]
    public void RubyGameSaveTest()
    {
        PKMNTest.GameSaveTest.TestGameSave(
            PKMN.GameSaveType.RUBY_SAPPHIRE,
            PKMN.Game.RUBY,
            "ruby_sapphire/pokemon_ruby.sav"
        );
    }

    /*
     * Emerald
     */
    [Test]
    public void EmeraldGameSaveTest()
    {
        PKMNTest.GameSaveTest.TestGameSave(
            PKMN.GameSaveType.EMERALD,
            PKMN.Game.EMERALD,
            "emerald/pokemon_emerald.sav"
        );
    }

    /*
     * FireRed
     */
    [Test]
    public void FireRedGameSaveTest()
    {
        PKMNTest.GameSaveTest.TestGameSave(
            PKMN.GameSaveType.FIRERED_LEAFGREEN,
            PKMN.Game.FIRERED,
            "firered_leafgreen/pokemon_firered.sav"
        );
    }

    /*
     * Colosseum
     */
    [Test]
    public void ColosseumGameSaveTest()
    {
        PKMNTest.GameSaveTest.TestGameSave(
            PKMN.GameSaveType.COLOSSEUM_XD,
            PKMN.Game.COLOSSEUM,
            "gamecube_saves/pokemon_colosseum.gci"
        );
    }

    /*
     * XD
     */
    [Test]
    public void XDGameSaveTest()
    {
        PKMNTest.GameSaveTest.TestGameSave(
            PKMN.GameSaveType.COLOSSEUM_XD,
            PKMN.Game.XD,
            "gamecube_saves/pokemon_xd.gci"
        );
    }
}
