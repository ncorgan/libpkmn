/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/util.hpp>

#include <pkmn/exception.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/enums/enum_to_string.hpp>

#include "utils/misc.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <tuple>

static inline bool string_in_vector(
    const std::vector<std::string> &vec,
    const std::string& str
) {
    return (std::find(vec.begin(), vec.end(), str) != vec.end());
}

TEST(cpp_lists_test, ability_lists_test)
{
    static const int MIN_GENERATION = 3;
    static const int MAX_GENERATION = 6;

    // Enums

    std::vector<pkmn::e_ability> abilities_gen3, abilities_gen4,
                                 abilities_gen5, abilities_gen6;

    EXPECT_THROW(
        (void)pkmn::database::get_ability_list(MIN_GENERATION-1);
    , std::out_of_range);
    EXPECT_THROW(
        (void)pkmn::database::get_ability_list(MAX_GENERATION+1);
    , std::out_of_range);

    abilities_gen3 = pkmn::database::get_ability_list(3);
    EXPECT_FALSE(pkmn::does_vector_contain_value(
        abilities_gen3,
        pkmn::e_ability::ADAPTABILITY
    ));

    abilities_gen4 = pkmn::database::get_ability_list(4);
    EXPECT_TRUE(pkmn::does_vector_contain_value(
        abilities_gen4,
        pkmn::e_ability::ADAPTABILITY)
    );
    EXPECT_FALSE(pkmn::does_vector_contain_value(
        abilities_gen4,
        pkmn::e_ability::ANALYTIC
    ));

    abilities_gen5 = pkmn::database::get_ability_list(5);
    EXPECT_TRUE(pkmn::does_vector_contain_value(
        abilities_gen5,
        pkmn::e_ability::ANALYTIC
    ));
    EXPECT_FALSE(pkmn::does_vector_contain_value(
        abilities_gen5,
        pkmn::e_ability::AERILATE
    ));

    abilities_gen6 = pkmn::database::get_ability_list(6);
    EXPECT_TRUE(pkmn::does_vector_contain_value(
        abilities_gen6,
        pkmn::e_ability::AERILATE
    ));

    ASSERT_FALSE(abilities_gen3.empty());
    EXPECT_TRUE(abilities_gen3.size() < abilities_gen4.size());
    EXPECT_TRUE(abilities_gen4.size() < abilities_gen5.size());
    EXPECT_TRUE(abilities_gen5.size() < abilities_gen6.size());

    // Names

    std::vector<std::string> ability_names_gen3, ability_names_gen4,
                             ability_names_gen5, ability_names_gen6;

    EXPECT_THROW(
        (void)pkmn::database::get_ability_name_list(MIN_GENERATION-1);
    , std::out_of_range);
    EXPECT_THROW(
        (void)pkmn::database::get_ability_name_list(MAX_GENERATION+1);
    , std::out_of_range);

    ability_names_gen3 = pkmn::database::get_ability_name_list(3);
    EXPECT_FALSE(pkmn::does_vector_contain_value<std::string>(
        ability_names_gen3,
        "Adaptability"
    ));

    ability_names_gen4 = pkmn::database::get_ability_name_list(4);
    EXPECT_TRUE(pkmn::does_vector_contain_value<std::string>(
        ability_names_gen4,
        "Adaptability"
    ));
    EXPECT_FALSE(pkmn::does_vector_contain_value<std::string>(
        ability_names_gen4,
        "Analytic"
    ));

    ability_names_gen5 = pkmn::database::get_ability_name_list(5);
    EXPECT_TRUE(pkmn::does_vector_contain_value<std::string>(
        ability_names_gen5,
        "Analytic"
    ));
    EXPECT_FALSE(pkmn::does_vector_contain_value<std::string>(
        ability_names_gen5,
        "Aerilate"
    ));

    ability_names_gen6 = pkmn::database::get_ability_name_list(6);
    EXPECT_TRUE(pkmn::does_vector_contain_value<std::string>(
        ability_names_gen6,
        "Aerilate"
    ));

    ASSERT_FALSE(ability_names_gen3.empty());
    EXPECT_TRUE(ability_names_gen3.size() < ability_names_gen4.size());
    EXPECT_TRUE(ability_names_gen4.size() < ability_names_gen5.size());
    EXPECT_TRUE(ability_names_gen5.size() < ability_names_gen6.size());

    // Compare the enum and string vectors.
    typedef std::pair<const std::vector<pkmn::e_ability>&, const std::vector<std::string>&> ability_vector_const_ref_pair_t;
    const std::vector<ability_vector_const_ref_pair_t> ABILITY_VECTOR_PAIRS =
    {
        {abilities_gen3, ability_names_gen3},
        {abilities_gen4, ability_names_gen4},
        {abilities_gen5, ability_names_gen5},
        {abilities_gen6, ability_names_gen6},
    };
    for(const ability_vector_const_ref_pair_t& ability_const_ref_pair: ABILITY_VECTOR_PAIRS)
    {
        const std::vector<pkmn::e_ability>& enum_vector = ability_const_ref_pair.first;
        const std::vector<std::string>& string_vector = ability_const_ref_pair.second;

        ASSERT_EQ(enum_vector.size(), string_vector.size());
        for(size_t ability_index = 0; ability_index < enum_vector.size(); ++ability_index)
        {
            EXPECT_EQ(
                string_vector[ability_index],
                pkmn::ability_to_string(enum_vector[ability_index])
            );
        }
    }
}

TEST(cpp_lists_test, game_lists_test)
{
    static const int MIN_GENERATION = 1;
    static const int MAX_GENERATION = 6;

    /*
     * Make sure invalid generations result in an exception.
     */

    EXPECT_THROW(
        (void)pkmn::database::get_game_list(MIN_GENERATION-1, true);
    , std::out_of_range);
    EXPECT_THROW(
        (void)pkmn::database::get_game_name_list(MIN_GENERATION-1, true);
    , std::out_of_range);

    EXPECT_THROW(
        (void)pkmn::database::get_game_name_list(MAX_GENERATION+1, true);
    , std::out_of_range);
    EXPECT_THROW(
        (void)pkmn::database::get_game_list(MAX_GENERATION+1, true);
    , std::out_of_range);

    struct game_lists_test_params
    {
        int generation;
        size_t num_games_in_generation;
        size_t num_games_up_to_generation;
    };
    static const std::vector<game_lists_test_params> ALL_TEST_PARAMS =
    {
        {1, 3, 3},
        {2, 3, 6},
        {3, 7, 13},
        {4, 5, 18},
        {5, 4, 22},
        {6, 4, 26}
    };
    for(const game_lists_test_params& test_params: ALL_TEST_PARAMS)
    {
        // Enums

        std::vector<pkmn::e_game> game_enums_one_gen = pkmn::database::get_game_list(
                                                           test_params.generation,
                                                           false
                                                       );
        EXPECT_EQ(
            test_params.num_games_in_generation,
            game_enums_one_gen.size()
        );

        std::vector<pkmn::e_game> game_enums_all_gens = pkmn::database::get_game_list(
                                                            test_params.generation,
                                                            true
                                                        );
        EXPECT_EQ(
            test_params.num_games_up_to_generation,
            game_enums_all_gens.size()
        );

        // Strings

        std::vector<std::string> game_names_one_gen = pkmn::database::get_game_name_list(
                                                          test_params.generation,
                                                          false
                                                      );
        EXPECT_EQ(
            test_params.num_games_in_generation,
            game_names_one_gen.size()
        );

        std::vector<std::string> game_names_all_gens = pkmn::database::get_game_name_list(
                                                           test_params.generation,
                                                           true
                                                       );
        EXPECT_EQ(
            test_params.num_games_up_to_generation,
            game_names_all_gens.size()
        );

        // Compare the enum and string vectors.

        ASSERT_EQ(game_enums_one_gen.size(), game_names_one_gen.size());
        for(size_t game_index = 0; game_index < game_enums_one_gen.size(); ++game_index)
        {
            EXPECT_EQ(
                game_names_one_gen[game_index],
                pkmn::game_to_string(game_enums_one_gen[game_index])
            );
        }

        ASSERT_EQ(game_enums_all_gens.size(), game_names_all_gens.size());
        for(size_t game_index = 0; game_index < game_enums_all_gens.size(); ++game_index)
        {
            EXPECT_EQ(
                game_names_all_gens[game_index],
                pkmn::game_to_string(game_enums_all_gens[game_index])
            );
        }
    }
}

