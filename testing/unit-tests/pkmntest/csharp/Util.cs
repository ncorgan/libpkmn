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

internal class Util
{
    private static System.Random rng = new System.Random();

    internal static int GameToGeneration(
        string game
    )
    {
        if(game.Equals("Red") || game.Equals("Blue") || game.Equals("Yellow"))
        {
            return 1;
        }
        else if(game.Equals("Gold") || game.Equals("Silver") || game.Equals("Crystal"))
        {
            return 2;
        }
        else if(game.Equals("Ruby") || game.Equals("Sapphire") || game.Equals("Emerald") ||
                game.Equals("FireRed") || game.Equals("LeafGreen") ||
                game.Equals("Colosseum") || game.Equals("XD")
        )
        {
            return 3;
        }
        else if(game.Equals("Diamond") || game.Equals("Pearl") || game.Equals("Platinum") ||
                  game.Equals("HeartGold") || game.Equals("SoulSilver")
        )
        {
            return 4;
        }
        else if(game.Equals("Black") || game.Equals("White") ||
                game.Equals("Black 2") || game.Equals("White 2")
        )
        {
            return 5;
        }
        else if(game.Equals("X") || game.Equals("Y") ||
                game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire")) {
            return 6;
        }
        else
        {
            return 0;
        }
    }

    internal static bool RandomBool()
    {
        return (rng.Next() % 2) == 0;
    }

    // https://stackoverflow.com/a/1344295
    internal static string RandomString(
        int length
    )
    {
        return System.IO.Path.GetRandomFileName().Replace(".","").Substring(0,length);
    }

    internal static PKMN.Pokemon GetRandomPokemon(
        string game,
        PKMN.StringList itemList,
        PKMN.StringList moveList,
        PKMN.StringList pokemonList
    )
    {
        int generation = Util.GameToGeneration(game);

        // Don't deal with Deoxys issues here.
        string species;
        if(generation == 3)
        {
            do
            {
                species = pokemonList[rng.Next(0, pokemonList.Count-1)];
            }
            while(species == "Deoxys");
        }
        else
        {
            species = pokemonList[rng.Next(0, pokemonList.Count-1)];
        }

        PKMN.Pokemon ret = new PKMN.Pokemon(
                                    species,
                                    game,
                                    "",
                                    rng.Next(2, 100)
                                );

        for(int moveIndex = 0; moveIndex < 4; ++moveIndex)
        {
            string move;
            do
            {
                move = moveList[rng.Next(0, moveList.Count-1)];
            }
            while(move.IndexOf("Shadow") != -1);

            ret.Moves[moveIndex].Move = move;
        }

        foreach(string EV in ret.EVs.Keys)
        {
            ret.EVs[EV] = rng.Next(0, 255);
        }
        foreach(string IV in ret.IVs.Keys)
        {
            ret.IVs[IV] = rng.Next(0, 15);
        }

        if(generation >= 2)
        {
            // Keep going until one is holdable.
            while(ret.HeldItem == "None")
            {
                try
                {
                    ret.HeldItem = itemList[rng.Next(0, itemList.Count-1)];
                }
                catch(ArgumentOutOfRangeException)
                {
                }
            }

            ret.PokerusDuration = rng.Next(0, 15);
        }
        if(generation >= 3)
        {
            foreach(string marking in ret.Markings.Keys)
            {
                ret.Markings[marking] = Util.RandomBool();
            }
            foreach(string ribbon in ret.Ribbons.Keys)
            {
                ret.Ribbons[ribbon] = Util.RandomBool();
            }
            foreach(string contestStat in ret.ContestStats.Keys)
            {
                ret.ContestStats[contestStat] = rng.Next(0, 255);
            }
        }

        return ret;
    }

