#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${SQLITE3_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKMN_SOURCE_DIR}/lib
)

SET(SQLiteCpp_sources
    Column.cpp
    Database.cpp
    Statement.cpp
    Transaction.cpp
)

# Add debug flag when set
IF(PKMN_SQLITE_DEBUG)
    SET(PKMN_SQLITE_DEBUG_FLAG "-DPKMN_SQLITE_DEBUG=1")
ENDIF(PKMN_SQLITE_DEBUG)

ADD_LIBRARY(SQLiteCpp STATIC ${SQLiteCpp_sources})
TARGET_LINK_LIBRARIES(SQLiteCpp ${SQLITE3_LIBRARY_RELEASE})
IF(UNIX)
    SET_TARGET_PROPERTIES(SQLiteCpp PROPERTIES
        COMPILE_FLAGS "-DSQLITE_THREADSAFE=0 -DSQLITE_ENABLE_COLUMN_METADATA -fPIC ${PKMN_SQLITE_DEBUG_FLAG}"
    )
ELSE()
    SET_TARGET_PROPERTIES(SQLiteCpp PROPERTIES
        COMPILE_FLAGS "-DSQLITE_THREADSAFE=0 -DSQLITE_ENABLE_COLUMN_METADATA ${PKMN_SQLITE_DEBUG_FLAG}"
    )
ENDIF(UNIX)
