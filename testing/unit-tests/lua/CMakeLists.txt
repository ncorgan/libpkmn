#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# Lua tests
#
INCLUDE(PKMNLua)
LUA_CHECK_MODULE("Bit32" "bit32" BIT32_FOUND)
LUA_CHECK_MODULE("LuaUnit" "luaunit" LUAUNIT_FOUND)

# Get stock Lua path and C path becuase environment variables override
# instead of append for whatever reason.
EXECUTE_PROCESS(
    COMMAND ${LUA_INTERPRETER} -e "print(package.path)"
    OUTPUT_VARIABLE OLD_LUA_PATH
    ERROR_QUIET
)
EXECUTE_PROCESS(
    COMMAND ${LUA_INTERPRETER} -e "print(package.cpath)"
    OUTPUT_VARIABLE OLD_LUA_CPATH
    ERROR_QUIET
)

IF(BIT32_FOUND AND LUAUNIT_FOUND)
    SET(lua_tests
        lua_calculations_test
        lua_database_entry_test
        lua_items_test
        lua_lists_test
        lua_paths_test
        lua_pokemon_test
    )

    FOREACH(test ${lua_tests})
        PKMN_ADD_LUA_TEST(${test})
    ENDFOREACH(test ${lua_tests})
ENDIF(BIT32_FOUND AND LUAUNIT_FOUND)
