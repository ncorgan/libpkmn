/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

internal class PokemonTestsCommon {

    private static string[] Starters = {
        "", "Bulbasaur", "Chikorita", "Treecko", "Turtwig", "Snivy", "Chespin"
    };

    private static string[] Gen1PokemonWithXYMegaForms = {
        "Venusaur", "Blastoise", "Alakazam", "Gengar", "Kangaskhan", "Pinsir", "Gyarados", "Aerodactyl"
    };

    private static string[] Gen1PokemonWithORASMegaForms = {
        "Beedrill", "Pidgeot", "Slowbro"
    };

    private static string[] Gen2PokemonWithXYMegaForms = {
        "Ampharos", "Scizor", "Heracross", "Houndoom", "Tyranitar"
    };

    private static string[] Gen3PokemonWithXYMegaForms = {
        "Blaziken", "Gardevoir", "Mawile", "Aggron", "Medicham",
        "Manectric", "Banette", "Absol", "Latias", "Latios", "Garchomp"
    };

    private static string[] Gen3PokemonWithORASMegaForms = {
        "Sceptile", "Swampert", "Sableye", "Sharpedo", "Camerupt",
        "Altaria", "Glalie", "Salamence", "Metagross", "Rayquaza"
    };

    private static int GameToGeneration(
        string game
    ) {
        if(game.Equals("Red") || game.Equals("Blue") || game.Equals("Yellow")) {
            return 1;
        } else if(game.Equals("Gold") || game.Equals("Silver") || game.Equals("Crystal")) {
            return 2;
        } else if(game.Equals("Ruby") || game.Equals("Sapphire") || game.Equals("Emerald") ||
                  game.Equals("FireRed") || game.Equals("LeafGreen") ||
                  game.Equals("Colosseum") || game.Equals("XD")
        ) {
            return 3;
        } else if(game.Equals("Diamond") || game.Equals("Pearl") || game.Equals("Platinum") ||
                  game.Equals("HeartGold") || game.Equals("SoulSilver")
        ) {
            return 4;
        } else if(game.Equals("Black") || game.Equals("White") ||
                  game.Equals("Black 2") || game.Equals("White 2")
        ) {
            return 5;
        } else if(game.Equals("X") || game.Equals("Y") ||
                  game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire")) {
            return 6;
        } else {
            return 0;
        }
    }

