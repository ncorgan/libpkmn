#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(PKMN_ENABLE_QT)
    CMAKE_POLICY(SET CMP0020 NEW)
ENDIF(PKMN_ENABLE_QT)

INCLUDE(PKMNUnitTest)
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

ADD_SUBDIRECTORY(pkmntest)

#
# C++ tests
#
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmntest/include
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

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_game_save_detect_test.cpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/cpp_game_save_detect_test.cpp
@ONLY)
PKMN_ADD_CPP_TEST(cpp_game_save_detect_test ${CMAKE_CURRENT_BINARY_DIR}/cpp_game_save_detect_test.cpp)

IF(PKMN_ENABLE_QT)
    INCLUDE_DIRECTORIES(
        ${QTx_INCLUDE_DIRS}
    )
    LIST(APPEND pkmn_cpp_test_libs
        ${QTx_LIBRARIES}
    )
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    PKMN_ADD_CPP_TEST(cpp_QtWidgetsTest cpp_QtWidgetsTest/cpp_QtWidgetsTest.cpp)
    SET_TARGET_PROPERTIES(cpp_QtWidgetsTest PROPERTIES AUTOMOC ON)
ENDIF(PKMN_ENABLE_QT)

#
# C tests
#
IF(PKMN_ENABLE_C)
    INCLUDE_DIRECTORIES(
        ${PKMN_SOURCE_DIR}/pksav/include
        ${PKMN_BINARY_DIR}/pksav/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../Unity
    )

    SET(c_tests
        c_calculations_test
        c_database_test
        c_items_test
        c_lists_test
        c_paths_test
    )

    FOREACH(test ${c_tests})
        PKMN_ADD_C_TEST(${test} ${test}.c)
    ENDFOREACH(test ${c_tests})

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
ENDIF(PKMN_ENABLE_C)

#
# Python tests
#
IF(PKMN_ENABLE_PYTHON)
    SET(python_tests
        python_calculations_test
        python_database_entry_test
        python_detect_game_save_type_test
        python_items_test
        python_lists_test
        python_paths_test
    )

    FOREACH(test ${python_tests})
        PKMN_ADD_PYTHON_TEST(${test})
    ENDFOREACH(test ${python_tests})
ENDIF(PKMN_ENABLE_PYTHON)
