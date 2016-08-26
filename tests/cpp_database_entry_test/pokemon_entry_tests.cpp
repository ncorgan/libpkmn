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

#include <boost/test/unit_test.hpp>

static void _pokemon_entry_none_test(
    pkmn::database::pokemon_entry &none_entry
) {
    (void)none_entry;
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
    BOOST_CHECK_EQUAL(types6.first, "Normal");

    BOOST_CHECK_EQUAL(types5.second, "None");
    BOOST_CHECK_EQUAL(types6.second, "Fairy");
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
    (void)pokemon_entry_gen1;
    (void)pokemon_entry_gen2;
    (void)pokemon_entry_gba;
    (void)pokemon_entry_gcn;
    (void)pokemon_entry_gen4;
    (void)pokemon_entry_gen5;
    (void)pokemon_entry_gen6;
}

BOOST_AUTO_TEST_CASE(pokemon_entry_test_main) {
}