TEST(cpp_lists_test, gamecube_shadow_pokemon_lists_test)
{
    // Colosseum
    static const size_t EXPECTED_NUM_COLOSSEUM_POKEMON = 48;

    std::vector<pkmn::e_species> colosseum_shadow_pokemon_enums =
        pkmn::database::get_gamecube_shadow_pokemon_list(true);
    ASSERT_EQ(
        EXPECTED_NUM_COLOSSEUM_POKEMON,
        colosseum_shadow_pokemon_enums.size()
    );

    std::vector<std::string> colosseum_shadow_pokemon_names =
        pkmn::database::get_gamecube_shadow_pokemon_name_list(true);
    ASSERT_EQ(
        EXPECTED_NUM_COLOSSEUM_POKEMON,
        colosseum_shadow_pokemon_names.size()
    );

    for(size_t species_index = 0;
        species_index < EXPECTED_NUM_COLOSSEUM_POKEMON;
        ++species_index)
    {
        EXPECT_EQ(
            colosseum_shadow_pokemon_names[species_index],
            pkmn::species_to_string(colosseum_shadow_pokemon_enums[species_index])
        );
    }

    // XD
    static const size_t EXPECTED_NUM_XD_POKEMON = 83;

    std::vector<pkmn::e_species> xd_shadow_pokemon_enums =
        pkmn::database::get_gamecube_shadow_pokemon_list(false);
    ASSERT_EQ(
        EXPECTED_NUM_XD_POKEMON,
        xd_shadow_pokemon_enums.size()
    );

    std::vector<std::string> xd_shadow_pokemon_names =
        pkmn::database::get_gamecube_shadow_pokemon_name_list(false);
    ASSERT_EQ(
        EXPECTED_NUM_XD_POKEMON,
        xd_shadow_pokemon_names.size()
    );

    for(size_t species_index = 0;
        species_index < EXPECTED_NUM_XD_POKEMON;
        ++species_index)
    {
        EXPECT_EQ(
            xd_shadow_pokemon_names[species_index],
            pkmn::species_to_string(xd_shadow_pokemon_enums[species_index])
        );
    }
}

