/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"

#include <pkmntest/util.hpp>

#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/enums/enum_to_string.hpp>
#include <pkmn/exception.hpp>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <iostream>

namespace fs = boost::filesystem;

static const pkmn::ability_pair_t ability_none_pair =
{
    pkmn::e_ability::NONE,
    pkmn::e_ability::NONE
};

static const pkmn::egg_group_pair_t egg_group_none_pair =
{
    pkmn::e_egg_group::NONE,
    pkmn::e_egg_group::NONE
};

static const pkmn::type_pair_t type_none_pair =
{
    pkmn::e_type::NONE,
    pkmn::e_type::NONE
};

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
BOOST_STATIC_CONSTEXPR int X = 23;

class pokemon_entry_none_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(pokemon_entry_none_test, pokemon_entry_none_test)
{
    pkmn::database::pokemon_entry none_entry(pkmn::e_species::NONE, GetParam(), "");

    EXPECT_EQ(pkmn::e_species::NONE, none_entry.get_species())
        << pkmn::species_to_string(none_entry.get_species());
    EXPECT_EQ("None", none_entry.get_species_name());
    EXPECT_EQ("None", none_entry.get_category());
    EXPECT_EQ("None", none_entry.get_pokedex_entry());
    EXPECT_EQ("None", none_entry.get_form());
    EXPECT_FLOAT_EQ(-1.0f, none_entry.get_height());
    EXPECT_FLOAT_EQ(-1.0f, none_entry.get_weight());
    EXPECT_FLOAT_EQ(-1.0f, none_entry.get_chance_male());
    EXPECT_FLOAT_EQ(-1.0f, none_entry.get_chance_female());
    EXPECT_FALSE(none_entry.has_gender_differences());
    EXPECT_EQ(-1.0f, none_entry.get_base_friendship());
    EXPECT_EQ(type_none_pair, none_entry.get_types());
    EXPECT_EQ(ability_none_pair, none_entry.get_abilities());
    EXPECT_EQ(pkmn::e_ability::NONE, none_entry.get_hidden_ability())
        << pkmn::ability_to_string(none_entry.get_hidden_ability());
    EXPECT_EQ(egg_group_none_pair, none_entry.get_egg_groups());
    if(none_entry.get_game_id() <= YELLOW)
    {
        EXPECT_EQ(none_entry.get_base_stats(), bad_stat_map_old);
        EXPECT_EQ(none_entry.get_EV_yields(), bad_stat_map_old);
    }
    else if(none_entry.get_game_id() <= CRYSTAL)
    {
        EXPECT_EQ(none_entry.get_base_stats(), bad_stat_map);
        EXPECT_EQ(none_entry.get_EV_yields(), bad_stat_map_old);
    }
    else
    {
        EXPECT_EQ(none_entry.get_base_stats(), bad_stat_map);
        EXPECT_EQ(none_entry.get_EV_yields(), bad_stat_map);
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
    if(none_entry.get_game_id() < X)
    {
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

TEST_P(pokemon_entry_evolutions_test, pokemon_entry_evolutions_test)
{
    pkmn::database::pokemon_entry eevee_entry(pkmn::e_species::EEVEE, GetParam(), "");
    int generation = pkmn::priv::game_enum_to_generation(eevee_entry.get_game());

    std::vector<pkmn::e_species> evolutions = eevee_entry.get_evolutions();
    EXPECT_GE(evolutions.size(), 3);

    EXPECT_EQ(pkmn::e_species::VAPOREON, evolutions.at(0));
    EXPECT_EQ(pkmn::e_species::JOLTEON, evolutions.at(1));
    EXPECT_EQ(pkmn::e_species::FLAREON, evolutions.at(2));

    if(generation >= 2)
    {
        EXPECT_EQ(pkmn::e_species::ESPEON, evolutions.at(3));
        EXPECT_EQ(pkmn::e_species::UMBREON, evolutions.at(4));
    }
    else
    {
        EXPECT_EQ(3, evolutions.size());
        return;
    }

    if(generation >= 4)
    {
        EXPECT_EQ(pkmn::e_species::LEAFEON, evolutions.at(5));
        EXPECT_EQ(pkmn::e_species::GLACEON, evolutions.at(6));
    }
    else
    {
        EXPECT_EQ(5, evolutions.size());
        return;
    }

    if(generation >= 6)
    {
        EXPECT_EQ(pkmn::e_species::SYLVEON, evolutions.at(7));
    }
    else
    {
        EXPECT_EQ(7, evolutions.size());
        return;
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
            byname_gen1 = pkmn::database::pokemon_entry(pkmn::e_species::ARTICUNO, pkmn::e_game::YELLOW, "");
            byname_gen2 = pkmn::database::pokemon_entry(pkmn::e_species::FERALIGATR, pkmn::e_game::GOLD, "");
            byname_gba = pkmn::database::pokemon_entry(pkmn::e_species::REGICE, pkmn::e_game::EMERALD, "");
            byname_gcn = pkmn::database::pokemon_entry(pkmn::e_species::SHROOMISH, pkmn::e_game::XD, "");
            byname_gen4 = pkmn::database::pokemon_entry(pkmn::e_species::KRICKETOT, pkmn::e_game::PEARL, "");
            byname_gen5 = pkmn::database::pokemon_entry(pkmn::e_species::STUNFISK, pkmn::e_game::BLACK2, "");
            byname_gen6 = pkmn::database::pokemon_entry(pkmn::e_species::SYLVEON, pkmn::e_game::ALPHA_SAPPHIRE, "");
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
            pkmn::e_species::SNUBBULL, pkmn::e_game::YELLOW, ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry ralts(
            pkmn::e_species::RALTS, pkmn::e_game::SILVER, ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry budew(
            pkmn::e_species::BUDEW, pkmn::e_game::SAPPHIRE, ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry liepard(
            pkmn::e_species::LIEPARD, pkmn::e_game::PEARL, ""
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::pokemon_entry espurr(
            pkmn::e_species::ESPURR, pkmn::e_game::WHITE, ""
        );
    , std::invalid_argument);

    /*
     * Make sure forms can't be used in the wrong games, even if
     * the Pokémon existed.
     */
    EXPECT_THROW(
        pkmn::database::pokemon_entry arceus(
            pkmn::e_species::ARCEUS, pkmn::e_game::DIAMOND, "Fairy"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus1(
        pkmn::e_species::ARCEUS, pkmn::e_game::Y, "Fairy"
    );
    EXPECT_THROW(
        pkmn::database::pokemon_entry arceus(
            pkmn::e_species::ARCEUS, pkmn::e_game::BLACK, "???"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus2(
        pkmn::e_species::ARCEUS, pkmn::e_game::PEARL, "???"
    );
    EXPECT_THROW(
        pkmn::database::pokemon_entry pichu(
            pkmn::e_species::PICHU, pkmn::e_game::SILVER, "Spiky-eared"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry pichu(
        pkmn::e_species::PICHU, pkmn::e_game::HEARTGOLD, "Spiky-eared"
    );
    EXPECT_THROW(
        pkmn::database::pokemon_entry pikachu(
            pkmn::e_species::PIKACHU, pkmn::e_game::RUBY, "Belle"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry pikachu(
        pkmn::e_species::PIKACHU, pkmn::e_game::OMEGA_RUBY, "Belle"
    );
}

/*
 * Make sure Pokémon whose type has changed between generations
 * reflect that in their entries.
 */
TEST_F(pokemon_entry_test, changing_type_test) {
    pkmn::database::pokemon_entry clefairy5(
        pkmn::e_species::CLEFAIRY, pkmn::e_game::BLACK, ""
    );
    pkmn::database::pokemon_entry clefairy6(
        pkmn::e_species::CLEFAIRY, pkmn::e_game::Y, ""
    );

    pkmn::type_pair_t clefairy_types5 = clefairy5.get_types();
    pkmn::type_pair_t clefairy_types6 = clefairy6.get_types();

    EXPECT_EQ(pkmn::e_type::NORMAL, clefairy_types5.first)
        << pkmn::type_to_string(clefairy_types5.first);
    EXPECT_EQ(pkmn::e_type::NONE, clefairy_types5.second);

    EXPECT_EQ(pkmn::e_type::FAIRY, clefairy_types6.first)
        << pkmn::type_to_string(clefairy_types6.first);
    EXPECT_EQ(pkmn::e_type::NONE, clefairy_types6.second);

    pkmn::database::pokemon_entry jigglypuff5(
        pkmn::e_species::JIGGLYPUFF, pkmn::e_game::BLACK, ""
    );
    pkmn::database::pokemon_entry jigglypuff6(
        pkmn::e_species::JIGGLYPUFF, pkmn::e_game::Y, ""
    );

    pkmn::type_pair_t jigglypuff_types5 = jigglypuff5.get_types();
    pkmn::type_pair_t jigglypuff_types6 = jigglypuff6.get_types();

    EXPECT_EQ(pkmn::e_type::NORMAL, jigglypuff_types5.first)
        << pkmn::type_to_string(jigglypuff_types5.first);
    EXPECT_EQ(pkmn::e_type::NONE, jigglypuff_types5.second);

    EXPECT_EQ(pkmn::e_type::NORMAL, jigglypuff_types6.first)
        << pkmn::type_to_string(jigglypuff_types6.first);
    EXPECT_EQ(pkmn::e_type::FAIRY, jigglypuff_types6.second)
        << pkmn::type_to_string(jigglypuff_types6.second);
}

/*
 * Make sure that if a second ability was added in a later generation,
 * that it does not appear in a too-early generation.
 */
TEST_F(pokemon_entry_test, changing_abilities_test)
{
    struct changing_abilities_test_params
    {
        pkmn::e_species species;
        pkmn::e_game first_game;
        pkmn::e_game second_game;

        pkmn::e_ability first_ability;
        pkmn::e_ability second_ability;
    };

    static const std::vector<changing_abilities_test_params> all_test_params =
    {
        {
            pkmn::e_species::PIDGEY,
            pkmn::e_game::RUBY, pkmn::e_game::DIAMOND,
            pkmn::e_ability::KEEN_EYE, pkmn::e_ability::TANGLED_FEET
        },
        {
            pkmn::e_species::SHROOMISH,
            pkmn::e_game::RUBY, pkmn::e_game::DIAMOND,
            pkmn::e_ability::EFFECT_SPORE, pkmn::e_ability::POISON_HEAL
        }
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

        pkmn::ability_pair_t first_game_abilities = first_game_entry.get_abilities();
        EXPECT_EQ(
            test_params.first_ability,
            first_game_abilities.first
        ) << pkmn::species_to_string(test_params.species) << " "
          << pkmn::game_to_string(test_params.first_game) << " "
          << pkmn::ability_to_string(test_params.first_ability);
        EXPECT_EQ(
            pkmn::e_ability::NONE,
            first_game_abilities.second
        ) << pkmn::species_to_string(test_params.species) << " "
          << pkmn::game_to_string(test_params.first_game) << " "
          << pkmn::ability_to_string(test_params.second_ability);

        pkmn::ability_pair_t second_game_abilities = second_game_entry.get_abilities();
        EXPECT_EQ(
            test_params.first_ability,
            second_game_abilities.first
        ) << pkmn::species_to_string(test_params.species) << " "
          << pkmn::game_to_string(test_params.second_game) << " "
          << pkmn::ability_to_string(test_params.first_ability);
        EXPECT_EQ(
            test_params.second_ability,
            second_game_abilities.second
        ) << pkmn::species_to_string(test_params.species) << " "
          << pkmn::game_to_string(test_params.second_game) << " "
          << pkmn::ability_to_string(test_params.second_ability);
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
    EXPECT_EQ(pkmn::e_species::ARTICUNO, pokemon_entry_gen1.get_species())
        << pkmn::species_to_string(pokemon_entry_gen1.get_species());
    EXPECT_EQ("Articuno", pokemon_entry_gen1.get_species_name());
    EXPECT_EQ(pkmn::e_game::YELLOW, pokemon_entry_gen1.get_game())
        << pkmn::game_to_string(pokemon_entry_gen1.get_game());
    EXPECT_EQ("Freeze", pokemon_entry_gen1.get_category());
    EXPECT_GT(pokemon_entry_gen1.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen1.get_form());
    EXPECT_FLOAT_EQ(1.7f, pokemon_entry_gen1.get_height());
    EXPECT_FLOAT_EQ(55.4f, pokemon_entry_gen1.get_weight());
    EXPECT_FLOAT_EQ(0.0f, pokemon_entry_gen1.get_chance_male());
    EXPECT_FLOAT_EQ(0.0f, pokemon_entry_gen1.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gen1.has_gender_differences());
    EXPECT_EQ(-1, pokemon_entry_gen1.get_base_friendship());

    pkmn::type_pair_t types1 = pokemon_entry_gen1.get_types();
    EXPECT_EQ(pkmn::e_type::ICE, types1.first)
        << pkmn::type_to_string(types1.first);
    EXPECT_EQ(pkmn::e_type::FLYING, types1.second)
        << pkmn::type_to_string(types1.second);

    EXPECT_EQ(ability_none_pair, pokemon_entry_gen1.get_abilities());
    EXPECT_EQ(pkmn::e_ability::NONE, pokemon_entry_gen1.get_hidden_ability())
        << pkmn::ability_to_string(pokemon_entry_gen1.get_hidden_ability());
    EXPECT_EQ(egg_group_none_pair, pokemon_entry_gen1.get_egg_groups());

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
    EXPECT_EQ(pkmn::e_species::FERALIGATR, pokemon_entry_gen2.get_species())
        << pkmn::species_to_string(pokemon_entry_gen2.get_species());
    EXPECT_EQ("Feraligatr", pokemon_entry_gen2.get_species_name());
    EXPECT_EQ(pkmn::e_game::GOLD, pokemon_entry_gen2.get_game())
        << pkmn::game_to_string(pokemon_entry_gen2.get_game());
    EXPECT_EQ("Big Jaw", pokemon_entry_gen2.get_category());
    EXPECT_GT(pokemon_entry_gen2.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen2.get_form());
    EXPECT_FLOAT_EQ(2.3f, pokemon_entry_gen2.get_height());
    EXPECT_FLOAT_EQ(88.8f, pokemon_entry_gen2.get_weight());
    EXPECT_FLOAT_EQ(0.875f, pokemon_entry_gen2.get_chance_male());
    EXPECT_FLOAT_EQ(0.125f, pokemon_entry_gen2.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gen2.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gen2.get_base_friendship());

    pkmn::type_pair_t types2 = pokemon_entry_gen2.get_types();
    EXPECT_EQ(pkmn::e_type::WATER, types2.first)
        << pkmn::type_to_string(types2.first);
    EXPECT_EQ(pkmn::e_type::NONE, types2.second)
        << pkmn::type_to_string(types2.second);

    EXPECT_EQ(ability_none_pair, pokemon_entry_gen2.get_abilities());
    EXPECT_EQ(pkmn::e_ability::NONE, pokemon_entry_gen2.get_hidden_ability())
        << pkmn::ability_to_string(pokemon_entry_gen2.get_hidden_ability());

    pkmn::egg_group_pair_t egg_groups2 = pokemon_entry_gen2.get_egg_groups();
    EXPECT_EQ(pkmn::e_egg_group::MONSTER, egg_groups2.first)
        << pkmn::egg_group_to_string(egg_groups2.first);
    EXPECT_EQ(pkmn::e_egg_group::WATER1, egg_groups2.second)
        << pkmn::egg_group_to_string(egg_groups2.second);

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
    EXPECT_EQ(pkmn::e_species::REGICE, pokemon_entry_gba.get_species())
        << pkmn::species_to_string(pokemon_entry_gba.get_species());
    EXPECT_EQ("Regice", pokemon_entry_gba.get_species_name());
    EXPECT_EQ(pkmn::e_game::EMERALD, pokemon_entry_gba.get_game())
        << pkmn::game_to_string(pokemon_entry_gba.get_game());
    EXPECT_EQ("Iceberg", pokemon_entry_gba.get_category());
    EXPECT_GT(pokemon_entry_gba.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gba.get_form());
    EXPECT_FLOAT_EQ(1.8f, pokemon_entry_gba.get_height());
    EXPECT_FLOAT_EQ(175.0f, pokemon_entry_gba.get_weight());
    EXPECT_FLOAT_EQ(0.0f, pokemon_entry_gba.get_chance_male());
    EXPECT_FLOAT_EQ(0.0f, pokemon_entry_gba.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gba.has_gender_differences());
    EXPECT_EQ(35, pokemon_entry_gba.get_base_friendship());

    pkmn::type_pair_t types_gba = pokemon_entry_gba.get_types();
    EXPECT_EQ(pkmn::e_type::ICE, types_gba.first)
        << pkmn::type_to_string(types_gba.first);
    EXPECT_EQ(pkmn::e_type::NONE, types_gba.second)
        << pkmn::type_to_string(types_gba.second);

    pkmn::ability_pair_t abilities_gba = pokemon_entry_gba.get_abilities();
    EXPECT_EQ(pkmn::e_ability::CLEAR_BODY, abilities_gba.first)
        << pkmn::ability_to_string(abilities_gba.first);
    EXPECT_EQ(pkmn::e_ability::NONE, abilities_gba.second)
        << pkmn::ability_to_string(abilities_gba.second);

    EXPECT_EQ(pkmn::e_ability::NONE, pokemon_entry_gba.get_hidden_ability())
        << pkmn::ability_to_string(pokemon_entry_gba.get_hidden_ability());

    pkmn::egg_group_pair_t egg_groups_gba = pokemon_entry_gba.get_egg_groups();
    EXPECT_EQ(pkmn::e_egg_group::UNDISCOVERED, egg_groups_gba.first)
        << pkmn::egg_group_to_string(egg_groups_gba.first);
    EXPECT_EQ(pkmn::e_egg_group::NONE, egg_groups_gba.second)
        << pkmn::egg_group_to_string(egg_groups_gba.second);

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
    EXPECT_EQ(pkmn::e_species::SHROOMISH, pokemon_entry_gcn.get_species())
        << pkmn::species_to_string(pokemon_entry_gcn.get_species());
    EXPECT_EQ("Shroomish", pokemon_entry_gcn.get_species_name());
    EXPECT_EQ(pkmn::e_game::XD, pokemon_entry_gcn.get_game())
        << pkmn::game_to_string(pokemon_entry_gcn.get_game());
    EXPECT_EQ("Mushroom", pokemon_entry_gcn.get_category());
    EXPECT_GT(pokemon_entry_gcn.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gcn.get_form());
    EXPECT_FLOAT_EQ(0.4f, pokemon_entry_gcn.get_height());
    EXPECT_FLOAT_EQ(4.5f, pokemon_entry_gcn.get_weight());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gcn.get_chance_male());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gcn.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gcn.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gcn.get_base_friendship());

    pkmn::type_pair_t types_gcn = pokemon_entry_gcn.get_types();
    EXPECT_EQ(pkmn::e_type::GRASS, types_gcn.first)
        << pkmn::type_to_string(types_gcn.first);
    EXPECT_EQ(pkmn::e_type::NONE, types_gcn.second)
        << pkmn::type_to_string(types_gcn.second);

    pkmn::ability_pair_t abilities_gcn = pokemon_entry_gcn.get_abilities();
    EXPECT_EQ(pkmn::e_ability::EFFECT_SPORE, abilities_gcn.first)
        << pkmn::ability_to_string(abilities_gcn.first);
    EXPECT_EQ(pkmn::e_ability::NONE, abilities_gcn.second)
        << pkmn::ability_to_string(abilities_gcn.second);

    EXPECT_EQ(pkmn::e_ability::NONE, pokemon_entry_gcn.get_hidden_ability()) << pkmn::ability_to_string(pokemon_entry_gcn.get_hidden_ability());

    pkmn::egg_group_pair_t egg_groups_gcn = pokemon_entry_gcn.get_egg_groups();
    EXPECT_EQ(pkmn::e_egg_group::FAIRY, egg_groups_gcn.first)
        << pkmn::egg_group_to_string(egg_groups_gcn.first);
    EXPECT_EQ(pkmn::e_egg_group::PLANT, egg_groups_gcn.second)
        << pkmn::egg_group_to_string(egg_groups_gcn.second);

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

    EXPECT_TRUE(fs::exists(pokemon_entry_gcn.get_icon_filepath(false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gcn.get_icon_filepath(true)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gcn.get_sprite_filepath(false,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gcn.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gcn.get_sprite_filepath(true,false)));
    EXPECT_TRUE(fs::exists(pokemon_entry_gcn.get_sprite_filepath(true,true)));

    /*
     * Generation IV entry
     */
    EXPECT_EQ(pkmn::e_species::KRICKETOT, pokemon_entry_gen4.get_species())
        << pkmn::species_to_string(pokemon_entry_gen4.get_species());
    EXPECT_EQ("Kricketot", pokemon_entry_gen4.get_species_name());
    EXPECT_EQ(pkmn::e_game::PEARL, pokemon_entry_gen4.get_game())
        << pkmn::game_to_string(pokemon_entry_gen4.get_game());
    EXPECT_EQ("Cricket", pokemon_entry_gen4.get_category());
    EXPECT_GT(pokemon_entry_gen4.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen4.get_form());
    EXPECT_FLOAT_EQ(0.3f, pokemon_entry_gen4.get_height());
    EXPECT_FLOAT_EQ(2.2f, pokemon_entry_gen4.get_weight());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gen4.get_chance_male());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gen4.get_chance_female());
    EXPECT_TRUE(pokemon_entry_gen4.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gen4.get_base_friendship());

    pkmn::type_pair_t types_gen4 = pokemon_entry_gen4.get_types();
    EXPECT_EQ(pkmn::e_type::BUG, types_gen4.first)
        << pkmn::type_to_string(types_gen4.first);
    EXPECT_EQ(pkmn::e_type::NONE, types_gen4.second)
        << pkmn::type_to_string(types_gen4.second);

    pkmn::ability_pair_t abilities_gen4 = pokemon_entry_gen4.get_abilities();
    EXPECT_EQ(pkmn::e_ability::SHED_SKIN, abilities_gen4.first)
        << pkmn::ability_to_string(abilities_gen4.first);
    EXPECT_EQ(pkmn::e_ability::NONE, abilities_gen4.second)
        << pkmn::ability_to_string(abilities_gen4.second);

    EXPECT_EQ(pkmn::e_ability::NONE, pokemon_entry_gen4.get_hidden_ability())
        << pkmn::ability_to_string(pokemon_entry_gen4.get_hidden_ability());

    pkmn::egg_group_pair_t egg_groups_gen4 = pokemon_entry_gen4.get_egg_groups();
    EXPECT_EQ(pkmn::e_egg_group::BUG, egg_groups_gen4.first)
        << pkmn::egg_group_to_string(egg_groups_gen4.first);
    EXPECT_EQ(pkmn::e_egg_group::NONE, egg_groups_gen4.second)
        << pkmn::egg_group_to_string(egg_groups_gen4.second);

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
    EXPECT_EQ(pkmn::e_species::STUNFISK, pokemon_entry_gen5.get_species())
        << pkmn::species_to_string(pokemon_entry_gen5.get_species());
    EXPECT_EQ("Stunfisk", pokemon_entry_gen5.get_species_name());
    EXPECT_EQ(pkmn::e_game::BLACK2, pokemon_entry_gen5.get_game())
        << pkmn::game_to_string(pokemon_entry_gen5.get_game());
    EXPECT_EQ("Trap", pokemon_entry_gen5.get_category());
    EXPECT_GT(pokemon_entry_gen5.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen5.get_form());
    EXPECT_FLOAT_EQ(0.7f, pokemon_entry_gen5.get_height());
    EXPECT_FLOAT_EQ(11.0f, pokemon_entry_gen5.get_weight());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gen5.get_chance_male());
    EXPECT_FLOAT_EQ(0.5f, pokemon_entry_gen5.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gen5.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gen5.get_base_friendship());

    pkmn::type_pair_t types_gen5 = pokemon_entry_gen5.get_types();
    EXPECT_EQ(pkmn::e_type::GROUND, types_gen5.first)
        << pkmn::type_to_string(types_gen5.first);
    EXPECT_EQ(pkmn::e_type::ELECTRIC, types_gen5.second)
        << pkmn::type_to_string(types_gen5.second);

    pkmn::ability_pair_t abilities_gen5 = pokemon_entry_gen5.get_abilities();
    EXPECT_EQ(pkmn::e_ability::STATIC, abilities_gen5.first)
        << pkmn::ability_to_string(abilities_gen5.first);
    EXPECT_EQ(pkmn::e_ability::LIMBER, abilities_gen5.second)
        << pkmn::ability_to_string(abilities_gen5.second);

    EXPECT_EQ(pkmn::e_ability::SAND_VEIL, pokemon_entry_gen5.get_hidden_ability())
        << pkmn::ability_to_string(pokemon_entry_gen5.get_hidden_ability());

    pkmn::egg_group_pair_t egg_groups_gen5 = pokemon_entry_gen5.get_egg_groups();
    EXPECT_EQ(pkmn::e_egg_group::WATER1, egg_groups_gen5.first)
        << pkmn::egg_group_to_string(egg_groups_gen5.first);
    EXPECT_EQ(pkmn::e_egg_group::INDETERMINATE, egg_groups_gen5.second)
        << pkmn::egg_group_to_string(egg_groups_gen5.second);

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
    EXPECT_EQ(pkmn::e_species::SYLVEON, pokemon_entry_gen6.get_species())
        << pkmn::species_to_string(pokemon_entry_gen6.get_species());
    EXPECT_EQ("Sylveon", pokemon_entry_gen6.get_species_name());
    EXPECT_EQ(pkmn::e_game::ALPHA_SAPPHIRE, pokemon_entry_gen6.get_game())
        << pkmn::game_to_string(pokemon_entry_gen6.get_game());
    EXPECT_EQ("Intertwining", pokemon_entry_gen6.get_category());
    EXPECT_GT(pokemon_entry_gen6.get_pokedex_entry().size(), 0);
    EXPECT_EQ("Standard", pokemon_entry_gen6.get_form());
    EXPECT_FLOAT_EQ(1.0f, pokemon_entry_gen6.get_height());
    EXPECT_FLOAT_EQ(23.5f, pokemon_entry_gen6.get_weight());
    EXPECT_FLOAT_EQ(0.875f, pokemon_entry_gen6.get_chance_male());
    EXPECT_FLOAT_EQ(0.125f, pokemon_entry_gen6.get_chance_female());
    EXPECT_FALSE(pokemon_entry_gen6.has_gender_differences());
    EXPECT_EQ(70, pokemon_entry_gen6.get_base_friendship());

    pkmn::type_pair_t types_gen6 = pokemon_entry_gen6.get_types();
    EXPECT_EQ(pkmn::e_type::FAIRY, types_gen6.first)
        << pkmn::type_to_string(types_gen6.first);
    EXPECT_EQ(pkmn::e_type::NONE, types_gen6.second)
        << pkmn::type_to_string(types_gen6.second);

    pkmn::ability_pair_t abilities_gen6 = pokemon_entry_gen6.get_abilities();
    EXPECT_EQ(pkmn::e_ability::CUTE_CHARM, abilities_gen6.first)
        << pkmn::ability_to_string(abilities_gen6.first);
    EXPECT_EQ(pkmn::e_ability::NONE, abilities_gen6.second)
        << pkmn::ability_to_string(abilities_gen6.second);

    EXPECT_EQ(pkmn::e_ability::PIXILATE, pokemon_entry_gen6.get_hidden_ability())
        << pkmn::ability_to_string(pokemon_entry_gen6.get_hidden_ability());

    pkmn::egg_group_pair_t egg_groups_gen6 = pokemon_entry_gen6.get_egg_groups();
    EXPECT_EQ(pkmn::e_egg_group::GROUND, egg_groups_gen6.first)
        << pkmn::egg_group_to_string(egg_groups_gen6.first);
    EXPECT_EQ(pkmn::e_egg_group::NONE, egg_groups_gen6.second)
        << pkmn::egg_group_to_string(egg_groups_gen6.second);

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
