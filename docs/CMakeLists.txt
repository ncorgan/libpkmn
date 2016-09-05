#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
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

FILE(GLOB_RECURSE hpp_files ${CMAKE_SOURCE_DIR}/include/*.hpp)
PKMN_BUILD_DOXYGEN_DOCS("${hpp_files}" "cpp" "C++")

IF(PKMN_ENABLE_OBJC)
    PKMN_BUILD_DOXYGEN_DOCS("" "objc" "Objective-C")
    ADD_DEPENDENCIES(doxygen_objc_docs pkmn-objc)
ENDIF(PKMN_ENABLE_OBJC)

#IF(PKMN_ENABLE_CSHARP)
#    PKMN_BUILD_DOXYGEN_DOCS("" "csharp" "C#")
#    ADD_DEPENDENCIES(doxygen_csharp_docs pkmn-cs)
#ENDIF(PKMN_ENABLE_CSHARP)

#IF(PKMN_ENABLE_JAVA)
#    PKMN_BUILD_DOXYGEN_DOCS("" "java" "Java")
#    ADD_DEPENDENCIES(doxygen_java_docs pkmn_jar)
#ENDIF(PKMN_ENABLE_JAVA)
