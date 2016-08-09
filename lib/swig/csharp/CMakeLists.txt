#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Generate Python docstrings
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkmn_csharp_docstrings.i
    COMMAND ${CSHARP_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py --language=csharp --include-dir=${PKMN_SOURCE_DIR}/include --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    WORKING_DIRECTORY ${PKMN_SOURCE_DIR}/scripts
    COMMENT "Generating C# XML docs"
)
ADD_CUSTOM_TARGET(
    pkmn_csharp_docstrings ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pkmn_csharp_docstrings.i
)
