#
# Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/qt
    ${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_SOURCE_DIR}/PkmGCTools/LibPkmGC/include
    ${PKMN_BINARY_DIR}/pksav/include
    ${PKMN_SOURCE_DIR}/pksav/include
    ${PKMN_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/database
)

SET(pkmn_libs
    ${Boost_LIBRARIES}
    pksav
    PkmGC
    SQLiteCpp
)

#
# Add sources included in all builds
#

ADD_SUBDIRECTORY(SQLiteCpp)
ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(conversions)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(io)
ADD_SUBDIRECTORY(pksav)
ADD_SUBDIRECTORY(utils)

#
# Configuration for Qt support
#
IF(PKMN_ENABLE_QT)
    CMAKE_POLICY(SET CMP0020 NEW)
    IF(POLICY CMP0043)
        CMAKE_POLICY(SET CMP0043 NEW)
    ENDIF()

    INCLUDE_DIRECTORIES(
        ${QTx_INCLUDE_DIRS}
    )
    LIST(APPEND pkmn_libs
        ${QTx_LIBRARIES}
    )
ENDIF()
ADD_SUBDIRECTORY(qt)

SET(pkmn_sources
    ${pkmn_calculations_sources}
    ${pkmn_conversions_sources}
    ${pkmn_database_sources}
    ${pkmn_io_sources}
    ${pkmn_pksav_sources}
    ${pkmn_qt_sources}
    ${pkmn_utils_sources}
    build_info.cpp
    exception.cpp
    game_save_impl.cpp
    game_save_gen1impl.cpp
    game_save_gen2impl.cpp
    game_save_gbaimpl.cpp
    game_save_gcnimpl.cpp
    item_bag_impl.cpp
    item_bag_gen1impl.cpp
    item_bag_gen2impl.cpp
    item_bag_gbaimpl.cpp
    item_bag_gcnimpl.cpp
    item_list_impl.cpp
    item_list_gen2_tmhmimpl.cpp
    item_list_gcnimpl.cpp
    item_list_modernimpl.cpp
    pokemon_impl.cpp
    pokemon_gen1impl.cpp
    pokemon_gen2impl.cpp
    pokemon_gbaimpl.cpp
    pokemon_gcnimpl.cpp
    pokemon_box_impl.cpp
    pokemon_box_gbaimpl.cpp
    pokemon_box_gcnimpl.cpp
    pokemon_party_impl.cpp
    pokemon_party_gbaimpl.cpp
    pokemon_party_gcnimpl.cpp
    pokemon_pc_impl.cpp
    pokemon_pc_gen1impl.cpp
    pokemon_pc_gen2impl.cpp
    pokemon_pc_gbaimpl.cpp
    pokemon_pc_gcnimpl.cpp
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
ENDIF()

#
# Add DLL resource file to Windows builds
#
IF(WIN32)
    IF(MINGW)
        SET(LIB_PREFIX "lib")
    ENDIF()
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/pkmn.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/pkmn.rc
    @ONLY)
    SET(pkmn_rc ${CMAKE_CURRENT_BINARY_DIR}/pkmn.rc)
ENDIF()

SET_SOURCE_FILES_PROPERTIES(${pkmn_sources}
    PROPERTIES COMPILE_FLAGS "${PKMN_CXX_FLAGS}"
)

ADD_LIBRARY(pkmn SHARED ${pkmn_sources} ${pkmn_rc})
TARGET_LINK_LIBRARIES(pkmn ${pkmn_libs})
SET_TARGET_PROPERTIES(pkmn PROPERTIES DEFINE_SYMBOL "PKMN_DLL_EXPORTS")
ADD_DEFINITIONS(-Dpkmn_EXPORTS)

#
# Static Analysis
#
IF(PKMN_USE_CLANGTIDY)
    SET_TARGET_PROPERTIES(
        pkmn PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_PROPERTY}"
    )
ENDIF()
IF(PKMN_USE_IWYC)
    SET_TARGET_PROPERTIES(
        pkmn PROPERTIES
        CXX_INCLUDE_WHAT_YOU_USE "${IWYC_PROPERTY}"
    )
ENDIF()

#
# Qt post-build stuff
#
IF(PKMN_ENABLE_QT)
    SET_TARGET_PROPERTIES(pkmn PROPERTIES AUTOMOC ON)
ENDIF()

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
ENDIF()
