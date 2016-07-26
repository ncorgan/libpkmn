#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${PKMN_SOURCE_DIR}/libpkmn-database/sqlite3
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKMN_SOURCE_DIR}/lib
)

SET(SQLiteCpp_sources
    Column.cpp
    Database.cpp
    Statement.cpp
    Transaction.cpp
)

ADD_LIBRARY(SQLiteCpp STATIC ${SQLiteCpp_sources})
TARGET_LINK_LIBRARIES(SQLiteCpp sqlite3)
IF(UNIX)
    SET_TARGET_PROPERTIES(SQLiteCpp PROPERTIES
        COMPILE_FLAGS "-DSQLITE_THREADSAFE=0 -DSQLITE_ENABLE_COLUMN_METADATA -fPIC"
    )
ELSE()
    SET_TARGET_PROPERTIES(SQLiteCpp PROPERTIES
        COMPILE_FLAGS "-DSQLITE_THREADSAFE=0 -DSQLITE_ENABLE_COLUMN_METADATA"
    )
ENDIF(UNIX)
