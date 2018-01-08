#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_utils_headers
        floating_point_comparison.hpp
        paths.hpp
    )

    INSTALL(
        FILES ${pkmn_utils_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/utils
        COMPONENT Headers
    )
ENDIF()
