/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/enums/enum_to_string.hpp>

#include <pkmn/enums/ability.hpp>

#include <gtest/gtest.h>

#include <functional>
#include <iostream>
#include <tuple>

// Math operators can be applied to C-style enums, but not enum classes.
template <typename enum_class_type>
static inline void increment_enum_class(enum_class_type& r_value)
{
    r_value = static_cast<enum_class_type>(static_cast<int>(r_value) + 1);
}
template <typename enum_class_type>
static inline void decrement_enum_class(enum_class_type& r_value)
{
    r_value = static_cast<enum_class_type>(static_cast<int>(r_value) - 1);
}

// Templated test types and classes. Unfortunately, GTest can't combine
// parameterized and typed tests, so this must be done manually.

template <typename enum_type>
using enum_to_string_fcn_t = std::function<std::string(enum_type)>;

template <typename enum_type>
using string_to_enum_fcn_t = std::function<enum_type(const std::string&)>;

template <typename enum_type>
static void test_enum_string_conversions(
    enum_type min_valid_enum_value,
    enum_type max_valid_enum_value,
    const std::string& field_name,
    const enum_to_string_fcn_t<enum_type>& enum_to_string_fcn,
    const string_to_enum_fcn_t<enum_type>& string_to_enum_fcn
)
{
    // Test valid values.
    for(enum_type enum_value = min_valid_enum_value;
        enum_value <= max_valid_enum_value;
        increment_enum_class<enum_type>(enum_value))
    {
        const std::string enum_as_string = enum_to_string_fcn(enum_value);
        ASSERT_EQ(enum_value, string_to_enum_fcn(enum_as_string)) << enum_as_string;
    }

    // Make sure invalid enum class values result in desired errors. As the desired
    // exception type is the same as is used for an internal SQLite error, we also
    // need to check the exception's message.

    // Test an enum value below the valid range.
    enum_type enum_value_below_min = min_valid_enum_value;
    decrement_enum_class<enum_type>(enum_value_below_min);
    try
    {
        (void)enum_to_string_fcn(enum_value_below_min);
        FAIL() << "Did not throw when attempting to convert "
               << field_name << " " << static_cast<int>(enum_value_below_min);
    }
    catch(const std::invalid_argument& e)
    {
        std::string expected_error_message = "Invalid ";
        expected_error_message += field_name;
        expected_error_message += ": ";
        expected_error_message += std::to_string(static_cast<int>(enum_value_below_min));

        EXPECT_EQ(expected_error_message, e.what());
    }
    catch(...)
    {
        FAIL() << "Did not throw std::invalid_argument when attempting to convert"
               << field_name << " " << static_cast<int>(enum_value_below_min);
    }

    // Test an enum value below the valid range.
    enum_type enum_value_above_max = max_valid_enum_value;
    increment_enum_class<enum_type>(enum_value_above_max);
    try
    {
        (void)enum_to_string_fcn(enum_value_above_max);
        FAIL() << "Did not throw when attempting to convert "
               << field_name << " " << static_cast<int>(enum_value_above_max);
    }
    catch(const std::invalid_argument& e)
    {
        std::string expected_error_message = "Invalid ";
        expected_error_message += field_name;
        expected_error_message += ": ";
        expected_error_message += std::to_string(static_cast<int>(enum_value_above_max));

        EXPECT_EQ(expected_error_message, e.what());
    }
    catch(...)
    {
        FAIL() << "Did not throw std::invalid_argument when attempting to convert"
               << field_name << " " << static_cast<int>(enum_value_above_max);
    }

    // Test an invalid string.
    static const std::string invalid_string = "Invalid string";
    try
    {
        (void)string_to_enum_fcn(invalid_string);
        FAIL() << "Did not throw when attempting to convert "
               << field_name << " " << invalid_string;
    }
    catch(const std::invalid_argument& e)
    {
        std::string expected_error_message = "Invalid ";
        expected_error_message += field_name;
        expected_error_message += ": ";
        expected_error_message += invalid_string;

    }
    catch(...)
    {
        FAIL() << "Did not throw std::invalid_argument when attempting to convert"
               << field_name << " " << invalid_string;
    }
}

