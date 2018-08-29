/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pksav/pksav_call.hpp"

#include "c/cpp_to_c.hpp"
#include "c/error_internal.hpp"

#ifdef _MSC_VER
#    pragma warning(disable: 4273) // Inconsistent DLL linkage
#endif
extern "C" {
    const char* pkmn_strerror();
}

#include <pkmn-c/types.h>

#include <pksav.h>

#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/thread/mutex.hpp>

#include <stdexcept>
#include <string>

class pkmn_test_exception: public std::exception {
    public:
        explicit pkmn_test_exception(const char* msg):
            msg_(msg) {}

        explicit pkmn_test_exception(const std::string& msg):
            msg_(msg) {}

        virtual ~pkmn_test_exception() throw() {}

        virtual const char* what() const throw() {
            return msg_.c_str();
        }

    protected:
        std::string msg_;
};

#define BUFFER_LEN 1024

/*
 * Converting C++ exceptions to C error codes
 */

enum pkmn_error throw_nothing() {
    PKMN_CPP_TO_C()
}

enum pkmn_error throw_unknown() {
    PKMN_CPP_TO_C(
        throw 5;
    )
}

// The "enum" tag must be removed for MSVC to compile.
template <typename exception_type>
pkmn_error throw_exception(
    const std::string& msg
) {
    PKMN_CPP_TO_C(
        throw exception_type(msg);
    )
}

enum pkmn_error throw_feature_not_in_game_error(
    const std::string& feature,
    const std::string& game
) {
    PKMN_CPP_TO_C(
        throw pkmn::feature_not_in_game_error(
                  feature,
                  game
              );
    )
}

enum pkmn_error throw_pksav_error(
    pksav_error pksav_error_code
) {
    PKMN_CPP_TO_C(
        PKSAV_CALL(pksav_error_code)
    )
}

enum pkmn_error throw_pkmn_unimplemented_error() {
    PKMN_CPP_TO_C(
        throw pkmn::unimplemented_error();
    )
}

TEST(cpp_to_c_test, exception_to_error_code_test) {
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = throw_nothing();
    EXPECT_EQ(PKMN_ERROR_NONE, error);
    EXPECT_STREQ("None", pkmn_strerror());

    error = throw_exception<pkmn::feature_not_in_build_error>("Qt support");
    EXPECT_EQ(PKMN_ERROR_FEATURE_NOT_IN_BUILD_ERROR, error);
    EXPECT_STREQ("This feature is not part of this build of LibPKMN: Qt support", pkmn_strerror());

    error = throw_feature_not_in_game_error("Contests", "Generation I");
    EXPECT_EQ(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    EXPECT_STREQ("Contests not in Generation I", pkmn_strerror());

    error = throw_exception<pkmn::feature_not_in_game_error>("feature_not_in_game_error");
    EXPECT_EQ(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    EXPECT_STREQ("feature_not_in_game_error", pkmn_strerror());

    error = throw_pksav_error(PKSAV_ERROR_INVALID_SAVE);
    EXPECT_EQ(PKMN_ERROR_INTERNAL_ERROR, error);
    EXPECT_STREQ("Internal error: PKSav returned the following error: \"Invalid save file\"", pkmn_strerror());

    error = throw_pkmn_unimplemented_error();
    EXPECT_EQ(PKMN_ERROR_UNIMPLEMENTED_ERROR, error);
    EXPECT_STREQ("Currently unimplemented.", pkmn_strerror());

    error = throw_exception<std::invalid_argument>("invalid_argument");
    EXPECT_EQ(PKMN_ERROR_INVALID_ARGUMENT, error);
    EXPECT_STREQ("invalid_argument", pkmn_strerror());

    error = throw_exception<std::domain_error>("domain_error");
    EXPECT_EQ(PKMN_ERROR_DOMAIN_ERROR, error);
    EXPECT_STREQ("domain_error", pkmn_strerror());

    error = throw_exception<std::length_error>("length_error");
    EXPECT_EQ(PKMN_ERROR_LENGTH_ERROR, error);
    EXPECT_STREQ("length_error", pkmn_strerror());

    error = throw_exception<std::out_of_range>("out_of_range");
    EXPECT_EQ(PKMN_ERROR_OUT_OF_RANGE, error);
    EXPECT_STREQ("out_of_range", pkmn_strerror());

    error = throw_exception<std::logic_error>("logic_error");
    EXPECT_EQ(PKMN_ERROR_LOGIC_ERROR, error);
    EXPECT_STREQ("logic_error", pkmn_strerror());

    error = throw_exception<std::range_error>("range_error");
    EXPECT_EQ(PKMN_ERROR_RANGE_ERROR, error);
    EXPECT_STREQ("range_error", pkmn_strerror());

    error = throw_exception<std::overflow_error>("overflow_error");
    EXPECT_EQ(PKMN_ERROR_OVERFLOW_ERROR, error);
    EXPECT_STREQ("overflow_error", pkmn_strerror());

    error = throw_exception<std::underflow_error>("underflow_error");
    EXPECT_EQ(PKMN_ERROR_UNDERFLOW_ERROR, error);
    EXPECT_STREQ("underflow_error", pkmn_strerror());

    error = throw_exception<std::runtime_error>("runtime_error");
    EXPECT_EQ(PKMN_ERROR_RUNTIME_ERROR, error);
    EXPECT_STREQ("runtime_error", pkmn_strerror());

    error = throw_exception<pkmn_test_exception>("std_exception");
    EXPECT_EQ(PKMN_ERROR_STD_EXCEPTION, error);
    EXPECT_STREQ("std_exception", pkmn_strerror());

    error = throw_unknown();
    EXPECT_EQ(PKMN_ERROR_UNKNOWN_ERROR, error);
    EXPECT_STREQ("Unknown error", pkmn_strerror());
}

/*
 * Converting C++ exceptions to C error codes (with handle)
 */

typedef struct {
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_test_handle_t;

enum pkmn_error throw_nothing_with_handle(
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,)
}

enum pkmn_error throw_unknown_with_handle(
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw 5;
    )
}

// The "enum" tag must be removed for MSVC to compile.
template <typename exception_type>
pkmn_error throw_exception_with_handle(
    const std::string& msg,
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw exception_type(msg);
    )
}

