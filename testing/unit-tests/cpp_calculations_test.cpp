/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/hidden_power.hpp>
#include <pkmn/calculations/shininess.hpp>
#include <pkmn/calculations/spinda_spots.hpp>
#include <pkmn/calculations/stats.hpp>

#include <pkmn/exception.hpp>

#include "pkmn_boost_unit_test.hpp"

#include <limits>

BOOST_AUTO_TEST_CASE(gen2_unown_form_test) {
    /*
     * Make sure expected exceptions are thrown
     */
    BOOST_CHECK_THROW(
        std::string form = pkmn::calculations::gen2_unown_form(
                               -1, 0, 0, 0
                           );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        std::string form = pkmn::calculations::gen2_unown_form(
                               16, 0, 0, 0
                           );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        std::string form = pkmn::calculations::gen2_unown_form(
                               0, -1, 0, 0
                           );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        std::string form = pkmn::calculations::gen2_unown_form(
                               0, 16, 0, 0
                           );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        std::string form = pkmn::calculations::gen2_unown_form(
                               0, 0, -1, 0
                           );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        std::string form = pkmn::calculations::gen2_unown_form(
                               0, 0, 16, 0
                           );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        std::string form = pkmn::calculations::gen2_unown_form(
                               0, 0, 0, -1
                           );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        std::string form = pkmn::calculations::gen2_unown_form(
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
    BOOST_CHECK_EQUAL(form1, "G");

    std::string form2 = pkmn::calculations::gen2_unown_form(
                            5, 15, 10, 5
                        );
    BOOST_CHECK_EQUAL(form2, "S");
}

BOOST_AUTO_TEST_CASE(gen3_unown_form_test) {
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
     */
    std::string form = pkmn::calculations::gen3_unown_form(
                           0x4C07DE71
                       );
    BOOST_CHECK_EQUAL(form, "B");
}

BOOST_AUTO_TEST_CASE(wurmple_becomes_silcoon_test) {
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
     */
    bool before_gen5_1 = pkmn::calculations::wurmple_becomes_silcoon(
        0x5CF4091C, true
    );
    BOOST_CHECK(before_gen5_1);
    bool before_gen5_2 = pkmn::calculations::wurmple_becomes_silcoon(
        0x091C5CF4, true
    );
    BOOST_CHECK(not before_gen5_2);

    bool gen5_1 = pkmn::calculations::wurmple_becomes_silcoon(
        0x091C5CF4, false
    );
    BOOST_CHECK(gen5_1);
    bool gen5_2 = pkmn::calculations::wurmple_becomes_silcoon(
        0x5CF4091C, false
    );
    BOOST_CHECK(not gen5_2);
}

BOOST_AUTO_TEST_CASE(gen2_gender_test) {
    /*
     * Make sure expected exceptions are thrown
     */
    BOOST_CHECK_THROW(
        std::string gender = pkmn::calculations::gen2_pokemon_gender(
                                 "Not a species", 0
                             );
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        std::string gender = pkmn::calculations::gen2_pokemon_gender(
                                 "Bulbasaur", -1
                             );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
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
    BOOST_CHECK_EQUAL(nidorino1, "Male");
    std::string nidorino2 = pkmn::calculations::gen2_pokemon_gender(
                                "Nidorino", 15
                            );
    BOOST_CHECK_EQUAL(nidorino2, "Male");

    /*
     * 87.5% male, 12.5% female
     */
    std::string charmander1 = pkmn::calculations::gen2_pokemon_gender(
                                  "Charmander", 1
                              );
    BOOST_CHECK_EQUAL(charmander1, "Female");
    std::string charmander2 = pkmn::calculations::gen2_pokemon_gender(
                                  "Charmander", 2
                              );
    BOOST_CHECK_EQUAL(charmander2, "Male");

    /*
     * 75% male, 25% female
     */
    std::string growlithe1 = pkmn::calculations::gen2_pokemon_gender(
                                 "Growlithe", 3
                             );
    BOOST_CHECK_EQUAL(growlithe1, "Female");
    std::string growlithe2 = pkmn::calculations::gen2_pokemon_gender(
                                 "Growlithe", 4
                             );
    BOOST_CHECK_EQUAL(growlithe2, "Male");

    /*
     * 50% male, 50% female
     */
    std::string pidgey1 = pkmn::calculations::gen2_pokemon_gender(
                              "Pidgey", 6
                          );
    BOOST_CHECK_EQUAL(pidgey1, "Female");
    std::string pidgey2 = pkmn::calculations::gen2_pokemon_gender(
                              "Pidgey", 7
                          );
    BOOST_CHECK_EQUAL(pidgey2, "Male");

    /*
     * 25% male, 75% female
     */
    std::string vulpix1 = pkmn::calculations::gen2_pokemon_gender(
                              "Vulpix", 11
                          );
    BOOST_CHECK_EQUAL(vulpix1, "Female");
    std::string vulpix2 = pkmn::calculations::gen2_pokemon_gender(
                              "Vulpix", 12
                          );
    BOOST_CHECK_EQUAL(vulpix2, "Male");

    /*
     * All female
     */
    std::string nidorina1 = pkmn::calculations::gen2_pokemon_gender(
                                "Nidorina", 0
                            );
    BOOST_CHECK_EQUAL(nidorina1, "Female");
    std::string nidorina2 = pkmn::calculations::gen2_pokemon_gender(
                                "Nidorina", 15
                            );
    BOOST_CHECK_EQUAL(nidorina2, "Female");

    /*
     * Genderless
     */
    std::string magnemite1 = pkmn::calculations::gen2_pokemon_gender(
                                 "Magnemite", 0
                             );
    BOOST_CHECK_EQUAL(magnemite1, "Genderless");
    std::string magnemite2 = pkmn::calculations::gen2_pokemon_gender(
                                 "Magnemite", 15
                             );
    BOOST_CHECK_EQUAL(magnemite2, "Genderless");
}

BOOST_AUTO_TEST_CASE(modern_gender_test) {
    /*
     * Make sure expected exceptions are thrown
     */
    BOOST_CHECK_THROW(
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
    BOOST_CHECK_EQUAL(nidorino1, "Male");
    std::string nidorino2 = pkmn::calculations::modern_pokemon_gender(
                                "Nidorino",
                                std::numeric_limits<uint32_t>::max()
                            );
    BOOST_CHECK_EQUAL(nidorino2, "Male");

    /*
     * 87.5% male, 12.5% female
     */
    std::string charmander1 = pkmn::calculations::modern_pokemon_gender(
                                  "Charmander", 30
                              );
    BOOST_CHECK_EQUAL(charmander1, "Female");
    std::string charmander2 = pkmn::calculations::modern_pokemon_gender(
                                  "Charmander", 31
                              );
    BOOST_CHECK_EQUAL(charmander2, "Male");

    /*
     * 75% male, 25% female
     */
    std::string growlithe1 = pkmn::calculations::modern_pokemon_gender(
                                 "Growlithe", 63
                             );
    BOOST_CHECK_EQUAL(growlithe1, "Female");
    std::string growlithe2 = pkmn::calculations::modern_pokemon_gender(
                                 "Growlithe", 64
                             );
    BOOST_CHECK_EQUAL(growlithe2, "Male");

    /*
     * 50% male, 50% female
     */
    std::string pidgey1 = pkmn::calculations::modern_pokemon_gender(
                              "Pidgey", 126
                          );
    BOOST_CHECK_EQUAL(pidgey1, "Female");
    std::string pidgey2 = pkmn::calculations::modern_pokemon_gender(
                              "Pidgey", 127
                          );
    BOOST_CHECK_EQUAL(pidgey2, "Male");

    /*
     * 25% male, 75% female
     */
    std::string vulpix1 = pkmn::calculations::modern_pokemon_gender(
                              "Vulpix", 190
                          );
    BOOST_CHECK_EQUAL(vulpix1, "Female");
    std::string vulpix2 = pkmn::calculations::modern_pokemon_gender(
                              "Vulpix", 191
                          );
    BOOST_CHECK_EQUAL(vulpix2, "Male");

    /*
     * All female
     */
    std::string nidorina1 = pkmn::calculations::modern_pokemon_gender(
                                "Nidorina", 0
                            );
    BOOST_CHECK_EQUAL(nidorina1, "Female");
    std::string nidorina2 = pkmn::calculations::modern_pokemon_gender(
                                "Nidorina",
                                std::numeric_limits<uint32_t>::max()
                            );
    BOOST_CHECK_EQUAL(nidorina2, "Female");

    /*
     * Genderless
     */
    std::string magnemite1 = pkmn::calculations::modern_pokemon_gender(
                                 "Magnemite", 0
                             );
    BOOST_CHECK_EQUAL(magnemite1, "Genderless");
    std::string magnemite2 = pkmn::calculations::modern_pokemon_gender(
                                 "Magnemite",
                                std::numeric_limits<uint32_t>::max()
                             );
    BOOST_CHECK_EQUAL(magnemite2, "Genderless");
}

BOOST_AUTO_TEST_CASE(gen2_hidden_power_test) {
    pkmn::calculations::hidden_power hidden_power;
    /*
     * Make sure expected exceptions are thrown
     */
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           -1, 0, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           16, 0, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, -1, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, 16, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, 0, -1, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, 0, 16, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::gen2_hidden_power(
                           0, 0, 0, -1
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
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
    BOOST_CHECK_EQUAL(hidden_power.type, "Dark");
    BOOST_CHECK_EQUAL(hidden_power.base_power, 69);
}

BOOST_AUTO_TEST_CASE(modern_hidden_power_test) {
    pkmn::calculations::hidden_power hidden_power;
    /*
     * Make sure expected exceptions are thrown
     */
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           -1, 0, 0, 0, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           32, 0, 0, 0, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, -1, 0, 0, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 32, 0, 0, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, -1, 0, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 32, 0, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, -1, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, 32, 0, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, 0, -1, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, 0, 32, 0
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        hidden_power = pkmn::calculations::modern_hidden_power(
                           0, 0, 0, 0, 0, -1
                       );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
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
    BOOST_CHECK_EQUAL(hidden_power.type, "Grass");
    BOOST_CHECK_EQUAL(hidden_power.base_power, 70);
}

BOOST_AUTO_TEST_CASE(gen2_shiny_test) {
    bool shiny = false;
    (void)shiny;

    /*
     * Make sure expected exceptions are thrown
     */
    BOOST_CHECK_THROW(
        shiny = pkmn::calculations::gen2_shiny(
                    -1, 0, 0, 0
                );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        shiny = pkmn::calculations::gen2_shiny(
                    16, 0, 0, 0
                );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        shiny = pkmn::calculations::gen2_shiny(
                    0, -1, 0, 0
                );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        shiny = pkmn::calculations::gen2_shiny(
                    0, 16, 0, 0
                );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        shiny = pkmn::calculations::gen2_shiny(
                    0, 0, -1, 0
                );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        shiny = pkmn::calculations::gen2_shiny(
                    0, 0, 16, 0
                );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        shiny = pkmn::calculations::gen2_shiny(
                    0, 0, 0, -1
                );
    , pkmn::range_error);
    BOOST_CHECK_THROW(
        shiny = pkmn::calculations::gen2_shiny(
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
    BOOST_CHECK(shiny1);
    BOOST_CHECK(not shiny2);
}

BOOST_AUTO_TEST_CASE(modern_shiny_test) {
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
    BOOST_CHECK(shiny1);
    BOOST_CHECK(shiny2);
}

BOOST_AUTO_TEST_CASE(spinda_coords_test) {
    /*
     * Check (in)equality operators.
     */
    BOOST_STATIC_CONSTEXPR pkmn::calculations::spinda_coords coords1(123,456);
    BOOST_STATIC_CONSTEXPR pkmn::calculations::spinda_coords coords2(123,456);
    BOOST_STATIC_CONSTEXPR pkmn::calculations::spinda_coords coords3(456,123);

    BOOST_CHECK(coords1 == coords2);
    BOOST_CHECK(not (coords1 != coords2));
    BOOST_CHECK(coords1 != coords3);
    BOOST_CHECK(not (coords1 == coords3));
}

BOOST_AUTO_TEST_CASE(spinda_spot_test) {
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
    BOOST_CHECK(SPOTS1 == SPOTS2);
    BOOST_CHECK(SPOTS1 != SPOTS3);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: https://github.com/magical/spinda
     */

    pkmn::calculations::spinda_spots spots1 = pkmn::calculations::spinda_spot_offset(
                                                  4064348759
                                              );
    BOOST_CHECK(spots1.left_ear   == SPOTS1.left_ear);
    BOOST_CHECK(spots1.right_ear  == SPOTS1.right_ear);
    BOOST_CHECK(spots1.left_face  == SPOTS1.left_face);
    BOOST_CHECK(spots1.right_face == SPOTS1.right_face);
}

/*
 * Off-by-one is acceptable for stat checks, but BOOST_CHECK_CLOSE
 * is for floats or doubles.
 */
PKMN_INLINE void PKMN_CHECK_STAT_CLOSE(
    int actual, int expected
) {
    BOOST_CHECK(int(std::abs(actual-expected)) <= 1);
}

BOOST_AUTO_TEST_CASE(gb_stat_test) {
    int stat = 0;
    (void)stat;

    /*
     * Make sure expected exceptions are thrown
     */

    // Invalid stat
    BOOST_CHECK_THROW(
        stat = pkmn::calculations::get_gb_stat(
                   "Not a stat", 1, 1, 1, 1
               );
    , std::invalid_argument);

    // Invalid EV
    BOOST_CHECK_THROW(
        stat = pkmn::calculations::get_gb_stat(
                   "Attack", 1, 1, 123456, 1
               );
    , pkmn::range_error);

    // Invalid IV
    BOOST_CHECK_THROW(
        stat = pkmn::calculations::get_gb_stat(
                   "Attack", 1, 1, 1, 12345
               );
    , pkmn::range_error);

    /*
     * Test with known good inputs.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
     */
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "HP", 81, 35, 22850, 7
        ), 189
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Attack", 81, 55, 23140, 8
        ), 137
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Defense", 81, 30, 17280, 13
        ), 101
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Special", 81, 50, 19625, 9
        ), 128
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Special Attack", 81, 50, 19625, 9
        ), 128
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Special Defense", 81, 40, 19625, 9
        ), 112
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_gb_stat(
            "Speed", 81, 90, 24795, 5
        ), 190
    );
}

