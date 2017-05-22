/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>
#include <pkmn/database/lists.hpp>

#include <gtest/gtest.h>

#include <algorithm>

static PKMN_INLINE bool string_in_vector(
    const std::vector<std::string> &vec,
    const std::string &str
) {
    return (std::find(vec.begin(), vec.end(), str) != vec.end());
}

TEST(cpp_lists_test, ability_list_test) {
    std::vector<std::string> abilities3, abilities4,
                             abilities5, abilities6;

    EXPECT_THROW(
        (void)pkmn::database::get_ability_list(2);
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::database::get_ability_list(7);
    , pkmn::range_error);

    abilities3 = pkmn::database::get_ability_list(3);
    EXPECT_FALSE(string_in_vector(abilities3, "Adaptability"));

    abilities4 = pkmn::database::get_ability_list(4);
    EXPECT_TRUE(string_in_vector(abilities4, "Adaptability"));
    EXPECT_FALSE(string_in_vector(abilities4, "Analytic"));

    abilities5 = pkmn::database::get_ability_list(5);
    EXPECT_TRUE(string_in_vector(abilities5, "Analytic"));
    EXPECT_FALSE(string_in_vector(abilities5, "Aerilate"));

    abilities6 = pkmn::database::get_ability_list(6);
    EXPECT_TRUE(string_in_vector(abilities6, "Aerilate"));

    EXPECT_TRUE(abilities3.size() > 0);
    EXPECT_TRUE(abilities3.size() < abilities4.size());
    EXPECT_TRUE(abilities4.size() < abilities5.size());
    EXPECT_TRUE(abilities5.size() < abilities6.size());
}

TEST(cpp_lists_test, game_list_test) {
    std::vector<std::string> games1, games2,
                             games3, games4,
                             games5, games6;

    /*
     * Make sure invalid generations throw an exception.
     */
    EXPECT_THROW(
        (void)pkmn::database::get_game_list(0, true);
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::database::get_game_list(7, true);
    , pkmn::range_error);

    /*
     * Generation I
     */
    games1 = pkmn::database::get_game_list(1, false);
    EXPECT_EQ(3, games1.size());
    games1 = pkmn::database::get_game_list(1, true);
    EXPECT_EQ(3, games1.size());

    /*
     * Generation II
     */
    games2 = pkmn::database::get_game_list(2, false);
    EXPECT_EQ(3, games2.size());
    games2 = pkmn::database::get_game_list(2, true);
    EXPECT_EQ(6, games2.size());

    /*
     * Generation III
     */
    games3 = pkmn::database::get_game_list(3, false);
    EXPECT_EQ(7, games3.size());
    games3 = pkmn::database::get_game_list(3, true);
    EXPECT_EQ(13, games3.size());

    /*
     * Generation IV
     */
    games4 = pkmn::database::get_game_list(4, false);
    EXPECT_EQ(5, games4.size());
    games4 = pkmn::database::get_game_list(4, true);
    EXPECT_EQ(18, games4.size());

    /*
     * Generation V
     */
    games5 = pkmn::database::get_game_list(5, false);
    EXPECT_EQ(4, games5.size());
    games5 = pkmn::database::get_game_list(5, true);
    EXPECT_EQ(22, games5.size());

    /*
     * Generation VI
     */
    games6 = pkmn::database::get_game_list(6, false);
    EXPECT_EQ(4, games6.size());
    games6 = pkmn::database::get_game_list(6, true);
    EXPECT_EQ(26, games6.size());
}

TEST(cpp_lists_test, gamecube_shadow_pokemon_list_test)
{
    std::vector<std::string> colosseum_shadow_pokemon = pkmn::database::get_gamecube_shadow_pokemon_list(true);
    EXPECT_EQ(48, colosseum_shadow_pokemon.size());

    std::vector<std::string> xd_shadow_pokemon = pkmn::database::get_gamecube_shadow_pokemon_list(false);
    EXPECT_EQ(83, xd_shadow_pokemon.size());
}

