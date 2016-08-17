/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/database/lists.hpp>

#include <boost/test/unit_test.hpp>

#include <algorithm>

static PKMN_INLINE bool string_in_vector(
    const std::vector<std::string> &vec,
    const std::string &str
) {
    return (std::find(vec.begin(), vec.end(), str) != vec.end());
}

BOOST_AUTO_TEST_CASE(ability_list_test) {
    std::vector<std::string> abilities2, abilities3,
                             abilities4, abilities5,
                             abilities6, abilities7;

    /*
     * Make sure invalid generations throw an exception.
     */
    BOOST_CHECK_THROW(
        pkmn::database::get_ability_list(
            2, abilities2
        );
    , std::out_of_range);
    BOOST_CHECK_THROW(
        pkmn::database::get_ability_list(
            7, abilities7
        );
    , std::out_of_range);

    /*
     * Make sure each generation adds the appropriate abilities.
     */
    pkmn::database::get_ability_list(
        3, abilities3
    );
    BOOST_CHECK(not string_in_vector(abilities3, "Adaptability"));

    pkmn::database::get_ability_list(
        4, abilities4
    );
    BOOST_CHECK(string_in_vector(abilities4, "Adaptability"));
    BOOST_CHECK(not string_in_vector(abilities4, "Analytic"));

    pkmn::database::get_ability_list(
        5, abilities5
    );
    BOOST_CHECK(string_in_vector(abilities5, "Analytic"));
    BOOST_CHECK(not string_in_vector(abilities5, "Aerilate"));

    pkmn::database::get_ability_list(
        6, abilities6
    );
    BOOST_CHECK(string_in_vector(abilities6, "Aerilate"));

    BOOST_CHECK(abilities3.size() > 0);
    BOOST_CHECK(abilities3.size() < abilities4.size());
    BOOST_CHECK(abilities4.size() < abilities5.size());
    BOOST_CHECK(abilities5.size() < abilities6.size());
}

BOOST_AUTO_TEST_CASE(game_list_test) {
    std::vector<std::string> games0, games1,
                             games2, games3,
                             games4, games5,
                             games6, games7;

    /*
     * Make sure invalid generations throw an exception.
     */
    BOOST_CHECK_THROW(
        pkmn::database::get_game_list(
            0, true, games0
        );
    , std::out_of_range);
    BOOST_CHECK_THROW(
        pkmn::database::get_game_list(
            7, true, games7
        );
    , std::out_of_range);

    /*
     * Generation I
     */
    pkmn::database::get_game_list(
        1, false, games1
    );
    BOOST_CHECK_EQUAL(games1.size(), 3);
    pkmn::database::get_game_list(
        1, true, games1
    );
    BOOST_CHECK_EQUAL(games1.size(), 3);

    /*
     * Generation II
     */
    pkmn::database::get_game_list(
        2, false, games2
    );
    BOOST_CHECK_EQUAL(games2.size(), 3);
    pkmn::database::get_game_list(
        2, true, games2
    );
    BOOST_CHECK_EQUAL(games2.size(), 6);

    /*
     * Generation III
     */
    pkmn::database::get_game_list(
        3, false, games3
    );
    BOOST_CHECK_EQUAL(games3.size(), 7);
    pkmn::database::get_game_list(
        3, true, games3
    );
    BOOST_CHECK_EQUAL(games3.size(), 13);

    /*
     * Generation IV
     */
    pkmn::database::get_game_list(
        4, false, games4
    );
    BOOST_CHECK_EQUAL(games4.size(), 5);
    pkmn::database::get_game_list(
        4, true, games4
    );
    BOOST_CHECK_EQUAL(games4.size(), 18);

    /*
     * Generation V
     */
    pkmn::database::get_game_list(
        5, false, games5
    );
    BOOST_CHECK_EQUAL(games5.size(), 4);
    pkmn::database::get_game_list(
        5, true, games5
    );
    BOOST_CHECK_EQUAL(games5.size(), 22);

    /*
     * Generation VI
     */
    pkmn::database::get_game_list(
        6, false, games6
    );
    BOOST_CHECK_EQUAL(games6.size(), 4);
    pkmn::database::get_game_list(
        6, true, games6
    );
    BOOST_CHECK_EQUAL(games6.size(), 26);
}

