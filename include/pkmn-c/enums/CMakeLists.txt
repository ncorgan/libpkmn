#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_types_headers
        condition.h
        contest_stat.h
        game.h
        gender.h
        language.h
        marking.h
        stat.h
    )

    INSTALL(
        FILES ${pkmn_c_types_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c/types
        COMPONENT Headers
    )
ENDIF()
