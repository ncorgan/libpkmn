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

public class PokemonTestsCommon
{
    private static string[] Gen1PokemonWithXYMegaForms =
    {
        "Venusaur", "Blastoise", "Alakazam", "Gengar", "Kangaskhan", "Pinsir", "Gyarados", "Aerodactyl"
    };

    private static string[] Gen1PokemonWithORASMegaForms =
    {
        "Beedrill", "Pidgeot", "Slowbro"
    };

    private static string[] Gen2PokemonWithXYMegaForms =
    {
        "Ampharos", "Scizor", "Heracross", "Houndoom", "Tyranitar"
    };

    private static string[] Gen3PokemonWithXYMegaForms =
    {
        "Blaziken", "Gardevoir", "Mawile", "Aggron", "Medicham",
        "Manectric", "Banette", "Absol", "Latias", "Latios", "Garchomp"
    };

    private static string[] Gen3PokemonWithORASMegaForms =
    {
        "Sceptile", "Swampert", "Sableye", "Sharpedo", "Camerupt",
        "Altaria", "Glalie", "Salamence", "Metagross", "Rayquaza"
    };

    private static void TestGen1Forms(
        string game
    )
    {
        int generation = Util.GameToGeneration(game);
        bool isORAS = (game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire"));

        // Check that Mega forms only work in their given games.
        foreach(string species in Gen1PokemonWithXYMegaForms)
        {
            if(generation >= 6)
            {
                new PKMN.Pokemon2(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon2(species, game, "Mega", 100);
                    }
                );
            }
        }
        foreach(string species in Gen1PokemonWithORASMegaForms)
        {
            if(isORAS)
            {
                new PKMN.Pokemon2(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon2(species, game, "Mega", 100);
                    }
                );
            }
        }

