#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
    ${ObjC_INCLUDE_DIRS}
)

SET(pkmn_objc_libs
    pkmn
    ${ObjC_LIBRARIES}
)

#
# Add sources included in all builds
#

ADD_SUBDIRECTORY(Database)
ADD_SUBDIRECTORY(Types)
ADD_SUBDIRECTORY(Utils)

SET(pkmn_objc_sources
    ${pkmn_objc_calculations_sources}
    ${pkmn_objc_database_sources}
    ${pkmn_objc_types_sources}
    ${pkmn_objc_utils_sources}
    CppToObjC.mm
)

SET_SOURCE_FILES_PROPERTIES(${pkmn_objc_sources}
    PROPERTIES COMPILE_FLAGS "${PKMN_OBJCXX_FLAGS} ${ObjCXX_FLAGS}"
)

ADD_LIBRARY(pkmn-objc SHARED ${pkmn_objc_sources})
TARGET_LINK_LIBRARIES(pkmn-objc ${pkmn_objc_libs})
SET_TARGET_PROPERTIES(pkmn-objc PROPERTIES DEFINE_SYMBOL "PKMN_DLL_EXPORTS")
ADD_DEFINITIONS(-Dpkmn_EXPORTS)

INSTALL(
    TARGETS pkmn-objc
    LIBRARY DESTINATION ${PKMN_LIBRARY_DIR} COMPONENT Libraries # .so
    ARCHIVE DESTINATION ${PKMN_LIBRARY_DIR} COMPONENT Libraries # .lib
    RUNTIME DESTINATION ${PKMN_RUNTIME_DIR} COMPONENT Libraries # .dll
)
