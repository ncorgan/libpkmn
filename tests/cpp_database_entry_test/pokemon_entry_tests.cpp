/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_database_entry_test.hpp"

#include <pkmn/database/pokemon_entry.hpp>

// Only create one main
#undef BOOST_TEST_MAIN

#include <boost/assign.hpp>
#include <boost/test/unit_test.hpp>

static const std::pair<std::string, std::string> none_pair("None", "None");
static const std::map<std::string, int> bad_stat_map_old = boost::assign::map_list_of
    ("HP", 0)("Attack", 0)("Defense", 0)
    ("Speed", 0)("Special", 0)
;
static const std::map<std::string, int> bad_stat_map = boost::assign::map_list_of
    ("HP", 0)("Attack", 0)("Defense", 0)
    ("Speed", 0)("Special Attack", 0)("Special Defense", 0)
;

static void _pokemon_entry_none_test(
    pkmn::database::pokemon_entry &none_entry
) {
    BOOST_CHECK_EQUAL(none_entry.get_name(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_species(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_pokedex_entry(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_form(), "None");
    BOOST_CHECK_CLOSE(none_entry.get_height(), -1.0f, 0.0001f);
    BOOST_CHECK_CLOSE(none_entry.get_weight(), -1.0f, 0.0001f);
    BOOST_CHECK_CLOSE(none_entry.get_chance_male(), -1.0f, 0.0001f);
    BOOST_CHECK_CLOSE(none_entry.get_chance_female(), -1.0f, 0.0001f);
    BOOST_CHECK(not none_entry.has_gender_differences());
    BOOST_CHECK_EQUAL(none_entry.get_base_happiness(), -1.0f);
    BOOST_CHECK(none_entry.get_types() == none_pair);
    BOOST_CHECK(none_entry.get_abilities() == none_pair);
    BOOST_CHECK_EQUAL(none_entry.get_hidden_ability(), "None");
    BOOST_CHECK(none_entry.get_egg_groups() == none_pair);
    if(none_entry.get_game_id() == 1) {
        BOOST_CHECK(none_entry.get_base_stats() == bad_stat_map_old);
        BOOST_CHECK(none_entry.get_EV_yields() == bad_stat_map_old);
    } else if(none_entry.get_game_id() == 4) {
        BOOST_CHECK(none_entry.get_base_stats() == bad_stat_map);
        BOOST_CHECK(none_entry.get_EV_yields() == bad_stat_map_old);
    } else {
        BOOST_CHECK(none_entry.get_base_stats() == bad_stat_map);
        BOOST_CHECK(none_entry.get_EV_yields() == bad_stat_map);
    }
    BOOST_CHECK_EQUAL(none_entry.get_experience_yield(), -1);
    BOOST_CHECK_EQUAL(none_entry.get_experience_at_level(2), -1);
    BOOST_CHECK_EQUAL(none_entry.get_level_at_experience(2), -1);
    BOOST_CHECK_EQUAL(none_entry.get_levelup_moves().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_tm_hm_moves().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_egg_moves().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_forms().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_evolutions().size(), 0);
}

/*
 * Make sure "None" move entries output expected results for all generations.
 */
BOOST_AUTO_TEST_CASE(pokemon_entry_none_test) {
    for(int i = 0; i < 6; ++i) {
        pkmn::database::pokemon_entry none_byindex(0, game_ids[i]);
        pkmn::database::pokemon_entry none_byname("None", game_names[i], "");

        _pokemon_entry_none_test(none_byindex);
        _pokemon_entry_none_test(none_byname);
    }
}

/*
 * Make sure Pokémon entries can't be instantiated for games that didn't
 * have the given move.
 */
BOOST_AUTO_TEST_CASE(pokemon_entry_wrong_game_test) {
    // Pokémon from later generations
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry snubbull(
            "Snubbull", "Yellow", ""
        );
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry ralts(
            "Ralts", "Silver", ""
        );
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry budew(
            "Budew", "Sapphire", ""
        );
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry liepard(
            "Liepard", "Pearl", ""
        );
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry espurr(
            "Espurr", "White", ""
        );
    , std::invalid_argument);

    /*
     * Make sure forms can't be used in the wrong games, even if
     * the Pokémon existed.
     */
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry arceus(
            "Arceus", "Diamond", "Fairy"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus1(
        "Arceus", "Y", "Fairy"
    );
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry arceus(
            "Arceus", "Black", "???"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus2(
        "Arceus", "Pearl", "???"
    );
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry pichu(
            "Pichu", "Silver", "Spiky-eared"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry pichu(
        "Pichu", "HeartGold", "Spiky-eared"
    );
    BOOST_CHECK_THROW(
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
BOOST_AUTO_TEST_CASE(changing_type_test) {
    pkmn::database::pokemon_entry jigglypuff5(
        "Jigglypuff", "Black", ""
    );
    pkmn::database::pokemon_entry jigglypuff6(
        "Jigglypuff", "Y", ""
    );

    std::pair<std::string, std::string> types5 = jigglypuff5.get_types();
    std::pair<std::string, std::string> types6 = jigglypuff6.get_types();

    BOOST_CHECK_EQUAL(types5.first, "Normal");
    BOOST_CHECK_EQUAL(types5.second, "None");

    BOOST_CHECK_EQUAL(types6.first, "Normal");
    BOOST_CHECK_EQUAL(types6.second, "Fairy");
}

static void _check_eevee_evolutions(
    const pkmn::database::pokemon_entries_t &evolutions,
    int generation
) {
    BOOST_CHECK_EQUAL(evolutions.at(0).get_name(), "Vaporeon");
    BOOST_CHECK_EQUAL(evolutions.at(1).get_name(), "Jolteon");
    BOOST_CHECK_EQUAL(evolutions.at(2).get_name(), "Flareon");

    if(generation >= 2) {
        BOOST_CHECK_EQUAL(evolutions.at(3).get_name(), "Espeon");
        BOOST_CHECK_EQUAL(evolutions.at(4).get_name(), "Umbreon");
    } else {
        BOOST_CHECK_EQUAL(evolutions.size(), 3);
    }

    if(generation >= 4) {
        BOOST_CHECK_EQUAL(evolutions.at(5).get_name(), "Leafeon");
        BOOST_CHECK_EQUAL(evolutions.at(6).get_name(), "Glaceon");
    } else {
        BOOST_CHECK_EQUAL(evolutions.size(), 7);
    }

    if(generation >= 6) {
        BOOST_CHECK_EQUAL(evolutions.at(7).get_name(), "Sylveon");
    } else {
        BOOST_CHECK_EQUAL(evolutions.size(), 8);
    }
}

/*
 * Make sure the same Pokémon from different generations shows
 * the correct evolutions.
 */
BOOST_AUTO_TEST_CASE(right_evolutions_test) {
    pkmn::database::pokemon_entry eevee1(
        "Eevee", "Blue", ""
    );
    pkmn::database::pokemon_entry eevee2(
        "Eevee", "Crystal", ""
    );
    pkmn::database::pokemon_entry eevee3(
        "Eevee", "Colosseum", ""
    );
    pkmn::database::pokemon_entry eevee4(
        "Eevee", "SoulSilver", ""
    );
    pkmn::database::pokemon_entry eevee5(
        "Eevee", "White 2", ""
    );
    pkmn::database::pokemon_entry eevee6(
        "Eevee", "Alpha Sapphire", ""
    );

    _check_eevee_evolutions(
        eevee1.get_evolutions(), 1
    );
    _check_eevee_evolutions(
        eevee2.get_evolutions(), 2
    );
    _check_eevee_evolutions(
        eevee3.get_evolutions(), 3
    );
    _check_eevee_evolutions(
        eevee4.get_evolutions(), 4
    );
    _check_eevee_evolutions(
        eevee5.get_evolutions(), 5
    );
    _check_eevee_evolutions(
        eevee6.get_evolutions(), 6
    );
}

static void _pokemon_entry_test_main(
    pkmn::database::pokemon_entry &pokemon_entry_gen1,
    pkmn::database::pokemon_entry &pokemon_entry_gen2,
    pkmn::database::pokemon_entry &pokemon_entry_gba,
    pkmn::database::pokemon_entry &pokemon_entry_gcn,
    pkmn::database::pokemon_entry &pokemon_entry_gen4,
    pkmn::database::pokemon_entry &pokemon_entry_gen5,
    pkmn::database::pokemon_entry &pokemon_entry_gen6
) {
    // Generation I entry
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_name(), "Articuno");
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_game(), "Yellow");
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_species(), "Freeze");
    (void)pokemon_entry_gen1.get_pokedex_entry();
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_form(), "Standard");
    BOOST_CHECK_CLOSE(pokemon_entry_gen1.get_height(), 1.7f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen1.get_weight(), 55.4f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen1.get_chance_male(), 0.0f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen1.get_chance_female(), 0.0f, 0.0001f);
    BOOST_CHECK(not pokemon_entry_gen1.has_gender_differences());
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_base_happiness(), -1);

    std::pair<std::string, std::string> types1 = pokemon_entry_gen1.get_types();
    BOOST_CHECK_EQUAL(types1.first, "Ice");
    BOOST_CHECK_EQUAL(types1.second, "Flying");

    BOOST_CHECK(pokemon_entry_gen1.get_abilities() == none_pair);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_hidden_ability(), "None");
    BOOST_CHECK(pokemon_entry_gen1.get_egg_groups() == none_pair);

    std::map<std::string, int> base_stats1 = pokemon_entry_gen1.get_base_stats();
    BOOST_CHECK_EQUAL(base_stats1.at("HP"), 90);
    BOOST_CHECK_EQUAL(base_stats1.at("Attack"), 85);
    BOOST_CHECK_EQUAL(base_stats1.at("Defense"), 100);
    BOOST_CHECK_EQUAL(base_stats1.at("Speed"), 85);
    BOOST_CHECK_EQUAL(base_stats1.at("Special"), 125);

    BOOST_CHECK(pokemon_entry_gen1.get_EV_yields() == base_stats1);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_experience_yield(), 215);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_experience_at_level(50), 156250);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_level_at_experience(200000), 54);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_levelup_moves().size(), 5);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_tm_hm_moves().size(), 20);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_egg_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_forms().size(), 1);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_evolutions().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_species_id(), 144);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_pokemon_id(), 144);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_form_id(), 144);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_pokemon_index(), 74);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_game_id(), 3);

    // Generation II entry
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_name(), "Feraligatr");
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_game(), "Gold");
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_species(), "Big Jaw");
    (void)pokemon_entry_gen2.get_pokedex_entry();
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_form(), "Standard");
    BOOST_CHECK_CLOSE(pokemon_entry_gen2.get_height(), 2.3f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen2.get_weight(), 88.8f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen2.get_chance_male(), 0.875f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen2.get_chance_female(), 0.125f, 0.0001f);
    BOOST_CHECK(not pokemon_entry_gen2.has_gender_differences());
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_base_happiness(), 70);

    std::pair<std::string, std::string> types2 = pokemon_entry_gen2.get_types();
    BOOST_CHECK_EQUAL(types2.first, "Water");
    BOOST_CHECK_EQUAL(types2.second, "None");

    BOOST_CHECK(pokemon_entry_gen2.get_abilities() == none_pair);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_hidden_ability(), "None");

    std::pair<std::string, std::string> egg_groups2 = pokemon_entry_gen2.get_egg_groups();
    BOOST_CHECK_EQUAL(egg_groups2.first, "Monster");
    BOOST_CHECK_EQUAL(egg_groups2.second, "Water 1");

    std::map<std::string, int> base_stats2 = pokemon_entry_gen2.get_base_stats();
    BOOST_CHECK_EQUAL(base_stats2.at("HP"), 85);
    BOOST_CHECK_EQUAL(base_stats2.at("Attack"), 105);
    BOOST_CHECK_EQUAL(base_stats2.at("Defense"), 100);
    BOOST_CHECK_EQUAL(base_stats2.at("Speed"), 78);
    BOOST_CHECK_EQUAL(base_stats2.at("Special Attack"), 79);
    BOOST_CHECK_EQUAL(base_stats2.at("Special Defense"), 83);

    std::map<std::string, int> EV_yields2 = pokemon_entry_gen2.get_EV_yields();
    BOOST_CHECK_EQUAL(EV_yields2.at("HP"), 85);
    BOOST_CHECK_EQUAL(EV_yields2.at("Attack"), 105);
    BOOST_CHECK_EQUAL(EV_yields2.at("Defense"), 100);
    BOOST_CHECK_EQUAL(EV_yields2.at("Speed"), 78);
    BOOST_CHECK_EQUAL(EV_yields2.at("Special"), 79);

    (void)pokemon_entry_gba;
    (void)pokemon_entry_gcn;
    (void)pokemon_entry_gen4;
    (void)pokemon_entry_gen5;
    (void)pokemon_entry_gen6;
}

BOOST_AUTO_TEST_CASE(pokemon_entry_test_main) {
    // Test with Pokémon entries created by name
    pkmn::database::pokemon_entry byname_gen1(
        "Articuno", "Yellow", ""
    );
    pkmn::database::pokemon_entry byname_gen2(
        "Feraligatr", "Gold", ""
    );
    pkmn::database::pokemon_entry byname_gba(
        "Regice", "Emerald", ""
    );
    pkmn::database::pokemon_entry byname_gcn(
        "Castform", "Colosseum", ""
    );
    pkmn::database::pokemon_entry byname_gen4(
        "Heatran", "Pearl", ""
    );
    pkmn::database::pokemon_entry byname_gen5(
        "Eelektross", "Black 2", ""
    );
    pkmn::database::pokemon_entry byname_gen6(
        "Sylveon", "X", ""
    );

    _pokemon_entry_test_main(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );
}
