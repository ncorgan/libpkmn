/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/hidden_power.hpp>
#include <pkmn/calculations/nature.hpp>
#include <pkmn/calculations/shininess.hpp>
#include <pkmn/calculations/size.hpp>
#include <pkmn/calculations/spinda_spots.hpp>
#include <pkmn/calculations/stats.hpp>

#include <pkmn/database/pokemon_entry.hpp>

#include <pkmn/exception.hpp>

#include <boost/assign.hpp>

#include <gtest/gtest.h>

#include <random>
#include <ctime>
#include <limits>

TEST(cpp_calculations_test, gen2_unown_form_test) {
    /*
     * Make sure expected exceptions are thrown.
     */
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_unown_form(
                  -1, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_unown_form(
                  16, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_unown_form(
                  0, -1, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_unown_form(
                  0, 16, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_unown_form(
                  0, 0, -1, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_unown_form(
                  0, 0, 16, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_unown_form(
                  0, 0, 0, -1
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_unown_form(
                  0, 0, 0, 16
              );
    , pkmn::range_error);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
     */
    std::string form1 = pkmn::calculations::gen2_unown_form(
                            10, 9, 1, 14
                        );
    EXPECT_EQ("G", form1);

    std::string form2 = pkmn::calculations::gen2_unown_form(
                            5, 15, 10, 5
                        );
    EXPECT_EQ("S", form2);
}

TEST(cpp_calculations_test, gen3_unown_form_test) {
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
     */
    std::string form = pkmn::calculations::gen3_unown_form(
                           0x4C07DE71
                       );
    EXPECT_EQ("B", form);
}

TEST(cpp_calculations_test, wurmple_becomes_silcoon_test) {
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
     */
    bool before_gen5_1 = pkmn::calculations::wurmple_becomes_silcoon(
        0x5CF4091C, true
    );
    EXPECT_TRUE(before_gen5_1);
    bool before_gen5_2 = pkmn::calculations::wurmple_becomes_silcoon(
        0x091C5CF4, true
    );
    EXPECT_FALSE(before_gen5_2);

    bool gen5_1 = pkmn::calculations::wurmple_becomes_silcoon(
        0x091C5CF4, false
    );
    EXPECT_TRUE(gen5_1);
    bool gen5_2 = pkmn::calculations::wurmple_becomes_silcoon(
        0x5CF4091C, false
    );
    EXPECT_FALSE(gen5_2);
}

TEST(cpp_calculations_test, gen2_gender_test) {
    /*
     * Make sure expected exceptions are thrown.
     */
    EXPECT_THROW(
        std::string gender = pkmn::calculations::gen2_pokemon_gender(
                                 "Not a species", 0
                             );
    , std::invalid_argument);
    EXPECT_THROW(
        std::string gender = pkmn::calculations::gen2_pokemon_gender(
                                 "Bulbasaur", -1
                             );
    , pkmn::range_error);
    EXPECT_THROW(
        std::string gender = pkmn::calculations::gen2_pokemon_gender(
                                 "Bulbasaur", 16
                             );
    , pkmn::range_error);

    /*
     * Make sure known good inputs result in expected results.
     */

    /*
     * All male
     */
    std::string nidorino1 = pkmn::calculations::gen2_pokemon_gender(
                                "Nidorino", 0
                            );
    EXPECT_EQ("Male", nidorino1);
    std::string nidorino2 = pkmn::calculations::gen2_pokemon_gender(
                                "Nidorino", 15
                            );
    EXPECT_EQ("Male", nidorino2);

    /*
     * 87.5% male, 12.5% female
     */
    std::string charmander1 = pkmn::calculations::gen2_pokemon_gender(
                                  "Charmander", 1
                              );
    EXPECT_EQ("Female", charmander1);
    std::string charmander2 = pkmn::calculations::gen2_pokemon_gender(
                                  "Charmander", 2
                              );
    EXPECT_EQ("Male", charmander2);

    /*
     * 75% male, 25% female
     */
    std::string growlithe1 = pkmn::calculations::gen2_pokemon_gender(
                                 "Growlithe", 3
                             );
    EXPECT_EQ("Female", growlithe1);
    std::string growlithe2 = pkmn::calculations::gen2_pokemon_gender(
                                 "Growlithe", 4
                             );
    EXPECT_EQ("Male", growlithe2);

    /*
     * 50% male, 50% female
     */
    std::string pidgey1 = pkmn::calculations::gen2_pokemon_gender(
                              "Pidgey", 6
                          );
    EXPECT_EQ("Female", pidgey1);
    std::string pidgey2 = pkmn::calculations::gen2_pokemon_gender(
                              "Pidgey", 7
                          );
    EXPECT_EQ("Male", pidgey2);

    /*
     * 25% male, 75% female
     */
    std::string vulpix1 = pkmn::calculations::gen2_pokemon_gender(
                              "Vulpix", 11
                          );
    EXPECT_EQ("Female", vulpix1);
    std::string vulpix2 = pkmn::calculations::gen2_pokemon_gender(
                              "Vulpix", 12
                          );
    EXPECT_EQ("Male", vulpix2);

    /*
     * All female
     */
    std::string nidorina1 = pkmn::calculations::gen2_pokemon_gender(
                                "Nidorina", 0
                            );
    EXPECT_EQ("Female", nidorina1);
    std::string nidorina2 = pkmn::calculations::gen2_pokemon_gender(
                                "Nidorina", 15
                            );
    EXPECT_EQ("Female", nidorina2);

    /*
     * Genderless
     */
    std::string magnemite1 = pkmn::calculations::gen2_pokemon_gender(
                                 "Magnemite", 0
                             );
    EXPECT_EQ("Genderless", magnemite1);
    std::string magnemite2 = pkmn::calculations::gen2_pokemon_gender(
                                 "Magnemite", 15
                             );
    EXPECT_EQ("Genderless", magnemite2);
}

TEST(cpp_calculations_test, modern_gender_test) {
    /*
     * Make sure expected exceptions are thrown
     */
    EXPECT_THROW(
        std::string gender = pkmn::calculations::modern_pokemon_gender(
                                 "Not a species", 0
                             );
    , std::invalid_argument);

    /*
     * Make sure known good inputs result in expected results.
     */

    /*
     * All male
     */
    std::string nidorino1 = pkmn::calculations::modern_pokemon_gender(
                                "Nidorino", 0
                            );
    EXPECT_EQ("Male", nidorino1);
    std::string nidorino2 = pkmn::calculations::modern_pokemon_gender(
                                "Nidorino",
                                std::numeric_limits<uint32_t>::max()
                            );
    EXPECT_EQ("Male", nidorino2);

    /*
     * 87.5% male, 12.5% female
     */
    std::string charmander1 = pkmn::calculations::modern_pokemon_gender(
                                  "Charmander", 30
                              );
    EXPECT_EQ("Female", charmander1);
    std::string charmander2 = pkmn::calculations::modern_pokemon_gender(
                                  "Charmander", 31
                              );
    EXPECT_EQ("Male", charmander2);

    /*
     * 75% male, 25% female
     */
    std::string growlithe1 = pkmn::calculations::modern_pokemon_gender(
                                 "Growlithe", 63
                             );
    EXPECT_EQ("Female", growlithe1);
    std::string growlithe2 = pkmn::calculations::modern_pokemon_gender(
                                 "Growlithe", 64
                             );
    EXPECT_EQ("Male", growlithe2);

    /*
     * 50% male, 50% female
     */
    std::string pidgey1 = pkmn::calculations::modern_pokemon_gender(
                              "Pidgey", 126
                          );
    EXPECT_EQ("Female", pidgey1);
    std::string pidgey2 = pkmn::calculations::modern_pokemon_gender(
                              "Pidgey", 127
                          );
    EXPECT_EQ("Male", pidgey2);

    /*
     * 25% male, 75% female
     */
    std::string vulpix1 = pkmn::calculations::modern_pokemon_gender(
                              "Vulpix", 190
                          );
    EXPECT_EQ("Female", vulpix1);
    std::string vulpix2 = pkmn::calculations::modern_pokemon_gender(
                              "Vulpix", 191
                          );
    EXPECT_EQ("Male", vulpix2);

    /*
     * All female
     */
    std::string nidorina1 = pkmn::calculations::modern_pokemon_gender(
                                "Nidorina", 0
                            );
    EXPECT_EQ("Female", nidorina1);
    std::string nidorina2 = pkmn::calculations::modern_pokemon_gender(
                                "Nidorina",
                                std::numeric_limits<uint32_t>::max()
                            );
    EXPECT_EQ("Female", nidorina2);

    /*
     * Genderless
     */
    std::string magnemite1 = pkmn::calculations::modern_pokemon_gender(
                                 "Magnemite", 0
                             );
    EXPECT_EQ("Genderless", magnemite1);
    std::string magnemite2 = pkmn::calculations::modern_pokemon_gender(
                                 "Magnemite",
                                std::numeric_limits<uint32_t>::max()
                             );
    EXPECT_EQ("Genderless", magnemite2);
}

TEST(cpp_calculations_test, gen2_hidden_power_test) {
    pkmn::calculations::hidden_power hidden_power;
    /*
     * Make sure expected exceptions are thrown
     */
    EXPECT_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           -1, 0, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           16, 0, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, -1, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, 16, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, 0, -1, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, 0, 16, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, 0, 0, -1
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, 0, 0, 16
                       );
    , pkmn::range_error);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
     */
    hidden_power = pkmn::calculations::gen2_hidden_power(
                       15, 15, 15, 14
                   );
    EXPECT_EQ("Dark", hidden_power.type);
    EXPECT_EQ(69, hidden_power.base_power);
}

TEST(cpp_calculations_test, modern_hidden_power_test) {
    pkmn::calculations::hidden_power hidden_power;
    /*
     * Make sure expected exceptions are thrown
     */
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           -1, 0, 0, 0, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           32, 0, 0, 0, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, -1, 0, 0, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 32, 0, 0, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, -1, 0, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 32, 0, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, -1, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, 32, 0, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, 0, -1, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, 0, 32, 0
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, 0, 0, -1
                       );
    , pkmn::range_error);
    EXPECT_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, 0, 0, 32
                       );
    , pkmn::range_error);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
     */
    hidden_power = pkmn::calculations::modern_hidden_power(
                       30, 31, 31, 31, 30, 31
                   );
    EXPECT_EQ("Grass", hidden_power.type);
    EXPECT_EQ(70, hidden_power.base_power);
}

