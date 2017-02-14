/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/database/move_entry.hpp>

#include <gtest/gtest.h>

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

class move_entry_none_test: public ::testing::TestWithParam<std::string> {};

TEST_P(move_entry_none_test, move_entry_none_test) {
    pkmn::database::move_entry none_entry("None", GetParam());

    EXPECT_EQ("None", none_entry.get_name());
    EXPECT_EQ("None", none_entry.get_type());
    EXPECT_EQ("None", none_entry.get_description());
    EXPECT_EQ("None", none_entry.get_target());
    EXPECT_EQ("None", none_entry.get_damage_class());
    EXPECT_EQ(-1, none_entry.get_base_power());
    EXPECT_EQ(-1, none_entry.get_pp(0));
    EXPECT_FLOAT_EQ(-1.0f, none_entry.get_accuracy());
    EXPECT_EQ(-9, none_entry.get_priority());
    EXPECT_EQ("None", none_entry.get_effect());
    EXPECT_EQ("None", none_entry.get_contest_type());
    EXPECT_EQ("None", none_entry.get_contest_effect());
    EXPECT_EQ("None", none_entry.get_super_contest_effect());
}

INSTANTIATE_TEST_CASE_P(
    cpp_move_entry_none_test,
    move_entry_none_test,
    ::testing::ValuesIn(games)
);

class move_entry_test: public ::testing::Test {
    public:
        static void SetUpTestCase() {
            byid_gen1   = pkmn::database::move_entry(89,1);
            byid_gen2   = pkmn::database::move_entry(190,5);
            byid_gba    = pkmn::database::move_entry(262,7);
            byid_gcn    = pkmn::database::move_entry(10003,20);
            byid_gen4   = pkmn::database::move_entry(418,13);
            byid_gen5   = pkmn::database::move_entry(525,17);
            byid_gen6   = pkmn::database::move_entry(609,24);
            byname_gen1 = pkmn::database::move_entry("Earthquake", "Red");
            byname_gen2 = pkmn::database::move_entry("Octazooka", "Silver");
            byname_gba  = pkmn::database::move_entry("Memento", "Ruby");
            byname_gcn  = pkmn::database::move_entry("Shadow Blitz", "XD");
            byname_gen4 = pkmn::database::move_entry("Bullet Punch", "Pearl");
            byname_gen5 = pkmn::database::move_entry("Dragon Tail", "Black");
            byname_gen6 = pkmn::database::move_entry("Nuzzle", "Y");
        }

        static pkmn::database::move_entry byid_gen1, byname_gen1;
        static pkmn::database::move_entry byid_gen2, byname_gen2;
        static pkmn::database::move_entry byid_gba,  byname_gba;
        static pkmn::database::move_entry byid_gcn,  byname_gcn;
        static pkmn::database::move_entry byid_gen4, byname_gen4;
        static pkmn::database::move_entry byid_gen5, byname_gen5;
        static pkmn::database::move_entry byid_gen6, byname_gen6;
};

pkmn::database::move_entry move_entry_test::byid_gen1;
pkmn::database::move_entry move_entry_test::byid_gen2;
pkmn::database::move_entry move_entry_test::byid_gba;
pkmn::database::move_entry move_entry_test::byid_gcn;
pkmn::database::move_entry move_entry_test::byid_gen4;
pkmn::database::move_entry move_entry_test::byid_gen5;
pkmn::database::move_entry move_entry_test::byid_gen6;
pkmn::database::move_entry move_entry_test::byname_gen1;
pkmn::database::move_entry move_entry_test::byname_gen2;
pkmn::database::move_entry move_entry_test::byname_gba;
pkmn::database::move_entry move_entry_test::byname_gcn;
pkmn::database::move_entry move_entry_test::byname_gen4;
pkmn::database::move_entry move_entry_test::byname_gen5;
pkmn::database::move_entry move_entry_test::byname_gen6;

TEST_F(move_entry_test, equality_test) {
    EXPECT_TRUE(byid_gen1 == byname_gen1);
    EXPECT_TRUE(byid_gen2 == byname_gen2);
    EXPECT_TRUE(byid_gba  == byname_gba);
    EXPECT_TRUE(byid_gcn  == byname_gcn);
    EXPECT_TRUE(byid_gen4 == byname_gen4);
    EXPECT_TRUE(byid_gen5 == byname_gen5);
    EXPECT_TRUE(byid_gen6 == byname_gen6);

    EXPECT_TRUE(byid_gen1 != byname_gen2);
    EXPECT_TRUE(byid_gen2 != byname_gba);
    EXPECT_TRUE(byid_gba  != byname_gcn);
    EXPECT_TRUE(byid_gcn  != byname_gen4);
    EXPECT_TRUE(byid_gen4 != byname_gen5);
    EXPECT_TRUE(byid_gen5 != byname_gen6);
}

