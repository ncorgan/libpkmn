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

#include <algorithm>

/*
 * These arrays have game IDs/names for one game from each generation, separating
 * GBA and GCN games. This does not distinguish between subgroups, such as
 * Ruby/Sapphire vs. Emerald, so those should be checked separately.
 */
BOOST_STATIC_CONSTEXPR int game_ids[] = {1,4,7,19,12,17,23};

static BOOST_CONSTEXPR const char* game_names[] = {
    "Red","Gold","Ruby","Colosseum","Diamond","Black","X"
};

static PKMN_INLINE bool string_in_vector(
    const std::vector<std::string> &vec,
    const std::string &str
) {
    return (std::find(vec.begin(), vec.end(), str) != vec.end());
}

#endif /* PKMN_TESTS_CPP_DATABASE_ENTRY_TEST_HPP */
