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
#include <pksav/gba/pokemon.h>
#include <pksav/gba/text.h>
#include <pksav/math/endian.h>

#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/format.hpp>

static const std::vector<std::string> contest_types = boost::assign::list_of
    ("Cool")("Beauty")("Cute")("Smart")("Tough")
;

static const std::vector<std::string> contest_levels = boost::assign::list_of
    ("")(" Super")(" Hyper")(" Master")
;

static const std::vector<std::string> ribbons = boost::assign::list_of
    ("Champion")("Winning")("Victory")("Artist")
    ("Effort")("Marine")("Land")("Sky")
    ("Country")("National")("Earth")("World")
;

class gba_pokemon_test: public pokemon_test {};

static void check_initial_ribbons_map(
    const std::map<std::string, bool>& ribbons_map
) {
    for(auto contest_type_iter = contest_types.begin();
        contest_type_iter != contest_types.end();
        ++contest_type_iter)
    {
        for(auto contest_level_iter = contest_levels.begin();
            contest_level_iter != contest_levels.end();
            ++contest_level_iter)
        {
            std::string ribbon_name = str(boost::format("%s%s")
                                          % contest_type_iter->c_str()
                                          % contest_level_iter->c_str());
            EXPECT_EQ(1, ribbons_map.count(ribbon_name));
            EXPECT_FALSE(ribbons_map.at(ribbon_name));
        }
    }

    for(auto ribbon_iter = ribbons.begin();
        ribbon_iter != ribbons.end();
        ++ribbon_iter)
    {
        EXPECT_EQ(1, ribbons_map.count(*ribbon_iter));
        EXPECT_FALSE(ribbons_map.at(*ribbon_iter));
    }
}

static void test_contest_ribbons(
    pkmn::pokemon::sptr pokemon,
    const std::string &contest_type
) {
    std::string ribbon_name = contest_type;
    std::string super_ribbon_name = contest_type + std::string(" Super");
    std::string hyper_ribbon_name = contest_type + std::string(" Hyper");
    std::string master_ribbon_name = contest_type + std::string(" Master");
    const std::map<std::string, bool>& ribbons = pokemon->get_ribbons();

    pokemon->set_ribbon(hyper_ribbon_name, true);
    EXPECT_TRUE(ribbons.at(ribbon_name));
    EXPECT_TRUE(ribbons.at(super_ribbon_name));
    EXPECT_TRUE(ribbons.at(hyper_ribbon_name));
    EXPECT_FALSE(ribbons.at(master_ribbon_name));

    pokemon->set_ribbon(super_ribbon_name, false);
    EXPECT_TRUE(ribbons.at(ribbon_name));
    EXPECT_FALSE(ribbons.at(super_ribbon_name));
    EXPECT_FALSE(ribbons.at(hyper_ribbon_name));
    EXPECT_FALSE(ribbons.at(master_ribbon_name));
}

static void test_ribbons(
    pkmn::pokemon::sptr pokemon,
    const std::string &ribbon
) {
    std::map<std::string, bool> ribbons_before = pokemon->get_ribbons();
    pokemon->set_ribbon(ribbon, true);
    const std::map<std::string, bool>& ribbons_after = pokemon->get_ribbons();
    for(auto ribbons_iter = ribbons_after.begin();
        ribbons_iter != ribbons_after.end();
        ++ribbons_iter)
    {
        if(ribbons_iter->first == ribbon) {
            EXPECT_TRUE(ribbons_iter->second);
        } else {
            EXPECT_EQ(ribbons_before.at(ribbons_iter->first), ribbons_iter->second);
        }
    }
}