enum pkmn_error throw_feature_not_in_game_error_with_handle(
    const std::string& feature,
    const std::string& game,
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw pkmn::feature_not_in_game_error(
                  feature,
                  game
              );
    )
}

enum pkmn_error throw_pksav_error_with_handle(
    pksav_error pksav_error_code,
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        PKSAV_CALL(pksav_error_code)
    )
}

enum pkmn_error throw_pkmn_unimplemented_error_with_handle(
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw pkmn::unimplemented_error();
    )
}

TEST(cpp_to_c_test, exception_to_error_code_with_handle_test) {
    enum pkmn_error error = PKMN_ERROR_NONE;
    pkmn_test_handle_t test_handle;

    error = throw_nothing_with_handle(&test_handle);
    EXPECT_EQ(PKMN_ERROR_NONE, error);
    EXPECT_EQ("None", test_handle.last_error);

    error = throw_exception_with_handle<pkmn::feature_not_in_build_error>(
        "Qt support", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_FEATURE_NOT_IN_BUILD_ERROR, error);
    EXPECT_EQ("This feature is not part of this build of LibPKMN: Qt support", test_handle.last_error);
    EXPECT_STREQ("This feature is not part of this build of LibPKMN: Qt support", pkmn_strerror());

    error = throw_feature_not_in_game_error_with_handle(
        "Contests", "Generation I", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    EXPECT_EQ("Contests not in Generation I", test_handle.last_error);
    EXPECT_STREQ("Contests not in Generation I", pkmn_strerror());

    error = throw_exception_with_handle<pkmn::feature_not_in_game_error>(
        "feature_not_in_game_error", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    EXPECT_EQ("feature_not_in_game_error", test_handle.last_error);
    EXPECT_STREQ("feature_not_in_game_error", pkmn_strerror());

    error = throw_pksav_error_with_handle(
        PKSAV_ERROR_INVALID_SAVE, &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_INTERNAL_ERROR, error);
    EXPECT_EQ("Internal error: PKSav returned the following error: \"Invalid save file\"", test_handle.last_error);
    EXPECT_STREQ("Internal error: PKSav returned the following error: \"Invalid save file\"", pkmn_strerror());

    error = throw_pkmn_unimplemented_error_with_handle(&test_handle);
    EXPECT_EQ(PKMN_ERROR_UNIMPLEMENTED_ERROR, error);
    EXPECT_EQ("Currently unimplemented.", test_handle.last_error);
    EXPECT_STREQ("Currently unimplemented.", pkmn_strerror());

    error = throw_exception_with_handle<std::invalid_argument>(
        "invalid_argument", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_INVALID_ARGUMENT, error);
    EXPECT_EQ("invalid_argument", test_handle.last_error);
    EXPECT_STREQ("invalid_argument", pkmn_strerror());

    error = throw_exception_with_handle<std::domain_error>(
        "domain_error", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_DOMAIN_ERROR, error);
    EXPECT_EQ("domain_error", test_handle.last_error);
    EXPECT_STREQ("domain_error", pkmn_strerror());

    error = throw_exception_with_handle<std::length_error>(
        "length_error", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_LENGTH_ERROR, error);
    EXPECT_EQ("length_error", test_handle.last_error);
    EXPECT_STREQ("length_error", pkmn_strerror());

    error = throw_exception_with_handle<std::out_of_range>(
        "out_of_range", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_OUT_OF_RANGE, error);
    EXPECT_EQ("out_of_range", test_handle.last_error);
    EXPECT_STREQ("out_of_range", pkmn_strerror());

    error = throw_exception_with_handle<std::logic_error>(
        "logic_error", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_LOGIC_ERROR, error);
    EXPECT_EQ("logic_error", test_handle.last_error);
    EXPECT_STREQ("logic_error", pkmn_strerror());

    error = throw_exception_with_handle<std::range_error>(
        "range_error", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_RANGE_ERROR, error);
    EXPECT_EQ("range_error", test_handle.last_error);
    EXPECT_STREQ("range_error", pkmn_strerror());

    error = throw_exception_with_handle<std::overflow_error>(
        "overflow_error", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_OVERFLOW_ERROR, error);
    EXPECT_EQ("overflow_error", test_handle.last_error);
    EXPECT_STREQ("overflow_error", pkmn_strerror());

    error = throw_exception_with_handle<std::underflow_error>(
        "underflow_error", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_UNDERFLOW_ERROR, error);
    EXPECT_EQ("underflow_error", test_handle.last_error);
    EXPECT_STREQ("underflow_error", pkmn_strerror());

    error = throw_exception_with_handle<std::runtime_error>(
        "runtime_error", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_RUNTIME_ERROR, error);
    EXPECT_EQ("runtime_error", test_handle.last_error);
    EXPECT_STREQ("runtime_error", pkmn_strerror());

    error = throw_exception_with_handle<pkmn_test_exception>(
        "std_exception", &test_handle
    );
    EXPECT_EQ(PKMN_ERROR_STD_EXCEPTION, error);
    EXPECT_EQ("std_exception", test_handle.last_error);
    EXPECT_STREQ("std_exception", pkmn_strerror());

    error = throw_unknown_with_handle(&test_handle);
    EXPECT_EQ(PKMN_ERROR_UNKNOWN_ERROR, error);
    EXPECT_EQ("Unknown error", test_handle.last_error);
    EXPECT_STREQ("Unknown error", pkmn_strerror());
}

/*
 * Converting C++ types to C types
 */

TEST(cpp_to_c_test, list_cpp_to_c_test)
{
    std::vector<int> int_vector = {1, 2, 3, 4, 5};
    std::vector<double> double_vector = {6.0, 7.0, 8.0};

    int int_list[BUFFER_LEN] = {0};
    double double_list[BUFFER_LEN] = {0.0};

    size_t output_size;

    // Int test

    pkmn::c::list_cpp_to_c(
        int_vector,
        int_list,
        int_vector.size() - 1, // Too short
        &output_size
    );
    EXPECT_EQ(int_vector.size(), output_size);
    for(size_t index = 0; index < int_vector.size(); ++index)
    {
        if(index == (int_vector.size() - 1))
        {
            EXPECT_EQ(0, int_list[index]);
        }
        else
        {
            EXPECT_EQ(int_vector[index], int_list[index]);
        }
    }

    std::memset(
        int_list,
        0,
        sizeof(int_list)
    );

    pkmn::c::list_cpp_to_c(
        int_vector,
        int_list,
        int_vector.size(),
        &output_size
    );
    EXPECT_EQ(int_vector.size(), output_size);
    for(size_t index = 0; index < int_vector.size(); ++index)
    {
        EXPECT_EQ(int_vector[index], int_list[index]);
    }

    // Double list

    pkmn::c::list_cpp_to_c(
        double_vector,
        double_list,
        double_vector.size() - 1, // Too short
        &output_size
    );
    EXPECT_EQ(double_vector.size(), output_size);
    for(size_t index = 0; index < double_vector.size(); ++index)
    {
        if(index == (double_vector.size() - 1))
        {
            EXPECT_DOUBLE_EQ(0.0, double_list[index]);
        }
        else
        {
            EXPECT_DOUBLE_EQ(double_vector[index], double_list[index]);
        }
    }

    std::memset(
        double_list,
        0,
        sizeof(double_list)
    );

    pkmn::c::list_cpp_to_c(
        double_vector,
        double_list,
        double_vector.size(),
        &output_size
    );
    EXPECT_EQ(double_vector.size(), output_size);
    for(size_t index = 0; index < double_vector.size(); ++index)
    {
        EXPECT_DOUBLE_EQ(double_vector[index], double_list[index]);
    }
}

enum class cpp_enum
{
    VALUE1 = 0,
    VALUE2,
    VALUE3
};

enum c_enum
{
    C_ENUM_VALUE1,
    C_ENUM_VALUE2,
    C_ENUM_VALUE3
};

struct c_test_enum_list
{
    c_enum* p_enums;
    size_t length;
};

TEST(cpp_to_c_test, enum_list_cpp_to_c_test)
{
    const std::vector<cpp_enum> cpp_enum_vector =
    {
        cpp_enum::VALUE2,
        cpp_enum::VALUE1,
        cpp_enum::VALUE3
    };
    struct c_test_enum_list c_enum_list =
    {
        nullptr,
        0
    };

    pkmn::c::enum_list_cpp_to_c<cpp_enum, enum c_enum, c_test_enum_list>(
        cpp_enum_vector,
        &c_enum_list
    );

    ASSERT_NE(nullptr, c_enum_list.p_enums);
    EXPECT_EQ(3ULL, c_enum_list.length);

    EXPECT_EQ(C_ENUM_VALUE2, c_enum_list.p_enums[0]);
    EXPECT_EQ(C_ENUM_VALUE1, c_enum_list.p_enums[1]);
    EXPECT_EQ(C_ENUM_VALUE3, c_enum_list.p_enums[2]);
}

struct c_test_enum_map
{
    int* p_values;
    size_t length;
};

TEST(cpp_to_c_test, enum_map_cpp_to_c)
{
    const std::map<cpp_enum, int> cpp_enum_map =
    {
        {cpp_enum::VALUE1, rand()},
        {cpp_enum::VALUE3, rand()}
    };
    struct c_test_enum_map c_enum_map =
    {
        nullptr,
        0ULL
    };

    pkmn::c::enum_map_cpp_to_c<cpp_enum, c_enum, c_test_enum_map, int, cpp_enum::VALUE3>(
        cpp_enum_map,
        &c_enum_map
    );

    ASSERT_NE(nullptr, c_enum_map.p_values);
    EXPECT_EQ(3ULL, c_enum_map.length);

    EXPECT_EQ(
        cpp_enum_map.at(cpp_enum::VALUE1),
        c_enum_map.p_values[C_ENUM_VALUE1]
    );
    EXPECT_EQ(
        cpp_enum_map.at(cpp_enum::VALUE3),
        c_enum_map.p_values[C_ENUM_VALUE3]
    );
}

TEST(cpp_to_c_test, hidden_power_cpp_to_c_test)
{
    pkmn::calculations::hidden_power hidden_power_cpp(
                                         pkmn::e_type::NORMAL,
                                         90
                                     );

    struct pkmn_hidden_power hidden_power_c = { PKMN_TYPE_NONE, 0 };

    pkmn::c::hidden_power_cpp_to_c(
        hidden_power_cpp,
        &hidden_power_c
    );

    EXPECT_EQ(PKMN_TYPE_NORMAL, hidden_power_c.type);
    EXPECT_EQ(90, hidden_power_c.base_power);
}

TEST(cpp_to_c_test, natural_gift_cpp_to_c_test)
{
    pkmn::calculations::natural_gift natural_gift_cpp(
                                         pkmn::e_type::NORMAL,
                                         90
                                     );

    struct pkmn_natural_gift natural_gift_c = { PKMN_TYPE_NONE, 0 };

    pkmn::c::natural_gift_cpp_to_c(
        natural_gift_cpp,
        &natural_gift_c
    );

    EXPECT_EQ(PKMN_TYPE_NORMAL, natural_gift_c.type);
    EXPECT_EQ(90, natural_gift_c.base_power);
}

TEST(cpp_to_c_test, item_slot_cpp_to_c_test)
{
    pkmn::item_slot item_slot_cpp(pkmn::e_item::POTION, 50);
    struct pkmn_item_slot item_slot_c;

    pkmn::c::item_slot_cpp_to_c(
        item_slot_cpp,
        &item_slot_c
    );

    EXPECT_EQ(PKMN_ITEM_POTION, item_slot_c.item);
    EXPECT_EQ(50, item_slot_c.amount);
}

TEST(cpp_to_c_test, item_slots_cpp_to_c_test)
{
    pkmn::item_slots_t item_slots_cpp =
    {
        pkmn::item_slot(pkmn::e_item::POTION, 50),
        pkmn::item_slot(pkmn::e_item::BERRY, 28),
        pkmn::item_slot(pkmn::e_item::BERRY_POUCH, 1)
    };

    struct pkmn_item_slots item_slots_c = { NULL, 0 };
    pkmn::c::item_slots_cpp_to_c(
        item_slots_cpp,
        &item_slots_c
    );

    EXPECT_EQ(3, item_slots_c.length);
    EXPECT_EQ(PKMN_ITEM_POTION, item_slots_c.p_item_slots[0].item);
    EXPECT_EQ(50, item_slots_c.p_item_slots[0].amount);
    EXPECT_EQ(PKMN_ITEM_BERRY, item_slots_c.p_item_slots[1].item);
    EXPECT_EQ(28, item_slots_c.p_item_slots[1].amount);
    EXPECT_EQ(PKMN_ITEM_BERRY_POUCH, item_slots_c.p_item_slots[2].item);
    EXPECT_EQ(1, item_slots_c.p_item_slots[2].amount);

    pkmn_item_slots_free(
        &item_slots_c
    );
    EXPECT_EQ(NULL, item_slots_c.p_item_slots);
    EXPECT_EQ(0, item_slots_c.length);
}

TEST(cpp_to_c_test, levelup_move_cpp_to_c_test)
{
    pkmn::database::levelup_move levelup_move_cpp(
        pkmn::e_move::SCRATCH,
        50
    );

    struct pkmn_levelup_move levelup_move_c = { PKMN_MOVE_NONE, 0 };

    pkmn::c::levelup_move_cpp_to_c(
        levelup_move_cpp,
        &levelup_move_c
    );

    EXPECT_EQ(PKMN_MOVE_SCRATCH, levelup_move_c.move);
    EXPECT_EQ(50, levelup_move_c.level);
}

TEST(cpp_to_c_test, levelup_moves_cpp_to_c_test)
{
    pkmn::database::levelup_moves_t levelup_moves_cpp =
    {
        pkmn::database::levelup_move(
            pkmn::e_move::SCRATCH,
            50
        ),
        pkmn::database::levelup_move(
            pkmn::e_move::SYNTHESIS,
            5
        ),
        pkmn::database::levelup_move(
            pkmn::e_move::FRENZY_PLANT,
            37
        )
    };

    struct pkmn_levelup_moves levelup_moves_c = { NULL, 0 };
    pkmn::c::levelup_moves_cpp_to_c(
        levelup_moves_cpp,
        &levelup_moves_c
    );

    EXPECT_EQ(3, levelup_moves_c.length);
    EXPECT_EQ(PKMN_MOVE_SCRATCH, levelup_moves_c.p_levelup_moves[0].move);
    EXPECT_EQ(50, levelup_moves_c.p_levelup_moves[0].level);
    EXPECT_EQ(PKMN_MOVE_SYNTHESIS, levelup_moves_c.p_levelup_moves[1].move);
    EXPECT_EQ(5, levelup_moves_c.p_levelup_moves[1].level);
    EXPECT_EQ(PKMN_MOVE_FRENZY_PLANT, levelup_moves_c.p_levelup_moves[2].move);
    EXPECT_EQ(37, levelup_moves_c.p_levelup_moves[2].level);

    pkmn_levelup_moves_free(
        &levelup_moves_c
    );
    EXPECT_EQ(NULL, levelup_moves_c.p_levelup_moves);
    EXPECT_EQ(0, levelup_moves_c.length);
}

TEST(cpp_to_c_test, move_list_cpp_to_c_test)
{
    pkmn::database::move_list_t move_list_cpp =
    {
        pkmn::database::move_entry(pkmn::e_move::SCRATCH, pkmn::e_game::RED),
        pkmn::database::move_entry(pkmn::e_move::SYNTHESIS, pkmn::e_game::SILVER),
        pkmn::database::move_entry(pkmn::e_move::FRENZY_PLANT, pkmn::e_game::EMERALD)
    };

    struct pkmn_string_list string_list_c = { NULL, 0 };
    pkmn::c::move_list_to_string_list(
        move_list_cpp,
        &string_list_c
    );

    EXPECT_EQ(3, string_list_c.length);
    EXPECT_STREQ("Scratch", string_list_c.pp_strings[0]);
    EXPECT_STREQ("Synthesis", string_list_c.pp_strings[1]);
    EXPECT_STREQ("Frenzy Plant", string_list_c.pp_strings[2]);

    pkmn_string_list_free(
        &string_list_c
    );
    EXPECT_EQ(NULL, string_list_c.pp_strings);
    EXPECT_EQ(0, string_list_c.length);
}

TEST(cpp_to_c_test, move_slot_cpp_to_c_test)
{
    pkmn::move_slot move_slot_cpp(
        pkmn::e_move::TACKLE,
        50
    );

    struct pkmn_move_slot move_slot_c;

    pkmn::c::move_slot_cpp_to_c(
        move_slot_cpp,
        &move_slot_c
    );

    EXPECT_EQ(PKMN_MOVE_TACKLE, move_slot_c.move);
    EXPECT_EQ(50, move_slot_c.pp);
}

TEST(cpp_to_c_test, move_slots_cpp_to_c_test)
{
    pkmn::move_slots_t move_slots_cpp =
    {
        pkmn::move_slot(
            pkmn::e_move::TACKLE,
            50
        ),
        pkmn::move_slot(
            pkmn::e_move::POUND,
            28
        ),
        pkmn::move_slot(
            pkmn::e_move::METRONOME,
            1
        )
    };

    struct pkmn_move_slots move_slots_c = { NULL, 0 };
    pkmn::c::move_slots_cpp_to_c(
        move_slots_cpp,
        &move_slots_c
    );

    EXPECT_EQ(3, move_slots_c.length);
    EXPECT_EQ(PKMN_MOVE_TACKLE, move_slots_c.p_move_slots[0].move);
    EXPECT_EQ(50, move_slots_c.p_move_slots[0].pp);
    EXPECT_EQ(PKMN_MOVE_POUND, move_slots_c.p_move_slots[1].move);
    EXPECT_EQ(28, move_slots_c.p_move_slots[1].pp);
    EXPECT_EQ(PKMN_MOVE_METRONOME, move_slots_c.p_move_slots[2].move);
    EXPECT_EQ(1, move_slots_c.p_move_slots[2].pp);

    pkmn_move_slots_free(
        &move_slots_c
    );
    EXPECT_EQ(NULL, move_slots_c.p_move_slots);
    EXPECT_EQ(0, move_slots_c.length);
}

TEST(cpp_to_c_test, pokemon_list_cpp_to_c)
{
    pkmn::pokemon_list_t pokemon_list_cpp =
    {
        pkmn::pokemon::make(
            pkmn::e_species::CHARMANDER, pkmn::e_game::RED, "", 5
        ),
        pkmn::pokemon::make(
            pkmn::e_species::SQUIRTLE, pkmn::e_game::BLUE, "", 10
        ),
        pkmn::pokemon::make(
            pkmn::e_species::BULBASAUR, pkmn::e_game::YELLOW, "", 15
        )
    };

    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon_list pokemon_list_c = { NULL, 0 };
    pkmn::c::pokemon_list_cpp_to_c(
        pokemon_list_cpp,
        &pokemon_list_c
    );
    EXPECT_EQ(3, pokemon_list_c.length);

    for(size_t i = 0; i < 3; ++i)
    {
        int level_c = 0;

        EXPECT_EQ(
            int(pokemon_list_cpp[i]->get_species()),
            pokemon_list_c.p_pokemon[i].species
        );
        EXPECT_EQ(
            int(pokemon_list_cpp[i]->get_game()),
            pokemon_list_c.p_pokemon[i].game
        );

        error = pkmn_pokemon_get_level(
                    &pokemon_list_c.p_pokemon[i],
                    &level_c
                );
        EXPECT_EQ(PKMN_ERROR_NONE, error);
        EXPECT_EQ(
            pokemon_list_cpp[i]->get_level(),
            level_c
        );
    }

    pkmn_pokemon_list_free(
        &pokemon_list_c
    );
    EXPECT_EQ(NULL, pokemon_list_c.p_pokemon);
    EXPECT_EQ(0, pokemon_list_c.length);
}

TEST(cpp_to_c_test, pokemon_box_list_cpp_to_c_test)
{
    pkmn::pokemon_box_list_t pokemon_box_list_cpp =
    {
        pkmn::pokemon_box::make(pkmn::e_game::GOLD),
        pkmn::pokemon_box::make(pkmn::e_game::RUBY),
        pkmn::pokemon_box::make(pkmn::e_game::FIRERED)
    };

    pokemon_box_list_cpp[0]->set_name("ABCD");
    pokemon_box_list_cpp[1]->set_name("EFGH");
    pokemon_box_list_cpp[2]->set_name("IJKL");

    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon_box_list pokemon_box_list_c = { NULL, 0 };
    pkmn::c::pokemon_box_list_cpp_to_c(
        pokemon_box_list_cpp,
        &pokemon_box_list_c
    );
    EXPECT_EQ(3, pokemon_box_list_c.length);

    for(size_t index = 0; index < 3; ++index)
    {
        char name_c[BUFFER_LEN] = {0};

        EXPECT_EQ(
            int(pokemon_box_list_cpp[index]->get_game()),
            int(pokemon_box_list_c.p_boxes[index].game)
        );

        error = pkmn_pokemon_box_get_name(
                    &pokemon_box_list_c.p_boxes[index],
                    name_c,
                    sizeof(name_c),
                    nullptr
                );
        EXPECT_EQ(PKMN_ERROR_NONE, error);
        EXPECT_STREQ(
            pokemon_box_list_cpp[index]->get_name().c_str(),
            name_c
        );
    }

    pkmn_pokemon_box_list_free(
        &pokemon_box_list_c
    );
    EXPECT_EQ(NULL, pokemon_box_list_c.p_boxes);
    EXPECT_EQ(0, pokemon_box_list_c.length);
}

TEST(cpp_to_c_test, int_pair_cpp_to_c_test)
{
    std::pair<int, int> int_pair_cpp(6322, 10011);
    struct pkmn_int_pair int_pair_c = {0, 0};

    pkmn::c::int_pair_cpp_to_c(
        int_pair_cpp,
        &int_pair_c
    );

    EXPECT_EQ(6322, int_pair_c.first);
    EXPECT_EQ(10011, int_pair_c.second);
}

TEST(cpp_to_c_test, string_cpp_to_c_test)
{
    std::string string_cpp = "LibPKMN";
    char string_c[50] = {0};
    char too_short_string_buffer[3] = {0};
    size_t string_length = 0;

    // Test not getting buffer length
    pkmn::c::string_cpp_to_c(
        string_cpp,
        string_c,
        sizeof(string_c)-1,
        nullptr
    );
    EXPECT_EQ(string_cpp, string_c);

    // Make sure accurate string length is returned.
    pkmn::c::string_cpp_to_c(
        string_cpp,
        string_c,
        sizeof(string_c)-1,
        &string_length 
    );
    EXPECT_EQ(string_cpp, string_c);
    EXPECT_EQ(string_cpp.size(), string_length);

    // Make sure if a too-small buffer is given, the partial string
    // is returned.
    pkmn::c::string_cpp_to_c(
        string_cpp,
        too_short_string_buffer,
        sizeof(too_short_string_buffer)-1,
        &string_length
    );
    EXPECT_STREQ("Li", too_short_string_buffer);
    EXPECT_EQ(string_cpp.size(), string_length);
}

TEST(cpp_to_c_test, string_pair_cpp_to_c_test)
{
    std::pair<std::string, std::string> string_pair_cpp("LibPKMN", "PKSav");
    struct pkmn_string_pair string_pair_c = {NULL, NULL};

    pkmn::c::string_pair_cpp_to_c(
        string_pair_cpp,
        &string_pair_c
    );

    EXPECT_STREQ("LibPKMN", string_pair_c.p_first);
    EXPECT_STREQ("PKSav", string_pair_c.p_second);

    pkmn_string_pair_free(&string_pair_c);
    EXPECT_EQ(NULL, string_pair_c.p_first);
    EXPECT_EQ(NULL, string_pair_c.p_second);
}

TEST(cpp_to_c_test, std_map_keys_to_string_list_test)
{
    static const std::map<std::string, bool> string_bool_map =
    {
        {"key1", true},
        {"key2", false},
        {"key3", false},
        {"key4", true},
    };
    static const std::map<std::string, int> string_int_map =
    {
        {"key5", 1},
        {"key6", 2},
        {"key7", 3},
        {"key8", 4},
        {"key9", 5},
    };

    struct pkmn_string_list string_list_c = {NULL, 0};

    pkmn::c::string_map_keys_to_string_list<bool>(
        string_bool_map,
        &string_list_c
    );

    // To suppress Clang-tidy warning, instead of assering length
    EXPECT_EQ(4ULL, string_list_c.length);
    if(string_list_c.pp_strings)
    {
        EXPECT_STREQ("key1", string_list_c.pp_strings[0]);
        EXPECT_STREQ("key2", string_list_c.pp_strings[1]);
        EXPECT_STREQ("key3", string_list_c.pp_strings[2]);
        EXPECT_STREQ("key4", string_list_c.pp_strings[3]);

        pkmn_string_list_free(&string_list_c);
        EXPECT_EQ(NULL, string_list_c.pp_strings);
        EXPECT_EQ(0, string_list_c.length);
    }

    pkmn::c::string_map_keys_to_string_list<int>(
        string_int_map,
        &string_list_c
    );

    // To suppress Clang-tidy warning, instead of asserting length
    EXPECT_EQ(5ULL, string_list_c.length);
    if(string_list_c.pp_strings)
    {
        EXPECT_STREQ("key5", string_list_c.pp_strings[0]);
        EXPECT_STREQ("key6", string_list_c.pp_strings[1]);
        EXPECT_STREQ("key7", string_list_c.pp_strings[2]);
        EXPECT_STREQ("key8", string_list_c.pp_strings[3]);
        EXPECT_STREQ("key9", string_list_c.pp_strings[4]);

        pkmn_string_list_free(&string_list_c);
        EXPECT_EQ(NULL, string_list_c.pp_strings);
        EXPECT_EQ(0, string_list_c.length);
    }
}

TEST(cpp_to_c_test, string_list_cpp_to_c_test)
{
    std::vector<std::string> string_vector_cpp{"LibPKMN", "PKSav", "TKO"};

    struct pkmn_string_list string_list_c = { NULL, 0 };
    pkmn::c::string_list_cpp_to_c(
        string_vector_cpp,
        &string_list_c
    );

    EXPECT_EQ(3, string_list_c.length);
    EXPECT_STREQ("LibPKMN", string_list_c.pp_strings[0]);
    EXPECT_STREQ("PKSav", string_list_c.pp_strings[1]);
    EXPECT_STREQ("TKO", string_list_c.pp_strings[2]);

    pkmn_string_list_free(
        &string_list_c
    );
    EXPECT_EQ(NULL, string_list_c.pp_strings);
    EXPECT_EQ(0, string_list_c.length);
}

TEST(cpp_to_c_test, time_duration_cpp_to_c_test)
{
    const pkmn::time_duration time_duration_cpp1 = {23, 36, 30, 30};
    struct pkmn_time_duration time_duration_c = {0, 0, 0, 0};

    pkmn::c::time_duration_cpp_to_c(
        time_duration_cpp1,
        &time_duration_c
    );

    EXPECT_EQ(time_duration_cpp1.hours,   time_duration_c.hours);
    EXPECT_EQ(time_duration_cpp1.minutes, time_duration_c.minutes);
    EXPECT_EQ(time_duration_cpp1.seconds, time_duration_c.seconds);
    EXPECT_EQ(time_duration_cpp1.frames,  time_duration_c.frames);

    pkmn::time_duration time_duration_cpp2 = pkmn::c::time_duration_c_to_cpp(
                                                 &time_duration_c
                                             );
    EXPECT_EQ(time_duration_cpp1, time_duration_cpp2);
}
