/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#    pragma warning(disable: 4273) // Inconsistent DLL linkage
#endif

#include "c/cpp_to_c.hpp"
#include "c/error_internal.hpp"

#include <pkmn.h>

#include <boost/thread/mutex.hpp>
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>

class pkmn_test_exception: public std::exception {
    public:
        explicit pkmn_test_exception(const char* msg):
            msg_(msg) {}

        explicit pkmn_test_exception(const std::string &msg):
            msg_(msg) {}

        virtual ~pkmn_test_exception() throw() {}

        virtual const char* what() const throw() {
            return msg_.c_str();
        }

    protected:
        std::string msg_;
};

/*
 * Converting C++ exceptions to C error codes
 */

pkmn_error_t throw_nothing() {
    PKMN_CPP_TO_C()
}

pkmn_error_t throw_unknown() {
    PKMN_CPP_TO_C(
        throw 5;
    )
}

template <typename exception_type>
pkmn_error_t throw_exception(
    const std::string &msg
) {
    PKMN_CPP_TO_C(
        throw exception_type(msg);
    )
}

BOOST_AUTO_TEST_CASE(exception_to_error_code_test) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = throw_nothing();
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "None");

    error = throw_exception<std::invalid_argument>("invalid_argument");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "invalid_argument");

    error = throw_exception<std::domain_error>("domain_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_DOMAIN_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "domain_error");

    error = throw_exception<std::length_error>("length_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LENGTH_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "length_error");

    error = throw_exception<std::out_of_range>("out_of_range");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OUT_OF_RANGE);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "out_of_range");

    error = throw_exception<std::logic_error>("logic_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LOGIC_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "logic_error");

    error = throw_exception<std::range_error>("range_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "range_error");

    error = throw_exception<std::overflow_error>("overflow_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OVERFLOW_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "overflow_error");

    error = throw_exception<std::underflow_error>("underflow_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNDERFLOW_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "underflow_error");

    error = throw_exception<std::runtime_error>("runtime_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RUNTIME_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "runtime_error");

    error = throw_exception<pkmn_test_exception>("std_exception");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_STD_EXCEPTION);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "std_exception");

    error = throw_unknown();
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNKNOWN_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "Unknown error");
}

/*
 * Converting C++ exceptions to C error codes (with handle)
 */

typedef struct {
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_test_handle_t;

pkmn_error_t throw_nothing_with_handle(
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,)
}

pkmn_error_t throw_unknown_with_handle(
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw 5;
    )
}

template <typename exception_type>
pkmn_error_t throw_exception_with_handle(
    const std::string &msg,
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw exception_type(msg);
    )
}

