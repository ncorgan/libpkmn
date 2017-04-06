#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying FILE LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(PKMNPlatform)

SET(RUBY_INSTALL_DIR ${PKMN_LIBRARY_DIR}/ruby/${RUBY_VERSION})

########################################################################
# Macro to build and install Ruby modules
########################################################################
MACRO(SWIG_BUILD_RUBY_MODULE module_name cplusplus)
    INCLUDE(UseSWIG)

    SET(SWIG_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${SWIG_MODULE_DIR}
        ${SWIG_RUBY_INCLUDE_DIRS}
        ${RUBY_INCLUDE_DIRS}
    )
    INCLUDE_DIRECTORIES(${SWIG_INCLUDE_DIRS})

    SET(SWIG_LIBRARIES
        ${SWIG_RUBY_LIBRARIES}
        ${RUBY_LIBRARY}
    )

    # Set flags to pass into SWIG call
    SET(CMAKE_SWIG_FLAGS ${SWIG_RUBY_FLAGS})
    FOREACH(dir ${SWIG_INCLUDE_DIRS})
        LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
    ENDFOREACH(dir ${SWIG_INCLUDE_DIRS})

    # Allows CMake variables to be placed in SWIG .i files
    CONFIGURE_FILE(
        ${SWIG_MODULE_DIR}/${module_name}.i
        ${CMAKE_CURRENT_BINARY_DIR}/${module_name}.i
    @ONLY)

    # Set SWIG's C++ flag if specified by the user
    IF(${cplusplus})
        SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/${module_name}.i PROPERTIES CPLUSPLUS ON)
    ENDIF(${cplusplus})

    # The actual CMake call for SWIG
    SWIG_ADD_MODULE(${module_name} ruby ${CMAKE_CURRENT_BINARY_DIR}/${module_name}.i)
    SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME} PROPERTIES PREFIX "")
    SWIG_LINK_LIBRARIES(${module_name} ${SWIG_LIBRARIES})

    INSTALL(
        TARGETS ${SWIG_MODULE_${module_name}_REAL_NAME}
        DESTINATION ${RUBY_INSTALL_DIR}/PKMN
        COMPONENT Ruby
    )
ENDMACRO(SWIG_BUILD_RUBY_MODULE module_name cplusplus)
