#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# The Boost.Predef check allows us to specify PKSav's endianness.
#
IF(PLATFORM_LITTLE_ENDIAN)
    SET(PKSAV_LITTLE_ENDIAN ON)
    SET(PKSAV_BIG_ENDIAN OFF)
ELSE()
    SET(PKSAV_LITTLE_ENDIAN OFF)
    SET(PKSAV_BIG_ENDIAN ON)
ENDIF(PLATFORM_LITTLE_ENDIAN)

#
# Set PKSav components
#
SET(PKSAV_ENABLE_GEN2 OFF)
SET(PKSAV_ENABLE_GBA  OFF)
SET(PKSAV_ENABLE_GCN  OFF)
SET(PKSAV_ENABLE_GEN4 OFF)
SET(PKSAV_ENABLE_GEN5 OFF)
SET(PKSAV_ENABLE_GEN6 OFF)
