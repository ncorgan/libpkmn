#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(SWIGJava)

#
# Set variables needed for CMake macros
#

SET(SWIG_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(SWIG_JAVA_INCLUDE_DIRS
    ${SWIG_MODULE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${SWIG_MODULE_DIR}/..
    ${SWIG_MODULE_DIR}/../modules
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
)

SET(SWIG_JAVA_LIBRARIES
    pkmn
)

SWIG_BUILD_JAVA_MODULE(env_java nc.Env TRUE)
JAVA_BUILD_JAR("PKMN.jar" "env_java" "nc" "")
