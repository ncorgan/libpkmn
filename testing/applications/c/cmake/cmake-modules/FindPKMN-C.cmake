#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_PATH(PKMN-C_INCLUDE_DIR pkmn-c.h)
FIND_LIBRARY(PKMN-C_LIBRARY NAMES pkmn-c)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PKMN-C DEFAULT_MSG PKMN-C_LIBRARY PKMN-C_INCLUDE_DIR)
MARK_AS_ADVANCED(PKMN-C_LIBRARY PKMN-C_INCLUDE_DIR)
