/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_database_entry_test.hpp"

// Only create one main
#undef BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

void item_entry_none_test(
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

void item_entry_wrong_game_test() {
    /*
     * Make sure items from later generations throw an
     * exception.
     */
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

    /*
     * Make sure items from different games in a given generation
     * throw an exception.
     */
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

    // TODO: more from other generations
}

void item_entry_test_common(
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
