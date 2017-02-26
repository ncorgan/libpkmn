#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# CMake configuration
########################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)
PROJECT(pkmn-c-app C)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules;${CMAKE_MODULE_PATH}")

########################################################################
# Find LibPKMN
########################################################################
FIND_PACKAGE(PKMN-C REQUIRED)

########################################################################
# Configure build
########################################################################
INCLUDE_DIRECTORIES(${PKMN-C_INCLUDE_DIR})

ADD_EXECUTABLE(pkmn-c-app pkmn-c-app.c)
TARGET_LINK_LIBRARIES(pkmn-c-app ${PKMN-C_LIBRARY})
