#
# Copyright (c) 2015-2016,2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(PKMNPlatform)

SET(sqlite3_sources
    ${PKMN_SOURCE_DIR}/libpkmn-database/sqlite3/sqlite3.c
PARENT_SCOPE)
SET(sqlite3_libs
    ${CMAKE_THREAD_LIBS_INIT}
)
IF(UNIX)
    LIST(APPEND sqlite3_libs dl)
ENDIF()

# Appending after setting to parent scope doesn't append to parent scope
SET(sqlite3_libs ${sqlite3_libs} PARENT_SCOPE)

SET(SQLiteCpp_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/Column.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Database.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Statement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Transaction.cpp
PARENT_SCOPE)

# Add debug flag when set
IF(PKMN_SQLITE_DEBUG)
    SET(PKMN_SQLITE_DEBUG_FLAG "-DPKMN_SQLITE_DEBUG=1")
ENDIF()

IF(PKMN_GCC OR PKMN_CLANG)
    SET(sqlite3_COMPILE_FLAGS
        "-DSQLITE_THREADSAFE=0 -DSQLITE_ENABLE_COLUMN_METADATA -fvisibility=hidden"
    PARENT_SCOPE)
    SET(SQLiteCpp_COMPILE_FLAGS
        "-DSQLITE_THREADSAFE=0 -DSQLITE_ENABLE_COLUMN_METADATA -std=c++11 -fvisibility=hidden ${PKMN_SQLITE_DEBUG_FLAG}"
    PARENT_SCOPE)
ELSE()
    SET(sqlite3_COMPILE_FLAGS
        "-DSQLITE_THREADSAFE=0 -DSQLITE_ENABLE_COLUMN_METADATA"
    PARENT_SCOPE)
    SET(SQLiteCpp_COMPILE_FLAGS
        ${sqlite3_COMPILE_FLAGS}
    PARENT_SCOPE)
ENDIF()