        // Cosplay Pikachu should only work in OR/AS.
        PKMN.Database.PokemonEntry cosplayPikachu = new PKMN.Database.PokemonEntry("Pikachu", game, "");
        foreach(string form in cosplayPikachu.Forms)
        {
            if(isORAS)
            {
                new PKMN.Pokemon2("Pikachu", game, form, 5);
            }
            else if(!form.Equals("Standard"))
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon2("Pikachu", game, form, 5);
                    }
                );
            }
        }

        // Hard-code Mega X/Y cases.
        if(generation >= 6)
        {
            new PKMN.Pokemon2("Charizard", game, "Mega X", 50);
            new PKMN.Pokemon2("Charizard", game, "Mega Y", 50);
            new PKMN.Pokemon2("Mewtwo", game, "Mega X", 50);
            new PKMN.Pokemon2("Mewtwo", game, "Mega Y", 50);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon2("Charizard", game, "Mega X", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon2("Charizard", game, "Mega Y", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon2("Mewtwo", game, "Mega X", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon2("Mewtwo", game, "Mega Y", 50);
                }
            );
        }
    }

    private static void TestGen2Forms(
        string game
    )
    {
        int generation = Util.GameToGeneration(game);
        bool isORAS = (game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire"));

        // Check that Mega forms only work in their given games.
        foreach(string species in Gen2PokemonWithXYMegaForms)
        {
            if(generation >= 6)
            {
                new PKMN.Pokemon2(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon2(species, game, "Mega", 100);
                    }
                );
            }
        }

        // Only one Generation II Pokémon with a OR/AS-specific Mega form.
        if(isORAS)
        {
            new PKMN.Pokemon2("Steelix", game, "Mega", 100);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon2("Steelix", game, "Mega", 100);
                }
            );
        }

        // Spiky-eared Pichu should only work in HG/SS.
        if(game.Equals("HeartGold") || game.Equals("SoulSilver"))
        {
            new PKMN.Pokemon2("Pichu", game, "Spiky-eared", 100);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon2("Pichu", game, "Spiky-eared", 100);
                }
            );
        }

        // Unown's "!" and "?" forms aren't in Generation II.
        PKMN.Database.PokemonEntry unownEntry = new PKMN.Database.PokemonEntry("Unown", "Omega Ruby", "");
        foreach(string form in unownEntry.Forms)
        {
            if(form.Equals("!") || form.Equals("?"))
            {
                if(generation > 2)
                {
                    PKMN.Pokemon2 unown = new PKMN.Pokemon2("Unown", game, form, 10);
                    Assert.IsTrue(System.IO.File.Exists(unown.IconFilepath));
                    Assert.IsTrue(System.IO.File.Exists(unown.SpriteFilepath));
                }
                else
                {
                    Assert.Throws<ArgumentOutOfRangeException>(
                        delegate
                        {
                            new PKMN.Pokemon2("Unown", game, form, 10);
                        }
                    );
                }
            }
            else
            {
                PKMN.Pokemon2 unown = new PKMN.Pokemon2("Unown", game, form, 10);
                Assert.IsTrue(System.IO.File.Exists(unown.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(unown.SpriteFilepath));
            }
        }
    }

    private static void TestGen3Forms(
        string game,
        bool gcn
    )
    {
        int generation = Util.GameToGeneration(game);
        bool isORAS = (game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire"));

        // Check that Mega forms only work in their given games.
        foreach(string species in Gen3PokemonWithXYMegaForms)
        {
            if(generation >= 6)
            {
                new PKMN.Pokemon2(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon2(species, game, "Mega", 100);
                    }
                );
            }
        }
        foreach(string species in Gen3PokemonWithORASMegaForms)
        {
            if(isORAS)
            {
                new PKMN.Pokemon2(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon2(species, game, "Mega", 100);
                    }
                );
            }
        }

        // Castform should always work.
        PKMN.Database.PokemonEntry castformEntry = new PKMN.Database.PokemonEntry("Castform", "Omega Ruby", "");
        foreach(string form in castformEntry.Forms)
        {
            PKMN.Pokemon2 castform = new PKMN.Pokemon2("Castform", game, form, 30);
            Assert.IsTrue(System.IO.File.Exists(castform.IconFilepath));
            Assert.IsTrue(System.IO.File.Exists(castform.SpriteFilepath));
        }

        // Primal Reversion forms should only work in OR/AS.

        PKMN.Pokemon2 groudon = new PKMN.Pokemon2("Groudon", game, "", 70);
        Assert.IsTrue(System.IO.File.Exists(groudon.IconFilepath));
        Assert.IsTrue(System.IO.File.Exists(groudon.SpriteFilepath));

        PKMN.Pokemon2 kyogre = new PKMN.Pokemon2("Kyogre", game, "", 70);
        Assert.IsTrue(System.IO.File.Exists(kyogre.IconFilepath));
        Assert.IsTrue(System.IO.File.Exists(kyogre.SpriteFilepath));

        if(isORAS)
        {
            new PKMN.Pokemon2("Groudon", game, "Primal Reversion", 70);
            new PKMN.Pokemon2("Kyogre", game, "Primal Reversion", 70);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon2("Groudon", game, "Primal Reversion", 70);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon2("Kyogre", game, "Primal Reversion", 70);
                }
            );
        }

        // In Generation III, Deoxys's form is game-specific.
        if(generation == 3)
        {
            if(game.Equals("Ruby") || game.Equals("Sapphire") || game.Equals("Colosseum") || game.Equals("XD"))
            {
                PKMN.Pokemon2 deoxys = new PKMN.Pokemon2("Deoxys", game, "Normal", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon2("Deoxys", game, "Normal", 70);
                    }
                );
            }

            if(game.Equals("FireRed"))
            {
                PKMN.Pokemon2 deoxys = new PKMN.Pokemon2("Deoxys", game, "Attack", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon2("Deoxys", game, "Attack", 70);
                    }
                );
            }

            if(game.Equals("LeafGreen"))
            {
                PKMN.Pokemon2 deoxys = new PKMN.Pokemon2("Deoxys", game, "Defense", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon2("Deoxys", game, "Defense", 70);
                    }
                );
            }

            if(game.Equals("Emerald"))
            {
                PKMN.Pokemon2 deoxys = new PKMN.Pokemon2("Deoxys", game, "Speed", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon2("Deoxys", game, "Speed", 70);
                    }
                );
            }
        }
        else
        {
            // Past Generation III, Deoxys's form can be switched.
            PKMN.Database.PokemonEntry deoxysEntry = new PKMN.Database.PokemonEntry("Deoxys", "Omega Ruby", "");
            foreach(string form in deoxysEntry.Forms)
            {
                PKMN.Pokemon2 deoxys = new PKMN.Pokemon2("Deoxys", game, form, 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
        }
    }

    public static void TestForms(
        string game
    )
    {
        int generation = Util.GameToGeneration(game);

        /*
         * Make sure forms that didn't appear for a given
         * generation's Pokémon until later don't work until
         * the correct generation.
         */
        if(generation >= 1)
        {
            TestGen1Forms(game);
        }
        if(generation >= 2)
        {
            TestGen2Forms(game);
        }
        if(generation >= 3)
        {
            TestGen3Forms(
                game,
                (game.Equals("Colosseum") || game.Equals("XD"))
            );
        }
    }

    public static void GenderTest(
        string game
    ) {

        // Single-gender
        PKMN.Pokemon2 nidorina = new PKMN.Pokemon2(
                                        "Nidorina",
                                        game,
                                        "",
                                        50
                                    );
        Assert.AreEqual(nidorina.Gender, "Female");
        nidorina.Gender = "Female";
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                nidorina.Gender = "Male";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                nidorina.Gender = "Genderless";
            }
        );

        PKMN.Pokemon2 nidorino = new PKMN.Pokemon2(
                                        "Nidorino",
                                        game,
                                        "",
                                        50
                                    );
        Assert.AreEqual(nidorino.Gender, "Male");
        nidorino.Gender = "Male";
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                nidorino.Gender = "Female";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                nidorino.Gender = "Genderless";
            }
        );

        PKMN.Pokemon2 magnemite = new PKMN.Pokemon2(
                                         "Magnemite",
                                         game,
                                         "",
                                         50
                                     );
        Assert.AreEqual(magnemite.Gender, "Genderless");
        magnemite.Gender = "Genderless";
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                magnemite.Gender = "Male";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                magnemite.Gender = "Female";
            }
        );

        string[] mixedPokemon = {
            "Charmander", // 87.5% male
            "Growlithe",  // 75% male
            "Pidgey",     // 50% male
            "Vulpix"      // 25% male
        };
        foreach(string species in mixedPokemon)
        {
            PKMN.Pokemon2 pokemon = new PKMN.Pokemon2(
                                           species,
                                           game,
                                           "",
                                           50
                                       );

            pokemon.Gender = "Female";
            Assert.AreEqual(pokemon.Gender, "Female");
            pokemon.Gender = "Male";
            Assert.AreEqual(pokemon.Gender, "Male");
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    pokemon.Gender = "Genderless";
                }
            );
        }
    }

    private static void checkInitialValues(
        PKMN.Pokemon2 pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        Assert.AreEqual(pokemon.Condition, "None");

        if(generation >= 5)
        {
            Assert.AreEqual(pokemon.Nickname, pokemon.Species);
        }
        else
        {
            Assert.AreEqual(pokemon.Nickname, pokemon.Species.ToUpper());
        }
        Assert.AreEqual(pokemon.OriginalTrainerName, PKMN.Pokemon2.DEFAULT_TRAINER_NAME);

        if(generation >= 2)
        {
            Assert.AreEqual(pokemon.HeldItem, "None");
        }

        Assert.AreEqual(pokemon.OriginalTrainerGender, "Male");
        Assert.AreEqual(
            pokemon.OriginalTrainerPublicID,
            (ushort)(PKMN.Pokemon2.DEFAULT_TRAINER_ID & 0xFFFF)
        );
        if(generation >= 3)
        {
            Assert.AreEqual(
                pokemon.OriginalTrainerSecretID,
                (ushort)(PKMN.Pokemon2.DEFAULT_TRAINER_ID >> 16)
            );
            Assert.AreEqual(
                pokemon.OriginalTrainerID,
                PKMN.Pokemon2.DEFAULT_TRAINER_ID
            );
        }
        else
        {
            Assert.AreEqual(
                pokemon.OriginalTrainerID,
                PKMN.Pokemon2.DEFAULT_TRAINER_ID
            );
        }

        if(generation >= 2)
        {
            Assert.AreEqual(
                pokemon.CurrentTrainerFriendship,
                pokemon.DatabaseEntry.BaseFriendship
            );
        }

        if(generation >= 3)
        {
            Assert.AreEqual(pokemon.Ball, "Premier Ball");

            // There is no distinction between Colosseum and XD in the game
            // storage.
            if(pokemon.Game.Equals("Colosseum") || pokemon.Game.Equals("XD"))
            {
                Assert.AreEqual(
                    pokemon.OriginalGame,
                    "Colosseum/XD"
                );
            }
            else
            {
                Assert.AreEqual(pokemon.OriginalGame, pokemon.Game);
            }
        }

        Assert.AreEqual(
            pokemon.Experience,
            pokemon.DatabaseEntry.GetExperienceAtLevel(pokemon.Level)
        );
        Assert.AreEqual(pokemon.Level, 30);

        if(generation >= 2)
        {
            Assert.AreEqual(pokemon.LevelMet, pokemon.Level);
        }

        // TODO: add iterator for move slots
        Assert.AreEqual(pokemon.Moves.Count, 4);
        for(int moveIndex = 0; moveIndex < pokemon.Moves.Count; ++moveIndex)
        {
            Assert.AreEqual(pokemon.Moves[moveIndex].Move, "None");
            Assert.AreEqual(pokemon.Moves[moveIndex].PP, 0);
        }

        Assert.IsTrue(System.IO.File.Exists(pokemon.IconFilepath));
        Assert.IsTrue(System.IO.File.Exists(pokemon.SpriteFilepath));

        Assert.AreEqual(pokemon.CurrentHP, pokemon.Stats["HP"]);
    }

    private static void CheckInitialMaps(
        PKMN.Pokemon2 pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        // EVs

        Assert.IsTrue(pokemon.EVs.ContainsKey("HP"));
        Assert.IsTrue(pokemon.EVs.ContainsKey("Attack"));
        Assert.IsTrue(pokemon.EVs.ContainsKey("Defense"));
        Assert.IsTrue(pokemon.EVs.ContainsKey("Speed"));
        if(generation >= 3)
        {
            Assert.IsFalse(pokemon.EVs.ContainsKey("Special"));
            Assert.IsTrue(pokemon.EVs.ContainsKey("Special Attack"));
            Assert.IsTrue(pokemon.EVs.ContainsKey("Special Defense"));
        }
        else
        {
            Assert.IsTrue(pokemon.EVs.ContainsKey("Special"));
            Assert.IsFalse(pokemon.EVs.ContainsKey("Special Attack"));
            Assert.IsFalse(pokemon.EVs.ContainsKey("Special Defense"));
        }
        foreach(string key in pokemon.EVs.Keys)
        {
            Assert.IsTrue(pokemon.EVs[key] >= 0);
            Assert.IsTrue(pokemon.EVs[key] <= ((generation >= 3) ? 255 : 65535));
        }

        // IVs

        Assert.IsTrue(pokemon.IVs.ContainsKey("HP"));
        Assert.IsTrue(pokemon.IVs.ContainsKey("Attack"));
        Assert.IsTrue(pokemon.IVs.ContainsKey("Defense"));
        Assert.IsTrue(pokemon.IVs.ContainsKey("Speed"));
        if(generation >= 3)
        {
            Assert.IsFalse(pokemon.IVs.ContainsKey("Special"));
            Assert.IsTrue(pokemon.IVs.ContainsKey("Special Attack"));
            Assert.IsTrue(pokemon.IVs.ContainsKey("Special Defense"));
        }
        else
        {
            Assert.IsTrue(pokemon.IVs.ContainsKey("Special"));
            Assert.IsFalse(pokemon.IVs.ContainsKey("Special Attack"));
            Assert.IsFalse(pokemon.IVs.ContainsKey("Special Defense"));
        }
        foreach(string key in pokemon.IVs.Keys)
        {
            Assert.IsTrue(pokemon.IVs[key] >= 0);
            Assert.IsTrue(pokemon.IVs[key] <= ((generation >= 3) ? 31 : 16));
        }

        // Stats

        Assert.IsTrue(pokemon.Stats.ContainsKey("HP"));
        Assert.IsTrue(pokemon.Stats.ContainsKey("Attack"));
        Assert.IsTrue(pokemon.Stats.ContainsKey("Defense"));
        Assert.IsTrue(pokemon.Stats.ContainsKey("Speed"));
        if(generation >= 3)
        {
            Assert.IsFalse(pokemon.Stats.ContainsKey("Special"));
            Assert.IsTrue(pokemon.Stats.ContainsKey("Special Attack"));
            Assert.IsTrue(pokemon.Stats.ContainsKey("Special Defense"));
        }
        else
        {
            Assert.IsTrue(pokemon.Stats.ContainsKey("Special"));
            Assert.IsFalse(pokemon.Stats.ContainsKey("Special Attack"));
            Assert.IsFalse(pokemon.Stats.ContainsKey("Special Defense"));
        }
        foreach(string key in pokemon.Stats.Keys)
        {
            Assert.IsTrue(pokemon.Stats[key] >= 0);
            Assert.IsTrue(pokemon.Stats[key] <= 65535);
        }

        if(generation >= 3)
        {
            // Markings

            Assert.IsTrue(pokemon.Markings.ContainsKey("Circle"));
            Assert.IsTrue(pokemon.Markings.ContainsKey("Triangle"));
            Assert.IsTrue(pokemon.Markings.ContainsKey("Square"));
            Assert.IsTrue(pokemon.Markings.ContainsKey("Heart"));
            if(generation >= 4)
            {
                Assert.IsTrue(pokemon.Markings.ContainsKey("Star"));
                Assert.IsTrue(pokemon.Markings.ContainsKey("Diamond"));
            }
            else
            {
                Assert.IsFalse(pokemon.Markings.ContainsKey("Star"));
                Assert.IsFalse(pokemon.Markings.ContainsKey("Diamond"));
            }
        }
    }

    static private void TestImageFilepaths(
        PKMN.Pokemon2 pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        Assert.IsTrue(System.IO.File.Exists(pokemon.IconFilepath));

        if(generation >= 2)
        {
            string[] genders = {"Male", "Female"};
            bool[] isShinyBools = {true, false};

            foreach(string gender in genders)
            {
                foreach(bool isShiny in isShinyBools)
                {
                    pokemon.Gender = gender;
                    pokemon.IsShiny = isShiny;
                    Assert.IsTrue(System.IO.File.Exists(pokemon.SpriteFilepath));
                }
            }
        }
        else
        {
            Assert.IsTrue(System.IO.File.Exists(pokemon.SpriteFilepath));
        }
    }

    static private void TestSettingAbility(
        PKMN.Pokemon2 pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 3)
        {
            Assert.AreNotEqual(pokemon.DatabaseEntry.Abilities.First, "None");

            pokemon.Ability = pokemon.DatabaseEntry.Abilities.First;
            Assert.AreEqual(
                pokemon.Ability,
                pokemon.DatabaseEntry.Abilities.First
            );

            if(!pokemon.DatabaseEntry.Abilities.Second.Equals("None"))
            {
                pokemon.Ability = pokemon.DatabaseEntry.Abilities.Second;
                Assert.AreEqual(
                    pokemon.Ability,
                    pokemon.DatabaseEntry.Abilities.Second
                );
            }

            if(generation >= 5)
            {
                pokemon.Ability = pokemon.DatabaseEntry.HiddenAbility;
                Assert.AreEqual(
                    pokemon.Ability,
                    pokemon.DatabaseEntry.HiddenAbility
                );
            }

            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    pokemon.Ability = "Not an ability";
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    pokemon.Ability = "Wonder Guard";
                }
            );
        }
        else
        {
            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.Ability = "Wonder Guard";
                }
            );
        }
    }

    static void TestSettingBall(
        PKMN.Pokemon2 pokemon,
        string ballName,
        string[] invalidBallNames
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 3)
        {
            pokemon.Ball = ballName;
            Assert.AreEqual(pokemon.Ball, ballName);

            foreach(string invalidBallName in invalidBallNames)
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        pokemon.Ball = invalidBallName;
                    }
                );
            }
        }
        else
        {
            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.Ball = "Great Ball";
                }
            );
        }
    }
}

}
