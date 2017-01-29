#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(PKMNPlatform)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_SOURCE_DIR}/testing/Unity
    ${PKMN_BINARY_DIR}/include
    ${PKMN_SOURCE_DIR}/pksav/include
    ${PKMN_BINARY_DIR}/pksav/include
)

SET(pkmntest_c_libs
    unity
    pkmn-c
)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pokemon_io_tests.c
    ${CMAKE_CURRENT_BINARY_DIR}/pokemon_io_tests.c
@ONLY)

SET(pkmntest_c_sources
    gen1_items_tests.c
    gen1_pokemon_tests.c
    gen2_items_tests.c
    gen2_pokemon_tests.c
    gba_items_tests.c
    gba_pokemon_tests.c
    items_tests_common.c
    ${CMAKE_CURRENT_BINARY_DIR}/pokemon_io_tests.c
    pokemon_tests_common.c
    pokemon_pc_tests.c
)

SET_SOURCE_FILES_PROPERTIES(${pkmntest_c_sources}
    PROPERTIES COMPILE_FLAGS "${PKMN_C_FLAGS}"
)

ADD_LIBRARY(pkmntest-c SHARED ${pkmntest_c_sources})
TARGET_LINK_LIBRARIES(pkmntest-c ${pkmntest_c_libs})
SET_TARGET_PROPERTIES(pkmntest-c PROPERTIES DEFINE_SYMBOL "PKMNTEST_DLL_EXPORTS")
ADD_DEFINITIONS(-Dpkmntest_EXPORTS)
