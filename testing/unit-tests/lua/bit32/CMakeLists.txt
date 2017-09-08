#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})

ADD_LIBRARY(bit32 SHARED bit32.c)
TARGET_LINK_LIBRARIES(bit32 ${LUA_LIBRARIES})
SET_TARGET_PROPERTIES(bit32 PROPERTIES PREFIX "")
