#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(PKMN_USE_CLANGTIDY "${PKMN_USE_CLANGTIDY}" CACHE BOOL "Enable Clang-tidy testing")

IF(PKMN_USE_CLANGTIDY)
    if(NOT CLANG_TIDY_EXECUTABLE)
        FIND_PROGRAM(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)
        MESSAGE(STATUS "Using clang-tidy: ${CLANG_TIDY_EXECUTABLE}")
    ENDIF()

    SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    SET(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
    SET(CLANG_TIDY_PROPERTY
        "${CLANG_TIDY_EXECUTABLE}"
        "-checks=\
         clang-diagnostic-*,\
         misc-macro-parentheses,google-readability-casting,\
         modernize-loop-convert,readability-else-after-return"
    )
ENDIF()
