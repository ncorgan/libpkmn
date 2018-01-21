#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# C tests
#
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKMN_SOURCE_DIR}/pksav/include
    ${PKMN_BINARY_DIR}/pksav/include
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Unity
)

SET(c_tests
    c_buildinfo_test
    c_calculations_test
    c_database_entry_test
    c_error_test
    #c_game_save_test
    c_gui_test
    c_lists_test
    c_paths_test
    c_pokemon_party_test
    c_pokemon_pc_test
)

FOREACH(test ${c_tests})
    PKMN_ADD_C_TEST(${test} ${test}.c)
ENDFOREACH(test ${c_tests})

SET(items_test_srcs
    c_items_test/gen1_items_tests.c
    c_items_test/gen2_items_tests.c
    c_items_test/gba_items_tests.c
    c_items_test/gcn_items_tests.c
    c_items_test/items_tests_common.c
    c_items_test/main.c
)
#PKMN_ADD_C_TEST(c_items_test "${items_test_srcs}")

SET(pokemon_test_srcs
    c_pokemon_test/gen1_pokemon_test.c
    c_pokemon_test/gen2_pokemon_test.c
    c_pokemon_test/gen3_pokemon_test.c
    c_pokemon_test/pokemon_test.c
    c_pokemon_test/form_test.c
    c_pokemon_test/gen1_friendship_test.c
    c_pokemon_test/gender_test.c
    c_pokemon_test/pokemon_io_test.c
    c_pokemon_test/unown_tests.c
    c_pokemon_test/main.c
)
PKMN_ADD_C_TEST(c_pokemon_test "${pokemon_test_srcs}")

# Separately add C++ -> C test
SET(cpp_to_c_test_srcs
    ${PKMN_SOURCE_DIR}/lib/c/cpp_to_c.cpp
    ${PKMN_SOURCE_DIR}/lib/c/error.cpp
    cpp_to_c_test.cpp
)
PKMN_ADD_CPP_TEST(cpp_to_c_test "${cpp_to_c_test_srcs}")
TARGET_LINK_LIBRARIES(cpp_to_c_test
    pkmn-c
)