TEST(cpp_lists_test, item_list_test) {
    /*
     * Make sure invalid games fail.
     */
    EXPECT_THROW(
        (void)pkmn::database::get_item_list("Not a game");
    , std::invalid_argument);

    /*
     * Generation I
     */
    std::vector<std::string> items_rb = pkmn::database::get_item_list("Red");
    std::vector<std::string> items_y  = pkmn::database::get_item_list("Yellow");
    EXPECT_TRUE(items_rb == items_y);

    /*
     * Generation II
     */
    std::vector<std::string> items_gs = pkmn::database::get_item_list("Gold");
    std::vector<std::string> items_c  = pkmn::database::get_item_list("Crystal");
    EXPECT_LT(items_gs.size(), items_c.size());
    EXPECT_FALSE(string_in_vector(items_gs, "GS Ball"));
    EXPECT_TRUE(string_in_vector(items_c, "GS Ball"));
    EXPECT_FALSE(string_in_vector(items_gs, "Black Apricorn"));
    EXPECT_TRUE(string_in_vector(items_gs, "Blk Apricorn"));

    /*
     * Generation III
     */
    std::vector<std::string> items_rs   = pkmn::database::get_item_list("Ruby");
    std::vector<std::string> items_e    = pkmn::database::get_item_list("Emerald");
    std::vector<std::string> items_frlg = pkmn::database::get_item_list("FireRed");
    std::vector<std::string> items_colo = pkmn::database::get_item_list("Colosseum");
    std::vector<std::string> items_xd   = pkmn::database::get_item_list("XD");

    EXPECT_TRUE(string_in_vector(items_rs, "Potion"));
    EXPECT_TRUE(string_in_vector(items_e, "Potion"));
    EXPECT_TRUE(string_in_vector(items_frlg, "Potion"));
    EXPECT_TRUE(string_in_vector(items_colo, "Potion"));
    EXPECT_TRUE(string_in_vector(items_xd, "Potion"));

    EXPECT_FALSE(string_in_vector(items_rs, "Magma Emblem"));
    EXPECT_TRUE(string_in_vector(items_e, "Magma Emblem"));
    EXPECT_FALSE(string_in_vector(items_frlg, "Magma Emblem"));
    EXPECT_FALSE(string_in_vector(items_colo, "Magma Emblem"));
    EXPECT_FALSE(string_in_vector(items_xd, "Magma Emblem"));

    EXPECT_FALSE(string_in_vector(items_rs, "Helix Fossil"));
    EXPECT_FALSE(string_in_vector(items_e, "Helix Fossil"));
    EXPECT_TRUE(string_in_vector(items_frlg, "Helix Fossil"));
    EXPECT_FALSE(string_in_vector(items_colo, "Helix Fossil"));
    EXPECT_FALSE(string_in_vector(items_xd, "Helix Fossil"));

    EXPECT_FALSE(string_in_vector(items_rs, "Time Flute"));
    EXPECT_FALSE(string_in_vector(items_e, "Time Flute"));
    EXPECT_FALSE(string_in_vector(items_frlg, "Time Flute"));
    EXPECT_TRUE(string_in_vector(items_colo, "Time Flute"));
    EXPECT_FALSE(string_in_vector(items_xd, "Time Flute"));

    EXPECT_FALSE(string_in_vector(items_rs, "Battle CD 01"));
    EXPECT_FALSE(string_in_vector(items_e, "Battle CD 01"));
    EXPECT_FALSE(string_in_vector(items_frlg, "Battle CD 01"));
    EXPECT_FALSE(string_in_vector(items_colo, "Battle CD 01"));
    EXPECT_TRUE(string_in_vector(items_xd, "Battle CD 01"));

    /*
     * Generation IV
     */
    std::vector<std::string> items_dp   = pkmn::database::get_item_list("Diamond");
    std::vector<std::string> items_p    = pkmn::database::get_item_list("Platinum");
    std::vector<std::string> items_hgss = pkmn::database::get_item_list("HeartGold");

    EXPECT_FALSE(string_in_vector(items_dp, "Griseous Orb"));
    EXPECT_TRUE(string_in_vector(items_p, "Griseous Orb"));
    EXPECT_TRUE(string_in_vector(items_hgss, "Griseous Orb"));

    EXPECT_FALSE(string_in_vector(items_dp, "Dowsing MCHN"));
    EXPECT_FALSE(string_in_vector(items_p, "Dowsing MCHN"));
    EXPECT_TRUE(string_in_vector(items_hgss, "Dowsing MCHN"));

    /*
     * Generation V
     */
    std::vector<std::string> items_bw   = pkmn::database::get_item_list("Black");
    std::vector<std::string> items_b2w2 = pkmn::database::get_item_list("Black 2");

    EXPECT_FALSE(string_in_vector(items_bw, "Colress MCHN"));
    EXPECT_TRUE(string_in_vector(items_b2w2, "Colress MCHN"));

    /*
     * Generation VI
     */
    std::vector<std::string> items_xy   = pkmn::database::get_item_list("X");
    std::vector<std::string> items_oras = pkmn::database::get_item_list("Omega Ruby");

    EXPECT_FALSE(string_in_vector(items_xy, "Slowbronite"));
    EXPECT_TRUE(string_in_vector(items_oras, "Slowbronite"));
}

