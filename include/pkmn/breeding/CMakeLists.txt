#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_breeding_headers
        child_info.hpp
        compatibility.hpp
    )

    INSTALL(
        FILES ${pkmn_breeding_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/breeding
        COMPONENT Headers
    )
ENDIF()
