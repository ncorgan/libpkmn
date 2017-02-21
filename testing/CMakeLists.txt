#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(PKMN_ENABLE_C)
    # Use PKSav's copy if it's already built
    IF(NOT PKSAV_ENABLE_TESTS)
        ADD_SUBDIRECTORY(Unity)
    ENDIF(NOT PKSAV_ENABLE_TESTS)
ENDIF(PKMN_ENABLE_C)

ADD_SUBDIRECTORY(googletest/googletest)

ADD_SUBDIRECTORY(unit-tests)
