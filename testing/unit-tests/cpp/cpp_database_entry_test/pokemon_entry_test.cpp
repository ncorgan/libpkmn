/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/exception.hpp>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <iostream>

namespace fs = boost::filesystem;

static const std::pair<std::string, std::string> none_pair("None", "None");
static const std::pair<std::string, std::string> invalid_pair("Unknown", "Unknown");
static const std::map<std::string, int> bad_stat_map_old = boost::assign::map_list_of
    ("HP", 0)("Attack", 0)("Defense", 0)
    ("Speed", 0)("Special", 0)
;
static const std::map<std::string, int> bad_stat_map = boost::assign::map_list_of
    ("HP", 0)("Attack", 0)("Defense", 0)
    ("Speed", 0)("Special Attack", 0)("Special Defense", 0)
;

static const std::string games[] = {
    "Red",
    "Blue",
    "Yellow",
    "Gold",
    "Silver",
    "Crystal",
    "Ruby",
    "Sapphire",
    "Emerald",
    "FireRed",
    "LeafGreen",
    "Colosseum",
    "XD",
    "Diamond",
    "Pearl",
    "HeartGold",
    "SoulSilver",
    "Black",
    "White",
    "Black 2",
    "White 2",
    "X",
    "Y",
    "Omega Ruby",
    "Alpha Sapphire"
};

BOOST_STATIC_CONSTEXPR int GENERATIONS[] = {
    0,1,1,1,2,2,2,3,3,3,3,3,4,4,4,4,4,5,5,3,3,5,5,6,6,6,6
};

BOOST_STATIC_CONSTEXPR int YELLOW = 3;
BOOST_STATIC_CONSTEXPR int CRYSTAL = 6;
BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;
BOOST_STATIC_CONSTEXPR int XD = 20;
BOOST_STATIC_CONSTEXPR int X = 23;

class pokemon_entry_none_test: public ::testing::TestWithParam<std::string> {};

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
    ::testing::ValuesIn(games)
);

class pokemon_entry_evolutions_test: public ::testing::TestWithParam<std::string> {};