TEST(cpp_calculations_test, gen3_gen4_nature_test) {
    static const std::vector<std::string> natures = boost::assign::list_of
        ("Hardy")("Lonely")("Brave")("Adamant")("Naughty")
        ("Bold")("Docile")("Relaxed")("Impish")("Lax")
        ("Timid")("Hasty")("Serious")("Jolly")("Naive")
        ("Modest")("Mild")("Quiet")("Bashful")("Rash")
        ("Calm")("Gentle")("Sassy")("Careful")("Quirky")
    ;

    std::srand((unsigned int)std::time(NULL));
    for(uint32_t i = 0; i < natures.size(); ++i) {
        EXPECT_EQ(
            pkmn::calculations::gen3_gen4_nature(uint32_t(((std::rand() % 50000) * 1000) + i)),
            natures[i]
        );
    }
}

TEST(cpp_calculations_test, gen2_shiny_test) {
    /*
     * Make sure expected exceptions are thrown
     */
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_shiny(
                  -1, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_shiny(
                  16, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_shiny(
                  0, -1, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_shiny(
                  0, 16, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_shiny(
                  0, 0, -1, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_shiny(
                  0, 0, 16, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_shiny(
                  0, 0, 0, -1
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::gen2_shiny(
                  0, 0, 0, 16
              );
    , pkmn::range_error);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
     */
    bool shiny1 = pkmn::calculations::gen2_shiny(
                      7, 10, 10, 10
                  );
    bool shiny2 = pkmn::calculations::gen2_shiny(
                      6, 15, 7, 5
                  );
    EXPECT_TRUE(shiny1);
    EXPECT_FALSE(shiny2);
}

TEST(cpp_calculations_test, modern_shiny_test) {
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
     *         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
     */
    bool shiny1 = pkmn::calculations::modern_shiny(
                      2814471828,
                      2545049318
                  );
    bool shiny2 = pkmn::calculations::modern_shiny(
                      0xB58F0B2A,
                      398174488
                  );
    EXPECT_TRUE(shiny1);
    EXPECT_TRUE(shiny2);
}

TEST(cpp_calculations_test, pokemon_size_test) {
    /*
     * There are no known good calculations, so just check for reasonable values
     * for each relevant Pokémon.
     */
    static const std::vector<pkmn::database::pokemon_entry> pokemon_with_size_checks = boost::assign::list_of
        (pkmn::database::pokemon_entry("Barboach", "Ruby", ""))
        (pkmn::database::pokemon_entry("Shroomish", "Ruby", ""))
        (pkmn::database::pokemon_entry("Seedot", "Emerald", ""))
        (pkmn::database::pokemon_entry("Lotad", "Emerald", ""))
        (pkmn::database::pokemon_entry("Magikarp", "FireRed", ""))
        (pkmn::database::pokemon_entry("Heracross", "FireRed", ""))
    ;

    // Test input validation.
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, -1, 0, 0, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 32, 0, 0, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, -1, 0, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, 32, 0, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, 0, -1, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, 0, 32, 0, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, 0, 0, -1, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, 0, 0, 32, 0, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, 0, 0, 0, -1, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, 0, 0, 0, 32, 0
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, 0, 0, 0, 0, -1
              );
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::calculations::pokemon_size(
                  "Barboach", 0, 0, 0, 0, 0, 0, 32
              );
    , pkmn::range_error);

    std::srand((unsigned int)std::time(NULL));
    for(auto pokemon_iter = pokemon_with_size_checks.begin();
        pokemon_iter != pokemon_with_size_checks.end();
        ++pokemon_iter
    ) {
        float height = pokemon_iter->get_height();
        std::string species = pokemon_iter->get_name();

        for(int i = 0; i < 10; ++i) {
            float size = pkmn::calculations::pokemon_size(
                             species,
                             uint32_t(std::rand()),
                             (std::rand() % 32),
                             (std::rand() % 32),
                             (std::rand() % 32),
                             (std::rand() % 32),
                             (std::rand() % 32),
                             (std::rand() % 32)
                         );
            EXPECT_LE(std::fabs(size-height), height);
        }
    }
}

TEST(cpp_calculations_test, spinda_coords_test) {
    /*
     * Check (in)equality operators.
     */
    BOOST_STATIC_CONSTEXPR pkmn::calculations::spinda_coords COORDS1(123,456);
    BOOST_STATIC_CONSTEXPR pkmn::calculations::spinda_coords COORDS2(123,456);
    BOOST_STATIC_CONSTEXPR pkmn::calculations::spinda_coords COORDS3(456,123);

    EXPECT_TRUE(COORDS1 == COORDS2);
    EXPECT_FALSE(COORDS1 != COORDS2);
    EXPECT_TRUE(COORDS1 != COORDS3);
    EXPECT_FALSE(COORDS1 == COORDS3);

    /*
     * Test adding coordinates.
     */
    pkmn::calculations::spinda_coords added_coords = COORDS1 + COORDS2;
    EXPECT_EQ(246, added_coords.x);
    EXPECT_EQ(912, added_coords.y);

    added_coords += COORDS3;
    EXPECT_EQ(702, added_coords.x);
    EXPECT_EQ(1035, added_coords.y);
}

TEST(cpp_calculations_test, spinda_spot_test) {
    /*
     * Check (in)equality operators.
     */
    BOOST_STATIC_CONSTEXPR pkmn::calculations::spinda_spots SPOTS1(
        pkmn::calculations::spinda_coords(7,5),
        pkmn::calculations::spinda_coords(10,0),
        pkmn::calculations::spinda_coords(1,4),
        pkmn::calculations::spinda_coords(2,15)
    );
    BOOST_STATIC_CONSTEXPR pkmn::calculations::spinda_spots SPOTS2(
        pkmn::calculations::spinda_coords(7,5),
        pkmn::calculations::spinda_coords(10,0),
        pkmn::calculations::spinda_coords(1,4),
        pkmn::calculations::spinda_coords(2,15)
    );
    BOOST_STATIC_CONSTEXPR pkmn::calculations::spinda_spots SPOTS3(
        pkmn::calculations::spinda_coords(7,5),
        pkmn::calculations::spinda_coords(10,0),
        pkmn::calculations::spinda_coords(2,15),
        pkmn::calculations::spinda_coords(1,4)
    );
    EXPECT_TRUE(SPOTS1 == SPOTS2);
    EXPECT_TRUE(SPOTS1 != SPOTS3);

    /*
     * Test adding spots.
     */
    pkmn::calculations::spinda_spots added_spots = SPOTS1 + SPOTS2;
    EXPECT_EQ(14, added_spots.left_ear.x);
    EXPECT_EQ(10, added_spots.left_ear.y);
    EXPECT_EQ(20, added_spots.right_ear.x);
    EXPECT_EQ(0, added_spots.right_ear.y);
    EXPECT_EQ(2, added_spots.left_face.x);
    EXPECT_EQ(8, added_spots.left_face.y);
    EXPECT_EQ(4, added_spots.right_face.x);
    EXPECT_EQ(30, added_spots.right_face.y);

    added_spots += SPOTS3;
    EXPECT_EQ(21, added_spots.left_ear.x);
    EXPECT_EQ(15, added_spots.left_ear.y);
    EXPECT_EQ(30, added_spots.right_ear.x);
    EXPECT_EQ(0, added_spots.right_ear.y);
    EXPECT_EQ(4, added_spots.left_face.x);
    EXPECT_EQ(23, added_spots.left_face.y);
    EXPECT_EQ(5, added_spots.right_face.x);
    EXPECT_EQ(34, added_spots.right_face.y);

    added_spots += pkmn::calculations::spinda_coords(20, 50);
    EXPECT_EQ(41, added_spots.left_ear.x);
    EXPECT_EQ(65, added_spots.left_ear.y);
    EXPECT_EQ(50, added_spots.right_ear.x);
    EXPECT_EQ(50, added_spots.right_ear.y);
    EXPECT_EQ(24, added_spots.left_face.x);
    EXPECT_EQ(73, added_spots.left_face.y);
    EXPECT_EQ(25, added_spots.right_face.x);
    EXPECT_EQ(84, added_spots.right_face.y);

    added_spots = added_spots + pkmn::calculations::spinda_coords(1, 5);
    EXPECT_EQ(42, added_spots.left_ear.x);
    EXPECT_EQ(70, added_spots.left_ear.y);
    EXPECT_EQ(51, added_spots.right_ear.x);
    EXPECT_EQ(55, added_spots.right_ear.y);
    EXPECT_EQ(25, added_spots.left_face.x);
    EXPECT_EQ(78, added_spots.left_face.y);
    EXPECT_EQ(26, added_spots.right_face.x);
    EXPECT_EQ(89, added_spots.right_face.y);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: https://github.com/magical/spinda
     */

    pkmn::calculations::spinda_spots spots1 = pkmn::calculations::spinda_spot_offset(
                                                  4064348759
                                              );
    EXPECT_TRUE(spots1.left_ear   == SPOTS1.left_ear);
    EXPECT_TRUE(spots1.right_ear  == SPOTS1.right_ear);
    EXPECT_TRUE(spots1.left_face  == SPOTS1.left_face);
    EXPECT_TRUE(spots1.right_face == SPOTS1.right_face);
}

PKMN_INLINE void PKMN_EXPECT_STAT_CLOSE(
    int actual, int expected
) {
    EXPECT_LE(int(std::abs(actual-expected)), 1);
}

TEST(cpp_calculations_test, gb_stat_test) {
    /*
     * Make sure expected exceptions are thrown
     */

    // Invalid stat
    EXPECT_THROW(
        pkmn::calculations::get_gb_stat(
            "Not a stat", 1, 1, 1, 1
        );
    , std::invalid_argument);

    // Invalid EV
    EXPECT_THROW(
        pkmn::calculations::get_gb_stat(
            "Attack", 1, 1, 123456, 1
        );
    , pkmn::range_error);

    // Invalid IV
    EXPECT_THROW(
        pkmn::calculations::get_gb_stat(
            "Attack", 1, 1, 1, 12345
        );
    , pkmn::range_error);

    /*
     * Test with known good inputs.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
     */
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "HP", 81, 35, 22850, 7
        ), 189
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Attack", 81, 55, 23140, 8
        ), 137
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Defense", 81, 30, 17280, 13
        ), 101
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Special", 81, 50, 19625, 9
        ), 128
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Special Attack", 81, 50, 19625, 9
        ), 128
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Special Defense", 81, 40, 19625, 9
        ), 112
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Speed", 81, 90, 24795, 5
        ), 190
    );
}

