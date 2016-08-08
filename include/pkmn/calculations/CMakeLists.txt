#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_calculations_headers
        form.hpp
        gender.hpp
        hidden_power.hpp
        shininess.hpp
        spinda_spots.hpp
        stats.hpp
    )

    INSTALL(
        FILES ${pkmn_calculations_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/calculations
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
