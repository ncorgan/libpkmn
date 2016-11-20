#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(PKMN_ENABLE_C)
    # TODO: remove when PKSav's copy is built
    ADD_SUBDIRECTORY(Unity)
ENDIF(PKMN_ENABLE_C)

ADD_SUBDIRECTORY(unit-tests)
