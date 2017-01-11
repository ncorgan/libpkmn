/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c/cpp_to_c.hpp"
#include "c/error_internal.hpp"

#ifdef _MSC_VER
#    pragma warning(disable: 4273) // Inconsistent DLL linkage
#endif
extern "C" {
    const char* pkmn_strerror();
}

#include <pkmn.h>

#include <pksav.h>

#include <boost/assign.hpp>
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

#define STRBUFFER_LEN 1024

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

pkmn_error_t throw_feature_not_in_game_error(
    const std::string &feature,
    const std::string &game
) {
    PKMN_CPP_TO_C(
        throw pkmn::feature_not_in_game_error(
                  feature,
                  game
              );
    )
}

pkmn_error_t throw_pksav_error(
    pksav_error_t pksav_error_code
) {
    PKMN_CPP_TO_C(
        throw pkmn::pksav_error(int(pksav_error_code));
    )
}

pkmn_error_t throw_pkmn_range_error(
    const std::string &value,
    int min,
    int max
) {
    PKMN_CPP_TO_C(
        throw pkmn::range_error(value, min, max);
    )
}

pkmn_error_t throw_pkmn_unimplemented_error() {
    PKMN_CPP_TO_C(
        throw pkmn::unimplemented_error();
    )
}

BOOST_AUTO_TEST_CASE(exception_to_error_code_test) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = throw_nothing();
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "None");

    error = throw_feature_not_in_game_error("Contests", "Generation I");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "Contests not in Generation I");

    error = throw_exception<pkmn::feature_not_in_game_error>("feature_not_in_game_error");
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "feature_not_in_game_error");

    error = throw_pksav_error(PKSAV_ERROR_INVALID_SAVE);
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_PKSAV_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "PKSav returned the following error: \"Invalid save file\"");

    error = throw_pkmn_range_error("var", 1, 10);
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "var: valid values 1-10");

    error = throw_pkmn_unimplemented_error();
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNIMPLEMENTED_ERROR);
    BOOST_CHECK_EQUAL(pkmn_strerror(), "Currently unimplemented.");

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
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_STD_RANGE_ERROR);
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

pkmn_error_t throw_feature_not_in_game_error_with_handle(
    const std::string &feature,
    const std::string &game,
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw pkmn::feature_not_in_game_error(
                  feature,
                  game
              );
    )
}

pkmn_error_t throw_pksav_error_with_handle(
    pksav_error_t pksav_error_code,
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw pkmn::pksav_error(int(pksav_error_code));
    )
}

pkmn_error_t throw_pkmn_range_error_with_handle(
    const std::string &value,
    int min,
    int max,
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw pkmn::range_error(value, min, max);
    )
}

pkmn_error_t throw_pkmn_unimplemented_error_with_handle(
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw pkmn::unimplemented_error();
    )
}

