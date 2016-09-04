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

    BOOST_CHECK_THROW(
        abilities2 = pkmn::database::get_ability_list(2);
    , std::out_of_range);
    BOOST_CHECK_THROW(
        abilities7 = pkmn::database::get_ability_list(7);
    , std::out_of_range);

    abilities3 = pkmn::database::get_ability_list(3);
    BOOST_CHECK(not string_in_vector(abilities3, "Adaptability"));

    abilities4 = pkmn::database::get_ability_list(4);
    BOOST_CHECK(string_in_vector(abilities4, "Adaptability"));
    BOOST_CHECK(not string_in_vector(abilities4, "Analytic"));

    abilities5 = pkmn::database::get_ability_list(5);
    BOOST_CHECK(string_in_vector(abilities5, "Analytic"));
    BOOST_CHECK(not string_in_vector(abilities5, "Aerilate"));

    abilities6 = pkmn::database::get_ability_list(6);
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
        games0 = pkmn::database::get_game_list(0, true);
    , std::out_of_range);
    BOOST_CHECK_THROW(
        games7 = pkmn::database::get_game_list(7, true);
    , std::out_of_range);

    /*
     * Generation I
     */
    games1 = pkmn::database::get_game_list(1, false);
    BOOST_CHECK_EQUAL(games1.size(), 3);
    games1 = pkmn::database::get_game_list(1, true);
    BOOST_CHECK_EQUAL(games1.size(), 3);

    /*
     * Generation II
     */
    games2 = pkmn::database::get_game_list(2, false);
    BOOST_CHECK_EQUAL(games2.size(), 3);
    games2 = pkmn::database::get_game_list(2, true);
    BOOST_CHECK_EQUAL(games2.size(), 6);

    /*
     * Generation III
     */
    games3 = pkmn::database::get_game_list(3, false);
    BOOST_CHECK_EQUAL(games3.size(), 7);
    games3 = pkmn::database::get_game_list(3, true);
    BOOST_CHECK_EQUAL(games3.size(), 13);

    /*
     * Generation IV
     */
    games4 = pkmn::database::get_game_list(4, false);
    BOOST_CHECK_EQUAL(games4.size(), 5);
    games4 = pkmn::database::get_game_list(4, true);
    BOOST_CHECK_EQUAL(games4.size(), 18);

    /*
     * Generation V
     */
    games5 = pkmn::database::get_game_list(5, false);
    BOOST_CHECK_EQUAL(games5.size(), 4);
    games5 = pkmn::database::get_game_list(5, true);
    BOOST_CHECK_EQUAL(games5.size(), 22);

    /*
     * Generation VI
     */
    games6 = pkmn::database::get_game_list(6, false);
    BOOST_CHECK_EQUAL(games6.size(), 4);
    games6 = pkmn::database::get_game_list(6, true);
    BOOST_CHECK_EQUAL(games6.size(), 26);
}

