/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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

    private static PKMN.Species[] Gen1PokemonWithXYMegaForms =
    {
        PKMN.Species.VENUSAUR,
        PKMN.Species.BLASTOISE,
        PKMN.Species.ALAKAZAM,
        PKMN.Species.GENGAR,
        PKMN.Species.KANGASKHAN,
        PKMN.Species.PINSIR,
        PKMN.Species.GYARADOS,
        PKMN.Species.AERODACTYL
    };
    private static PKMN.Species[] Gen1PokemonWithORASMegaForms =
    {
        PKMN.Species.BEEDRILL,
        PKMN.Species.PIDGEOT,
        PKMN.Species.SLOWBRO
    };

    private static PKMN.Species[] Gen2PokemonWithXYMegaForms =
    {
        PKMN.Species.AMPHAROS,
        PKMN.Species.SCIZOR,
        PKMN.Species.HERACROSS,
        PKMN.Species.HOUNDOOM,
        PKMN.Species.TYRANITAR
    };

    private static PKMN.Species[] Gen3PokemonWithXYMegaForms =
    {
        PKMN.Species.BLAZIKEN,
        PKMN.Species.GARDEVOIR,
        PKMN.Species.MAWILE,
        PKMN.Species.AGGRON,
        PKMN.Species.MEDICHAM,
        PKMN.Species.MANECTRIC,
        PKMN.Species.BANETTE,
        PKMN.Species.ABSOL,
        PKMN.Species.LATIAS,
        PKMN.Species.LATIOS,
        PKMN.Species.GARCHOMP,
    };
    private static PKMN.Species[] Gen3PokemonWithORASMegaForms =
    {
        PKMN.Species.SCEPTILE,
        PKMN.Species.SWAMPERT,
        PKMN.Species.SABLEYE,
        PKMN.Species.SHARPEDO,
        PKMN.Species.CAMERUPT,
        PKMN.Species.ALTARIA,
        PKMN.Species.GLALIE,
        PKMN.Species.SALAMENCE,
        PKMN.Species.METAGROSS,
        PKMN.Species.RAYQUAZA,
    };

    private static void TestGen1Forms(
        PKMN.Game game
    )
    {
        int generation = Util.GameToGeneration(game);
        bool isORAS = (game == PKMN.Game.OMEGA_RUBY) || (game == PKMN.Game.ALPHA_SAPPHIRE);

        // Check that Mega forms only work in their given games.
        foreach(PKMN.Species species in Gen1PokemonWithXYMegaForms)
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
        foreach(PKMN.Species species in Gen1PokemonWithORASMegaForms)
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
        PKMN.Database.PokemonEntry cosplayPikachu = new PKMN.Database.PokemonEntry(
                                                            PKMN.Species.PIKACHU,
                                                            game,
                                                            ""
                                                        );
        foreach(string form in cosplayPikachu.Forms)
        {
            if(isORAS)
            {
                new PKMN.Pokemon(PKMN.Species.PIKACHU, game, form, 5);
            }
            else if(!form.Equals("Standard"))
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon(PKMN.Species.PIKACHU, game, form, 5);
                    }
                );
            }
        }

        // Hard-code Mega X/Y cases.
        if(generation >= 6)
        {
            new PKMN.Pokemon(PKMN.Species.CHARIZARD, game, "Mega X", 50);
            new PKMN.Pokemon(PKMN.Species.CHARIZARD, game, "Mega Y", 50);
            new PKMN.Pokemon(PKMN.Species.MEWTWO, game, "Mega X", 50);
            new PKMN.Pokemon(PKMN.Species.MEWTWO, game, "Mega Y", 50);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon(PKMN.Species.CHARIZARD, game, "Mega X", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon(PKMN.Species.CHARIZARD, game, "Mega Y", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon(PKMN.Species.MEWTWO, game, "Mega X", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon(PKMN.Species.MEWTWO, game, "Mega Y", 50);
                }
            );
        }
    }

    private static void TestGen2Forms(
        PKMN.Game game
    )
    {
        int generation = Util.GameToGeneration(game);
        bool isHGSS = (game == PKMN.Game.HEARTGOLD) || (game == PKMN.Game.SOULSILVER);
        bool isORAS = (game == PKMN.Game.OMEGA_RUBY) || (game == PKMN.Game.ALPHA_SAPPHIRE);

        // Check that Mega forms only work in their given games.
        foreach(PKMN.Species species in Gen2PokemonWithXYMegaForms)
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
            new PKMN.Pokemon(PKMN.Species.STEELIX, game, "Mega", 100);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon(PKMN.Species.STEELIX, game, "Mega", 100);
                }
            );
        }

        // Spiky-eared Pichu should only work in HG/SS.
        if(isHGSS)
        {
            new PKMN.Pokemon(PKMN.Species.PICHU, game, "Spiky-eared", 100);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon(PKMN.Species.PICHU, game, "Spiky-eared", 100);
                }
            );
        }

        // Unown's "!" and "?" forms aren't in Generation II.
        PKMN.Database.PokemonEntry unownEntry = new PKMN.Database.PokemonEntry(
                                                        PKMN.Species.UNOWN,
                                                        PKMN.Game.OMEGA_RUBY,
                                                        ""
                                                    );
        foreach(string form in unownEntry.Forms)
        {
            if(form.Equals("!") || form.Equals("?"))
            {
                if(generation > 2)
                {
                    PKMN.Pokemon unown = new PKMN.Pokemon(PKMN.Species.UNOWN, game, form, 10);
                    Assert.IsTrue(System.IO.File.Exists(unown.IconFilepath));
                    Assert.IsTrue(System.IO.File.Exists(unown.SpriteFilepath));
                }
                else
                {
                    Assert.Throws<ArgumentOutOfRangeException>(
                        delegate
                        {
                            new PKMN.Pokemon(PKMN.Species.UNOWN, game, form, 10);
                        }
                    );
                }
            }
            else
            {
                PKMN.Pokemon unown = new PKMN.Pokemon(PKMN.Species.UNOWN, game, form, 10);
                Assert.IsTrue(System.IO.File.Exists(unown.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(unown.SpriteFilepath));
            }
        }
    }

    private static void TestGen3Forms(
        PKMN.Game game,
        bool gcn
    )
    {
        int generation = Util.GameToGeneration(game);
        bool isORAS = (game == PKMN.Game.OMEGA_RUBY) || (game == PKMN.Game.ALPHA_SAPPHIRE);

        // Check that Mega forms only work in their given games.
        foreach(PKMN.Species species in Gen3PokemonWithXYMegaForms)
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
        foreach(PKMN.Species species in Gen3PokemonWithORASMegaForms)
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
        PKMN.Database.PokemonEntry castformEntry = new PKMN.Database.PokemonEntry(
                                                           PKMN.Species.CASTFORM,
                                                           PKMN.Game.OMEGA_RUBY,
                                                           ""
                                                       );
        foreach(string form in castformEntry.Forms)
        {
            PKMN.Pokemon castform = new PKMN.Pokemon(PKMN.Species.CASTFORM, game, form, 30);
            Assert.IsTrue(System.IO.File.Exists(castform.IconFilepath));
            Assert.IsTrue(System.IO.File.Exists(castform.SpriteFilepath));
        }

        // Primal Reversion forms should only work in OR/AS.

        PKMN.Pokemon groudon = new PKMN.Pokemon(PKMN.Species.GROUDON, game, "", 70);
        Assert.IsTrue(System.IO.File.Exists(groudon.IconFilepath));
        Assert.IsTrue(System.IO.File.Exists(groudon.SpriteFilepath));

        PKMN.Pokemon kyogre = new PKMN.Pokemon(PKMN.Species.KYOGRE, game, "", 70);
        Assert.IsTrue(System.IO.File.Exists(kyogre.IconFilepath));
        Assert.IsTrue(System.IO.File.Exists(kyogre.SpriteFilepath));

        if(isORAS)
        {
            new PKMN.Pokemon(PKMN.Species.GROUDON, game, "Primal Reversion", 70);
            new PKMN.Pokemon(PKMN.Species.KYOGRE, game, "Primal Reversion", 70);
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon(PKMN.Species.GROUDON, game, "Primal Reversion", 70);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon(PKMN.Species.KYOGRE, game, "Primal Reversion", 70);
                }
            );
        }

        // In Generation III, Deoxys's form is game-specific.
        if(generation == 3)
        {
            if((game == PKMN.Game.RUBY) || (game == PKMN.Game.SAPPHIRE) || (game == PKMN.Game.COLOSSEUM) || (game == PKMN.Game.XD))
            {
                PKMN.Pokemon deoxys = new PKMN.Pokemon(PKMN.Species.DEOXYS, game, "Normal", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon(PKMN.Species.DEOXYS, game, "Normal", 70);
                    }
                );
            }

            if(game == PKMN.Game.FIRERED)
            {
                PKMN.Pokemon deoxys = new PKMN.Pokemon(PKMN.Species.DEOXYS, game, "Attack", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon(PKMN.Species.DEOXYS, game, "Attack", 70);
                    }
                );
            }

            if(game == PKMN.Game.LEAFGREEN)
            {
                PKMN.Pokemon deoxys = new PKMN.Pokemon(PKMN.Species.DEOXYS, game, "Defense", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon(PKMN.Species.DEOXYS, game, "Defense", 70);
                    }
                );
            }

            if(game == PKMN.Game.EMERALD)
            {
                PKMN.Pokemon deoxys = new PKMN.Pokemon(PKMN.Species.DEOXYS, game, "Speed", 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
            else
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        new PKMN.Pokemon(PKMN.Species.DEOXYS, game, "Speed", 70);
                    }
                );
            }
        }
        else
        {
            // Past Generation III, Deoxys's form can be switched.
            PKMN.Database.PokemonEntry deoxysEntry = new PKMN.Database.PokemonEntry(
                                                             PKMN.Species.DEOXYS,
                                                             PKMN.Game.OMEGA_RUBY,
                                                             ""
                                                         );
            foreach(string form in deoxysEntry.Forms)
            {
                PKMN.Pokemon deoxys = new PKMN.Pokemon(PKMN.Species.DEOXYS, game, form, 70);
                Assert.IsTrue(System.IO.File.Exists(deoxys.IconFilepath));
                Assert.IsTrue(System.IO.File.Exists(deoxys.SpriteFilepath));
            }
        }
    }

    public static void FormTest(
        PKMN.Game game
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
                ((game == PKMN.Game.COLOSSEUM) || (game == PKMN.Game.XD))
            );
        }
    }

    public static void GenderTest(
        PKMN.Game game
    ) {

        // Single-gender
        PKMN.Pokemon nidorina = new PKMN.Pokemon(
                                        PKMN.Species.NIDORINA,
                                        game,
                                        "",
                                        50
                                    );
        Assert.AreEqual(PKMN.Gender.FEMALE, nidorina.Gender);
        nidorina.Gender = PKMN.Gender.FEMALE;
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                nidorina.Gender = PKMN.Gender.MALE;
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                nidorina.Gender = PKMN.Gender.GENDERLESS;
            }
        );

        PKMN.Pokemon nidorino = new PKMN.Pokemon(
                                        PKMN.Species.NIDORINO,
                                        game,
                                        "",
                                        50
                                    );
        Assert.AreEqual(PKMN.Gender.MALE, nidorino.Gender);
        nidorino.Gender = PKMN.Gender.MALE;
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                nidorino.Gender = PKMN.Gender.FEMALE;
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                nidorino.Gender = PKMN.Gender.GENDERLESS;
            }
        );

        PKMN.Pokemon magnemite = new PKMN.Pokemon(
                                         PKMN.Species.MAGNEMITE,
                                         game,
                                         "",
                                         50
                                     );
        Assert.AreEqual(PKMN.Gender.GENDERLESS, magnemite.Gender);
        magnemite.Gender = PKMN.Gender.GENDERLESS;
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                magnemite.Gender = PKMN.Gender.MALE;
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                magnemite.Gender = PKMN.Gender.FEMALE;
            }
        );

        PKMN.Species[] mixedGenderPokemon =
        {
            PKMN.Species.CHARMANDER, // 87.5% male
            PKMN.Species.GROWLITHE,  // 75% male
            PKMN.Species.PIDGEY,     // 50% male
            PKMN.Species.VULPIX      // 25% male
        };
        foreach(PKMN.Species species in mixedGenderPokemon)
        {
            PKMN.Pokemon pokemon = new PKMN.Pokemon(
                                           species,
                                           game,
                                           "",
                                           50
                                       );

            pokemon.Gender = PKMN.Gender.FEMALE;
            Assert.AreEqual(PKMN.Gender.FEMALE, pokemon.Gender);
            pokemon.Gender = PKMN.Gender.MALE;
            Assert.AreEqual(PKMN.Gender.MALE, pokemon.Gender);
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    pokemon.Gender = PKMN.Gender.GENDERLESS;
                }
            );
        }
    }

    private static void CheckInitialValues(
        PKMN.Pokemon pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        Assert.AreEqual("None", pokemon.Condition);

        if(generation >= 5)
        {
            Assert.AreEqual(
                PKMN.PKMN.SpeciesToString(pokemon.Species),
                pokemon.Nickname
            );
        }
        else
        {
            Assert.AreEqual(
                PKMN.PKMN.SpeciesToString(pokemon.Species).ToUpper(),
                pokemon.Nickname
            );
        }
        Assert.AreEqual(PKMN.Pokemon.DefaultTrainerName, pokemon.OriginalTrainerName);

        Assert.AreEqual(PKMN.Item.NONE, pokemon.HeldItem);

        if(generation >= 2)
        {
            Assert.AreEqual(PKMN.Gender.MALE, pokemon.OriginalTrainerGender);
        }
        else
        {
            Assert.AreEqual(PKMN.Gender.NONE, pokemon.OriginalTrainerGender);
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
            Assert.AreEqual(0, pokemon.OriginalTrainerSecretID);
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
            Assert.AreEqual(PKMN.Ball.POKE_BALL, pokemon.Ball);

            // There is no distinction between Colosseum and XD in the game
            // storage.
            if((pokemon.Game == PKMN.Game.COLOSSEUM) || (pokemon.Game == PKMN.Game.XD))
            {
                Assert.AreEqual(
                    pokemon.OriginalGame,
                    PKMN.Game.COLOSSEUM
                );
            }
            else
            {
                Assert.AreEqual(pokemon.Game, pokemon.OriginalGame);
            }
        }
        else
        {
            // Default values since getters shouldn't throw exceptions.
            Assert.AreEqual(PKMN.Ball.NONE, pokemon.Ball);
            Assert.AreEqual(PKMN.Game.NONE, pokemon.OriginalGame);
        }

        Assert.AreEqual(
            pokemon.Experience,
            pokemon.DatabaseEntry.GetExperienceAtLevel(pokemon.Level)
        );
        Assert.AreEqual(30, pokemon.Level);

        if(generation >= 2)
        {
            Assert.AreEqual(pokemon.Level, pokemon.LevelMet);
        }

        Assert.AreEqual(4, pokemon.Moves.Count);

        Assert.IsTrue(System.IO.File.Exists(pokemon.IconFilepath));
        Assert.IsTrue(System.IO.File.Exists(pokemon.SpriteFilepath));

        Assert.AreEqual(pokemon.Stats[PKMN.Stat.HP], pokemon.CurrentHP);
    }

    private static void CheckInitialMaps(
        PKMN.Pokemon pokemon
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        // EVs

        Assert.IsTrue(pokemon.EVs.ContainsKey(PKMN.Stat.HP));
        Assert.IsTrue(pokemon.EVs.ContainsKey(PKMN.Stat.ATTACK));
        Assert.IsTrue(pokemon.EVs.ContainsKey(PKMN.Stat.DEFENSE));
        Assert.IsTrue(pokemon.EVs.ContainsKey(PKMN.Stat.SPEED));
        if(generation >= 3)
        {
            Assert.IsFalse(pokemon.EVs.ContainsKey(PKMN.Stat.SPECIAL));
            Assert.IsTrue(pokemon.EVs.ContainsKey(PKMN.Stat.SPECIAL_ATTACK));
            Assert.IsTrue(pokemon.EVs.ContainsKey(PKMN.Stat.SPECIAL_DEFENSE));
        }
        else
        {
            Assert.IsTrue(pokemon.EVs.ContainsKey(PKMN.Stat.SPECIAL));
            Assert.IsFalse(pokemon.EVs.ContainsKey(PKMN.Stat.SPECIAL_ATTACK));
            Assert.IsFalse(pokemon.EVs.ContainsKey(PKMN.Stat.SPECIAL_DEFENSE));
        }
        foreach(PKMN.Stat key in pokemon.EVs.Keys)
        {
            Assert.AreEqual(pokemon.EVs[key], 0);
        }

        // IVs

        Assert.IsTrue(pokemon.IVs.ContainsKey(PKMN.Stat.HP));
        Assert.IsTrue(pokemon.IVs.ContainsKey(PKMN.Stat.ATTACK));
        Assert.IsTrue(pokemon.IVs.ContainsKey(PKMN.Stat.DEFENSE));
        Assert.IsTrue(pokemon.IVs.ContainsKey(PKMN.Stat.SPEED));
        if(generation >= 3)
        {
            Assert.IsFalse(pokemon.IVs.ContainsKey(PKMN.Stat.SPECIAL));
            Assert.IsTrue(pokemon.IVs.ContainsKey(PKMN.Stat.SPECIAL_ATTACK));
            Assert.IsTrue(pokemon.IVs.ContainsKey(PKMN.Stat.SPECIAL_DEFENSE));
        }
        else
        {
            Assert.IsTrue(pokemon.IVs.ContainsKey(PKMN.Stat.SPECIAL));
            Assert.IsFalse(pokemon.IVs.ContainsKey(PKMN.Stat.SPECIAL_ATTACK));
            Assert.IsFalse(pokemon.IVs.ContainsKey(PKMN.Stat.SPECIAL_DEFENSE));
        }
        foreach(PKMN.Stat key in pokemon.IVs.Keys)
        {
            Assert.IsTrue(pokemon.IVs[key] >= 0);
            Assert.IsTrue(pokemon.IVs[key] <= ((generation >= 3) ? 31 : 15));
        }

        // Stats

        Assert.IsTrue(pokemon.Stats.ContainsKey(PKMN.Stat.HP));
        Assert.IsTrue(pokemon.Stats.ContainsKey(PKMN.Stat.ATTACK));
        Assert.IsTrue(pokemon.Stats.ContainsKey(PKMN.Stat.DEFENSE));
        Assert.IsTrue(pokemon.Stats.ContainsKey(PKMN.Stat.SPEED));
        if(generation >= 2)
        {
            Assert.IsFalse(pokemon.Stats.ContainsKey(PKMN.Stat.SPECIAL));
            Assert.IsTrue(pokemon.Stats.ContainsKey(PKMN.Stat.SPECIAL_ATTACK));
            Assert.IsTrue(pokemon.Stats.ContainsKey(PKMN.Stat.SPECIAL_DEFENSE));
        }
        else
        {
            Assert.IsTrue(pokemon.Stats.ContainsKey(PKMN.Stat.SPECIAL));
            Assert.IsFalse(pokemon.Stats.ContainsKey(PKMN.Stat.SPECIAL_ATTACK));
            Assert.IsFalse(pokemon.Stats.ContainsKey(PKMN.Stat.SPECIAL_DEFENSE));
        }
        foreach(PKMN.Stat key in pokemon.Stats.Keys)
        {
            Assert.IsTrue(pokemon.Stats[key] >= 0);
            Assert.IsTrue(pokemon.Stats[key] <= 65535);
        }

        if(generation >= 3)
        {
            // Contest Stats

            Assert.IsTrue(pokemon.ContestStats.ContainsKey(PKMN.ContestStat.COOL));
            Assert.IsTrue(pokemon.ContestStats.ContainsKey(PKMN.ContestStat.BEAUTY));
            Assert.IsTrue(pokemon.ContestStats.ContainsKey(PKMN.ContestStat.COOL));
            Assert.IsTrue(pokemon.ContestStats.ContainsKey(PKMN.ContestStat.SMART));
            Assert.IsTrue(pokemon.ContestStats.ContainsKey(PKMN.ContestStat.TOUGH));
            if(generation == 3)
            {
                Assert.IsTrue(pokemon.ContestStats.ContainsKey(PKMN.ContestStat.FEEL));
                Assert.IsFalse(pokemon.ContestStats.ContainsKey(PKMN.ContestStat.SHEEN));
            }
            else
            {
                Assert.IsFalse(pokemon.ContestStats.ContainsKey(PKMN.ContestStat.FEEL));
                Assert.IsTrue(pokemon.ContestStats.ContainsKey(PKMN.ContestStat.SHEEN));
            }
            foreach(PKMN.ContestStat contestStat in pokemon.ContestStats.Keys)
            {
                Assert.AreEqual(0, pokemon.ContestStats[contestStat]);
            }

            // Markings

            Assert.IsTrue(pokemon.Markings.ContainsKey(PKMN.Marking.CIRCLE));
            Assert.IsTrue(pokemon.Markings.ContainsKey(PKMN.Marking.TRIANGLE));
            Assert.IsTrue(pokemon.Markings.ContainsKey(PKMN.Marking.SQUARE));
            Assert.IsTrue(pokemon.Markings.ContainsKey(PKMN.Marking.HEART));
            if(generation >= 4)
            {
                Assert.IsTrue(pokemon.Markings.ContainsKey(PKMN.Marking.STAR));
                Assert.IsTrue(pokemon.Markings.ContainsKey(PKMN.Marking.DIAMOND));
            }
            else
            {
                Assert.IsFalse(pokemon.Markings.ContainsKey(PKMN.Marking.STAR));
                Assert.IsFalse(pokemon.Markings.ContainsKey(PKMN.Marking.DIAMOND));
            }
            foreach(PKMN.Marking marking in pokemon.Markings.Keys)
            {
                Assert.IsFalse(pokemon.Markings[marking]);
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
            PKMN.Gender[] genders = {PKMN.Gender.MALE, PKMN.Gender.FEMALE};
            bool[] isShinyBools = {true, false};

            foreach(PKMN.Gender gender in genders)
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
            Assert.AreNotEqual(pokemon.DatabaseEntry.Abilities.First, PKMN.Ability.NONE);

            pokemon.Ability = pokemon.DatabaseEntry.Abilities.First;
            Assert.AreEqual(
                pokemon.Ability,
                pokemon.DatabaseEntry.Abilities.First
            );

            if(pokemon.DatabaseEntry.Abilities.Second != PKMN.Ability.NONE)
            {
                pokemon.Ability = pokemon.DatabaseEntry.Abilities.Second;
                Assert.AreEqual(
                    pokemon.Ability,
                    pokemon.DatabaseEntry.Abilities.Second
                );
            }

            if(generation >= 5)
            {
                if(!pokemon.DatabaseEntry.HiddenAbility.Equals(PKMN.Ability.NONE))
                {
                    pokemon.Ability = pokemon.DatabaseEntry.HiddenAbility;
                    Assert.AreEqual(
                        pokemon.Ability,
                        pokemon.DatabaseEntry.HiddenAbility
                    );
                }
            }

            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    pokemon.Ability = PKMN.Ability.NONE;
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    pokemon.Ability = PKMN.Ability.WONDER_GUARD;
                }
            );
        }
        else
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(PKMN.Ability.NONE, pokemon.Ability);

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.Ability = PKMN.Ability.WONDER_GUARD;
                }
            );
        }
    }

    static private void TestSettingBall(
        PKMN.Pokemon pokemon,
        PKMN.Ball ball,
        PKMN.Ball[] invalidBalls
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 3)
        {
            pokemon.Ball = ball;
            Assert.AreEqual(ball, pokemon.Ball);

            foreach(PKMN.Ball invalidBall in invalidBalls)
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        pokemon.Ball = invalidBall;
                    }
                );
            }
        }
        else
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(PKMN.Ball.NONE, pokemon.Ball);

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.Ball = PKMN.Ball.GREAT_BALL;
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
            Assert.AreEqual(condition, pokemon.Condition);
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
            Assert.AreEqual(testFriendship, pokemon.CurrentTrainerFriendship);

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

            Assert.AreEqual(0, pokemon.CurrentTrainerFriendship);

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
        PKMN.Item validItem,
        PKMN.Item[] invalidItems
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 2)
        {
            pokemon.HeldItem = validItem;
            Assert.AreEqual(validItem, pokemon.HeldItem);

            foreach(PKMN.Item invalidItem in invalidItems)
            {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate
                    {
                        pokemon.HeldItem = invalidItem;
                    }
                );
            }
        }
        else
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(PKMN.Item.NONE, pokemon.HeldItem);

            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.HeldItem = PKMN.Item.POTION;
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
        Assert.AreEqual(level, pokemon.Level);
        Assert.AreEqual(level, pokemon.DatabaseEntry.GetLevelAtExperience(pokemon.Experience));

        int experience = 123456;
        pokemon.Experience = experience;
        Assert.AreEqual(experience, pokemon.Experience);
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

                Assert.AreEqual(0, pokemon.LevelMet);

                Assert.Throws<ApplicationException>(
                    delegate
                    {
                        pokemon.LevelMet = 10;
                    }
                );
                break;

            case 2:
                pokemon.LevelMet = 5;
                Assert.AreEqual(5, pokemon.LevelMet);

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
                Assert.AreEqual(5, pokemon.LevelMet);

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

                Assert.AreEqual("", pokemon.LocationMet);
                Assert.AreEqual("", pokemon.LocationMetAsEgg);

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
                Assert.AreEqual(expectedOriginalLocation, pokemon.LocationMet);

                foreach(string validLocation in validLocations)
                {
                    pokemon.LocationMet = validLocation;
                    Assert.AreEqual(validLocation, pokemon.LocationMet);
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

                Assert.AreEqual("", pokemon.LocationMetAsEgg);

                Assert.Throws<ApplicationException>(
                    delegate
                    {
                        pokemon.LocationMetAsEgg = validLocations[0];
                    }
                );
                break;

            default:
                Assert.AreEqual(expectedOriginalLocation, pokemon.LocationMet);
                Assert.AreEqual(expectedOriginalLocation, pokemon.LocationMetAsEgg);

                foreach(string validLocation in validLocations)
                {
                    pokemon.LocationMet = validLocation;
                    Assert.AreEqual(validLocation, pokemon.LocationMet);

                    pokemon.LocationMetAsEgg = validLocation;
                    Assert.AreEqual(validLocation, pokemon.LocationMetAsEgg);
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
            foreach(PKMN.Marking marking in pokemon.Markings.Keys)
            {
                Assert.IsFalse(pokemon.Markings[marking]);
                pokemon.Markings[marking] = true;
                Assert.IsTrue(pokemon.Markings[marking]);
            }
        }
        else
        {
            Assert.AreEqual(0, pokemon.Markings.Count);
            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.Markings[PKMN.Marking.CIRCLE] = true;
                }
            );
        }
    }

    static private void TestSettingMoves(
        PKMN.Pokemon pokemon,
        PKMN.Move[] validMoves,
        PKMN.Move[] invalidMoves
    )
    {
        Assert.AreEqual(4, validMoves.Length);

        for(int moveIndex = 0; moveIndex < 4; ++moveIndex)
        {
            pokemon.Moves[moveIndex].Move = validMoves[moveIndex];
            Assert.AreEqual(validMoves[moveIndex], pokemon.Moves[moveIndex].Move);

            PKMN.Database.MoveEntry validMoveEntry = new PKMN.Database.MoveEntry(
                                                             validMoves[moveIndex],
                                                             pokemon.Game
                                                         );

            pokemon.Moves[moveIndex].PP = 0;
            Assert.AreEqual(0, pokemon.Moves[moveIndex].PP);

            int maxPP = validMoveEntry.GetPP(3);
            pokemon.Moves[moveIndex].PP = maxPP;
            Assert.AreEqual(maxPP, pokemon.Moves[moveIndex].PP);
        }
        foreach(PKMN.Move invalidMove in invalidMoves)
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    pokemon.Moves[0].Move = invalidMove;
                }
            );
        }

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.Moves[-1].Move = validMoves[0];
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                pokemon.Moves[4].Move = validMoves[0];
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
        PKMN.Game[] validGames,
        PKMN.Game[] invalidGames
    )
    {
        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 3)
        {
            foreach(PKMN.Game validGame in validGames)
            {
                pokemon.OriginalGame = validGame;
                if((validGame == PKMN.Game.COLOSSEUM) || (validGame == PKMN.Game.XD))
                {
                    Assert.AreEqual(PKMN.Game.COLOSSEUM, pokemon.OriginalGame);
                }
                else
                {
                    Assert.AreEqual(validGame, pokemon.OriginalGame);
                }
            }

            foreach(PKMN.Game invalidGame in invalidGames)
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

            Assert.AreEqual(PKMN.Game.NONE, pokemon.OriginalGame);

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
            Assert.AreEqual(testPersonality, pokemon.Personality);
        }
        else
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(0, pokemon.Personality);

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
            Assert.AreEqual(testPokerusDuration, pokemon.PokerusDuration);
        }
        else
        {
            // The getter shouldn't throw by convention, but the setter will.

            Assert.AreEqual(0, pokemon.PokerusDuration);

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
                pokemon.CurrentHP = pokemon.Stats[PKMN.Stat.HP] + 1;
            }
        );

        pokemon.CurrentHP = 0;
        Assert.AreEqual(0, pokemon.CurrentHP);

        pokemon.CurrentHP = pokemon.Stats[PKMN.Stat.HP] - 1;
        Assert.AreEqual(pokemon.Stats[PKMN.Stat.HP] - 1, pokemon.CurrentHP);

        pokemon.CurrentHP = pokemon.Stats[PKMN.Stat.HP];
        Assert.AreEqual(pokemon.Stats[PKMN.Stat.HP], pokemon.CurrentHP);

        // Set the HP stat to lower than the current HP, and make sure
        // it's updated.

        int currentHP = pokemon.CurrentHP;
        pokemon.EVs[PKMN.Stat.HP] = 0;
        pokemon.IVs[PKMN.Stat.HP] = 0;
        Assert.LessOrEqual(pokemon.CurrentHP, currentHP);

        int generation = Util.GameToGeneration(pokemon.Game);

        if(generation >= 3)
        {
            foreach(PKMN.ContestStat contestStat in pokemon.ContestStats.Keys)
            {
                int statValue = rng.Next(0, 256);
                pokemon.ContestStats[contestStat] = statValue;
                Assert.AreEqual(statValue, pokemon.ContestStats[contestStat]);
            }
        }
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
        Assert.AreEqual("foobarbaz", pokemon.Nickname);

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
        Assert.AreEqual("foobar", pokemon.OriginalTrainerName);

        if(generation >= 2)
        {
            pokemon.OriginalTrainerGender = PKMN.Gender.MALE;
            Assert.AreEqual(PKMN.Gender.MALE, pokemon.OriginalTrainerGender);

            pokemon.OriginalTrainerGender = PKMN.Gender.FEMALE;
            Assert.AreEqual(PKMN.Gender.FEMALE, pokemon.OriginalTrainerGender);

            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    pokemon.OriginalTrainerGender = PKMN.Gender.GENDERLESS;
                }
            );
        }
        else
        {
            Assert.Throws<ApplicationException>(
                delegate
                {
                    pokemon.OriginalTrainerGender = PKMN.Gender.MALE;
                }
            );
        }

        if(generation >= 3)
        {
            pokemon.OriginalTrainerID = 0x1234ABCD;
            Assert.AreEqual(0x1234ABCD, pokemon.OriginalTrainerID);
            Assert.AreEqual(0xABCD, pokemon.OriginalTrainerPublicID);
            Assert.AreEqual(0x1234, pokemon.OriginalTrainerSecretID);

            pokemon.OriginalTrainerPublicID = 0x1A2B;
            Assert.AreEqual(0x12341A2B, pokemon.OriginalTrainerID);
            Assert.AreEqual(0x1A2B, pokemon.OriginalTrainerPublicID);
            Assert.AreEqual(0x1234, pokemon.OriginalTrainerSecretID);

            pokemon.OriginalTrainerSecretID = 0x3C4D;
            Assert.AreEqual(0x3C4D1A2B, pokemon.OriginalTrainerID);
            Assert.AreEqual(0x1A2B, pokemon.OriginalTrainerPublicID);
            Assert.AreEqual(0x3C4D, pokemon.OriginalTrainerSecretID);
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
            Assert.AreEqual(0xABCD, pokemon.OriginalTrainerID);
            Assert.AreEqual(0xABCD, pokemon.OriginalTrainerPublicID);

            pokemon.OriginalTrainerPublicID = 0x9876;
            Assert.AreEqual(0x9876, pokemon.OriginalTrainerID);
            Assert.AreEqual(0x9876, pokemon.OriginalTrainerPublicID);
        }
    }

    static public void TestCommon(
        PKMN.Pokemon pokemon,
        PokemonTestParams testParams
    )
    {
        CheckInitialValues(pokemon);
        CheckInitialMaps(pokemon);
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
