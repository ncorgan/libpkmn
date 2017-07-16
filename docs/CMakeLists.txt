#
# Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MACRO(PKMN_BUILD_DOXYGEN_DOCS files lang langname)
    SET(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen/${lang}")
    MAKE_DIRECTORY(${DOXYGEN_OUTPUT_DIR})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_${lang}.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_${lang}
    @ONLY)
    ADD_CUSTOM_COMMAND(
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_${lang}
        OUTPUT ${DOXYGEN_OUTPUT_DIR}/html/index.html DEPENDS ${files}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_${lang}
        COMMENT "Generating ${langname} Doxygen documentation"
    )
    ADD_CUSTOM_TARGET(doxygen_${lang}_docs ALL DEPENDS ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    INSTALL(DIRECTORY ${DOXYGEN_OUTPUT_DIR} DESTINATION ${PKMN_DOCS_DIR} COMPONENT doxygen)
ENDMACRO(PKMN_BUILD_DOXYGEN_DOCS)

# C++ documentation
FILE(GLOB_RECURSE hpp_files
    ${PKMN_SOURCE_DIR}/include/pkmn/*.hpp
    ${PKMN_BINARY_DIR}/include/pkmn/*.hpp
)
PKMN_BUILD_DOXYGEN_DOCS("${hpp_files}" "cpp" "C++")

# C documentation
IF(PKMN_ENABLE_C)
    FILE(GLOB_RECURSE h_files
        ${PKMN_SOURCE_DIR}/include/pkmn-c/*.h
        ${PKMN_BINARY_DIR}/include/pkmn-c/*.h
    )
    PKMN_BUILD_DOXYGEN_DOCS("${h_files}" "c" "C")
ENDIF(PKMN_ENABLE_C)

IF(PKMN_ENABLE_CSHARP)
    PKMN_BUILD_DOXYGEN_DOCS("" "csharp" "C#")
    ADD_DEPENDENCIES(doxygen_csharp_docs pkmn-cs)
ENDIF(PKMN_ENABLE_CSHARP)

IF(PKMN_ENABLE_JAVA)
    PKMN_BUILD_DOXYGEN_DOCS("" "java" "Java")
    ADD_DEPENDENCIES(doxygen_java_docs PKMN_jar)
ENDIF(PKMN_ENABLE_JAVA)