BOOST_AUTO_TEST_CASE(item_list_test) {
    /*
     * Make sure invalid games fail.
     */
    std::vector<std::string> items_bad;
    BOOST_CHECK_THROW(
        items_bad = pkmn::database::get_item_list("Not a game");
    , std::invalid_argument);

    /*
     * Generation I
     */
    std::vector<std::string> items_rb = pkmn::database::get_item_list("Red");
    std::vector<std::string> items_y  = pkmn::database::get_item_list("Yellow");
    BOOST_CHECK(items_rb == items_y);

    /*
     * Generation II
     */
    std::vector<std::string> items_gs = pkmn::database::get_item_list("Gold");
    std::vector<std::string> items_c  = pkmn::database::get_item_list("Crystal");
    BOOST_CHECK(items_gs.size() < items_c.size());
    BOOST_CHECK(not string_in_vector(items_gs, "GS Ball"));
    BOOST_CHECK(string_in_vector(items_c, "GS Ball"));
    BOOST_CHECK(not string_in_vector(items_gs, "Black Apricorn"));
    BOOST_CHECK(string_in_vector(items_gs, "Blk Apricorn"));

    /*
     * Generation III
     */
    std::vector<std::string> items_rs   = pkmn::database::get_item_list("Ruby");
    std::vector<std::string> items_e    = pkmn::database::get_item_list("Emerald");
    std::vector<std::string> items_frlg = pkmn::database::get_item_list("FireRed");
    std::vector<std::string> items_colo = pkmn::database::get_item_list("Colosseum");
    std::vector<std::string> items_xd   = pkmn::database::get_item_list("XD");

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
    std::vector<std::string> items_dp   = pkmn::database::get_item_list("Diamond");
    std::vector<std::string> items_p    = pkmn::database::get_item_list("Platinum");
    std::vector<std::string> items_hgss = pkmn::database::get_item_list("HeartGold");

    BOOST_CHECK(not string_in_vector(items_dp, "Griseous Orb"));
    BOOST_CHECK(string_in_vector(items_p, "Griseous Orb"));
    BOOST_CHECK(string_in_vector(items_hgss, "Griseous Orb"));

    BOOST_CHECK(not string_in_vector(items_dp, "Dowsing MCHN"));
    BOOST_CHECK(not string_in_vector(items_p, "Dowsing MCHN"));
    BOOST_CHECK(string_in_vector(items_hgss, "Dowsing MCHN"));

    /*
     * Generation V
     */
    std::vector<std::string> items_bw   = pkmn::database::get_item_list("Black");
    std::vector<std::string> items_b2w2 = pkmn::database::get_item_list("Black 2");

    BOOST_CHECK(not string_in_vector(items_bw, "Colress MCHN"));
    BOOST_CHECK(string_in_vector(items_b2w2, "Colress MCHN"));

    /*
     * Generation VI
     */
    std::vector<std::string> items_xy   = pkmn::database::get_item_list("X");
    std::vector<std::string> items_oras = pkmn::database::get_item_list("Omega Ruby");

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
    std::vector<std::string> natures = pkmn::database::get_nature_list();
    BOOST_CHECK_EQUAL(natures.size(), 25);
}

