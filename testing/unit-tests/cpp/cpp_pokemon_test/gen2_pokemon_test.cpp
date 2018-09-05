/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>
#include <pkmn/database/item_entry.hpp>

#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include <pksav/common/pokerus.h>
#include <pksav/common/stats.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gen2/time.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

class gen2_pokemon_test: public pokemon_test {};

TEST_P(gen2_pokemon_test, gen2_pokemon_test) {
    pkmn::pokemon::sptr pokemon = get_pokemon();

    pokemon_test_common(
        pokemon,
        {
            pkmn::e_ball::GREAT_BALL,
            {pkmn::e_ball::GREAT_BALL},

            pkmn::e_item::BERRY,
            {pkmn::e_item::RAZZ_BERRY, pkmn::e_item::BICYCLE},

            "Special",
            {"Sprout Tower", "Tohjo Falls"},
            {"Littleroot Town", "Petalburg Woods"},

            {
                pkmn::e_move::SLASH,
                pkmn::e_move::FLAMETHROWER,
                pkmn::e_move::RETURN,
                pkmn::e_move::FIRE_BLAST
            },
            {
                pkmn::e_move::FRENZY_PLANT,
                pkmn::e_move::ROOST
            },

            {pkmn::e_game::GOLD},
            {pkmn::e_game::GOLD}
        }
    );

    // Gender affects IVs, so make sure the abstraction reflects that.
    const std::map<pkmn::e_stat, int>& IVs = pokemon->get_IVs();
    pokemon->set_gender(pkmn::e_gender::MALE);
    EXPECT_EQ(15, IVs.at(pkmn::e_stat::ATTACK));
    pokemon->set_gender(pkmn::e_gender::FEMALE);
    EXPECT_LT(IVs.at(pkmn::e_stat::ATTACK), 15);

    pokemon->set_IV(pkmn::e_stat::ATTACK, 0);
    EXPECT_EQ(pkmn::e_gender::FEMALE, pokemon->get_gender());
    pokemon->set_IV(pkmn::e_stat::ATTACK, 15);
    EXPECT_EQ(pkmn::e_gender::MALE, pokemon->get_gender());

    // Shininess affects IVs, so make sure the abstraction reflects that.
    pokemon->set_shininess(false);
    EXPECT_FALSE(pokemon->is_shiny());
    EXPECT_EQ(13, IVs.at(pkmn::e_stat::ATTACK));

    pokemon->set_shininess(true);
    EXPECT_TRUE(pokemon->is_shiny());
    EXPECT_EQ(15, IVs.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(10, IVs.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(10, IVs.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(10, IVs.at(pkmn::e_stat::SPECIAL));

    // On the C++ level, make sure functions that affect the same PKSav field don't impact each other.
    std::string location_met_before_change = pokemon->get_location_met(false);
    pkmn::e_gender trainer_gender_before_change = pokemon->get_original_trainer_gender();
    int level_met_before_change = pokemon->get_level_met();

    const struct pksav_gen2_pc_pokemon* native_pc = &reinterpret_cast<const struct pksav_gen2_party_pokemon*>(
                                                         pokemon->get_native()
                                                     )->pc_data;
    uint16_t time_caught_before_change = (native_pc->caught_data & PKSAV_GEN2_TIME_OF_DAY_MASK);
    time_caught_before_change >>= PKSAV_GEN2_TIME_OF_DAY_OFFSET;

    // Setting location caught shouldn't affect level caught, trainer gender, or time of day caught.
    EXPECT_THROW(
        pokemon->set_location_met("Pallet Town", true);
    , pkmn::feature_not_in_game_error);
    pokemon->set_location_met("Pallet Town", false);
    EXPECT_EQ("Pallet Town", pokemon->get_location_met(false));
    EXPECT_EQ(trainer_gender_before_change, pokemon->get_original_trainer_gender());
    EXPECT_EQ(level_met_before_change, pokemon->get_level_met());

    uint16_t time_caught = (native_pc->caught_data & PKSAV_GEN2_TIME_OF_DAY_MASK);
    time_caught >>= PKSAV_GEN2_TIME_OF_DAY_OFFSET;
    EXPECT_EQ(time_caught_before_change, time_caught);

    pokemon->set_location_met(location_met_before_change, false);

    // Setting trainer gender shouldn't affect level caught, location caught, or time of day caught.
    pokemon->set_original_trainer_gender(pkmn::e_gender::FEMALE);
    EXPECT_EQ(pkmn::e_gender::FEMALE, pokemon->get_original_trainer_gender());
    EXPECT_EQ(location_met_before_change, pokemon->get_location_met(false));
    EXPECT_EQ(level_met_before_change, pokemon->get_level_met());

    time_caught = (native_pc->caught_data & PKSAV_GEN2_TIME_OF_DAY_MASK);
    time_caught >>= PKSAV_GEN2_TIME_OF_DAY_OFFSET;
    EXPECT_EQ(time_caught_before_change, time_caught);

    pokemon->set_original_trainer_gender(trainer_gender_before_change);

    // Setting level caught shouldn't affect location caught, trainer gender, or time of day caught.
    pokemon->set_level_met(3);
    EXPECT_EQ(3, pokemon->get_level_met());
    EXPECT_EQ(location_met_before_change, pokemon->get_location_met(false));
    EXPECT_EQ(trainer_gender_before_change, pokemon->get_original_trainer_gender());

    time_caught = (native_pc->caught_data & PKSAV_GEN2_TIME_OF_DAY_MASK);
    time_caught >>= PKSAV_GEN2_TIME_OF_DAY_OFFSET;
    EXPECT_EQ(time_caught_before_change, time_caught);

    /*
     * On the C++ level, check the underlying PKSav struct and make
     * sure our abstractions match.
     */
    const struct pksav_gen2_pokemon_party_data* native_party_data = &reinterpret_cast<const struct pksav_gen2_party_pokemon*>(
                                                                         pokemon->get_native()
                                                                     )->party_data;

    EXPECT_EQ(native_pc->species, uint8_t(pokemon->get_database_entry().get_pokemon_index()));
    EXPECT_EQ(
        native_pc->held_item,
        uint8_t(pkmn::database::item_entry(pokemon->get_held_item(), get_game()).get_item_index())
    );

    uint8_t duration_from_pokerus = 0;
    PKSAV_CALL(
        pksav_pokerus_get_duration(
            &native_pc->pokerus,
            &duration_from_pokerus
        )
    )
    EXPECT_EQ(pokemon->get_pokerus_duration(), int(duration_from_pokerus));

    const pkmn::move_slots_t& moves = pokemon->get_moves();
    for(size_t i = 0; i < 4; ++i)
    {
        pkmn::database::move_entry entry(moves.at(i).move, get_game());
        EXPECT_EQ(entry.get_move_id(), int(native_pc->moves[i]));
        EXPECT_EQ(
            moves.at(i).pp,
            int(native_pc->move_pps[i] & PKSAV_GEN2_POKEMON_MOVE_PP_MASK)
        );
        EXPECT_EQ(
            3,
            PKSAV_GEN2_POKEMON_MOVE_PP_UP(native_pc->move_pps[i])
        );
    }

    const std::map<pkmn::e_stat, int>& EVs = pokemon->get_EVs();
    EXPECT_EQ(EVs.at(pkmn::e_stat::HP), int(pksav_bigendian16(native_pc->ev_hp)));
    EXPECT_EQ(EVs.at(pkmn::e_stat::ATTACK), int(pksav_bigendian16(native_pc->ev_atk)));
    EXPECT_EQ(EVs.at(pkmn::e_stat::DEFENSE), int(pksav_bigendian16(native_pc->ev_def)));
    EXPECT_EQ(EVs.at(pkmn::e_stat::SPEED), int(pksav_bigendian16(native_pc->ev_spd)));
    EXPECT_EQ(EVs.at(pkmn::e_stat::SPECIAL), int(pksav_bigendian16(native_pc->ev_spcl)));

    uint8_t pksav_IVs[PKSAV_NUM_GB_IVS] = {0};
    PKSAV_CALL(
        pksav_get_gb_IVs(
            &native_pc->iv_data,
            pksav_IVs,
            sizeof(pksav_IVs)
        )
    )

    EXPECT_EQ(IVs.at(pkmn::e_stat::HP),      int(pksav_IVs[PKSAV_GB_IV_HP]));
    EXPECT_EQ(IVs.at(pkmn::e_stat::ATTACK),  int(pksav_IVs[PKSAV_GB_IV_ATTACK]));
    EXPECT_EQ(IVs.at(pkmn::e_stat::DEFENSE), int(pksav_IVs[PKSAV_GB_IV_DEFENSE]));
    EXPECT_EQ(IVs.at(pkmn::e_stat::SPEED),   int(pksav_IVs[PKSAV_GB_IV_SPEED]));
    EXPECT_EQ(IVs.at(pkmn::e_stat::SPECIAL), int(pksav_IVs[PKSAV_GB_IV_SPECIAL]));

    EXPECT_EQ(pokemon->get_level(), int(native_pc->level));

    /*
     * Party data
     */
    static const pksav::gb_condition_bimap_t& GB_CONDITION_BIMAP = pksav::get_gb_condition_bimap();
    EXPECT_EQ(
        uint8_t(GB_CONDITION_BIMAP.left.at(pokemon->get_condition())),
        native_party_data->condition
    );
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::HP), int(pksav_bigendian16(native_party_data->max_hp)));
    EXPECT_EQ(pokemon->get_current_hp(), int(pksav_bigendian16(native_party_data->current_hp)));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::ATTACK), int(pksav_bigendian16(native_party_data->atk)));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::DEFENSE), int(pksav_bigendian16(native_party_data->def)));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::SPEED), int(pksav_bigendian16(native_party_data->spd)));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::SPECIAL_ATTACK), int(pksav_bigendian16(native_party_data->spatk)));
    EXPECT_EQ(pokemon->get_stats().at(pkmn::e_stat::SPECIAL_DEFENSE) , int(pksav_bigendian16(native_party_data->spdef)));
}

static const std::vector<std::pair<pkmn::e_game, pkmn::e_species>> params =
{
    {pkmn::e_game::GOLD,    pkmn::e_species::CYNDAQUIL},
    {pkmn::e_game::SILVER,  pkmn::e_species::TOTODILE},
    {pkmn::e_game::CRYSTAL, pkmn::e_species::CHIKORITA},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen2_pokemon_test,
    gen2_pokemon_test,
    ::testing::ValuesIn(params)
);
