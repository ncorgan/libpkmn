/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_database_entry_test.hpp"

// Only create one main
#undef BOOST_TEST_MAIN

#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>

static void _item_entry_none_test(
    const pkmn::database::item_entry &none_entry
) {
    BOOST_CHECK_EQUAL(none_entry.get_name(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_category(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_pocket(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_description(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_cost(), -1);
    BOOST_CHECK(not none_entry.holdable());
    BOOST_CHECK_EQUAL(none_entry.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(none_entry.get_fling_effect(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_item_id(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_item_index(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_item_list_id(), 0);
}

/*
 * Make sure "None" item entries output expected results for all generations.
 */
BOOST_AUTO_TEST_CASE(item_entry_none_test) {
    for(int i = 0; i < 6; ++i) {
        pkmn::database::item_entry none_byindex(0, game_ids[i]);
        pkmn::database::item_entry none_byname("None", game_names[i]);

        _item_entry_none_test(none_byindex);
        _item_entry_none_test(none_byname);
    }
}

/*
 * Make sure item entries can't be instantiated for games that didn't
 * have the given item.
 */
BOOST_AUTO_TEST_CASE(item_entry_wrong_game_test) {
    // Items from later generations
    BOOST_CHECK_THROW(
        pkmn::database::item_entry pink_bow(
            "Pink Bow", "Red"
        )
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::item_entry razz_berry(
            "Razz Berry", "Silver"
        )
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::item_entry flame_plate(
            "Flame Plate", "Emerald"
        )
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::item_entry dna_splicers(
            "DNA Splicers", "Pearl"
        )
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::item_entry mega_ring(
            "Mega Ring", "White"
        )
    , std::invalid_argument);

    // Items that were only in certain games within a generation
    BOOST_CHECK_THROW(
        pkmn::database::item_entry gs_ball(
            "GS Ball", "Gold"
        )
    , std::invalid_argument);
    pkmn::database::item_entry gs_ball(
        "GS Ball", "Crystal"
    );

    BOOST_CHECK_THROW(
        pkmn::database::item_entry f_disk(
            "F-Disk", "Ruby"
        )
    , std::invalid_argument);
    pkmn::database::item_entry f_disk(
        "F-Disk", "Colosseum"
    );

    // Items that didn't make it into later generations
    pkmn::database::item_entry pink_bow1(
        "Pink Bow", "Silver"
    );
    BOOST_CHECK_THROW(
        pkmn::database::item_entry pink_bow2(
            "Pink Bow", "Ruby"
        )
    , std::invalid_argument);
}

/*
 * Make sure item entries can be created from any name
 * the item has ever had, even from different games.
 */
BOOST_AUTO_TEST_CASE(item_entry_different_name_test) {
    /*
     * Test an item that changed only once.
     */

    pkmn::database::item_entry elixir1("Elixer", "Red");
    pkmn::database::item_entry elixir2("Elixir", "Red");
    BOOST_CHECK(elixir1 == elixir2);
    BOOST_CHECK_EQUAL(elixir1.get_name(), "Elixer");
    BOOST_CHECK_EQUAL(elixir2.get_name(), "Elixer");

    pkmn::database::item_entry elixir3("Elixir", "X");
    pkmn::database::item_entry elixir4("Elixer", "X");
    BOOST_CHECK(elixir3 == elixir4);
    BOOST_CHECK_EQUAL(elixir3.get_name(), "Elixir");
    BOOST_CHECK_EQUAL(elixir4.get_name(), "Elixir");

    BOOST_CHECK(elixir1 != elixir3);

    /*
     * Test an item that changed twice.
     */

    pkmn::database::item_entry dowsing_machine1("Itemfinder", "Red");
    pkmn::database::item_entry dowsing_machine2("Dowsing MCHN", "Red");
    pkmn::database::item_entry dowsing_machine3("Dowsing Machine", "Red");
    BOOST_CHECK(dowsing_machine1 == dowsing_machine2);
    BOOST_CHECK(dowsing_machine2 == dowsing_machine3);
    BOOST_CHECK_EQUAL(dowsing_machine1.get_name(), "Itemfinder");
    BOOST_CHECK_EQUAL(dowsing_machine2.get_name(), "Itemfinder");
    BOOST_CHECK_EQUAL(dowsing_machine3.get_name(), "Itemfinder");

    pkmn::database::item_entry dowsing_machine4("Itemfinder", "HeartGold");
    pkmn::database::item_entry dowsing_machine5("Dowsing MCHN", "HeartGold");
    pkmn::database::item_entry dowsing_machine6("Dowsing Machine", "HeartGold");
    BOOST_CHECK(dowsing_machine4 == dowsing_machine5);
    BOOST_CHECK(dowsing_machine5 == dowsing_machine6);
    BOOST_CHECK_EQUAL(dowsing_machine4.get_name(), "Dowsing MCHN");
    BOOST_CHECK_EQUAL(dowsing_machine5.get_name(), "Dowsing MCHN");
    BOOST_CHECK_EQUAL(dowsing_machine6.get_name(), "Dowsing MCHN");

    pkmn::database::item_entry dowsing_machine7("Itemfinder", "X");
    pkmn::database::item_entry dowsing_machine8("Dowsing MCHN", "X");
    pkmn::database::item_entry dowsing_machine9("Dowsing Machine", "X");
    BOOST_CHECK(dowsing_machine7 == dowsing_machine8);
    BOOST_CHECK(dowsing_machine8 == dowsing_machine9);
    BOOST_CHECK_EQUAL(dowsing_machine7.get_name(), "Dowsing Machine");
    BOOST_CHECK_EQUAL(dowsing_machine8.get_name(), "Dowsing Machine");
    BOOST_CHECK_EQUAL(dowsing_machine9.get_name(), "Dowsing Machine");

    BOOST_CHECK(dowsing_machine1 != dowsing_machine4);
    BOOST_CHECK(dowsing_machine4 != dowsing_machine7);
    BOOST_CHECK(dowsing_machine1 != dowsing_machine7);
}

static void _item_entry_invalid_index_test(
    const pkmn::database::item_entry &entry
) {
    BOOST_CHECK_EQUAL(entry.get_name(), "Invalid (0x258)");
    BOOST_CHECK_EQUAL(entry.get_category(), "Unknown");
    BOOST_CHECK_EQUAL(entry.get_pocket(), "Unknown");
    BOOST_CHECK_EQUAL(entry.get_description(), "Invalid (0x258)");
    BOOST_CHECK_EQUAL(entry.get_cost(), -1);
    BOOST_CHECK(not entry.holdable());
    BOOST_CHECK_EQUAL(entry.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(entry.get_fling_effect(), (entry.get_game_id() == 20 ? "None"
                                                                           : "Unknown"));
}

/*
 * Make sure item entries created from invalid item slots don't
 * try to query the database for information that doesn't
 * exist.
 */
BOOST_AUTO_TEST_CASE(item_entry_invalid_index_test) {
    pkmn::database::item_entry invalid(600, 15);
    pkmn::database::item_entry invalid_gcn(600, 20);
    _item_entry_invalid_index_test(invalid);
    _item_entry_invalid_index_test(invalid_gcn);
}

static PKMN_INLINE void tmhm_entry_check_move(
    const pkmn::database::item_entry &tmhm_entry,
    const std::string &move
) {
    static boost::format tmhm_desc("Teaches the move %s.");
    BOOST_CHECK_EQUAL(tmhm_entry.get_description(), str(tmhm_desc % move.c_str()));
}

/*
 * Make sure TM/HM item entries place the correct corresponding
 * move in the description, even if the item name is the same.
 */
BOOST_AUTO_TEST_CASE(item_entry_tmhm_test) {
    pkmn::database::item_entry tm16_gen1(
        "TM16", "Red"
    );
    pkmn::database::item_entry tm16_gen2(
        "TM16", "Silver"
    );
    pkmn::database::item_entry tm16_gen3(
        "TM16", "XD"
    );
    tmhm_entry_check_move(tm16_gen1, "Pay Day");
    tmhm_entry_check_move(tm16_gen2, "Icy Wind");
    tmhm_entry_check_move(tm16_gen3, "Light Screen");

    pkmn::database::item_entry tm83_gen1(
        "TM83", "Platinum"
    );
    pkmn::database::item_entry tm83_gen2(
        "TM83", "White"
    );
    pkmn::database::item_entry tm83_gen3(
        "TM83", "Omega Ruby"
    );
    tmhm_entry_check_move(tm83_gen1, "Natural Gift");
    tmhm_entry_check_move(tm83_gen2, "Workup");
    tmhm_entry_check_move(tm83_gen3, "Infestation");

    pkmn::database::item_entry tm83_xy(
        "TM94", "Y"
    );
    pkmn::database::item_entry tm83_oras(
        "TM94", "Alpha Sapphire"
    );
    tmhm_entry_check_move(tm83_xy, "Rock Smash");
    tmhm_entry_check_move(tm83_oras, "Secret Power");

    pkmn::database::item_entry hm05_gen3(
        "HM05", "Emerald"
    );
    pkmn::database::item_entry hm05_dppt(
        "HM05", "Diamond"
    );
    pkmn::database::item_entry hm05_hgss(
        "HM05", "SoulSilver"
    );
    pkmn::database::item_entry hm05_gen5(
        "HM05", "Black"
    );
    tmhm_entry_check_move(hm05_gen3, "Flash");
    tmhm_entry_check_move(hm05_dppt, "Defog");
    tmhm_entry_check_move(hm05_hgss, "Whirlpool");
    tmhm_entry_check_move(hm05_gen5, "Waterfall");
}

static void _item_entry_test_main(
    const pkmn::database::item_entry &item_entry_gen1,
    const pkmn::database::item_entry &item_entry_gen2,
    const pkmn::database::item_entry &item_entry_gba,
    const pkmn::database::item_entry &item_entry_gcn,
    const pkmn::database::item_entry &item_entry_gen4,
    const pkmn::database::item_entry &item_entry_gen5,
    const pkmn::database::item_entry &item_entry_gen6
) {
    /*
     * For descriptions, just make sure it doesn't fail. If
     * there is no description in the database for the corresponding
     * game, it should fallback onto the X/Y description.
     */

    // Generation I entry
    BOOST_CHECK_EQUAL(item_entry_gen1.get_name(), "Potion");
    BOOST_CHECK_EQUAL(item_entry_gen1.get_game(), "Red");
    BOOST_CHECK_EQUAL(item_entry_gen1.get_category(), "Healing");
    BOOST_CHECK_EQUAL(item_entry_gen1.get_pocket(), "Items");
    (void)item_entry_gen1.get_description();
    BOOST_CHECK_EQUAL(item_entry_gen1.get_cost(), 300);
    BOOST_CHECK(not item_entry_gen1.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen1.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(item_entry_gen1.get_fling_effect(), "None");

    // Generation II entry
    BOOST_CHECK_EQUAL(item_entry_gen2.get_name(), "Pink Bow");
    BOOST_CHECK_EQUAL(item_entry_gen2.get_game(), "Silver");
    BOOST_CHECK_EQUAL(item_entry_gen2.get_category(), "Stat boosts");
    BOOST_CHECK_EQUAL(item_entry_gen2.get_pocket(), "Items");
    (void)item_entry_gen2.get_description();
    BOOST_CHECK_EQUAL(item_entry_gen2.get_cost(), 50);
    BOOST_CHECK(item_entry_gen2.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen2.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(item_entry_gen2.get_fling_effect(), "None");

    // GBA entry
    BOOST_CHECK_EQUAL(item_entry_gba.get_name(), "Razz Berry");
    BOOST_CHECK_EQUAL(item_entry_gba.get_game(), "Emerald");
    BOOST_CHECK_EQUAL(item_entry_gba.get_category(), "Baking only");
    BOOST_CHECK_EQUAL(item_entry_gba.get_pocket(), "Berries");
    (void)item_entry_gba.get_description();
    BOOST_CHECK_EQUAL(item_entry_gba.get_cost(), 20);
    BOOST_CHECK(item_entry_gba.holdable());
    BOOST_CHECK_EQUAL(item_entry_gba.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(item_entry_gba.get_fling_effect(), "None");

    // Gamecube entry
    BOOST_CHECK_EQUAL(item_entry_gcn.get_name(), "Battle CD 03");
    BOOST_CHECK_EQUAL(item_entry_gcn.get_game(), "XD");
    BOOST_CHECK_EQUAL(item_entry_gcn.get_category(), "Battle CDs");
    BOOST_CHECK_EQUAL(item_entry_gcn.get_pocket(), "Battle CDs");
    (void)item_entry_gcn.get_description();
    BOOST_CHECK_EQUAL(item_entry_gcn.get_cost(), 0);
    BOOST_CHECK(not item_entry_gcn.holdable());
    BOOST_CHECK_EQUAL(item_entry_gcn.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(item_entry_gcn.get_fling_effect(), "None");

    // Generation IV entry
    BOOST_CHECK_EQUAL(item_entry_gen4.get_name(), "Fast Ball");
    BOOST_CHECK_EQUAL(item_entry_gen4.get_game(), "SoulSilver");
    BOOST_CHECK_EQUAL(item_entry_gen4.get_category(), "Apricorn balls");
    BOOST_CHECK_EQUAL(item_entry_gen4.get_pocket(), "Poké Balls");
    (void)item_entry_gen4.get_description();
    BOOST_CHECK_EQUAL(item_entry_gen4.get_cost(), 300);
    BOOST_CHECK(item_entry_gen4.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen4.get_fling_power(), 0);
    BOOST_CHECK_EQUAL(item_entry_gen4.get_fling_effect(), "None");

    // Generation V entry
    BOOST_CHECK_EQUAL(item_entry_gen5.get_name(), "Reveal Glass");
    BOOST_CHECK_EQUAL(item_entry_gen5.get_game(), "Black 2");
    BOOST_CHECK_EQUAL(item_entry_gen5.get_category(), "Gameplay");
    BOOST_CHECK_EQUAL(item_entry_gen5.get_pocket(), "Key Items");
    (void)item_entry_gen5.get_description();
    BOOST_CHECK_EQUAL(item_entry_gen5.get_cost(), 0);
    BOOST_CHECK(not item_entry_gen5.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen5.get_fling_power(), 0);
    BOOST_CHECK_EQUAL(item_entry_gen5.get_fling_effect(), "None");

    // Generation VI entry
    BOOST_CHECK_EQUAL(item_entry_gen6.get_name(), "Salamencite");
    BOOST_CHECK_EQUAL(item_entry_gen6.get_game(), "Omega Ruby");
    BOOST_CHECK_EQUAL(item_entry_gen6.get_category(), "Mega Stones");
    BOOST_CHECK_EQUAL(item_entry_gen6.get_pocket(), "Items");
    (void)item_entry_gen6.get_description();
    BOOST_CHECK_EQUAL(item_entry_gen6.get_cost(), 0);
    BOOST_CHECK(item_entry_gen6.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen6.get_fling_power(), 0);
    BOOST_CHECK_EQUAL(item_entry_gen6.get_fling_effect(), "None");
}

BOOST_AUTO_TEST_CASE(item_entry_test_main) {

    // Test with item entries created by index
    pkmn::database::item_entry byindex_gen1(20, 1);
    pkmn::database::item_entry byindex_gen2(104, 5);
    pkmn::database::item_entry byindex_gba(148, 9);
    pkmn::database::item_entry byindex_gcn(536, 20);
    pkmn::database::item_entry byindex_gen4(492, 16);
    pkmn::database::item_entry byindex_gen5(638, 21);
    pkmn::database::item_entry byindex_gen6(769, 25);

    // Test with item entries created by name
    pkmn::database::item_entry byname_gen1("Potion", "Red");
    pkmn::database::item_entry byname_gen2("Pink Bow", "Silver");
    pkmn::database::item_entry byname_gba("Razz Berry", "Emerald");
    pkmn::database::item_entry byname_gcn("Battle CD 03", "XD");
    pkmn::database::item_entry byname_gen4("Fast Ball", "SoulSilver");
    pkmn::database::item_entry byname_gen5("Reveal Glass", "Black 2");
    pkmn::database::item_entry byname_gen6("Salamencite", "Omega Ruby");

    // These should be equal
    BOOST_CHECK(byindex_gen1 == byname_gen1);
    BOOST_CHECK(byindex_gen2 == byname_gen2);
    BOOST_CHECK(byindex_gba  == byname_gba);
    BOOST_CHECK(byindex_gcn  == byname_gcn);
    BOOST_CHECK(byindex_gen4 == byname_gen4);
    BOOST_CHECK(byindex_gen5 == byname_gen5);
    BOOST_CHECK(byindex_gen6 == byname_gen6);

    _item_entry_test_main(
        byindex_gen1, byindex_gen2, byindex_gba,
        byindex_gcn, byindex_gen4, byindex_gen5,
        byindex_gen6
    );
    _item_entry_test_main(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );

    /*
     * TODO:
     *  * Make sure TMs' description have move names, verify for each generation
     */
}
