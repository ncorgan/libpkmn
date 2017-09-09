#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Unity
    ${PKMN_SOURCE_DIR}/include/PKMN-ObjC
    ${PKMN_SOURCE_DIR}/lib/obj-c
    ${ObjC_INCLUDE_DIRS}
)

SET(objc_tests
    ObjCListsTest
)

FOREACH(test ${objc_tests})
    PKMN_ADD_OBJC_TEST(${test} ${test}.m)
ENDFOREACH(test ${objc_tests})

PKMN_ADD_CPP_TEST(CppToObjCTest CppToObjCTest.mm)
TARGET_LINK_LIBRARIES(CppToObjCTest
    pkmn-objc
)
