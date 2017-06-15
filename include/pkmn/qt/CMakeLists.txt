#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_qt_headers
        Spinda.hpp
    )

    INSTALL(
        FILES ${pkmn_qt_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/qt
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
