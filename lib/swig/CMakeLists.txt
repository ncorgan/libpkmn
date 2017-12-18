#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(PKMN_ENABLE_CSHARP)
    ADD_SUBDIRECTORY(csharp)
ENDIF()
IF(PKMN_ENABLE_LUA)
    ADD_SUBDIRECTORY(lua)
ENDIF()
IF(PKMN_ENABLE_PYTHON)
    ADD_SUBDIRECTORY(python)
ENDIF()
IF(PKMN_ENABLE_RUBY)
    ADD_SUBDIRECTORY(ruby)
ENDIF()
