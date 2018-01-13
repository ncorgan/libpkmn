--
-- Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local items_tests = {}

function items_tests.item_list_test_empty_slot(items)
    for item_index = 1, #items
    do
        luaunit.assertEquals(items[item_index].item, "None")
        luaunit.assertEquals(items[item_index].amount, 0)
    end
end

function items_tests.item_list_test_out_of_range_error(items, item_name)
    luaunit.assertError(pkmn.item_list.add, items, item_name, 0)
    luaunit.assertError(pkmn.item_list.add, items, item_name, 100)
    luaunit.assertError(pkmn.item_list.remove, items, item_name, 0)
    luaunit.assertError(pkmn.item_list.remove, items, item_name, 100)
end

function items_tests.item_bag_test_invalid_items(bag, item_names)
    for i = 1, #item_names
    do
        luaunit.assertError(pkmn.item_bag.add, bag, item_names[i], 1)
    end
end

function items_tests.item_list_test_invalid_items(items, item_names)
    for i = 1, #item_names
    do
        luaunit.assertError(pkmn.item_list.add, items, item_names[i], 1)
    end
end

function items_tests.item_list_test_setting_items(items, item_names)
    luaunit.assertEquals(items.num_items, 0)
    luaunit.assertEquals(#item_names, 8)

    luaunit.assertError(pkmn.item_list.__getitem, items, 0)
    luaunit.assertError(pkmn.item_list.__getitem, items, #items+1)

    items[1].item = item_names[1]
    items[1].amount = 50
    items[2].item = item_names[2]
    items[2].amount = 40
    items[3].item = item_names[3]
    items[3].amount = 30

    luaunit.assertEquals(items.num_items, 3)
    luaunit.assertEquals(items[1].item, item_names[1])
    luaunit.assertEquals(items[1].amount, 50)
    luaunit.assertEquals(items[2].item, item_names[2])
    luaunit.assertEquals(items[2].amount, 40)
    luaunit.assertEquals(items[3].item, item_names[3])
    luaunit.assertEquals(items[3].amount, 30)

    items[2].item = "None"

    luaunit.assertEquals(items.num_items, 2)
    luaunit.assertEquals(items[1].item, item_names[1])
    luaunit.assertEquals(items[1].amount, 50)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 30)
    luaunit.assertEquals(items[3].item, "None")
    luaunit.assertEquals(items[3].amount, 0)

    items[1].item = "None"
    items[1].item = "None"

    luaunit.assertEquals(items.num_items, 0)
    for item_index = 1, 3
    do
        luaunit.assertEquals(items[item_index].item, "None")
        luaunit.assertEquals(items[item_index].amount, 0)
    end
end

function items_tests.item_list_test_add_remove(items, item_names)
    luaunit.assertEquals(items.num_items, 0)
    luaunit.assertEquals(#item_names, 8)

    -- Test add/remove functions.
    items:add(item_names[1], 30)
    items:add(item_names[2], 99)
    items:add(item_names[3], 1)

    luaunit.assertEquals(items[1].item, item_names[1])
    luaunit.assertEquals(items[1].amount, 30)
    luaunit.assertEquals(items[2].item, item_names[2])
    luaunit.assertEquals(items[2].amount, 99)
    luaunit.assertEquals(items[3].item, item_names[3])
    luaunit.assertEquals(items[3].amount, 1)
    luaunit.assertEquals(items.num_items, 3)

    items:add(item_names[3], 15)

    luaunit.assertEquals(items[1].item, item_names[1])
    luaunit.assertEquals(items[1].amount, 30)
    luaunit.assertEquals(items[2].item, item_names[2])
    luaunit.assertEquals(items[2].amount, 99)
    luaunit.assertEquals(items[3].item, item_names[3])
    luaunit.assertEquals(items[3].amount, 16)
    luaunit.assertEquals(items.num_items, 3)

    items:remove(item_names[2], 20)

    luaunit.assertEquals(items[1].item, item_names[1])
    luaunit.assertEquals(items[1].amount, 30)
    luaunit.assertEquals(items[2].item, item_names[2])
    luaunit.assertEquals(items[2].amount, 79)
    luaunit.assertEquals(items[3].item, item_names[3])
    luaunit.assertEquals(items[3].amount, 16)
    luaunit.assertEquals(items.num_items, 3)

    items:move(1, 2)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 79)
    luaunit.assertEquals(items[2].item, item_names[1])
    luaunit.assertEquals(items[2].amount, 30)
    luaunit.assertEquals(items[3].item, item_names[3])
    luaunit.assertEquals(items[3].amount, 16)
    luaunit.assertEquals(items.num_items, 3)

    items:remove(item_names[1], 30)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 79)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 16)
    luaunit.assertEquals(items[3].item, "None")
    luaunit.assertEquals(items[3].amount, 0)
    luaunit.assertEquals(items.num_items, 2)

    items:add(item_names[4], 90)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 79)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 16)
    luaunit.assertEquals(items[3].item, item_names[4])
    luaunit.assertEquals(items[3].amount, 90)
    luaunit.assertEquals(items.num_items, 3)

    items:add(item_names[5], 2)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 79)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 16)
    luaunit.assertEquals(items[3].item, item_names[4])
    luaunit.assertEquals(items[3].amount, 90)
    luaunit.assertEquals(items[4].item, item_names[5])
    luaunit.assertEquals(items[4].amount, 2)
    luaunit.assertEquals(items.num_items, 4)

    items:remove(item_names[2], 30)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 49)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 16)
    luaunit.assertEquals(items[3].item, item_names[4])
    luaunit.assertEquals(items[3].amount, 90)
    luaunit.assertEquals(items[4].item, item_names[5])
    luaunit.assertEquals(items[4].amount, 2)
    luaunit.assertEquals(items.num_items, 4)

    items:add(item_names[6], 12)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 49)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 16)
    luaunit.assertEquals(items[3].item, item_names[4])
    luaunit.assertEquals(items[3].amount, 90)
    luaunit.assertEquals(items[4].item, item_names[5])
    luaunit.assertEquals(items[4].amount, 2)
    luaunit.assertEquals(items[5].item, item_names[6])
    luaunit.assertEquals(items[5].amount, 12)
    luaunit.assertEquals(items.num_items, 5)

    items:remove(item_names[3], 16)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 49)
    luaunit.assertEquals(items[2].item, item_names[4])
    luaunit.assertEquals(items[2].amount, 90)
    luaunit.assertEquals(items[3].item, item_names[5])
    luaunit.assertEquals(items[3].amount, 2)
    luaunit.assertEquals(items[4].item, item_names[6])
    luaunit.assertEquals(items[4].amount, 12)
    luaunit.assertEquals(items.num_items, 4)

    items:add(item_names[7], 65)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 49)
    luaunit.assertEquals(items[2].item, item_names[4])
    luaunit.assertEquals(items[2].amount, 90)
    luaunit.assertEquals(items[3].item, item_names[5])
    luaunit.assertEquals(items[3].amount, 2)
    luaunit.assertEquals(items[4].item, item_names[6])
    luaunit.assertEquals(items[4].amount, 12)
    luaunit.assertEquals(items[5].item, item_names[7])
    luaunit.assertEquals(items[5].amount, 65)
    luaunit.assertEquals(items.num_items, 5)

    items:add(item_names[8], 6)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 49)
    luaunit.assertEquals(items[2].item, item_names[4])
    luaunit.assertEquals(items[2].amount, 90)
    luaunit.assertEquals(items[3].item, item_names[5])
    luaunit.assertEquals(items[3].amount, 2)
    luaunit.assertEquals(items[4].item, item_names[6])
    luaunit.assertEquals(items[4].amount, 12)
    luaunit.assertEquals(items[5].item, item_names[7])
    luaunit.assertEquals(items[5].amount, 65)
    luaunit.assertEquals(items[6].item, item_names[8])
    luaunit.assertEquals(items[6].amount, 6)
    luaunit.assertEquals(items.num_items, 6)

    items:remove(item_names[5], 2)
    items:remove(item_names[2], 49)
    items:remove(item_names[8], 6)
    items:remove(item_names[6], 12)
    items:remove(item_names[4], 90)
    items:remove(item_names[7], 65)

    luaunit.assertEquals(items.num_items, 0)
end

return items_tests
