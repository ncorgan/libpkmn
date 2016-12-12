#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(PKMN_ENABLE_LUA)
    ADD_SUBDIRECTORY(lua)
ENDIF(PKMN_ENABLE_LUA)
IF(PKMN_ENABLE_PYTHON)
    ADD_SUBDIRECTORY(python)
ENDIF(PKMN_ENABLE_PYTHON)
