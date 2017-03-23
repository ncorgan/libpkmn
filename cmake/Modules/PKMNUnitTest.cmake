#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Source/binary dirs
SET(TESTS_SOURCE_DIR ${PKMN_SOURCE_DIR}/testing/unit-tests)
SET(TESTS_BINARY_DIR ${PKMN_BINARY_DIR}/testing/unit-tests)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
    ${PKMN_SOURCE_DIR}/pksav/include
    ${PKMN_BINARY_DIR}/pksav/include
    ${PKMN_SOURCE_DIR}/lib
)

SET(pkmn_cpp_test_libs
    ${Boost_LIBRARIES}
    gtest_main
    pkmn
    pkmntest
)

SET(pkmn_c_test_libs
    pkmn-c
    #pkmntest-c
    unity
)

IF(PKMN_GCC)
    SET(PKMNTEST_CXX_FLAGS "-Wno-sign-compare ${PKMN_CXX_FLAGS}")
ELSEIF(PKMN_CLANG)
    SET(PKMNTEST_CXX_FLAGS "${PKMN_CXX_FLAGS} -Wno-error -Wno-sign-compare -Werror")
ELSE()
    SET(PKMNTEST_CXX_FLAGS "${PKMN_CXX_FLAGS} /wd4018")
ENDIF(PKMN_GCC)

