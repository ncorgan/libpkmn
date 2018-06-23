/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"

#include <pkmntest/util.hpp>

#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/exception.hpp>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <iostream>

namespace fs = boost::filesystem;

static const std::pair<std::string, std::string> none_pair("None", "None");
static const std::pair<std::string, std::string> invalid_pair("Unknown", "Unknown");
static const std::map<pkmn::e_stat, int> bad_stat_map_old = boost::assign::map_list_of
    (pkmn::e_stat::HP, 0)
    (pkmn::e_stat::ATTACK, 0)
    (pkmn::e_stat::DEFENSE, 0)
    (pkmn::e_stat::SPEED, 0)
    (pkmn::e_stat::SPECIAL, 0)
;
static const std::map<pkmn::e_stat, int> bad_stat_map = boost::assign::map_list_of
    (pkmn::e_stat::HP, 0)
    (pkmn::e_stat::ATTACK, 0)
    (pkmn::e_stat::DEFENSE, 0)
    (pkmn::e_stat::SPEED, 0)
    (pkmn::e_stat::SPECIAL_ATTACK, 0)
    (pkmn::e_stat::SPECIAL_DEFENSE, 0)
;

BOOST_STATIC_CONSTEXPR int YELLOW = 3;
BOOST_STATIC_CONSTEXPR int CRYSTAL = 6;
BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;
BOOST_STATIC_CONSTEXPR int XD = 20;
BOOST_STATIC_CONSTEXPR int X = 23;

class pokemon_entry_none_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(pokemon_entry_none_test, pokemon_entry_none_test) {
    pkmn::database::pokemon_entry none_entry("None", GetParam(), "");

    EXPECT_EQ("None", none_entry.get_name());
    EXPECT_EQ("None", none_entry.get_species());
    EXPECT_EQ("None", none_entry.get_pokedex_entry());
    EXPECT_EQ("None", none_entry.get_form());
    EXPECT_FLOAT_EQ(-1.0f, none_entry.get_height());
    EXPECT_FLOAT_EQ(-1.0f, none_entry.get_weight());
    EXPECT_FLOAT_EQ(-1.0f, none_entry.get_chance_male());
    EXPECT_FLOAT_EQ(-1.0f, none_entry.get_chance_female());
    EXPECT_TRUE(not none_entry.has_gender_differences());
    EXPECT_EQ(-1.0f, none_entry.get_base_friendship());
    EXPECT_TRUE(none_entry.get_types() == none_pair);
    EXPECT_TRUE(none_entry.get_abilities() == none_pair);
    EXPECT_EQ("None", none_entry.get_hidden_ability());
    EXPECT_TRUE(none_entry.get_egg_groups() == none_pair);
    if(none_entry.get_game_id() <= YELLOW) {
        EXPECT_TRUE(none_entry.get_base_stats() == bad_stat_map_old);
        EXPECT_TRUE(none_entry.get_EV_yields() == bad_stat_map_old);
    } else if(none_entry.get_game_id() <= CRYSTAL) {
        EXPECT_TRUE(none_entry.get_base_stats() == bad_stat_map);
        EXPECT_TRUE(none_entry.get_EV_yields() == bad_stat_map_old);
    } else {
        EXPECT_TRUE(none_entry.get_base_stats() == bad_stat_map);
        EXPECT_TRUE(none_entry.get_EV_yields() == bad_stat_map);
    }
    EXPECT_EQ(-1, none_entry.get_experience_yield());
    EXPECT_EQ(-1, none_entry.get_experience_at_level(2));
    EXPECT_EQ(-1, none_entry.get_level_at_experience(2));
    EXPECT_EQ(0, none_entry.get_levelup_moves().size());
    EXPECT_EQ(0, none_entry.get_tm_hm_moves().size());
    EXPECT_EQ(0, none_entry.get_egg_moves().size());
    EXPECT_EQ(0, none_entry.get_tutor_moves().size());
    EXPECT_EQ(0, none_entry.get_forms().size());
    EXPECT_EQ(0, none_entry.get_evolutions().size());

    EXPECT_TRUE(fs::exists(none_entry.get_icon_filepath(false)));
    EXPECT_NE(std::string::npos, none_entry.get_icon_filepath(false).find("0.png"));
    EXPECT_TRUE(fs::exists(none_entry.get_icon_filepath(true)));
    EXPECT_NE(std::string::npos, none_entry.get_icon_filepath(true).find("0.png"));

    // TODO: remove after GCN and Gen VI support
    if(none_entry.get_game_id() != COLOSSEUM and
       none_entry.get_game_id() != XD and
       none_entry.get_game_id() < X
    ) {
        EXPECT_TRUE(fs::exists(none_entry.get_sprite_filepath(false,false)));
        EXPECT_NE(std::string::npos, none_entry.get_sprite_filepath(false,false).find("0.png"));
        EXPECT_TRUE(fs::exists(none_entry.get_sprite_filepath(false,true)));
        EXPECT_NE(std::string::npos, none_entry.get_sprite_filepath(false,true).find("0.png"));
        EXPECT_TRUE(fs::exists(none_entry.get_sprite_filepath(true,false)));
        EXPECT_NE(std::string::npos, none_entry.get_sprite_filepath(true,false).find("0.png"));
        EXPECT_TRUE(fs::exists(none_entry.get_sprite_filepath(true,true)));
        EXPECT_NE(std::string::npos, none_entry.get_sprite_filepath(true,true).find("0.png"));
    }
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_entry_none_test,
    pokemon_entry_none_test,
    ::testing::ValuesIn(pkmntest::ALL_GAMES)
);

