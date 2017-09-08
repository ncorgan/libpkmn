#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pokemon_io_tests.lua.in
    ${CMAKE_CURRENT_BINARY_DIR}/pokemon_io_tests.lua
@ONLY)
