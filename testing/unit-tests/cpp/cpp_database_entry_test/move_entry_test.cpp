/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/util.hpp>

#include <pkmn/database/move_entry.hpp>
#include <pkmn/enums/enum_to_string.hpp>

#include <gtest/gtest.h>

class move_entry_none_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(move_entry_none_test, move_entry_none_test)
{
    pkmn::database::move_entry none_entry("None", GetParam());

    EXPECT_EQ(pkmn::e_move::NONE, none_entry.get_move());
    EXPECT_EQ("None", none_entry.get_name());
    EXPECT_EQ(pkmn::e_type::NONE, none_entry.get_type());
    EXPECT_EQ("None", none_entry.get_description());
    EXPECT_EQ(pkmn::e_move_target::NONE, none_entry.get_target());
    EXPECT_EQ(pkmn::e_move_damage_class::NONE, none_entry.get_damage_class());
    EXPECT_EQ(-1, none_entry.get_base_power());
    EXPECT_EQ(0, none_entry.get_pp(0));
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
    ::testing::ValuesIn(pkmntest::ALL_GAMES)
);

class move_entry_test: public ::testing::Test
{
    public:
        static void SetUpTestCase() {
            byid_gen1   = pkmn::database::move_entry(89,1);
            byid_gen2   = pkmn::database::move_entry(190,5);
            byid_gba    = pkmn::database::move_entry(262,7);
            byid_gcn    = pkmn::database::move_entry(10003,20);
            byid_gen4   = pkmn::database::move_entry(418,13);
            byid_gen5   = pkmn::database::move_entry(525,17);
            byid_gen6   = pkmn::database::move_entry(609,24);
            byname_gen1 = pkmn::database::move_entry("Earthquake", pkmn::e_game::RED);
            byname_gen2 = pkmn::database::move_entry("Octazooka", pkmn::e_game::SILVER);
            byname_gba  = pkmn::database::move_entry("Memento", pkmn::e_game::RUBY);
            byname_gcn  = pkmn::database::move_entry("Shadow Blitz", pkmn::e_game::XD);
            byname_gen4 = pkmn::database::move_entry("Bullet Punch", pkmn::e_game::PEARL);
            byname_gen5 = pkmn::database::move_entry("Dragon Tail", pkmn::e_game::BLACK);
            byname_gen6 = pkmn::database::move_entry("Nuzzle", pkmn::e_game::Y);
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

TEST_F(move_entry_test, equality_test)
{
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
)
{
    // Check for expected exceptions in get_pp()
    EXPECT_THROW(
        (void)move_entry_gen1.get_pp(-1)
    , std::out_of_range);
    EXPECT_THROW(
        (void)move_entry_gen1.get_pp(4)
    , std::out_of_range);

    // Generation I entry
    EXPECT_EQ(pkmn::e_move::EARTHQUAKE, move_entry_gen1.get_move());
    EXPECT_EQ("Earthquake", move_entry_gen1.get_name());
    EXPECT_EQ(pkmn::e_game::RED, move_entry_gen1.get_game());
    EXPECT_EQ(pkmn::e_type::GROUND, move_entry_gen1.get_type());
    (void)move_entry_gen1.get_description();
    EXPECT_EQ(pkmn::e_move_target::ALL_OTHER_POKEMON, move_entry_gen1.get_target());
    EXPECT_EQ(pkmn::e_move_damage_class::PHYSICAL, move_entry_gen1.get_damage_class());
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
    EXPECT_EQ(pkmn::e_move::OCTAZOOKA, move_entry_gen2.get_move());
    EXPECT_EQ("Octazooka", move_entry_gen2.get_name());
    EXPECT_EQ(pkmn::e_game::SILVER, move_entry_gen2.get_game());
    EXPECT_EQ(pkmn::e_type::WATER, move_entry_gen2.get_type());
    (void)move_entry_gen2.get_description();
    EXPECT_EQ(pkmn::e_move_target::SELECTED_POKEMON, move_entry_gen2.get_target());
    EXPECT_EQ(pkmn::e_move_damage_class::SPECIAL, move_entry_gen2.get_damage_class());
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
    EXPECT_EQ(pkmn::e_move::MEMENTO, move_entry_gba.get_move());
    EXPECT_EQ("Memento", move_entry_gba.get_name());
    EXPECT_EQ(pkmn::e_game::RUBY, move_entry_gba.get_game());
    EXPECT_EQ(pkmn::e_type::DARK, move_entry_gba.get_type());
    (void)move_entry_gba.get_description();
    EXPECT_EQ(pkmn::e_move_target::SELECTED_POKEMON, move_entry_gba.get_target());
    EXPECT_EQ(pkmn::e_move_damage_class::STATUS, move_entry_gba.get_damage_class());
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
    EXPECT_EQ(pkmn::e_move::SHADOW_BLITZ, move_entry_gcn.get_move());
    EXPECT_EQ("Shadow Blitz", move_entry_gcn.get_name());
    EXPECT_EQ(pkmn::e_game::XD, move_entry_gcn.get_game());
    EXPECT_EQ(pkmn::e_type::SHADOW, move_entry_gcn.get_type());
    (void)move_entry_gcn.get_description();
    EXPECT_EQ(pkmn::e_move_target::SELECTED_POKEMON, move_entry_gcn.get_target());
    EXPECT_EQ(pkmn::e_move_damage_class::PHYSICAL, move_entry_gcn.get_damage_class());
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
    EXPECT_EQ(pkmn::e_move::BULLET_PUNCH, move_entry_gen4.get_move());
    EXPECT_EQ("Bullet Punch", move_entry_gen4.get_name());
    EXPECT_EQ(pkmn::e_game::PEARL, move_entry_gen4.get_game());
    EXPECT_EQ(pkmn::e_type::STEEL, move_entry_gen4.get_type());
    (void)move_entry_gen4.get_description();
    EXPECT_EQ(pkmn::e_move_target::SELECTED_POKEMON, move_entry_gen4.get_target());
    EXPECT_EQ(pkmn::e_move_damage_class::PHYSICAL, move_entry_gen4.get_damage_class());
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
    EXPECT_EQ(pkmn::e_move::DRAGON_TAIL, move_entry_gen5.get_move());
    EXPECT_EQ("Dragon Tail", move_entry_gen5.get_name());
    EXPECT_EQ(pkmn::e_game::BLACK, move_entry_gen5.get_game());
    EXPECT_EQ(pkmn::e_type::DRAGON, move_entry_gen5.get_type());
    (void)move_entry_gen5.get_description();
    EXPECT_EQ(pkmn::e_move_target::SELECTED_POKEMON, move_entry_gen5.get_target());
    EXPECT_EQ(pkmn::e_move_damage_class::PHYSICAL, move_entry_gen5.get_damage_class());
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
    EXPECT_EQ(pkmn::e_move::NUZZLE, move_entry_gen6.get_move());
    EXPECT_EQ("Nuzzle", move_entry_gen6.get_name());
    EXPECT_EQ(pkmn::e_game::Y, move_entry_gen6.get_game());
    EXPECT_EQ(pkmn::e_type::ELECTRIC, move_entry_gen6.get_type());
    (void)move_entry_gen6.get_description();
    EXPECT_EQ(pkmn::e_move_target::SELECTED_POKEMON, move_entry_gen6.get_target());
    EXPECT_EQ(pkmn::e_move_damage_class::PHYSICAL, move_entry_gen6.get_damage_class());
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

TEST_F(move_entry_test, by_id_test)
{
    _move_entry_test(
        byid_gen1, byid_gen2, byid_gba,
        byid_gcn, byid_gen4, byid_gen5,
        byid_gen6
    );
}

TEST_F(move_entry_test, by_name_test)
{
    _move_entry_test(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );
}

// Test fields that change between games/generations
// https://bulbapedia.bulbagarden.net/wiki/List_of_modified_moves
