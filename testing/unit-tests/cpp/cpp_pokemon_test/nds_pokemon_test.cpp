/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/move_entry.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/common/nds_pokemon.h>
#include <pksav/gen4/text.h>
#include <pksav/gen5/text.h>
#include <pksav/math/endian.h>

#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/format.hpp>

static void test_nds_pokemon_common(
    pkmn::pokemon::sptr pokemon,
    bool gen4
) {
    // TODO: ribbons

    /*
     * On the C++ level, check the underlying PKSav struct and make
     * sure our abstractions match.
     */
    const pksav_nds_pc_pokemon_t* native_pc_data = reinterpret_cast<const pksav_nds_pc_pokemon_t*>(
                                                       pokemon->get_native_pc_data()
                                                   );
    const pksav_nds_pokemon_blockA_t* blockA = &native_pc_data->blocks.blockA;
    const pksav_nds_pokemon_blockB_t* blockB = &native_pc_data->blocks.blockB;
    const pksav_nds_pokemon_blockC_t* blockC = &native_pc_data->blocks.blockC;
    const pksav_nds_pokemon_blockD_t* blockD = &native_pc_data->blocks.blockD;

    EXPECT_EQ(pokemon->get_personality(), pksav_littleendian32(native_pc_data->personality));
    EXPECT_TRUE(native_pc_data->isdecrypted_isegg & PKSAV_NDS_PC_DATA_DECRYPTED_MASK);
    EXPECT_FALSE(native_pc_data->isdecrypted_isegg & PKSAV_NDS_IS_EGG_MASK);

    EXPECT_EQ(
        pokemon->get_database_entry().get_pokemon_index(),
        int(pksav_littleendian16(blockA->species))
    );
    EXPECT_EQ(
        pkmn::database::item_entry(
            pokemon->get_held_item(),
            pokemon->get_game()
        ).get_item_index(),
        int(pksav_littleendian16(blockA->held_item))
    );
    EXPECT_EQ(
        pokemon->get_original_trainer_id(),
        int(pksav_littleendian32(blockA->ot_id.id))
    );
    EXPECT_EQ(
        pokemon->get_original_trainer_public_id(),
        int(pksav_littleendian32(blockA->ot_id.pid))
    );
    EXPECT_EQ(
        pokemon->get_original_trainer_secret_id(),
        int(pksav_littleendian32(blockA->ot_id.sid))
    );
    EXPECT_EQ(
        pokemon->get_experience(),
        int(pksav_littleendian32(blockA->exp))
    );
    EXPECT_EQ(
        pokemon->get_current_trainer_friendship(),
        int(blockA->friendship)
    );
    // TODO: check ability

    const std::map<std::string, bool>& markings = pokemon->get_markings();
    EXPECT_EQ(
        markings.at("Circle"),
        bool(blockA->markings & PKSAV_MARKING_CIRCLE)
    );
    EXPECT_EQ(
        markings.at("Triangle"),
        bool(blockA->markings & PKSAV_MARKING_TRIANGLE)
    );
    EXPECT_EQ(
        markings.at("Square"),
        bool(blockA->markings & PKSAV_MARKING_SQUARE)
    );
    EXPECT_EQ(
        markings.at("Heart"),
        bool(blockA->markings & PKSAV_MARKING_HEART)
    );
    EXPECT_EQ(
        markings.at("Star"),
        bool(blockA->markings & PKSAV_MARKING_STAR)
    );
    EXPECT_EQ(
        markings.at("Diamond"),
        bool(blockA->markings & PKSAV_MARKING_DIAMOND)
    );

    // TODO: check country

    const std::map<std::string, int>& EVs = pokemon->get_EVs();
    EXPECT_EQ(
        EVs.at("HP"),
        int(blockA->ev_hp)
    );
    EXPECT_EQ(
        EVs.at("Attack"),
        int(blockA->ev_atk)
    );
    EXPECT_EQ(
        EVs.at("Defense"),
        int(blockA->ev_def)
    );
    EXPECT_EQ(
        EVs.at("Speed"),
        int(blockA->ev_spd)
    );
    EXPECT_EQ(
        EVs.at("Special Attack"),
        int(blockA->ev_spatk)
    );
    EXPECT_EQ(
        EVs.at("Special Defense"),
        int(blockA->ev_spdef)
    );

    const std::map<std::string, int>& contest_stats = pokemon->get_contest_stats();
    EXPECT_EQ(
        contest_stats.at("Cool"),
        int(blockA->contest_stats.cool)
    );
    EXPECT_EQ(
        contest_stats.at("Beauty"),
        int(blockA->contest_stats.beauty)
    );
    EXPECT_EQ(
        contest_stats.at("Cute"),
        int(blockA->contest_stats.cute)
    );
    EXPECT_EQ(
        contest_stats.at("Smart"),
        int(blockA->contest_stats.smart)
    );
    EXPECT_EQ(
        contest_stats.at("Tough"),
        int(blockA->contest_stats.tough)
    );
    EXPECT_EQ(
        contest_stats.at("Sheen"),
        int(blockA->contest_stats.sheen)
    );

    const pkmn::move_slots_t& move_slots = pokemon->get_moves();
    ASSERT_EQ(4, move_slots.size());
    for(size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(
            pkmn::database::move_entry(
                move_slots.at(i).move,
                pokemon->get_game()
            ).get_move_id(),
            int(pksav_littleendian16(blockB->moves[i]))
        );
        EXPECT_EQ(
            move_slots.at(i).pp,
            int(blockB->move_pps[i])
        );
    }

    uint8_t IV;
    const std::map<std::string, int>& IVs = pokemon->get_IVs();

    PKSAV_CALL(
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_HP,
            &IV
        )
    );
    EXPECT_EQ(IVs.at("HP"), int(IV));
    PKSAV_CALL(
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_ATTACK,
            &IV
        )
    );
    EXPECT_EQ(IVs.at("Attack"), int(IV));
    PKSAV_CALL(
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_DEFENSE,
            &IV
        )
    );
    EXPECT_EQ(IVs.at("Defense"), int(IV));
    PKSAV_CALL(
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_SPEED,
            &IV
        )
    );
    EXPECT_EQ(IVs.at("Speed"), int(IV));
    PKSAV_CALL(
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_SPATK,
            &IV
        )
    );
    EXPECT_EQ(IVs.at("Special Attack"), int(IV));
    PKSAV_CALL(
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_SPDEF,
            &IV
        )
    );
    EXPECT_EQ(IVs.at("Special Defense"), int(IV));

    if(not gen4) {
        EXPECT_EQ(
            (pokemon->get_personality() % 25),
            int(blockB->nature)
        );
    }

    char nickname[11] = {0};
    if(gen4) {
        PKSAV_CALL(
            pksav_text_from_gen4(
                blockC->nickname,
                nickname,
                sizeof(nickname)
            )
        );
    } else {
        PKSAV_CALL(
            pksav_text_from_gen5(
                blockC->nickname,
                nickname,
                sizeof(nickname)
            )
        );
    }
    EXPECT_EQ(
        pokemon->get_nickname(),
        std::string(nickname)
    );
    // TODO: hometown

    char otname[8] = {0};
    if(gen4) {
        PKSAV_CALL(
            pksav_text_from_gen4(
                blockD->otname,
                otname,
                sizeof(otname)
            )
        );
    } else {
        PKSAV_CALL(
            pksav_text_from_gen5(
                blockD->otname,
                otname,
                sizeof(otname)
            )
        );
    }
    EXPECT_EQ(
        pokemon->get_original_trainer_name(),
        std::string(otname)
    );

    pkmn::datetime eggmet_date = pokemon->get_date_met(true);
    EXPECT_EQ(eggmet_date.year, int(blockD->eggmet_date.year + 2000));
    EXPECT_EQ(eggmet_date.month, int(blockD->eggmet_date.month));
    EXPECT_EQ(eggmet_date.day, int(blockD->eggmet_date.day));
    EXPECT_EQ(0, eggmet_date.hour);
    EXPECT_EQ(0, eggmet_date.minute);
    EXPECT_EQ(0, eggmet_date.second);

    pkmn::datetime met_date = pokemon->get_date_met(false);
    EXPECT_EQ(met_date.year, int(blockD->met_date.year + 2000));
    EXPECT_EQ(met_date.month, int(blockD->met_date.month));
    EXPECT_EQ(met_date.day, int(blockD->met_date.day));
    EXPECT_EQ(0, met_date.hour);
    EXPECT_EQ(0, met_date.minute);
    EXPECT_EQ(0, met_date.second);

    // TODO: stuff that needs IDs

    EXPECT_EQ(
        pokemon->get_level_met(),
        int(blockD->metlevel_otgender & PKSAV_NDS_LEVELMET_MASK)
    );
}

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
            {"Gold", "Black", "Omega Ruby"}
        }
    );

    test_nds_pokemon_common(pokemon, true);
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

    test_nds_pokemon_common(pokemon, false);
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
