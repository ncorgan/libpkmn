#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pkmn_calculations_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/form.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gender.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/nature.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/personality.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shininess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/size.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spinda_spots.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/moves/critical_hit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/moves/damage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/moves/hidden_power.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/moves/modifiers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/moves/natural_gift.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/moves/power.cpp
PARENT_SCOPE)
