--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")
local pkmn = require("pkmn")

function assertInRange(num, min_num, max_num)
    luaunit.assertTrue(num >= min_num)
    luaunit.assertTrue(num <= max_num)
end

function test_current_datetime()
    local now = pkmn.current_datetime()
    luaunit.assertTrue(now.year >= 1900)
    assertInRange(now.month, 1, 12)
    assertInRange(now.day, 1, 31)
    assertInRange(now.hour, 0, 23)
    assertInRange(now.minute, 0, 59)
    assertInRange(now.second, 0, 59)
    luaunit.assertEquals(0, now.frames)
end

function test_datetime_equality()
    local datetime1 = pkmn.datetime(1,2,3,4,5,6,7)
    local datetime2 = pkmn.datetime(1,2,3,4,5,6,7)
    local datetime3 = pkmn.datetime(3,2,1,4,5,6,7)

    luaunit.assertEquals(datetime1, datetime2)
    luaunit.assertNotEquals(datetime1, datetime3)
    luaunit.assertNotEquals(datetime2, datetime3)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
