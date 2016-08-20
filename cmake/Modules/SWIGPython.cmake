#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying FILE LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# This file creates the macro SWIG_BUILD_PYTHON_MODULE. It builds a
# Python SWIG module and installs it in the desired location without the
# user having to deal with CMake SWIG details.
#
# Macro parameters:
#  * module_name: The module filename, minus the .i extension.
#  * install_dir: The desired location within the Python directory.
#  * cplusplus:   TRUE or FALSE, whether or not this is a C++ module.
#
# Assumes the following variables are set:
#  * SWIG_MODULE_DIR:          The source directory that contains the .i
#                              file. This is needed so the .i file can
#                              include other local .i files.
#
#  * SWIG_PYTHON_INCLUDE_DIRS: C/C++ include directories needed by the
#                              source files.
#
#  * SWIG_PYTHON_LIBRARIES:    C/C++ libraries the Python module should
#                              link against.
#
#  * SWIG_PYTHON_FLAGS:        Flags to pass into the SWIG command (optional).
#
# Example (mymodule.i, mysubmodule.i):
#  * SWIG_BUILD_PYTHON_MODULE(mymodule mymodule TRUE)
#  * SWIG_BUILD_PYTHON_MODULE(mysubmodule mymodule/mysubmodule TRUE)
#
#  * From Python, call:
#        import mymodule
#        import mymodule.mysubmodule
#
########################################################################

# Only run the install location check once
IF(DEFINED __INCLUDED_SWIGPYTHON_CMAKE)
    RETURN()
ENDIF(DEFINED __INCLUDED_SWIGPYTHON_CMAKE)
SET(__INCLUDED_SWIGPYTHON_CMAKE TRUE)

########################################################################
# Set the Python install directory
########################################################################
EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "
from distutils import sysconfig
print(sysconfig.get_python_lib(plat_specific=True, prefix=''))
    " OUTPUT_VARIABLE PYTHON_INSTALL_DIR OUTPUT_STRIP_TRAILING_WHITESPACE
)
FILE(TO_CMAKE_PATH ${PYTHON_INSTALL_DIR} PYTHON_INSTALL_DIR)
SET(PYTHON_INSTALL_DIR ${PYTHON_INSTALL_DIR} CACHE FILEPATH "Python install directory")

########################################################################
# Macro to build and install Python SWIG modules
########################################################################
MACRO(SWIG_BUILD_PYTHON_MODULE module_name install_dir cplusplus)
    INCLUDE(UseSWIG)

    SET(SWIG_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${SWIG_MODULE_DIR}
        ${SWIG_PYTHON_INCLUDE_DIRS}
        ${PYTHON_INCLUDE_DIRS}
    )
    INCLUDE_DIRECTORIES(${SWIG_INCLUDE_DIRS})

    SET(SWIG_LIBRARIES
        ${SWIG_PYTHON_LIBRARIES}
        ${PYTHON_LIBRARIES}
    )

    # Set flags to pass into SWIG call
    SET(CMAKE_SWIG_FLAGS -module ${module_name} ${SWIG_PYTHON_FLAGS})
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
    SWIG_ADD_MODULE(${module_name} python ${CMAKE_CURRENT_BINARY_DIR}/${module_name}.i)
    SWIG_LINK_LIBRARIES(${module_name} ${SWIG_LIBRARIES})

    SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME}
        PROPERTIES COMPILE_FLAGS "${PKMN_CXX_FLAGS}"
    )
    ADD_DEPENDENCIES(${SWIG_MODULE_${module_name}_REAL_NAME}
        pkmn_python_docstrings
    )

    # Make sure SWIG artifacts are placed in same location on each platform
    # Makes Python unit testing easier
    IF(MSVC)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}
            LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}
        )
    ENDIF(MSVC)

    # Install files
    SET(py_files
        ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
        ${CMAKE_CURRENT_BINARY_DIR}/${module_name}.py
    )

    INSTALL(
        FILES ${py_files}
        DESTINATION ${PYTHON_INSTALL_DIR}/${install_dir}
        COMPONENT Python
    )
    INSTALL(
        TARGETS ${SWIG_MODULE_${module_name}_REAL_NAME}
        DESTINATION ${PYTHON_INSTALL_DIR}/${install_dir}
        COMPONENT Python
    )

ENDMACRO(SWIG_BUILD_PYTHON_MODULE)
