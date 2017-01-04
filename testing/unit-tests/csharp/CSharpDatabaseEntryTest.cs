/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
                new PKMN.ItemEntry("Not an item", "SoulSilver");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.ItemEntry("Fast Ball", "Not a game");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.ItemEntry("Fast Ball", "Red");
            }
        );

        PKMN.ItemEntry itemEntry = new PKMN.ItemEntry("Fast Ball", "SoulSilver");

        Assert.AreEqual(itemEntry.GetName(), "Fast Ball");
        Assert.AreEqual(itemEntry.GetGame(), "SoulSilver");
        Assert.AreEqual(itemEntry.GetCategory(), "Apricorn balls");
        Assert.AreEqual(itemEntry.GetPocket(), "Poké Balls");

        // Just make sure it works
        itemEntry.GetDescription();

        Assert.IsTrue(itemEntry.Holdable());
        Assert.AreEqual(itemEntry.GetFlingPower(), 0);
        Assert.AreEqual(itemEntry.GetFlingEffect(), "None");

        PKMN.ItemEntry itemEntrySame = new PKMN.ItemEntry("Fast Ball", "SoulSilver");
        PKMN.ItemEntry itemEntryDifferentItem = new PKMN.ItemEntry("Potion", "SoulSilver");
        PKMN.ItemEntry itemEntryDifferentGame = new PKMN.ItemEntry("Potion", "HeartGold");

        Assert.AreEqual(itemEntry, itemEntry);
        Assert.AreEqual(itemEntry, itemEntrySame);
        Assert.AreEqual(itemEntry.GetHashCode(), itemEntrySame.GetHashCode());

        Assert.AreNotEqual(itemEntry, itemEntryDifferentItem);
        Assert.AreNotEqual(itemEntry.GetHashCode(), itemEntryDifferentItem.GetHashCode());

        Assert.AreNotEqual(itemEntry, itemEntryDifferentGame);
        Assert.AreNotEqual(itemEntry.GetHashCode(), itemEntryDifferentGame.GetHashCode());
     }

    [Test]
    public void MoveEntryTest() {
        // Make sure trying to create an invalid entry results in an error.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.MoveEntry("Not an item", "Silver");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.MoveEntry("Octazooka", "Not a game");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.MoveEntry("Octazooka", "Red");
            }
        );

        PKMN.MoveEntry moveEntry = new PKMN.MoveEntry("Octazooka", "Silver");

        Assert.AreEqual(moveEntry.GetName(), "Octazooka");
        Assert.AreEqual(moveEntry.GetGame(), "Silver");
        Assert.AreEqual(moveEntry.GetMoveType(), "Water");

        // Just make sure it works
        moveEntry.GetDescription();

        Assert.AreEqual(moveEntry.GetTarget(), "Selected Pokémon");
        Assert.AreEqual(moveEntry.GetDamageClass(), "Special");
        Assert.AreEqual(moveEntry.GetBasePower(), 65);

        int[] PPs = {10, 12, 14, 16};
        for(int i = 0; i < 4; ++i) {
            Assert.AreEqual(moveEntry.GetPP(i), PPs[i]);
        }

        Assert.That(moveEntry.GetAccuracy(), Is.EqualTo(0.85f).Within(0.0001));
        Assert.AreEqual(moveEntry.GetPriority(), 0);

        // Just make sure it works
        moveEntry.GetEffect();

        Assert.AreEqual(moveEntry.GetContestType(), "None");
        Assert.AreEqual(moveEntry.GetContestEffect(), "None");
        Assert.AreEqual(moveEntry.GetSuperContestEffect(), "None");

        PKMN.MoveEntry moveEntrySame = new PKMN.MoveEntry("Octazooka", "Silver");
        PKMN.MoveEntry moveEntryDifferentMove = new PKMN.MoveEntry("Hydro Pump", "Silver");
        PKMN.MoveEntry moveEntryDifferentGame = new PKMN.MoveEntry("Octazooka", "Gold");

        Assert.AreEqual(moveEntry, moveEntry);
        Assert.AreEqual(moveEntry, moveEntrySame);
        Assert.AreEqual(moveEntry.GetHashCode(), moveEntrySame.GetHashCode());

        Assert.AreNotEqual(moveEntry, moveEntryDifferentMove);
        Assert.AreNotEqual(moveEntry.GetHashCode(), moveEntryDifferentMove.GetHashCode());

        Assert.AreNotEqual(moveEntry, moveEntryDifferentGame);
        Assert.AreNotEqual(moveEntry.GetHashCode(), moveEntryDifferentGame.GetHashCode());
    }

    [Test]
    public void PokemonEntryTest() {
        // Make sure trying to create an invalid entry results in an error.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.PokemonEntry("Invalid", "Black 2", "Sunny");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.PokemonEntry("Castform", "Not a game", "Sunny");
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                new PKMN.PokemonEntry("Castform", "Black 2", "Not a form");
            }
        );

        PKMN.PokemonEntry pokemonEntry = new PKMN.PokemonEntry("Stunfisk", "Black 2", "");

        Assert.AreEqual(pokemonEntry.GetName(), "Stunfisk");
        Assert.AreEqual(pokemonEntry.GetGame(), "Black 2");
        Assert.AreEqual(pokemonEntry.GetSpecies(), "Trap");
        Assert.AreEqual(pokemonEntry.GetForm(), "Standard");

        // Just make sure this works
        pokemonEntry.GetPokedexEntry();

        Assert.That(pokemonEntry.GetHeight(), Is.EqualTo(0.7).Within(0.0001));
        Assert.That(pokemonEntry.GetWeight(), Is.EqualTo(11.0).Within(0.0001));
        Assert.That(pokemonEntry.GetChanceMale(), Is.EqualTo(0.5).Within(0.0001));
        Assert.That(pokemonEntry.GetChanceFemale(), Is.EqualTo(0.5).Within(0.0001));
        Assert.IsFalse(pokemonEntry.HasGenderDifferences());
        Assert.AreEqual(pokemonEntry.GetBaseFriendship(), 70);
        Assert.AreEqual(pokemonEntry.GetTypes(), (new PKMN.StringPair("Ground", "Electric")));
        Assert.AreEqual(pokemonEntry.GetAbilities(), (new PKMN.StringPair("Static", "Limber")));
        Assert.AreEqual(pokemonEntry.GetHiddenAbility(), "Sand Veil");
        Assert.AreEqual(pokemonEntry.GetEggGroups(), (new PKMN.StringPair("Water 1", "Amorphous")));

        PKMN.StringIntDict baseStats = pokemonEntry.GetBaseStats();
        Assert.AreEqual(baseStats["HP"], 109);
        Assert.AreEqual(baseStats["Attack"], 66);
        Assert.AreEqual(baseStats["Defense"], 84);
        Assert.AreEqual(baseStats["Speed"], 32);
        Assert.AreEqual(baseStats["Special Attack"], 81);
        Assert.AreEqual(baseStats["Special Defense"], 99);

        PKMN.StringIntDict evYields = pokemonEntry.GetEVYields();
        Assert.AreEqual(evYields["HP"], 2);
        Assert.AreEqual(evYields["Attack"], 0);
        Assert.AreEqual(evYields["Defense"], 0);
        Assert.AreEqual(evYields["Speed"], 0);
        Assert.AreEqual(evYields["Special Attack"], 0);
        Assert.AreEqual(evYields["Special Defense"], 0);

        Assert.AreEqual(pokemonEntry.GetExperienceYield(), 165);
        Assert.AreEqual(pokemonEntry.GetExperienceAtLevel(50), 125000);
        Assert.AreEqual(pokemonEntry.GetLevelAtExperience(200000), 58);
        Assert.Greater(pokemonEntry.GetLevelupMoves().Count, 0);
        Assert.Greater(pokemonEntry.GetTMHMMoves().Count, 0);
        Assert.Greater(pokemonEntry.GetEggMoves().Count, 0);
        Assert.Greater(pokemonEntry.GetTutorMoves().Count, 0);
        Assert.AreEqual(pokemonEntry.GetForms().Count, 1);
        Assert.AreEqual(pokemonEntry.GetEvolutions().Count, 0);

        PKMN.PokemonEntry pokemonEntryFirst = new PKMN.PokemonEntry("Pikachu", "Omega Ruby", "Standard");
        PKMN.PokemonEntry pokemonEntrySame = new PKMN.PokemonEntry("Pikachu", "Omega Ruby", "Standard");
        PKMN.PokemonEntry pokemonEntryDifferentPokemon = new PKMN.PokemonEntry("Pichu", "Omega Ruby", "Standard");
        PKMN.PokemonEntry pokemonEntryDifferentGame = new PKMN.PokemonEntry("Pikachu", "Alpha Sapphire", "Standard");
        PKMN.PokemonEntry pokemonEntryDifferentForm = new PKMN.PokemonEntry("Pikachu", "Omega Ruby", "Belle");

        Assert.AreEqual(pokemonEntryFirst, pokemonEntryFirst);
        Assert.AreEqual(pokemonEntryFirst, pokemonEntrySame);
        Assert.AreEqual(pokemonEntryFirst.GetHashCode(), pokemonEntrySame.GetHashCode());

        Assert.AreNotEqual(pokemonEntryFirst, pokemonEntryDifferentPokemon);
        Assert.AreNotEqual(pokemonEntryFirst.GetHashCode(), pokemonEntryDifferentPokemon.GetHashCode());

        Assert.AreNotEqual(pokemonEntryFirst, pokemonEntryDifferentGame);
        Assert.AreNotEqual(pokemonEntryFirst.GetHashCode(), pokemonEntryDifferentGame.GetHashCode());

        Assert.AreNotEqual(pokemonEntryFirst, pokemonEntryDifferentForm);
        Assert.AreNotEqual(pokemonEntryFirst.GetHashCode(), pokemonEntryDifferentForm.GetHashCode());
    }
}