BOOST_AUTO_TEST_CASE(pokemon_list_test) {
    std::vector<std::string> pokemon0, pokemon1,
                             pokemon2, pokemon3,
                             pokemon4, pokemon5,
                             pokemon6, pokemon7;

    BOOST_CHECK_THROW(
        pokemon0 = pkmn::database::get_pokemon_list(0, true);
    , std::out_of_range);
    BOOST_CHECK_THROW(
        pokemon7 = pkmn::database::get_pokemon_list(7, true);
    , std::out_of_range);

    /*
     * Generation I
     */
    pokemon1 = pkmn::database::get_pokemon_list(1, true);
    BOOST_CHECK_EQUAL(pokemon1.size(), 151);
    BOOST_CHECK_EQUAL(pokemon1.front(), "Bulbasaur");
    BOOST_CHECK_EQUAL(pokemon1.back(), "Mew");

    pokemon1 = pkmn::database::get_pokemon_list(1, false);
    BOOST_CHECK_EQUAL(pokemon1.size(), 151);
    BOOST_CHECK_EQUAL(pokemon1.front(), "Bulbasaur");
    BOOST_CHECK_EQUAL(pokemon1.back(), "Mew");

    /*
     * Generation II
     */
    pokemon2 = pkmn::database::get_pokemon_list(2, true);
    BOOST_CHECK_EQUAL(pokemon2.size(),  251);
    BOOST_CHECK_EQUAL(pokemon2.front(), "Bulbasaur");
    BOOST_CHECK_EQUAL(pokemon2.back(),  "Celebi");

    pokemon2 = pkmn::database::get_pokemon_list(2, false);
    BOOST_CHECK_EQUAL(pokemon2.size(),  100);
    BOOST_CHECK_EQUAL(pokemon2.front(), "Chikorita");
    BOOST_CHECK_EQUAL(pokemon2.back(),  "Celebi");

    /*
     * Generation III
     */
    pokemon3 = pkmn::database::get_pokemon_list(3, true);
    BOOST_CHECK_EQUAL(pokemon3.size(),  386);
    BOOST_CHECK_EQUAL(pokemon3.front(), "Bulbasaur");
    BOOST_CHECK_EQUAL(pokemon3.back(),  "Deoxys");

    pokemon3 = pkmn::database::get_pokemon_list(3, false);
    BOOST_CHECK_EQUAL(pokemon3.size(),  135);
    BOOST_CHECK_EQUAL(pokemon3.front(), "Treecko");
    BOOST_CHECK_EQUAL(pokemon3.back(),  "Deoxys");

    /*
     * Generation IV
     */
    pokemon4 = pkmn::database::get_pokemon_list(4, true);
    BOOST_CHECK_EQUAL(pokemon4.size(),  493);
    BOOST_CHECK_EQUAL(pokemon4.front(), "Bulbasaur");
    BOOST_CHECK_EQUAL(pokemon4.back(),  "Arceus");

    pokemon4 = pkmn::database::get_pokemon_list(4, false);
    BOOST_CHECK_EQUAL(pokemon4.size(),  107);
    BOOST_CHECK_EQUAL(pokemon4.front(), "Turtwig");
    BOOST_CHECK_EQUAL(pokemon4.back(),  "Arceus");

    /*
     * Generation V
     */
    pokemon5 = pkmn::database::get_pokemon_list(5, true);
    BOOST_CHECK_EQUAL(pokemon5.size(),  649);
    BOOST_CHECK_EQUAL(pokemon5.front(), "Bulbasaur");
    BOOST_CHECK_EQUAL(pokemon5.back(),  "Genesect");

    pokemon5 = pkmn::database::get_pokemon_list(5, false);
    BOOST_CHECK_EQUAL(pokemon5.size(),  156);
    BOOST_CHECK_EQUAL(pokemon5.front(), "Victini");
    BOOST_CHECK_EQUAL(pokemon5.back(),  "Genesect");

    /*
     * Generation VI
     */
    pokemon6 = pkmn::database::get_pokemon_list(6, true);
    BOOST_CHECK_EQUAL(pokemon6.size(),  721);
    BOOST_CHECK_EQUAL(pokemon6.front(), "Bulbasaur");
    BOOST_CHECK_EQUAL(pokemon6.back(),  "Volcanion");

    pokemon6 = pkmn::database::get_pokemon_list(6, false);
    BOOST_CHECK_EQUAL(pokemon6.size(),  72);
    BOOST_CHECK_EQUAL(pokemon6.front(), "Chespin");
    BOOST_CHECK_EQUAL(pokemon6.back(),  "Volcanion");
}

BOOST_AUTO_TEST_CASE(region_list_test) {
    std::vector<std::string> regions = pkmn::database::get_region_list();
    BOOST_CHECK_EQUAL(regions.size(), 7);
    BOOST_CHECK_EQUAL(regions.front(), "Kanto");
    BOOST_CHECK_EQUAL(regions.back(), "Kalos");
    BOOST_CHECK(string_in_vector(regions, "Orre"));
}

BOOST_AUTO_TEST_CASE(ribbon_list_test) {
    // TODO: make sure function checks for bad generation
    //       test good cases
}

BOOST_AUTO_TEST_CASE(super_training_medal_list_test) {
    // TODO: confirm length
}