TEST(cpp_lists_test, item_lists_test)
{
    /*
     * Make sure invalid games fail.
     */
    EXPECT_THROW(
        (void)pkmn::database::get_item_list(pkmn::e_game::NONE);
    , std::invalid_argument);
    EXPECT_THROW(
        (void)pkmn::database::get_item_name_list(pkmn::e_game::NONE);
    , std::invalid_argument);

    struct item_list_test_params
    {
        std::vector<pkmn::e_game> games;
        std::vector<pkmn::e_item> items_expected_present;
        std::vector<pkmn::e_item> items_expected_not_present;
        std::vector<std::string> item_names_expected_present;
        std::vector<std::string> item_names_expected_not_present;
    };
    static const std::vector<item_list_test_params> ALL_TEST_PARAMS =
    {
        {
            pkmntest::GEN1_GAMES,
            {pkmn::e_item::DOWSING_MACHINE},
            {},
            {"Itemfinder"},
            {
                "Dowsing MCHN",
                "Dowsing Machine"
            }
        },
        {
            {pkmn::e_game::YELLOW},
            {pkmn::e_item::DOWSING_MACHINE},
            {},
            {"Itemfinder"},
            {
                "Dowsing MCHN",
                "Dowsing Machine"
            }
        },
        {
            {
                pkmn::e_game::GOLD,
                pkmn::e_game::SILVER
            },
            {pkmn::e_item::DOWSING_MACHINE},
            {pkmn::e_item::GS_BALL},
            {
                "Blk Apricorn",
                "RAGECANDYBAR",
                "Itemfinder"
            },
            {
                "Black Apricorn",
                "GS Ball",
                "Rage Candy Bar",
                "Dowsing MCHN",
                "Dowsing Machine"
            }
        },
        {
            {pkmn::e_game::CRYSTAL},
            {
                pkmn::e_item::GS_BALL,
                pkmn::e_item::DOWSING_MACHINE
            },
            {},
            {
                "Blk Apricorn",
                "GS Ball",
                "RAGECANDYBAR",
                "Itemfinder"
            },
            {
                "Black Apricorn",
                "Rage Candy Bar",
                "Dowsing MCHN",
                "Dowsing Machine"
            }
        },
        {
            {
                pkmn::e_game::RUBY,
                pkmn::e_game::SAPPHIRE
            },
            {pkmn::e_item::DOWSING_MACHINE},
            {
                pkmn::e_item::MAGMA_EMBLEM,
                pkmn::e_item::HELIX_FOSSIL,
                pkmn::e_item::TIME_FLUTE,
                pkmn::e_item::BATTLE_CD_01
            },
            {"Itemfinder"},
            {
                "Magma Emblem",
                "Helix Fossil",
                "Time Flute",
                "Battle CD 01",
                "Dowsing MCHN",
                "Dowsing Machine"
            },
        },
        {
            {pkmn::e_game::EMERALD},
            {
                pkmn::e_item::MAGMA_EMBLEM,
                pkmn::e_item::HELIX_FOSSIL,
                pkmn::e_item::DOWSING_MACHINE
            },
            {
                pkmn::e_item::TIME_FLUTE,
                pkmn::e_item::BATTLE_CD_01
            },
            {
                "Magma Emblem",
                "Helix Fossil",
                "Itemfinder"
            },
            {
                "Time Flute",
                "Battle CD 01",
                "Dowsing MCHN",
                "Dowsing Machine"
            },
        },
        {
            {
                pkmn::e_game::FIRERED,
                pkmn::e_game::LEAFGREEN
            },
            {
                pkmn::e_item::HELIX_FOSSIL,
                pkmn::e_item::DOWSING_MACHINE
            },
            {
                pkmn::e_item::MAGMA_EMBLEM,
                pkmn::e_item::TIME_FLUTE,
                pkmn::e_item::BATTLE_CD_01
            },
            {
                "Helix Fossil",
                "Itemfinder"
            },
            {
                "Magma Emblem",
                "Time Flute",
                "Battle CD 01",
                "Dowsing MCHN",
                "Dowsing Machine"
            },
        },
        {
            {pkmn::e_game::COLOSSEUM},
            {pkmn::e_item::TIME_FLUTE},
            {
                pkmn::e_item::MAGMA_EMBLEM,
                pkmn::e_item::HELIX_FOSSIL,
                pkmn::e_item::BATTLE_CD_01,
                pkmn::e_item::DOWSING_MACHINE
            },
            {"Time Flute"},
            {
                "Magma Emblem",
                "Helix Fossil",
                "Battle CD 01",
                "Itemfinder"
            },
        },
        {
            {pkmn::e_game::XD},
            {pkmn::e_item::BATTLE_CD_01},
            {
                pkmn::e_item::MAGMA_EMBLEM,
                pkmn::e_item::HELIX_FOSSIL,
                pkmn::e_item::TIME_FLUTE,
                pkmn::e_item::DOWSING_MACHINE
            },
            {"Battle CD 01"},
            {
                "Magma Emblem",
                "Helix Fossil",
                "Time Flute",
                "Itemfinder"
            },
        },
        {
            {
                pkmn::e_game::DIAMOND,
                pkmn::e_game::PEARL
            },
            {},
            {
                pkmn::e_item::GRISEOUS_ORB,
                pkmn::e_item::DOWSING_MACHINE
            },
            {},
            {
                "Griseous Orb",
                "Dowsing MCHN"
            },
        },
        {
            {pkmn::e_game::PLATINUM},
            {pkmn::e_item::GRISEOUS_ORB},
            {pkmn::e_item::DOWSING_MACHINE},
            {"Griseous Orb"},
            {"Dowsing MCHN"}
        },
        {
            {
                pkmn::e_game::HEARTGOLD,
                pkmn::e_game::SOULSILVER
            },
            {
                pkmn::e_item::GRISEOUS_ORB,
                pkmn::e_item::DOWSING_MACHINE
            },
            {},
            {
                "Griseous Orb",
                "Dowsing MCHN"
            },
            {
                "Itemfinder",
                "Dowsing Machine"
            },
        },
        {
            {
                pkmn::e_game::BLACK,
                pkmn::e_game::WHITE
            },
            {pkmn::e_item::DOWSING_MACHINE},
            {pkmn::e_item::COLRESS_MACHINE},
            {"Dowsing MCHN"},
            {
                "Itemfinder",
                "Dowsing Machine",
                "Colress MCHN"
            }
        },
        {
            {
                pkmn::e_game::BLACK2,
                pkmn::e_game::WHITE2
            },
            {
                pkmn::e_item::DOWSING_MACHINE,
                pkmn::e_item::COLRESS_MACHINE
            },
            {},
            {
                "Dowsing MCHN",
                "Colress MCHN"
            },
            {
                "Itemfinder",
                "Dowsing Machine",
                "Colress Machine"
            },
        },
        {
            {
                pkmn::e_game::X,
                pkmn::e_game::Y
            },
            {pkmn::e_item::DOWSING_MACHINE},
            {pkmn::e_item::SLOWBRONITE},
            {
                "Dowsing Machine"
            },
            {
                "Itemfinder",
                "Dowsing MCHN",
                "Slowbronite"
            },
        },
        {
            {
                pkmn::e_game::OMEGA_RUBY,
                pkmn::e_game::ALPHA_SAPPHIRE
            },
            {
                pkmn::e_item::DOWSING_MACHINE,
                pkmn::e_item::SLOWBRONITE
            },
            {},
            {
                "Dowsing Machine",
                "Slowbronite"
            },
            {
                "Itemfinder",
                "Dowsing MCHN",
            },
        },
    };
    for(const item_list_test_params& test_params: ALL_TEST_PARAMS)
    {
        for(pkmn::e_game game: test_params.games)
        {
            // Enums

            std::vector<pkmn::e_item> items = pkmn::database::get_item_list(game);
            for(pkmn::e_item item: test_params.items_expected_present)
            {
                EXPECT_TRUE(pkmn::does_vector_contain_value(items, item))
                    << pkmn::game_to_string(game) << " " << int(item);
            }
            for(pkmn::e_item item: test_params.items_expected_not_present)
            {
                EXPECT_FALSE(pkmn::does_vector_contain_value(items, item))
                    << pkmn::game_to_string(game) << " " << int(item);
            }

            // Names

            std::vector<std::string> item_names = pkmn::database::get_item_name_list(game);
            for(const std::string& item_name: test_params.item_names_expected_present)
            {
                EXPECT_TRUE(pkmn::does_vector_contain_value(item_names, item_name))
                    << pkmn::game_to_string(game) << " " << item_name;
            }
            for(const std::string& item_name: test_params.item_names_expected_not_present)
            {
                EXPECT_FALSE(pkmn::does_vector_contain_value(item_names, item_name))
                    << pkmn::game_to_string(game) << " " << item_name;
            }

            // Compare the enum and item lists.
            ASSERT_EQ(items.size(), item_names.size()) << int(game);
            for(size_t item_index = 0; item_index < items.size(); ++item_index)
            {
                // TODO after creating string_to_item, since there can be multiple strings
                //      for a given ID
            }
        }
    }
}