BOOST_AUTO_TEST_CASE(exception_to_error_code_with_handle_test) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_test_handle_t test_handle;

    error = throw_nothing_with_handle(&test_handle);
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
    BOOST_CHECK_EQUAL(test_handle.last_error, "None");

    error = throw_feature_not_in_game_error_with_handle(
        "Contests", "Generation I", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "Contests not in Generation I");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "Contests not in Generation I");

    error = throw_exception_with_handle<pkmn::feature_not_in_game_error>(
        "feature_not_in_game_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "feature_not_in_game_error");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "feature_not_in_game_error");

    error = throw_pksav_error_with_handle(
        PKSAV_ERROR_INVALID_SAVE, &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_PKSAV_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "PKSav returned the following error: \"Invalid save file\"");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "PKSav returned the following error: \"Invalid save file\"");

    error = throw_pkmn_range_error_with_handle(
        "var", 1, 10, &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "var: valid values 1-10");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "var: valid values 1-10");

    error = throw_pkmn_unimplemented_error_with_handle(&test_handle);
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNIMPLEMENTED_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "Currently unimplemented.");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "Currently unimplemented.");

    error = throw_exception_with_handle<std::invalid_argument>(
        "invalid_argument", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    BOOST_CHECK_EQUAL(test_handle.last_error, "invalid_argument");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "invalid_argument");

    error = throw_exception_with_handle<std::domain_error>(
        "domain_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_DOMAIN_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "domain_error");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "domain_error");

    error = throw_exception_with_handle<std::length_error>(
        "length_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LENGTH_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "length_error");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "length_error");

    error = throw_exception_with_handle<std::out_of_range>(
        "out_of_range", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OUT_OF_RANGE);
    BOOST_CHECK_EQUAL(test_handle.last_error, "out_of_range");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "out_of_range");

    error = throw_exception_with_handle<std::logic_error>(
        "logic_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_LOGIC_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "logic_error");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "logic_error");

    error = throw_exception_with_handle<std::range_error>(
        "range_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_STD_RANGE_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "range_error");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "range_error");

    error = throw_exception_with_handle<std::overflow_error>(
        "overflow_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_OVERFLOW_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "overflow_error");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "overflow_error");

    error = throw_exception_with_handle<std::underflow_error>(
        "underflow_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNDERFLOW_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "underflow_error");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "underflow_error");

    error = throw_exception_with_handle<std::runtime_error>(
        "runtime_error", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_RUNTIME_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "runtime_error");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "runtime_error");

    error = throw_exception_with_handle<pkmn_test_exception>(
        "std_exception", &test_handle
    );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_STD_EXCEPTION);
    BOOST_CHECK_EQUAL(test_handle.last_error, "std_exception");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "std_exception");

    error = throw_unknown_with_handle(&test_handle);
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_UNKNOWN_ERROR);
    BOOST_CHECK_EQUAL(test_handle.last_error, "Unknown error");
    BOOST_CHECK_EQUAL(pkmn_strerror(), "Unknown error");
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
    BOOST_CHECK_EQUAL(item_slot_c.amount, 0);
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

    pkmn_item_slots_t item_slots_c = { NULL, 0 };
    pkmn::pkmn_item_slots_cpp_to_c(
        item_slots_cpp,
        &item_slots_c
    );

    BOOST_CHECK_EQUAL(item_slots_c.length, 3);
    BOOST_CHECK_EQUAL(item_slots_c.item_slots[0].item, "Potion");
    BOOST_CHECK_EQUAL(item_slots_c.item_slots[0].amount, 50);
    BOOST_CHECK_EQUAL(item_slots_c.item_slots[1].item, "Berry");
    BOOST_CHECK_EQUAL(item_slots_c.item_slots[1].amount, 28);
    BOOST_CHECK_EQUAL(item_slots_c.item_slots[2].item, "Berry Pouch");
    BOOST_CHECK_EQUAL(item_slots_c.item_slots[2].amount, 1);

    pkmn_item_slots_free(
        &item_slots_c
    );
    BOOST_CHECK(!item_slots_c.item_slots);
    BOOST_CHECK_EQUAL(item_slots_c.length, 0);
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
    BOOST_CHECK_EQUAL(levelup_move_c.level, 0);
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

    pkmn_levelup_moves_t levelup_moves_c = { NULL, 0 };
    pkmn::pkmn_levelup_moves_cpp_to_c(
        levelup_moves_cpp,
        &levelup_moves_c
    );

    BOOST_CHECK_EQUAL(levelup_moves_c.length, 3);
    BOOST_CHECK_EQUAL(levelup_moves_c.levelup_moves[0].move, "Scratch");
    BOOST_CHECK_EQUAL(levelup_moves_c.levelup_moves[0].level, 50);
    BOOST_CHECK_EQUAL(levelup_moves_c.levelup_moves[1].move, "Synthesis");
    BOOST_CHECK_EQUAL(levelup_moves_c.levelup_moves[1].level, 5);
    BOOST_CHECK_EQUAL(levelup_moves_c.levelup_moves[2].move, "Frenzy Plant");
    BOOST_CHECK_EQUAL(levelup_moves_c.levelup_moves[2].level, 37);

    pkmn_levelup_moves_free(
        &levelup_moves_c
    );
    BOOST_CHECK(!levelup_moves_c.levelup_moves);
    BOOST_CHECK_EQUAL(levelup_moves_c.length, 0);
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

    pkmn_string_list_t string_list_c = { NULL, 0 };
    pkmn::pkmn_move_list_to_string_list(
        move_list_cpp,
        &string_list_c
    );

    BOOST_CHECK_EQUAL(string_list_c.length, 3);
    BOOST_CHECK_EQUAL(string_list_c.strings[0], "Scratch");
    BOOST_CHECK_EQUAL(string_list_c.strings[1], "Synthesis");
    BOOST_CHECK_EQUAL(string_list_c.strings[2], "Frenzy Plant");

    pkmn_string_list_free(
        &string_list_c
    );
    BOOST_CHECK(!string_list_c.strings);
    BOOST_CHECK_EQUAL(string_list_c.length, 0);
}

