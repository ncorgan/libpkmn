#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(PKMN_ENABLE_QT)
    CMAKE_POLICY(SET CMP0020 NEW)
ENDIF(PKMN_ENABLE_QT)


#
# C++ tests
#
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${PKMN_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_database_entry_test
    ${Boost_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

SET(cpp_tests
    cpp_calculations_test
    cpp_exception_test
    cpp_game_save_test
    cpp_lists_test
    cpp_paths_test
    cpp_pokedex_test
    cpp_pokemon_party_test
    cpp_pokemon_pc_test
    cpp_swig_wrapper_test
)

FOREACH(test ${cpp_tests})
    PKMN_ADD_CPP_TEST(${test} ${test}.cpp)
ENDFOREACH(test ${boost_cpp_tests})

SET(database_entry_test_srcs
    cpp_database_entry_test/item_entry_test.cpp
    cpp_database_entry_test/move_entry_test.cpp
    cpp_database_entry_test/pokemon_entry_test.cpp
)
PKMN_ADD_CPP_TEST(cpp_database_entry_test "${database_entry_test_srcs}")

SET(item_test_srcs
    cpp_item_test/gen1_item_test.cpp
    cpp_item_test/gen2_item_test.cpp
    cpp_item_test/gba_item_test.cpp
    cpp_item_test/gcn_item_test.cpp
    cpp_item_test/item_test_common.cpp
)
PKMN_ADD_CPP_TEST(cpp_item_test "${item_test_srcs}")

SET(pokemon_test_srcs
    cpp_pokemon_test/conversions_test.cpp
    cpp_pokemon_test/form_test.cpp
    cpp_pokemon_test/gen1_pokemon_test.cpp
    cpp_pokemon_test/gen2_pokemon_test.cpp
    cpp_pokemon_test/gen3_pokemon_tests.cpp
    cpp_pokemon_test/gen1_friendship_test.cpp
    cpp_pokemon_test/gender_test.cpp
    cpp_pokemon_test/pokemon_io_test.cpp
    cpp_pokemon_test/unown_tests.cpp
    cpp_pokemon_test/pokemon_test_common.cpp
)
PKMN_ADD_CPP_TEST(cpp_pokemon_test "${pokemon_test_srcs}")

IF(PKMN_ENABLE_QT)
    INCLUDE_DIRECTORIES(
        ${QTx_INCLUDE_DIRS}
    )
    LIST(APPEND pkmn_cpp_test_libs
        ${QTx_LIBRARIES}
    )
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    PKMN_ADD_CPP_TEST(cpp_QtTest_GUI cpp_QtTest/cpp_QtTest.cpp)
    SET_TARGET_PROPERTIES(cpp_QtTest_GUI PROPERTIES AUTOMOC ON)
ENDIF(PKMN_ENABLE_QT)
