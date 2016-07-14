#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Set build type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    SET(PKMN_GCC TRUE)
ELSEIF(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    SET(PKMN_CLANG TRUE)
ENDIF(${CMAKE_C_COMPILER_ID} STREQUAL "GNU")

IF(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    SET(PKMN_64BIT TRUE)
ELSE()
    SET(PKMN_32BIT TRUE)
ENDIF(${CMAKE_SIZEOF_VOID_P} EQUAL 8)

IF(PKMN_GCC OR PKMN_CLANG)
    SET(PKMN_C_FLAGS "-O3 -std=gnu99 -Wall -Wextra -fvisibility=hidden")
ELSEIF(MSVC)
    ADD_DEFINITIONS(/MP)                       # Multi-threaded build
    ADD_DEFINITIONS(/EHsc)                     # Exception handling
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS) # Ignore deprecation warnings
ENDIF(PKMN_GCC OR PKMN_CLANG)

# Checks for required headers
#INCLUDE(CheckIncludeFile)
#SET(CMAKE_REQUIRED_FLAGS "${PKMN_C_FLAGS}")
#CHECK_INCLUDE_FILE(stdbool.h HAVE_STDBOOL_H)
#IF(NOT HAVE_STDBOOL_H)
#    MESSAGE(FATAL_ERROR "LibPKMN requires the header stdbool.h to compile.")
#ENDIF(NOT HAVE_STDBOOL_H)
#CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)
#IF(NOT HAVE_STDINT_H)
#    MESSAGE(FATAL_ERROR "LibPKMN requires the header stdint.h to compile.")
#ENDIF(NOT HAVE_STDINT_H)

# Set compiler name for CMake display
IF(MSVC)
    IF(MSVC12)
        SET(PKMN_COMPILER_NAME "Microsoft Visual Studio 2013")
    ELSEIF(MSVC14)
        SET(PKMN_COMPILER_NAME "Microsoft Visual Studio 2015")
    ELSE()
        SET(PKMN_COMPILER_NAME "${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
    ENDIF(MSVC12)
ELSEIF(MINGW)
    SET(PKMN_COMPILER_NAME "MinGW-${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
ELSE()
    SET(PKMN_COMPILER_NAME "${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
ENDIF(MSVC)

MESSAGE(STATUS "")
