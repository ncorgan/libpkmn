/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_tests_common.hpp>

#include <pkmn/pokemon.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/assign.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

// No database access here
static const std::map<std::string, int> game_generations = boost::assign::map_list_of
    ("Red", 1)
    ("Blue", 1)
    ("Yellow", 1)
    ("Gold", 2)
    ("Silver", 2)
    ("Crystal", 2)
    ("Ruby", 3)
    ("Sapphire", 3)
    ("Emerald", 3)
    ("FireRed", 3)
    ("LeafGreen", 3)
    ("Colosseum", 3)
    ("XD", 3)
    ("Diamond", 4)
    ("Pearl", 4)
    ("Platinum", 4)
    ("HeartGold", 4)
    ("SoulSilver", 4)
    ("Black", 5)
    ("White", 5)
    ("Black 2", 5)
    ("White 2", 5)
    ("X", 6)
    ("Y", 6)
    ("Omega Ruby", 6)
    ("Alpha Sapphire", 6)
;

static const std::vector<std::string> starters = boost::assign::list_of
    ("")("Bulbasaur")("Chikorita")("Treecko")("Turtwig")("Snivy")("Chespin")
;

static const std::vector<std::string> gen1_pokemon_with_xy_mega_forms = boost::assign::list_of
    ("Venusaur")("Blastoise")("Alakazam")("Gengar")("Kangaskhan")("Pinsir")("Gyarados")("Aerodactyl")
;

static const std::vector<std::string> gen1_pokemon_with_oras_mega_forms = boost::assign::list_of
    ("Beedrill")("Pidgeot")("Slowbro")
;

static const std::vector<std::string> gen2_pokemon_with_xy_mega_forms = boost::assign::list_of
    ("Ampharos")("Scizor")("Heracross")("Houndoom")("Tyranitar")
;

static const std::vector<std::string> gen3_pokemon_with_xy_mega_forms = boost::assign::list_of
    ("Blaziken")("Gardevoir")("Mawile")("Aggron")("Medicham")
    ("Manectric")("Banette")("Absol")("Latias")("Latios")("Garchomp")
;

static const std::vector<std::string> gen3_pokemon_with_oras_mega_forms = boost::assign::list_of
    ("Sceptile")("Swampert")("Sableye")("Sharpedo")("Camerupt")
    ("Altaria")("Glalie")("Salamence")("Metagross")("Rayquaza")
;

namespace pkmntest {