TEST(cpp_lists_test, location_list_test)
{
    std::vector<std::string> locations_gen1,
                             locations_gs, locations_c,
                             locations_rs, locations_e,
                             locations_frlg, locations_colo,
                             locations_xd, locations_dp,
                             locations_pt, locations_hgss,
                             locations_bw, locations_b2w2,
                             locations_xy, locations_oras;

    /*
     * Make sure invalid games fail.
     */
    EXPECT_THROW(
        (void)pkmn::database::get_location_name_list(pkmn::e_game::NONE, true);
    , std::invalid_argument);

    /*
     * Generation I (TODO)
     */
    locations_gen1 = pkmn::database::get_location_name_list(pkmn::e_game::RED, true);
    /*EXPECT_GT(locations_gen1.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_gen1, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_gen1, "Cerulean Cave"));*/

    locations_gen1 = pkmn::database::get_location_name_list(pkmn::e_game::RED, false);
    /*EXPECT_GT(locations_gen1.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_gen1, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_gen1, "Cerulean Cave"));*/

    /*
     * Gold/Silver
     */
    locations_gs = pkmn::database::get_location_name_list(pkmn::e_game::SILVER, true);
    EXPECT_GT(locations_gs.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_gs, "Sprout Tower"));
    EXPECT_TRUE(string_in_vector(locations_gs, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_gs, "Battle Tower"));

    locations_gs = pkmn::database::get_location_name_list(pkmn::e_game::SILVER, false);
    EXPECT_GT(locations_gs.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_gs, "Sprout Tower"));
    EXPECT_TRUE(string_in_vector(locations_gs, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_gs, "Battle Tower"));

    /*
     * Crystal
     */
    locations_c = pkmn::database::get_location_name_list(pkmn::e_game::CRYSTAL, true);
    EXPECT_GT(locations_c.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_c, "Sprout Tower"));
    EXPECT_TRUE(string_in_vector(locations_c, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_c, "Battle Tower"));

    locations_c = pkmn::database::get_location_name_list(pkmn::e_game::CRYSTAL, false);
    EXPECT_GT(locations_c.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_c, "Sprout Tower"));
    EXPECT_TRUE(string_in_vector(locations_c, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_c, "Battle Tower"));

    /*
     * Ruby/Sapphire
     */
    locations_rs = pkmn::database::get_location_name_list(pkmn::e_game::RUBY, true);
    EXPECT_GT(locations_rs.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_rs, "New Mauville"));
    EXPECT_TRUE(string_in_vector(locations_rs, "Artisan Cave"));
    EXPECT_TRUE(string_in_vector(locations_rs, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Pyrite Town"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Kaminko's House"));

    locations_rs = pkmn::database::get_location_name_list(pkmn::e_game::RUBY, false);
    EXPECT_GT(locations_rs.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_rs, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_rs, "Kaminko's House"));

    /*
     * Emerald
     */
    locations_e = pkmn::database::get_location_name_list(pkmn::e_game::EMERALD, true);
    EXPECT_GT(locations_e.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_e, "New Mauville"));
    EXPECT_TRUE(string_in_vector(locations_e, "Artisan Cave"));
    EXPECT_TRUE(string_in_vector(locations_e, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_e, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_e, "Kaminko's House"));

    locations_e = pkmn::database::get_location_name_list(pkmn::e_game::EMERALD, false);
    EXPECT_GT(locations_e.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_e, "New Mauville"));
    EXPECT_TRUE(string_in_vector(locations_e, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_e, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_e, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_e, "Kaminko's House"));

    /*
     * FireRed/LeafGreen
     */
    locations_frlg = pkmn::database::get_location_name_list(pkmn::e_game::LEAFGREEN, true);
    EXPECT_GT(locations_frlg.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_frlg, "New Mauville"));
    EXPECT_TRUE(string_in_vector(locations_frlg, "Artisan Cave"));
    EXPECT_TRUE(string_in_vector(locations_frlg, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Kaminko's House"));

    locations_frlg = pkmn::database::get_location_name_list(pkmn::e_game::LEAFGREEN, false);
    EXPECT_GT(locations_frlg.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_frlg, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Artisan Cave"));
    EXPECT_TRUE(string_in_vector(locations_frlg, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_frlg, "Kaminko's House"));

    /*
     * Colosseum
     */
    locations_colo = pkmn::database::get_location_name_list(pkmn::e_game::COLOSSEUM, true);
    EXPECT_GT(locations_colo.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_colo, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_colo, "Realgamtwr Dome"));
    EXPECT_TRUE(string_in_vector(locations_colo, "Kaminko's House"));

    locations_colo = pkmn::database::get_location_name_list(pkmn::e_game::COLOSSEUM, false);
    EXPECT_GT(locations_colo.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_colo, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_colo, "Realgamtwr Dome"));
    EXPECT_FALSE(string_in_vector(locations_colo, "Kaminko's House"));

    /*
     * XD
     */
    locations_xd = pkmn::database::get_location_name_list(pkmn::e_game::XD, true);
    EXPECT_GT(locations_xd.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_xd, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Pallet Town"));
    EXPECT_TRUE(string_in_vector(locations_xd, "Realgamtwr Dome"));
    EXPECT_TRUE(string_in_vector(locations_xd, "Kaminko's House"));

    locations_xd = pkmn::database::get_location_name_list(pkmn::e_game::XD, false);
    EXPECT_GT(locations_xd.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_xd, "New Mauville"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Artisan Cave"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Pallet Town"));
    EXPECT_FALSE(string_in_vector(locations_xd, "Realgamtwr Dome"));
    EXPECT_TRUE(string_in_vector(locations_xd, "Kaminko's House"));

    /*
     * Diamond/Pearl
     */
    locations_dp = pkmn::database::get_location_name_list(pkmn::e_game::DIAMOND, true);
    EXPECT_GT(locations_dp.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_dp, "Route 221"));
    EXPECT_TRUE(string_in_vector(locations_dp, "Distortion World"));
    EXPECT_TRUE(string_in_vector(locations_dp, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_dp, "PC Tokyo"));

    locations_dp = pkmn::database::get_location_name_list(pkmn::e_game::DIAMOND, false);
    EXPECT_GT(locations_dp.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_dp, "Route 221"));
    EXPECT_FALSE(string_in_vector(locations_dp, "Distortion World"));
    EXPECT_FALSE(string_in_vector(locations_dp, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_dp, "PC Tokyo"));

    /*
     * Platinum
     */
    locations_pt = pkmn::database::get_location_name_list(pkmn::e_game::PLATINUM, true);
    EXPECT_GT(locations_pt.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_pt, "Route 221"));
    EXPECT_TRUE(string_in_vector(locations_pt, "Distortion World"));
    EXPECT_TRUE(string_in_vector(locations_pt, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_pt, "PC Tokyo"));

    locations_pt = pkmn::database::get_location_name_list(pkmn::e_game::PLATINUM, false);
    EXPECT_GT(locations_pt.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_pt, "Route 221"));
    EXPECT_TRUE(string_in_vector(locations_pt, "Distortion World"));
    EXPECT_FALSE(string_in_vector(locations_pt, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_pt, "PC Tokyo"));

    /*
     * HeartGold/SoulSilver
     */
    locations_hgss = pkmn::database::get_location_name_list(pkmn::e_game::HEARTGOLD, true);
    EXPECT_GT(locations_hgss.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_hgss, "Route 221"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "Distortion World"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "PC Tokyo"));

    locations_hgss = pkmn::database::get_location_name_list(pkmn::e_game::HEARTGOLD, false);
    EXPECT_GT(locations_hgss.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_hgss, "Route 221"));
    EXPECT_FALSE(string_in_vector(locations_hgss, "Distortion World"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "Sinjoh Ruins"));
    EXPECT_TRUE(string_in_vector(locations_hgss, "PC Tokyo"));

    /*
     * Black/White
     */
    locations_bw = pkmn::database::get_location_name_list(pkmn::e_game::WHITE, true);
    EXPECT_GT(locations_bw.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_bw, "Cold Storage"));
    EXPECT_TRUE(string_in_vector(locations_bw, "PWT"));
    EXPECT_TRUE(string_in_vector(locations_bw, "Castelia Sewers"));
    EXPECT_TRUE(string_in_vector(locations_bw, "PC Tokyo"));

    locations_bw = pkmn::database::get_location_name_list(pkmn::e_game::WHITE, false);
    EXPECT_GT(locations_bw.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_bw, "Cold Storage"));
    EXPECT_FALSE(string_in_vector(locations_bw, "PWT"));
    EXPECT_FALSE(string_in_vector(locations_bw, "Castelia Sewers"));
    EXPECT_TRUE(string_in_vector(locations_bw, "PC Tokyo"));

    /*
     * Black 2/White 2
     */
    locations_b2w2 = pkmn::database::get_location_name_list(pkmn::e_game::WHITE2, true);
    EXPECT_GT(locations_b2w2.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_b2w2, "Cold Storage"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "PWT"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "Castelia Sewers"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "PC Tokyo"));

    locations_b2w2 = pkmn::database::get_location_name_list(pkmn::e_game::WHITE2, false);
    EXPECT_GT(locations_b2w2.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_b2w2, "Cold Storage"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "PWT"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "Castelia Sewers"));
    EXPECT_TRUE(string_in_vector(locations_b2w2, "PC Tokyo"));

    /*
     * X/Y
     */
    locations_xy = pkmn::database::get_location_name_list(pkmn::e_game::X, true);
    EXPECT_GT(locations_xy.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_xy, "Zubat Roost"));
    EXPECT_TRUE(string_in_vector(locations_xy, "Route 101"));
    EXPECT_TRUE(string_in_vector(locations_xy, "PC Tokyo"));

    locations_xy = pkmn::database::get_location_name_list(pkmn::e_game::X, false);
    EXPECT_GT(locations_xy.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_xy, "Zubat Roost"));
    EXPECT_FALSE(string_in_vector(locations_xy, "Route 101"));
    EXPECT_TRUE(string_in_vector(locations_xy, "PC Tokyo"));

    /*
     * Omega Ruby/Alpha Sapphire
     */
    locations_oras = pkmn::database::get_location_name_list(pkmn::e_game::OMEGA_RUBY, true);
    EXPECT_GT(locations_oras.size(), 0);
    EXPECT_TRUE(string_in_vector(locations_oras, "Zubat Roost"));
    EXPECT_TRUE(string_in_vector(locations_oras, "Route 101"));
    EXPECT_TRUE(string_in_vector(locations_oras, "PC Tokyo"));

    locations_oras = pkmn::database::get_location_name_list(pkmn::e_game::OMEGA_RUBY, false);
    EXPECT_GT(locations_oras.size(), 0);
    EXPECT_FALSE(string_in_vector(locations_oras, "Zubat Roost"));
    EXPECT_TRUE(string_in_vector(locations_oras, "Route 101"));
    EXPECT_TRUE(string_in_vector(locations_oras, "PC Tokyo"));
}

