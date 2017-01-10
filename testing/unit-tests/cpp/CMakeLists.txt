#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
)

SET(cpp_tests
    cpp_calculations_test
    cpp_items_test
    cpp_lists_test
    cpp_paths_test
    cpp_pokemon_test
    cpp_pokemon_pc_test
)

FOREACH(test ${cpp_tests})
    PKMN_ADD_CPP_TEST(${test} ${test}.cpp)
ENDFOREACH(test ${cpp_tests})

SET(database_entry_test_srcs
    cpp_database_entry_test/item_entry_tests.cpp
    cpp_database_entry_test/move_entry_tests.cpp
    cpp_database_entry_test/pokemon_entry_tests.cpp
    cpp_database_entry_test/cpp_database_entry_test.cpp
)
PKMN_ADD_CPP_TEST(cpp_database_entry_test "${database_entry_test_srcs}")

IF(PKMN_ENABLE_QT)
    INCLUDE_DIRECTORIES(
        ${QTx_INCLUDE_DIRS}
    )
    LIST(APPEND pkmn_cpp_test_libs
        ${QTx_LIBRARIES}
    )
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    PKMN_ADD_CPP_TEST(cpp_QtWidgetsTest_GUI cpp_QtWidgetsTest/cpp_QtWidgetsTest.cpp)
    SET_TARGET_PROPERTIES(cpp_QtWidgetsTest_GUI PROPERTIES AUTOMOC ON)
ENDIF(PKMN_ENABLE_QT)
