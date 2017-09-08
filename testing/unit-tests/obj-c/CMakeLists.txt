#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Unity
)

SET(objc_tests
    ObjCListsTest
)

FOREACH(test ${objc_tests})
    PKMN_ADD_OBJC_TEST(${test} ${test}.m)
ENDFOREACH(test ${objc_tests})
