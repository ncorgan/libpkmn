/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>
#include <pkmn/database/item_entry.hpp>

#include "pksav/pksav_call.hpp"

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
            "Great Ball",
            {"Great Ball"},

            "Berry",
            {"Razz Berry", "Bicycle"},

            "Special",
            {"Sprout Tower", "Tohjo Falls"},
            {"Littleroot Town", "Petalburg Woods"},

            {"Slash", "Flamethrower", "Return", "Fire Blast"},
            {"Frenzy Plant", "Roost"},

            {"Gold"},
            {"Gold"}
        }
    );

    // Gender affects IVs, so make sure the abstraction reflects that.
    const std::map<std::string, int>& IVs = pokemon->get_IVs();
    pokemon->set_gender("Male");
    EXPECT_EQ(15, IVs.at("Attack"));
    pokemon->set_gender("Female");
    EXPECT_LT(IVs.at("Attack"), 15);

    pokemon->set_IV("Attack", 0);
    EXPECT_EQ("Female", pokemon->get_gender());
    pokemon->set_IV("Attack", 15);
    EXPECT_EQ("Male", pokemon->get_gender());

    // Shininess affects IVs, so make sure the abstraction reflects that.
    pokemon->set_shininess(false);
    EXPECT_FALSE(pokemon->is_shiny());
    EXPECT_EQ(13, IVs.at("Attack"));

    pokemon->set_shininess(true);
    EXPECT_TRUE(pokemon->is_shiny());
    EXPECT_EQ(15, IVs.at("Attack"));
    EXPECT_EQ(10, IVs.at("Defense"));
    EXPECT_EQ(10, IVs.at("Speed"));
    EXPECT_EQ(10, IVs.at("Special"));

    // On the C++ level, make sure functions that affect the same PKSav field don't impact each other.
    std::string location_met_before_change = pokemon->get_location_met(false);
    std::string trainer_gender_before_change = pokemon->get_original_trainer_gender();
    int level_met_before_change = pokemon->get_level_met();

    const pksav_gen2_pc_pokemon_t* native_pc = reinterpret_cast<const pksav_gen2_pc_pokemon_t*>(
                                                   pokemon->get_native_pc_data()
                                               );
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
    pokemon->set_original_trainer_gender("Female");
    EXPECT_EQ("Female", pokemon->get_original_trainer_gender());
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
    const pksav_gen2_pokemon_party_data_t* native_party_data = reinterpret_cast<const pksav_gen2_pokemon_party_data_t*>(
                                                                   pokemon->get_native_party_data()
                                                               );

    EXPECT_EQ(native_pc->species, uint8_t(pokemon->get_database_entry().get_pokemon_index()));
    EXPECT_EQ(
        native_pc->held_item,
        uint8_t(pkmn::database::item_entry(pokemon->get_held_item(), get_game()).get_item_index())
    );
    const pkmn::move_slots_t& move_slots = pokemon->get_moves();
    for(int i = 0; i < 4; ++i) {
        EXPECT_EQ(
            uint8_t(pkmn::database::move_entry(move_slots.at(i).move, get_game()).get_move_id()),
            native_pc->moves[i]
        );
        EXPECT_EQ(uint8_t(move_slots.at(i).pp), (native_pc->move_pps[i] & PKSAV_GEN2_MOVE_PP_MASK));
    }
    EXPECT_EQ(uint16_t(pokemon->get_original_trainer_id()), pksav_bigendian16(native_pc->ot_id));
    EXPECT_EQ(pokemon->get_original_trainer_public_id(), pksav_bigendian16(native_pc->ot_id));

    const std::map<std::string, int>& EVs = pokemon->get_EVs();
    EXPECT_EQ(EVs.at("HP"), int(pksav_bigendian16(native_pc->ev_hp)));
    EXPECT_EQ(EVs.at("Attack"), int(pksav_bigendian16(native_pc->ev_atk)));
    EXPECT_EQ(EVs.at("Defense"), int(pksav_bigendian16(native_pc->ev_def)));
    EXPECT_EQ(EVs.at("Speed"), int(pksav_bigendian16(native_pc->ev_spd)));
    EXPECT_EQ(EVs.at("Special"), int(pksav_bigendian16(native_pc->ev_spcl)));

    uint8_t IV = 0;

    PKSAV_CALL(
        pksav_get_gb_IV(
            &native_pc->iv_data,
            PKSAV_STAT_HP,
            &IV
        )
    )
    EXPECT_EQ(IVs.at("HP"), int(IV));
    PKSAV_CALL(
        pksav_get_gb_IV(
            &native_pc->iv_data,
            PKSAV_STAT_ATTACK,
            &IV
        )
    )
    EXPECT_EQ(IVs.at("Attack"), int(IV));
    PKSAV_CALL(
        pksav_get_gb_IV(
            &native_pc->iv_data,
            PKSAV_STAT_DEFENSE,
            &IV
        )
    )
    EXPECT_EQ(IVs.at("Defense"), int(IV));
    PKSAV_CALL(
        pksav_get_gb_IV(
            &native_pc->iv_data,
            PKSAV_STAT_SPEED,
            &IV
        )
    )
    EXPECT_EQ(IVs.at("Speed"), int(IV));
    PKSAV_CALL(
        pksav_get_gb_IV(
            &native_pc->iv_data,
            PKSAV_STAT_SPECIAL,
            &IV
        )
    )
    EXPECT_EQ(IVs.at("Special"), int(IV));
    EXPECT_EQ(pokemon->get_level(), int(native_pc->level));

    /*
     * Party data
     */
    EXPECT_EQ(pokemon->get_stats().at("HP"), int(pksav_bigendian16(native_party_data->max_hp)));
    EXPECT_EQ(pokemon->get_current_hp(), int(pksav_bigendian16(native_party_data->current_hp)));
    EXPECT_EQ(pokemon->get_stats().at("Attack"), int(pksav_bigendian16(native_party_data->atk)));
    EXPECT_EQ(pokemon->get_stats().at("Defense"), int(pksav_bigendian16(native_party_data->def)));
    EXPECT_EQ(pokemon->get_stats().at("Speed"), int(pksav_bigendian16(native_party_data->spd)));
    EXPECT_EQ(pokemon->get_stats().at("Special Attack"), int(pksav_bigendian16(native_party_data->spatk)));
    EXPECT_EQ(pokemon->get_stats().at("Special Defense") , int(pksav_bigendian16(native_party_data->spdef)));
}

static const std::vector<std::pair<std::string, std::string>> params = {
    {"Gold", "Cyndaquil"},
    {"Silver", "Totodile"},
    {"Crystal", "Chikorita"}
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen2_pokemon_test,
    gen2_pokemon_test,
    ::testing::ValuesIn(params)
);
