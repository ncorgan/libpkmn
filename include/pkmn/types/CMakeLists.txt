#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/shared_ptr.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/shared_ptr.hpp
@ONLY)

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_types_headers
        attribute_engine.hpp
        class_with_attributes.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/shared_ptr.hpp
    )

    INSTALL(
        FILES ${pkmn_types_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/types
        COMPONENT Headers
    )
ENDIF()