    public static void TestGen1Forms(
        string game
    ) {
        int generation = GameToGeneration(game);

        // Check that Mega forms only work in their given games.
        foreach(string species in Gen1PokemonWithXYMegaForms) {
            if(generation >= 6) {
                new PKMN.Pokemon(species, game, "Mega", 100);
            } else {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon(species, game, "Mega", 100);
                    }
                );
            }
        }
        foreach(string species in Gen1PokemonWithORASMegaForms) {
            if(game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire")) {
                new PKMN.Pokemon(species, game, "Mega", 100);
            } else {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon(species, game, "Mega", 100);
                    }
                );
            }
        }

        // Cosplay Pikachu should only work in OR/AS.
        PKMN.PokemonEntry cosplayPikachu = new PKMN.PokemonEntry("Pikachu", game, "");
        foreach(string form in cosplayPikachu.GetForms()) {
            if(game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire")) {
                new PKMN.Pokemon("Pikachu", game, form, 5);
            } else if(!form.Equals("Standard")) {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon("Pikachu", game, form, 5);
                    }
                );
            }
        }

        // Hard-code Mega X/Y cases.
        if(generation >= 6) {
            new PKMN.Pokemon("Charizard", game, "Mega X", 50);
            new PKMN.Pokemon("Charizard", game, "Mega Y", 50);
            new PKMN.Pokemon("Mewtwo", game, "Mega X", 50);
            new PKMN.Pokemon("Mewtwo", game, "Mega Y", 50);
        } else {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    new PKMN.Pokemon("Charizard", game, "Mega X", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    new PKMN.Pokemon("Charizard", game, "Mega Y", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    new PKMN.Pokemon("Mewtwo", game, "Mega X", 50);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    new PKMN.Pokemon("Mewtwo", game, "Mega Y", 50);
                }
            );
        }
    }

    public static void TestGen2Forms(
        string game
    ) {
        int generation = GameToGeneration(game);

        // Check that Mega forms only work in their given games.
        foreach(string species in Gen2PokemonWithXYMegaForms) {
            if(generation >= 6) {
                new PKMN.Pokemon(species, game, "Mega", 100);
            } else {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon(species, game, "Mega", 100);
                    }
                );
            }
        }

        // Only one Generation II Pokémon with a OR/AS-specific Mega form.
        if(game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire")) {
            new PKMN.Pokemon("Steelix", game, "Mega", 100);
        } else {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    new PKMN.Pokemon("Steelix", game, "Mega", 100);
                }
            );
        }

        // Spiky-eared Pichu should only work in HG/SS.
        if(game.Equals("HeartGold") || game.Equals("SoulSilver")) {
            new PKMN.Pokemon("Pichu", game, "Spiky-eared", 100);
        } else {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    new PKMN.Pokemon("Pichu", game, "Spiky-eared", 100);
                }
            );
        }

        // Unown's "!" and "?" forms aren't in Generation II.
        PKMN.PokemonEntry unownEntry = new PKMN.PokemonEntry("Unown", "Omega Ruby", "");
        foreach(string form in unownEntry.GetForms()) {
            if(form.Equals("!") || form.Equals("?")) {
                if(generation > 2) {
                    new PKMN.Pokemon("Unown", game, form, 10);
                } else {
                    Assert.Throws<ArgumentOutOfRangeException>(
                        delegate {
                            new PKMN.Pokemon("Unown", game, form, 10);
                        }
                    );
                }
            } else {
                new PKMN.Pokemon("Unown", game, form, 10);
            }
        }
    }

    private static void TestGen3Forms(
        string game,
        bool gcn
    ) {
        // TODO: Shadow forms
        int generation = GameToGeneration(game);

        // Check that Mega forms only work in their given games.
        foreach(string species in Gen3PokemonWithXYMegaForms) {
            if(generation >= 6) {
                new PKMN.Pokemon(species, game, "Mega", 100);
            } else {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon(species, game, "Mega", 100);
                    }
                );
            }
        }
        foreach(string species in Gen3PokemonWithORASMegaForms) {
            if(game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire")) {
                new PKMN.Pokemon(species, game, "Mega", 100);
            } else {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon(species, game, "Mega", 100);
                    }
                );
            }
        }

        // Castform should always work.
        PKMN.PokemonEntry castformEntry = new PKMN.PokemonEntry("Castform", "Omega Ruby", "");
        foreach(string form in castformEntry.GetForms()) {
            new PKMN.Pokemon("Castform", game, form, 30);
        }

        // Primal Reversion forms should only work in OR/AS.
        new PKMN.Pokemon("Groudon", game, "", 70);
        new PKMN.Pokemon("Kyogre", game, "", 70);
        if(game.Equals("Omega Ruby") || game.Equals("Alpha Sapphire")) {
            new PKMN.Pokemon("Groudon", game, "Primal Reversion", 70);
            new PKMN.Pokemon("Kyogre", game, "Primal Reversion", 70);
        } else {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    new PKMN.Pokemon("Groudon", game, "Primal Reversion", 70);
                }
            );
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    new PKMN.Pokemon("Kyogre", game, "Primal Reversion", 70);
                }
            );
        }

        // In Generation III, Deoxys's form is game-specific.
        if(generation == 3) {
            if(game.Equals("Ruby") || game.Equals("Sapphire") || game.Equals("Colosseum") || game.Equals("XD")) {
                new PKMN.Pokemon("Deoxys", game, "Normal", 70);
            } else {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon("Deoxys", game, "Normal", 70);
                    }
                );
            }

            if(game.Equals("FireRed")) {
                new PKMN.Pokemon("Deoxys", game, "Attack", 70);
            } else {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon("Deoxys", game, "Attack", 70);
                    }
                );
            }

            if(game.Equals("LeafGreen")) {
                new PKMN.Pokemon("Deoxys", game, "Defense", 70);
            } else {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon("Deoxys", game, "Defense", 70);
                    }
                );
            }

            if(game.Equals("Emerald")) {
                new PKMN.Pokemon("Deoxys", game, "Speed", 70);
            } else {
                Assert.Throws<ArgumentOutOfRangeException>(
                    delegate {
                        new PKMN.Pokemon("Deoxys", game, "Speed", 70);
                    }
                );
            }
        } else {
            // Past Generation III, Deoxys's form can be switched.
            PKMN.PokemonEntry deoxysEntry = new PKMN.PokemonEntry("Deoxys", "Omega Ruby", "");
            foreach(string form in deoxysEntry.GetForms()) {
                new PKMN.Pokemon("Deoxys", game, form, 70);
            }
        }
    }

    private static void TestForms(
        string game
    ) {
        int generation = GameToGeneration(game);

        /*
         * Make sure forms that didn't appear for a given
         * generation's Pokémon until later don't work until
         * the correct generation.
         */
        if(generation >= 1) {
            TestGen1Forms(game);
        }
        if(generation >= 2) {
            TestGen2Forms(game);
        }
        if(generation >= 3) {
            TestGen3Forms(
                game,
                (game.Equals("Colosseum") || game.Equals("XD"))
            );
        }
    }

    private static void TestInvalidStarters(
        string game
    ) {
        int generation = GameToGeneration(game);

        for(int i = (generation+1); i <= 6; ++i) {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate {
                    new PKMN.Pokemon(Starters[i], game, "", 5);
                }
            );
        }
    }

    public static void TestInvalidPokemon(
        string game
    ) {
        TestForms(game);
        TestInvalidStarters(game);
    }
}

}