BOOST_AUTO_TEST_CASE(item_list_test) {
    /*
     * Make sure invalid games fail.
     */
    std::vector<std::string> items_bad;
    BOOST_CHECK_THROW(
        pkmn::database::get_item_list(
            "Not a game", items_bad
        )
    , std::invalid_argument);

    /*
     * Generation I
     */
    std::vector<std::string> items_rb, items_y;
    pkmn::database::get_item_list(
        "Red", items_rb
    );
    pkmn::database::get_item_list(
        "Yellow", items_y
    );
    BOOST_CHECK(items_rb == items_y);

    /*
     * Generation II
     */
    std::vector<std::string> items_gs, items_c;
    pkmn::database::get_item_list(
        "Gold", items_gs
    );
    pkmn::database::get_item_list(
        "Crystal", items_c
    );
    BOOST_CHECK(items_gs.size() < items_c.size());
    BOOST_CHECK(not string_in_vector(items_gs, "GS Ball"));
    BOOST_CHECK(string_in_vector(items_c, "GS Ball"));
    BOOST_CHECK(not string_in_vector(items_gs, "Black Apricorn"));
    BOOST_CHECK(string_in_vector(items_gs, "Blk Apricorn"));

    /*
     * Generation III
     */
    std::vector<std::string> items_rs, items_e, items_frlg,
                             items_colo, items_xd;
    pkmn::database::get_item_list(
        "Ruby", items_rs
    );
    pkmn::database::get_item_list(
        "Emerald", items_e
    );
    pkmn::database::get_item_list(
        "FireRed", items_frlg
    );
    pkmn::database::get_item_list(
        "Colosseum", items_colo
    );
    pkmn::database::get_item_list(
        "XD", items_xd
    );

    BOOST_CHECK(not string_in_vector(items_rs, "Magma Emblem"));
    BOOST_CHECK(string_in_vector(items_e, "Magma Emblem"));
    BOOST_CHECK(not string_in_vector(items_frlg, "Magma Emblem"));
    BOOST_CHECK(not string_in_vector(items_colo, "Magma Emblem"));
    BOOST_CHECK(not string_in_vector(items_xd, "Magma Emblem"));

    BOOST_CHECK(not string_in_vector(items_rs, "Helix Fossil"));
    BOOST_CHECK(not string_in_vector(items_e, "Helix Fossil"));
    BOOST_CHECK(string_in_vector(items_frlg, "Helix Fossil"));
    BOOST_CHECK(not string_in_vector(items_colo, "Helix Fossil"));
    BOOST_CHECK(not string_in_vector(items_xd, "Helix Fossil"));

    BOOST_CHECK(not string_in_vector(items_rs, "Time Flute"));
    BOOST_CHECK(not string_in_vector(items_e, "Time Flute"));
    BOOST_CHECK(not string_in_vector(items_frlg, "Time Flute"));
    BOOST_CHECK(string_in_vector(items_colo, "Time Flute"));
    BOOST_CHECK(not string_in_vector(items_xd, "Time Flute"));

    BOOST_CHECK(not string_in_vector(items_rs, "Battle CD 01"));
    BOOST_CHECK(not string_in_vector(items_e, "Battle CD 01"));
    BOOST_CHECK(not string_in_vector(items_frlg, "Battle CD 01"));
    BOOST_CHECK(not string_in_vector(items_colo, "Battle CD 01"));
    BOOST_CHECK(string_in_vector(items_xd, "Battle CD 01"));

    /*
     * Generation IV
     */
    std::vector<std::string> items_dp, items_p, items_hgss;
    pkmn::database::get_item_list(
        "Diamond", items_dp
    );
    pkmn::database::get_item_list(
        "Platinum", items_p
    );
    pkmn::database::get_item_list(
        "HeartGold", items_hgss
    );

    BOOST_CHECK(not string_in_vector(items_dp, "Griseous Orb"));
    BOOST_CHECK(string_in_vector(items_p, "Griseous Orb"));
    BOOST_CHECK(string_in_vector(items_hgss, "Griseous Orb"));

    BOOST_CHECK(not string_in_vector(items_dp, "Dowsing MCHN"));
    BOOST_CHECK(not string_in_vector(items_p, "Dowsing MCHN"));
    BOOST_CHECK(string_in_vector(items_hgss, "Dowsing MCHN"));

    /*
     * Generation V
     */
    std::vector<std::string> items_bw, items_b2w2;
    pkmn::database::get_item_list(
        "Black", items_bw
    );
    pkmn::database::get_item_list(
        "Black 2", items_b2w2
    );

    BOOST_CHECK(not string_in_vector(items_bw, "Colress MCHN"));
    BOOST_CHECK(string_in_vector(items_b2w2, "Colress MCHN"));

    /*
     * Generation VI
     */
    std::vector<std::string> items_xy, items_oras;
    pkmn::database::get_item_list(
        "X", items_xy
    );
    pkmn::database::get_item_list(
        "Omega Ruby", items_oras
    );

    BOOST_CHECK(string_in_vector(items_xy, "Bicycle"));
    BOOST_CHECK(not string_in_vector(items_oras, "Bicycle"));

    BOOST_CHECK(not string_in_vector(items_xy, "Bike"));
    BOOST_CHECK(string_in_vector(items_oras, "Bike"));

    BOOST_CHECK(not string_in_vector(items_xy, "Slowbronite"));
    BOOST_CHECK(string_in_vector(items_oras, "Slowbronite"));
}

BOOST_AUTO_TEST_CASE(location_list_test) {
    // TODO: make sure function checks for bad game input
    //       test good cases, whole_generation parameter
}

BOOST_AUTO_TEST_CASE(move_list_test) {
    // TODO: make sure function checks for bad game input
    //       test good cases
}

BOOST_AUTO_TEST_CASE(nature_list_test) {
    // TODO: test good cases
}

BOOST_AUTO_TEST_CASE(pokemon_list_test) {
    // TODO: make sure function checks for bad generation
    //       test good cases, make sure include_previous parameter works
}

BOOST_AUTO_TEST_CASE(region_list_test) {
    // TODO: confirm length
}

BOOST_AUTO_TEST_CASE(ribbon_list_test) {
    // TODO: make sure function checks for bad generation
    //       test good cases
}

BOOST_AUTO_TEST_CASE(super_training_medal_list_test) {
    // TODO: confirm length
}

BOOST_AUTO_TEST_CASE(type_list_test) {
    // TODO: make sure function checks for bad game
    //       make sure Shadow only appears for Gamecube games
}
