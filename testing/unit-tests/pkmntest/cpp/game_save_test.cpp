/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"

#include <pkmntest/game_save_test.hpp>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <map>

// No database access here
static const std::map<std::string, int> game_generations = boost::assign::map_list_of
    ("Red", 1)
    ("Blue", 1)
    ("Yellow", 1)
    ("Gold", 2)
    ("Silver", 2)
    ("Crystal", 2)
    ("Ruby", 3)
    ("Sapphire", 3)
    ("Emerald", 3)
    ("FireRed", 3)
    ("LeafGreen", 3)
    ("Colosseum", 3)
    ("XD", 3)
    ("Diamond", 4)
    ("Pearl", 4)
    ("Platinum", 4)
    ("HeartGold", 4)
    ("SoulSilver", 4)
    ("Black", 5)
    ("White", 5)
    ("Black 2", 5)
    ("White 2", 5)
    ("X", 6)
    ("Y", 6)
    ("Omega Ruby", 6)
    ("Alpha Sapphire", 6)
;

namespace fs = boost::filesystem;

namespace pkmntest {

    static const fs::path PKSAV_TEST_SAVES(pkmn_getenv("PKSAV_TEST_SAVES"));

    void game_save_test::load_save() {
        std::string save_path = fs::path(PKSAV_TEST_SAVES / std::get<2>(_params)).string();
        ASSERT_EQ(std::get<0>(_params), pkmn::game_save::detect_type(save_path));
        _game_save = pkmn::game_save::from_file(save_path);
        ASSERT_EQ(std::get<1>(_params), _game_save->get_game());
    }

}
