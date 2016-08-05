/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <boost/config.hpp>
#include <boost/test/unit_test.hpp>

/*
 * These arrays have game IDs/names for one game from each generation, separating
 * GBA and GCN games. This does not distinguish between subgroups, such as
 * Ruby/Sapphire vs. Emerald, so those should be checked separately.
 */
BOOST_STATIC_CONSTEXPR int game_ids[] = {1,4,7,19,12,17,23};

static BOOST_CONSTEXPR const char* game_names[] = {
    "Red","Gold","Ruby","Colosseum","Diamond","Black","X"
};

static void item_entry_none_test(
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

static void item_entry_test_common(
    const pkmn::database::item_entry &item_entry_gen1,
    const pkmn::database::item_entry &item_entry_gen2,
    const pkmn::database::item_entry &item_entry_gba,
    const pkmn::database::item_entry &item_entry_gcn,
    const pkmn::database::item_entry &item_entry_gen4,
    const pkmn::database::item_entry &item_entry_gen5,
    const pkmn::database::item_entry &item_entry_gen6
) {
    // Generation I entry
    BOOST_CHECK_EQUAL(item_entry_gen1.get_name(), "Potion");
    BOOST_CHECK_EQUAL(item_entry_gen1.get_game(), "Red");
    // TODO: category
    BOOST_CHECK_EQUAL(item_entry_gen1.get_pocket(), "Items");
    // TODO: description
    BOOST_CHECK_EQUAL(item_entry_gen1.get_cost(), 300);
    BOOST_CHECK(not item_entry_gen1.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen1.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(item_entry_gen1.get_fling_effect(), "None");

    // Generation II entry
    BOOST_CHECK_EQUAL(item_entry_gen2.get_name(), "Pink Bow");
    BOOST_CHECK_EQUAL(item_entry_gen2.get_game(), "Silver");
    // TODO: category
    BOOST_CHECK_EQUAL(item_entry_gen2.get_pocket(), "Items");
    // TODO: description
    BOOST_CHECK_EQUAL(item_entry_gen2.get_cost(), 50);
    BOOST_CHECK(item_entry_gen2.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen2.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(item_entry_gen2.get_fling_effect(), "None");

    // GBA entry
    BOOST_CHECK_EQUAL(item_entry_gba.get_name(), "Razz Berry");
    BOOST_CHECK_EQUAL(item_entry_gba.get_game(), "Emerald");
    // TODO: category
    BOOST_CHECK_EQUAL(item_entry_gba.get_pocket(), "Berries");
    // TODO: description
    BOOST_CHECK_EQUAL(item_entry_gba.get_cost(), 20);
    BOOST_CHECK(item_entry_gba.holdable());
    BOOST_CHECK_EQUAL(item_entry_gba.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(item_entry_gba.get_fling_effect(), "None");

    // Gamecube entry
    BOOST_CHECK_EQUAL(item_entry_gcn.get_name(), "Battle CD 03");
    BOOST_CHECK_EQUAL(item_entry_gcn.get_game(), "XD");
    // TODO: category
    BOOST_CHECK_EQUAL(item_entry_gcn.get_pocket(), "Battle CDs");
    // TODO: description
    BOOST_CHECK_EQUAL(item_entry_gcn.get_cost(), 0);
    BOOST_CHECK(not item_entry_gcn.holdable());
    BOOST_CHECK_EQUAL(item_entry_gcn.get_fling_power(), -1);
    BOOST_CHECK_EQUAL(item_entry_gcn.get_fling_effect(), "None");

    // Generation IV entry
    BOOST_CHECK_EQUAL(item_entry_gen4.get_name(), "Fast Ball");
    BOOST_CHECK_EQUAL(item_entry_gen4.get_game(), "SoulSilver");
    // TODO: category
    BOOST_CHECK_EQUAL(item_entry_gen4.get_pocket(), "Poké Balls");
    // TODO: description
    BOOST_CHECK_EQUAL(item_entry_gen4.get_cost(), 300);
    BOOST_CHECK(item_entry_gen4.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen4.get_fling_power(), 0);
    BOOST_CHECK_EQUAL(item_entry_gen4.get_fling_effect(), "None");

    // Generation V entry
    BOOST_CHECK_EQUAL(item_entry_gen5.get_name(), "Reveal Glass");
    BOOST_CHECK_EQUAL(item_entry_gen5.get_game(), "Black 2");
    // TODO: category
    BOOST_CHECK_EQUAL(item_entry_gen5.get_pocket(), "Items");
    // TODO: description
    BOOST_CHECK_EQUAL(item_entry_gen5.get_cost(), 0);
    BOOST_CHECK(item_entry_gen5.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen5.get_fling_power(), 0);
    BOOST_CHECK_EQUAL(item_entry_gen5.get_fling_effect(), "None");

    // Generation VI entry
    BOOST_CHECK_EQUAL(item_entry_gen6.get_name(), "Salamencite");
    BOOST_CHECK_EQUAL(item_entry_gen6.get_game(), "Omega Ruby");
    // TODO: category
    BOOST_CHECK_EQUAL(item_entry_gen6.get_pocket(), "Battle Items");
    // TODO: description
    BOOST_CHECK_EQUAL(item_entry_gen6.get_cost(), 0);
    BOOST_CHECK(item_entry_gen6.holdable());
    BOOST_CHECK_EQUAL(item_entry_gen6.get_fling_power(), 0);
    BOOST_CHECK_EQUAL(item_entry_gen6.get_fling_effect(), "None");
}

BOOST_AUTO_TEST_CASE(item_entry_test) {
    // Make sure "None" entries work
    for(int i = 0; i < 6; ++i) {
        pkmn::database::item_entry none_byindex(0, game_ids[i]);
        pkmn::database::item_entry none_byname("None", game_names[i]);

        item_entry_none_test(none_byindex);
        item_entry_none_test(none_byname);
    }

    // TODO: get equivalent entries created by index

    // Test with item entries created by name
    pkmn::database::item_entry byname_gen1("Potion", "Red");
    pkmn::database::item_entry byname_gen2("Pink Bow", "Silver");
    pkmn::database::item_entry byname_gba("Razz Berry", "Emerald");
    pkmn::database::item_entry byname_gcn("Battle CD 03", "XD");
    pkmn::database::item_entry byname_gen4("Fast Ball", "SoulSilver");
    pkmn::database::item_entry byname_gen5("Reveal Glass", "Black 2");
    pkmn::database::item_entry byname_gen6("Salamencite", "Omega Ruby");

    item_entry_test_common(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );

    /*
     * TODO:
     *  * Make sure items from later generations throw exceptions
     *  * Make sure items from same generation, wrong game throw exceptions
     *  * Make sure TMs' description have move names, verify for each generation
     *  * Check equality operator between equal entries above
     *  * Check inequality operator
     *  * Make sure different names correspond to same item
     */
}

BOOST_AUTO_TEST_CASE(move_entry_test) {
    (void)0;
}

BOOST_AUTO_TEST_CASE(pokemon_entry_test) {
    (void)0;
}
