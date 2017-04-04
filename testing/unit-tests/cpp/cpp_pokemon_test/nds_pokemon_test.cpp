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

            "Faraway place",
            {"Verity Cavern", "Lake of Rage"},
            {"Lilicove City", "Relic Castle"},

            {"Iron Head", "Signal Beam", "Return", "Fire Blast"},
            {"Bulldoze", "Petal Blizzard"},

            {"Ruby", "Diamond", "HeartGold"},
            {"Gold", "Black"}
        }
    );
}

static const std::vector<std::pair<std::string, std::string>> gen4_params = {
    {"Diamond", "Turtwig"},
    {"Pearl", "Chimchar"},
    {"Platinum", "Piplup"},
    {"HeartGold", "Cyndaquil"},
    {"SoulSilver", "Totodile"}
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen4_pokemon_test,
    gen4_pokemon_test,
    ::testing::ValuesIn(gen4_params)
);

class gen5_pokemon_test: public pokemon_test {};

TEST_P(gen5_pokemon_test, gen5_pokemon_test) {
    pkmn::pokemon::sptr pokemon = get_pokemon();

    pokemon_test_common(
        pokemon,
        {
            "Dream Ball",
            {"Beast Ball"},

            "Lucky Punch",
            {"Berry", "Roseli Berry", "Liberty Pass"},

            "Faraway place",
            {"Giant Chasm", "Castelia Gate"},
            {"Viridian Forest", "Vaniville Town"},

            {"Drill Run", "Iron Head", "Signal Beam", "Return"},
            {"Ion Deluge"},

            {"Ruby", "Diamond", "HeartGold", "Black"},
            {"Gold", "X"}
        }
    );
}

static const std::vector<std::pair<std::string, std::string>> gen5_params = {
    {"Black", "Snivy"},
    {"White", "Tepig"},
    {"Black 2", "Oshawott"},
    {"White 2", "Snivy"},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen5_pokemon_test,
    gen5_pokemon_test,
    ::testing::ValuesIn(gen5_params)
);
