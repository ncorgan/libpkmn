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

public class PokemonTestCommon
{
    private static System.Random rng = new System.Random();

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
                new PKMN.Pokemon(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon(species, game, "Mega", 100);
                    }
                );
            }
        }
        foreach(string species in Gen1PokemonWithORASMegaForms)
        {
            if(isORAS)
            {
                new PKMN.Pokemon(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon(species, game, "Mega", 100);
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
                new PKMN.Pokemon("Pikachu", game, form, 5);
            }
            else if(!form.Equals("Standard"))
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon("Pikachu", game, form, 5);
                    }
                );
            }
        }

        // Hard-code Mega X/Y cases.
        if(generation >= 6)
        {
            new PKMN.Pokemon("Charizard", game, "Mega X", 50);
            new PKMN.Pokemon("Charizard", game, "Mega Y", 50);
            new PKMN.Pokemon("Mewtwo", game, "Mega X", 50);
            new PKMN.Pokemon("Mewtwo", game, "Mega Y", 50);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon("Charizard", game, "Mega X", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon("Charizard", game, "Mega Y", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon("Mewtwo", game, "Mega X", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon("Mewtwo", game, "Mega Y", 50);
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
                new PKMN.Pokemon(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon(species, game, "Mega", 100);
                    }
                );
            }
        }

        // Only one Generation II Pokémon with a OR/AS-specific Mega form.
        if(isORAS)
        {
            new PKMN.Pokemon("Steelix", game, "Mega", 100);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon("Steelix", game, "Mega", 100);
                }
            );
        }

        // Spiky-eared Pichu should only work in HG/SS.
        if(game.Equals("HeartGold") || game.Equals("SoulSilver"))
        {
            new PKMN.Pokemon("Pichu", game, "Spiky-eared", 100);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon("Pichu", game, "Spiky-eared", 100);
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
                    PKMN.Pokemon unown = new PKMN.Pokemon("Unown", game, form, 10);
                    Assert.IsTrue(System.IO.File.Exists(unown.IconFilepath));
                    Assert.IsTrue(System.IO.File.Exists(unown.SpriteFilepath));
                }
                else
                {
                    Assert.Throws<ArgumentOutOfRangeException>(
                        delegate
                        {
                            new PKMN.Pokemon("Unown", game, form, 10);
                        }
                    );
                }
            }
            else
            {
                PKMN.Pokemon unown = new PKMN.Pokemon("Unown", game, form, 10);
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
                new PKMN.Pokemon(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon(species, game, "Mega", 100);
                    }
                );
            }
        }
        foreach(string species in Gen3PokemonWithORASMegaForms)
        {
            if(isORAS)
            {
                new PKMN.Pokemon(species, game, "Mega", 100);
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon(species, game, "Mega", 100);
                    }
                );
            }
        }

        // Castform should always work.
        PKMN.Database.PokemonEntry castformEntry = new PKMN.Database.PokemonEntry("Castform", "Omega Ruby", "");
        foreach(string form in castformEntry.Forms)
        {
            PKMN.Pokemon castform = new PKMN.Pokemon("Castform", game, form, 30);
            Assert.IsTrue(System.IO.File.Exists(castform.IconFilepath));
            Assert.IsTrue(System.IO.File.Exists(castform.SpriteFilepath));
        }

        // Primal Reversion forms should only work in OR/AS.

        PKMN.Pokemon groudon = new PKMN.Pokemon("Groudon", game, "", 70);
        Assert.IsTrue(System.IO.File.Exists(groudon.IconFilepath));
        Assert.IsTrue(System.IO.File.Exists(groudon.SpriteFilepath));

        PKMN.Pokemon kyogre = new PKMN.Pokemon("Kyogre", game, "", 70);
        Assert.IsTrue(System.IO.File.Exists(kyogre.IconFilepath));
        Assert.IsTrue(System.IO.File.Exists(kyogre.SpriteFilepath));

        if(isORAS)
        {
            new PKMN.Pokemon("Groudon", game, "Primal Reversion", 70);
            new PKMN.Pokemon("Kyogre", game, "Primal Reversion", 70);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon("Groudon", game, "Primal Reversion", 70);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon("Kyogre", game, "Primal Reversion", 70);
                }
            );
        }

        // In Generation III, Deoxys's form is game-specific.
        if(generation == 3)
        {
            if(game.Equals("Ruby") || game.Equals("Sapphire") || game.Equals("Colosseum") || game.Equals("XD"))
            {
                PKMN.Pokemon deoxys = new PKMN.Pokemon("Deoxys", game, "Normal", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon("Deoxys", game, "Normal", 70);
                    }
                );
            }

            if(game.Equals("FireRed"))
            {
                PKMN.Pokemon deoxys = new PKMN.Pokemon("Deoxys", game, "Attack", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon("Deoxys", game, "Attack", 70);
                    }
                );
            }

            if(game.Equals("LeafGreen"))
            {
                PKMN.Pokemon deoxys = new PKMN.Pokemon("Deoxys", game, "Defense", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon("Deoxys", game, "Defense", 70);
                    }
                );
            }

            if(game.Equals("Emerald"))
            {
                PKMN.Pokemon deoxys = new PKMN.Pokemon("Deoxys", game, "Speed", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon("Deoxys", game, "Speed", 70);
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
                PKMN.Pokemon deoxys = new PKMN.Pokemon("Deoxys", game, form, 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
        }
    }

    public static void FormTest(
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
        PKMN.Pokemon nidorina = new PKMN.Pokemon(
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

        PKMN.Pokemon nidorino = new PKMN.Pokemon(
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

        PKMN.Pokemon magnemite = new PKMN.Pokemon(
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
            PKMN.Pokemon pokemon = new PKMN.Pokemon(
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

    private static void CheckInitialValues(
        PKMN.Pokemon pokemon
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
        Assert.AreEqual(pokemon.OriginalTrainerName, PKMN.Pokemon.DefaultTrainerName);

        if(generation >= 2)
        {
            Assert.AreEqual(pokemon.HeldItem, "None");
            Assert.AreEqual(pokemon.OriginalTrainerGender, "Male");
        }
        else
        {
            // Default value since a getter shouldn't throw an exception.
            Assert.AreEqual(pokemon.OriginalTrainerGender, "");
        }

        Assert.AreEqual(
            pokemon.OriginalTrainerPublicID,
            (ushort)(PKMN.Pokemon.DefaultTrainerID & 0xFFFF)
        );
        if(generation >= 3)
        {
            Assert.AreEqual(
                pokemon.OriginalTrainerSecretID,
                (ushort)(PKMN.Pokemon.DefaultTrainerID >> 16)
            );
            Assert.AreEqual(
                pokemon.OriginalTrainerID,
                PKMN.Pokemon.DefaultTrainerID
            );
        }
        else
        {
            Assert.AreEqual(
                pokemon.OriginalTrainerID,
                (PKMN.Pokemon.DefaultTrainerID & 0xFFFF)
            );

            // Default value since a getter shouldn't throw an exception.
            Assert.AreEqual(pokemon.OriginalTrainerSecretID, 0);
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
        else
        {
            // Default values since getters shouldn't throw exceptions.
            Assert.AreEqual(pokemon.Ball, "");
            Assert.AreEqual(pokemon.OriginalGame, "");
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
        PKMN.Pokemon pokemon
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
        PKMN.Pokemon pokemon
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
        PKMN.Pokemon pokemon
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
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(pokemon.Ability, "");

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.Ability = "Wonder Guard";
                }
            );
        }
    }

    static private void TestSettingBall(
        PKMN.Pokemon pokemon,
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
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(pokemon.Ball, "");

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.Ball = "Great Ball";
                }
            );
        }
    }

    static private void TestSettingCondition(
        PKMN.Pokemon pokemon
    )
    {
        System.Collections.Generic.List<string> conditions =
        new System.Collections.Generic.List<string>(
            new string[]{"None", "Asleep", "Poison", "Burn", "Frozen", "Paralysis"}
        );

        if(Util.GameToGeneration(pokemon.Game) >= 3)
        {
            conditions.Add("Bad Poison");
        }

        foreach(string condition in conditions)
        {
            pokemon.Condition = condition;
            Assert.AreEqual(pokemon.Condition, condition);
        }
    }

    static private void TestSettingFriendship(
        PKMN.Pokemon pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 2)
        {
            int testFriendship = rng.Next(0, 256);

            pokemon.CurrentTrainerFriendship = testFriendship;
            Assert.AreEqual(pokemon.CurrentTrainerFriendship, testFriendship);

            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    pokemon.CurrentTrainerFriendship = -1;
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    pokemon.CurrentTrainerFriendship = 256;
                }
            );
        }
        else
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(pokemon.CurrentTrainerFriendship, 0);

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.CurrentTrainerFriendship = 0;
                }
            );
        }
    }

    static private void TestSettingItem(
        PKMN.Pokemon pokemon,
        string validItemName,
        string[] invalidItemNames
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 2)
        {
            pokemon.HeldItem = validItemName;
            Assert.AreEqual(pokemon.HeldItem, validItemName);

            foreach(string invalidItemName in invalidItemNames)
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        pokemon.HeldItem = invalidItemName;
                    }
                );
            }
        }
        else
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(pokemon.HeldItem, "");

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.HeldItem = "Potion";
                }
            );
        }
    }

    static private void TestSettingLevels(
        PKMN.Pokemon pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.Level = -1;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.Level = 101;
            }
        );

        int level = 50;
        pokemon.Level = level;
        Assert.AreEqual(pokemon.Level, level);
        Assert.AreEqual(pokemon.DatabaseEntry.GetLevelAtExperience(pokemon.Experience), level);

        int experience = 123456;
        pokemon.Experience = experience;
        Assert.AreEqual(pokemon.Experience, experience);
        Assert.Less(pokemon.DatabaseEntry.GetExperienceAtLevel(pokemon.Level-1), experience);
        Assert.LessOrEqual(pokemon.DatabaseEntry.GetExperienceAtLevel(pokemon.Level), experience);

        switch(generation)
        {
            case 1:
                Assert.Throws<IndexOutOfRangeException>(
                    delegate
                    {
                        pokemon.Level = 1;
                    }
                );

                // The getter shouldn't throw by convention, but the setter will.

                Assert.AreEqual(pokemon.LevelMet, 0);

                Assert.Throws<ApplicationException>(
                    delegate
                    {
                        pokemon.LevelMet = 10;
                    }
                );
                break;

            case 2:
                pokemon.LevelMet = 5;
                Assert.AreEqual(pokemon.LevelMet, 5);

                Assert.Throws<IndexOutOfRangeException>(
                    delegate
                    {
                        pokemon.Level = 1;
                    }
                );
                Assert.Throws<IndexOutOfRangeException>(
                    delegate
                    {
                        pokemon.LevelMet = 101;
                    }
                );
                break;

            default:
                pokemon.LevelMet = 5;
                Assert.AreEqual(pokemon.LevelMet, 5);

                Assert.Throws<IndexOutOfRangeException>(
                    delegate
                    {
                        pokemon.Level = 0;
                    }
                );
                Assert.Throws<IndexOutOfRangeException>(
                    delegate
                    {
                        pokemon.LevelMet = 101;
                    }
                );
                break;
        }
    }

    static private void TestSettingLocationMet(
        PKMN.Pokemon pokemon,
        string expectedOriginalLocation,
        string[] validLocations,
        string[] invalidLocations
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        switch(generation)
        {
            case 1:
                // The getters shouldn't throw by convention, but the setters will.

                Assert.AreEqual(pokemon.LocationMet, "");
                Assert.AreEqual(pokemon.LocationMetAsEgg, "");

                Assert.Throws<ApplicationException>(
                    delegate
                    {
                        pokemon.LocationMet = validLocations[0];
                    }
                );
                Assert.Throws<ApplicationException>(
                    delegate
                    {
                        pokemon.LocationMetAsEgg = validLocations[0];
                    }
                );
                break;

            case 2:
            case 3:
                Assert.AreEqual(pokemon.LocationMet, expectedOriginalLocation);

                foreach(string validLocation in validLocations)
                {
                    pokemon.LocationMet = validLocation;
                    Assert.AreEqual(pokemon.LocationMet, validLocation);
                }
                foreach(string invalidLocation in invalidLocations)
                {
                    Assert.Throws<ArgumentOutOfRangeException>(
                        delegate
                        {
                            pokemon.LocationMet = invalidLocation;
                        }
                    );
                }

                // The getter shouldn't throw by convention, but the setter will.

                Assert.AreEqual(pokemon.LocationMetAsEgg, "");

                Assert.Throws<ApplicationException>(
                    delegate
                    {
                        pokemon.LocationMetAsEgg = validLocations[0];
                    }
                );
                break;

            default:
                Assert.AreEqual(pokemon.LocationMet, expectedOriginalLocation);
                Assert.AreEqual(pokemon.LocationMetAsEgg, expectedOriginalLocation);

                foreach(string validLocation in validLocations)
                {
                    pokemon.LocationMet = validLocation;
                    Assert.AreEqual(pokemon.LocationMet, validLocation);

                    pokemon.LocationMetAsEgg = validLocation;
                    Assert.AreEqual(pokemon.LocationMetAsEgg, validLocation);
                }
                foreach(string invalidLocation in invalidLocations)
                {
                    Assert.Throws<ArgumentOutOfRangeException>(
                        delegate
                        {
                            pokemon.LocationMet = invalidLocation;
                        }
                    );
                    Assert.Throws<ArgumentOutOfRangeException>(
                        delegate
                        {
                            pokemon.LocationMetAsEgg = invalidLocation;
                        }
                    );
                }
                break;
        }
    }

    static private void TestSettingMarkings(
        PKMN.Pokemon pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 3)
        {
            foreach(string marking in pokemon.Markings.Keys)
            {
                Assert.IsFalse(pokemon.Markings[marking]);
                pokemon.Markings[marking] = true;
                Assert.IsTrue(pokemon.Markings[marking]);
            }
        }
        else
        {
            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.Markings["Circle"] = true;
                }
            );
        }
    }

    static private void TestSettingMoves(
        PKMN.Pokemon pokemon,
        string[] validMoveNames,
        string[] invalidMoveNames
    )
    {
        Assert.AreEqual(validMoveNames.Length, 4);

        for(int moveIndex = 0; moveIndex < 4; ++moveIndex)
        {
            pokemon.Moves[moveIndex].Move = validMoveNames[moveIndex];
            Assert.AreEqual(pokemon.Moves[moveIndex].Move, validMoveNames[moveIndex]);

            PKMN.Database.MoveEntry validMoveEntry = new PKMN.Database.MoveEntry(
                                                             validMoveNames[moveIndex],
                                                             pokemon.Game
                                                         );

            pokemon.Moves[moveIndex].PP = 0;
            Assert.AreEqual(pokemon.Moves[moveIndex].PP, 0);

            int maxPP = validMoveEntry.GetPP(3);
            pokemon.Moves[moveIndex].PP = maxPP;
            Assert.AreEqual(pokemon.Moves[moveIndex].PP, maxPP);
        }

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.Moves[-1].Move = validMoveNames[0];
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.Moves[4].Move = validMoveNames[0];
            }
        );

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.Moves[-1].PP = 0;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.Moves[4].PP = 0;
            }
        );
    }

    static private void TestSettingOriginalGame(
        PKMN.Pokemon pokemon,
        string[] validGames,
        string[] invalidGames
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 3)
        {
            foreach(string validGame in validGames)
            {
                pokemon.OriginalGame = validGame;
                if(validGame.Equals("Colosseum") || validGame.Equals("XD"))
                {
                    Assert.AreEqual(pokemon.OriginalGame, "Colosseum/XD");
                }
                else
                {
                    Assert.AreEqual(pokemon.OriginalGame, validGame);
                }
            }

            foreach(string invalidGame in invalidGames)
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        pokemon.OriginalGame = invalidGame;
                    }
                );
            }
        }
        else
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(pokemon.OriginalGame, "");

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.OriginalGame = validGames[0];
                }
            );
        }
    }

    static private void TestSettingPersonality(
        PKMN.Pokemon pokemon
    )
    {
        uint testPersonality = (uint)rng.Next();

        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 3)
        {
            pokemon.Personality = testPersonality;
            Assert.AreEqual(pokemon.Personality, testPersonality);
        }
        else
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(pokemon.Personality, 0);

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.Personality = testPersonality;
                }
            );
        }
    }

    static private void TestSettingPokerus(
        PKMN.Pokemon pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 2)
        {
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    pokemon.PokerusDuration = -1;
                }
            );
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    pokemon.PokerusDuration = 16;
                }
            );

            int testPokerusDuration = rng.Next(1, 15);
            pokemon.PokerusDuration = testPokerusDuration;
            Assert.AreEqual(pokemon.PokerusDuration, testPokerusDuration);
        }
        else
        {
            // The getter shouldn't set by convention, but the setter will.

            Assert.AreEqual(pokemon.PokerusDuration, 0);

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.PokerusDuration = 1;
                }
            );
        }
    }

    static private void TestSettingStats(
        PKMN.Pokemon pokemon
    )
    {
        // Check bounds for setting the current HP.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.CurrentHP = -1;
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.CurrentHP = pokemon.Stats["HP"] + 1;
            }
        );

        pokemon.CurrentHP = 0;
        Assert.AreEqual(pokemon.CurrentHP, 0);

        pokemon.CurrentHP = pokemon.Stats["HP"];
        Assert.AreEqual(pokemon.CurrentHP, pokemon.Stats["HP"]);

        pokemon.CurrentHP = pokemon.Stats["HP"] - 1;
        Assert.AreEqual(pokemon.CurrentHP, pokemon.Stats["HP"] - 1);

        // Set the HP stat to lower than the current HP, and make sure
        // it's updated.

        int currentHP = pokemon.CurrentHP;
        pokemon.EVs["HP"] = 0;
        pokemon.IVs["HP"] = 0;
        Assert.LessOrEqual(pokemon.CurrentHP, currentHP);
    }

    static private void TestSettingTrainerInfo(
        PKMN.Pokemon pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                pokemon.Nickname = "";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                pokemon.Nickname = "Too long nickname";
            }
        );

        pokemon.Nickname = "foobarbaz";
        Assert.AreEqual(pokemon.Nickname, "foobarbaz");

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                pokemon.OriginalTrainerName = "";
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                pokemon.OriginalTrainerName = "Too long trainer name";
            }
        );

        pokemon.OriginalTrainerName = "foobar";
        Assert.AreEqual(pokemon.OriginalTrainerName, "foobar");

        if(generation >= 2)
        {
            pokemon.OriginalTrainerGender = "Male";
            Assert.AreEqual(pokemon.OriginalTrainerGender, "Male");

            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    pokemon.OriginalTrainerGender = "Genderless";
                }
            );
        }
        else
        {
            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.OriginalTrainerGender = "Male";
                }
            );
        }

        if(generation >= 3)
        {
            pokemon.OriginalTrainerID = 0x1234ABCD;
            Assert.AreEqual(pokemon.OriginalTrainerID, 0x1234ABCD);
            Assert.AreEqual(pokemon.OriginalTrainerPublicID, 0xABCD);
            Assert.AreEqual(pokemon.OriginalTrainerSecretID, 0x1234);

            pokemon.OriginalTrainerPublicID = 0x1A2B;
            Assert.AreEqual(pokemon.OriginalTrainerID, 0x12341A2B);
            Assert.AreEqual(pokemon.OriginalTrainerPublicID, 0x1A2B);
            Assert.AreEqual(pokemon.OriginalTrainerSecretID, 0x1234);

            pokemon.OriginalTrainerSecretID = 0x3C4D;
            Assert.AreEqual(pokemon.OriginalTrainerID, 0x3C4D1A2B);
            Assert.AreEqual(pokemon.OriginalTrainerPublicID, 0x1A2B);
            Assert.AreEqual(pokemon.OriginalTrainerSecretID, 0x3C4D);
        }
        else
        {
            Assert.Throws<IndexOutOfRangeException>(
                delegate
                {
                    pokemon.OriginalTrainerID = 0xFFFF+1;
                }
            );
            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.OriginalTrainerSecretID = 0xFFFF;
                }
            );

            pokemon.OriginalTrainerID = 0xABCD;
            Assert.AreEqual(pokemon.OriginalTrainerID, 0xABCD);
            Assert.AreEqual(pokemon.OriginalTrainerPublicID, 0xABCD);

            pokemon.OriginalTrainerPublicID = 0x9876;
            Assert.AreEqual(pokemon.OriginalTrainerID, 0x9876);
            Assert.AreEqual(pokemon.OriginalTrainerPublicID, 0x9876);
        }
    }

    static public void TestCommon(
        PKMN.Pokemon pokemon,
        PokemonTestParams testParams
    )
    {
        CheckInitialValues(pokemon);
        TestSettingAbility(pokemon);
        TestSettingBall(
            pokemon,
            testParams.ValidBall,
            testParams.InvalidBalls
        );
        TestSettingCondition(pokemon);
        TestImageFilepaths(pokemon);
        TestSettingFriendship(pokemon);
        TestSettingItem(
            pokemon,
            testParams.ValidItem,
            testParams.InvalidItems
        );
        TestSettingLevels(pokemon);
        TestSettingLocationMet(
            pokemon,
            testParams.ExpectedOriginalLocation,
            testParams.ValidLocations,
            testParams.InvalidLocations
        );
        TestSettingMarkings(pokemon);
        TestSettingMoves(
            pokemon,
            testParams.ValidMoves,
            testParams.InvalidMoves
        );
        TestSettingOriginalGame(
            pokemon,
            testParams.ValidOriginalGames,
            testParams.InvalidOriginalGames
        );
        TestSettingPersonality(pokemon);
        TestSettingPokerus(pokemon);
        TestSettingStats(pokemon);
        TestSettingTrainerInfo(pokemon);
    }
}

}
