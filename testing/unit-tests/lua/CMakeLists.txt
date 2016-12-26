#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# Lua tests
#
INCLUDE(PKMNLua)
LUA_CHECK_MODULE("LuaUnit" "luaunit" LUAUNIT_FOUND)

IF(LUAUNIT_FOUND)
    SET(lua_tests
        lua_lists_test
    )

    FOREACH(test ${lua_tests})
        PKMN_ADD_LUA_TEST(${test})
    ENDFOREACH(test ${lua_tests})
ENDIF(LUAUNIT_FOUND)