BOOST_AUTO_TEST_CASE(move_slot_cpp_to_c_test) {
    pkmn::move_slot move_slot_cpp(
        pkmn::database::move_entry("Tackle", "Red"),
        50
    );

    pkmn_move_slot_t move_slot_c;

    pkmn::pkmn_move_slot_cpp_to_c(
        move_slot_cpp,
        &move_slot_c
    );

    BOOST_CHECK_EQUAL(move_slot_c.move, "Tackle");
    BOOST_CHECK_EQUAL(move_slot_c.pp, 50);

    pkmn_move_slot_free(&move_slot_c);
    BOOST_CHECK(!move_slot_c.move);
    BOOST_CHECK_EQUAL(move_slot_c.pp, 0);
}

BOOST_AUTO_TEST_CASE(move_slots_cpp_to_c_test) {
    pkmn::move_slots_t move_slots_cpp;
    move_slots_cpp.emplace_back(
        pkmn::move_slot(
            pkmn::database::move_entry("Tackle", "Red"),
            50
        )
    );
    move_slots_cpp.emplace_back(
        pkmn::move_slot(
            pkmn::database::move_entry("Pound", "Silver"),
            28
        )
    );
    move_slots_cpp.emplace_back(
        pkmn::move_slot(
            pkmn::database::move_entry("Metronome", "LeafGreen"),
            1
        )
    );

    pkmn_move_slots_t move_slots_c = { NULL, 0 };
    pkmn::pkmn_move_slots_cpp_to_c(
        move_slots_cpp,
        &move_slots_c
    );

    BOOST_CHECK_EQUAL(move_slots_c.length, 3);
    BOOST_CHECK_EQUAL(move_slots_c.move_slots[0].move, "Tackle");
    BOOST_CHECK_EQUAL(move_slots_c.move_slots[0].pp, 50);
    BOOST_CHECK_EQUAL(move_slots_c.move_slots[1].move, "Pound");
    BOOST_CHECK_EQUAL(move_slots_c.move_slots[1].pp, 28);
    BOOST_CHECK_EQUAL(move_slots_c.move_slots[2].move, "Metronome");
    BOOST_CHECK_EQUAL(move_slots_c.move_slots[2].pp, 1);

    pkmn_move_slots_free(
        &move_slots_c
    );
    BOOST_CHECK(!move_slots_c.move_slots);
    BOOST_CHECK_EQUAL(move_slots_c.length, 0);
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

    pkmn_string_list_t string_list_c = { NULL, 0 };
    pkmn::pkmn_pokemon_entries_to_string_list(
        pokemon_entries_cpp,
        &string_list_c
    );

    BOOST_CHECK_EQUAL(string_list_c.length, 3);
    BOOST_CHECK_EQUAL(string_list_c.strings[0], "Charmander");
    BOOST_CHECK_EQUAL(string_list_c.strings[1], "Totodile");
    BOOST_CHECK_EQUAL(string_list_c.strings[2], "Treecko");

    pkmn_string_list_free(
        &string_list_c
    );
    BOOST_CHECK(!string_list_c.strings);
    BOOST_CHECK_EQUAL(string_list_c.length, 0);
}