TEST(cpp_lists_test, location_list_test) {
    std::vector<std::string> locations_gen1,
                             locations_gs, locations_c,
                             locations_rs, locations_e,
                             locations_frlg, locations_colo,
                             locations_xd, locations_dp,
                             locations_pt, locations_hgss,
                             locations_bw, locations_b2w2,
                             locations_xy, locations_oras;

    /*
     * Make sure invalid games fail.
     */
    EXPECT_THROW(
        (void)pkmn::database::get_location_list("Not a game", true);
    , std::invalid_argument);

    /*
     * Generation I (TODO)
     */
    locations_gen1 = pkmn::database::get_location_list("Red", true);
    /*EXPECT_GT(locations_gen1.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_gen1, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_gen1, "Cerulean Cave"));*/

    locations_gen1 = pkmn::database::get_location_list("Red", false);
    /*EXPECT_GT(locations_gen1.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_gen1, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_gen1, "Cerulean Cave"));*/

    /*
     * Gold/Silver
     */
    locations_gs = pkmn::database::get_location_list("Silver", true);
    EXPECT_GT(locations_gs.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_gs, "Sprout Tower"));
    EXPECT_TRUE(string_in_vector(locations_gs, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_gs, "Battle Tower"));

    locations_gs = pkmn::database::get_location_list("Silver", false);
    EXPECT_GT(locations_gs.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_gs, "Sprout Tower"));
    EXPECT_TRUE(string_in_vector(locations_gs, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_gs, "Battle Tower"));

    /*
     * Crystal
     */
    locations_c = pkmn::database::get_location_list("Crystal", true);
    EXPECT_GT(locations_c.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_c, "Sprout Tower"));
    EXPECT_TRUE(string_in_vector(locations_c, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_c, "Battle Tower"));

    locations_c = pkmn::database::get_location_list("Crystal", false);
    EXPECT_GT(locations_c.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_c, "Sprout Tower"));
    EXPECT_TRUE(string_in_vector(locations_c, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_c, "Battle Tower"));

    /*
     * Ruby/Sapphire
     */
    locations_rs = pkmn::database::get_location_list("Ruby", true);
    EXPECT_GT(locations_rs.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_rs, "New Mauville"));
    EXPECT_TRUE(string_in_vector(locations_rs, "Artisan Cave"));
    EXPECT_TRUE(string_in_vector(locations_rs, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Pyrite Town"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Kaminko's House"));

    locations_rs = pkmn::database::get_location_list("Ruby", false);
    EXPECT_GT(locations_rs.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_rs, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Kaminko's House"));

    /*
     * Emerald
     */
    locations_e = pkmn::database::get_location_list("Emerald", true);
    EXPECT_GT(locations_e.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_e, "New Mauville"));
    EXPECT_TRUE(string_in_vector(locations_e, "Artisan Cave"));
    EXPECT_TRUE(string_in_vector(locations_e, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_e, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_e, "Kaminko's House"));

    locations_e = pkmn::database::get_location_list("Emerald", false);
    EXPECT_GT(locations_e.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_e, "New Mauville"));
    EXPECT_TRUE(string_in_vector(locations_e, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_e, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_e, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_e, "Kaminko's House"));

    /*
     * FireRed/LeafGreen
     */
    locations_frlg = pkmn::database::get_location_list("LeafGreen", true);
    EXPECT_GT(locations_frlg.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_frlg, "New Mauville"));
    EXPECT_TRUE(string_in_vector(locations_frlg, "Artisan Cave"));
    EXPECT_TRUE(string_in_vector(locations_frlg, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Kaminko's House"));

    locations_frlg = pkmn::database::get_location_list("LeafGreen", false);
    EXPECT_GT(locations_frlg.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_frlg, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Artisan Cave"));
    EXPECT_TRUE(string_in_vector(locations_frlg, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Kaminko's House"));

    /*
     * Colosseum
     */
    locations_colo = pkmn::database::get_location_list("Colosseum", true);
    EXPECT_GT(locations_colo.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_colo, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_colo, "Realgamtwr Dome"));
    EXPECT_TRUE(string_in_vector(locations_colo, "Kaminko's House"));

    locations_colo = pkmn::database::get_location_list("Colosseum", false);
    EXPECT_GT(locations_colo.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_colo, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_colo, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Kaminko's House"));

    /*
     * XD
     */
    locations_xd = pkmn::database::get_location_list("XD", true);
    EXPECT_GT(locations_xd.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_xd, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_xd, "Realgamtwr Dome"));
    EXPECT_TRUE(string_in_vector(locations_xd, "Kaminko's House"));

    locations_xd = pkmn::database::get_location_list("XD", false);
    EXPECT_GT(locations_xd.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_xd, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Realgamtwr Dome"));
    EXPECT_TRUE(string_in_vector(locations_xd, "Kaminko's House"));

    /*
     * Diamond/Pearl
     */
    locations_dp = pkmn::database::get_location_list("Diamond", true);
    EXPECT_GT(locations_dp.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_dp, "Route 221"));
    EXPECT_TRUE(string_in_vector(locations_dp, "Distortion World"));
    EXPECT_TRUE(string_in_vector(locations_dp, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_dp, "PC Tokyo"));

    locations_dp = pkmn::database::get_location_list("Diamond", false);
    EXPECT_GT(locations_dp.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_dp, "Route 221"));
    EXPECT_FALSE(string_in_vector(locations_dp, "Distortion World"));
    EXPECT_FALSE(string_in_vector(locations_dp, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_dp, "PC Tokyo"));

    /*
     * Platinum
     */
    locations_pt = pkmn::database::get_location_list("Platinum", true);
    EXPECT_GT(locations_pt.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_pt, "Route 221"));
    EXPECT_TRUE(string_in_vector(locations_pt, "Distortion World"));
    EXPECT_TRUE(string_in_vector(locations_pt, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_pt, "PC Tokyo"));

    locations_pt = pkmn::database::get_location_list("Platinum", false);
    EXPECT_GT(locations_pt.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_pt, "Route 221"));
    EXPECT_TRUE(string_in_vector(locations_pt, "Distortion World"));
    EXPECT_FALSE(string_in_vector(locations_pt, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_pt, "PC Tokyo"));

    /*
     * HeartGold/SoulSilver
     */
    locations_hgss = pkmn::database::get_location_list("HeartGold", true);
    EXPECT_GT(locations_hgss.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_hgss, "Route 221"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "Distortion World"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "PC Tokyo"));

    locations_hgss = pkmn::database::get_location_list("HeartGold", false);
    EXPECT_GT(locations_hgss.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_hgss, "Route 221"));
    EXPECT_FALSE(string_in_vector(locations_hgss, "Distortion World"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "PC Tokyo"));

    /*
     * Black/White
     */
    locations_bw = pkmn::database::get_location_list("White", true);
    EXPECT_GT(locations_bw.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_bw, "Cold Storage"));
    EXPECT_TRUE(string_in_vector(locations_bw, "PWT"));
    EXPECT_TRUE(string_in_vector(locations_bw, "Castelia Sewers"));
    EXPECT_TRUE(string_in_vector(locations_bw, "PC Tokyo"));

    locations_bw = pkmn::database::get_location_list("White", false);
    EXPECT_GT(locations_bw.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_bw, "Cold Storage"));
    EXPECT_FALSE(string_in_vector(locations_bw, "PWT"));
    EXPECT_FALSE(string_in_vector(locations_bw, "Castelia Sewers"));
    EXPECT_TRUE(string_in_vector(locations_bw, "PC Tokyo"));

    /*
     * Black 2/White 2
     */
    locations_b2w2 = pkmn::database::get_location_list("White 2", true);
    EXPECT_GT(locations_b2w2.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_b2w2, "Cold Storage"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "PWT"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "Castelia Sewers"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "PC Tokyo"));

    locations_b2w2 = pkmn::database::get_location_list("White 2", false);
    EXPECT_GT(locations_b2w2.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_b2w2, "Cold Storage"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "PWT"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "Castelia Sewers"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "PC Tokyo"));

    /*
     * X/Y
     */
    locations_xy = pkmn::database::get_location_list("X", true);
    EXPECT_GT(locations_xy.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_xy, "Zubat Roost"));
    EXPECT_TRUE(string_in_vector(locations_xy, "Route 101"));
    EXPECT_TRUE(string_in_vector(locations_xy, "PC Tokyo"));

    locations_xy = pkmn::database::get_location_list("X", false);
    EXPECT_GT(locations_xy.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_xy, "Zubat Roost"));
    EXPECT_FALSE(string_in_vector(locations_xy, "Route 101"));
    EXPECT_TRUE(string_in_vector(locations_xy, "PC Tokyo"));

    /*
     * Omega Ruby/Alpha Sapphire
     */
    locations_oras = pkmn::database::get_location_list("Omega Ruby", true);
    EXPECT_GT(locations_oras.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_oras, "Zubat Roost"));
    EXPECT_TRUE(string_in_vector(locations_oras, "Route 101"));
    EXPECT_TRUE(string_in_vector(locations_oras, "PC Tokyo"));

    locations_oras = pkmn::database::get_location_list("Omega Ruby", false);
    EXPECT_GT(locations_oras.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_oras, "Zubat Roost"));
    EXPECT_TRUE(string_in_vector(locations_oras, "Route 101"));
    EXPECT_TRUE(string_in_vector(locations_oras, "PC Tokyo"));
}

