/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpDatabaseEntryTest {
    [Test]
    public void ItemEntryTest() {
        // Make sure trying to create an invalid entry results in an error.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.Database.ItemEntry(PKMN.Item.INVALID, PKMN.Game.SOULSILVER);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.Database.ItemEntry(PKMN.Item.FAST_BALL, PKMN.Game.NONE);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.Database.ItemEntry(PKMN.Item.FAST_BALL, PKMN.Game.RED);
            }
        );

        PKMN.Database.ItemEntry itemEntry = new PKMN.Database.ItemEntry(PKMN.Item.FAST_BALL, PKMN.Game.SOULSILVER);

        Assert.AreEqual(itemEntry.Item, PKMN.Item.FAST_BALL);
        Assert.AreEqual(itemEntry.Name, "Fast Ball");
        Assert.AreEqual(itemEntry.Game, PKMN.Game.SOULSILVER);
        Assert.AreEqual(itemEntry.Category, "Apricorn balls");
        Assert.AreEqual(itemEntry.Pocket, "Poké Balls");

        // Just make sure it works
        string description = itemEntry.Description;

        Assert.IsTrue(itemEntry.IsHoldable);
        Assert.AreEqual(itemEntry.FlingPower, 0);
        Assert.AreEqual(itemEntry.FlingEffect, "None");
     }

    [Test]
    public void MoveEntryTest() {
        // Make sure trying to create an invalid entry results in an error.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.Database.MoveEntry(PKMN.Move.OCTAZOOKA, PKMN.Game.NONE);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.Database.MoveEntry(PKMN.Move.OCTAZOOKA, PKMN.Game.RED);
            }
        );

        PKMN.Database.MoveEntry moveEntry = new PKMN.Database.MoveEntry(
                                                    PKMN.Move.OCTAZOOKA,
                                                    PKMN.Game.SILVER
                                                );

        Assert.AreEqual(moveEntry.Move, PKMN.Move.OCTAZOOKA);
        Assert.AreEqual(moveEntry.Name, "Octazooka");
        Assert.AreEqual(moveEntry.Game, PKMN.Game.SILVER);
        Assert.AreEqual(moveEntry.MoveType, PKMN.Type.WATER);

        // Just make sure it works
        string description = moveEntry.Description;

        Assert.AreEqual(moveEntry.Target, PKMN.MoveTarget.SELECTED_POKEMON);
        Assert.AreEqual(moveEntry.DamageClass, PKMN.MoveDamageClass.SPECIAL);
        Assert.AreEqual(moveEntry.BasePower, 65);

        int[] PPs = {10, 12, 14, 16};
        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(moveEntry.GetPP(i), PPs[i]);
        }

        Assert.That(moveEntry.Accuracy, Is.EqualTo(0.85f).Within(0.0001));
        Assert.AreEqual(moveEntry.Priority, 0);

        // Just make sure it works
        string effect = moveEntry.Effect;

        Assert.AreEqual(moveEntry.ContestType, "None");
        Assert.AreEqual(moveEntry.ContestEffect, "None");
        Assert.AreEqual(moveEntry.SuperContestEffect, "None");
    }

    [Test]
    public void PokemonEntryTest() {
        // Make sure trying to create an invalid entry results in an error.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.Database.PokemonEntry(PKMN.Species.CASTFORM, PKMN.Game.NONE, "Sunny");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.Database.PokemonEntry(PKMN.Species.CASTFORM, PKMN.Game.BLACK2, "Not a form");
            }
        );

        PKMN.Database.PokemonEntry pokemonEntry = new PKMN.Database.PokemonEntry(PKMN.Species.STUNFISK, PKMN.Game.BLACK2, "");

        Assert.AreEqual(pokemonEntry.Species, PKMN.Species.STUNFISK);
        Assert.AreEqual(pokemonEntry.SpeciesName, "Stunfisk");
        Assert.AreEqual(pokemonEntry.Game, PKMN.Game.BLACK2);
        Assert.AreEqual(pokemonEntry.Category, "Trap");
        Assert.AreEqual(pokemonEntry.Form, "Standard");

        // Just make sure this works
        string pokedexEntry = pokemonEntry.PokedexEntry;

        Assert.That(pokemonEntry.Height, Is.EqualTo(0.7).Within(0.0001));
        Assert.That(pokemonEntry.Weight, Is.EqualTo(11.0).Within(0.0001));
        Assert.That(pokemonEntry.ChanceMale, Is.EqualTo(0.5).Within(0.0001));
        Assert.That(pokemonEntry.ChanceFemale, Is.EqualTo(0.5).Within(0.0001));
        Assert.IsFalse(pokemonEntry.HasGenderDifferences);
        Assert.AreEqual(pokemonEntry.BaseFriendship, 70);
        Assert.AreEqual(
            pokemonEntry.Types,
            new PKMN.TypeEnumPair(PKMN.Type.GROUND, PKMN.Type.ELECTRIC)
        );
        Assert.AreEqual(
            pokemonEntry.Abilities,
            new PKMN.AbilityEnumPair(PKMN.Ability.STATIC, PKMN.Ability.LIMBER)
        );
        Assert.AreEqual(pokemonEntry.HiddenAbility, PKMN.Ability.SAND_VEIL);
        Assert.AreEqual(
            pokemonEntry.EggGroups,
            new PKMN.EggGroupEnumPair(PKMN.EggGroup.WATER1, PKMN.EggGroup.INDETERMINATE)
        );

        PKMN.StatDict baseStats = pokemonEntry.BaseStats;
        Assert.AreEqual(baseStats[PKMN.Stat.HP], 109);
        Assert.AreEqual(baseStats[PKMN.Stat.ATTACK], 66);
        Assert.AreEqual(baseStats[PKMN.Stat.DEFENSE], 84);
        Assert.AreEqual(baseStats[PKMN.Stat.SPEED], 32);
        Assert.AreEqual(baseStats[PKMN.Stat.SPECIAL_ATTACK], 81);
        Assert.AreEqual(baseStats[PKMN.Stat.SPECIAL_DEFENSE], 99);

        PKMN.StatDict evYields = pokemonEntry.EVYields;
        Assert.AreEqual(evYields[PKMN.Stat.HP], 2);
        Assert.AreEqual(evYields[PKMN.Stat.ATTACK], 0);
        Assert.AreEqual(evYields[PKMN.Stat.DEFENSE], 0);
        Assert.AreEqual(evYields[PKMN.Stat.SPEED], 0);
        Assert.AreEqual(evYields[PKMN.Stat.SPECIAL_ATTACK], 0);
        Assert.AreEqual(evYields[PKMN.Stat.SPECIAL_DEFENSE], 0);

        Assert.AreEqual(pokemonEntry.ExperienceYield, 165);
        Assert.AreEqual(pokemonEntry.GetExperienceAtLevel(50), 125000);
        Assert.AreEqual(pokemonEntry.GetLevelAtExperience(200000), 58);
        Assert.Greater(pokemonEntry.LevelupMoves.Count, 0);
        Assert.Greater(pokemonEntry.TMHMMoves.Count, 0);
        Assert.Greater(pokemonEntry.EggMoves.Count, 0);
        Assert.Greater(pokemonEntry.TutorMoves.Count, 0);
        Assert.AreEqual(pokemonEntry.Forms.Count, 1);
        Assert.AreEqual(pokemonEntry.Evolutions.Count, 0);

        Assert.IsTrue(System.IO.File.Exists(pokemonEntry.GetIconFilepath(false)));
        Assert.IsTrue(System.IO.File.Exists(pokemonEntry.GetIconFilepath(true)));
        Assert.IsTrue(System.IO.File.Exists(pokemonEntry.GetSpriteFilepath(false,false)));
        Assert.IsTrue(System.IO.File.Exists(pokemonEntry.GetSpriteFilepath(false,true)));
        Assert.IsTrue(System.IO.File.Exists(pokemonEntry.GetSpriteFilepath(true,false)));
        Assert.IsTrue(System.IO.File.Exists(pokemonEntry.GetSpriteFilepath(true,true)));
    }
}
