#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(PKMNUnitTest)
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

SET(cpp_tests
    cpp_calculations_test
    cpp_lists_test
)

FOREACH(test ${cpp_tests})
    PKMN_ADD_CPP_TEST(${test} ${test}.cpp)
ENDFOREACH(test ${cpp_tests})

IF(PKMN_ENABLE_C)
    ADD_SUBDIRECTORY(Unity)

    INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_SOURCE_DIR}/Unity
    )

    SET(c_tests
        c_lists_test
    )

    FOREACH(test ${c_tests})
        PKMN_ADD_C_TEST(${test} ${test}.c)
    ENDFOREACH(test ${c_tests})
ENDIF(PKMN_ENABLE_C)
