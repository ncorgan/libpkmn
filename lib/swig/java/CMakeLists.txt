#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(SWIGJava)

# Generate CamelCase conversions
FILE(GLOB_RECURSE header_files "${PKMN_SOURCE_DIR}/include/*.hpp")
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py
    DEPENDS ${header_files}
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/java_CamelCase.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py --include-dir=${PKMN_SOURCE_DIR}/include --output=${CMAKE_CURRENT_BINARY_DIR}/java_CamelCase.i --java
    COMMENT "Generating java_CamelCase.i"
)
ADD_CUSTOM_TARGET(
    java_CamelCase_i ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/java_CamelCase.i
)

# Generate Java docstrings
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkmn_javadocs.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py --language=java --include-dir=${PKMN_SOURCE_DIR}/include --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Javadocs"
)
ADD_CUSTOM_TARGET(
    pkmn_javadocs ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pkmn_javadocs.i
)

#
# Set variables needed for CMake macros
#

SET(SWIG_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(SWIG_JAVA_INCLUDE_DIRS
    ${SWIG_MODULE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${SWIG_MODULE_DIR}/..
    ${SWIG_MODULE_DIR}/../modules
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
)

SET(SWIG_JAVA_LIBRARIES
    pkmn
)

SWIG_BUILD_JAVA_MODULE(env_java "Env" TRUE)
SWIG_BUILD_JAVA_MODULE(paths_java "Paths" TRUE)
SWIG_BUILD_JAVA_MODULE(stl_java "STL" TRUE)
JAVA_BUILD_JAR("PKMN.jar" "env_java;paths_java;stl_java" "nc" "")
