/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <boost/test/unit_test.hpp>

static void item_entry_test_common(
    const pkmn::database::item_entry &item_entry_gen1,
    const pkmn::database::item_entry &item_entry_gen2,
    const pkmn::database::item_entry &item_entry_gba,
    const pkmn::database::item_entry &item_entry_gcn,
    const pkmn::database::item_entry &item_entry_gen4,
    const pkmn::database::item_entry &item_entry_gen5,
    const pkmn::database::item_entry &item_entry_gen6
) {
    (void)item_entry_gen1;
    (void)item_entry_gen2;
    (void)item_entry_gba;
    (void)item_entry_gcn;
    (void)item_entry_gen4;
    (void)item_entry_gen5;
    (void)item_entry_gen6;
}

BOOST_AUTO_TEST_CASE(item_entry_test) {
    // TODO: get equivalent entries created by index

    // Test with item entries created by name
    pkmn::database::item_entry byname_gen1("Potion", "Red");
    pkmn::database::item_entry byname_gen2("Pink Bow", "Silver");
    pkmn::database::item_entry byname_gba("Razz Berry", "Emerald");
    pkmn::database::item_entry byname_gcn("Battle CD 03", "XD");
    pkmn::database::item_entry byname_gen4("Fast Ball", "SoulSilver");
    pkmn::database::item_entry byname_gen5("TM05", "Black");
    pkmn::database::item_entry byname_gen6("Salamencite", "Omega Ruby");

    item_entry_test_common(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );
}

BOOST_AUTO_TEST_CASE(move_entry_test) {
    (void)0;
}

BOOST_AUTO_TEST_CASE(pokemon_entry_test) {
    (void)0;
}
