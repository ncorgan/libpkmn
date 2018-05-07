#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#


IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_types_headers
        attribute_engine.hpp
        class_with_attributes.hpp
        time_duration.hpp
    )

    INSTALL(
        FILES ${pkmn_types_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/types
        COMPONENT Headers
    )
ENDIF()
