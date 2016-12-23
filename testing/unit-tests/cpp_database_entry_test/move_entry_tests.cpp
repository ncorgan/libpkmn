/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_database_entry_test.hpp"

// Only create one main
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

static void _move_entry_none_test(
    const pkmn::database::move_entry &none_entry
) {
    BOOST_CHECK_EQUAL(none_entry.get_name(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_type(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_description(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_target(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_damage_class(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_base_power(), -1);
    BOOST_CHECK_EQUAL(none_entry.get_pp(0), -1);
    BOOST_CHECK_CLOSE(none_entry.get_accuracy(), -1.0f, 0.0001f);
    BOOST_CHECK_EQUAL(none_entry.get_priority(), -9);
    BOOST_CHECK_EQUAL(none_entry.get_effect(), "None");
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

    BOOST_CHECK(doubleslap1 != doubleslap3);
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

/*
 * Make sure moves whose accuracy has changed between generations
 * show the correct accuracy for each generation.
 */
BOOST_AUTO_TEST_CASE(move_entry_changing_accuracy_test) {
    pkmn::database::move_entry poison_gas1(
        "Poison Gas", "Diamond"
    );
    BOOST_CHECK_CLOSE(poison_gas1.get_accuracy(), 0.55f, 0.0001f);
    pkmn::database::move_entry poison_gas2(
        "Poison Gas", "White"
    );
    BOOST_CHECK_CLOSE(poison_gas2.get_accuracy(), 0.8f, 0.0001f);
    pkmn::database::move_entry poison_gas3(
        "Poison Gas", "X"
    );
    BOOST_CHECK_CLOSE(poison_gas3.get_accuracy(), 0.9f, 0.0001f);
}

/*
 * Make sure moves whose base power has changed between generations
 * show the correct base power for each generation.
 */
BOOST_AUTO_TEST_CASE(move_entry_changing_base_power_test) {
    pkmn::database::move_entry selfdestruct1(
        "Self-Destruct", "Red"
    );
    BOOST_CHECK_EQUAL(selfdestruct1.get_base_power(), 130);
    pkmn::database::move_entry selfdestruct2(
        "Self-Destruct", "Ruby"
    );
    BOOST_CHECK_EQUAL(selfdestruct2.get_base_power(), 200);
    pkmn::database::move_entry selfdestruct3(
        "Self-Destruct", "Omega Ruby"
    );
    BOOST_CHECK_EQUAL(selfdestruct3.get_base_power(), 200);
}

/*
 * Make sure moves whose base PP has changed between generations
 * show the correct base PP for each generation.
 */
BOOST_AUTO_TEST_CASE(move_entry_changing_pp_test) {
    pkmn::database::move_entry absorb1(
        "Absorb", "LeafGreen"
    );
    BOOST_CHECK_EQUAL(absorb1.get_pp(0), 20);
    pkmn::database::move_entry absorb2(
        "Absorb", "Black"
    );
    BOOST_CHECK_EQUAL(absorb2.get_pp(0), 25);
    pkmn::database::move_entry absorb3(
        "Absorb", "X"
    );
    BOOST_CHECK_EQUAL(absorb3.get_pp(0), 25);
}

BOOST_AUTO_TEST_CASE(move_entry_changing_priority_test) {
    pkmn::database::move_entry roar1(
        "Roar", "Blue"
    );
    BOOST_CHECK_EQUAL(roar1.get_priority(), 0);
    pkmn::database::move_entry roar2(
        "Roar", "Crystal"
    );
    BOOST_CHECK_EQUAL(roar2.get_priority(), -1);
    pkmn::database::move_entry roar3(
        "Roar", "XD"
    );
    BOOST_CHECK_EQUAL(roar3.get_priority(), -5);
    pkmn::database::move_entry roar4(
        "Roar", "SoulSilver"
    );
    BOOST_CHECK_EQUAL(roar4.get_priority(), -1);
    pkmn::database::move_entry roar5(
        "Roar", "Y"
    );
    BOOST_CHECK_EQUAL(roar5.get_priority(), -6);
}

void move_entry_invalid_index_test() {
    pkmn::database::move_entry invalid(468, 15);
    BOOST_CHECK_EQUAL(invalid.get_name(), "Invalid (0x1D4)");
    BOOST_CHECK_EQUAL(invalid.get_type(), "Unknown");
    BOOST_CHECK_EQUAL(invalid.get_description(), "Invalid (0x1D4)");
    BOOST_CHECK_EQUAL(invalid.get_target(), "Unknown");
    BOOST_CHECK_EQUAL(invalid.get_damage_class(), "Unknown");
    BOOST_CHECK_EQUAL(invalid.get_base_power(), -1);
    BOOST_CHECK_EQUAL(invalid.get_pp(0), -1);
    BOOST_CHECK_EQUAL(invalid.get_pp(1), -1);
    BOOST_CHECK_EQUAL(invalid.get_pp(2), -1);
    BOOST_CHECK_EQUAL(invalid.get_pp(3), -1);
    BOOST_CHECK_CLOSE(invalid.get_accuracy(), -1.0f, 0.0001f);
    BOOST_CHECK_EQUAL(invalid.get_priority(), -9);
    BOOST_CHECK_EQUAL(invalid.get_effect(), "Unknown");
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
    BOOST_CHECK_CLOSE(swift.get_accuracy(), 1.0f, 0.0001f);

    pkmn::database::move_entry fissure(
        "Fissure", "X"
    );
    BOOST_CHECK_EQUAL(fissure.get_base_power(), 0);

    pkmn::database::move_entry swords_dance(
        "Swords Dance", "X"
    );
    BOOST_CHECK_EQUAL(swords_dance.get_base_power(), 0);
}

static void _move_entry_test_common(
    const pkmn::database::move_entry &move_entry_gen1,
    const pkmn::database::move_entry &move_entry_gen2,
    const pkmn::database::move_entry &move_entry_gba,
    const pkmn::database::move_entry &move_entry_gcn,
    const pkmn::database::move_entry &move_entry_gen4,
    const pkmn::database::move_entry &move_entry_gen5,
    const pkmn::database::move_entry &move_entry_gen6
) {
    // Check for expected exceptions in get_pp()
    BOOST_CHECK_THROW(
        (void)move_entry_gen1.get_pp(-1)
    , std::out_of_range);
    BOOST_CHECK_THROW(
        (void)move_entry_gen1.get_pp(4)
    , std::out_of_range);

    // Generation I entry
    BOOST_CHECK_EQUAL(move_entry_gen1.get_name(), "Earthquake");
    BOOST_CHECK_EQUAL(move_entry_gen1.get_game(), "Red");
    BOOST_CHECK_EQUAL(move_entry_gen1.get_type(), "Ground");
    (void)move_entry_gen1.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen1.get_target(), "All other Pokémon");
    BOOST_CHECK_EQUAL(move_entry_gen1.get_damage_class(), "Physical");
    BOOST_CHECK_EQUAL(move_entry_gen1.get_base_power(), 100);
    BOOST_CHECK_EQUAL(move_entry_gen1.get_pp(0), 10);
    BOOST_CHECK_EQUAL(move_entry_gen1.get_pp(1), 12);
    BOOST_CHECK_EQUAL(move_entry_gen1.get_pp(2), 14);
    BOOST_CHECK_EQUAL(move_entry_gen1.get_pp(3), 16);
    BOOST_CHECK_CLOSE(move_entry_gen1.get_accuracy(), 1.0f, 0.0001f);
    BOOST_CHECK_EQUAL(move_entry_gen1.get_priority(), 0);
    (void)move_entry_gen1.get_effect();
    BOOST_CHECK_EQUAL(move_entry_gen1.get_contest_type(), "None");
    BOOST_CHECK_EQUAL(move_entry_gen1.get_contest_effect(), "None");
    BOOST_CHECK_EQUAL(move_entry_gen1.get_super_contest_effect(), "None");

    // Generation II entry
    BOOST_CHECK_EQUAL(move_entry_gen2.get_name(), "Octazooka");
    BOOST_CHECK_EQUAL(move_entry_gen2.get_game(), "Silver");
    BOOST_CHECK_EQUAL(move_entry_gen2.get_type(), "Water");
    (void)move_entry_gen2.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen2.get_target(), "Selected Pokémon");
    BOOST_CHECK_EQUAL(move_entry_gen2.get_damage_class(), "Special");
    BOOST_CHECK_EQUAL(move_entry_gen2.get_base_power(), 65);
    BOOST_CHECK_EQUAL(move_entry_gen2.get_pp(0), 10);
    BOOST_CHECK_EQUAL(move_entry_gen2.get_pp(1), 12);
    BOOST_CHECK_EQUAL(move_entry_gen2.get_pp(2), 14);
    BOOST_CHECK_EQUAL(move_entry_gen2.get_pp(3), 16);
    BOOST_CHECK_CLOSE(move_entry_gen2.get_accuracy(), 0.85f, 0.0001f);
    BOOST_CHECK_EQUAL(move_entry_gen2.get_priority(), 0);
    (void)move_entry_gen2.get_effect();
    BOOST_CHECK_EQUAL(move_entry_gen2.get_contest_type(), "None");
    BOOST_CHECK_EQUAL(move_entry_gen2.get_contest_effect(), "None");
    BOOST_CHECK_EQUAL(move_entry_gen2.get_super_contest_effect(), "None");

    // GBA entry
    BOOST_CHECK_EQUAL(move_entry_gba.get_name(), "Memento");
    BOOST_CHECK_EQUAL(move_entry_gba.get_game(), "Ruby");
    BOOST_CHECK_EQUAL(move_entry_gba.get_type(), "Dark");
    (void)move_entry_gba.get_description();
    BOOST_CHECK_EQUAL(move_entry_gba.get_target(), "Selected Pokémon");
    BOOST_CHECK_EQUAL(move_entry_gba.get_damage_class(), "Status");
    BOOST_CHECK_EQUAL(move_entry_gba.get_base_power(), 0);
    BOOST_CHECK_EQUAL(move_entry_gba.get_pp(0), 10);
    BOOST_CHECK_EQUAL(move_entry_gba.get_pp(1), 12);
    BOOST_CHECK_EQUAL(move_entry_gba.get_pp(2), 14);
    BOOST_CHECK_EQUAL(move_entry_gba.get_pp(3), 16);
    BOOST_CHECK_CLOSE(move_entry_gba.get_accuracy(), 1.0f, 0.0001f);
    BOOST_CHECK_EQUAL(move_entry_gba.get_priority(), 0);
    (void)move_entry_gba.get_effect();
    BOOST_CHECK_EQUAL(move_entry_gba.get_contest_type(), "Tough");
    BOOST_CHECK(move_entry_gba.get_contest_effect() != "None");
    BOOST_CHECK_EQUAL(move_entry_gba.get_super_contest_effect(), "None");

    // Gamecube entry
    BOOST_CHECK_EQUAL(move_entry_gcn.get_name(), "Shadow Blitz");
    BOOST_CHECK_EQUAL(move_entry_gcn.get_game(), "XD");
    BOOST_CHECK_EQUAL(move_entry_gcn.get_type(), "Shadow");
    (void)move_entry_gcn.get_description();
    BOOST_CHECK_EQUAL(move_entry_gcn.get_target(), "Selected Pokémon");
    BOOST_CHECK_EQUAL(move_entry_gcn.get_damage_class(), "Physical");
    BOOST_CHECK_EQUAL(move_entry_gcn.get_base_power(), 40);
    BOOST_CHECK_EQUAL(move_entry_gcn.get_pp(0), 0);
    BOOST_CHECK_EQUAL(move_entry_gcn.get_pp(1), 0);
    BOOST_CHECK_EQUAL(move_entry_gcn.get_pp(2), 0);
    BOOST_CHECK_EQUAL(move_entry_gcn.get_pp(3), 0);
    BOOST_CHECK_CLOSE(move_entry_gcn.get_accuracy(), 1.0f, 0.0001f);
    BOOST_CHECK_EQUAL(move_entry_gcn.get_priority(), 0);
    (void)move_entry_gcn.get_effect();
    BOOST_CHECK_EQUAL(move_entry_gcn.get_contest_type(), "None");
    BOOST_CHECK_EQUAL(move_entry_gcn.get_contest_effect(), "None");
    BOOST_CHECK_EQUAL(move_entry_gcn.get_super_contest_effect(), "None");

    // Generation IV entry
    BOOST_CHECK_EQUAL(move_entry_gen4.get_name(), "Bullet Punch");
    BOOST_CHECK_EQUAL(move_entry_gen4.get_game(), "Pearl");
    BOOST_CHECK_EQUAL(move_entry_gen4.get_type(), "Steel");
    (void)move_entry_gen4.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen4.get_target(), "Selected Pokémon");
    BOOST_CHECK_EQUAL(move_entry_gen4.get_damage_class(), "Physical");
    BOOST_CHECK_EQUAL(move_entry_gen4.get_base_power(), 40);
    BOOST_CHECK_EQUAL(move_entry_gen4.get_pp(0), 30);
    BOOST_CHECK_EQUAL(move_entry_gen4.get_pp(1), 36);
    BOOST_CHECK_EQUAL(move_entry_gen4.get_pp(2), 42);
    BOOST_CHECK_EQUAL(move_entry_gen4.get_pp(3), 48);
    BOOST_CHECK_CLOSE(move_entry_gen4.get_accuracy(), 1.0f, 0.0001f);
    BOOST_CHECK_EQUAL(move_entry_gen4.get_priority(), 1);
    (void)move_entry_gen4.get_effect();
    BOOST_CHECK_EQUAL(move_entry_gen4.get_contest_type(), "Smart");
    BOOST_CHECK_EQUAL(move_entry_gen4.get_contest_effect(),  "None");
    BOOST_CHECK(move_entry_gen4.get_super_contest_effect() != "None");

    // Generation V entry
    BOOST_CHECK_EQUAL(move_entry_gen5.get_name(), "Dragon Tail");
    BOOST_CHECK_EQUAL(move_entry_gen5.get_game(), "Black");
    BOOST_CHECK_EQUAL(move_entry_gen5.get_type(), "Dragon");
    (void)move_entry_gen5.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen5.get_target(), "Selected Pokémon");
    BOOST_CHECK_EQUAL(move_entry_gen5.get_damage_class(), "Physical");
    BOOST_CHECK_EQUAL(move_entry_gen5.get_base_power(), 60);
    BOOST_CHECK_EQUAL(move_entry_gen5.get_pp(0), 10);
    BOOST_CHECK_EQUAL(move_entry_gen5.get_pp(1), 12);
    BOOST_CHECK_EQUAL(move_entry_gen5.get_pp(2), 14);
    BOOST_CHECK_EQUAL(move_entry_gen5.get_pp(3), 16);
    BOOST_CHECK_CLOSE(move_entry_gen5.get_accuracy(), 0.9f, 0.0001f);
    BOOST_CHECK_EQUAL(move_entry_gen5.get_priority(), -6);
    (void)move_entry_gen5.get_effect();
    BOOST_CHECK_EQUAL(move_entry_gen5.get_contest_type(), "None");
    BOOST_CHECK_EQUAL(move_entry_gen5.get_contest_effect(), "None");
    BOOST_CHECK_EQUAL(move_entry_gen5.get_super_contest_effect(), "None");

    // Generation VI entry
    BOOST_CHECK_EQUAL(move_entry_gen6.get_name(), "Nuzzle");
    BOOST_CHECK_EQUAL(move_entry_gen6.get_game(), "Y");
    BOOST_CHECK_EQUAL(move_entry_gen6.get_type(), "Electric");
    (void)move_entry_gen6.get_description();
    BOOST_CHECK_EQUAL(move_entry_gen6.get_target(), "Selected Pokémon");
    BOOST_CHECK_EQUAL(move_entry_gen6.get_damage_class(), "Physical");
    BOOST_CHECK_EQUAL(move_entry_gen6.get_base_power(), 20);
    BOOST_CHECK_EQUAL(move_entry_gen6.get_pp(0), 20);
    BOOST_CHECK_EQUAL(move_entry_gen6.get_pp(1), 24);
    BOOST_CHECK_EQUAL(move_entry_gen6.get_pp(2), 28);
    BOOST_CHECK_EQUAL(move_entry_gen6.get_pp(3), 32);
    BOOST_CHECK_CLOSE(move_entry_gen6.get_accuracy(), 1.0f, 0.0001f);
    BOOST_CHECK_EQUAL(move_entry_gen6.get_priority(), 0);
    (void)move_entry_gen6.get_effect();
    BOOST_CHECK_EQUAL(move_entry_gen6.get_contest_type(), "None");
    BOOST_CHECK_EQUAL(move_entry_gen6.get_contest_effect(), "None");
    BOOST_CHECK_EQUAL(move_entry_gen6.get_super_contest_effect(), "None");
}

// Move entries created by ID
static pkmn::database::move_entry byid_gen1(89,1);
static pkmn::database::move_entry byid_gen2(190,5);
static pkmn::database::move_entry byid_gba(262,7);
static pkmn::database::move_entry byid_gcn(10003,20);
static pkmn::database::move_entry byid_gen4(418,13);
static pkmn::database::move_entry byid_gen5(525,17);
static pkmn::database::move_entry byid_gen6(609,24);

// Move entries created by name
static pkmn::database::move_entry byname_gen1("Earthquake", "Red");
static pkmn::database::move_entry byname_gen2("Octazooka", "Silver");
static pkmn::database::move_entry byname_gba("Memento", "Ruby");
static pkmn::database::move_entry byname_gcn("Shadow Blitz", "XD");
static pkmn::database::move_entry byname_gen4("Bullet Punch", "Pearl");
static pkmn::database::move_entry byname_gen5("Dragon Tail", "Black");
static pkmn::database::move_entry byname_gen6("Nuzzle", "Y");

BOOST_AUTO_TEST_CASE(move_entry_equality) {
    BOOST_CHECK(byid_gen1 == byname_gen1);
    BOOST_CHECK(byid_gen2 == byname_gen2);
    BOOST_CHECK(byid_gba  == byname_gba);
    BOOST_CHECK(byid_gcn  == byname_gcn);
    BOOST_CHECK(byid_gen4 == byname_gen4);
    BOOST_CHECK(byid_gen5 == byname_gen5);
    BOOST_CHECK(byid_gen6 == byname_gen6);

    BOOST_CHECK(byid_gen1 != byname_gen2);
    BOOST_CHECK(byid_gen2 != byname_gba);
    BOOST_CHECK(byid_gba  != byname_gcn);
    BOOST_CHECK(byid_gcn  != byname_gen4);
    BOOST_CHECK(byid_gen4 != byname_gen5);
    BOOST_CHECK(byid_gen5 != byname_gen6);
}

BOOST_AUTO_TEST_CASE(move_entry_by_id_test) {
    _move_entry_test_common(
        byid_gen1, byid_gen2, byid_gba,
        byid_gcn, byid_gen4, byid_gen5,
        byid_gen6
    );
}

BOOST_AUTO_TEST_CASE(move_entry_by_name_test) {
    _move_entry_test_common(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );
}
