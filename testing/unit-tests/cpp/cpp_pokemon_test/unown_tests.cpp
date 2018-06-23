/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/form.hpp>
#include <pkmn/pokemon.hpp>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

namespace fs = boost::filesystem;

class gen2_unown_test: public ::testing::TestWithParam<pkmn::e_game> {};
class gen3_unown_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(gen2_unown_test, gen2_unown_test) {
    pkmn::e_game game = GetParam();

    pkmn::database::pokemon_entry unown_entry(
        "Unown",
        game,
        ""
    );
    std::vector<std::string> unown_forms = unown_entry.get_forms();

    for(int i = 0; i < 26; ++i) {
        pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                        "Unown",
                                        game,
                                        unown_forms[i],
                                        5
                                    );
        EXPECT_EQ(unown_forms[i], unown->get_form());

        // Make sure IVs are properly set
        const std::map<pkmn::e_stat, int>& IVs = unown->get_IVs();
        std::string form_from_IVs = pkmn::calculations::gen2_unown_form(
                                        IVs.at(pkmn::e_stat::ATTACK),
                                        IVs.at(pkmn::e_stat::DEFENSE),
                                        IVs.at(pkmn::e_stat::SPEED),
                                        IVs.at(pkmn::e_stat::SPECIAL)
                                    );
        EXPECT_EQ(form_from_IVs, unown->get_form());
        EXPECT_TRUE(fs::exists(unown->get_icon_filepath()));
        EXPECT_TRUE(fs::exists(unown->get_sprite_filepath()));
    }

    pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                    "Unown",
                                    game,
                                    "A",
                                    5
                                );
    const std::map<pkmn::e_stat, int>& IVs = unown->get_IVs();

    // Make sure setting the form properly changes the IVs.
    for(int i = 0; i < 26; ++i) {
        unown->set_form(unown_forms[i]);
        std::string form_from_IVs = pkmn::calculations::gen2_unown_form(
                                        IVs.at(pkmn::e_stat::ATTACK),
                                        IVs.at(pkmn::e_stat::DEFENSE),
                                        IVs.at(pkmn::e_stat::SPEED),
                                        IVs.at(pkmn::e_stat::SPECIAL)
                                    );
        EXPECT_EQ(form_from_IVs, unown->get_form());
    }

    // Make sure setting IVs properly changes the form.
    unown->set_IV(pkmn::e_stat::ATTACK, 10);
    unown->set_IV(pkmn::e_stat::DEFENSE, 9);
    unown->set_IV(pkmn::e_stat::SPEED, 1);
    unown->set_IV(pkmn::e_stat::SPECIAL, 14);
    EXPECT_EQ("G", unown->get_form());
}

TEST_P(gen3_unown_test, gen3_unown_test) {
    pkmn::e_game game = GetParam();

    pkmn::database::pokemon_entry unown_entry(
        "Unown",
        game,
        ""
    );
    std::vector<std::string> unown_forms = unown_entry.get_forms();

    for(auto form_iter = unown_forms.begin(); form_iter != unown_forms.end(); ++form_iter) {
        pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                        "Unown",
                                        game,
                                        *form_iter,
                                        5
                                    );
        EXPECT_EQ(*form_iter, unown->get_form());

        EXPECT_TRUE(fs::exists(unown->get_icon_filepath()));
        EXPECT_TRUE(fs::exists(unown->get_sprite_filepath()));
    }

    // Make sure setting the personality properly sets the form.
    pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                    "Unown",
                                    game,
                                    "A",
                                    5
                                );
    unown->set_personality(0x4C07DE71);
    EXPECT_EQ("B", unown->get_form());

    // Make sure setting the form properly sets the personality.
    for(auto form_iter = unown_forms.begin(); form_iter != unown_forms.end(); ++form_iter) {
        unown->set_form(*form_iter);
        EXPECT_EQ(*form_iter, unown->get_form());
        std::string form_from_personality = pkmn::calculations::gen3_unown_form(
                                                unown->get_personality()
                                            );
        EXPECT_EQ(*form_iter, form_from_personality);
    }
}

static const std::vector<pkmn::e_game> gen2_games =
{
    pkmn::e_game::GOLD,
    pkmn::e_game::SILVER,
    pkmn::e_game::CRYSTAL
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen2_unown_test,
    gen2_unown_test,
    ::testing::ValuesIn(gen2_games)
);

static const std::vector<pkmn::e_game> gen3_games =
{
    pkmn::e_game::RUBY,
    pkmn::e_game::SAPPHIRE,
    pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED,
    pkmn::e_game::LEAFGREEN,
    pkmn::e_game::COLOSSEUM,
    pkmn::e_game::XD
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen3_unown_test,
    gen3_unown_test,
    ::testing::ValuesIn(gen3_games)
);