BOOST_AUTO_TEST_CASE(modern_stat_test) {
    int stat = 0;
    (void)stat;

    /*
     * Make sure expected exceptions are thrown
     */

    // Invalid stat
    BOOST_CHECK_THROW(
        stat = pkmn::calculations::get_modern_stat(
                   "Not a stat", 1, 1.0f, 1, 1, 1
               );
    , std::invalid_argument);

    // No Special past Generation I
    BOOST_CHECK_THROW(
        stat = pkmn::calculations::get_modern_stat(
                   "Special", 1, 1.0f, 1, 1, 1
               );
    , std::invalid_argument);

    // Invalid nature modifier
    BOOST_CHECK_THROW(
        stat = pkmn::calculations::get_modern_stat(
                   "Attack", 1, 0.666f, 1, 1, 1
               );
    , std::domain_error);

    // Invalid EV
    BOOST_CHECK_THROW(
        stat = pkmn::calculations::get_modern_stat(
                   "Attack", 1, 1.0f, 1, 12345, 1
               );
    , pkmn::range_error);

    // Invalid IV
    BOOST_CHECK_THROW(
        stat = pkmn::calculations::get_modern_stat(
                   "Attack", 1, 1.0f, 1, 1, 12345
               );
    , pkmn::range_error);

    /*
     * Test with known good inputs.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
     */
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "HP", 78, 1.0f, 108, 74, 24
        ), 289
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Attack", 78, 1.1f, 130, 195, 12
        ), 280
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Defense", 78, 1.0f, 95, 86, 30
        ), 193
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Special Attack", 78, 0.9f, 80, 48, 16
        ), 136
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Special Defense", 78, 1.0f, 85, 84, 23
        ), 171
    );
    PKMN_CHECK_STAT_CLOSE(
        pkmn::calculations::get_modern_stat(
            "Speed", 78, 1.0f, 102, 23, 5
        ), 172
    );
}
