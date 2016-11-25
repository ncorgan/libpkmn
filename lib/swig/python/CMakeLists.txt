#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Generate Python docstrings
# This is not done in SWIG 3.0.6 because of a docstring bug.
# See https://github.com/swig/swig/issues/475
IF(NOT "${SWIG_VERSION}" STREQUAL "3.0.6")
    ADD_CUSTOM_COMMAND(
        DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkmn_python_docstrings.i
        COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py --language=python --include-dir=${PKMN_SOURCE_DIR}/include --output-dir=${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${PKMN_SOURCE_DIR}/scripts
        COMMENT "Generating Python docstrings"
    )
    ADD_CUSTOM_TARGET(
        pkmn_python_docstrings ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pkmn_python_docstrings.i
    )
ELSE()
    MESSAGE(STATUS "SWIG 3.0.6 detected. Skipping Python docstring generation.")
    MESSAGE(STATUS "See: https://github.com/swig/swig/issues/475")

    # Write empty file so include works
    FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/pkmn_python_docstrings.i "")
ENDIF(NOT "${SWIG_VERSION}" STREQUAL "3.0.6")

ADD_SUBDIRECTORY(pkmn)