TEST(cpp_lists_test, move_list_test) {
    std::vector<std::string> moves_gen1,
                             moves_gen2, moves_gba,
                             moves_colo, moves_xd,
                             moves_gen4, moves_gen5,
                             moves_xy,   moves_oras;

    /*
     * Make sure invalid games fail.
     */
    EXPECT_THROW(
        (void)pkmn::database::get_type_list("Not a game");
    , std::invalid_argument);

    /*
     * Generation I
     */
    moves_gen1 = pkmn::database::get_move_list("Red");
    EXPECT_EQ(165, moves_gen1.size());
    EXPECT_EQ("Pound", moves_gen1.front());
    EXPECT_EQ("Struggle", moves_gen1.back());

    /*
     * Generation II
     */
    moves_gen2 = pkmn::database::get_move_list("Gold");
    EXPECT_EQ(251, moves_gen2.size());
    EXPECT_EQ("Pound", moves_gen2.front());
    EXPECT_EQ("Beat Up", moves_gen2.back());

    /*
     * Game Boy Advance
     */
    moves_gba = pkmn::database::get_move_list("Sapphire");
    EXPECT_EQ(354, moves_gba.size());
    EXPECT_EQ("Pound", moves_gba.front());
    EXPECT_EQ("Psycho Boost", moves_gba.back());

    /*
     * Colosseum
     */
    moves_colo = pkmn::database::get_move_list("Colosseum");
    EXPECT_EQ(355, moves_colo.size());
    EXPECT_EQ("Pound", moves_colo.front());
    EXPECT_EQ("Shadow Rush", moves_colo.back());

    /*
     * XD
     */
    moves_xd = pkmn::database::get_move_list("XD");
    EXPECT_EQ(372, moves_xd.size());
    EXPECT_EQ("Pound", moves_xd.front());
    EXPECT_EQ("Shadow Sky", moves_xd.back());

    /*
     * Generation IV
     */
    moves_gen4 = pkmn::database::get_move_list("SoulSilver");
    EXPECT_EQ(467, moves_gen4.size());
    EXPECT_EQ("Pound", moves_gen4.front());
    EXPECT_EQ("Shadow Force", moves_gen4.back());

    /*
     * Generation V
     */
    moves_gen5 = pkmn::database::get_move_list("White");
    EXPECT_EQ(559, moves_gen5.size());
    EXPECT_EQ("Pound", moves_gen5.front());
    EXPECT_EQ("Fusion Bolt", moves_gen5.back());

    /*
     * X/Y
     */
    moves_xy = pkmn::database::get_move_list("Y");
    EXPECT_EQ(617, moves_xy.size());
    EXPECT_EQ("Pound", moves_xy.front());
    EXPECT_EQ("Light of Ruin", moves_xy.back());

    /*
     * Omega Ruby/Alpha Sapphire
     */
    moves_oras = pkmn::database::get_move_list("Omega Ruby");
    EXPECT_EQ(621, moves_oras.size());
    EXPECT_EQ("Pound", moves_oras.front());
    EXPECT_EQ("Hyperspace Fury", moves_oras.back());
}