static void _move_entry_test(
    const pkmn::database::move_entry &move_entry_gen1,
    const pkmn::database::move_entry &move_entry_gen2,
    const pkmn::database::move_entry &move_entry_gba,
    const pkmn::database::move_entry &move_entry_gcn,
    const pkmn::database::move_entry &move_entry_gen4,
    const pkmn::database::move_entry &move_entry_gen5,
    const pkmn::database::move_entry &move_entry_gen6
) {
    // Check for expected exceptions in get_pp()
    EXPECT_THROW(
        (void)move_entry_gen1.get_pp(-1)
    , std::out_of_range);
    EXPECT_THROW(
        (void)move_entry_gen1.get_pp(4)
    , std::out_of_range);

    // Generation I entry
    EXPECT_EQ("Earthquake", move_entry_gen1.get_name());
    EXPECT_EQ("Red", move_entry_gen1.get_game());
    EXPECT_EQ("Ground", move_entry_gen1.get_type());
    (void)move_entry_gen1.get_description();
    EXPECT_EQ("All other Pokémon", move_entry_gen1.get_target());
    EXPECT_EQ("Physical", move_entry_gen1.get_damage_class());
    EXPECT_EQ(100, move_entry_gen1.get_base_power());
    EXPECT_EQ(10, move_entry_gen1.get_pp(0));
    EXPECT_EQ(12, move_entry_gen1.get_pp(1));
    EXPECT_EQ(14, move_entry_gen1.get_pp(2));
    EXPECT_EQ(16, move_entry_gen1.get_pp(3));
    EXPECT_FLOAT_EQ(1.0f, move_entry_gen1.get_accuracy());
    EXPECT_EQ(0, move_entry_gen1.get_priority());
    (void)move_entry_gen1.get_effect();
    EXPECT_EQ("None", move_entry_gen1.get_contest_type());
    EXPECT_EQ("None", move_entry_gen1.get_contest_effect());
    EXPECT_EQ("None", move_entry_gen1.get_super_contest_effect());

    // Generation II entry
    EXPECT_EQ("Octazooka", move_entry_gen2.get_name());
    EXPECT_EQ("Silver", move_entry_gen2.get_game());
    EXPECT_EQ("Water", move_entry_gen2.get_type());
    (void)move_entry_gen2.get_description();
    EXPECT_EQ("Selected Pokémon", move_entry_gen2.get_target());
    EXPECT_EQ("Special", move_entry_gen2.get_damage_class());
    EXPECT_EQ(65, move_entry_gen2.get_base_power());
    EXPECT_EQ(10, move_entry_gen2.get_pp(0));
    EXPECT_EQ(12, move_entry_gen2.get_pp(1));
    EXPECT_EQ(14, move_entry_gen2.get_pp(2));
    EXPECT_EQ(16, move_entry_gen2.get_pp(3));
    EXPECT_FLOAT_EQ(0.85f, move_entry_gen2.get_accuracy());
    EXPECT_EQ(0, move_entry_gen2.get_priority());
    (void)move_entry_gen2.get_effect();
    EXPECT_EQ("None", move_entry_gen2.get_contest_type());
    EXPECT_EQ("None", move_entry_gen2.get_contest_effect());
    EXPECT_EQ("None", move_entry_gen2.get_super_contest_effect());

    // GBA entry
    EXPECT_EQ("Memento", move_entry_gba.get_name());
    EXPECT_EQ("Ruby", move_entry_gba.get_game());
    EXPECT_EQ("Dark", move_entry_gba.get_type());
    (void)move_entry_gba.get_description();
    EXPECT_EQ("Selected Pokémon", move_entry_gba.get_target());
    EXPECT_EQ("Status", move_entry_gba.get_damage_class());
    EXPECT_EQ(0, move_entry_gba.get_base_power());
    EXPECT_EQ(10, move_entry_gba.get_pp(0));
    EXPECT_EQ(12, move_entry_gba.get_pp(1));
    EXPECT_EQ(14, move_entry_gba.get_pp(2));
    EXPECT_EQ(16, move_entry_gba.get_pp(3));
    EXPECT_FLOAT_EQ(1.0f, move_entry_gba.get_accuracy());
    EXPECT_EQ(0, move_entry_gba.get_priority());
    (void)move_entry_gba.get_effect();
    EXPECT_EQ("Tough", move_entry_gba.get_contest_type());
    EXPECT_NE("None", move_entry_gba.get_contest_effect());
    EXPECT_EQ("None", move_entry_gba.get_super_contest_effect());

    // Gamecube entry
    EXPECT_EQ("Shadow Blitz", move_entry_gcn.get_name());
    EXPECT_EQ("XD", move_entry_gcn.get_game());
    EXPECT_EQ("Shadow", move_entry_gcn.get_type());
    (void)move_entry_gcn.get_description();
    EXPECT_EQ("Selected Pokémon", move_entry_gcn.get_target());
    EXPECT_EQ("Physical", move_entry_gcn.get_damage_class());
    EXPECT_EQ(40, move_entry_gcn.get_base_power());
    EXPECT_EQ(0, move_entry_gcn.get_pp(0));
    EXPECT_EQ(0, move_entry_gcn.get_pp(1));
    EXPECT_EQ(0, move_entry_gcn.get_pp(2));
    EXPECT_EQ(0, move_entry_gcn.get_pp(3));
    EXPECT_FLOAT_EQ(1.0f, move_entry_gcn.get_accuracy());
    EXPECT_EQ(0, move_entry_gcn.get_priority());
    (void)move_entry_gcn.get_effect();
    EXPECT_EQ("None", move_entry_gcn.get_contest_type());
    EXPECT_EQ("None", move_entry_gcn.get_contest_effect());
    EXPECT_EQ("None", move_entry_gcn.get_super_contest_effect());

    // Generation IV entry
    EXPECT_EQ("Bullet Punch", move_entry_gen4.get_name());
    EXPECT_EQ("Pearl", move_entry_gen4.get_game());
    EXPECT_EQ("Steel", move_entry_gen4.get_type());
    (void)move_entry_gen4.get_description();
    EXPECT_EQ("Selected Pokémon", move_entry_gen4.get_target());
    EXPECT_EQ("Physical", move_entry_gen4.get_damage_class());
    EXPECT_EQ(40, move_entry_gen4.get_base_power());
    EXPECT_EQ(30, move_entry_gen4.get_pp(0));
    EXPECT_EQ(36, move_entry_gen4.get_pp(1));
    EXPECT_EQ(42, move_entry_gen4.get_pp(2));
    EXPECT_EQ(48, move_entry_gen4.get_pp(3));
    EXPECT_FLOAT_EQ(1.0f, move_entry_gen4.get_accuracy());
    EXPECT_EQ(1, move_entry_gen4.get_priority());
    (void)move_entry_gen4.get_effect();
    EXPECT_EQ("Smart", move_entry_gen4.get_contest_type());
    EXPECT_EQ( "None", move_entry_gen4.get_contest_effect());
    EXPECT_NE("None", move_entry_gen4.get_super_contest_effect());

    // Generation V entry
    EXPECT_EQ("Dragon Tail", move_entry_gen5.get_name());
    EXPECT_EQ("Black", move_entry_gen5.get_game());
    EXPECT_EQ("Dragon", move_entry_gen5.get_type());
    (void)move_entry_gen5.get_description();
    EXPECT_EQ("Selected Pokémon", move_entry_gen5.get_target());
    EXPECT_EQ("Physical", move_entry_gen5.get_damage_class());
    EXPECT_EQ(60, move_entry_gen5.get_base_power());
    EXPECT_EQ(10, move_entry_gen5.get_pp(0));
    EXPECT_EQ(12, move_entry_gen5.get_pp(1));
    EXPECT_EQ(14, move_entry_gen5.get_pp(2));
    EXPECT_EQ(16, move_entry_gen5.get_pp(3));
    EXPECT_FLOAT_EQ(0.9f, move_entry_gen5.get_accuracy());
    EXPECT_EQ(-6, move_entry_gen5.get_priority());
    (void)move_entry_gen5.get_effect();
    EXPECT_EQ("None", move_entry_gen5.get_contest_type());
    EXPECT_EQ("None", move_entry_gen5.get_contest_effect());
    EXPECT_EQ("None", move_entry_gen5.get_super_contest_effect());

    // Generation VI entry
    EXPECT_EQ("Nuzzle", move_entry_gen6.get_name());
    EXPECT_EQ("Y", move_entry_gen6.get_game());
    EXPECT_EQ("Electric", move_entry_gen6.get_type());
    (void)move_entry_gen6.get_description();
    EXPECT_EQ("Selected Pokémon", move_entry_gen6.get_target());
    EXPECT_EQ("Physical", move_entry_gen6.get_damage_class());
    EXPECT_EQ(20, move_entry_gen6.get_base_power());
    EXPECT_EQ(20, move_entry_gen6.get_pp(0));
    EXPECT_EQ(24, move_entry_gen6.get_pp(1));
    EXPECT_EQ(28, move_entry_gen6.get_pp(2));
    EXPECT_EQ(32, move_entry_gen6.get_pp(3));
    EXPECT_FLOAT_EQ(1.0f, move_entry_gen6.get_accuracy());
    EXPECT_EQ(0, move_entry_gen6.get_priority());
    (void)move_entry_gen6.get_effect();
    EXPECT_EQ("None", move_entry_gen6.get_contest_type());
    EXPECT_EQ("None", move_entry_gen6.get_contest_effect());
    EXPECT_EQ("None", move_entry_gen6.get_super_contest_effect());
}

TEST_F(move_entry_test, by_id_test) {
    _move_entry_test(
        byid_gen1, byid_gen2, byid_gba,
        byid_gcn, byid_gen4, byid_gen5,
        byid_gen6
    );
}

TEST_F(move_entry_test, by_name_test) {
    _move_entry_test(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );
}
