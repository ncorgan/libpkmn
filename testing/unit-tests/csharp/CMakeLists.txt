#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# C# tests
#
FIND_PACKAGE(NUnit)
IF(NUNIT_FOUND)
    # To get CSHARP_INTERPRETER
    FIND_PACKAGE(CSharp)
    SET(CSHARP_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    SET(csharp_tests
        CSharpBuildInfoTest
        CSharpCalculationsTest
        CSharpDatabaseEntryTest
        CSharpEqualityHashCodeTest
        CSharpGameSaveTest
        CSharpItemsTest
        CSharpListsTest
        CSharpPathsTest
        CSharpPokemonTest
        CSharpPokemonPartyTest
        CSharpPokemonPCTest
        CSharpGUITest_GUI
    )

    SET(csharp_pkmn_dlls
        "${CMAKE_CURRENT_BINARY_DIR}/PKMN.CSharp.dll;${CMAKE_CURRENT_BINARY_DIR}/PKMNTest.CSharp.dll"
    )

    FOREACH(test ${csharp_tests})
        PKMN_ADD_CSHARP_TEST(
            ${test}
            ${CMAKE_CURRENT_SOURCE_DIR}/${test}.cs
            "${csharp_pkmn_dlls}"
        )
    ENDFOREACH(test ${csharp_tests})
ENDIF()