TEST_P(gba_pokemon_test, gba_pokemon_test) {
    pkmn::pokemon::sptr pokemon = get_pokemon();

    pokemon_test_common(
        pokemon,
        {
            "Great Ball",
            {"Friend Ball", "Heal Ball"},

            "Razz Berry",
            {"Berry", "Mach Bike"},

            "Fateful encounter",
            {"Petalburg Woods", "Viridian Forest"},
            {"New Bark Town", "Twinleaf Town"},

            {"Swallow", "Flamethrower", "Return", "Fire Blast"},
            {"Shadow Sky", "Roost"},

            {"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"}, // TODO: test Colosseum/XD
            {"Gold", "HeartGold"}
        }
    );

    // Test ribbons.
    check_initial_ribbons_map(
        pokemon->get_ribbons()
    );
    for(auto contest_type_iter = contest_types.begin();
        contest_type_iter != contest_types.end();
        ++contest_type_iter)
    {
        test_contest_ribbons(pokemon, *contest_type_iter);
    }

    for(auto ribbon_iter = ribbons.begin();
        ribbon_iter != ribbons.end();
        ++ribbon_iter)
    {
        test_ribbons(pokemon, *ribbon_iter);
    }

    /*
     * On the C++ level, check the underlying PKSav struct and make
     * sure our abstractions match.
     */
    const pksav_gba_pc_pokemon_t* native_pc_data = reinterpret_cast<const pksav_gba_pc_pokemon_t*>(
                                                       pokemon->get_native_pc_data()
                                                   );
    const pksav_gba_pokemon_growth_t* growth = &native_pc_data->blocks.growth;
    const pksav_gba_pokemon_attacks_t* attacks = &native_pc_data->blocks.attacks;
    const pksav_gba_pokemon_effort_t* effort = &native_pc_data->blocks.effort;
    const pksav_gba_pokemon_misc_t* misc = &native_pc_data->blocks.misc;

    EXPECT_EQ(pokemon->get_personality(), pksav_littleendian32(native_pc_data->personality));
    EXPECT_EQ(pokemon->get_trainer_id(), pksav_littleendian32(native_pc_data->ot_id.id));
    EXPECT_EQ(pokemon->get_trainer_public_id(), pksav_littleendian16(native_pc_data->ot_id.pid));
    EXPECT_EQ(pokemon->get_trainer_secret_id(), pksav_littleendian16(native_pc_data->ot_id.sid));

    char nickname[11] = {0};
    PKSAV_CALL(
        pksav_text_from_gba(
            native_pc_data->nickname,
            nickname,
            10
        );
    );
    EXPECT_EQ(pokemon->get_nickname(), std::string(nickname));

    // TODO: language

    char otname[8] = {0};
    PKSAV_CALL(
        pksav_text_from_gba(
            native_pc_data->otname,
            otname,
            7
        );
    );
    EXPECT_EQ(pokemon->get_trainer_name(), std::string(otname));

    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_CIRCLE);
    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_TRIANGLE);
    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_SQUARE);
    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_HEART);

    EXPECT_EQ(pokemon->get_database_entry().get_pokemon_index(), int(pksav_littleendian16(growth->species)));
    EXPECT_EQ(pokemon->get_held_item().get_item_index(), int(pksav_littleendian16(growth->held_item)));
    EXPECT_EQ(pokemon->get_experience(), int(pksav_littleendian32(growth->exp)));
    // TODO: PP Up
    EXPECT_EQ(pokemon->get_friendship(), int(growth->friendship));

    const pkmn::move_slots_t& move_slots = pokemon->get_moves();
    for(int i = 0; i < 4; ++i) {
        EXPECT_EQ(move_slots.at(i).move.get_move_id(), int(attacks->moves[i]));
        EXPECT_EQ(move_slots.at(i).pp, int(attacks->move_pps[i]));
    }

    const std::map<std::string, int>& EVs = pokemon->get_EVs();
    EXPECT_EQ(EVs.at("HP"), int(pksav_littleendian16(effort->ev_hp)));
    EXPECT_EQ(EVs.at("Attack"), int(pksav_littleendian16(effort->ev_atk)));
    EXPECT_EQ(EVs.at("Defense"), int(pksav_littleendian16(effort->ev_def)));
    EXPECT_EQ(EVs.at("Speed"), int(pksav_littleendian16(effort->ev_spd)));
    EXPECT_EQ(EVs.at("Special Attack"), int(pksav_littleendian16(effort->ev_spatk)));
    EXPECT_EQ(EVs.at("Special Defense"), int(pksav_littleendian16(effort->ev_spdef)));

    const std::map<std::string, int>& contest_stats = pokemon->get_contest_stats();
    EXPECT_EQ(contest_stats.at("Cool"), int(effort->contest_stats.cool));
    EXPECT_EQ(contest_stats.at("Beauty"), int(effort->contest_stats.beauty));
    EXPECT_EQ(contest_stats.at("Cute"), int(effort->contest_stats.cute));
    EXPECT_EQ(contest_stats.at("Smart"), int(effort->contest_stats.smart));
    EXPECT_EQ(contest_stats.at("Tough"), int(effort->contest_stats.tough));
    EXPECT_EQ(contest_stats.at("Feel"), int(effort->contest_stats.feel));

    // TODO: Pokérus
    // TODO: get location indices for what we set

    EXPECT_EQ(pokemon->get_level_met(), int(misc->origin_info & PKSAV_GBA_LEVEL_MET_MASK));

    // TODO: get ball ID for what we set
    EXPECT_TRUE(misc->origin_info & PKSAV_GBA_OTGENDER_MASK);

    const std::map<std::string, int>& IVs = pokemon->get_IVs();
    uint8_t IV = 0;

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_HP,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("HP"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_ATTACK,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Attack"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_DEFENSE,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Defense"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPEED,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Speed"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPATK,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Special Attack"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPDEF,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Special Defense"), int(IV));

    EXPECT_TRUE(!(misc->iv_egg_ability & PKSAV_GBA_EGG_MASK));
    EXPECT_TRUE(!(misc->iv_egg_ability & PKSAV_GBA_ABILITY_MASK));

    /*
     * Party data
     */
    const pksav_gba_pokemon_party_data_t* native_party_data = reinterpret_cast<const pksav_gba_pokemon_party_data_t*>(
                                                                  pokemon->get_native_party_data()
                                                              );
    // TODO: condition

    EXPECT_EQ(pokemon->get_level(), int(native_party_data->level));

    // TODO: Pokérus

    const std::map<std::string, int>& stats = pokemon->get_stats();
    EXPECT_EQ(stats.at("HP"), int(pksav_littleendian16(native_party_data->current_hp)));
    EXPECT_EQ(stats.at("HP"), int(pksav_littleendian16(native_party_data->max_hp)));
    EXPECT_EQ(stats.at("Attack"), int(pksav_littleendian16(native_party_data->atk)));
    EXPECT_EQ(stats.at("Defense"), int(pksav_littleendian16(native_party_data->def)));
    EXPECT_EQ(stats.at("Speed"), int(pksav_littleendian16(native_party_data->spd)));
    EXPECT_EQ(stats.at("Special Attack"), int(pksav_littleendian16(native_party_data->spatk)));
    EXPECT_EQ(stats.at("Special Defense"), int(pksav_littleendian16(native_party_data->spdef)));
}

static const std::vector<std::pair<std::string, std::string>> params = {
    {"Ruby", "Torchic"},
    {"Sapphire", "Mudkip"},
    {"Emerald", "Treecko"},
    {"FireRed", "Charmander"},
    {"LeafGreen", "Bulbasaur"}
};

INSTANTIATE_TEST_CASE_P(
    cpp_gba_pokemon_test,
    gba_pokemon_test,
    ::testing::ValuesIn(params)
);