class pokemon_entry_evolutions_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(pokemon_entry_evolutions_test, pokemon_entry_evolutions_test) {
    pkmn::database::pokemon_entry eevee_entry("Eevee", GetParam(), "");
    int generation = pkmn::priv::game_enum_to_generation(eevee_entry.get_game());

    pkmn::database::pokemon_entries_t evolutions = eevee_entry.get_evolutions();
    EXPECT_GE(evolutions.size(), 3);

    EXPECT_EQ("Vaporeon", evolutions.at(0).get_name());
    EXPECT_EQ("Jolteon", evolutions.at(1).get_name());
    EXPECT_EQ("Flareon", evolutions.at(2).get_name());

    if(generation >= 2) {
        EXPECT_EQ("Espeon", evolutions.at(3).get_name());
        EXPECT_EQ("Umbreon", evolutions.at(4).get_name());
    } else {
        EXPECT_EQ(3, evolutions.size());
        return;
    }

    if(generation >= 4) {
        EXPECT_EQ("Leafeon", evolutions.at(5).get_name());
        EXPECT_EQ("Glaceon", evolutions.at(6).get_name());
    } else {
        EXPECT_EQ(5, evolutions.size());
        return;
    }

    if(generation >= 6) {
        EXPECT_EQ("Sylveon", evolutions.at(7).get_name());
    } else {
        EXPECT_EQ(7, evolutions.size());
    }
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_entry_evolutions_test,
    pokemon_entry_evolutions_test,
    ::testing::ValuesIn(pkmntest::ALL_GAMES)
);

class pokemon_entry_test: public ::testing::Test {
    public:
        static void SetUpTestCase() {
            byindex_gen1 = pkmn::database::pokemon_entry(74,3);
            byindex_gen2 = pkmn::database::pokemon_entry(160,4);
            byindex_gba = pkmn::database::pokemon_entry(402,9);
            byindex_gcn  = pkmn::database::pokemon_entry(306,20);
            byindex_gen4 = pkmn::database::pokemon_entry(401,13);
            byindex_gen5 = pkmn::database::pokemon_entry(618,21);
            byindex_gen6 = pkmn::database::pokemon_entry(700,26);
            byname_gen1 = pkmn::database::pokemon_entry("Articuno", pkmn::e_game::YELLOW, "");
            byname_gen2 = pkmn::database::pokemon_entry("Feraligatr", pkmn::e_game::GOLD, "");
            byname_gba = pkmn::database::pokemon_entry("Regice", pkmn::e_game::EMERALD, "");
            byname_gcn = pkmn::database::pokemon_entry("Shroomish", pkmn::e_game::XD, "");
            byname_gen4 = pkmn::database::pokemon_entry("Kricketot", pkmn::e_game::PEARL, "");
            byname_gen5 = pkmn::database::pokemon_entry("Stunfisk", pkmn::e_game::BLACK2, "");
            byname_gen6 = pkmn::database::pokemon_entry("Sylveon", pkmn::e_game::ALPHA_SAPPHIRE, "");
        }

        static pkmn::database::pokemon_entry byindex_gen1, byname_gen1;
        static pkmn::database::pokemon_entry byindex_gen2, byname_gen2;
        static pkmn::database::pokemon_entry byindex_gba,  byname_gba;
        static pkmn::database::pokemon_entry byindex_gcn,  byname_gcn;
        static pkmn::database::pokemon_entry byindex_gen4, byname_gen4;
        static pkmn::database::pokemon_entry byindex_gen5, byname_gen5;
        static pkmn::database::pokemon_entry byindex_gen6, byname_gen6;
};

pkmn::database::pokemon_entry pokemon_entry_test::byindex_gen1;
pkmn::database::pokemon_entry pokemon_entry_test::byindex_gen2;
pkmn::database::pokemon_entry pokemon_entry_test::byindex_gba;
pkmn::database::pokemon_entry pokemon_entry_test::byindex_gcn;
pkmn::database::pokemon_entry pokemon_entry_test::byindex_gen4;
pkmn::database::pokemon_entry pokemon_entry_test::byindex_gen5;
pkmn::database::pokemon_entry pokemon_entry_test::byindex_gen6;
pkmn::database::pokemon_entry pokemon_entry_test::byname_gen1;
pkmn::database::pokemon_entry pokemon_entry_test::byname_gen2;
pkmn::database::pokemon_entry pokemon_entry_test::byname_gba;
pkmn::database::pokemon_entry pokemon_entry_test::byname_gcn;
pkmn::database::pokemon_entry pokemon_entry_test::byname_gen4;
pkmn::database::pokemon_entry pokemon_entry_test::byname_gen5;
pkmn::database::pokemon_entry pokemon_entry_test::byname_gen6;

