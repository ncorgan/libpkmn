#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_gui_headers
        spinda.h
    )

    INSTALL(
        FILES ${pkmn_c_gui_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c/gui
        COMPONENT Headers
    )
ENDIF()
