#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(PKMN_ENABLE_CSHARP)
    ADD_SUBDIRECTORY(csharp)
ENDIF(PKMN_ENABLE_CSHARP)
IF(PKMN_ENABLE_PYTHON)
    ADD_SUBDIRECTORY(python)
ENDIF(PKMN_ENABLE_PYTHON)
