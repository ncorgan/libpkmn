/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen2_pokemon_tests.hpp>
#include "pokemon_tests_common.hpp"

#include <pkmn/exception.hpp>
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/math/endian.h>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

static const std::map<std::string, pkmn::move_slot> none_move_slots = boost::assign::map_list_of
    ("Gold", pkmn::move_slot(
                 pkmn::database::move_entry("None", "Gold"),
                 0
             ))
    ("Silver", pkmn::move_slot(
                   pkmn::database::move_entry("None", "Silver"),
                   0
               ))
    ("Crystal", pkmn::move_slot(
                    pkmn::database::move_entry("None", "Crystal"),
                    0
                ))
;

static const std::map<std::string, pkmn::database::pokemon_entry> none_pokemon_entries = boost::assign::map_list_of
    ("Gold", pkmn::database::pokemon_entry("None", "Gold", ""))
    ("Silver", pkmn::database::pokemon_entry("None", "Silver", ""))
    ("Crystal", pkmn::database::pokemon_entry("None", "Crystal", ""))
;

namespace pkmntest {

    void gen2_invalid_pokemon_test(
        const std::string &game
    ) {
        pkmntest::test_invalid_pokemon(game);
    }

    static void gen2_pokemon_check_stat_map(
        const std::map<std::string, int>& stat_map,
        bool is_stats
    ) {
        (void)stat_map;
        (void)is_stats;
        /*BOOST_CHECK_EQUAL(stat_map.count("HP"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Attack"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Defense"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Speed"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Special"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Special Attack"), 0);
        BOOST_CHECK_EQUAL(stat_map.count("Special Defense"), 0);*/
    }

    void gen2_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    ) {
        (void)pokemon;
        (void)species;
        (void)game;

        gen2_pokemon_check_stat_map(
            pokemon->get_EVs(),
            false
        );
    }
}