struct changed_move_names
{
    std::string old_name;
    std::string new_name;
};

static const std::vector<changed_move_names> GEN1_CHANGED_MOVE_NAMES =
{
    {"BubbleBeam",   "Bubble Beam"},
    {"DoubleSlap",   "Double Slap"},
    {"Hi Jump Kick", "High Jump Kick"},
    {"PoisonPowder", "Poison Powder"},
    {"Sand-Attack",  "Sand Attack"},
    {"Selfdestruct", "Self-Destruct"},
    {"SmokeScreen",  "Smokescreen"},
    {"Softboiled",   "Soft-Boiled"},
    {"SolarBeam",    "Solar Beam"},
    {"SonicBoom",    "Sonic Boom"},
    {"ThunderPunch", "Thunder Punch"},
    {"ThunderShock", "Thunder Shock"},
    {"ViceGrip",     "Vice Grip"}
};
static const std::vector<changed_move_names> GEN2_CHANGED_MOVE_NAMES =
{
    {"AncientPower", "Ancient Power"},
    {"DragonBreath", "Dragon Breath"},
    {"DynamicPunch", "Dynamic Punch"},
    {"ExtremeSpeed", "Extreme Speed"},
    {"Faint Attack", "Feint Attack"}
};
static const std::vector<changed_move_names> GEN3_CHANGED_MOVE_NAMES =
{
    {"FeatherDance", "Feather Dance"},
    {"GrassWhistle", "Grass Whistle"},
    {"SmellingSalt", "Smelling Salts"}
};

