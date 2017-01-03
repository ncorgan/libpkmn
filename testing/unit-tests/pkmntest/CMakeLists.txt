#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

ADD_SUBDIRECTORY(cpp)
IF(PKMN_ENABLE_C)
    ADD_SUBDIRECTORY(c)
ENDIF(PKMN_ENABLE_C)
IF(PKMN_ENABLE_CSHARP AND NUNIT_FOUND)
    ADD_SUBDIRECTORY(csharp)
ENDIF(PKMN_ENABLE_CSHARP AND NUNIT_FOUND)
IF(PKMN_ENABLE_PYTHON)
    ADD_SUBDIRECTORY(python)
ENDIF(PKMN_ENABLE_PYTHON)
