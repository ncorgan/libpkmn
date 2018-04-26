#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKMN_SOURCE_DIR}/lib
    ${PKMN_SOURCE_DIR}/pksav/lib
    ${Boost_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

SET(input_fuzzing_test_c_srcs
    ${PKMN_SOURCE_DIR}/pksav/lib/gen1/save.c
    ${PKMN_SOURCE_DIR}/pksav/lib/gen2/save.c
    ${PKMN_SOURCE_DIR}/pksav/lib/gba/checksum.c
    ${PKMN_SOURCE_DIR}/pksav/lib/util/fs.c
)
SET(input_fuzzing_test_cpp_srcs
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_input_fuzzing_test.cpp
)

SET_SOURCE_FILES_PROPERTIES(${input_fuzzing_test_c_srcs}
    PROPERTIES COMPILE_FLAGS "${PKMN_C_FLAGS}"
)
SET_SOURCE_FILES_PROPERTIES(${input_fuzzing_test_cpp_srcs}
    PROPERTIES COMPILE_FLAGS "${PKMN_CXX_FLAGS}"
)

ADD_EXECUTABLE(
    cpp_input_fuzzing_test
    "${input_fuzzing_test_c_srcs};${input_fuzzing_test_cpp_srcs}"
)
TARGET_LINK_LIBRARIES(cpp_input_fuzzing_test
    ${Boost_LIBRARIES}
    gtest
    gtest_main
    pkmn
    pksav
    PkmGC
)

IF(MINGW)
    SET(cpp_test_cmd "${CMAKE_CURRENT_BINARY_DIR}/cpp_input_fuzzing_test.exe")
    STRING(REPLACE "/" "\\\\" cpp_test_cmd ${cpp_test_cmd})
ELSEIF(WIN32)
    SET(cpp_test_cmd "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/cpp_input_fuzzing_test.exe")
    STRING(REPLACE "/" "\\\\" cpp_test_cmd ${cpp_test_cmd})
ELSE()
    SET(cpp_test_cmd "${CMAKE_CURRENT_BINARY_DIR}/cpp_input_fuzzing_test")
ENDIF()
PKMN_ADD_TEST(cpp_input_fuzzing_test ${cpp_test_cmd})