BOOST_AUTO_TEST_CASE(pokemon_list_cpp_to_c) {
    pkmn::pokemon_list_t pokemon_list_cpp;
    pokemon_list_cpp.emplace_back(
        pkmn::pokemon::make(
            "Charmander", "Red", "", 5
        )
    );
    pokemon_list_cpp.emplace_back(
        pkmn::pokemon::make(
            "Squirtle", "Blue", "", 10
        )
    );
    pokemon_list_cpp.emplace_back(
        pkmn::pokemon::make(
            "Bulbasaur", "Yellow", "", 15
        )
    );

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_list_t pokemon_list_c = { NULL, 0 };
    pkmn::pkmn_pokemon_list_cpp_to_c(
        pokemon_list_cpp,
        &pokemon_list_c
    );
    BOOST_CHECK_EQUAL(
        pokemon_list_c.length,
        3
    );

    for(size_t i = 0; i < 3; ++i) {
        char species_c[STRBUFFER_LEN] = {0};
        char game_c[STRBUFFER_LEN] = {0};
        int level_c = 0;

        error = pkmn_pokemon_get_species(
                    pokemon_list_c.pokemon_list[i],
                    species_c,
                    sizeof(species_c)
                );
        BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
        BOOST_CHECK_EQUAL(strcmp(pokemon_list_cpp[i]->get_species().c_str(), species_c), 0);

        error = pkmn_pokemon_get_game(
                    pokemon_list_c.pokemon_list[i],
                    game_c,
                    sizeof(game_c)
                );
        BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
        BOOST_CHECK_EQUAL(strcmp(pokemon_list_cpp[i]->get_game().c_str(), game_c), 0);

        error = pkmn_pokemon_get_level(
                    pokemon_list_c.pokemon_list[i],
                    &level_c
                );
        BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
        BOOST_CHECK_EQUAL(pokemon_list_cpp[i]->get_level(), level_c);
    }

    pkmn_pokemon_list_free(
        &pokemon_list_c
    );
    BOOST_CHECK(!pokemon_list_c.pokemon_list);
    BOOST_CHECK_EQUAL(pokemon_list_c.length, 0);
}

