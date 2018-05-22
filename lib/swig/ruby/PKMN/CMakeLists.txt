#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(SWIGRuby)

#
# These files needs to be in the working directory when SWIG is called.
#
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/ruby/rubycontainer.swg
    ${CMAKE_CURRENT_BINARY_DIR}/rubycontainer.swg
@ONLY)
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/ruby/rubyfragments.swg
    ${CMAKE_CURRENT_BINARY_DIR}/rubyfragments.swg
@ONLY)

#
# Set variables needed for CMake macros
#

SET(SWIG_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(SWIG_RUBY_INCLUDE_DIRS
    ${SWIG_MODULE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${SWIG_MODULE_DIR}/../..
    ${SWIG_MODULE_DIR}/../../modules
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
)

SET(SWIG_RUBY_LIBRARIES
    pkmn
)

SWIG_BUILD_RUBY_MODULE(PKMNRuby     TRUE)
SWIG_BUILD_RUBY_MODULE(Breeding     TRUE)
SWIG_BUILD_RUBY_MODULE(BuildInfo    TRUE)
SWIG_BUILD_RUBY_MODULE(Calculations TRUE)
SWIG_BUILD_RUBY_MODULE(Database     TRUE)
SWIG_BUILD_RUBY_MODULE(GUI          TRUE)
SWIG_BUILD_RUBY_MODULE(Paths        TRUE)
SWIG_BUILD_RUBY_MODULE(STL          TRUE)

#
# The PKMN module's SWIG file needs to have a different filename
# because pkmn.i is already in use, and Windows uses a case-insensitive
# filesystem.
#
SET_TARGET_PROPERTIES(${SWIG_MODULE_PKMNRuby_REAL_NAME} PROPERTIES OUTPUT_NAME "PKMN")
