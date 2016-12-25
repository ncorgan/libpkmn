#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(CSHARP_BINARY_DIRECTORY ${PKMN_BINARY_DIR}/testing/unit-tests)

CSHARP_ADD_LIBRARY(
    "pkmntest-cs"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cs"
    ${PKMN_BINARY_DIR}/testing/unit-tests/pkmn-cs.dll
    ${NUNIT_LIBRARIES}
)
ADD_DEPENDENCIES(pkmntest-cs pkmn-cs)
