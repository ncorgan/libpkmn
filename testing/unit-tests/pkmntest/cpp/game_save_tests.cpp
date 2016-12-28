/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/game_save_tests.hpp>

#include <pkmn/exception.hpp>

#include <boost/config.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

#include <string>

// TODO: replace with pokemon static fields
BOOST_STATIC_CONSTEXPR const char* LIBPKMN_OT_NAME = "LibPKMN";
BOOST_STATIC_CONSTEXPR const char* TOO_LONG_OT_NAME = "LibPKMNLibPKMN";
BOOST_STATIC_CONSTEXPR uint32_t LIBPKMN_OT_ID = 2105214279;
BOOST_STATIC_CONSTEXPR uint16_t LIBPKMN_OT_PID = 1351;
BOOST_STATIC_CONSTEXPR uint16_t LIBPKMN_OT_SID = 32123;

BOOST_STATIC_CONSTEXPR int MONEY_MAX_VALUE = 999999;

static const std::string GB_GAMES[] = {
    "Red", "Blue", "Yellow",
    "Gold",  "Silver", "Crystal"
};

static const std::string RIVAL_NAME_SET_GAMES[] = {
    "Ruby", "Sapphire", "Emerald",
    "Black", "White",
    "X", "Y"
};

namespace pkmntest {

    static void test_trainer_name(
        pkmn::game_save::sptr save
    ) {
        BOOST_CHECK_THROW(
            save->set_trainer_name("");
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            save->set_trainer_name(TOO_LONG_OT_NAME);
        , std::invalid_argument);

        save->set_trainer_name(LIBPKMN_OT_NAME);
        BOOST_CHECK_EQUAL(
            save->get_trainer_name(),
            std::string(LIBPKMN_OT_NAME)
        );
    }

    static void test_rival_name(
        pkmn::game_save::sptr save,
        bool is_rival_name_set
    ) {
        if(is_rival_name_set) {
            BOOST_CHECK_THROW(
                save->set_rival_name(LIBPKMN_OT_NAME);
            , pkmn::feature_not_in_game_error);
            BOOST_CHECK_THROW(
                save->get_rival_name();
            , pkmn::feature_not_in_game_error);
        } else {
            BOOST_CHECK_THROW(
                save->set_rival_name("");
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                save->set_rival_name(TOO_LONG_OT_NAME);
            , std::invalid_argument);

            save->set_rival_name(LIBPKMN_OT_NAME);
            BOOST_CHECK_EQUAL(
                save->get_rival_name(),
                std::string(LIBPKMN_OT_NAME)
            );
        }
    }

    static void test_trainer_id(
        pkmn::game_save::sptr save,
        bool is_gb_game
    ) {
        BOOST_CHECK_EQUAL(
            save->get_trainer_id(),
            is_gb_game ? LIBPKMN_OT_PID : LIBPKMN_OT_ID
        );
        BOOST_CHECK_EQUAL(
            save->get_trainer_public_id(),
            LIBPKMN_OT_PID
        );
        if(is_gb_game) {
            BOOST_CHECK_THROW(
                save->get_trainer_secret_id();
            , pkmn::feature_not_in_game_error);
        } else {
            BOOST_CHECK_EQUAL(
                save->get_trainer_secret_id(),
                LIBPKMN_OT_SID
            );
        }
    }

    void game_save_test_common_fields(
        pkmn::game_save::sptr save
    ) {
        test_trainer_name(save);

        bool is_gb_game = (
            std::find(
                GB_GAMES,
                GB_GAMES+6,
                save->get_game()
            ) == GB_GAMES+6
        );
        save->set_trainer_id(
            is_gb_game ? LIBPKMN_OT_PID : LIBPKMN_OT_ID
        );
        test_trainer_id(
            save,
            is_gb_game
        );

        bool is_rival_name_set = (
            std::find(
                RIVAL_NAME_SET_GAMES,
                RIVAL_NAME_SET_GAMES+7,
                save->get_game()
            ) == RIVAL_NAME_SET_GAMES+7
        );
        test_rival_name(
            save,
            is_rival_name_set
        );

        if(is_gb_game) {
            BOOST_CHECK_THROW(
                save->set_trainer_id(LIBPKMN_OT_ID);
            , pkmn::range_error);
            BOOST_CHECK_THROW(
                save->set_trainer_secret_id(LIBPKMN_OT_SID);
            , pkmn::feature_not_in_game_error);
        } else {
            save->set_trainer_secret_id(LIBPKMN_OT_SID);
            test_trainer_id(
                save,
                is_gb_game
            );
        }

        save->set_trainer_public_id(LIBPKMN_OT_PID);
        test_trainer_id(
            save,
            is_gb_game
        );

        BOOST_CHECK_THROW(
            save->set_money(-1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            save->set_money(MONEY_MAX_VALUE+1);
        , pkmn::range_error);

        save->set_money(123456);
        BOOST_CHECK_EQUAL(
            save->get_money(),
            123456
        );
    }
}
