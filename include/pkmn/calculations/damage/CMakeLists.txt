#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_calculations_headers
        critical_hit.hpp
        damage.hpp
        modifiers.hpp
        moves.hpp
    )

    INSTALL(
        FILES ${pkmn_calculations_damage_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/calculations/damage
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