TEST(cpp_lists_test, nature_list_test) {
    std::vector<std::string> natures = pkmn::database::get_nature_list();
    EXPECT_EQ(25, natures.size());
    EXPECT_EQ("Hardy", natures.front());
    EXPECT_EQ("Quirky", natures.back());
}

TEST(cpp_lists_test, pokemon_list_test) {
    std::vector<std::string> pokemon1, pokemon2,
                             pokemon3, pokemon4,
                             pokemon5, pokemon6;

    EXPECT_THROW(
        (void)pkmn::database::get_pokemon_list(0, true);
    , pkmn::range_error);
    EXPECT_THROW(
        (void)pkmn::database::get_pokemon_list(7, true);
    , pkmn::range_error);

    /*
     * Generation I
     */
    pokemon1 = pkmn::database::get_pokemon_list(1, true);
    EXPECT_EQ(151, pokemon1.size());
    EXPECT_EQ("Bulbasaur", pokemon1.front());
    EXPECT_EQ("Mew", pokemon1.back());

    pokemon1 = pkmn::database::get_pokemon_list(1, false);
    EXPECT_EQ(151, pokemon1.size());
    EXPECT_EQ("Bulbasaur", pokemon1.front());
    EXPECT_EQ("Mew", pokemon1.back());

    /*
     * Generation II
     */
    pokemon2 = pkmn::database::get_pokemon_list(2, true);
    EXPECT_EQ(251, pokemon2.size());
    EXPECT_EQ("Bulbasaur", pokemon2.front());
    EXPECT_EQ("Celebi", pokemon2.back());

    pokemon2 = pkmn::database::get_pokemon_list(2, false);
    EXPECT_EQ(100, pokemon2.size());
    EXPECT_EQ("Chikorita", pokemon2.front());
    EXPECT_EQ("Celebi", pokemon2.back());

    /*
     * Generation III
     */
    pokemon3 = pkmn::database::get_pokemon_list(3, true);
    EXPECT_EQ(386, pokemon3.size());
    EXPECT_EQ("Bulbasaur", pokemon3.front());
    EXPECT_EQ("Deoxys", pokemon3.back());

    pokemon3 = pkmn::database::get_pokemon_list(3, false);
    EXPECT_EQ(135, pokemon3.size());
    EXPECT_EQ("Treecko", pokemon3.front());
    EXPECT_EQ("Deoxys", pokemon3.back());

    /*
     * Generation IV
     */
    pokemon4 = pkmn::database::get_pokemon_list(4, true);
    EXPECT_EQ(493, pokemon4.size());
    EXPECT_EQ("Bulbasaur", pokemon4.front());
    EXPECT_EQ("Arceus", pokemon4.back());

    pokemon4 = pkmn::database::get_pokemon_list(4, false);
    EXPECT_EQ(107, pokemon4.size());
    EXPECT_EQ("Turtwig", pokemon4.front());
    EXPECT_EQ("Arceus", pokemon4.back());

    /*
     * Generation V
     */
    pokemon5 = pkmn::database::get_pokemon_list(5, true);
    EXPECT_EQ(649, pokemon5.size());
    EXPECT_EQ("Bulbasaur", pokemon5.front());
    EXPECT_EQ("Genesect", pokemon5.back());

    pokemon5 = pkmn::database::get_pokemon_list(5, false);
    EXPECT_EQ(156, pokemon5.size());
    EXPECT_EQ("Victini", pokemon5.front());
    EXPECT_EQ("Genesect", pokemon5.back());

    /*
     * Generation VI
     */
    pokemon6 = pkmn::database::get_pokemon_list(6, true);
    EXPECT_EQ(721, pokemon6.size());
    EXPECT_EQ("Bulbasaur", pokemon6.front());
    EXPECT_EQ("Volcanion", pokemon6.back());

    pokemon6 = pkmn::database::get_pokemon_list(6, false);
    EXPECT_EQ(72, pokemon6.size());
    EXPECT_EQ("Chespin", pokemon6.front());
    EXPECT_EQ("Volcanion", pokemon6.back());
}

