/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_database_entry_test.hpp"

#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <boost/test/unit_test.hpp>

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
    // Make sure "None" entries work
    for(int i = 0; i < 6; ++i) {
        pkmn::database::move_entry none_byindex(0, game_ids[i]);
        pkmn::database::move_entry none_byname("None", game_names[i]);

        move_entry_none_test(none_byindex);
        move_entry_none_test(none_byname);
    }

    /*
     * TODO:
     *  * Make sure moves from later generations throw exceptions
     *  * Make sure moves from same generation, wrong game throw exceptions
     *  * Check equality operator between equal entries above
     *  * Check inequality operator
     *  * Make sure different names correspond to same move
     */
}

BOOST_AUTO_TEST_CASE(pokemon_entry_test) {
    (void)0;
}
