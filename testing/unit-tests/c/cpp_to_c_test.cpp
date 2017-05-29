/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/thread/mutex.hpp>

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

pkmn_error_t throw_pkmn_unimplemented_error() {
    PKMN_CPP_TO_C(
        throw pkmn::unimplemented_error();
    )
}

TEST(cpp_to_c_test, exception_to_error_code_test) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = throw_nothing();
    EXPECT_EQ(PKMN_ERROR_NONE, error);
    EXPECT_STREQ("None", pkmn_strerror());

    error = throw_feature_not_in_game_error("Contests", "Generation I");
    EXPECT_EQ(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    EXPECT_STREQ("Contests not in Generation I", pkmn_strerror());

    error = throw_exception<pkmn::feature_not_in_game_error>("feature_not_in_game_error");
    EXPECT_EQ(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    EXPECT_STREQ("feature_not_in_game_error", pkmn_strerror());

    error = throw_pksav_error(PKSAV_ERROR_INVALID_SAVE);
    EXPECT_EQ(PKMN_ERROR_PKSAV_ERROR, error);
    EXPECT_STREQ("PKSav returned the following error: \"Invalid save file\"", pkmn_strerror());

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

pkmn_error_t throw_pkmn_unimplemented_error_with_handle(
    pkmn_test_handle_t* handle
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        throw pkmn::unimplemented_error();
    )
}

TEST(cpp_to_c_test, exception_to_error_code_with_handle_test) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_test_handle_t test_handle;

    error = throw_nothing_with_handle(&test_handle);
    EXPECT_EQ(PKMN_ERROR_NONE, error);
    EXPECT_EQ("None", test_handle.last_error);

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
    EXPECT_EQ(PKMN_ERROR_PKSAV_ERROR, error);
    EXPECT_EQ("PKSav returned the following error: \"Invalid save file\"", test_handle.last_error);
    EXPECT_STREQ("PKSav returned the following error: \"Invalid save file\"", pkmn_strerror());

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

TEST(cpp_to_c_test, hidden_power_cpp_to_c_test) {
    pkmn::calculations::hidden_power hidden_power_cpp("Normal", 90);

    pkmn_hidden_power_t hidden_power_c;

    pkmn::pkmn_hidden_power_cpp_to_c(
        hidden_power_cpp,
        &hidden_power_c
    );

    EXPECT_STREQ("Normal", hidden_power_c.type);
    EXPECT_EQ(90, hidden_power_c.base_power);

    pkmn_hidden_power_free(&hidden_power_c);
    EXPECT_EQ(NULL, hidden_power_c.type);
}

TEST(cpp_to_c_test, item_slot_cpp_to_c_test) {
    pkmn::item_slot item_slot_cpp(
        pkmn::database::item_entry("Potion", "Red"),
        50
    );

    pkmn_item_slot_t item_slot_c;

    pkmn::pkmn_item_slot_cpp_to_c(
        item_slot_cpp,
        &item_slot_c
    );

    EXPECT_STREQ("Potion", item_slot_c.item);
    EXPECT_EQ(50, item_slot_c.amount);

    pkmn_item_slot_free(&item_slot_c);
    EXPECT_EQ(NULL, item_slot_c.item);
    EXPECT_EQ(0, item_slot_c.amount);
}

TEST(cpp_to_c_test, item_slots_cpp_to_c_test) {
    pkmn::item_slots_t item_slots_cpp{
        pkmn::item_slot(
            pkmn::database::item_entry("Potion", "Red"),
            50
        ),
        pkmn::item_slot(
            pkmn::database::item_entry("Berry", "Silver"),
            28
        ),
        pkmn::item_slot(
            pkmn::database::item_entry("Berry Pouch", "LeafGreen"),
            1
        )
    };

    pkmn_item_slots_t item_slots_c = { NULL, 0 };
    pkmn::pkmn_item_slots_cpp_to_c(
        item_slots_cpp,
        &item_slots_c
    );

    EXPECT_EQ(3, item_slots_c.length);
    EXPECT_STREQ("Potion", item_slots_c.item_slots[0].item);
    EXPECT_EQ(50, item_slots_c.item_slots[0].amount);
    EXPECT_STREQ("Berry", item_slots_c.item_slots[1].item);
    EXPECT_EQ(28, item_slots_c.item_slots[1].amount);
    EXPECT_STREQ("Berry Pouch", item_slots_c.item_slots[2].item);
    EXPECT_EQ(1, item_slots_c.item_slots[2].amount);

    pkmn_item_slots_free(
        &item_slots_c
    );
    EXPECT_EQ(NULL, item_slots_c.item_slots);
    EXPECT_EQ(0, item_slots_c.length);
}

TEST(cpp_to_c_test, levelup_move_cpp_to_c_test) {
    pkmn::database::levelup_move levelup_move_cpp(
        pkmn::database::move_entry("Scratch", "Red"),
        50
    );

    pkmn_levelup_move_t levelup_move_c;

    pkmn::pkmn_levelup_move_cpp_to_c(
        levelup_move_cpp,
        &levelup_move_c
    );

    EXPECT_STREQ("Scratch", levelup_move_c.move);
    EXPECT_EQ(50, levelup_move_c.level);

    pkmn_levelup_move_free(&levelup_move_c);
    EXPECT_EQ(NULL, levelup_move_c.move);
    EXPECT_EQ(0, levelup_move_c.level);
}

TEST(cpp_to_c_test, levelup_moves_cpp_to_c_test) {
    pkmn::database::levelup_moves_t levelup_moves_cpp{
        pkmn::database::levelup_move(
            pkmn::database::move_entry("Scratch", "Red"),
            50
        ),
        pkmn::database::levelup_move(
            pkmn::database::move_entry("Synthesis", "Silver"),
            5
        ),
        pkmn::database::levelup_move(
            pkmn::database::move_entry("Frenzy Plant", "Emerald"),
            37
        )
    };

    pkmn_levelup_moves_t levelup_moves_c = { NULL, 0 };
    pkmn::pkmn_levelup_moves_cpp_to_c(
        levelup_moves_cpp,
        &levelup_moves_c
    );

    EXPECT_EQ(3, levelup_moves_c.length);
    EXPECT_STREQ("Scratch", levelup_moves_c.levelup_moves[0].move);
    EXPECT_EQ(50, levelup_moves_c.levelup_moves[0].level);
    EXPECT_STREQ("Synthesis", levelup_moves_c.levelup_moves[1].move);
    EXPECT_EQ(5, levelup_moves_c.levelup_moves[1].level);
    EXPECT_STREQ("Frenzy Plant", levelup_moves_c.levelup_moves[2].move);
    EXPECT_EQ(37, levelup_moves_c.levelup_moves[2].level);

    pkmn_levelup_moves_free(
        &levelup_moves_c
    );
    EXPECT_EQ(NULL, levelup_moves_c.levelup_moves);
    EXPECT_EQ(0, levelup_moves_c.length);
}

TEST(cpp_to_c_test, move_list_cpp_to_c_test) {
    pkmn::database::move_list_t move_list_cpp{
        pkmn::database::move_entry("Scratch", "Red"),
        pkmn::database::move_entry("Synthesis", "Silver"),
        pkmn::database::move_entry("Frenzy Plant", "Emerald")
    };

    pkmn_string_list_t string_list_c = { NULL, 0 };
    pkmn::pkmn_move_list_to_string_list(
        move_list_cpp,
        &string_list_c
    );

    EXPECT_EQ(3, string_list_c.length);
    EXPECT_STREQ("Scratch", string_list_c.strings[0]);
    EXPECT_STREQ("Synthesis", string_list_c.strings[1]);
    EXPECT_STREQ("Frenzy Plant", string_list_c.strings[2]);

    pkmn_string_list_free(
        &string_list_c
    );
    EXPECT_EQ(NULL, string_list_c.strings);
    EXPECT_EQ(0, string_list_c.length);
}

TEST(cpp_to_c_test, move_slot_cpp_to_c_test) {
    pkmn::move_slot move_slot_cpp(
        "Tackle",
        50
    );

    pkmn_move_slot_t move_slot_c;

    pkmn::pkmn_move_slot_cpp_to_c(
        move_slot_cpp,
        &move_slot_c
    );

    EXPECT_STREQ("Tackle", move_slot_c.move);
    EXPECT_EQ(50, move_slot_c.pp);

    pkmn_move_slot_free(&move_slot_c);
    EXPECT_EQ(NULL, move_slot_c.move);
    EXPECT_EQ(0, move_slot_c.pp);
}

TEST(cpp_to_c_test, move_slots_cpp_to_c_test) {
    pkmn::move_slots_t move_slots_cpp{
        pkmn::move_slot(
            "Tackle",
            50
        ),
        pkmn::move_slot(
            "Pound",
            28
        ),
        pkmn::move_slot(
            "Metronome",
            1
        )
    };

    pkmn_move_slots_t move_slots_c = { NULL, 0 };
    pkmn::pkmn_move_slots_cpp_to_c(
        move_slots_cpp,
        &move_slots_c
    );

    EXPECT_EQ(3, move_slots_c.length);
    EXPECT_STREQ("Tackle", move_slots_c.move_slots[0].move);
    EXPECT_EQ(50, move_slots_c.move_slots[0].pp);
    EXPECT_STREQ("Pound", move_slots_c.move_slots[1].move);
    EXPECT_EQ(28, move_slots_c.move_slots[1].pp);
    EXPECT_STREQ("Metronome", move_slots_c.move_slots[2].move);
    EXPECT_EQ(1, move_slots_c.move_slots[2].pp);

    pkmn_move_slots_free(
        &move_slots_c
    );
    EXPECT_EQ(NULL, move_slots_c.move_slots);
    EXPECT_EQ(0, move_slots_c.length);
}

TEST(cpp_to_c_test, pokemon_entries_cpp_to_c_test) {
    pkmn::database::pokemon_entries_t pokemon_entries_cpp{
        pkmn::database::pokemon_entry("Charmander", "Red", ""),
        pkmn::database::pokemon_entry("Totodile", "Gold", ""),
        pkmn::database::pokemon_entry("Treecko", "Ruby", "")
    };

    pkmn_string_list_t string_list_c = { NULL, 0 };
    pkmn::pkmn_pokemon_entries_to_string_list(
        pokemon_entries_cpp,
        &string_list_c
    );

    EXPECT_EQ(3, string_list_c.length);
    EXPECT_STREQ("Charmander", string_list_c.strings[0]);
    EXPECT_STREQ("Totodile", string_list_c.strings[1]);
    EXPECT_STREQ("Treecko", string_list_c.strings[2]);

    pkmn_string_list_free(
        &string_list_c
    );
    EXPECT_EQ(NULL, string_list_c.strings);
    EXPECT_EQ(0, string_list_c.length);
}

TEST(cpp_to_c_test, pokemon_list_cpp_to_c) {
    pkmn::pokemon_list_t pokemon_list_cpp{
        pkmn::pokemon::make(
            "Charmander", "Red", "", 5
        ),
        pkmn::pokemon::make(
            "Squirtle", "Blue", "", 10
        ),
        pkmn::pokemon::make(
            "Bulbasaur", "Yellow", "", 15
        )
    };

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_list_t pokemon_list_c = { NULL, 0 };
    pkmn::pkmn_pokemon_list_cpp_to_c(
        pokemon_list_cpp,
        &pokemon_list_c
    );
    EXPECT_EQ(3, pokemon_list_c.length);

    for(size_t i = 0; i < 3; ++i) {
        char species_c[STRBUFFER_LEN] = {0};
        char game_c[STRBUFFER_LEN] = {0};
        int level_c = 0;

        error = pkmn_pokemon_get_species(
                    pokemon_list_c.pokemon_list[i],
                    species_c,
                    sizeof(species_c)
                );
        EXPECT_EQ(PKMN_ERROR_NONE, error);
        EXPECT_STREQ(pokemon_list_cpp[i]->get_species().c_str(), species_c);

        error = pkmn_pokemon_get_game(
                    pokemon_list_c.pokemon_list[i],
                    game_c,
                    sizeof(game_c)
                );
        EXPECT_EQ(PKMN_ERROR_NONE, error);
        EXPECT_STREQ(pokemon_list_cpp[i]->get_game().c_str(), game_c);

        error = pkmn_pokemon_get_level(
                    pokemon_list_c.pokemon_list[i],
                    &level_c
                );
        EXPECT_EQ(PKMN_ERROR_NONE, error);
        EXPECT_EQ(level_c, pokemon_list_cpp[i]->get_level());
    }

    pkmn_pokemon_list_free(
        &pokemon_list_c
    );
    EXPECT_EQ(NULL, pokemon_list_c.pokemon_list);
    EXPECT_EQ(0, pokemon_list_c.length);
}

TEST(cpp_to_c_test, pokemon_box_list_cpp_to_c_test) {
    pkmn::pokemon_box_list_t pokemon_box_list_cpp{
        pkmn::pokemon_box::make("Gold"),
        pkmn::pokemon_box::make("Ruby"),
        pkmn::pokemon_box::make("FireRed")
    };

    pokemon_box_list_cpp[0]->set_name("ABCD");
    pokemon_box_list_cpp[1]->set_name("EFGH");
    pokemon_box_list_cpp[2]->set_name("IJKL");

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_box_list_t pokemon_box_list_c = { NULL, 0 };
    pkmn::pkmn_pokemon_box_list_cpp_to_c(
        pokemon_box_list_cpp,
        &pokemon_box_list_c
    );
    EXPECT_EQ(3, pokemon_box_list_c.length);

    for(size_t i = 0; i < 3; ++i) {
        char game_c[STRBUFFER_LEN] = {0};
        char name_c[STRBUFFER_LEN] = {0};

        error = pkmn_pokemon_box_get_game(
                    pokemon_box_list_c.pokemon_boxes[i],
                    game_c,
                    sizeof(game_c)
                );
        EXPECT_EQ(PKMN_ERROR_NONE, error);
        EXPECT_STREQ(pokemon_box_list_cpp[i]->get_game().c_str(), game_c);

        error = pkmn_pokemon_box_get_name(
                    pokemon_box_list_c.pokemon_boxes[i],
                    name_c,
                    sizeof(name_c)
                );
        EXPECT_EQ(PKMN_ERROR_NONE, error);
        EXPECT_STREQ(pokemon_box_list_cpp[i]->get_name().c_str(), name_c);
    }

    pkmn_pokemon_box_list_free(
        &pokemon_box_list_c
    );
    EXPECT_EQ(NULL, pokemon_box_list_c.pokemon_boxes);
    EXPECT_EQ(0, pokemon_box_list_c.length);
}

TEST(cpp_to_c_test, int_pair_cpp_to_c_test) {
    std::pair<int, int> int_pair_cpp(6322, 10011);
    pkmn_int_pair_t int_pair_c;

    pkmn::std_pair_int_to_int_pair(
        int_pair_cpp,
        &int_pair_c
    );

    EXPECT_EQ(6322, int_pair_c.first);
    EXPECT_EQ(10011, int_pair_c.second);
}

TEST(cpp_to_c_test, std_string_cpp_to_c_test) {
    std::string string_cpp = "LibPKMN";
    char string_c[8] = "";
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn::std_string_to_c_str(
                string_cpp,
                string_c,
                0
            );
    EXPECT_EQ(PKMN_ERROR_BUFFER_TOO_SMALL, error);

    // Full buffer
    error = pkmn::std_string_to_c_str(
                string_cpp,
                string_c,
                sizeof(string_c)
            );
    EXPECT_STREQ("LibPKMN", string_c);
}

TEST(cpp_to_c_test, string_pair_cpp_to_c_test) {
    std::pair<std::string, std::string> string_pair_cpp("LibPKMN", "PKSav");
    pkmn_string_pair_t string_pair_c;

    pkmn::std_pair_std_string_to_string_pair(
        string_pair_cpp,
        &string_pair_c
    );

    EXPECT_STREQ("LibPKMN", string_pair_c.first);
    EXPECT_STREQ("PKSav", string_pair_c.second);

    pkmn_string_pair_free(&string_pair_c);
    EXPECT_EQ(NULL, string_pair_c.first);
    EXPECT_EQ(NULL, string_pair_c.second);
}

TEST(cpp_to_c_test, std_map_keys_to_string_list_test) {
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

    ASSERT_EQ(4, string_list_c.length);
    EXPECT_STREQ("key1", string_list_c.strings[0]);
    EXPECT_STREQ("key2", string_list_c.strings[1]);
    EXPECT_STREQ("key3", string_list_c.strings[2]);
    EXPECT_STREQ("key4", string_list_c.strings[3]);

    pkmn_string_list_free(&string_list_c);
    EXPECT_EQ(NULL, string_list_c.strings);
    EXPECT_EQ(0, string_list_c.length);

    pkmn::std_map_keys_to_string_list<int>(
        string_int_map,
        &string_list_c
    );

    ASSERT_EQ(5, string_list_c.length);
    EXPECT_STREQ("key5", string_list_c.strings[0]);
    EXPECT_STREQ("key6", string_list_c.strings[1]);
    EXPECT_STREQ("key7", string_list_c.strings[2]);
    EXPECT_STREQ("key8", string_list_c.strings[3]);
    EXPECT_STREQ("key9", string_list_c.strings[4]);

    pkmn_string_list_free(&string_list_c);
    EXPECT_EQ(NULL, string_list_c.strings);
    EXPECT_EQ(0, string_list_c.length);
}

TEST(cpp_to_c_test, string_vector_cpp_to_c_test) {
    std::vector<std::string> string_vector_cpp{"LibPKMN", "PKSav", "TKO"};

    pkmn_string_list_t string_list_c = { NULL, 0 };
    pkmn::std_vector_std_string_to_string_list(
        string_vector_cpp,
        &string_list_c
    );

    EXPECT_EQ(3, string_list_c.length);
    EXPECT_STREQ("LibPKMN", string_list_c.strings[0]);
    EXPECT_STREQ("PKSav", string_list_c.strings[1]);
    EXPECT_STREQ("TKO", string_list_c.strings[2]);

    pkmn_string_list_free(
        &string_list_c
    );
    EXPECT_EQ(NULL, string_list_c.strings);
    EXPECT_EQ(0, string_list_c.length);
}
