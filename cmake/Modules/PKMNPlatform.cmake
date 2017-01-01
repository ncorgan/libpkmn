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

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(PKMN_GCC TRUE)
ELSEIF(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    SET(PKMN_CLANG TRUE)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    SET(PKMN_64BIT TRUE)
ELSE()
    SET(PKMN_32BIT TRUE)
ENDIF(${CMAKE_SIZEOF_VOID_P} EQUAL 8)

IF(PKMN_GCC OR PKMN_CLANG)
    SET(PKMN_C_FLAGS   "-O3 -std=gnu99 -Wall -Wextra -Werror -fvisibility=hidden")
    SET(PKMN_CXX_FLAGS "-O3 -std=c++11 -Wall -Wextra -Werror -fvisibility=hidden")
ELSEIF(MSVC)
    SET(PKMN_C_FLAGS   "/MP /EHsc /D _CRT_SECURE_NO_WARNINGS /WX")
    SET(PKMN_CXX_FLAGS ${PKMN_C_FLAGS})
ENDIF(PKMN_GCC OR PKMN_CLANG)

#
# Use Boost.Predef to check endianness
#
SET(CMAKE_REQUIRED_INCLUDES
    ${Boost_INCLUDE_DIRS}
)

INCLUDE(CheckCXXSourceCompiles)
CHECK_CXX_SOURCE_COMPILES("
    #include <boost/predef/other/endian.h>

    #ifndef BOOST_ENDIAN_LITTLE_WORD
    #error Not little endian
    #endif

    int main() {
        return 0;
    }
" PLATFORM_LITTLE_ENDIAN)

#
# Use Boost.Config to check for constexpr
#
CHECK_CXX_SOURCE_COMPILES("
    #include <boost/config/suffix.hpp>

    #ifdef BOOST_NO_CXX11_CONSTEXPR
    #error No constexpr
    #endif

    int main() {
        return 0;
    }
" COMPILER_HAS_CONSTEXPR)

IF(COMPILER_HAS_CONSTEXPR)
    SET(PKMN_CONSTEXPR_OR_INLINE "constexpr")
ELSE()
    SET(PKMN_CONSTEXPR_OR_INLINE "PKMN_INLINE")
ENDIF(COMPILER_HAS_CONSTEXPR)

#
# Look for required C++ headers
#
INCLUDE(CheckIncludeFileCXX)
SET(CMAKE_REQUIRED_FLAGS "${PKMN_CXX_FLAGS}")
CHECK_INCLUDE_FILE_CXX(memory HAVE_MEMORY)
IF(HAVE_MEMORY)
    SET(SPTR_HEADER    "<memory>")
    SET(SPTR_NAMESPACE "std")
ELSE()
    CHECK_INCLUDE_FILE_CXX(tr1/memory HAVE_TR1_MEMORY)
    IF(HAVE_TR1_MEMORY)
        SET(SPTR_HEADER    "<tr1/memory>")
        SET(SPTR_NAMESPACE "std::tr1")
    ELSE()
        MESSAGE(FATAL_ERROR "Could not find an STL shared_ptr!")
    ENDIF(HAVE_TR1_MEMORY)
ENDIF(HAVE_MEMORY)

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