    internal static void ComparePokemon(
        PKMN.Pokemon pokemon1,
        PKMN.Pokemon pokemon2
    )
    {
        string game = pokemon1.Game;
        int generation = Util.GameToGeneration(game);

        // There is no way to tell what game an imported Generation I-II
        // Pokémon comes from, so LibPKMN defaults to a default valid game.
        if(generation >= 3)
        {
            Assert.AreEqual(pokemon2.Game, game);
        }

        Assert.AreEqual(pokemon1.Species, pokemon2.Species);
        Assert.AreEqual(pokemon1.Form, pokemon2.Form);
        Assert.AreEqual(pokemon1.OriginalTrainerID, pokemon2.OriginalTrainerID);
        Assert.AreEqual(pokemon1.Experience, pokemon2.Experience);
        Assert.AreEqual(pokemon1.Level, pokemon2.Level);
        Assert.AreEqual(pokemon1.Nickname, pokemon2.Nickname);
        Assert.AreEqual(pokemon1.OriginalTrainerName, pokemon2.OriginalTrainerName);

        Assert.AreEqual(pokemon1.EVs.Keys, pokemon2.EVs.Keys);
        foreach(string EV in pokemon1.EVs.Keys)
        {
            Assert.AreEqual(pokemon1.EVs[EV], pokemon2.EVs[EV]);
        }

        Assert.AreEqual(pokemon1.IVs.Keys, pokemon2.IVs.Keys);
        foreach(string IV in pokemon1.IVs.Keys)
        {
            Assert.AreEqual(pokemon1.IVs[IV], pokemon2.IVs[IV]);
        }

        Assert.AreEqual(pokemon1.Stats.Keys, pokemon2.Stats.Keys);
        foreach(string stat in pokemon1.Stats.Keys)
        {
            Assert.AreEqual(pokemon1.Stats[stat], pokemon2.Stats[stat]);
        }

        if(pokemon1.Game.Equals(pokemon2.Game))
        {
            Assert.AreEqual(pokemon1.IconFilepath, pokemon2.IconFilepath);
            Assert.AreEqual(pokemon1.SpriteFilepath, pokemon2.SpriteFilepath);
        }

        Assert.AreEqual(pokemon1.NumericAttributes.Names, pokemon2.NumericAttributes.Names);
        foreach(string attributeName in pokemon1.NumericAttributes.Names)
        {
            Assert.AreEqual(
                pokemon1.NumericAttributes[attributeName],
                pokemon2.NumericAttributes[attributeName]
            );
        }
        Assert.AreEqual(pokemon1.StringAttributes.Names, pokemon2.StringAttributes.Names);
        foreach(string attributeName in pokemon1.StringAttributes.Names)
        {
            Assert.AreEqual(
                pokemon1.StringAttributes[attributeName],
                pokemon2.StringAttributes[attributeName]
            );
        }
        Assert.AreEqual(pokemon1.BooleanAttributes.Names, pokemon2.BooleanAttributes.Names);
        foreach(string attributeName in pokemon1.BooleanAttributes.Names)
        {
            Assert.AreEqual(
                pokemon1.BooleanAttributes[attributeName],
                pokemon2.BooleanAttributes[attributeName]
            );
        }

        if(generation >= 2)
        {
            Assert.AreEqual(pokemon1.OriginalTrainerGender, pokemon2.OriginalTrainerGender);
            Assert.AreEqual(pokemon1.CurrentTrainerFriendship, pokemon2.CurrentTrainerFriendship);
            Assert.AreEqual(pokemon1.Gender, pokemon2.Gender);
            Assert.AreEqual(pokemon1.IsShiny, pokemon2.IsShiny);
            Assert.AreEqual(pokemon1.HeldItem, pokemon2.HeldItem);
            Assert.AreEqual(pokemon1.LevelMet, pokemon2.LevelMet);
            Assert.AreEqual(pokemon1.LocationMet, pokemon2.LocationMet);
            Assert.AreEqual(pokemon1.PokerusDuration, pokemon2.PokerusDuration);
        }
        if(generation >= 3)
        {
            Assert.AreEqual(pokemon1.Ability, pokemon2.Ability);
            Assert.AreEqual(pokemon1.Ball, pokemon2.Ball);
            Assert.AreEqual(pokemon1.Personality, pokemon2.Personality);

            Assert.AreEqual(pokemon1.Markings.Keys, pokemon2.Markings.Keys);
            foreach(string marking in pokemon1.Markings.Keys)
            {
                Assert.AreEqual(pokemon1.Markings[marking], pokemon2.Markings[marking]);
            }

            Assert.AreEqual(pokemon1.Ribbons.Keys, pokemon2.Ribbons.Keys);
            foreach(string ribbon in pokemon1.Ribbons.Keys)
            {
                Assert.AreEqual(pokemon1.Ribbons[ribbon], pokemon2.Ribbons[ribbon]);
            }

            Assert.AreEqual(pokemon1.ContestStats.Keys, pokemon2.ContestStats.Keys);
            foreach(string contestStat in pokemon1.ContestStats.Keys)
            {
                Assert.AreEqual(pokemon1.ContestStats[contestStat], pokemon2.ContestStats[contestStat]);
            }
        }
        if(generation >= 4)
        {
            Assert.AreEqual(pokemon1.LocationMetAsEgg, pokemon2.LocationMetAsEgg);
        }
    }
}
}
