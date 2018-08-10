--
-- Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local items_tests = require("items_tests")

local gen1_items_tests = {}

gen1_items_tests.invalid_generation_items =
{
    "Amulet Coin", "Apicot Berry", "Air Mail",
    "Air Balloon", "Aqua Suit"
};


gen1_items_tests.valid_item_names =
{
    pkmn.item.POTION, "Great Ball", "Ether", "PP Up",
    "TM34", "Moon Stone", "Bicycle", "Full Heal"
}

function gen1_items_tests.test_item_list_common(items, game)
    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(items)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(items, pkmn.item.POTION)

    -- Confirm items from later generations can't be added.
    items_tests.item_list_test_invalid_items(
        items,
        gen1_items_tests.invalid_generation_items
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    items_tests.item_list_test_setting_items(
        items,
        gen1_items_tests.valid_item_names
    )
    items_tests.item_list_test_add_remove(
        items,
        gen1_items_tests.valid_item_names
    )

    local full_item_list = pkmn.database.get_item_list(game)
    luaunit.assertEquals(#items.valid_items, #full_item_list)
end

function gen1_items_tests.test_item_list(items, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(items.name, "Items")
    luaunit.assertEquals(items.game, game)
    luaunit.assertEquals(#items, 20)
    luaunit.assertEquals(items.num_items, 0)

    gen1_items_tests.test_item_list_common(items, game)
end

function gen1_items_tests.test_item_pc(pc, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(pc.name, "PC")
    luaunit.assertEquals(pc.game, game)
    luaunit.assertEquals(#pc, 50)
    luaunit.assertEquals(pc.num_items, 0)

    gen1_items_tests.test_item_list_common(pc, game)
end

function gen1_items_tests.test_item_bag(bag, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(bag.game, game)
    luaunit.assertEquals(#bag, 1)

    gen1_items_tests.test_item_list(bag["Items"], game)

    -- Confirm items from later generations can't be added.
    items_tests.item_bag_test_invalid_items(
        bag,
        gen1_items_tests.invalid_generation_items
    )

    -- Make sure adding items through the bag adds to the pocket.
    luaunit.assertEquals(bag["Items"].num_items, 0)

    local items = {pkmn.item.POTION, "Great Ball", "Ether", "PP Up",
                   "TM34", "Moon Stone", "Bicycle", "Full Heal"}
    for i = 1, #items
    do
        bag:add(items[i], i)
    end
    for i = 1, #items
    do
        luaunit.assertEquals(bag["Items"][i].item, items[i])
        luaunit.assertEquals(bag["Items"][i].amount, i)
    end
    luaunit.assertEquals(bag["Items"][9].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][9].amount, 0)

    for i = 1, #items
    do
        bag:remove(items[i], i)
    end
    for i = 1, #items+1
    do
        luaunit.assertEquals(bag["Items"][i].item, pkmn.item.NONE)
        luaunit.assertEquals(bag["Items"][i].amount, 0)
    end
end

--
-- Red
--

function test_red_item_list()
    local items = pkmn.item_list("Items", pkmn.game.RED)
    gen1_items_tests.test_item_list(items, pkmn.game.RED)
end

function test_red_item_pc()
    local pc = pkmn.item_list("PC", pkmn.game.RED)
    gen1_items_tests.test_item_pc(pc, pkmn.game.RED)
end

function test_red_item_bag()
    local bag = pkmn.item_bag(pkmn.game.RED)
    gen1_items_tests.test_item_bag(bag, pkmn.game.RED)
end

--
-- Blue
--

function test_blue_item_list()
    local items = pkmn.item_list("Items", pkmn.game.BLUE)
    gen1_items_tests.test_item_list(items, pkmn.game.BLUE)
end

function test_blue_item_pc()
    local pc = pkmn.item_list("PC", pkmn.game.BLUE)
    gen1_items_tests.test_item_pc(pc, pkmn.game.BLUE)
end

function test_blue_item_bag()
    local bag = pkmn.item_bag(pkmn.game.BLUE)
    gen1_items_tests.test_item_bag(bag, pkmn.game.BLUE)
end

--
-- Yellow
--

function test_yellow_item_list()
    local items = pkmn.item_list("Items", pkmn.game.YELLOW)
    gen1_items_tests.test_item_list(items, pkmn.game.YELLOW)
end

function test_yellow_item_pc()
    local pc = pkmn.item_list("PC", pkmn.game.YELLOW)
    gen1_items_tests.test_item_pc(pc, pkmn.game.YELLOW)
end

function test_yellow_item_bag()
    local bag = pkmn.item_bag(pkmn.game.YELLOW)
    gen1_items_tests.test_item_bag(bag, pkmn.game.YELLOW)
end
