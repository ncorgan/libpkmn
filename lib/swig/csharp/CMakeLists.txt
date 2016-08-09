#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Generate CamelCase conversions
FILE(GLOB_RECURSE header_files "${PKMN_SOURCE_DIR}/include/*.hpp")
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py
    DEPENDS ${header_files}
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py --include-dir=${PKMN_SOURCE_DIR}/include --output=${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
    COMMENT "Generating csharp_CamelCase.i"
)
ADD_CUSTOM_TARGET(
    csharp_CamelCase_i ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
)

# Generate C# docstrings
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkmn_csharp_docs.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py --language=csharp --include-dir=${PKMN_SOURCE_DIR}/include --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating C# XML docs"
)
ADD_CUSTOM_TARGET(
    pkmn_csharp_docs ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pkmn_csharp_docs.i
)

# Build SWIG modules
INCLUDE(SWIGCSharp)
SWIG_CSHARP_INIT()
