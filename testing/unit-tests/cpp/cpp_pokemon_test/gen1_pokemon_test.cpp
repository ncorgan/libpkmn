/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test_common.hpp"

#include <pkmn/exception.hpp>
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

class gen1_pokemon_test: public pokemon_test {};

TEST_P(gen1_pokemon_test, gen1_pokemon_test) {
    pkmn::pokemon::sptr pokemon = get_pokemon();

    pokemon_test_common(
        pokemon,
        {
            "Great Ball",
            {"Great Ball"},

            "Potion",
            {"Potion"},

            "Special",
            {"Route 1"},
            {"Route 1"},

            {"Slash", "Flamethrower", "Tail Whip", "Fire Blast"},
            {"Return"},

            {"Red"},
            {"Red"}
        }
    );

    /*
     * On the C++ level, check the underlying PKSav struct and make
     * sure our abstractions match.
     */
    const struct pksav_gen1_pc_pokemon* native_pc = reinterpret_cast<struct pksav_gen1_pc_pokemon*>(
                                                        pokemon->get_native_pc_data()
                                                    );
    const struct pksav_gen1_pokemon_party_data* native_party_data = reinterpret_cast<struct pksav_gen1_pokemon_party_data*>(
                                                                        pokemon->get_native_party_data()
                                                                    );

    /*
     * PC data
     */
    EXPECT_EQ(pokemon->get_database_entry().get_pokemon_index(), int(native_pc->species));
    EXPECT_EQ(pokemon->get_current_hp(), int(pksav_bigendian16(native_pc->current_hp)));
    EXPECT_EQ(pokemon->get_level(), int(native_pc->level));
    // TODO: change condition and check
    EXPECT_EQ(0, native_pc->condition);
    // TODO: programmatic check for types, catch rate

    const pkmn::move_slots_t& moves = pokemon->get_moves();
    for(size_t i = 0; i < 4; ++i)
    {
        pkmn::database::move_entry entry(moves.at(i).move, get_game());
        EXPECT_EQ(entry.get_move_id(), int(native_pc->moves[i]));
        EXPECT_EQ(
            moves.at(i).pp,
            int(native_pc->move_pps[i] & PKSAV_GEN1_POKEMON_MOVE_PP_MASK)
        );
        EXPECT_EQ(
            3,
            PKSAV_GEN1_POKEMON_MOVE_PP_UP(native_pc->move_pps[i])
        );
    }

    EXPECT_EQ(pokemon->get_original_trainer_id(), int(pksav_bigendian16(native_pc->ot_id)));

    const std::map<std::string, int>& EVs = pokemon->get_EVs();
    EXPECT_EQ(EVs.at("HP"), int(pksav_bigendian16(native_pc->ev_hp)));
    EXPECT_EQ(EVs.at("Attack") , int(pksav_bigendian16(native_pc->ev_atk)));
    EXPECT_EQ(EVs.at("Defense"), int(pksav_bigendian16(native_pc->ev_def)));
    EXPECT_EQ(EVs.at("Speed"), int(pksav_bigendian16(native_pc->ev_spd)));
    EXPECT_EQ(EVs.at("Special"), int(pksav_bigendian16(native_pc->ev_spcl)));

    const std::map<std::string, int>& IVs = pokemon->get_IVs();
    uint8_t pksav_IVs[PKSAV_NUM_GB_IVS] = {0};
    PKSAV_CALL(
        pksav_get_gb_IVs(
            &native_pc->iv_data,
            pksav_IVs,
            sizeof(pksav_IVs)
        );
    )

    EXPECT_EQ(IVs.at("HP"),      int(pksav_IVs[PKSAV_GB_IV_HP]));
    EXPECT_EQ(IVs.at("Attack"),  int(pksav_IVs[PKSAV_GB_IV_ATTACK]));
    EXPECT_EQ(IVs.at("Defense"), int(pksav_IVs[PKSAV_GB_IV_DEFENSE]));
    EXPECT_EQ(IVs.at("Speed"),   int(pksav_IVs[PKSAV_GB_IV_SPEED]));
    EXPECT_EQ(IVs.at("Special"), int(pksav_IVs[PKSAV_GB_IV_SPECIAL]));

    /*
     * Party data
     */
    EXPECT_EQ(pokemon->get_level(), int(native_party_data->level));
    EXPECT_EQ(pokemon->get_stats().at("HP"), int(pksav_bigendian16(native_party_data->max_hp)));
    EXPECT_EQ(pokemon->get_stats().at("Attack"), int(pksav_bigendian16(native_party_data->atk)));
    EXPECT_EQ(pokemon->get_stats().at("Defense"), int(pksav_bigendian16(native_party_data->def)));
    EXPECT_EQ(pokemon->get_stats().at("Speed"), int(pksav_bigendian16(native_party_data->spd)));
    EXPECT_EQ(pokemon->get_stats().at("Special"), int(pksav_bigendian16(native_party_data->spcl)));

    /*
     * Test attributes.
     */
    EXPECT_EQ(pokemon->get_numeric_attribute("Catch rate"), int(native_pc->catch_rate));
}

static const std::vector<std::pair<std::string, std::string>> params = {
    {"Red", "Charmander"},
    {"Blue", "Squirtle"},
    {"Yellow", "Bulbasaur"}
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen1_pokemon_test,
    gen1_pokemon_test,
    ::testing::ValuesIn(params)
);
