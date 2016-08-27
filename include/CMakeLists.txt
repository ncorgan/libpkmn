#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

ADD_SUBDIRECTORY(pkmn)

IF(PKMN_ENABLE_C)
    IF(NOT PKMN_DONT_INSTALL_HEADERS)
        INSTALL(
            FILES pkmn.h
            DESTINATION ${PKMN_INCLUDE_DIR}
            COMPONENT Headers
        )
    ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)

    ADD_SUBDIRECTORY(pkmn-c)
ENDIF(PKMN_ENABLE_C)
