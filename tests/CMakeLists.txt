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

# Tests with multiple files have to be done separately

SET(cpp_items_test_srcs
    cpp_items_test/cpp_items_test.cpp
    cpp_items_test/gen1_items_tests.cpp
)
PKMN_ADD_CPP_TEST(cpp_items_test "${cpp_items_test_srcs}")
