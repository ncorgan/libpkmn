#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_calculations_moves_headers
        critical_hit.hpp
        hidden_power.hpp
        modifiers.hpp
        natural_gift.hpp
        power.hpp
    )

    INSTALL(
        FILES ${pkmn_calculations_moves_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/calculations/moves
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