TEST_P(pokemon_entry_evolutions_test, pokemon_entry_evolutions_test) {
    pkmn::database::pokemon_entry eevee_entry("Eevee", GetParam(), "");
    int generation = GENERATIONS[eevee_entry.get_game_id()];

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
    ::testing::ValuesIn(games)
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
            byname_gen1 = pkmn::database::pokemon_entry("Articuno", "Yellow", "");
            byname_gen2 = pkmn::database::pokemon_entry("Feraligatr", "Gold", "");
            byname_gba = pkmn::database::pokemon_entry("Regice", "Emerald", "");
            byname_gcn = pkmn::database::pokemon_entry("Shroomish", "XD", "");
            byname_gen4 = pkmn::database::pokemon_entry("Kricketot", "Pearl", "");
            byname_gen5 = pkmn::database::pokemon_entry("Stunfisk", "Black 2", "");
            byname_gen6 = pkmn::database::pokemon_entry("Sylveon", "Alpha Sapphire", "");
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
            "Snubbull", "Yellow", ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry ralts(
            "Ralts", "Silver", ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry budew(
            "Budew", "Sapphire", ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry liepard(
            "Liepard", "Pearl", ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry espurr(
            "Espurr", "White", ""
        );
    , std::invalid_argument);

    /*
     * Make sure forms can't be used in the wrong games, even if
     * the Pokémon existed.
     */
    EXPECT_THROW(
        pkmn::database::pokemon_entry arceus(
            "Arceus", "Diamond", "Fairy"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus1(
        "Arceus", "Y", "Fairy"
    );
    EXPECT_THROW(
        pkmn::database::pokemon_entry arceus(
            "Arceus", "Black", "???"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus2(
        "Arceus", "Pearl", "???"
    );
    EXPECT_THROW(
        pkmn::database::pokemon_entry pichu(
            "Pichu", "Silver", "Spiky-eared"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry pichu(
        "Pichu", "HeartGold", "Spiky-eared"
    );
    EXPECT_THROW(
        pkmn::database::pokemon_entry pikachu(
            "Pikachu", "Ruby", "Belle"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry pikachu(
        "Pikachu", "Omega Ruby", "Belle"
    );
}

/*
 * Make sure Pokémon whose type has changed between generations
 * reflect that in their entries.
 */
TEST_F(pokemon_entry_test, changing_type_test) {
    pkmn::database::pokemon_entry clefairy5(
        "Clefairy", "Black", ""
    );
    pkmn::database::pokemon_entry clefairy6(
        "Clefairy", "Y", ""
    );

    std::pair<std::string, std::string> clefairy_types5 = clefairy5.get_types();
    std::pair<std::string, std::string> clefairy_types6 = clefairy6.get_types();

    EXPECT_EQ("Normal", clefairy_types5.first);
    EXPECT_EQ("None", clefairy_types5.second);

    EXPECT_EQ("Fairy", clefairy_types6.first);
    EXPECT_EQ("None", clefairy_types6.second);

    pkmn::database::pokemon_entry jigglypuff5(
        "Jigglypuff", "Black", ""
    );
    pkmn::database::pokemon_entry jigglypuff6(
        "Jigglypuff", "Y", ""
    );

    std::pair<std::string, std::string> jigglypuff_types5 = jigglypuff5.get_types();
    std::pair<std::string, std::string> jigglypuff_types6 = jigglypuff6.get_types();

    EXPECT_EQ("Normal", jigglypuff_types5.first);
    EXPECT_EQ("None", jigglypuff_types5.second);

    EXPECT_EQ("Normal", jigglypuff_types6.first);
    EXPECT_EQ("Fairy", jigglypuff_types6.second);
}

/*
 * Make sure that if a second ability was added in a later generation,
 * that it does not appear in a too-early generation.
 */
TEST_F(pokemon_entry_test, changing_abilities_test)
{
    struct changing_abilities_test_params
    {
        std::string species;
        std::string first_game;
        std::string second_game;

        std::string first_ability;
        std::string second_ability;
    };

    static const std::vector<changing_abilities_test_params> all_test_params =
    {
        {"Pidgey", "Ruby", "Diamond", "Keen Eye", "Tangled Feet"},
        {"Shroomish", "Ruby", "Diamond", "Effect Spore", "Poison Heal"}
    };
    for(const changing_abilities_test_params& test_params: all_test_params)
    {
        pkmn::database::pokemon_entry first_game_entry(
            test_params.species,
            test_params.first_game,
            ""
        );
        pkmn::database::pokemon_entry second_game_entry(
            test_params.species,
            test_params.second_game,
            ""
        );

        std::pair<std::string, std::string> first_game_abilities = first_game_entry.get_abilities();
        EXPECT_EQ(
            test_params.first_ability,
            first_game_abilities.first
        ) << test_params.species << " " << test_params.first_game;
        EXPECT_EQ(
            "None",
            first_game_abilities.second
        ) << test_params.species << " " << test_params.first_game;

        std::pair<std::string, std::string> second_game_abilities = second_game_entry.get_abilities();
        EXPECT_EQ(
            test_params.first_ability,
            second_game_abilities.first
        ) << test_params.species << " " << test_params.second_game;
        EXPECT_EQ(
            test_params.second_ability,
            second_game_abilities.second
        ) << test_params.species << " " << test_params.second_game;
    }
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
    EXPECT_EQ("Yellow", pokemon_entry_gen1.get_game());
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

    std::map<std::string, int> base_stats1 = pokemon_entry_gen1.get_base_stats();
    EXPECT_EQ(90, base_stats1.at("HP"));
    EXPECT_EQ(85, base_stats1.at("Attack"));
    EXPECT_EQ(100, base_stats1.at("Defense"));
    EXPECT_EQ(85, base_stats1.at("Speed"));
    EXPECT_EQ(125, base_stats1.at("Special"));

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
    EXPECT_EQ("Gold", pokemon_entry_gen2.get_game());
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

    std::map<std::string, int> base_stats2 = pokemon_entry_gen2.get_base_stats();
    EXPECT_EQ(85, base_stats2.at("HP"));
    EXPECT_EQ(105, base_stats2.at("Attack"));
    EXPECT_EQ(100, base_stats2.at("Defense"));
    EXPECT_EQ(78, base_stats2.at("Speed"));
    EXPECT_EQ(79, base_stats2.at("Special Attack"));
    EXPECT_EQ(83, base_stats2.at("Special Defense"));

    std::map<std::string, int> EV_yields2 = pokemon_entry_gen2.get_EV_yields();
    EXPECT_EQ(85, EV_yields2.at("HP"));
    EXPECT_EQ(105, EV_yields2.at("Attack"));
    EXPECT_EQ(100, EV_yields2.at("Defense"));
    EXPECT_EQ(78, EV_yields2.at("Speed"));
    EXPECT_EQ(79, EV_yields2.at("Special"));

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
    EXPECT_EQ("Emerald", pokemon_entry_gba.get_game());
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

    std::map<std::string, int> base_stats_gba = pokemon_entry_gba.get_base_stats();
    EXPECT_EQ(80, base_stats_gba.at("HP"));
    EXPECT_EQ(50, base_stats_gba.at("Attack"));
    EXPECT_EQ(100, base_stats_gba.at("Defense"));
    EXPECT_EQ(50, base_stats_gba.at("Speed"));
    EXPECT_EQ(100, base_stats_gba.at("Special Attack"));
    EXPECT_EQ(200, base_stats_gba.at("Special Defense"));

    std::map<std::string, int> EV_yields_gba = pokemon_entry_gba.get_EV_yields();
    EXPECT_EQ(0, EV_yields_gba.at("HP"));
    EXPECT_EQ(0, EV_yields_gba.at("Attack"));
    EXPECT_EQ(0, EV_yields_gba.at("Defense"));
    EXPECT_EQ(0, EV_yields_gba.at("Speed"));
    EXPECT_EQ(0, EV_yields_gba.at("Special Attack"));
    EXPECT_EQ(3, EV_yields_gba.at("Special Defense"));

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
    EXPECT_EQ("XD", pokemon_entry_gcn.get_game());
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
    EXPECT_EQ("None", abilities_gcn.second);

    EXPECT_EQ("None", pokemon_entry_gcn.get_hidden_ability());

    std::pair<std::string, std::string> egg_groups_gcn = pokemon_entry_gcn.get_egg_groups();
    EXPECT_EQ("Fairy", egg_groups_gcn.first);
    EXPECT_EQ("Grass", egg_groups_gcn.second);

    std::map<std::string, int> base_stats_gcn = pokemon_entry_gcn.get_base_stats();
    EXPECT_EQ(60, base_stats_gcn.at("HP"));
    EXPECT_EQ(40, base_stats_gcn.at("Attack"));
    EXPECT_EQ(60, base_stats_gcn.at("Defense"));
    EXPECT_EQ(35, base_stats_gcn.at("Speed"));
    EXPECT_EQ(40, base_stats_gcn.at("Special Attack"));
    EXPECT_EQ(60, base_stats_gcn.at("Special Defense"));

    std::map<std::string, int> EV_yields_gcn = pokemon_entry_gcn.get_EV_yields();
    EXPECT_EQ(1, EV_yields_gcn.at("HP"));
    EXPECT_EQ(0, EV_yields_gcn.at("Attack"));
    EXPECT_EQ(0, EV_yields_gcn.at("Defense"));
    EXPECT_EQ(0, EV_yields_gcn.at("Speed"));
    EXPECT_EQ(0, EV_yields_gcn.at("Special Attack"));
    EXPECT_EQ(0, EV_yields_gcn.at("Special Defense"));

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
    EXPECT_EQ("Pearl", pokemon_entry_gen4.get_game());
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

    std::map<std::string, int> base_stats_gen4 = pokemon_entry_gen4.get_base_stats();
    EXPECT_EQ(37, base_stats_gen4.at("HP"));
    EXPECT_EQ(25, base_stats_gen4.at("Attack"));
    EXPECT_EQ(41, base_stats_gen4.at("Defense"));
    EXPECT_EQ(25, base_stats_gen4.at("Speed"));
    EXPECT_EQ(25, base_stats_gen4.at("Special Attack"));
    EXPECT_EQ(41, base_stats_gen4.at("Special Defense"));

    std::map<std::string, int> EV_yields_gen4 = pokemon_entry_gen4.get_EV_yields();
    EXPECT_EQ(0, EV_yields_gen4.at("HP"));
    EXPECT_EQ(0, EV_yields_gen4.at("Attack"));
    EXPECT_EQ(1, EV_yields_gen4.at("Defense"));
    EXPECT_EQ(0, EV_yields_gen4.at("Speed"));
    EXPECT_EQ(0, EV_yields_gen4.at("Special Attack"));
    EXPECT_EQ(0, EV_yields_gen4.at("Special Defense"));

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
    EXPECT_EQ("Black 2", pokemon_entry_gen5.get_game());
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

    std::map<std::string, int> base_stats_gen5 = pokemon_entry_gen5.get_base_stats();
    EXPECT_EQ(109, base_stats_gen5.at("HP"));
    EXPECT_EQ(66, base_stats_gen5.at("Attack"));
    EXPECT_EQ(84, base_stats_gen5.at("Defense"));
    EXPECT_EQ(32, base_stats_gen5.at("Speed"));
    EXPECT_EQ(81, base_stats_gen5.at("Special Attack"));
    EXPECT_EQ(99, base_stats_gen5.at("Special Defense"));

    std::map<std::string, int> EV_yields_gen5 = pokemon_entry_gen5.get_EV_yields();
    EXPECT_EQ(2, EV_yields_gen5.at("HP"));
    EXPECT_EQ(0, EV_yields_gen5.at("Attack"));
    EXPECT_EQ(0, EV_yields_gen5.at("Defense"));
    EXPECT_EQ(0, EV_yields_gen5.at("Speed"));
    EXPECT_EQ(0, EV_yields_gen5.at("Special Attack"));
    EXPECT_EQ(0, EV_yields_gen5.at("Special Defense"));

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
    EXPECT_EQ("Alpha Sapphire", pokemon_entry_gen6.get_game());
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

    std::map<std::string, int> base_stats_gen6 = pokemon_entry_gen6.get_base_stats();
    EXPECT_EQ(95, base_stats_gen6.at("HP"));
    EXPECT_EQ(65, base_stats_gen6.at("Attack"));
    EXPECT_EQ(65, base_stats_gen6.at("Defense"));
    EXPECT_EQ(60, base_stats_gen6.at("Speed"));
    EXPECT_EQ(110, base_stats_gen6.at("Special Attack"));
    EXPECT_EQ(130, base_stats_gen6.at("Special Defense"));

    std::map<std::string, int> EV_yields_gen6 = pokemon_entry_gen6.get_EV_yields();
    EXPECT_EQ(0, EV_yields_gen6.at("HP"));
    EXPECT_EQ(0, EV_yields_gen6.at("Attack"));
    EXPECT_EQ(0, EV_yields_gen6.at("Defense"));
    EXPECT_EQ(0, EV_yields_gen6.at("Speed"));
    EXPECT_EQ(0, EV_yields_gen6.at("Special Attack"));
    EXPECT_EQ(2, EV_yields_gen6.at("Special Defense"));

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
