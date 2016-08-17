/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_TESTS_CPP_DATABASE_ENTRY_TEST_HPP
#define PKMN_TESTS_CPP_DATABASE_ENTRY_TEST_HPP

#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <boost/config.hpp>

/*
 * These arrays have game IDs/names for one game from each generation, separating
 * GBA and GCN games. This does not distinguish between subgroups, such as
 * Ruby/Sapphire vs. Emerald, so those should be checked separately.
 */
BOOST_STATIC_CONSTEXPR int game_ids[] = {1,4,7,19,12,17,23};

static BOOST_CONSTEXPR const char* game_names[] = {
    "Red","Gold","Ruby","Colosseum","Diamond","Black","X"
};

/*
 * Item entry tests
 */

void item_entry_none_test(
    const pkmn::database::item_entry &none_entry
);

void item_entry_wrong_game_test();

void item_entry_different_name_test();

void item_entry_test_main(
    const pkmn::database::item_entry &item_entry_gen1,
    const pkmn::database::item_entry &item_entry_gen2,
    const pkmn::database::item_entry &item_entry_gba,
    const pkmn::database::item_entry &item_entry_gcn,
    const pkmn::database::item_entry &item_entry_gen4,
    const pkmn::database::item_entry &item_entry_gen5,
    const pkmn::database::item_entry &item_entry_gen6
);

/*
 * Move entry tests
 */

void move_entry_none_test(
    const pkmn::database::move_entry &none_entry
);

void move_entry_test_common(
    const pkmn::database::move_entry &move_entry_gen1,
    const pkmn::database::move_entry &move_entry_gen2,
    const pkmn::database::move_entry &move_entry_gba,
    const pkmn::database::move_entry &move_entry_gcn,
    const pkmn::database::move_entry &move_entry_gen4,
    const pkmn::database::move_entry &move_entry_gen5,
    const pkmn::database::move_entry &move_entry_gen6
);

/*
 * Pokémon entry tests
 */

void pokemon_entry_none_test(
    pkmn::database::pokemon_entry &none_entry
);

void pokemon_entry_test_common(
    pkmn::database::pokemon_entry &pokemon_entry_gen1,
    pkmn::database::pokemon_entry &pokemon_entry_gen2,
    pkmn::database::pokemon_entry &pokemon_entry_gba,
    pkmn::database::pokemon_entry &pokemon_entry_gcn,
    pkmn::database::pokemon_entry &pokemon_entry_gen4,
    pkmn::database::pokemon_entry &pokemon_entry_gen5,
    pkmn::database::pokemon_entry &pokemon_entry_gen6
);

#endif /* PKMN_TESTS_CPP_DATABASE_ENTRY_TEST_HPP */
