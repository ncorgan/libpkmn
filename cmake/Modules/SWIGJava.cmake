#
# Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# This file creates the following macros (example usage at end):
#
# SWIG_BUILD_JAVA_MODULE: Build a Java SWIG module and install it in
#                         the desired location.
#
# Parameters:
#  * swig_source:  The SWIG source .i file.
#  * module_name:  The Java class this module will be part of
#  * package_name: The name of this module's Java package.
#  * cplusplus:    TRUE or FALSE, whether or not this is a C++ module.
#
# Assumes the following variables are set:
#  * SWIG_MODULE_DIR:          The source directory that contains the .i
#                              file. This is needed so the .i file can
#                              include other local .i files.
#
#  * SWIG_JAVA_INCLUDE_DIRS: C/C++ include directories needed by the
#                            source files.
#
#  * SWIG_JAVA_LIBRARIES:    C/C++ libraries the Java module should
#                            link against.
#
#  * SWIG_JAVA_FLAGS:        Flags to pass into the SWIG command (optional).
#
# JAVA_BUILD_JAR: Build a Java JAR file. Meant to be used after the
#                 previous macro.
#
# Parameters:
#  * jar_name: the name of the JAR to be produced
#  * swig_modules: SWIG modules made with SWIG_BUILD_JAVA_MODULE
#  * package_name: same name passed into SWIG_BUILD_JAVA_MODULE
#  * manifest_txt (optional): path to Manifest.txt
########################################################################

MACRO(SWIG_BUILD_JAVA_MODULE swig_source module_name cplusplus)
    INCLUDE(UseSWIG)

    SET(SWIG_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${SWIG_MODULE_DIR}
        ${SWIG_JAVA_INCLUDE_DIRS}
        ${JAVA_INCLUDE_PATH}
        ${JNI_INCLUDE_DIRS}
    )
    INCLUDE_DIRECTORIES(${SWIG_INCLUDE_DIRS})

    SET(SWIG_LIBRARIES
        ${SWIG_JAVA_LIBRARIES}
        ${JAVA_AWT_LIBRARY}
        ${JAVA_JVM_LIBRARY}
    )

    # Set flags to pass into SWIG call
    SET(CMAKE_SWIG_FLAGS -module ${module_name} -package "nc.PKMN" ${SWIG_JAVA_FLAGS})
    FOREACH(dir ${SWIG_INCLUDE_DIRS})
        LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
    ENDFOREACH(dir ${SWIG_INCLUDE_DIRS})

    # Allows CMake variables to be placed in SWIG .i files
    CONFIGURE_FILE(
        ${SWIG_MODULE_DIR}/${swig_source}.i
        ${CMAKE_CURRENT_BINARY_DIR}/${swig_source}.i
    @ONLY)

    # Set SWIG's C++ flag if specified by the user
    IF(${cplusplus})
        SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/${swig_source}.i PROPERTIES CPLUSPLUS ON)
    ENDIF(${cplusplus})

    # The actual CMake call for SWIG
    SWIG_ADD_MODULE(${swig_source} java ${CMAKE_CURRENT_BINARY_DIR}/${swig_source}.i)
    SWIG_LINK_LIBRARIES(${swig_source} ${SWIG_LIBRARIES})

    # Add dependencies
    ADD_DEPENDENCIES(${SWIG_MODULE_${swig_source}_REAL_NAME} java_CamelCase_i pkmn_javadocs)

    # Install files
    IF(WIN32)
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_source}_REAL_NAME}
            DESTINATION bin
            COMPONENT Java
        )
    ELSE()
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_source}_REAL_NAME}
            DESTINATION lib
            COMPONENT Java
        )
    ENDIF(WIN32)

ENDMACRO(SWIG_BUILD_JAVA_MODULE)

MACRO(JAVA_BUILD_JAR jar_name swig_modules package_name manifest_txt)
    # Derive depending files from SWIG module names
    FOREACH(module ${swig_modules})
        LIST(APPEND java_depends ${SWIG_MODULE_${module}_REAL_NAME})
    ENDFOREACH(module ${swig_modules})

    # Get info from variables
    GET_FILENAME_COMPONENT(package_dir ${package_name} NAME_WE) # Not a filename, but it works
    STRING(REPLACE "." "_" jar_target ${jar_name})

    # Check for manifest file, determines a flag
    IF("${manifest_txt}" STREQUAL "")
        SET(CF_FLAG "cf")
    ELSE()
        SET(CF_FLAG "cfm")
    ENDIF("${manifest_txt}" STREQUAL "")

    # Build JAR
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${jar_name}
        COMMENT "Creating ${jar_name}"
        COMMAND ${Java_JAVAC_EXECUTABLE} -Xlint:unchecked -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/*.java
        COMMAND ${Java_JAR_EXECUTABLE} ${CF_FLAG} ${CMAKE_CURRENT_BINARY_DIR}/${jar_name} ${manifest_txt} -C ${CMAKE_CURRENT_BINARY_DIR} ${package_dir}
        DEPENDS ${java_depends}
        DEPENDS ${swig_depends}
    )
    ADD_CUSTOM_TARGET(
        ${jar_target} ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${jar_name}
    )
    SET_TARGET_PROPERTIES(${jar_target}
        PROPERTIES JAR_FILE ${CMAKE_CURRENT_BINARY_DIR}/${jar_name}
    )

    # Install file
    INSTALL(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${jar_name}
        DESTINATION share/java
        COMPONENT Java
    )
ENDMACRO(JAVA_BUILD_JAR)

MACRO(JAVA_BUILD_PROGRAM program_name jar_name)
    ADD_CUSTOM_COMMAND(
        DEPENDS ${jar_name}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${program_name}.java
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${program_name}.class
        COMMAND ${Java_JAVAC_EXECUTABLE} -classpath ${jar_name} -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${program_name}.java
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Building Java program ${program_name}"
    )
    ADD_CUSTOM_TARGET(
        ${program_name} ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${program_name}.class
    )
ENDMACRO(JAVA_BUILD_PROGRAM program_name jar_name)
