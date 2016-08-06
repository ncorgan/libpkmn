#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_headers
        build_info.hpp
        config.hpp
        item_bag.hpp
        item_list.hpp
        item_slot.hpp
        pokemon.hpp
    )

    INSTALL(
        FILES ${pkmn_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)

ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(types)
ADD_SUBDIRECTORY(utils)
