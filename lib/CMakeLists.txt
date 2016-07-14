#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/database
)

SET(pkmn_cpp_libs
    ${Boost_LIBRARIES}
)

#
# Add sources included in all builds
#

ADD_SUBDIRECTORY(utils)

SET(pkmn_sources
    ${pkmn_utils_sources}
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
        DESTINATION ${LIBRARY_DIR}/pkgconfig
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
#TARGET_LINK_LIBRARIES(pkmn ${pkmn_libs})
SET_TARGET_PROPERTIES(pkmn PROPERTIES DEFINE_SYMBOL "PKMN_DLL_EXPORTS")
ADD_DEFINITIONS(-Dpkmn_EXPORTS)

INSTALL(
    TARGETS pkmn
    LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Libraries # .so
    ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT Libraries # .lib
    RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Libraries # .dll
)
