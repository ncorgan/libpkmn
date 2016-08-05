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

void move_entry_none_test(
    const pkmn::database::move_entry &none_entry
) {
    (void)none_entry;
}

void move_entry_test_common(
    const pkmn::database::move_entry &move_entry_gen1,
    const pkmn::database::move_entry &move_entry_gen2,
    const pkmn::database::move_entry &move_entry_gba,
    const pkmn::database::move_entry &move_entry_gcn,
    const pkmn::database::move_entry &move_entry_gen4,
    const pkmn::database::move_entry &move_entry_gen5,
    const pkmn::database::move_entry &move_entry_gen6
) {
    (void)move_entry_gen1;
    (void)move_entry_gen2;
    (void)move_entry_gba;
    (void)move_entry_gcn;
    (void)move_entry_gen4;
    (void)move_entry_gen5;
    (void)move_entry_gen6;
}
