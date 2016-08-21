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

static void _move_entry_none_test(
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

/*
 * Make sure "None" move entries output expected results for all generations.
 */
BOOST_AUTO_TEST_CASE(move_entry_none_test) {
    for(int i = 0; i < 6; ++i) {
        pkmn::database::move_entry none_byindex(0, game_ids[i]);
        pkmn::database::move_entry none_byname("None", game_names[i]);

        _move_entry_none_test(none_byindex);
        _move_entry_none_test(none_byname);
    }
}

/*
 * Make sure move entries can't be instantiated for games that didn't
 * have the given move.
 */
BOOST_AUTO_TEST_CASE(move_entry_wrong_game_test) {
    // Moves from later generations
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

    // Moves that were only in certain games within a generation
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
BOOST_AUTO_TEST_CASE(move_entry_different_name_test) {
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
BOOST_AUTO_TEST_CASE(move_entry_different_type_test) {
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

/*
 * Moves that always hit have their accuracy columns set to NULL, and
 * moves that always KO or do no damage have their power columns set to
 * NULL, so account for those.
 */
BOOST_AUTO_TEST_CASE(move_entry_null_columns_test) {
    pkmn::database::move_entry swift(
        "Swift", "X"
    );
    BOOST_CHECK_EQUAL(swift.get_accuracy(), 1.0f);

    pkmn::database::move_entry fissure(
        "Fissure", "X"
    );
    BOOST_CHECK_EQUAL(fissure.get_base_power(), 0);

    pkmn::database::move_entry swords_dance(
        "Swords Dance", "X"
    );
    BOOST_CHECK_EQUAL(swords_dance.get_base_power(), 0);
}

static void _move_entry_test_main(
    const pkmn::database::move_entry &move_entry_gen1,
    const pkmn::database::move_entry &move_entry_gen2,
    const pkmn::database::move_entry &move_entry_gba,
    const pkmn::database::move_entry &move_entry_gcn,
    const pkmn::database::move_entry &move_entry_gen4,
    const pkmn::database::move_entry &move_entry_gen5,
    const pkmn::database::move_entry &move_entry_gen6
) {
    // Generation I entry
    BOOST_CHECK_EQUAL(move_entry_gen1.get_name(), "Slash");
    BOOST_CHECK_EQUAL(move_entry_gen1.get_game(), "Red");
    BOOST_CHECK_EQUAL(move_entry_gen1.get_type(), "Normal");
    (void)move_entry_gen1.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen1.get_damage_class(), "Physical");

    // Generation II entry
    BOOST_CHECK_EQUAL(move_entry_gen2.get_name(), "Octazooka");
    BOOST_CHECK_EQUAL(move_entry_gen2.get_game(), "Silver");
    BOOST_CHECK_EQUAL(move_entry_gen2.get_type(), "Water");
    (void)move_entry_gen2.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen2.get_damage_class(), "Special");

    // GBA entry
    BOOST_CHECK_EQUAL(move_entry_gba.get_name(), "Memento");
    BOOST_CHECK_EQUAL(move_entry_gba.get_game(), "Ruby");
    BOOST_CHECK_EQUAL(move_entry_gba.get_type(), "Dark");
    (void)move_entry_gba.get_description();
    BOOST_CHECK_EQUAL(move_entry_gba.get_damage_class(), "Status");

    // Gamecube entry
    BOOST_CHECK_EQUAL(move_entry_gcn.get_name(), "Shadow Blitz");
    BOOST_CHECK_EQUAL(move_entry_gcn.get_game(), "XD");
    BOOST_CHECK_EQUAL(move_entry_gcn.get_type(), "Shadow");
    (void)move_entry_gcn.get_description();
    BOOST_CHECK_EQUAL(move_entry_gcn.get_damage_class(), "Physical");

    // Generation IV entry
    BOOST_CHECK_EQUAL(move_entry_gen4.get_name(), "Bullet Punch");
    BOOST_CHECK_EQUAL(move_entry_gen4.get_game(), "Pearl");
    BOOST_CHECK_EQUAL(move_entry_gen4.get_type(), "Steel");
    (void)move_entry_gen4.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen4.get_damage_class(), "Physical");

    // Generation V entry
    BOOST_CHECK_EQUAL(move_entry_gen5.get_name(), "Dragon Tail");
    BOOST_CHECK_EQUAL(move_entry_gen5.get_game(), "Black");
    BOOST_CHECK_EQUAL(move_entry_gen5.get_type(), "Dragon");
    (void)move_entry_gen5.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen5.get_damage_class(), "Physical");

    // Generation VI entry
    BOOST_CHECK_EQUAL(move_entry_gen6.get_name(), "Nuzzle");
    BOOST_CHECK_EQUAL(move_entry_gen6.get_game(), "Y");
    BOOST_CHECK_EQUAL(move_entry_gen6.get_type(), "Electric");
    (void)move_entry_gen6.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen6.get_damage_class(), "Physical");
}

BOOST_AUTO_TEST_CASE(move_entry_test_main) {
    /*
     * TODO:
     *  * Make sure moves from later generations throw exceptions
     *  * Make sure moves from same generation, wrong game throw exceptions
     *  * Check equality operator between equal entries above
     *  * Check inequality operator
     *  * Make sure different names correspond to same move
     */
    pkmn::database::move_entry byname_gen1("Slash", "Red");
    pkmn::database::move_entry byname_gen2("Octazooka", "Silver");
    pkmn::database::move_entry byname_gba("Memento", "Ruby");
    pkmn::database::move_entry byname_gcn("Shadow Blitz", "XD");
    pkmn::database::move_entry byname_gen4("Bullet Punch", "Pearl");
    pkmn::database::move_entry byname_gen5("Dragon Tail", "Black");
    pkmn::database::move_entry byname_gen6("Nuzzle", "Y");

    _move_entry_test_main(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );
}
