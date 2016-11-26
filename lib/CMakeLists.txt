#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp
    ${PKMN_SOURCE_DIR}/libpkmn-database/sqlite3
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/pksav/include
    ${PKMN_SOURCE_DIR}/pksav/include
    ${PKMN_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/database
)

SET(pkmn_libs
    ${Boost_LIBRARIES}
    pksav
    SQLiteCpp
)

#
# Add sources included in all builds
#

ADD_SUBDIRECTORY(SQLiteCpp)
ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(pksav)
ADD_SUBDIRECTORY(utils)

#
# Configuration for Qt5 support
#
IF(PKMN_ENABLE_QT5)
    CMAKE_POLICY(SET CMP0043 NEW)

    INCLUDE_DIRECTORIES(
        ${Qt5Widgets_INCLUDE_DIRS}
    )
    LIST(APPEND pkmn_libs
        ${Qt5Widgets_LIBRARIES}
    )

    ADD_SUBDIRECTORY(qt5)
ENDIF(PKMN_ENABLE_QT5)

SET(pkmn_sources
    ${pkmn_calculations_sources}
    ${pkmn_database_sources}
    ${pkmn_pksav_sources}
    ${pkmn_qt5_sources} # Will be empty without Qt5 support
    ${pkmn_utils_sources}
    build_info.cpp
    item_bag_impl.cpp
    item_bag_gen1impl.cpp
    item_bag_gen2impl.cpp
    item_list_impl.cpp
    item_list_gen2_tmhmimpl.cpp
)

#
# pkg-config file
#
FIND_PACKAGE(PkgConfig)
IF(PKG_CONFIG_FOUND)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/pkmn.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/pkmn.pc
    @ONLY)
    INSTALL(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/pkmn.pc
        DESTINATION ${PKMN_LIBRARY_DIR}/pkgconfig
    )
ENDIF(PKG_CONFIG_FOUND)

#
# Add DLL resource file to Windows builds
#
IF(WIN32)
    IF(MINGW)
        SET(LIB_PREFIX "lib")
    ENDIF(MINGW)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/pkmn.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/pkmn.rc
    @ONLY)
    SET(pkmn_rc ${CMAKE_CURRENT_BINARY_DIR}/pkmn.rc)
ENDIF(WIN32)

SET_SOURCE_FILES_PROPERTIES(${pkmn_sources}
    PROPERTIES COMPILE_FLAGS "${PKMN_CXX_FLAGS}"
)

ADD_LIBRARY(pkmn SHARED ${pkmn_sources} ${pkmn_rc})
TARGET_LINK_LIBRARIES(pkmn ${pkmn_libs})
SET_TARGET_PROPERTIES(pkmn PROPERTIES DEFINE_SYMBOL "PKMN_DLL_EXPORTS")
ADD_DEFINITIONS(-Dpkmn_EXPORTS)

#
# Qt5 post-build stuff
#
IF(PKMN_ENABLE_QT5)
    SET_TARGET_PROPERTIES(pkmn PROPERTIES AUTOMOC ON)
    qt5_use_modules(pkmn Widgets)
ENDIF(PKMN_ENABLE_QT5)

INSTALL(
    TARGETS pkmn
    LIBRARY DESTINATION ${PKMN_LIBRARY_DIR} COMPONENT Libraries # .so
    ARCHIVE DESTINATION ${PKMN_LIBRARY_DIR} COMPONENT Libraries # .lib
    RUNTIME DESTINATION ${PKMN_RUNTIME_DIR} COMPONENT Libraries # .dll
)

# API wrappers
ADD_SUBDIRECTORY(swig)
IF(PKMN_ENABLE_C)
    ADD_SUBDIRECTORY(c)
ENDIF(PKMN_ENABLE_C)
