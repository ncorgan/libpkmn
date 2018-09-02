#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py --language=ruby --include-dir=${PKMN_SOURCE_DIR}/lib/swig/cpp_wrappers --output=${CMAKE_CURRENT_BINARY_DIR}/ruby_CamelCase.i
    COMMENT "Generating ruby_CamelCase.i"
)
ADD_CUSTOM_TARGET(
    ruby_CamelCase_i ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ruby_CamelCase.i
)

# Get list of enum modules to add to dependencies, as well as the import
# string.
EXECUTE_PROCESS(
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/print_enum_names_for_cmake.py --enum-dir=${PKMN_SOURCE_DIR}/include/pkmn/enums --print-as-cmake-list-ruby
    OUTPUT_VARIABLE ENUM_LIST OUTPUT_STRIP_TRAILING_WHITESPACE
)
SET(ENUM_LIST ${ENUM_LIST} CACHE STRING "Enum list")
EXECUTE_PROCESS(
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/print_enum_names_for_cmake.py --enum-dir=${PKMN_SOURCE_DIR}/include/pkmn/enums --print-as-ruby-imports
    OUTPUT_VARIABLE ENUM_IMPORT OUTPUT_STRIP_TRAILING_WHITESPACE
)
SET(ENUM_IMPORT ${ENUM_IMPORT} CACHE STRING "Enum import")

INCLUDE(SWIGRuby)

# Copy top-level .rb file for unit tests
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/PKMN.rb
    ${CMAKE_CURRENT_BINARY_DIR}/PKMN.rb
@ONLY)

INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/PKMN.rb
    DESTINATION ${RUBY_INSTALL_DIR}
    COMPONENT Ruby
)

ADD_SUBDIRECTORY(PKMN)