TEST(cpp_enum_test, test_ability_enum)
{
    static const pkmn::e_ability MIN_ABILITY_ENUM = pkmn::e_ability::NONE;
    static const pkmn::e_ability MAX_ABILITY_ENUM = pkmn::e_ability::DELTA_STREAM;

    test_enum_string_conversions<pkmn::e_ability>(
        MIN_ABILITY_ENUM,
        MAX_ABILITY_ENUM,
        "ability",
        pkmn::ability_to_string,
        pkmn::string_to_ability
    );
}

TEST(cpp_enum_test, test_ball_enum)
{
    static const pkmn::e_ball MIN_BALL_ENUM = pkmn::e_ball::NONE;
    static const pkmn::e_ball MAX_BALL_ENUM = pkmn::e_ball::DREAM_BALL;

    test_enum_string_conversions<pkmn::e_ball>(
        MIN_BALL_ENUM,
        MAX_BALL_ENUM,
        "ball",
        pkmn::ball_to_string,
        pkmn::string_to_ball
    );
}

TEST(cpp_enum_test, test_condition_enum)
{
    static const pkmn::e_condition MIN_CONDITION_ENUM = pkmn::e_condition::NONE;
    static const pkmn::e_condition MAX_CONDITION_ENUM = pkmn::e_condition::BAD_POISON;

    test_enum_string_conversions<pkmn::e_condition>(
        MIN_CONDITION_ENUM,
        MAX_CONDITION_ENUM,
        "condition",
        pkmn::condition_to_string,
        pkmn::string_to_condition
    );
}

TEST(cpp_enum_test, test_egg_group_enum)
{
    static const pkmn::e_egg_group MIN_EGG_GROUP_ENUM = pkmn::e_egg_group::NONE;
    static const pkmn::e_egg_group MAX_EGG_GROUP_ENUM = pkmn::e_egg_group::UNDISCOVERED;

    test_enum_string_conversions<pkmn::e_egg_group>(
        MIN_EGG_GROUP_ENUM,
        MAX_EGG_GROUP_ENUM,
        "egg group",
        pkmn::egg_group_to_string,
        pkmn::string_to_egg_group
    );
}

TEST(cpp_enum_test, test_game_enum)
{
    static const pkmn::e_game MIN_GAME_ENUM = pkmn::e_game::NONE;
    static const pkmn::e_game MAX_GAME_ENUM = pkmn::e_game::ALPHA_SAPPHIRE;

    test_enum_string_conversions<pkmn::e_game>(
        MIN_GAME_ENUM,
        MAX_GAME_ENUM,
        "game",
        pkmn::game_to_string,
        pkmn::string_to_game
    );
}

TEST(cpp_enum_test, test_gender_enum)
{
    static const pkmn::e_gender MIN_GENDER_ENUM = pkmn::e_gender::NONE;
    static const pkmn::e_gender MAX_GENDER_ENUM = pkmn::e_gender::GENDERLESS;

    test_enum_string_conversions<pkmn::e_gender>(
        MIN_GENDER_ENUM,
        MAX_GENDER_ENUM,
        "gender",
        pkmn::gender_to_string,
        pkmn::string_to_gender
    );
}

TEST(cpp_enum_test, test_item_enum)
{
    // TODO: with items moving into Veekun database, enums will change
    static const std::vector<std::pair<pkmn::e_item, pkmn::e_item>> ITEM_ENUM_GROUPS =
    {
        {pkmn::e_item::NONE,         pkmn::e_item::GRAM_3},
        {pkmn::e_item::DRAGON_GEM,   pkmn::e_item::TM100},
        {pkmn::e_item::GOLD_BERRY,   pkmn::e_item::MEGA_CUFF},
        {pkmn::e_item::SMALL_TABLET, pkmn::e_item::BATTLE_CD_60}
    };
    for(const std::pair<pkmn::e_item, pkmn::e_item>& item_enum_group: ITEM_ENUM_GROUPS)
    {
        test_enum_string_conversions<pkmn::e_item>(
            item_enum_group.first,
            item_enum_group.second,
            "item",
            pkmn::item_to_string,
            pkmn::string_to_item
        );
    }
}

