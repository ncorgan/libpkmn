#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_calculations_moves_headers
        critical_hit.h
        damage.h
        hidden_power.h
        modifiers.h
        natural_gift.h
        power.h
    )

    INSTALL(
        FILES ${pkmn_c_calculations_moves_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c/calculations/damage
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
