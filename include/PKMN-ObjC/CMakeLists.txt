#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_objc_headers
        PKItemSlot.h
        PKItemSlotArray.h
    )

    INSTALL(
        FILES ${pkmn_objc_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/PKMN-ObjC
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)

ADD_SUBDIRECTORY(Database)
ADD_SUBDIRECTORY(Types)
ADD_SUBDIRECTORY(Utils)
