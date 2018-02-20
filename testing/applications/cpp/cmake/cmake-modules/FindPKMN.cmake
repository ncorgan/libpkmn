#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_PATH(PKMN_INCLUDE_DIR pkmn/config.hpp)
FIND_LIBRARY(PKMN_LIBRARY NAMES pkmn)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PKMN DEFAULT_MSG PKMN_LIBRARY PKMN_INCLUDE_DIR)
MARK_AS_ADVANCED(PKMN_LIBRARY PKMN_INCLUDE_DIR)
