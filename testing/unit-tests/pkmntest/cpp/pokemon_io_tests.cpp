/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_io_tests.hpp>

#include <boost/filesystem.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace fs = boost::filesystem;

namespace pkmntest {
    
    static const fs::path LIBPKMN_TEST_FILES("@LIBPKMN_TEST_FILES@");

    void test_3gpkm() {
        /*
         * Test files in repo and compare to known values.
         */
        const fs::path _3GPKM_DIR = LIBPKMN_TEST_FILES / "3gpkm";

        pkmn::pokemon::sptr mightyena = pkmn::pokemon::from_file(
                                            (_3GPKM_DIR / "MIGHTYENA.3gpkm").string()
                                        );
        BOOST_CHECK_EQUAL(
            mightyena->get_species(),
            "Mightyena"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_game(),
            "Emerald"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_form(),
            "Standard"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_nickname(),
            "MIGHTYENA"
        );
        BOOST_CHECK(not mightyena->is_shiny());
        BOOST_CHECK_EQUAL(
            mightyena->get_held_item().get_name(),
            "Heart Scale"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_trainer_name(),
            "A"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_trainer_public_id(),
            61415
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_trainer_secret_id(),
            3417
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_trainer_id(),
            223997927
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_trainer_gender(),
            "Female"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_friendship(),
            254
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_ability(),
            "Intimidate"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_ball(),
            "Great Ball"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_level_met(),
            25
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_location_met(false),
            "Route 120"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_original_game(),
            "Emerald"
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_personality(),
            3557601241
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_experience(),
            128734
        );
        BOOST_CHECK_EQUAL(
            mightyena->get_level(),
            50
        );

        const std::map<std::string, bool>& mightyena_markings = mightyena->get_markings();
        BOOST_CHECK_EQUAL(mightyena_markings.size(), 4);
        for(auto iter = mightyena_markings.begin(); iter != mightyena_markings.end(); ++iter) {
            BOOST_CHECK(not iter->second);
        }

        const std::map<std::string, bool>& mightyena_ribbons = mightyena->get_ribbons();
        BOOST_CHECK_EQUAL(mightyena_ribbons.size(), 32);
        for(auto iter = mightyena_ribbons.begin(); iter != mightyena_ribbons.end(); ++iter) {
            if(iter->first == "Champion") {
                BOOST_CHECK(iter->second);
            } else {
                BOOST_CHECK(not iter->second);
            }
        }

        const std::map<std::string, int>& mightyena_contest_stats = mightyena->get_contest_stats();
        BOOST_CHECK_EQUAL(mightyena_contest_stats.size(), 6);
        for(auto iter = mightyena_contest_stats.begin(); iter != mightyena_contest_stats.end(); ++iter) {
            BOOST_CHECK_EQUAL(iter->second, 0);
        }

        static const std::string expected_mightyena_moves[] = {
            "Crunch", "Strength", "Shadow Ball", "Double-Edge"
        };
        const pkmn::move_slots_t mightyena_moves = mightyena->get_moves();
        BOOST_CHECK_EQUAL(mightyena_moves.size(), 4);
        for(int i = 0; i < 4; ++i) {
            BOOST_CHECK_EQUAL(mightyena_moves[i].move.get_name(), expected_mightyena_moves[i]);
        }

        const std::map<std::string, int>& mightyena_EVs = mightyena->get_EVs();
        BOOST_CHECK_EQUAL(mightyena_EVs.size(), 6);
        BOOST_CHECK_EQUAL(mightyena_EVs.at("HP"), 30);
        BOOST_CHECK_EQUAL(mightyena_EVs.at("Attack"), 110);
        BOOST_CHECK_EQUAL(mightyena_EVs.at("Defense"), 32);
        BOOST_CHECK_EQUAL(mightyena_EVs.at("Speed"), 48);
        BOOST_CHECK_EQUAL(mightyena_EVs.at("Special Attack"), 17);
        BOOST_CHECK_EQUAL(mightyena_EVs.at("Special Defense"), 83);

        const std::map<std::string, int>& mightyena_IVs = mightyena->get_IVs();
        BOOST_CHECK_EQUAL(mightyena_IVs.size(), 6);
        BOOST_CHECK_EQUAL(mightyena_IVs.at("HP"), 26);
        BOOST_CHECK_EQUAL(mightyena_IVs.at("Attack"), 28);
        BOOST_CHECK_EQUAL(mightyena_IVs.at("Defense"), 4);
        BOOST_CHECK_EQUAL(mightyena_IVs.at("Speed"), 13);
        BOOST_CHECK_EQUAL(mightyena_IVs.at("Special Attack"), 25);
        BOOST_CHECK_EQUAL(mightyena_IVs.at("Special Defense"), 26);

        const std::map<std::string, int>& mightyena_stats = mightyena->get_stats();
        BOOST_CHECK_EQUAL(mightyena_stats.size(), 6);
        BOOST_CHECK_EQUAL(mightyena_stats.at("HP"), 146);
        BOOST_CHECK_EQUAL(mightyena_stats.at("Attack"), 122);
        BOOST_CHECK_EQUAL(mightyena_stats.at("Defense"), 81);
        BOOST_CHECK_EQUAL(mightyena_stats.at("Speed"), 87);
        BOOST_CHECK_EQUAL(mightyena_stats.at("Special Attack"), 79);
        BOOST_CHECK_EQUAL(mightyena_stats.at("Special Defense"), 88);
    }

}
