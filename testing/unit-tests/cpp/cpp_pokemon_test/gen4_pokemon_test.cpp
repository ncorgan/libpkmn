/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/common/nds_pokemon.h>
#include <pksav/gen4/text.h>
#include <pksav/math/endian.h>

#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/format.hpp>

// TODO: test function common to both NDS generations

class gen4_pokemon_test: public pokemon_test {};

TEST_P(gen4_pokemon_test, gen4_pokemon_test) {
    pkmn::pokemon::sptr pokemon = get_pokemon();

    pokemon_test_common(
        pokemon,
        {
            "Heal Ball",
            {"Dream Ball"},

            "Occa Berry",
            {"Berry", "Roseli Berry", "Galactic Key"},

            "Faraway Place",
            {"Verity Cavern", "Lake of Rage"},
            {"Lilicove City", "Relic Castle"},

            {"Iron Head", "Signal Beam", "Return", "Fire Blast"},
            {"Bulldoze", "Petal Blizzard"},

            {"Ruby", "Diamond", "HeartGold"},
            {"Gold", "Black"}
        }
    );
}

static const std::vector<std::pair<std::string, std::string>> params = {
    {"Diamond", "Turtwig"},
    {"Pearl", "Chimchar"},
    {"Platinum", "Piplup"},
    {"HeartGold", "Cyndaquil"},
    {"SoulSilver", "Totodile"}
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen4_pokemon_test,
    gen4_pokemon_test,
    ::testing::ValuesIn(params)
);
