#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_PACKAGE(PKHeX)

IF(PKMN_ENABLE_CSHARP AND PKHeX_FOUND)
    ADD_SUBDIRECTORY(pkhex)
ENDIF()
