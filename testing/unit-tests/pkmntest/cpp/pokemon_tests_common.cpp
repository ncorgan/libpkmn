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
#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

namespace fs = boost::filesystem;

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
            pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                            "Unown",
                                            game,
                                            unown_forms.at(i),
                                            10
                                        );
            BOOST_CHECK(fs::exists(unown->get_icon_filepath()));
            BOOST_CHECK(fs::exists(unown->get_sprite_filepath()));
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
            pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                            "Unown",
                                            game,
                                            "!",
                                            10
                                        );
            BOOST_CHECK(fs::exists(unown->get_icon_filepath()));
            BOOST_CHECK(fs::exists(unown->get_sprite_filepath()));

            unown = pkmn::pokemon::make(
                        "Unown",
                        game,
                        "?",
                        10
                    );
            BOOST_CHECK(fs::exists(unown->get_icon_filepath()));
            BOOST_CHECK(fs::exists(unown->get_sprite_filepath()));
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
            pkmn::pokemon::sptr castform = pkmn::pokemon::make(
                                               "Castform",
                                               game,
                                               *iter,
                                               30
                                           );
            BOOST_CHECK(fs::exists(castform->get_icon_filepath()));
            BOOST_CHECK(fs::exists(castform->get_sprite_filepath()));
        }

        // Primal Reversion forms should only work in OR/AS.
        pkmn::pokemon::sptr groudon = pkmn::pokemon::make(
                                          "Groudon",
                                          game,
                                          "",
                                          70
                                      );
        BOOST_CHECK(fs::exists(groudon->get_icon_filepath()));
        BOOST_CHECK(fs::exists(groudon->get_sprite_filepath()));

        pkmn::pokemon::sptr kyogre = pkmn::pokemon::make(
                                         "Kyogre",
                                         game,
                                         "",
                                         70
                                     );
        BOOST_CHECK(fs::exists(kyogre->get_icon_filepath()));
        BOOST_CHECK(fs::exists(kyogre->get_sprite_filepath()));

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
        pkmn::pokemon::sptr deoxys;
        if(generation == 3) {
            if(game == "Ruby" or game == "Sapphire" or game == "Colosseum" or game == "XD") {
                deoxys = pkmn::pokemon::make(
                             "Deoxys",
                             game,
                             "Normal",
                             70
                         );
                BOOST_CHECK(fs::exists(deoxys->get_icon_filepath()));
                BOOST_CHECK(fs::exists(deoxys->get_sprite_filepath()));
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
                deoxys = pkmn::pokemon::make(
                             "Deoxys",
                             game,
                             "Attack",
                             70
                         );
                BOOST_CHECK(fs::exists(deoxys->get_icon_filepath()));
                BOOST_CHECK(fs::exists(deoxys->get_sprite_filepath()));
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
                deoxys = pkmn::pokemon::make(
                             "Deoxys",
                             game,
                             "Defense",
                             70
                         );
                BOOST_CHECK(fs::exists(deoxys->get_icon_filepath()));
                BOOST_CHECK(fs::exists(deoxys->get_sprite_filepath()));
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
                deoxys = pkmn::pokemon::make(
                             "Deoxys",
                             game,
                             "Speed",
                             70
                         );
                BOOST_CHECK(fs::exists(deoxys->get_icon_filepath()));
                BOOST_CHECK(fs::exists(deoxys->get_sprite_filepath()));
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
                deoxys = pkmn::pokemon::make(
                             "Deoxys",
                             game,
                             *iter,
                             30
                         );
                BOOST_CHECK(fs::exists(deoxys->get_icon_filepath()));
                BOOST_CHECK(fs::exists(deoxys->get_sprite_filepath()));
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

    void gender_test(
        const std::string &game
    ) {
        // Single-gender
        pkmn::pokemon::sptr nidorina = pkmn::pokemon::make(
                                           "Nidorina",
                                           game,
                                           "",
                                           50
                                       );
        BOOST_CHECK_EQUAL(nidorina->get_gender(), "Female");
        nidorina->set_gender("Female");
        BOOST_CHECK_THROW(
            nidorina->set_gender("Male");
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            nidorina->set_gender("Genderless");
        , std::invalid_argument);

        pkmn::pokemon::sptr nidorino = pkmn::pokemon::make(
                                           "Nidorino",
                                           game,
                                           "",
                                           50
                                       );
        BOOST_CHECK_EQUAL(nidorino->get_gender(), "Male");
        nidorino->set_gender("Male");
        BOOST_CHECK_THROW(
            nidorino->set_gender("Female");
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            nidorino->set_gender("Genderless");
        , std::invalid_argument);

        pkmn::pokemon::sptr magnemite = pkmn::pokemon::make(
                                            "Magnemite",
                                            game,
                                            "",
                                            50
                                        );
        BOOST_CHECK_EQUAL(magnemite->get_gender(), "Genderless");
        magnemite->set_gender("Genderless");
        BOOST_CHECK_THROW(
            magnemite->set_gender("Male");
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            magnemite->set_gender("Female");
        , std::invalid_argument);

        static const std::vector<std::string> mixed_pokemon = boost::assign::list_of
            ("Charmander") // 87.5% male
            ("Growlithe")  // 75% male
            ("Pidgey")     // 50% male
            ("Vulpix")     // 25% male
        ;

        for(auto pokemon_iter = mixed_pokemon.begin(); pokemon_iter != mixed_pokemon.end(); ++pokemon_iter) {
            pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                              *pokemon_iter,
                                              game,
                                              "",
                                              50
                                          );
            BOOST_CHECK_NE(pokemon->get_gender(), "Genderless");

            pokemon->set_gender("Male");
            BOOST_CHECK_EQUAL(pokemon->get_gender(), "Male");
            pokemon->set_gender("Female");
            BOOST_CHECK_EQUAL(pokemon->get_gender(), "Female");
            BOOST_CHECK_THROW(
                pokemon->set_gender("Genderless");
            , std::invalid_argument);
        }
    }

}