TEST(cpp_enum_test, test_language_enum)
{
    static const pkmn::e_language MIN_LANGUAGE_ENUM = pkmn::e_language::NONE;
    static const pkmn::e_language MAX_LANGUAGE_ENUM = pkmn::e_language::KOREAN;

    test_enum_string_conversions<pkmn::e_language>(
        MIN_LANGUAGE_ENUM,
        MAX_LANGUAGE_ENUM,
        "language",
        pkmn::language_to_string,
        pkmn::string_to_language
    );
}

TEST(cpp_enum_test, test_move_damage_class_enum)
{
    static const pkmn::e_move_damage_class MIN_MOVE_DAMAGE_CLASS_ENUM = pkmn::e_move_damage_class::NONE;
    static const pkmn::e_move_damage_class MAX_MOVE_DAMAGE_CLASS_ENUM = pkmn::e_move_damage_class::SPECIAL;

    test_enum_string_conversions<pkmn::e_move_damage_class>(
        MIN_MOVE_DAMAGE_CLASS_ENUM,
        MAX_MOVE_DAMAGE_CLASS_ENUM,
        "move damage class",
        pkmn::move_damage_class_to_string,
        pkmn::string_to_move_damage_class
    );
}

TEST(cpp_enum_test, test_move_enum)
{
    // TODO: with moves moving into Veekun database, enums will change
    static const std::vector<std::pair<pkmn::e_move, pkmn::e_move>> MOVE_ENUM_GROUPS =
    {
        {pkmn::e_move::NONE,        pkmn::e_move::HYPERSPACE_FURY},
        {pkmn::e_move::SHADOW_RUSH, pkmn::e_move::SHADOW_SKY},
    };
    for(const std::pair<pkmn::e_move, pkmn::e_move>& move_enum_group: MOVE_ENUM_GROUPS)
    {
        test_enum_string_conversions<pkmn::e_move>(
            move_enum_group.first,
            move_enum_group.second,
            "move",
            pkmn::move_to_string,
            pkmn::string_to_move
        );
    }
}

TEST(cpp_enum_test, test_nature_enum)
{
    static const pkmn::e_nature MIN_NATURE_ENUM = pkmn::e_nature::NONE;
    static const pkmn::e_nature MAX_NATURE_ENUM = pkmn::e_nature::QUIRKY;

    test_enum_string_conversions<pkmn::e_nature>(
        MIN_NATURE_ENUM,
        MAX_NATURE_ENUM,
        "nature",
        pkmn::nature_to_string,
        pkmn::string_to_nature
    );
}

TEST(cpp_enum_test, test_species_enum)
{
    static const pkmn::e_species MIN_SPECIES_ENUM = pkmn::e_species::NONE;
    static const pkmn::e_species MAX_SPECIES_ENUM = pkmn::e_species::VOLCANION;

    test_enum_string_conversions<pkmn::e_species>(
        MIN_SPECIES_ENUM,
        MAX_SPECIES_ENUM,
        "species",
        pkmn::species_to_string,
        pkmn::string_to_species
    );
}

TEST(cpp_enum_test, test_stat_enum)
{
    static const pkmn::e_stat MIN_STAT_ENUM = pkmn::e_stat::NONE;
    static const pkmn::e_stat MAX_STAT_ENUM = pkmn::e_stat::SPECIAL_DEFENSE;

    test_enum_string_conversions<pkmn::e_stat>(
        MIN_STAT_ENUM,
        MAX_STAT_ENUM,
        "stat",
        pkmn::stat_to_string,
        pkmn::string_to_stat
    );
}

TEST(cpp_enum_test, test_type_enum)
{
    static const std::vector<pkmn::type_pair_t> TYPE_ENUM_GROUPS =
    {
        {pkmn::e_type::NONE,          pkmn::e_type::FAIRY},
        {pkmn::e_type::QUESTION_MARK, pkmn::e_type::SHADOW},
    };
    for(const pkmn::type_pair_t& type_enum_group: TYPE_ENUM_GROUPS)
    {
        test_enum_string_conversions<pkmn::e_type>(
            type_enum_group.first,
            type_enum_group.second,
            "type",
            pkmn::type_to_string,
            pkmn::string_to_type
        );
    }
}