TEST(cpp_lists_test, region_list_test) {
    std::vector<std::string> regions = pkmn::database::get_region_list();
    EXPECT_EQ(7, regions.size());
    EXPECT_EQ("Kanto", regions.front());
    EXPECT_EQ("Orre", regions[3]);
    EXPECT_EQ("Kalos", regions.back());
}

TEST(cpp_lists_test, ribbon_list_test) {
    // TODO: make sure function checks for bad generation
    //       test good cases
}

TEST(cpp_lists_test, super_training_medal_list_test) {
    std::vector<std::string> super_training_medals = \
        pkmn::database::get_super_training_medal_list();

    EXPECT_EQ(30, super_training_medals.size());
}

TEST(cpp_lists_test, type_list_test) {
    std::vector<std::string> types_gen1,
                             types_gen2, types_gba,
                             types_gcn,  types_gen4,
                             types_gen5, types_gen6;

    /*
     * Make sure invalid games fail.
     */
    EXPECT_THROW(
        (void)pkmn::database::get_type_list("Not a game");
    , std::invalid_argument);

    /*
     * Generation I
     */
    types_gen1 = pkmn::database::get_type_list("Red");

    EXPECT_EQ(15, types_gen1.size());
    EXPECT_FALSE(string_in_vector(types_gen1, "Dark"));
    EXPECT_FALSE(string_in_vector(types_gen1, "Steel"));
    EXPECT_FALSE(string_in_vector(types_gen1, "???"));
    EXPECT_FALSE(string_in_vector(types_gen1, "Shadow"));
    EXPECT_FALSE(string_in_vector(types_gen1, "Fairy"));

    /*
     * Generation II
     */
    types_gen2 = pkmn::database::get_type_list("Silver");
    EXPECT_EQ(18, types_gen2.size());
    EXPECT_TRUE(string_in_vector(types_gen2, "Dark"));
    EXPECT_TRUE(string_in_vector(types_gen2, "Steel"));
    EXPECT_TRUE(string_in_vector(types_gen2, "???"));
    EXPECT_FALSE(string_in_vector(types_gen2, "Shadow"));
    EXPECT_FALSE(string_in_vector(types_gen2, "Fairy"));

    /*
     * Game Boy Advance
     */
    types_gba = pkmn::database::get_type_list("Emerald");
    EXPECT_EQ(18, types_gba.size());
    EXPECT_TRUE(string_in_vector(types_gba, "Dark"));
    EXPECT_TRUE(string_in_vector(types_gba, "Steel"));
    EXPECT_TRUE(string_in_vector(types_gba, "???"));
    EXPECT_FALSE(string_in_vector(types_gba, "Shadow"));
    EXPECT_FALSE(string_in_vector(types_gba, "Fairy"));

    /*
     * Gamecube
     */
    types_gcn = pkmn::database::get_type_list("XD");
    EXPECT_EQ(19, types_gcn.size());
    EXPECT_TRUE(string_in_vector(types_gcn, "Dark"));
    EXPECT_TRUE(string_in_vector(types_gcn, "Steel"));
    EXPECT_TRUE(string_in_vector(types_gcn, "???"));
    EXPECT_TRUE(string_in_vector(types_gcn, "Shadow"));
    EXPECT_FALSE(string_in_vector(types_gcn, "Fairy"));

    /*
     * Generation IV
     */
    types_gen4 = pkmn::database::get_type_list("Platinum");
    EXPECT_EQ(18, types_gen4.size());
    EXPECT_TRUE(string_in_vector(types_gen4, "Dark"));
    EXPECT_TRUE(string_in_vector(types_gen4, "Steel"));
    EXPECT_TRUE(string_in_vector(types_gen4, "???"));
    EXPECT_FALSE(string_in_vector(types_gen4, "Shadow"));
    EXPECT_FALSE(string_in_vector(types_gen4, "Fairy"));

    /*
     * Generation V
     */
    types_gen5 = pkmn::database::get_type_list("White 2");
    EXPECT_EQ(17, types_gen5.size());
    EXPECT_TRUE(string_in_vector(types_gen5, "Dark"));
    EXPECT_TRUE(string_in_vector(types_gen5, "Steel"));
    EXPECT_FALSE(string_in_vector(types_gen5, "???"));
    EXPECT_FALSE(string_in_vector(types_gen5, "Shadow"));
    EXPECT_FALSE(string_in_vector(types_gen5, "Fairy"));

    /*
     * Generation VI
     */
    types_gen6 = pkmn::database::get_type_list("X");
    EXPECT_EQ(18, types_gen6.size());
    EXPECT_TRUE(string_in_vector(types_gen6, "Dark"));
    EXPECT_TRUE(string_in_vector(types_gen6, "Steel"));
    EXPECT_FALSE(string_in_vector(types_gen6, "???"));
    EXPECT_FALSE(string_in_vector(types_gen6, "Shadow"));
    EXPECT_TRUE(string_in_vector(types_gen6, "Fairy"));
}
