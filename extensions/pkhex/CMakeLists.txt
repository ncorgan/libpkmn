#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(UseCSharp)

GET_FILENAME_COMPONENT(
    PKHEX_DIR
    ${PKHEX_CORE_DLL}
    DIRECTORY
    TRUE
)

SET(pkmn_pkhex_sources
    # Actual sources
    "${CMAKE_CURRENT_SOURCE_DIR}/PKHeXPokemon.cs"

    # Search paths
    "${PKMN_BINARY_DIR}/testing/unit-tests/csharp"
    "${PKHEX_DIR}"

    # Reference DLLs
    "PKMN.CSharp.dll"
    "PKHeX.Core.dll"
)
MESSAGE(STATUS "${assembly_paths}")

SET(CSHARP_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
CSHARP_ADD_LIBRARY(
    "PKMN.PKHeX"
    "${pkmn_pkhex_sources}"
)
ADD_DEPENDENCIES(PKMN.PKHeX PKMN.CSharp)

INSTALL(FILES ${CSHARP_BINARY_DIRECTORY}/PKMN.PKHeX.dll
    DESTINATION bin
    COMPONENT Extensions
)
