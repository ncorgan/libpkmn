/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_database_entry_test.hpp"

// Only create one main
#undef BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

void move_entry_none_test(
    const pkmn::database::move_entry &none_entry
) {
    BOOST_CHECK_EQUAL(none_entry.get_name(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_type(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_description(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_damage_class(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_base_power(), -1);
    BOOST_CHECK_EQUAL(none_entry.get_pp(0), -1);
    BOOST_CHECK_CLOSE(none_entry.get_accuracy(), -1.0f, 0.0001f);
    BOOST_CHECK_EQUAL(none_entry.get_priority(), -9);
    BOOST_CHECK_EQUAL(none_entry.get_effect(), "None");
    BOOST_CHECK_CLOSE(none_entry.get_effect_chance(), -1.0f, 0.0001f);
    BOOST_CHECK_EQUAL(none_entry.get_contest_type(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_contest_effect(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_super_contest_effect(), "None");
}

void move_entry_wrong_game_test() {
    /*
     * Make sure moves from later generations throw an
     * exception.
     */
    BOOST_CHECK_THROW(
        pkmn::database::move_entry sketch(
            "Sketch", "Red"
        )
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::move_entry uproar(
            "Uproar", "Silver"
        )
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::move_entry roost(
            "Roost", "Emerald"
        )
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::move_entry hone_claws(
            "Hone Claws", "Pearl"
        )
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::move_entry flying_press(
            "Flying Press", "White"
        )
    , std::invalid_argument);

    /*
     * Make sure moves from different games in a given generation
     * throw an exception.
     */
    BOOST_CHECK_THROW(
        pkmn::database::move_entry shadow_rush(
            "Shadow Rush", "Ruby"
        )
    , std::invalid_argument);
    pkmn::database::move_entry shadow_rush(
        "Shadow Rush", "XD"
    );

    BOOST_CHECK_THROW(
        pkmn::database::move_entry shadow_bolt(
            "Shadow Bolt", "Colosseum"
        )
    , std::invalid_argument);
    pkmn::database::move_entry shadow_bolt(
        "Shadow Bolt", "XD"
    );

    BOOST_CHECK_THROW(
        pkmn::database::move_entry hyperspace_fury(
            "Hyperspace Fury", "X"
        )
    , std::invalid_argument);
    pkmn::database::move_entry hyperspace_fury(
        "Hyperspace Fury", "Alpha Sapphire"
    );
}

/*
 * Make sure move entries can be created from any name
 * the move has ever had, even from different games.
 */
void move_entry_different_name_test() {
    pkmn::database::move_entry doubleslap1(
        "DoubleSlap", "Red"
    );
    pkmn::database::move_entry doubleslap2(
        "Double Slap", "Red"
    );
    BOOST_CHECK(doubleslap1 == doubleslap2);
    BOOST_CHECK_EQUAL(doubleslap1.get_name(), "DoubleSlap");
    BOOST_CHECK_EQUAL(doubleslap2.get_name(), "DoubleSlap");

    pkmn::database::move_entry doubleslap3(
        "DoubleSlap", "X"
    );
    pkmn::database::move_entry doubleslap4(
        "Double Slap", "X"
    );
    BOOST_CHECK(doubleslap3 == doubleslap4);
    BOOST_CHECK_EQUAL(doubleslap3.get_name(), "Double Slap");
    BOOST_CHECK_EQUAL(doubleslap4.get_name(), "Double Slap");
}

/*
 * Make sure moves whose types have changed between
 * generations are reflected in their entries.
 */
void move_entry_different_type_test() {
    pkmn::database::move_entry bite1(
        "Bite", "Red"
    );
    pkmn::database::move_entry bite2(
        "Bite", "Silver"
    );
    BOOST_CHECK_EQUAL(bite1.get_type(), "Normal");
    BOOST_CHECK_EQUAL(bite2.get_type(), "Dark");

    pkmn::database::move_entry charm1(
        "Charm", "Black 2"
    );
    pkmn::database::move_entry charm2(
        "Charm", "X"
    );
    BOOST_CHECK_EQUAL(charm1.get_type(), "Normal");
    BOOST_CHECK_EQUAL(charm2.get_type(), "Fairy");
}

void move_entry_invalid_index_test() {
    pkmn::database::move_entry invalid(468, 15);
    BOOST_CHECK_EQUAL(invalid.get_name(), "Invalid (0x1D4)");
    BOOST_CHECK_EQUAL(invalid.get_type(), "Unknown");
    BOOST_CHECK_EQUAL(invalid.get_description(), "Invalid (0x1D4)");
    BOOST_CHECK_EQUAL(invalid.get_damage_class(), "Unknown");
    BOOST_CHECK_EQUAL(invalid.get_base_power(), -1);
    BOOST_CHECK_EQUAL(invalid.get_pp(0), -1);
    BOOST_CHECK_EQUAL(invalid.get_accuracy(), -1.0f);
    BOOST_CHECK_EQUAL(invalid.get_priority(), -9);
    BOOST_CHECK_EQUAL(invalid.get_effect(), "Unknown");
    BOOST_CHECK_EQUAL(invalid.get_effect_chance(), -1.0f);
    BOOST_CHECK_EQUAL(invalid.get_contest_type(), "Unknown");
    BOOST_CHECK_EQUAL(invalid.get_contest_effect(), "Unknown");
    BOOST_CHECK_EQUAL(invalid.get_super_contest_effect(), "Unknown");
}

void move_entry_null_columns_test() {
}

void move_entry_test_common(
    const pkmn::database::move_entry &move_entry_gen1,
    const pkmn::database::move_entry &move_entry_gen2,
    const pkmn::database::move_entry &move_entry_gba,
    const pkmn::database::move_entry &move_entry_gcn,
    const pkmn::database::move_entry &move_entry_gen4,
    const pkmn::database::move_entry &move_entry_gen5,
    const pkmn::database::move_entry &move_entry_gen6
) {
    (void)move_entry_gen1;
    (void)move_entry_gen2;
    (void)move_entry_gba;
    (void)move_entry_gcn;
    (void)move_entry_gen4;
    (void)move_entry_gen5;
    (void)move_entry_gen6;
}
