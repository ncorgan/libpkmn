#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Generate CamelCase conversions
FILE(GLOB_RECURSE header_files "${PKMN_SOURCE_DIR}/include/*.hpp")
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py
    DEPENDS ${header_files}
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ruby_CamelCase.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py --language=ruby --include-dir=${PKMN_SOURCE_DIR}/include --output=${CMAKE_CURRENT_BINARY_DIR}/ruby_CamelCase.i
    COMMENT "Generating ruby_CamelCase.i"
)
ADD_CUSTOM_TARGET(
    ruby_CamelCase_i ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ruby_CamelCase.i
)

INCLUDE(SWIGRuby)

#
# Set variables needed for CMake macros
#

SET(SWIG_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(SWIG_RUBY_INCLUDE_DIRS
    ${SWIG_MODULE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${SWIG_MODULE_DIR}/..
    ${SWIG_MODULE_DIR}/../modules
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
)

SET(SWIG_RUBY_LIBRARIES
    pkmn
)

SWIG_BUILD_RUBY_MODULE(Calculations TRUE)
SWIG_BUILD_RUBY_MODULE(Database     TRUE)
SWIG_BUILD_RUBY_MODULE(STL          TRUE)

# Copy top-level .rb file for unit tests
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/PKMN.rb
    ${CMAKE_CURRENT_BINARY_DIR}/PKMN.rb
COPYONLY)

INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/PKMN.rb
    DESTINATION ${RUBY_INSTALL_DIR}
    COMPONENT Ruby
)