TEST(cpp_lists_test, move_lists_test)
{
    /*
     * Make sure invalid games fail.
     */
    EXPECT_THROW(
        (void)pkmn::database::get_move_list(pkmn::e_game::NONE);
    , std::invalid_argument);
    EXPECT_THROW(
        (void)pkmn::database::get_move_name_list(pkmn::e_game::NONE);
    , std::invalid_argument);

    /*
     * Check for list bounds.
     */
    struct move_lists_test_params
    {
        std::vector<pkmn::e_game> games;
        size_t expected_num_moves;
        std::pair<pkmn::e_move, pkmn::e_move> expected_enum_ends;
        std::pair<std::string, std::string> expected_string_ends;
    };
    static const std::vector<move_lists_test_params> ALL_TEST_PARAMS =
    {
        {
            pkmntest::GEN1_GAMES,
            165,
            {pkmn::e_move::POUND, pkmn::e_move::STRUGGLE},
            {"Pound", "Struggle"}
        },
        {
            pkmntest::GEN2_GAMES,
            251,
            {pkmn::e_move::POUND, pkmn::e_move::BEAT_UP},
            {"Pound", "Beat Up"}
        },
        {
            pkmntest::GBA_GAMES,
            354,
            {pkmn::e_move::POUND, pkmn::e_move::PSYCHO_BOOST},
            {"Pound", "Psycho Boost"}
        },
        {
            {pkmn::e_game::COLOSSEUM},
            355,
            {pkmn::e_move::POUND, pkmn::e_move::SHADOW_RUSH},
            {"Pound", "Shadow Rush"}
        },
        {
            {pkmn::e_game::XD},
            372,
            {pkmn::e_move::POUND, pkmn::e_move::SHADOW_SKY},
            {"Pound", "Shadow Sky"}
        },
        {
            pkmntest::GEN4_GAMES,
            467,
            {pkmn::e_move::POUND, pkmn::e_move::SHADOW_FORCE},
            {"Pound", "Shadow Force"}
        },
        {
            pkmntest::GEN5_GAMES,
            559,
            {pkmn::e_move::POUND, pkmn::e_move::FUSION_BOLT},
            {"Pound", "Fusion Bolt"}
        },
        {
            {
                pkmn::e_game::X,
                pkmn::e_game::Y
            },
            617,
            {pkmn::e_move::POUND, pkmn::e_move::LIGHT_OF_RUIN},
            {"Pound", "Light of Ruin"}
        },
        {
            {
                pkmn::e_game::OMEGA_RUBY,
                pkmn::e_game::ALPHA_SAPPHIRE
            },
            621,
            {pkmn::e_move::POUND, pkmn::e_move::HYPERSPACE_FURY},
            {"Pound", "Hyperspace Fury"}
        },
    };

    for(const move_lists_test_params& test_params: ALL_TEST_PARAMS)
    {
        pkmn::e_move expected_first_move_enum = test_params.expected_enum_ends.first;
        pkmn::e_move expected_last_move_enum  = test_params.expected_enum_ends.second;

        const std::string& expected_first_move_name = test_params.expected_string_ends.first;
        const std::string& expected_last_move_name  = test_params.expected_string_ends.second;

        for(pkmn::e_game game: test_params.games)
        {
            std::vector<pkmn::e_move> moves = pkmn::database::get_move_list(game);
            EXPECT_EQ(test_params.expected_num_moves, moves.size()) << pkmn::game_to_string(game);
            EXPECT_EQ(expected_first_move_enum, moves.front()) << pkmn::game_to_string(game);
            EXPECT_EQ(expected_last_move_enum, moves.back()) << pkmn::game_to_string(game);

            std::vector<std::string> move_names = pkmn::database::get_move_name_list(game);
            EXPECT_EQ(test_params.expected_num_moves, move_names.size()) << pkmn::game_to_string(game);
            EXPECT_EQ(expected_first_move_name, move_names.front()) << pkmn::game_to_string(game);
            EXPECT_EQ(expected_last_move_name, move_names.back()) << pkmn::game_to_string(game);

            ASSERT_EQ(moves.size(), move_names.size());
            for(size_t move_index = 0; move_index < moves.size(); ++move_index)
            {
                // TODO: check with string -> move since an enum can correspond
                //       to different spellings
            }
        }
    }

    /*
     * Check for changed spelling.
     */

    for(pkmn::e_game game: pkmntest::GEN1_GAMES)
    {
        std::vector<std::string> move_names = pkmn::database::get_move_name_list(game);

        // Make sure old move names appear.
        for(const auto& changed_move_names: GEN1_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
    }
    for(pkmn::e_game game: pkmntest::GEN2_GAMES)
    {
        std::vector<std::string> move_names = pkmn::database::get_move_name_list(game);

        // Make sure old move names appear.
        for(const auto& changed_move_names: GEN1_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
        for(const auto& changed_move_names: GEN2_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
    }
    for(pkmn::e_game game: pkmntest::GEN3_GAMES)
    {
        std::vector<std::string> move_names = pkmn::database::get_move_name_list(game);

        // Make sure old move names appear.
        for(const auto& changed_move_names: GEN1_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
        for(const auto& changed_move_names: GEN2_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
        for(const auto& changed_move_names: GEN3_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
    }
    for(pkmn::e_game game: pkmntest::GEN4_GAMES)
    {
        std::vector<std::string> move_names = pkmn::database::get_move_name_list(game);

        // Make sure old move names appear.
        for(const auto& changed_move_names: GEN1_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
        for(const auto& changed_move_names: GEN2_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
        for(const auto& changed_move_names: GEN3_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
    }
    for(pkmn::e_game game: pkmntest::GEN5_GAMES)
    {
        std::vector<std::string> move_names = pkmn::database::get_move_name_list(game);

        // Make sure new move names appear.
        for(const auto& changed_move_names: GEN1_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
        for(const auto& changed_move_names: GEN2_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
        for(const auto& changed_move_names: GEN3_CHANGED_MOVE_NAMES)
        {
            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
    }
    for(pkmn::e_game game: pkmntest::GEN6_GAMES)
    {
        std::vector<std::string> move_names = pkmn::database::get_move_name_list(game);

        // Make sure new move names appear.
        for(const auto& changed_move_names: GEN1_CHANGED_MOVE_NAMES)
        {
            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
        for(const auto& changed_move_names: GEN2_CHANGED_MOVE_NAMES)
        {
            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
        for(const auto& changed_move_names: GEN3_CHANGED_MOVE_NAMES)
        {
            EXPECT_FALSE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.old_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.old_name;

            EXPECT_TRUE(
                pkmn::does_vector_contain_value(
                    move_names,
                    changed_move_names.new_name
                )
            ) << pkmn::game_to_string(game) << " " << changed_move_names.new_name;
        }
    }
}

TEST(cpp_lists_test, region_lists_test)
{
    std::vector<std::string> regions = pkmn::database::get_region_name_list();
    EXPECT_EQ(7, regions.size());
    EXPECT_EQ("Kanto", regions.front());
    EXPECT_EQ("Orre", regions[3]);
    EXPECT_EQ("Kalos", regions.back());
}

TEST(cpp_lists_test, ribbon_lists_test) {
    // TODO: make sure function checks for bad generation
    //       test good cases
}

TEST(cpp_lists_test, super_training_medal_lists_test) {
    std::vector<std::string> super_training_medals =
        pkmn::database::get_super_training_medal_name_list();

    EXPECT_EQ(30, super_training_medals.size());
}

TEST(cpp_lists_test, type_lists_test)
{
    std::vector<std::string> types_gen1,
                             types_gen2, types_gba,
                             types_gcn,  types_gen4,
                             types_gen5, types_gen6;

    /*
     * Make sure invalid games fail.
     */
    EXPECT_THROW(
        (void)pkmn::database::get_type_list(pkmn::e_game::NONE);
    , std::invalid_argument);
    EXPECT_THROW(
        (void)pkmn::database::get_type_name_list(pkmn::e_game::NONE);
    , std::invalid_argument);

    struct type_lists_test_params
    {
        std::vector<pkmn::e_game> games;
        size_t expected_num_types;
        std::vector<pkmn::e_type> expected_types_present;
        std::vector<pkmn::e_type> expected_types_not_present;
        std::vector<std::string>  expected_type_names_present;
        std::vector<std::string>  expected_type_names_not_present;
    };
    static const std::vector<type_lists_test_params> ALL_TEST_PARAMS =
    {
        {
            pkmntest::GEN1_GAMES,
            15,
            {},
            {
                pkmn::e_type::DARK,
                pkmn::e_type::STEEL,
                pkmn::e_type::QUESTION_MARK,
                pkmn::e_type::SHADOW,
                pkmn::e_type::FAIRY
            },
            {},
            {
                "Dark",
                "Steel",
                "???",
                "Shadow",
                "Fairy"
            }
        },
        {
            pkmntest::GEN2_GAMES,
            18,
            {
                pkmn::e_type::DARK,
                pkmn::e_type::STEEL,
                pkmn::e_type::QUESTION_MARK,
            },
            {
                pkmn::e_type::SHADOW,
                pkmn::e_type::FAIRY
            },
            {
                "Dark",
                "Steel",
                "???",
            },
            {
                "Shadow",
                "Fairy"
            }
        },
        {
            pkmntest::GBA_GAMES,
            18,
            {
                pkmn::e_type::DARK,
                pkmn::e_type::STEEL,
                pkmn::e_type::QUESTION_MARK,
            },
            {
                pkmn::e_type::SHADOW,
                pkmn::e_type::FAIRY
            },
            {
                "Dark",
                "Steel",
                "???",
            },
            {
                "Shadow",
                "Fairy"
            }
        },
        {
            pkmntest::GCN_GAMES,
            19,
            {
                pkmn::e_type::DARK,
                pkmn::e_type::STEEL,
                pkmn::e_type::QUESTION_MARK,
                pkmn::e_type::SHADOW,
            },
            {
                pkmn::e_type::FAIRY
            },
            {
                "Dark",
                "Steel",
                "???",
                "Shadow",
            },
            {
                "Fairy"
            }
        },
        {
            pkmntest::GEN4_GAMES,
            18,
            {
                pkmn::e_type::DARK,
                pkmn::e_type::STEEL,
                pkmn::e_type::QUESTION_MARK,
            },
            {
                pkmn::e_type::SHADOW,
                pkmn::e_type::FAIRY
            },
            {
                "Dark",
                "Steel",
                "???",
            },
            {
                "Shadow",
                "Fairy"
            }
        },
        {
            pkmntest::GEN5_GAMES,
            17,
            {
                pkmn::e_type::DARK,
                pkmn::e_type::STEEL,
            },
            {
                pkmn::e_type::QUESTION_MARK,
                pkmn::e_type::SHADOW,
                pkmn::e_type::FAIRY
            },
            {
                "Dark",
                "Steel",
            },
            {
                "???",
                "Shadow",
                "Fairy"
            }
        },
        {
            pkmntest::GEN6_GAMES,
            18,
            {
                pkmn::e_type::DARK,
                pkmn::e_type::STEEL,
                pkmn::e_type::FAIRY
            },
            {
                pkmn::e_type::QUESTION_MARK,
                pkmn::e_type::SHADOW,
            },
            {
                "Dark",
                "Steel",
                "Fairy"
            },
            {
                "???",
                "Shadow",
            }
        },
    };

    for(const type_lists_test_params& test_params: ALL_TEST_PARAMS)
    {
        for(pkmn::e_game game: test_params.games)
        {
            std::vector<pkmn::e_type> types = pkmn::database::get_type_list(game);
            EXPECT_EQ(
                test_params.expected_num_types,
                types.size()
            ) << pkmn::game_to_string(game);
            for(pkmn::e_type type: test_params.expected_types_present)
            {
                EXPECT_TRUE(pkmn::does_vector_contain_value(types, type))
                    << pkmn::game_to_string(game) << " " << pkmn::type_to_string(type);
            }
            for(pkmn::e_type type: test_params.expected_types_not_present)
            {
                EXPECT_FALSE(pkmn::does_vector_contain_value(types, type))
                    << pkmn::game_to_string(game) << " " << pkmn::type_to_string(type);
            }

            std::vector<std::string> type_names = pkmn::database::get_type_name_list(game);
            EXPECT_EQ(
                test_params.expected_num_types,
                type_names.size()
            ) << pkmn::game_to_string(game);
            for(const std::string& type_name: test_params.expected_type_names_present)
            {
                EXPECT_TRUE(pkmn::does_vector_contain_value(type_names, type_name))
                    << pkmn::game_to_string(game) << " " << type_name;
            }
            for(const std::string& type_name: test_params.expected_type_names_not_present)
            {
                EXPECT_FALSE(pkmn::does_vector_contain_value(type_names, type_name))
                    << pkmn::game_to_string(game) << " " << type_name;
            }

            ASSERT_EQ(types.size(), type_names.size()) << pkmn::game_to_string(game);
            for(size_t type_index = 0; type_index < types.size(); ++type_index)
            {
                EXPECT_EQ(
                    type_names[type_index],
                    pkmn::type_to_string(types[type_index])
                ) << pkmn::game_to_string(game);
            }
        }
    }
}

/*
 * Machine tests
 */

typedef std::function<std::vector<pkmn::e_move>(pkmn::e_game)> get_machine_move_enum_list_fcn_t;
typedef std::function<std::vector<std::string>(pkmn::e_game)>  get_machine_move_name_list_fcn_t;

typedef std::tuple<
            std::vector<pkmn::e_game>,             // Games
            size_t,                                // List size
            std::pair<pkmn::e_move, pkmn::e_move>, // Expected beginning+end moves
            std::pair<std::string, std::string>,   // Expected beginning+end move names
            get_machine_move_enum_list_fcn_t,      // Enum function
            get_machine_move_name_list_fcn_t>      // Name function
        machine_move_lists_test_params_t;

class cpp_machine_move_lists_test: public ::testing::TestWithParam<machine_move_lists_test_params_t> {};

TEST_P(cpp_machine_move_lists_test, test_machine_move_lists)
{
    machine_move_lists_test_params_t test_params = GetParam();

    const std::vector<pkmn::e_game>& games = std::get<0>(test_params);
    size_t list_size                       = std::get<1>(test_params);

    pkmn::e_move expected_first_move = std::get<2>(test_params).first;
    pkmn::e_move expected_last_move  = std::get<2>(test_params).second;

    const std::string& expected_first_move_name = std::get<3>(test_params).first;
    const std::string& expected_last_move_name  = std::get<3>(test_params).second;

    const get_machine_move_enum_list_fcn_t& get_machine_move_enum_list_fcn = std::get<4>(test_params);
    const get_machine_move_name_list_fcn_t& get_machine_move_name_list_fcn = std::get<5>(test_params);

    for(pkmn::e_game game: games)
    {
        std::vector<pkmn::e_move> machine_moves = get_machine_move_enum_list_fcn(game);
        EXPECT_EQ(list_size, machine_moves.size()) << pkmn::game_to_string(game);
        EXPECT_EQ(expected_first_move, machine_moves.front())
            << pkmn::game_to_string(game) << ": "   << pkmn::move_to_string(expected_first_move)
                                          << " vs " << pkmn::move_to_string(machine_moves.front());

        EXPECT_EQ(expected_last_move, machine_moves.back())
            << pkmn::game_to_string(game) << ": "   << pkmn::move_to_string(expected_last_move)
                                          << " vs " << pkmn::move_to_string(machine_moves.back());

        std::vector<std::string> machine_move_names = get_machine_move_name_list_fcn(game);
        EXPECT_EQ(list_size, machine_move_names.size()) << pkmn::game_to_string(game);
        EXPECT_EQ(expected_first_move_name, machine_move_names.front()) << pkmn::game_to_string(game);
        EXPECT_EQ(expected_last_move_name, machine_move_names.back()) << pkmn::game_to_string(game);

        ASSERT_EQ(machine_moves.size(), machine_move_names.size());
        for(size_t move_index = 0; move_index < machine_moves.size(); ++move_index)
        {
            // TODO: test with name to enum since an enum can correspond to multiple
            // names
        }
    }
}

// TODO: move names should match what they were in a given game
static const std::vector<machine_move_lists_test_params_t> TM_LISTS_TEST_PARAMS =
{
    machine_move_lists_test_params_t(
        pkmntest::GEN1_GAMES,
        50,
        {pkmn::e_move::MEGA_PUNCH, pkmn::e_move::SUBSTITUTE},
        {"Mega Punch", "Substitute"},
        pkmn::database::get_tm_move_list,
        pkmn::database::get_tm_move_name_list
    ),
    machine_move_lists_test_params_t(
        pkmntest::GEN2_GAMES,
        50,
        {pkmn::e_move::DYNAMIC_PUNCH, pkmn::e_move::NIGHTMARE},
        {"Dynamic Punch", "Nightmare"},
        pkmn::database::get_tm_move_list,
        pkmn::database::get_tm_move_name_list
    ),
    machine_move_lists_test_params_t(
        pkmntest::GEN3_GAMES,
        50,
        {pkmn::e_move::FOCUS_PUNCH, pkmn::e_move::OVERHEAT},
        {"Focus Punch", "Overheat"},
        pkmn::database::get_tm_move_list,
        pkmn::database::get_tm_move_name_list
    ),
    machine_move_lists_test_params_t(
        pkmntest::GEN4_GAMES,
        92,
        {pkmn::e_move::FOCUS_PUNCH, pkmn::e_move::TRICK_ROOM},
        {"Focus Punch", "Trick Room"},
        pkmn::database::get_tm_move_list,
        pkmn::database::get_tm_move_name_list
    ),
    machine_move_lists_test_params_t(
        pkmntest::GEN5_GAMES,
        95,
        {pkmn::e_move::HONE_CLAWS, pkmn::e_move::SNARL},
        {"Hone Claws", "Snarl"},
        pkmn::database::get_tm_move_list,
        pkmn::database::get_tm_move_name_list
    ),
    machine_move_lists_test_params_t(
        pkmntest::GEN6_GAMES,
        100,
        {pkmn::e_move::HONE_CLAWS, pkmn::e_move::CONFIDE},
        {"Hone Claws", "Confide"},
        pkmn::database::get_tm_move_list,
        pkmn::database::get_tm_move_name_list
    ),
};

INSTANTIATE_TEST_CASE_P(
    tm_moves_list_test,
    cpp_machine_move_lists_test,
    ::testing::ValuesIn(TM_LISTS_TEST_PARAMS)
);

static const std::vector<machine_move_lists_test_params_t> HM_LISTS_TEST_PARAMS =
{
    machine_move_lists_test_params_t(
        pkmntest::GEN1_GAMES,
        5,
        {pkmn::e_move::CUT, pkmn::e_move::FLASH},
        {"Cut", "Flash"},
        pkmn::database::get_hm_move_list,
        pkmn::database::get_hm_move_name_list
    ),
    machine_move_lists_test_params_t(
        pkmntest::GEN2_GAMES,
        7,
        {pkmn::e_move::CUT, pkmn::e_move::WATERFALL},
        {"Cut", "Waterfall"},
        pkmn::database::get_hm_move_list,
        pkmn::database::get_hm_move_name_list
    ),
    machine_move_lists_test_params_t(
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
        },
        8,
        {pkmn::e_move::CUT, pkmn::e_move::DIVE},
        {"Cut", "Dive"},
        pkmn::database::get_hm_move_list,
        pkmn::database::get_hm_move_name_list
    ),
    machine_move_lists_test_params_t(
        {
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN,
        },
        7,
        {pkmn::e_move::CUT, pkmn::e_move::WATERFALL},
        {"Cut", "Waterfall"},
        pkmn::database::get_hm_move_list,
        pkmn::database::get_hm_move_name_list
    ),
    machine_move_lists_test_params_t(
        pkmntest::GEN4_GAMES,
        8,
        {pkmn::e_move::CUT, pkmn::e_move::ROCK_CLIMB},
        {"Cut", "Rock Climb"},
        pkmn::database::get_hm_move_list,
        pkmn::database::get_hm_move_name_list
    ),
    machine_move_lists_test_params_t(
        pkmntest::GEN5_GAMES,
        6,
        {pkmn::e_move::CUT, pkmn::e_move::DIVE},
        {"Cut", "Dive"},
        pkmn::database::get_hm_move_list,
        pkmn::database::get_hm_move_name_list
    ),
    machine_move_lists_test_params_t(
        {
            pkmn::e_game::X,
            pkmn::e_game::Y
        },
        5,
        {pkmn::e_move::CUT, pkmn::e_move::WATERFALL},
        {"Cut", "Waterfall"},
        pkmn::database::get_hm_move_list,
        pkmn::database::get_hm_move_name_list
    ),
    machine_move_lists_test_params_t(
        {
            pkmn::e_game::OMEGA_RUBY,
            pkmn::e_game::ALPHA_SAPPHIRE
        },
        7,
        {pkmn::e_move::CUT, pkmn::e_move::DIVE},
        {"Cut", "Dive"},
        pkmn::database::get_hm_move_list,
        pkmn::database::get_hm_move_name_list
    ),
};

INSTANTIATE_TEST_CASE_P(
    hm_move_lists_test,
    cpp_machine_move_lists_test,
    ::testing::ValuesIn(HM_LISTS_TEST_PARAMS)
);

TEST(cpp_lists_test, test_machine_special_cases)
{
    // Between X/Y and OR/AS, TM94 was changed from Rock Smash to Secret Power,
    // due to Rock Smash becoming an HM.
    for(pkmn::e_game xy_game: {pkmn::e_game::X, pkmn::e_game::Y})
    {
        std::vector<pkmn::e_move> xy_tm_move_list = pkmn::database::get_tm_move_list(xy_game);
        EXPECT_EQ(pkmn::e_move::ROCK_SMASH, xy_tm_move_list[93]);

        std::vector<std::string> xy_tm_move_name_list = pkmn::database::get_tm_move_name_list(xy_game);
        EXPECT_EQ("Rock Smash", xy_tm_move_name_list[93]);
    }
    for(pkmn::e_game oras_game: {pkmn::e_game::OMEGA_RUBY, pkmn::e_game::ALPHA_SAPPHIRE})
    {
        std::vector<pkmn::e_move> oras_tm_move_list = pkmn::database::get_tm_move_list(oras_game);
        EXPECT_EQ(pkmn::e_move::SECRET_POWER, oras_tm_move_list[93]);

        std::vector<std::string> oras_tm_move_name_list = pkmn::database::get_tm_move_name_list(oras_game);
        EXPECT_EQ("Secret Power", oras_tm_move_name_list[93]);
    }

    // HMs were not present in the Gamecube games.
    for(pkmn::e_game gamecube_game: {pkmn::e_game::COLOSSEUM, pkmn::e_game::XD})
    {
        EXPECT_THROW(
            pkmn::database::get_hm_move_list(gamecube_game);
        , pkmn::feature_not_in_game_error);
        EXPECT_THROW(
            pkmn::database::get_hm_move_name_list(gamecube_game);
        , pkmn::feature_not_in_game_error);
    }
}