    static void test_gen1_forms(
        const std::string &game
    ) {
        int generation = game_generations.at(game);

        // Check that Mega forms only work in their given games
        for(auto iter = gen1_pokemon_with_xy_mega_forms.begin();
                 iter != gen1_pokemon_with_xy_mega_forms.end();
                 ++iter)
        {
            if(generation >= 6) {
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              *iter,
                              game,
                              "Mega",
                              100
                          );
                , std::invalid_argument);
            }
        }
        for(auto iter = gen1_pokemon_with_oras_mega_forms.begin();
                 iter != gen1_pokemon_with_oras_mega_forms.end();
                 ++iter)
        {
            if(game == "Omega Ruby" or game == "Alpha Sapphire") {
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              *iter,
                              game,
                              "Mega",
                              100
                          );
                , std::invalid_argument);
            }
        }

        // Cosplay Pikachu should only work for OR/AS
        static const pkmn::database::pokemon_entry oras_pikachu(
            "Pikachu",
            "Omega Ruby",
            ""
        );
        const std::vector<std::string>& cosplay_forms = oras_pikachu.get_forms();
        for(auto iter = cosplay_forms.begin()+1;
                 iter != cosplay_forms.end();
                 ++iter)
        {
            if(game == "Omega Ruby" or game == "Alpha Sapphire") {
                (void)pkmn::pokemon::make(
                          "Pikachu",
                          game,
                          *iter,
                          5
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              "Pikachu",
                              game,
                              *iter,
                              5
                          );
                , std::invalid_argument);
            }
        }

        // Hardcode Mega X/Y cases
        if(generation >= 6) {
            (void)pkmn::pokemon::make(
                      "Charizard",
                      game,
                      "Mega X",
                      50
                  );
            (void)pkmn::pokemon::make(
                      "Charizard",
                      game,
                      "Mega Y",
                      50
                  );
            (void)pkmn::pokemon::make(
                      "Mewtwo",
                      game,
                      "Mega X",
                      50
                  );
            (void)pkmn::pokemon::make(
                      "Mewtwo",
                      game,
                      "Mega Y",
                      50
                  );
        } else {
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Charizard",
                          game,
                          "Mega X",
                          50
                      );
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Charizard",
                          game,
                          "Mega Y",
                          50
                      );
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Mewtwo",
                          game,
                          "Mega X",
                          50
                      );
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Mewtwo",
                          game,
                          "Mega Y",
                          50
                      );
            , std::invalid_argument);
        }
    }

    static void test_gen2_forms(
        const std::string &game
    ) {
        int generation = game_generations.at(game);

        // Check that Mega forms only work in their given games
        for(auto iter = gen2_pokemon_with_xy_mega_forms.begin();
                 iter != gen2_pokemon_with_xy_mega_forms.end();
                 ++iter)
        {
            if(generation >= 6) {
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              *iter,
                              game,
                              "Mega",
                              100
                          );
                , std::invalid_argument);
            }
        }

        if(game == "Omega Ruby" or game == "Alpha Sapphire") {
            (void)pkmn::pokemon::make(
                      "Steelix",
                      game,
                      "Mega",
                      100
                  );
        } else {
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Steelix",
                          game,
                          "Mega",
                          100
                      );
            , std::invalid_argument);
        }

        // Spiky-eared Pichu should only work in HG/SS
        if(game == "HeartGold" or game == "SoulSilver") {
            (void)pkmn::pokemon::make(
                      "Pichu",
                      game,
                      "Spiky-eared",
                      100
                  );
        } else {
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Pichu",
                          game,
                          "Spiky-eared",
                          100
                      );
            , std::invalid_argument);
        }

        // Unown's "!" and "?" forms aren't in Generation II
        static const pkmn::database::pokemon_entry oras_unown("Unown", "Omega Ruby", "");
        const std::vector<std::string>& unown_forms = oras_unown.get_forms();
        for(size_t i = 0; i < 26; ++i) {
            (void)pkmn::pokemon::make(
                      "Unown",
                      game,
                      unown_forms.at(i),
                      10
                  );
        }
        if(generation == 2) {
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Unown",
                          game,
                          "!",
                          10
                      );
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Unown",
                          game,
                          "?",
                          10
                      );
            , std::invalid_argument);
        } else {
            (void)pkmn::pokemon::make(
                      "Unown",
                      game,
                      "!",
                      10
                  );
            (void)pkmn::pokemon::make(
                      "Unown",
                      game,
                      "?",
                      10
                  );
        }
    }

    static void test_gen3_forms(
        const std::string &game,
        bool gcn
    ) {
        // TODO: Shadow forms
        (void)gcn;

        int generation = game_generations.at(game);

        // Check that Mega forms only work in their given games
        for(auto iter = gen3_pokemon_with_xy_mega_forms.begin();
                 iter != gen3_pokemon_with_xy_mega_forms.end();
                 ++iter)
        {
            if(generation >= 6) {
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              *iter,
                              game,
                              "Mega",
                              100
                          );
                , std::invalid_argument);
            }
        }
        for(auto iter = gen3_pokemon_with_oras_mega_forms.begin();
                 iter != gen3_pokemon_with_oras_mega_forms.end();
                 ++iter)
        {
            if(game == "Omega Ruby" or game == "Alpha Sapphire") {
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              *iter,
                              game,
                              "Mega",
                              100
                          );
                , std::invalid_argument);
            }
        }

        // Castform should always work.
        std::vector<std::string> castform_forms = pkmn::database::pokemon_entry(
                                                      "Castform",
                                                      "Omega Ruby",
                                                      ""
                                                  ).get_forms();
        for(auto iter = castform_forms.begin(); iter != castform_forms.end(); ++iter) {
            (void)pkmn::pokemon::make(
                      "Castform",
                      game,
                      *iter,
                      30
                  );
        }

        // Primal Reversion forms should only work in OR/AS.
        (void)pkmn::pokemon::make(
                  "Groudon",
                  game,
                  "",
                  70
              );
        (void)pkmn::pokemon::make(
                  "Kyogre",
                  game,
                  "",
                  70
              );
        if(game == "Omega Ruby" or game == "Alpha Sapphire") {
            (void)pkmn::pokemon::make(
                      "Groudon",
                      game,
                      "Primal Reversion",
                      70
                  );
            (void)pkmn::pokemon::make(
                      "Kyogre",
                      game,
                      "Primal Reversion",
                      70
                  );
        } else {
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Groudon",
                          game,
                          "Primal Reversion",
                          70
                      );
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          "Kyogre",
                          game,
                          "Primal Reversion",
                          70
                      );
            , std::invalid_argument);
        }

        // In Generation III, Deoxys's form is game-specific.
        if(generation == 3) {
            if(game == "Ruby" or game == "Sapphire" or game == "Colosseum" or game == "XD") {
                (void)pkmn::pokemon::make(
                          "Deoxys",
                          game,
                          "Normal",
                          70
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              "Deoxys",
                              game,
                              "Normal",
                              70
                          );
                , std::invalid_argument);
            }

            if(game == "FireRed") {
                (void)pkmn::pokemon::make(
                          "Deoxys",
                          game,
                          "Attack",
                          70
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              "Deoxys",
                              game,
                              "Attack",
                              70
                          );
                , std::invalid_argument);
            }

            if(game == "LeafGreen") {
                (void)pkmn::pokemon::make(
                          "Deoxys",
                          game,
                          "Defense",
                          70
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              "Deoxys",
                              game,
                              "Defense",
                              70
                          );
                , std::invalid_argument);
            }

            if(game == "Emerald") {
                (void)pkmn::pokemon::make(
                          "Deoxys",
                          game,
                          "Speed",
                          70
                      );
            } else {
                BOOST_CHECK_THROW(
                    (void)pkmn::pokemon::make(
                              "Deoxys",
                              game,
                              "Speed",
                              70
                          );
                , std::invalid_argument);
            }
        } else {
            // Past Generation III, Deoxys's form can be switched.
            std::vector<std::string> deoxys_forms = pkmn::database::pokemon_entry(
                                                        "Deoxys",
                                                        "Omega Ruby",
                                                        ""
                                                    ).get_forms();
            for(auto iter = deoxys_forms.begin(); iter != deoxys_forms.end(); ++iter) {
                (void)pkmn::pokemon::make(
                          "Deoxys",
                          game,
                          *iter,
                          30
                      );
            }
        }
    }

    // TODO: Check Alola forms when Generation VII supported
    static void test_forms(
        const std::string &game
    ) {
        int generation = game_generations.at(game);

        // Make sure forms that didn't appear for a given
        // generation's Pokémon until later don't work until
        // the correct generation.
        if(generation >= 1) {
            test_gen1_forms(game);
        }
        if(generation >= 2) {
            test_gen2_forms(game);
        }
        if(generation >= 3) {
            test_gen3_forms(
                game,
                (game == "Colosseum" or game == "XD")
            );
        }
    }

    static void test_invalid_starters(
        const std::string &game
    ) {
        int generation = game_generations.at(game);
        for(int i = (generation+1); i <= 6; ++i) {
            BOOST_CHECK_THROW(
                (void)pkmn::pokemon::make(
                          starters.at(i),
                          game,
                          "",
                          5
                      );
            , std::invalid_argument);
        }
    }

    void invalid_pokemon_test(
        const std::string &game
    ) {
        test_forms(game);
        test_invalid_starters(game);
    }

}
