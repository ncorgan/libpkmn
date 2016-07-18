#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_database_headers
        item_entry.hpp
        levelup_move.hpp
        lists.hpp
        move_entry.hpp
    )

    INSTALL(
        FILES ${pkmn_database_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/database
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