TEST(cpp_calculations_test, modern_stat_test) {
    /*
     * Make sure expected exceptions are thrown
     */

    // Invalid stat
    EXPECT_THROW(
        pkmn::calculations::get_modern_stat(
            "Not a stat", 1, 1.0f, 1, 1, 1
        );
    , std::invalid_argument);

    // No Special past Generation I
    EXPECT_THROW(
        pkmn::calculations::get_modern_stat(
            "Special", 1, 1.0f, 1, 1, 1
        );
    , std::invalid_argument);

    // Invalid nature modifier
    EXPECT_THROW(
        pkmn::calculations::get_modern_stat(
            "Attack", 1, 0.666f, 1, 1, 1
        );
    , std::domain_error);

    // Invalid EV
    EXPECT_THROW(
        pkmn::calculations::get_modern_stat(
            "Attack", 1, 1.0f, 1, 12345, 1
        );
    , pkmn::range_error);

    // Invalid IV
    EXPECT_THROW(
        pkmn::calculations::get_modern_stat(
            "Attack", 1, 1.0f, 1, 1, 12345
        );
    , pkmn::range_error);

    /*
     * Test with known good inputs.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
     */
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "HP", 78, 1.0f, 108, 74, 24
        ), 289
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Attack", 78, 1.1f, 130, 195, 12
        ), 280
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Defense", 78, 1.0f, 95, 86, 30
        ), 193
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Special Attack", 78, 0.9f, 80, 48, 16
        ), 136
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Special Defense", 78, 1.0f, 85, 84, 23
        ), 171
    );
    PKMN_EXPECT_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Speed", 78, 1.0f, 102, 23, 5
        ), 172
    );
}
