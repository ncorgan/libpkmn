#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pkmn_headers
    config.hpp
)

INSTALL(
    FILES ${pkmn_headers}
    DESTINATION ${INCLUDE_DIR}/pkmn
    COMPONENT Headers
)
