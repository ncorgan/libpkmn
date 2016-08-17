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

void pokemon_entry_none_test(
    pkmn::database::pokemon_entry &none_entry
) {
    (void)none_entry;
}

void pokemon_entry_test_common(
    pkmn::database::pokemon_entry &pokemon_entry_gen1,
    pkmn::database::pokemon_entry &pokemon_entry_gen2,
    pkmn::database::pokemon_entry &pokemon_entry_gba,
    pkmn::database::pokemon_entry &pokemon_entry_gcn,
    pkmn::database::pokemon_entry &pokemon_entry_gen4,
    pkmn::database::pokemon_entry &pokemon_entry_gen5,
    pkmn::database::pokemon_entry &pokemon_entry_gen6
) {
    (void)pokemon_entry_gen1;
    (void)pokemon_entry_gen2;
    (void)pokemon_entry_gba;
    (void)pokemon_entry_gcn;
    (void)pokemon_entry_gen4;
    (void)pokemon_entry_gen5;
    (void)pokemon_entry_gen6;
}