BOOST_AUTO_TEST_CASE(type_list_test) {
    std::vector<std::string> types_bad,  types_gen1,
                             types_gen2, types_gba,
                             types_gcn,  types_gen4,
                             types_gen5, types_gen6;

    /*
     * Make sure invalid games fail.
     */
    BOOST_CHECK_THROW(
        types_bad = pkmn::database::get_type_list("Not a game");
    , std::invalid_argument);

    /*
     * Generation I
     */
    types_gen1 = pkmn::database::get_type_list("Red");
    BOOST_CHECK_EQUAL(types_gen1.size(), 15);
    BOOST_CHECK(not string_in_vector(types_gen1, "Dark"));
    BOOST_CHECK(not string_in_vector(types_gen1, "Steel"));
    BOOST_CHECK(not string_in_vector(types_gen1, "???"));
    BOOST_CHECK(not string_in_vector(types_gen1, "Shadow"));
    BOOST_CHECK(not string_in_vector(types_gen1, "Fairy"));

    /*
     * Generation II
     */
    types_gen2 = pkmn::database::get_type_list("Silver");
    BOOST_CHECK_EQUAL(types_gen2.size(), 18);
    BOOST_CHECK(string_in_vector(types_gen2, "Dark"));
    BOOST_CHECK(string_in_vector(types_gen2, "Steel"));
    BOOST_CHECK(string_in_vector(types_gen2, "???"));
    BOOST_CHECK(not string_in_vector(types_gen2, "Shadow"));
    BOOST_CHECK(not string_in_vector(types_gen2, "Fairy"));

    /*
     * Game Boy Advance
     */
    types_gba = pkmn::database::get_type_list("Emerald");
    BOOST_CHECK_EQUAL(types_gba.size(), 18);
    BOOST_CHECK(string_in_vector(types_gba, "Dark"));
    BOOST_CHECK(string_in_vector(types_gba, "Steel"));
    BOOST_CHECK(string_in_vector(types_gba, "???"));
    BOOST_CHECK(not string_in_vector(types_gba, "Shadow"));
    BOOST_CHECK(not string_in_vector(types_gba, "Fairy"));

    /*
     * Gamecube
     */
    types_gcn = pkmn::database::get_type_list("XD");
    BOOST_CHECK_EQUAL(types_gcn.size(), 19);
    BOOST_CHECK(string_in_vector(types_gcn, "Dark"));
    BOOST_CHECK(string_in_vector(types_gcn, "Steel"));
    BOOST_CHECK(string_in_vector(types_gcn, "???"));
    BOOST_CHECK(string_in_vector(types_gcn, "Shadow"));
    BOOST_CHECK(not string_in_vector(types_gcn, "Fairy"));

    /*
     * Generation IV
     */
    types_gen4 = pkmn::database::get_type_list("Platinum");
    BOOST_CHECK_EQUAL(types_gen4.size(), 19);
    BOOST_CHECK(string_in_vector(types_gen4, "Dark"));
    BOOST_CHECK(string_in_vector(types_gen4, "Steel"));
    BOOST_CHECK(string_in_vector(types_gen4, "???"));
    BOOST_CHECK(string_in_vector(types_gen4, "Shadow"));
    BOOST_CHECK(not string_in_vector(types_gen4, "Fairy"));

    /*
     * Generation V
     */
    types_gen5 = pkmn::database::get_type_list("Platinum");
    BOOST_CHECK_EQUAL(types_gen5.size(), 17);
    BOOST_CHECK(string_in_vector(types_gen5, "Dark"));
    BOOST_CHECK(string_in_vector(types_gen5, "Steel"));
    BOOST_CHECK(not string_in_vector(types_gen5, "???"));
    BOOST_CHECK(not string_in_vector(types_gen5, "Shadow"));
    BOOST_CHECK(not string_in_vector(types_gen5, "Fairy"));

    /*
     * Generation VI
     */
    types_gen6 = pkmn::database::get_type_list("Platinum");
    BOOST_CHECK_EQUAL(types_gen6.size(), 18);
    BOOST_CHECK(string_in_vector(types_gen6, "Dark"));
    BOOST_CHECK(string_in_vector(types_gen6, "Steel"));
    BOOST_CHECK(not string_in_vector(types_gen6, "???"));
    BOOST_CHECK(not string_in_vector(types_gen6, "Shadow"));
    BOOST_CHECK(string_in_vector(types_gen6, "Fairy"));
}
