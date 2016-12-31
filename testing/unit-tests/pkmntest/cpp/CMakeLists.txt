#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(PKMNPlatform)

INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
    ${PKMN_SOURCE_DIR}/pksav/include
    ${PKMN_BINARY_DIR}/pksav/include
)

SET(pkmntest_libs
    ${Boost_LIBRARIES}
    pkmn
)

SET(pkmntest_sources
    gen1_items_tests.cpp
    gen1_pokemon_tests.cpp
    gen2_items_tests.cpp
    gen2_pokemon_tests.cpp
    gba_items_tests.cpp
    gba_pokemon_tests.cpp
    items_tests_common.cpp
    pokemon_tests_common.cpp
)

SET_SOURCE_FILES_PROPERTIES(${pkmntest_sources}
    PROPERTIES COMPILE_FLAGS "${PKMN_CXX_FLAGS}"
)

ADD_LIBRARY(pkmntest SHARED ${pkmntest_sources})
TARGET_LINK_LIBRARIES(pkmntest ${pkmntest_libs})
SET_TARGET_PROPERTIES(pkmntest PROPERTIES DEFINE_SYMBOL "PKMN_DLL_EXPORTS")
ADD_DEFINITIONS(-Dpkmn_EXPORTS)