MACRO(PKMN_ADD_TEST test_name test_cmd)
    IF(CMAKE_CROSSCOMPILING)
        ADD_TEST(${test_name} ${test_cmd})
    ELSE()
        SET(TEST_CMD ${test_cmd})
        SET(DATABASE_PATH ${PKMN_BINARY_DIR}/libpkmn-database/database/libpkmn.db)
        SET(IMAGES_DIR ${PKMN_SOURCE_DIR}/images)
        SET(LIBPKMN_TEST_FILES ${PKMN_SOURCE_DIR}/testing/libpkmn-test-files)
        SET(PKSAV_TEST_SAVES ${PKMN_SOURCE_DIR}/pksav/testing/pksav-test-saves)
        SET(PYTHONPATH
            "${PKMN_BINARY_DIR}/lib/swig/python"
            "${TESTS_BINARY_DIR}/pkmntest/python"
        )
        SET(CLASSPATH
            "${CMAKE_CURRENT_BINARY_DIR}"
            "${PKMN_BINARY_DIR}/lib/swig/java/PKMN.jar"
        )
        SET(LUA_PATH
            "${PKMN_BINARY_DIR}/lib/swig/lua/?.lua"
            "${PKMN_SOURCE_DIR}/testing/unit-tests/pkmntest/lua/?.lua"
            "${PKMN_BINARY_DIR}/testing/unit-tests/pkmntest/lua/?.lua"
        )
        IF(WIN32)
            SET(LIBRARY_PATHS
                "${Boost_LIBRARY_DIRS}"
                "${QTx_RUNTIME_DIR}"
                "${PKMN_BINARY_DIR}/pksav/lib/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/c/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/swig/csharp/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/swig/java/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/calculations"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/database"
                "${TESTS_BINARY_DIR}/pkmntest/cpp/${CMAKE_BUILD_TYPE}"
                "${TESTS_BINARY_DIR}/pkmntest/c/${CMAKE_BUILD_TYPE}"
            )
            SET(TEST_CMD ${test_cmd})
            SET(LIBRARY_DIR ${PKMN_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
            SET(LUA_CPATH
                "${PKMN_BINARY_DIR}/lib/swig/lua/${CMAKE_BUILD_TYPE}/?.dll"
                "${TESTS_BINARY_DIR}/lua/bit32/${CMAKE_BUILD_TYPE}/?.dll"
            )

            STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
            STRING(REPLACE "/" "\\" LIBRARY_PATHS "${LIBRARY_PATHS}")
            STRING(REPLACE "/" "\\" PYTHONPATH "${PYTHONPATH}")
            STRING(REPLACE "/" "\\" CLASSPATH "${CLASSPATH}")
            STRING(REPLACE "/" "\\" LUA_PATH "${LUA_PATH}")
            STRING(REPLACE "/" "\\" LUA_CPATH "${LUA_CPATH}")
            STRING(REPLACE "/" "\\" DATABASE_PATH "${DATABASE_PATH}")
            STRING(REPLACE "/" "\\" IMAGES_DIR "${IMAGES_DIR}")
            STRING(REPLACE "/" "\\" LIBPKMN_TEST_FILES "${LIBPKMN_TEST_FILES}")
            STRING(REPLACE "/" "\\" PKSAV_TEST_SAVES "${PKSAV_TEST_SAVES}")
            CONFIGURE_FILE(
                ${TESTS_SOURCE_DIR}/unit_test_template.bat.in
                ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
            @ONLY)
            ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
        ELSE()
            IF(APPLE)
                SET(DY "DY")
            ENDIF(APPLE)
            SET(LIBRARY_PATHS
                "${Boost_LIBRARY_DIRS}"
                "${PKMN_BINARY_DIR}/pksav/lib"
                "${PKMN_BINARY_DIR}/lib"
                "${PKMN_BINARY_DIR}/lib/c"
                "${PKMN_BINARY_DIR}/lib/swig/csharp"
                "${PKMN_BINARY_DIR}/lib/swig/java"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/calculations"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/database"
                "${TESTS_BINARY_DIR}/pkmntest/cpp"
                "${TESTS_BINARY_DIR}/pkmntest/c"
            )
            SET(LUA_CPATH
                "${PKMN_BINARY_DIR}/lib/swig/lua/?.so"
                "${TESTS_BINARY_DIR}/lua/bit32/?.so"
            )

            STRING(REPLACE ";" ":" LIBRARY_PATHS "${LIBRARY_PATHS}")
            STRING(REPLACE ";" ":" CLASSPATH "${CLASSPATH}")
            STRING(REPLACE ";" ":" PYTHONPATH "${PYTHONPATH}")
            CONFIGURE_FILE(
                ${TESTS_SOURCE_DIR}/unit_test_template.sh.in
                ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
            @ONLY)
            ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
        ENDIF(WIN32)
    ENDIF(CMAKE_CROSSCOMPILING)
ENDMACRO(PKMN_ADD_TEST)

MACRO(PKMN_ADD_CPP_TEST test_name test_srcs)
    ADD_EXECUTABLE(${test_name} ${test_srcs})
    SET_SOURCE_FILES_PROPERTIES(${test_srcs}
        PROPERTIES COMPILE_FLAGS "${PKMNTEST_CXX_FLAGS}"
    )
    TARGET_LINK_LIBRARIES(${test_name} ${pkmn_cpp_test_libs})

    IF(WIN32)
        SET(cpp_test_cmd "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${test_name}.exe")
        STRING(REPLACE "/" "\\\\" cpp_test_cmd ${cpp_test_cmd})
    ELSE()
        SET(cpp_test_cmd "${CMAKE_CURRENT_BINARY_DIR}/${test_name}")
    ENDIF(WIN32)
    PKMN_ADD_TEST(${test_name} ${cpp_test_cmd})
ENDMACRO(PKMN_ADD_CPP_TEST)

MACRO(PKMN_ADD_C_TEST test_name test_srcs)
    ADD_EXECUTABLE(${test_name} ${test_srcs})
    SET_SOURCE_FILES_PROPERTIES(${test_srcs}
        PROPERTIES COMPILE_FLAGS "${PKMN_C_FLAGS}"
    )
    TARGET_LINK_LIBRARIES(${test_name} ${pkmn_c_test_libs})

    IF(WIN32)
        SET(c_test_cmd "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${test_name}.exe")
        STRING(REPLACE "/" "\\\\" c_test_cmd ${c_test_cmd})
    ELSE()
        SET(c_test_cmd "${CMAKE_CURRENT_BINARY_DIR}/${test_name}")
    ENDIF(WIN32)
    PKMN_ADD_TEST(${test_name} ${c_test_cmd})
ENDMACRO(PKMN_ADD_C_TEST)

INCLUDE(SWIGCSharp)

MACRO(PKMN_ADD_CSHARP_TEST test_name test_srcs test_dlls)
    CSHARP_ADD_LIBRARY(${test_name} ${test_srcs} ${test_dlls} ${NUNIT_LIBRARIES})
    ADD_DEPENDENCIES(${test_name} "pkmn-cs" "pkmntest-cs")

    # TODO: Determine programmatically, probably parsing variables
    IF(NOT APPVEYOR)
        IF(WIN32)
            STRING(REPLACE "/" "\\\\" native_nunit_command "${NUNIT_COMMAND}")
            STRING(REPLACE "/" "\\\\" native_dll_path "${CMAKE_CURRENT_BINARY_DIR}/${test_name}.dll")
            SET(csharp_test_cmd "\"${native_nunit_command}\" -framework=net-3.5 -labels \"${native_dll_path}\"")
        ELSE()
            SET(csharp_test_cmd "\"${NUNIT_COMMAND}\" -framework=4.0 -labels \"${CMAKE_CURRENT_BINARY_DIR}/${test_name}.dll\"")
        ENDIF(WIN32)
        PKMN_ADD_TEST(${test_name} ${csharp_test_cmd})
    ENDIF(NOT APPVEYOR)
ENDMACRO(PKMN_ADD_CSHARP_TEST)

MACRO(PKMN_ADD_LUA_TEST test_name)
    SET(CMD "\"${LUA_INTERPRETER}\" \"${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.lua\"")
    PKMN_ADD_TEST(${test_name} ${CMD})
ENDMACRO(PKMN_ADD_LUA_TEST test_name)

MACRO(PKMN_ADD_PYTHON_TEST test_name)
    SET(CMD "\"${PYTHON_EXECUTABLE}\" \"${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.py\"")
    PKMN_ADD_TEST(${test_name} ${CMD})
ENDMACRO(PKMN_ADD_PYTHON_TEST)

#
# Set locations of test saves.
#
SET(POKEMON_SAVE_DIR "${PKMN_SOURCE_DIR}/pksav/testing/pksav-test-saves")

SET(POKEMON_RED_SAV     "${POKEMON_SAVE_DIR}/red_blue/pokemon_red.sav")
SET(POKEMON_YELLOW_SAV  "${POKEMON_SAVE_DIR}/yellow/pokemon_yellow.sav")
SET(POKEMON_GOLD_SAV    "${POKEMON_SAVE_DIR}/gold_silver/pokemon_gold.sav")
SET(POKEMON_CRYSTAL_SAV "${POKEMON_SAVE_DIR}/crystal/pokemon_crystal.sav")
SET(POKEMON_RUBY_SAV    "${POKEMON_SAVE_DIR}/ruby_sapphire/pokemon_ruby.sav")
SET(POKEMON_EMERALD_SAV "${POKEMON_SAVE_DIR}/emerald/pokemon_emerald.sav")
SET(POKEMON_FIRERED_SAV "${POKEMON_SAVE_DIR}/firered_leafgreen/pokemon_firered.sav")

# Fix Windows paths
IF(WIN32)
    FILE(TO_CMAKE_PATH "${POKEMON_RED_SAV}" POKEMON_RED_SAV)
    FILE(TO_CMAKE_PATH "${POKEMON_YELLOW_SAV}" POKEMON_YELLOW_SAV)
    FILE(TO_CMAKE_PATH "${POKEMON_GOLD_SAV}" POKEMON_GOLD_SAV)
    FILE(TO_CMAKE_PATH "${POKEMON_CRYSTAL_SAV}" POKEMON_CRYSTAL_SAV)
    FILE(TO_CMAKE_PATH "${POKEMON_RUBY_SAV}" POKEMON_RUBY_SAV)
    FILE(TO_CMAKE_PATH "${POKEMON_EMERALD_SAV}" POKEMON_EMERALD_SAV)
    FILE(TO_CMAKE_PATH "${POKEMON_FIRERED_SAV}" POKEMON_FIRERED_SAV)
ENDIF(WIN32)
