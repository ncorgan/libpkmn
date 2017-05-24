#
# Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
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
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(io)
ADD_SUBDIRECTORY(mem)
ADD_SUBDIRECTORY(pksav)
ADD_SUBDIRECTORY(types)
ADD_SUBDIRECTORY(utils)

#
# Configuration for Qt support
#
IF(PKMN_ENABLE_QT)
    CMAKE_POLICY(SET CMP0020 NEW)
    CMAKE_POLICY(SET CMP0043 NEW)

    INCLUDE_DIRECTORIES(
        ${QTx_INCLUDE_DIRS}
    )
    LIST(APPEND pkmn_libs
        ${QTx_LIBRARIES}
    )

    ADD_SUBDIRECTORY(qt)
ENDIF(PKMN_ENABLE_QT)

SET(pkmn_sources
    ${pkmn_calculations_sources}
    ${pkmn_database_sources}
    ${pkmn_io_sources}
    ${pkmn_mem_sources}
    ${pkmn_pksav_sources}
    ${pkmn_qt_sources} # Will be empty without Qt support
    ${pkmn_types_sources}
    ${pkmn_utils_sources}
    build_info.cpp
    exception.cpp
    game_save_impl.cpp
    game_save_gen1impl.cpp
    game_save_gen2impl.cpp
    game_save_gbaimpl.cpp
    game_save_gcnimpl.cpp
    game_save_gen4impl.cpp
    item_bag_impl.cpp
    item_bag_gen1impl.cpp
    item_bag_gen2impl.cpp
    item_bag_gbaimpl.cpp
    item_bag_gcnimpl.cpp
    item_bag_gen4impl.cpp
    item_list_impl.cpp
    item_list_gen2_tmhmimpl.cpp
    item_list_modernimpl.cpp
    item_list_gcnimpl.cpp
    pokemon_impl.cpp
    pokemon_gen1impl.cpp
    pokemon_gen2impl.cpp
    pokemon_gbaimpl.cpp
    pokemon_gcnimpl.cpp
    pokemon_ndsimpl.cpp
    pokemon_box_impl.cpp
    pokemon_box_gbaimpl.cpp
    pokemon_box_gcnimpl.cpp
    pokemon_box_gen4impl.cpp
    pokemon_party_impl.cpp
    pokemon_party_gbaimpl.cpp
    pokemon_party_gcnimpl.cpp
    pokemon_party_gen4impl.cpp
    pokemon_pc_impl.cpp
    pokemon_pc_gen1impl.cpp
    pokemon_pc_gen2impl.cpp
    pokemon_pc_gbaimpl.cpp
    pokemon_pc_gcnimpl.cpp
    pokemon_pc_gen4impl.cpp
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
# Qt post-build stuff
#
IF(PKMN_ENABLE_QT)
    SET_TARGET_PROPERTIES(pkmn PROPERTIES AUTOMOC ON)
ENDIF(PKMN_ENABLE_QT)

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
