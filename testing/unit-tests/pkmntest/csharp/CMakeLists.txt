#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pkmntest_cs_srcs
    Gen1ItemsTests.cs
    Gen1PokemonTests.cs
    Gen2ItemsTests.cs
    Gen2PokemonTests.cs
    Gen3PokemonTests.cs
    GBAItemsTests.cs
    GameSaveTest.cs
    ItemsTestsCommon.cs
    PokemonIOTests.cs
    PokemonPartyTests.cs
    PokemonPCTests.cs
    PokemonTestsCommon.cs
)
FOREACH(src ${pkmntest_cs_srcs})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${src}
        ${CMAKE_CURRENT_BINARY_DIR}/${src}
    @ONLY)
ENDFOREACH(src ${pkmntest_cs_srcs})

SET(CSHARP_BINARY_DIRECTORY ${PKMN_BINARY_DIR}/testing/unit-tests/csharp)

CSHARP_ADD_LIBRARY(
    "pkmntest-cs"
    "${CMAKE_CURRENT_BINARY_DIR}/*.cs"
    ${PKMN_BINARY_DIR}/testing/unit-tests/csharp/pkmn-cs.dll
    ${NUNIT_LIBRARIES}
)
ADD_DEPENDENCIES(pkmntest-cs pkmn-cs)
