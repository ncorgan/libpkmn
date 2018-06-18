#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_enums_headers
        gender.hpp
        stat.hpp
    )

    INSTALL(
        FILES ${pkmn_enums_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/enums
        COMPONENT Headers
    )
ENDIF()
