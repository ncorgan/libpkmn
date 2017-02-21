#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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
    gtest
    pkmn
)

SET(pkmntest_sources
    item_test.cpp
    pokemon_test.cpp
    pokemon_pc_test.cpp
)

# Ignore overzealous error when comparing a size_t to a numeric literal.
IF(PKMN_GCC)
    SET_SOURCE_FILES_PROPERTIES(${pkmntest_sources}
        PROPERTIES COMPILE_FLAGS "-Wno-sign-compare ${PKMN_CXX_FLAGS}"
    )
ELSEIF(PKMN_CLANG)
    SET_SOURCE_FILES_PROPERTIES(${pkmntest_sources}
        PROPERTIES COMPILE_FLAGS "${PKMN_CXX_FLAGS} -Wno-error -Wno-sign-compare -Werror"
    )
ELSE()
    SET_SOURCE_FILES_PROPERTIES(${pkmntest_sources}
        PROPERTIES COMPILE_FLAGS "${PKMN_CXX_FLAGS}"
    )
ENDIF(PKMN_GCC)

ADD_LIBRARY(pkmntest SHARED ${pkmntest_sources})
TARGET_LINK_LIBRARIES(pkmntest ${pkmntest_libs})
SET_TARGET_PROPERTIES(pkmntest PROPERTIES DEFINE_SYMBOL "PKMNTEST_DLL_EXPORTS")
ADD_DEFINITIONS(-Dpkmntest_EXPORTS)
