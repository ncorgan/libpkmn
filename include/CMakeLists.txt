#
# Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

ADD_SUBDIRECTORY(pkmn)

IF(PKMN_ENABLE_C)
    IF(NOT PKMN_DONT_INSTALL_HEADERS)
        INSTALL(
            FILES pkmn-c.h
            DESTINATION ${PKMN_INCLUDE_DIR}
            COMPONENT Headers
        )
    ENDIF()

    ADD_SUBDIRECTORY(pkmn-c)
ENDIF()
