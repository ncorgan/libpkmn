#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_calculations_headers
        form.h
        gender.h
        hidden_power.h
        nature.h
        personality.h
        shininess.h
        size.h
        spinda_spots.h
        stats.h
    )

    INSTALL(
        FILES ${pkmn_c_calculations_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c/calculations
        COMPONENT Headers
    )
ENDIF()
