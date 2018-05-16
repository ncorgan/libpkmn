#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(LIBPKMN_TEST_FILES "${PKMN_SOURCE_DIR}/testing/libpkmn-test-files")
FILE(TO_NATIVE_PATH ${LIBPKMN_TEST_FILES} LIBPKMN_TEST_FILES)
STRING(REPLACE "\\" "\\\\" LIBPKMN_TEST_FILES ${LIBPKMN_TEST_FILES})

ADD_SUBDIRECTORY(cpp)
IF(PKMN_ENABLE_C)
    ADD_SUBDIRECTORY(c)
ENDIF()
IF(PKMN_ENABLE_CSHARP AND NUNIT_FOUND)
    ADD_SUBDIRECTORY(csharp)
ENDIF()