BOOST_AUTO_TEST_CASE(exception_to_error_code_with_handle_test) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_test_handle_t test_handle;

    error = throw_nothing_with_handle(&test_handle);
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
    BOOST_CHECK_EQUAL(test_handle.last_error, "None");

    error = throw_exception_with_handle<std::invalid_argument>(
        "invalid_argument", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    BOOST_CHECK_EQUAL(test_handle.last_error, "invalid_argument");

    error = throw_exception_with_handle<std::domain_error>(
        "domain_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_DOMAIN_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "domain_error");

    error = throw_exception_with_handle<std::length_error>(
        "length_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LENGTH_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "length_error");

    error = throw_exception_with_handle<std::out_of_range>(
        "out_of_range", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OUT_OF_RANGE);
    BOOST_CHECK_EQUAL(test_handle.last_error, "out_of_range");

    error = throw_exception_with_handle<std::logic_error>(
        "logic_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LOGIC_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "logic_error");

    error = throw_exception_with_handle<std::range_error>(
        "range_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "range_error");

    error = throw_exception_with_handle<std::overflow_error>(
        "overflow_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OVERFLOW_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "overflow_error");

    error = throw_exception_with_handle<std::underflow_error>(
        "underflow_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNDERFLOW_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "underflow_error");

    error = throw_exception_with_handle<std::runtime_error>(
        "runtime_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RUNTIME_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "runtime_error");

    error = throw_exception_with_handle<pkmn_test_exception>(
        "std_exception", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_STD_EXCEPTION);
    BOOST_CHECK_EQUAL(test_handle.last_error, "std_exception");

    error = throw_unknown_with_handle(&test_handle);
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNKNOWN_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "Unknown error");
}

/*
 * Converting C++ types to C types
 */

BOOST_AUTO_TEST_CASE(hidden_power_cpp_to_c_test) {
    pkmn::calculations::hidden_power hidden_power_cpp("Normal", 90);

    pkmn_hidden_power_t hidden_power_c;

    pkmn::pkmn_hidden_power_cpp_to_c(
        hidden_power_cpp,
        &hidden_power_c
    );

    BOOST_CHECK_EQUAL(hidden_power_c.type, "Normal");
    BOOST_CHECK_EQUAL(hidden_power_c.base_power, 90);

    pkmn_hidden_power_free(&hidden_power_c);
    BOOST_CHECK(!hidden_power_c.type);
}

BOOST_AUTO_TEST_CASE(item_slot_cpp_to_c_test) {
    pkmn::item_slot item_slot_cpp(
        pkmn::database::item_entry("Potion", "Red"),
        50
    );

    pkmn_item_slot_t item_slot_c;

    pkmn::pkmn_item_slot_cpp_to_c(
        item_slot_cpp,
        &item_slot_c
    );

    BOOST_CHECK_EQUAL(item_slot_c.item, "Potion");
    BOOST_CHECK_EQUAL(item_slot_c.amount, 50);

    pkmn_item_slot_free(&item_slot_c);
    BOOST_CHECK(!item_slot_c.item);
}

BOOST_AUTO_TEST_CASE(item_slots_cpp_to_c_test) {
    pkmn::item_slots_t item_slots_cpp;
    item_slots_cpp.emplace_back(
        pkmn::item_slot(
            pkmn::database::item_entry("Potion", "Red"),
            50
        )
    );
    item_slots_cpp.emplace_back(
        pkmn::item_slot(
            pkmn::database::item_entry("Berry", "Silver"),
            28
        )
    );
    item_slots_cpp.emplace_back(
        pkmn::item_slot(
            pkmn::database::item_entry("Berry Pouch", "LeafGreen"),
            1
        )
    );

    pkmn_item_slots_t item_slots_c = NULL;
    size_t list_length = 0;

    pkmn::pkmn_item_slots_cpp_to_c(
        item_slots_cpp,
        &item_slots_c,
        &list_length
    );

    BOOST_CHECK_EQUAL(list_length, 3);
    BOOST_CHECK_EQUAL(item_slots_c[0].item, "Potion");
    BOOST_CHECK_EQUAL(item_slots_c[0].amount, 50);
    BOOST_CHECK_EQUAL(item_slots_c[1].item, "Berry");
    BOOST_CHECK_EQUAL(item_slots_c[1].amount, 28);
    BOOST_CHECK_EQUAL(item_slots_c[2].item, "Berry Pouch");
    BOOST_CHECK_EQUAL(item_slots_c[2].amount, 1);

    pkmn_item_slots_free(
        &item_slots_c,
        list_length
    );
    BOOST_CHECK(!item_slots_c);
}

BOOST_AUTO_TEST_CASE(levelup_move_cpp_to_c_test) {
    pkmn::database::levelup_move levelup_move_cpp(
        pkmn::database::move_entry("Scratch", "Red"),
        50
    );

    pkmn_levelup_move_t levelup_move_c;

    pkmn::pkmn_levelup_move_cpp_to_c(
        levelup_move_cpp,
        &levelup_move_c
    );

    BOOST_CHECK_EQUAL(levelup_move_c.move, "Scratch");
    BOOST_CHECK_EQUAL(levelup_move_c.level, 50);

    pkmn_levelup_move_free(&levelup_move_c);
    BOOST_CHECK(!levelup_move_c.move);
}

BOOST_AUTO_TEST_CASE(levelup_moves_cpp_to_c_test) {
    pkmn::database::levelup_moves_t levelup_moves_cpp;
    levelup_moves_cpp.emplace_back(
        pkmn::database::levelup_move(
            pkmn::database::move_entry("Scratch", "Red"),
            50
        )
    );
    levelup_moves_cpp.emplace_back(
        pkmn::database::levelup_move(
            pkmn::database::move_entry("Synthesis", "Silver"),
            5
        )
    );
    levelup_moves_cpp.emplace_back(
        pkmn::database::levelup_move(
            pkmn::database::move_entry("Frenzy Plant", "Emerald"),
            37
        )
    );

    pkmn_levelup_moves_t levelup_moves_c = NULL;
    size_t list_length = 0;

    pkmn::pkmn_levelup_moves_cpp_to_c(
        levelup_moves_cpp,
        &levelup_moves_c,
        &list_length
    );

    BOOST_CHECK_EQUAL(list_length, 3);
    BOOST_CHECK_EQUAL(levelup_moves_c[0].move, "Scratch");
    BOOST_CHECK_EQUAL(levelup_moves_c[0].level, 50);
    BOOST_CHECK_EQUAL(levelup_moves_c[1].move, "Synthesis");
    BOOST_CHECK_EQUAL(levelup_moves_c[1].level, 5);
    BOOST_CHECK_EQUAL(levelup_moves_c[2].move, "Frenzy Plant");
    BOOST_CHECK_EQUAL(levelup_moves_c[2].level, 37);

    pkmn_levelup_moves_free(
        &levelup_moves_c,
        list_length
    );
    BOOST_CHECK(!levelup_moves_c);
}

BOOST_AUTO_TEST_CASE(move_list_cpp_to_c_test) {
    pkmn::database::move_list_t move_list_cpp;
    move_list_cpp.emplace_back(
        pkmn::database::move_entry("Scratch", "Red")
    );
    move_list_cpp.emplace_back(
        pkmn::database::move_entry("Synthesis", "Silver")
    );
    move_list_cpp.emplace_back(
        pkmn::database::move_entry("Frenzy Plant", "Emerald")
    );

    pkmn_string_list_t string_list_c = NULL;
    size_t list_length = 0;

    pkmn::pkmn_move_list_to_string_list(
        move_list_cpp,
        &string_list_c,
        &list_length
    );

    BOOST_CHECK_EQUAL(list_length, 3);
    BOOST_CHECK_EQUAL(string_list_c[0], "Scratch");
    BOOST_CHECK_EQUAL(string_list_c[1], "Synthesis");
    BOOST_CHECK_EQUAL(string_list_c[2], "Frenzy Plant");

    pkmn_string_list_free(
        &string_list_c,
        list_length
    );
    BOOST_CHECK(!string_list_c);
}

BOOST_AUTO_TEST_CASE(pokemon_entries_cpp_to_c_test) {
    pkmn::database::pokemon_entries_t pokemon_entries_cpp;
    pokemon_entries_cpp.emplace_back(
        pkmn::database::pokemon_entry("Charmander", "Red", "")
    );
    pokemon_entries_cpp.emplace_back(
        pkmn::database::pokemon_entry("Totodile", "Gold", "")
    );
    pokemon_entries_cpp.emplace_back(
        pkmn::database::pokemon_entry("Treecko", "Ruby", "")
    );

    pkmn_string_list_t string_list_c = NULL;
    size_t list_length = 0;

    pkmn::pkmn_pokemon_entries_to_string_list(
        pokemon_entries_cpp,
        &string_list_c,
        &list_length
    );

    BOOST_CHECK_EQUAL(list_length, 3);
    BOOST_CHECK_EQUAL(string_list_c[0], "Charmander");
    BOOST_CHECK_EQUAL(string_list_c[1], "Totodile");
    BOOST_CHECK_EQUAL(string_list_c[2], "Treecko");

    pkmn_string_list_free(
        &string_list_c,
        list_length
    );
    BOOST_CHECK(!string_list_c);
}

BOOST_AUTO_TEST_CASE(int_pair_cpp_to_c_test) {
    std::pair<int, int> int_pair_cpp(6322, 10011);
    pkmn_int_pair_t int_pair_c;

    pkmn::std_pair_int_to_int_pair(
        int_pair_cpp,
        &int_pair_c
    );

    BOOST_CHECK_EQUAL(int_pair_c.first, 6322);
    BOOST_CHECK_EQUAL(int_pair_c.second, 10011);
}

BOOST_AUTO_TEST_CASE(std_string_cpp_to_c_test) {
    std::string string_cpp = "LibPKMN";
    char string_c[8] = "";
    size_t actual_strlen = 0;

    // Buffer too small
    pkmn::std_string_to_c_str(
        string_cpp,
        string_c,
        5,
        &actual_strlen
    );
    BOOST_CHECK_EQUAL(string_c, "LibPK");
    BOOST_CHECK_EQUAL(actual_strlen, 8);

    // Full buffer
    pkmn::std_string_to_c_str(
        string_cpp,
        string_c,
        8,
        &actual_strlen
    );
    BOOST_CHECK_EQUAL(string_c, "LibPKMN");
    BOOST_CHECK_EQUAL(actual_strlen, 8);
}

BOOST_AUTO_TEST_CASE(string_pair_cpp_to_c_test) {
    std::pair<std::string, std::string> string_pair_cpp("LibPKMN", "PKSav");
    pkmn_string_pair_t string_pair_c;

    pkmn::std_pair_std_string_to_string_pair(
        string_pair_cpp,
        &string_pair_c
    );

    BOOST_CHECK_EQUAL(string_pair_c.first, "LibPKMN");
    BOOST_CHECK_EQUAL(string_pair_c.second, "PKSav");

    pkmn_string_pair_free(&string_pair_c);
    BOOST_CHECK(!string_pair_c.first);
    BOOST_CHECK(!string_pair_c.second);
}

BOOST_AUTO_TEST_CASE(string_vector_cpp_to_c_test) {
    std::vector<std::string> string_vector_cpp;
    string_vector_cpp.emplace_back("LibPKMN");
    string_vector_cpp.emplace_back("PKSav");
    string_vector_cpp.emplace_back("TKO");

    pkmn_string_list_t string_list_c = NULL;
    size_t list_length = 0;

    pkmn::std_vector_std_string_to_string_list(
        string_vector_cpp,
        &string_list_c,
        &list_length
    );

    BOOST_CHECK_EQUAL(list_length, 3);
    BOOST_CHECK_EQUAL(string_list_c[0], "LibPKMN");
    BOOST_CHECK_EQUAL(string_list_c[1], "PKSav");
    BOOST_CHECK_EQUAL(string_list_c[2], "TKO");

    pkmn_string_list_free(
        &string_list_c,
        list_length
    );
    BOOST_CHECK(!string_list_c);
}
