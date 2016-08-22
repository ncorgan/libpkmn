#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_database_headers
        items.h
        lists.h
        moves.h
        pokemon.h
    )

    INSTALL(
        FILES ${pkmn_c_database_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c/database
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