BOOST_AUTO_TEST_CASE(pokemon_box_list_cpp_to_c_test) {
    pkmn::pokemon_box_list_t pokemon_box_list_cpp;
    pokemon_box_list_cpp.emplace_back(
        pkmn::pokemon_box::make("Gold")
    );
    pokemon_box_list_cpp.emplace_back(
        pkmn::pokemon_box::make("Ruby")
    );
    pokemon_box_list_cpp.emplace_back(
        pkmn::pokemon_box::make("FireRed")
    );

    pokemon_box_list_cpp[0]->set_name("ABCD");
    pokemon_box_list_cpp[1]->set_name("EFGH");
    pokemon_box_list_cpp[2]->set_name("IJKL");

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_box_list_t pokemon_box_list_c = { NULL, 0 };
    pkmn::pkmn_pokemon_box_list_cpp_to_c(
        pokemon_box_list_cpp,
        &pokemon_box_list_c
    );
    BOOST_CHECK_EQUAL(
        pokemon_box_list_c.length,
        3
    );

    for(size_t i = 0; i < 3; ++i) {
        char game_c[STRBUFFER_LEN] = {0};
        char name_c[STRBUFFER_LEN] = {0};

        error = pkmn_pokemon_box_get_game(
                    pokemon_box_list_c.pokemon_boxes[i],
                    game_c,
                    sizeof(game_c)
                );
        BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
        BOOST_CHECK_EQUAL(strcmp(pokemon_box_list_cpp[i]->get_game().c_str(), game_c), 0);

        error = pkmn_pokemon_box_get_name(
                    pokemon_box_list_c.pokemon_boxes[i],
                    name_c,
                    sizeof(name_c)
                );
        BOOST_CHECK_EQUAL(error, PKMN_ERROR_NONE);
        BOOST_CHECK_EQUAL(strcmp(pokemon_box_list_cpp[i]->get_name().c_str(), name_c), 0);
    }

    pkmn_pokemon_box_list_free(
        &pokemon_box_list_c
    );
    BOOST_CHECK(!pokemon_box_list_c.pokemon_boxes);
    BOOST_CHECK_EQUAL(pokemon_box_list_c.length, 0);
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
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn::std_string_to_c_str(
                string_cpp,
                string_c,
                0
            );
    BOOST_CHECK_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    // Full buffer
    error = pkmn::std_string_to_c_str(
                string_cpp,
                string_c,
                sizeof(string_c)
            );
    BOOST_CHECK_EQUAL(string_c, "LibPKMN");
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

BOOST_AUTO_TEST_CASE(std_map_keys_to_string_list_test) {
    static const std::map<std::string, bool> string_bool_map = boost::assign::map_list_of
        ("key1", true)
        ("key2", false)
        ("key3", false)
        ("key4", true)
    ;
    static const std::map<std::string, int> string_int_map = boost::assign::map_list_of
        ("key5", 1)
        ("key6", 2)
        ("key7", 3)
        ("key8", 4)
        ("key9", 5)
    ;

    pkmn_string_list_t string_list_c = {NULL, 0};

    pkmn::std_map_keys_to_string_list<bool>(
        string_bool_map,
        &string_list_c
    );

    BOOST_REQUIRE_EQUAL(string_list_c.length, 4);
    BOOST_CHECK_EQUAL(string_list_c.strings[0], "key1");
    BOOST_CHECK_EQUAL(string_list_c.strings[1], "key2");
    BOOST_CHECK_EQUAL(string_list_c.strings[2], "key3");
    BOOST_CHECK_EQUAL(string_list_c.strings[3], "key4");

    pkmn_string_list_free(&string_list_c);
    BOOST_CHECK(!string_list_c.strings);
    BOOST_CHECK_EQUAL(string_list_c.length, 0);

    pkmn::std_map_keys_to_string_list<int>(
        string_int_map,
        &string_list_c
    );

    BOOST_REQUIRE_EQUAL(string_list_c.length, 5);
    BOOST_CHECK_EQUAL(string_list_c.strings[0], "key5");
    BOOST_CHECK_EQUAL(string_list_c.strings[1], "key6");
    BOOST_CHECK_EQUAL(string_list_c.strings[2], "key7");
    BOOST_CHECK_EQUAL(string_list_c.strings[3], "key8");
    BOOST_CHECK_EQUAL(string_list_c.strings[4], "key9");

    pkmn_string_list_free(&string_list_c);
    BOOST_CHECK(!string_list_c.strings);
    BOOST_CHECK_EQUAL(string_list_c.length, 0);
}

BOOST_AUTO_TEST_CASE(string_vector_cpp_to_c_test) {
    std::vector<std::string> string_vector_cpp;
    string_vector_cpp.emplace_back("LibPKMN");
    string_vector_cpp.emplace_back("PKSav");
    string_vector_cpp.emplace_back("TKO");

    pkmn_string_list_t string_list_c = { NULL, 0 };
    pkmn::std_vector_std_string_to_string_list(
        string_vector_cpp,
        &string_list_c
    );

    BOOST_CHECK_EQUAL(string_list_c.length, 3);
    BOOST_CHECK_EQUAL(string_list_c.strings[0], "LibPKMN");
    BOOST_CHECK_EQUAL(string_list_c.strings[1], "PKSav");
    BOOST_CHECK_EQUAL(string_list_c.strings[2], "TKO");

    pkmn_string_list_free(
        &string_list_c
    );
    BOOST_CHECK(!string_list_c.strings);
    BOOST_CHECK_EQUAL(string_list_c.length, 0);
}
