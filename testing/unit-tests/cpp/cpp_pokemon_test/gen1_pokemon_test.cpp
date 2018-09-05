/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test_common.hpp"

#include <pkmn/exception.hpp>
#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

class gen1_pokemon_test: public pokemon_test {};

TEST_P(gen1_pokemon_test, gen1_pokemon_test)
{
    pkmn::pokemon::sptr pokemon = get_pokemon();

    pokemon_test_common(
        pokemon,
        {
            pkmn::e_ball::GREAT_BALL,
            {pkmn::e_ball::GREAT_BALL},

            pkmn::e_item::POTION,
            {pkmn::e_item::POTION},

            "Special",
            {"Route 1"},
            {"Route 1"},

            {
                pkmn::e_move::SLASH,
                pkmn::e_move::FLAMETHROWER,
                pkmn::e_move::TAIL_WHIP,
                pkmn::e_move::FIRE_BLAST
            },
            {pkmn::e_move::RETURN},

            {pkmn::e_game::RED},
            {pkmn::e_game::RED}
        }
    );

    /*
     * On the C++ level, check the underlying PKSav struct and make
     * sure our abstractions match.
     */
    const struct pksav_gen1_pc_pokemon* native_pc = &reinterpret_cast<const struct pksav_gen1_party_pokemon*>(
                                                         pokemon->get_native()
                                                     )->pc_data;
    const struct pksav_gen1_pokemon_party_data* native_party_data = &reinterpret_cast<const struct pksav_gen1_party_pokemon*>(
                                                                         pokemon->get_native()
                                                                     )->party_data;
    static const pksav::gb_condition_bimap_t& GB_CONDITION_BIMAP = pksav::get_gb_condition_bimap();


    /*
     * PC data
     */
    EXPECT_EQ(pokemon->get_database_entry().get_pokemon_index(), int(native_pc->species));
    EXPECT_EQ(pokemon->get_current_hp(), int(pksav_bigendian16(native_pc->current_hp)));
    EXPECT_EQ(pokemon->get_level(), int(native_pc->level));

    EXPECT_EQ(
        uint8_t(GB_CONDITION_BIMAP.left.at(pokemon->get_condition())),
        native_pc->condition
    );
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

    const std::map<pkmn::e_stat, int>& EVs = pokemon->get_EVs();
    EXPECT_EQ(EVs.at(pkmn::e_stat::HP), int(pksav_bigendian16(native_pc->ev_hp)));
    EXPECT_EQ(EVs.at(pkmn::e_stat::ATTACK) , int(pksav_bigendian16(native_pc->ev_atk)));
    EXPECT_EQ(EVs.at(pkmn::e_stat::DEFENSE), int(pksav_bigendian16(native_pc->ev_def)));
    EXPECT_EQ(EVs.at(pkmn::e_stat::SPEED), int(pksav_bigendian16(native_pc->ev_spd)));
    EXPECT_EQ(EVs.at(pkmn::e_stat::SPECIAL), int(pksav_bigendian16(native_pc->ev_spcl)));

    const std::map<pkmn::e_stat, int>& IVs = pokemon->get_IVs();
    uint8_t pksav_IVs[PKSAV_NUM_GB_IVS] = {0};
    PKSAV_CALL(
        pksav_get_gb_IVs(
            &native_pc->iv_data,
            pksav_IVs,
            sizeof(pksav_IVs)
        );
    )

    EXPECT_EQ(IVs.at(pkmn::e_stat::HP),      int(pksav_IVs[PKSAV_GB_IV_HP]));
    EXPECT_EQ(IVs.at(pkmn::e_stat::ATTACK),  int(pksav_IVs[PKSAV_GB_IV_ATTACK]));
    EXPECT_EQ(IVs.at(pkmn::e_stat::DEFENSE), int(pksav_IVs[PKSAV_GB_IV_DEFENSE]));
    EXPECT_EQ(IVs.at(pkmn::e_stat::SPEED),   int(pksav_IVs[PKSAV_GB_IV_SPEED]));
    EXPECT_EQ(IVs.at(pkmn::e_stat::SPECIAL), int(pksav_IVs[PKSAV_GB_IV_SPECIAL]));

    /*
     * Party data
     */
    EXPECT_EQ(pokemon->get_level(), int(native_party_data->level));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::HP), int(pksav_bigendian16(native_party_data->max_hp)));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::ATTACK), int(pksav_bigendian16(native_party_data->atk)));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::DEFENSE), int(pksav_bigendian16(native_party_data->def)));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::SPEED), int(pksav_bigendian16(native_party_data->spd)));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::SPECIAL), int(pksav_bigendian16(native_party_data->spcl)));

    /*
     * Test attributes.
     */
    EXPECT_EQ(pokemon->get_numeric_attribute("Catch rate"), int(native_pc->catch_rate));
}

static const std::vector<std::pair<pkmn::e_game, pkmn::e_species>> params =
{
    {pkmn::e_game::RED,    pkmn::e_species::CHARMANDER},
    {pkmn::e_game::BLUE,   pkmn::e_species::SQUIRTLE},
    {pkmn::e_game::YELLOW, pkmn::e_species::BULBASAUR},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen1_pokemon_test,
    gen1_pokemon_test,
    ::testing::ValuesIn(params)
);
