#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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
        CSharpItemsTest
        CSharpListsTest
        CSharpPathsTest
        CSharpPokemonTest
        CSharpWinFormsTest_GUI
    )

    SET(csharp_pkmn_dlls
        "${CMAKE_CURRENT_BINARY_DIR}/pkmn-cs.dll;${CMAKE_CURRENT_BINARY_DIR}/pkmntest-cs.dll"
    )

    FOREACH(test ${csharp_tests})
        PKMN_ADD_CSHARP_TEST(
            ${test}
            ${CMAKE_CURRENT_SOURCE_DIR}/${test}.cs
            "${csharp_pkmn_dlls}"
        )
    ENDFOREACH(test ${csharp_tests})
ENDIF(NUNIT_FOUND)