/*
 * Make sure Pokémon entries can't be instantiated for games that didn't
 * have the given Pokémon.
 */
TEST_F(pokemon_entry_test, wrong_game_test) {
    // Pokémon from later generations
    EXPECT_THROW(
        pkmn::database::pokemon_entry snubbull(
            "Snubbull", pkmn::e_game::YELLOW, ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry ralts(
            "Ralts", pkmn::e_game::SILVER, ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry budew(
            "Budew", pkmn::e_game::SAPPHIRE, ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry liepard(
            "Liepard", pkmn::e_game::PEARL, ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry espurr(
            "Espurr", pkmn::e_game::WHITE, ""
        );
    , std::invalid_argument);

    /*
     * Make sure forms can't be used in the wrong games, even if
     * the Pokémon existed.
     */
    EXPECT_THROW(
        pkmn::database::pokemon_entry arceus(
            "Arceus", pkmn::e_game::DIAMOND, "Fairy"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus1(
        "Arceus", pkmn::e_game::Y, "Fairy"
    );
    EXPECT_THROW(
        pkmn::database::pokemon_entry arceus(
            "Arceus", pkmn::e_game::BLACK, "???"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus2(
        "Arceus", pkmn::e_game::PEARL, "???"
    );
    EXPECT_THROW(
        pkmn::database::pokemon_entry pichu(
            "Pichu", pkmn::e_game::SILVER, "Spiky-eared"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry pichu(
        "Pichu", pkmn::e_game::HEARTGOLD, "Spiky-eared"
    );
    EXPECT_THROW(
        pkmn::database::pokemon_entry pikachu(
            "Pikachu", pkmn::e_game::RUBY, "Belle"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry pikachu(
        "Pikachu", pkmn::e_game::OMEGA_RUBY, "Belle"
    );
}

/*
 * Make sure Pokémon whose type has changed between generations
 * reflect that in their entries.
 */
TEST_F(pokemon_entry_test, changing_type_test) {
    pkmn::database::pokemon_entry clefairy5(
        "Clefairy", pkmn::e_game::BLACK, ""
    );
    pkmn::database::pokemon_entry clefairy6(
        "Clefairy", pkmn::e_game::Y, ""
    );

    std::pair<std::string, std::string> clefairy_types5 = clefairy5.get_types();
    std::pair<std::string, std::string> clefairy_types6 = clefairy6.get_types();

    EXPECT_EQ("Normal", clefairy_types5.first);
    EXPECT_EQ("None", clefairy_types5.second);

    EXPECT_EQ("Fairy", clefairy_types6.first);
    EXPECT_EQ("None", clefairy_types6.second);

    pkmn::database::pokemon_entry jigglypuff5(
        "Jigglypuff", pkmn::e_game::BLACK, ""
    );
    pkmn::database::pokemon_entry jigglypuff6(
        "Jigglypuff", pkmn::e_game::Y, ""
    );

    std::pair<std::string, std::string> jigglypuff_types5 = jigglypuff5.get_types();
    std::pair<std::string, std::string> jigglypuff_types6 = jigglypuff6.get_types();

    EXPECT_EQ("Normal", jigglypuff_types5.first);
    EXPECT_EQ("None", jigglypuff_types5.second);

    EXPECT_EQ("Normal", jigglypuff_types6.first);
    EXPECT_EQ("Fairy", jigglypuff_types6.second);
}

TEST_F(pokemon_entry_test, equality_test) {
    EXPECT_TRUE(byindex_gen1 == byname_gen1);
    EXPECT_TRUE(byindex_gen2 == byname_gen2);
    EXPECT_TRUE(byindex_gba  == byname_gba);
    EXPECT_TRUE(byindex_gcn  == byname_gcn);
    EXPECT_TRUE(byindex_gen4 == byname_gen4);
    EXPECT_TRUE(byindex_gen5 == byname_gen5);
    EXPECT_TRUE(byindex_gen6 == byname_gen6);

    EXPECT_TRUE(byindex_gen1 != byname_gen2);
    EXPECT_TRUE(byindex_gen2 != byname_gba);
    EXPECT_TRUE(byindex_gba  != byname_gcn);
    EXPECT_TRUE(byindex_gcn  != byname_gen4);
    EXPECT_TRUE(byindex_gen4 != byname_gen5);
    EXPECT_TRUE(byindex_gen5 != byname_gen6);
}

static void _pokemon_entry_test(
    pkmn::database::pokemon_entry &pokemon_entry_gen1,
    pkmn::database::pokemon_entry &pokemon_entry_gen2,
    pkmn::database::pokemon_entry &pokemon_entry_gba,
    pkmn::database::pokemon_entry &pokemon_entry_gcn,
    pkmn::database::pokemon_entry &pokemon_entry_gen4,
    pkmn::database::pokemon_entry &pokemon_entry_gen5,
    pkmn::database::pokemon_entry &pokemon_entry_gen6
) {
    /*
     * Generation I entry
     */
    EXPECT_EQ("Articuno", pokemon_entry_gen1.get_name());
    EXPECT_EQ(pkmn::e_game::YELLOW, pokemon_entry_gen1.get_game());
    EXPECT_EQ("Freeze", pokemon_entry_gen1.get_species());
    EXPECT_GT(pokemon_entry_gen1.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen1.get_form());
    EXPECT_FLOAT_EQ(1.7f, pokemon_entry_gen1.get_height());
    EXPECT_FLOAT_EQ(55.4f, pokemon_entry_gen1.get_weight());
    EXPECT_FLOAT_EQ(0.0f, pokemon_entry_gen1.get_chance_male());
    EXPECT_FLOAT_EQ(0.0f, pokemon_entry_gen1.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gen1.has_gender_differences());
    EXPECT_EQ(-1, pokemon_entry_gen1.get_base_friendship());

    std::pair<std::string, std::string> types1 = pokemon_entry_gen1.get_types();
    EXPECT_EQ("Ice", types1.first);
    EXPECT_EQ("Flying", types1.second);

    EXPECT_TRUE(pokemon_entry_gen1.get_abilities() == none_pair);
    EXPECT_EQ("None", pokemon_entry_gen1.get_hidden_ability());
    EXPECT_TRUE(pokemon_entry_gen1.get_egg_groups() == none_pair);

    std::map<pkmn::e_stat, int> base_stats1 = pokemon_entry_gen1.get_base_stats();
    EXPECT_EQ(90, base_stats1.at(pkmn::e_stat::HP));
    EXPECT_EQ(85, base_stats1.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(100, base_stats1.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(85, base_stats1.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(125, base_stats1.at(pkmn::e_stat::SPECIAL));

    EXPECT_TRUE(pokemon_entry_gen1.get_EV_yields() == base_stats1);
    EXPECT_EQ(215, pokemon_entry_gen1.get_experience_yield());
    EXPECT_EQ(156250, pokemon_entry_gen1.get_experience_at_level(50));
    EXPECT_EQ(54, pokemon_entry_gen1.get_level_at_experience(200000));
    EXPECT_GT(pokemon_entry_gen1.get_levelup_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gen1.get_tm_hm_moves().size(), 0);
    EXPECT_EQ(0, pokemon_entry_gen1.get_egg_moves().size());
    EXPECT_EQ(0, pokemon_entry_gen1.get_tutor_moves().size());
    EXPECT_EQ(1, pokemon_entry_gen1.get_forms().size());
    EXPECT_EQ(0, pokemon_entry_gen1.get_evolutions().size());
    EXPECT_EQ(144, pokemon_entry_gen1.get_species_id());
    EXPECT_EQ(144, pokemon_entry_gen1.get_pokemon_id());
    EXPECT_EQ(144, pokemon_entry_gen1.get_form_id());
    EXPECT_EQ(74, pokemon_entry_gen1.get_pokemon_index());
    EXPECT_EQ(3, pokemon_entry_gen1.get_game_id());

    EXPECT_TRUE(fs::exists(pokemon_entry_gen1.get_icon_filepath(false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen1.get_icon_filepath(true)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen1.get_sprite_filepath(false,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen1.get_sprite_filepath(true,false)));

    // No shininess in Generation I
    EXPECT_THROW(
        (void)pokemon_entry_gen1.get_sprite_filepath(false,true);
    , pkmn::feature_not_in_game_error);
    EXPECT_THROW(
        (void)pokemon_entry_gen1.get_sprite_filepath(true,true);
    , pkmn::feature_not_in_game_error);

    /*
     * Generation II entry
     */
    EXPECT_EQ("Feraligatr", pokemon_entry_gen2.get_name());
    EXPECT_EQ(pkmn::e_game::GOLD, pokemon_entry_gen2.get_game());
    EXPECT_EQ("Big Jaw", pokemon_entry_gen2.get_species());
    EXPECT_GT(pokemon_entry_gen2.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen2.get_form());
    EXPECT_FLOAT_EQ(2.3f, pokemon_entry_gen2.get_height());
    EXPECT_FLOAT_EQ(88.8f, pokemon_entry_gen2.get_weight());
    EXPECT_FLOAT_EQ(0.875f, pokemon_entry_gen2.get_chance_male());
    EXPECT_FLOAT_EQ(0.125f, pokemon_entry_gen2.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gen2.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gen2.get_base_friendship());

    std::pair<std::string, std::string> types2 = pokemon_entry_gen2.get_types();
    EXPECT_EQ("Water", types2.first);
    EXPECT_EQ("None", types2.second);

    EXPECT_TRUE(pokemon_entry_gen2.get_abilities() == none_pair);
    EXPECT_EQ("None", pokemon_entry_gen2.get_hidden_ability());

    std::pair<std::string, std::string> egg_groups2 = pokemon_entry_gen2.get_egg_groups();
    EXPECT_EQ("Monster", egg_groups2.first);
    EXPECT_EQ("Water 1", egg_groups2.second);

    std::map<pkmn::e_stat, int> base_stats2 = pokemon_entry_gen2.get_base_stats();
    EXPECT_EQ(85, base_stats2.at(pkmn::e_stat::HP));
    EXPECT_EQ(105, base_stats2.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(100, base_stats2.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(78, base_stats2.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(79, base_stats2.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(83, base_stats2.at(pkmn::e_stat::SPECIAL_DEFENSE));

    std::map<pkmn::e_stat, int> EV_yields2 = pokemon_entry_gen2.get_EV_yields();
    EXPECT_EQ(85, EV_yields2.at(pkmn::e_stat::HP));
    EXPECT_EQ(105, EV_yields2.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(100, EV_yields2.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(78, EV_yields2.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(79, EV_yields2.at(pkmn::e_stat::SPECIAL));

    EXPECT_EQ(210, pokemon_entry_gen2.get_experience_yield());
    EXPECT_EQ(117360, pokemon_entry_gen2.get_experience_at_level(50));
    EXPECT_EQ(59, pokemon_entry_gen2.get_level_at_experience(200000));
    EXPECT_GT(pokemon_entry_gen2.get_levelup_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gen2.get_tm_hm_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gen2.get_egg_moves().size(), 0);
    EXPECT_EQ(0, pokemon_entry_gen2.get_tutor_moves().size());
    EXPECT_EQ(1, pokemon_entry_gen2.get_forms().size());
    EXPECT_EQ(0, pokemon_entry_gen2.get_evolutions().size());
    EXPECT_EQ(160, pokemon_entry_gen2.get_species_id());
    EXPECT_EQ(160, pokemon_entry_gen2.get_pokemon_id());
    EXPECT_EQ(160, pokemon_entry_gen2.get_form_id());
    EXPECT_EQ(160, pokemon_entry_gen2.get_pokemon_index());

    EXPECT_TRUE(fs::exists(pokemon_entry_gen2.get_icon_filepath(false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen2.get_icon_filepath(true)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen2.get_sprite_filepath(false,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen2.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen2.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen2.get_sprite_filepath(true,true)));

    /*
     * Game Boy Advance entry
     */
    EXPECT_EQ("Regice", pokemon_entry_gba.get_name());
    EXPECT_EQ(pkmn::e_game::EMERALD, pokemon_entry_gba.get_game());
    EXPECT_EQ("Iceberg", pokemon_entry_gba.get_species());
    EXPECT_GT(pokemon_entry_gba.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gba.get_form());
    EXPECT_FLOAT_EQ(1.8f, pokemon_entry_gba.get_height());
    EXPECT_FLOAT_EQ(175.0f, pokemon_entry_gba.get_weight());
    EXPECT_FLOAT_EQ(0.0f, pokemon_entry_gba.get_chance_male());
    EXPECT_FLOAT_EQ(0.0f, pokemon_entry_gba.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gba.has_gender_differences());
    EXPECT_EQ(35, pokemon_entry_gba.get_base_friendship());

    std::pair<std::string, std::string> types_gba = pokemon_entry_gba.get_types();
    EXPECT_EQ("Ice", types_gba.first);
    EXPECT_EQ("None", types_gba.second);

    std::pair<std::string, std::string> abilities_gba = pokemon_entry_gba.get_abilities();
    EXPECT_EQ("Clear Body", abilities_gba.first);
    EXPECT_EQ("None", abilities_gba.second);

    EXPECT_EQ("None", pokemon_entry_gba.get_hidden_ability());

    std::pair<std::string, std::string> egg_groups_gba = pokemon_entry_gba.get_egg_groups();
    EXPECT_EQ("Undiscovered", egg_groups_gba.first);
    EXPECT_EQ("None", egg_groups_gba.second);

    std::map<pkmn::e_stat, int> base_stats_gba = pokemon_entry_gba.get_base_stats();
    EXPECT_EQ(80, base_stats_gba.at(pkmn::e_stat::HP));
    EXPECT_EQ(50, base_stats_gba.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(100, base_stats_gba.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(50, base_stats_gba.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(100, base_stats_gba.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(200, base_stats_gba.at(pkmn::e_stat::SPECIAL_DEFENSE));

    std::map<pkmn::e_stat, int> EV_yields_gba = pokemon_entry_gba.get_EV_yields();
    EXPECT_EQ(0, EV_yields_gba.at(pkmn::e_stat::HP));
    EXPECT_EQ(0, EV_yields_gba.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(0, EV_yields_gba.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(0, EV_yields_gba.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(0, EV_yields_gba.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(3, EV_yields_gba.at(pkmn::e_stat::SPECIAL_DEFENSE));

    EXPECT_EQ(216, pokemon_entry_gba.get_experience_yield());
    EXPECT_EQ(156250, pokemon_entry_gba.get_experience_at_level(50));
    EXPECT_EQ(54, pokemon_entry_gba.get_level_at_experience(200000));
    EXPECT_GT(pokemon_entry_gba.get_levelup_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gba.get_tm_hm_moves().size(), 0);
    EXPECT_EQ(0, pokemon_entry_gba.get_egg_moves().size());
    EXPECT_GT(pokemon_entry_gba.get_tutor_moves().size(), 0);
    EXPECT_EQ(1, pokemon_entry_gba.get_forms().size());
    EXPECT_EQ(0, pokemon_entry_gba.get_evolutions().size());
    EXPECT_EQ(378, pokemon_entry_gba.get_species_id());
    EXPECT_EQ(378, pokemon_entry_gba.get_pokemon_id());
    EXPECT_EQ(378, pokemon_entry_gba.get_form_id());
    EXPECT_EQ(402, pokemon_entry_gba.get_pokemon_index());

    EXPECT_TRUE(fs::exists(pokemon_entry_gba.get_icon_filepath(false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gba.get_icon_filepath(true)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gba.get_sprite_filepath(false,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gba.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gba.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gba.get_sprite_filepath(true,true)));

    /*
     * Gamecube entry
     */
    EXPECT_EQ("Shroomish", pokemon_entry_gcn.get_name());
    EXPECT_EQ(pkmn::e_game::XD, pokemon_entry_gcn.get_game());
    EXPECT_EQ("Mushroom", pokemon_entry_gcn.get_species());
    EXPECT_GT(pokemon_entry_gcn.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gcn.get_form());
    EXPECT_FLOAT_EQ(0.4f, pokemon_entry_gcn.get_height());
    EXPECT_FLOAT_EQ(4.5f, pokemon_entry_gcn.get_weight());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gcn.get_chance_male());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gcn.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gcn.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gcn.get_base_friendship());

    std::pair<std::string, std::string> types_gcn = pokemon_entry_gcn.get_types();
    EXPECT_EQ("Grass", types_gcn.first);
    EXPECT_EQ("None", types_gcn.second);

    std::pair<std::string, std::string> abilities_gcn = pokemon_entry_gcn.get_abilities();
    EXPECT_EQ("Effect Spore", abilities_gcn.first);
    EXPECT_EQ("Poison Heal", abilities_gcn.second);

    EXPECT_EQ("None", pokemon_entry_gcn.get_hidden_ability());

    std::pair<std::string, std::string> egg_groups_gcn = pokemon_entry_gcn.get_egg_groups();
    EXPECT_EQ("Fairy", egg_groups_gcn.first);
    EXPECT_EQ("Grass", egg_groups_gcn.second);

    std::map<pkmn::e_stat, int> base_stats_gcn = pokemon_entry_gcn.get_base_stats();
    EXPECT_EQ(60, base_stats_gcn.at(pkmn::e_stat::HP));
    EXPECT_EQ(40, base_stats_gcn.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(60, base_stats_gcn.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(35, base_stats_gcn.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(40, base_stats_gcn.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(60, base_stats_gcn.at(pkmn::e_stat::SPECIAL_DEFENSE));

    std::map<pkmn::e_stat, int> EV_yields_gcn = pokemon_entry_gcn.get_EV_yields();
    EXPECT_EQ(1, EV_yields_gcn.at(pkmn::e_stat::HP));
    EXPECT_EQ(0, EV_yields_gcn.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(0, EV_yields_gcn.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(0, EV_yields_gcn.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(0, EV_yields_gcn.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(0, EV_yields_gcn.at(pkmn::e_stat::SPECIAL_DEFENSE));

    EXPECT_EQ(65, pokemon_entry_gcn.get_experience_yield());
    EXPECT_EQ(142500, pokemon_entry_gcn.get_experience_at_level(50));
    EXPECT_EQ(55, pokemon_entry_gcn.get_level_at_experience(200000));
    EXPECT_GT(pokemon_entry_gcn.get_levelup_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gcn.get_tm_hm_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gcn.get_egg_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gcn.get_tutor_moves().size(), 0);
    EXPECT_EQ(2, pokemon_entry_gcn.get_forms().size());
    EXPECT_EQ(1, pokemon_entry_gcn.get_evolutions().size());
    EXPECT_EQ(285, pokemon_entry_gcn.get_species_id());
    EXPECT_EQ(285, pokemon_entry_gcn.get_pokemon_id());
    EXPECT_EQ(285, pokemon_entry_gcn.get_form_id());
    EXPECT_EQ(306, pokemon_entry_gcn.get_pokemon_index());

    // TODO: sprites+icons

    /*
     * Generation IV entry
     */
    EXPECT_EQ("Kricketot", pokemon_entry_gen4.get_name());
    EXPECT_EQ(pkmn::e_game::PEARL, pokemon_entry_gen4.get_game());
    EXPECT_EQ("Cricket", pokemon_entry_gen4.get_species());
    EXPECT_GT(pokemon_entry_gen4.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen4.get_form());
    EXPECT_FLOAT_EQ(0.3f, pokemon_entry_gen4.get_height());
    EXPECT_FLOAT_EQ(2.2f, pokemon_entry_gen4.get_weight());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gen4.get_chance_male());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gen4.get_chance_female());
    EXPECT_TRUE(pokemon_entry_gen4.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gen4.get_base_friendship());

    std::pair<std::string, std::string> types_gen4 = pokemon_entry_gen4.get_types();
    EXPECT_EQ("Bug", types_gen4.first);
    EXPECT_EQ("None", types_gen4.second);

    std::pair<std::string, std::string> abilities_gen4 = pokemon_entry_gen4.get_abilities();
    EXPECT_EQ("Shed Skin", abilities_gen4.first);
    EXPECT_EQ("None", abilities_gen4.second);

    EXPECT_EQ("None", pokemon_entry_gen4.get_hidden_ability());

    std::pair<std::string, std::string> egg_groups_gen4 = pokemon_entry_gen4.get_egg_groups();
    EXPECT_EQ("Bug", egg_groups_gen4.first);
    EXPECT_EQ("None", egg_groups_gen4.second);

    std::map<pkmn::e_stat, int> base_stats_gen4 = pokemon_entry_gen4.get_base_stats();
    EXPECT_EQ(37, base_stats_gen4.at(pkmn::e_stat::HP));
    EXPECT_EQ(25, base_stats_gen4.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(41, base_stats_gen4.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(25, base_stats_gen4.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(25, base_stats_gen4.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(41, base_stats_gen4.at(pkmn::e_stat::SPECIAL_DEFENSE));

    std::map<pkmn::e_stat, int> EV_yields_gen4 = pokemon_entry_gen4.get_EV_yields();
    EXPECT_EQ(0, EV_yields_gen4.at(pkmn::e_stat::HP));
    EXPECT_EQ(0, EV_yields_gen4.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(1, EV_yields_gen4.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(0, EV_yields_gen4.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(0, EV_yields_gen4.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(0, EV_yields_gen4.at(pkmn::e_stat::SPECIAL_DEFENSE));

    EXPECT_EQ(54, pokemon_entry_gen4.get_experience_yield());
    EXPECT_EQ(117360, pokemon_entry_gen4.get_experience_at_level(50));
    EXPECT_EQ(59, pokemon_entry_gen4.get_level_at_experience(200000));
    EXPECT_GT(pokemon_entry_gen4.get_levelup_moves().size(), 0);
    EXPECT_EQ(0, pokemon_entry_gen4.get_tm_hm_moves().size());
    EXPECT_EQ(0, pokemon_entry_gen4.get_egg_moves().size());
    EXPECT_EQ(0, pokemon_entry_gen4.get_tutor_moves().size());
    EXPECT_EQ(1, pokemon_entry_gen4.get_forms().size());
    EXPECT_EQ(1, pokemon_entry_gen4.get_evolutions().size());
    EXPECT_EQ(401, pokemon_entry_gen4.get_species_id());
    EXPECT_EQ(401, pokemon_entry_gen4.get_pokemon_id());
    EXPECT_EQ(401, pokemon_entry_gen4.get_form_id());
    EXPECT_EQ(401, pokemon_entry_gen4.get_pokemon_index());

    EXPECT_TRUE(fs::exists(pokemon_entry_gen4.get_icon_filepath(false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen4.get_icon_filepath(true)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen4.get_sprite_filepath(false,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen4.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen4.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen4.get_sprite_filepath(true,true)));

    /*
     * Generation V entry
     */
    EXPECT_EQ("Stunfisk", pokemon_entry_gen5.get_name());
    EXPECT_EQ(pkmn::e_game::BLACK2, pokemon_entry_gen5.get_game());
    EXPECT_EQ("Trap", pokemon_entry_gen5.get_species());
    EXPECT_GT(pokemon_entry_gen5.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen5.get_form());
    EXPECT_FLOAT_EQ(0.7f, pokemon_entry_gen5.get_height());
    EXPECT_FLOAT_EQ(11.0f, pokemon_entry_gen5.get_weight());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gen5.get_chance_male());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gen5.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gen5.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gen5.get_base_friendship());

    std::pair<std::string, std::string> types_gen5 = pokemon_entry_gen5.get_types();
    EXPECT_EQ("Ground", types_gen5.first);
    EXPECT_EQ("Electric", types_gen5.second);

    std::pair<std::string, std::string> abilities_gen5 = pokemon_entry_gen5.get_abilities();
    EXPECT_EQ("Static", abilities_gen5.first);
    EXPECT_EQ("Limber", abilities_gen5.second);

    EXPECT_EQ("Sand Veil", pokemon_entry_gen5.get_hidden_ability());

    std::pair<std::string, std::string> egg_groups_gen5 = pokemon_entry_gen5.get_egg_groups();
    EXPECT_EQ("Water 1", egg_groups_gen5.first);
    EXPECT_EQ("Amorphous", egg_groups_gen5.second);

    std::map<pkmn::e_stat, int> base_stats_gen5 = pokemon_entry_gen5.get_base_stats();
    EXPECT_EQ(109, base_stats_gen5.at(pkmn::e_stat::HP));
    EXPECT_EQ(66, base_stats_gen5.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(84, base_stats_gen5.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(32, base_stats_gen5.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(81, base_stats_gen5.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(99, base_stats_gen5.at(pkmn::e_stat::SPECIAL_DEFENSE));

    std::map<pkmn::e_stat, int> EV_yields_gen5 = pokemon_entry_gen5.get_EV_yields();
    EXPECT_EQ(2, EV_yields_gen5.at(pkmn::e_stat::HP));
    EXPECT_EQ(0, EV_yields_gen5.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(0, EV_yields_gen5.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(0, EV_yields_gen5.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(0, EV_yields_gen5.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(0, EV_yields_gen5.at(pkmn::e_stat::SPECIAL_DEFENSE));

    EXPECT_EQ(165, pokemon_entry_gen5.get_experience_yield());
    EXPECT_EQ(125000, pokemon_entry_gen5.get_experience_at_level(50));
    EXPECT_EQ(58, pokemon_entry_gen5.get_level_at_experience(200000));
    EXPECT_GT(pokemon_entry_gen5.get_levelup_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gen5.get_tm_hm_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gen5.get_egg_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gen5.get_tutor_moves().size(), 0);
    EXPECT_EQ(1, pokemon_entry_gen5.get_forms().size());
    EXPECT_EQ(0, pokemon_entry_gen5.get_evolutions().size());
    EXPECT_EQ(618, pokemon_entry_gen5.get_species_id());
    EXPECT_EQ(618, pokemon_entry_gen5.get_pokemon_id());
    EXPECT_EQ(618, pokemon_entry_gen5.get_form_id());
    EXPECT_EQ(618, pokemon_entry_gen5.get_pokemon_index());

    EXPECT_TRUE(fs::exists(pokemon_entry_gen5.get_icon_filepath(false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen5.get_icon_filepath(true)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen5.get_sprite_filepath(false,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen5.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen5.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gen5.get_sprite_filepath(true,true)));

    /*
     * Generation VI entry
     */
    EXPECT_EQ("Sylveon", pokemon_entry_gen6.get_name());
    EXPECT_EQ(pkmn::e_game::ALPHA_SAPPHIRE, pokemon_entry_gen6.get_game());
    EXPECT_EQ("Intertwining", pokemon_entry_gen6.get_species());
    EXPECT_GT(pokemon_entry_gen6.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen6.get_form());
    EXPECT_FLOAT_EQ(1.0f, pokemon_entry_gen6.get_height());
    EXPECT_FLOAT_EQ(23.5f, pokemon_entry_gen6.get_weight());
    EXPECT_FLOAT_EQ(0.875f, pokemon_entry_gen6.get_chance_male());
    EXPECT_FLOAT_EQ(0.125f, pokemon_entry_gen6.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gen6.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gen6.get_base_friendship());

    std::pair<std::string, std::string> types_gen6 = pokemon_entry_gen6.get_types();
    EXPECT_EQ("Fairy", types_gen6.first);
    EXPECT_EQ("None", types_gen6.second);

    std::pair<std::string, std::string> abilities_gen6 = pokemon_entry_gen6.get_abilities();
    EXPECT_EQ("Cute Charm", abilities_gen6.first);
    EXPECT_EQ("None", abilities_gen6.second);

    EXPECT_EQ("Pixilate", pokemon_entry_gen6.get_hidden_ability());

    std::pair<std::string, std::string> egg_groups_gen6 = pokemon_entry_gen6.get_egg_groups();
    EXPECT_EQ("Field", egg_groups_gen6.first);
    EXPECT_EQ("None", egg_groups_gen6.second);

    std::map<pkmn::e_stat, int> base_stats_gen6 = pokemon_entry_gen6.get_base_stats();
    EXPECT_EQ(95, base_stats_gen6.at(pkmn::e_stat::HP));
    EXPECT_EQ(65, base_stats_gen6.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(65, base_stats_gen6.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(60, base_stats_gen6.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(110, base_stats_gen6.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(130, base_stats_gen6.at(pkmn::e_stat::SPECIAL_DEFENSE));

    std::map<pkmn::e_stat, int> EV_yields_gen6 = pokemon_entry_gen6.get_EV_yields();
    EXPECT_EQ(0, EV_yields_gen6.at(pkmn::e_stat::HP));
    EXPECT_EQ(0, EV_yields_gen6.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(0, EV_yields_gen6.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(0, EV_yields_gen6.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(0, EV_yields_gen6.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(2, EV_yields_gen6.at(pkmn::e_stat::SPECIAL_DEFENSE));

    EXPECT_EQ(184, pokemon_entry_gen6.get_experience_yield());
    EXPECT_EQ(125000, pokemon_entry_gen6.get_experience_at_level(50));
    EXPECT_EQ(58, pokemon_entry_gen6.get_level_at_experience(200000));
    EXPECT_GT(pokemon_entry_gen6.get_levelup_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gen6.get_tm_hm_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gen6.get_egg_moves().size(), 0);
    EXPECT_GT(pokemon_entry_gen6.get_tutor_moves().size(), 0);
    EXPECT_EQ(1, pokemon_entry_gen6.get_forms().size());
    EXPECT_EQ(0, pokemon_entry_gen6.get_evolutions().size());
    EXPECT_EQ(700, pokemon_entry_gen6.get_species_id());
    EXPECT_EQ(700, pokemon_entry_gen6.get_pokemon_id());
    EXPECT_EQ(700, pokemon_entry_gen6.get_form_id());
    EXPECT_EQ(700, pokemon_entry_gen6.get_pokemon_index());

    // TODO: sprites+icons
}

TEST_F(pokemon_entry_test, by_index_test) {
    _pokemon_entry_test(
        byindex_gen1, byindex_gen2, byindex_gba,
        byindex_gcn, byindex_gen4, byindex_gen5,
        byindex_gen6
    );
}

TEST_F(pokemon_entry_test, by_name_test) {
    _pokemon_entry_test(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );
}
