--
-- Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local items_tests = require("items_tests")

local gen1_items_tests = {}

function gen1_items_tests.test_item_list_common(items, game)
    luaunit.assertEquals(#items, items:get_capacity())

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(items)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(items, "Potion")

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    items_tests.item_list_test_add_remove(
        items,
        {"Potion", "Great Ball", "Ether", "PP Up",
         "TM34", "Moon Stone", "Bicycle", "Full Heal"}
    )

    local valid_items = items:get_valid_items()
    local full_item_list = pkmn.database.get_item_list(game)
    luaunit.assertEquals(#valid_items, #full_item_list)
end

function gen1_items_tests.test_item_list(items, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(items:get_name(), "Items")
    luaunit.assertEquals(items:get_game(), game)
    luaunit.assertEquals(items:get_capacity(), 20)
    luaunit.assertEquals(items:get_num_items(), 0)

    gen1_items_tests.test_item_list_common(items, game)
end

function gen1_items_tests.test_item_pc(pc, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(pc:get_name(), "PC")
    luaunit.assertEquals(pc:get_game(), game)
    luaunit.assertEquals(pc:get_capacity(), 50)
    luaunit.assertEquals(pc:get_num_items(), 0)

    gen1_items_tests.test_item_list_common(pc, game)
end

function gen1_items_tests.test_item_bag(bag, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(bag:get_game(), game)

    local pockets = bag:get_pockets()
    luaunit.assertEquals(#pockets, 1)
    luaunit.assertTrue(pockets:has_key("Items"))

    gen1_items_tests.test_item_list(pockets["Items"], game)
end

--
-- Red
--

function test_red_item_list()
    local items = pkmn.item_list("Items", "Red")
    gen1_items_tests.test_item_list(items, "Red")
end

function test_red_item_pc()
    local pc = pkmn.item_list("PC", "Red")
    gen1_items_tests.test_item_pc(pc, "Red")
end

function test_red_item_bag()
    local bag = pkmn.item_bag("Red")
    gen1_items_tests.test_item_bag(bag, "Red")
end

--
-- Blue
--

function test_blue_item_list()
    local items = pkmn.item_list("Items", "Blue")
    gen1_items_tests.test_item_list(items, "Blue")
end

function test_blue_item_pc()
    local pc = pkmn.item_list("PC", "Blue")
    gen1_items_tests.test_item_pc(pc, "Blue")
end

function test_blue_item_bag()
    local bag = pkmn.item_bag("Blue")
    gen1_items_tests.test_item_bag(bag, "Blue")
end

--
-- Yellow
--

function test_yellow_item_list()
    local items = pkmn.item_list("Items", "Yellow")
    gen1_items_tests.test_item_list(items, "Yellow")
end

function test_yellow_item_pc()
    local pc = pkmn.item_list("PC", "Yellow")
    gen1_items_tests.test_item_pc(pc, "Yellow")
end

function test_yellow_item_bag()
    local bag = pkmn.item_bag("Yellow")
    gen1_items_tests.test_item_bag(bag, "Yellow")
end
