#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Source/binary dirs
FILE(TO_NATIVE_PATH ${PKMN_SOURCE_DIR}/tests TESTS_SOURCE_DIR)
FILE(TO_NATIVE_PATH ${PKMN_BINARY_DIR}/tests TESTS_BINARY_DIR)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
    ${PKMN_SOURCE_DIR}/lib
)

SET(pkmn_test_libs
    pkmn
)

MACRO(PKMN_ADD_TEST test_name test_cmd)
    IF(CMAKE_CROSSCOMPILING)
        ADD_TEST(${test_name} ${test_cmd})
    ELSE()
        SET(TEST_CMD ${test_cmd})
        SET(DATABASE_PATH ${CMAKE_BINARY_DIR}/pkmn-database/database/pkmn.db)
        SET(PYTHONPATH ${CMAKE_BINARY_DIR}/lib/swig/python)
        SET(CLASSPATH
            "${CMAKE_CURRENT_BINARY_DIR}"
            "${CMAKE_BINARY_DIR}/lib/swig/java/PKMN.jar"
        )
        IF(WIN32)
            SET(LIBRARY_PATHS
                "${PKMN_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/c/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/swig/csharp/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/swig/java/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/database"
            )
            SET(TEST_CMD ${test_cmd})
            SET(LIBRARY_DIR ${PKMN_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
            SET(DATABASE_PATH ${PKMN_BINARY_DIR}/pkmn-database/database/pkmn.db)
            STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
            STRING(REPLACE "/" "\\" LIBRARY_PATHS "${LIBRARY_PATHS}")
            STRING(REPLACE "/" "\\" PYTHONPATH "${PYTHONPATH}")
            STRING(REPLACE "/" "\\" CLASSPATH "${CLASSPATH}")
            STRING(REPLACE "/" "\\" DATABASE_PATH "${DATABASE_PATH}")
            CONFIGURE_FILE(
                ${TESTS_SOURCE_DIR}/unit_test_template.bat.in
                ${TESTS_BINARY_DIR}/${test_name}.bat
            @ONLY)
            ADD_TEST(${test_name} ${TESTS_BINARY_DIR}/${test_name}.bat)
        ELSE()
            IF(APPLE)
                SET(DY "DY")
            ENDIF(APPLE)
            SET(LIBRARY_PATHS
                "${PKMN_BINARY_DIR}/lib"
                "${PKMN_BINARY_DIR}/lib/c"
                "${PKMN_BINARY_DIR}/lib/swig/csharp"
                "${PKMN_BINARY_DIR}/lib/swig/java"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/database"
            )
            STRING(REPLACE ";" ":" LIBRARY_PATHS "${LIBRARY_PATHS}")
            STRING(REPLACE ";" ":" CLASSPATH "${CLASSPATH}")
            CONFIGURE_FILE(
                ${TESTS_SOURCE_DIR}/unit_test_template.sh.in
                ${TESTS_BINARY_DIR}/${test_name}.sh
            @ONLY)
            ADD_TEST(${test_name} ${TESTS_BINARY_DIR}/${test_name}.sh)
        ENDIF(WIN32)
    ENDIF(CMAKE_CROSSCOMPILING)
ENDMACRO(PKMN_ADD_TEST)

MACRO(PKMN_ADD_CPP_TEST test_name test_srcs)
    ADD_EXECUTABLE(${test_name} ${test_srcs})
    TARGET_LINK_LIBRARIES(${test_name} ${pkmn_test_libs})
    PKMN_ADD_TEST(${test_name} "${TESTS_BINARY_DIR}/${test_name}")
ENDMACRO(PKMN_ADD_CPP_TEST test_name test_src)
