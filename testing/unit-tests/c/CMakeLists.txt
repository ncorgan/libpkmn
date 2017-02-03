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
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Unity
)

SET(c_tests
    c_calculations_test
    c_database_entry_test
    c_items_test
    c_lists_test
    c_paths_test
    c_pokemon_test
    c_pokemon_party_test
    c_pokemon_pc_test
)

FOREACH(test ${c_tests})
    PKMN_ADD_C_TEST(${test} ${test}.c)
ENDFOREACH(test ${c_tests})

SET(configure_c_tests
    c_game_save_test
    c_error_test
)

FOREACH(test ${configure_c_tests})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${test}.c.in
        ${CMAKE_CURRENT_BINARY_DIR}/${test}.c
    @ONLY)
    PKMN_ADD_C_TEST(${test} ${CMAKE_CURRENT_BINARY_DIR}/${test}.c)
ENDFOREACH(test ${configure_c_tests})

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
