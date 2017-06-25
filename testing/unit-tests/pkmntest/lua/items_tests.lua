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
    for i = 1, (items:get_capacity())
    do
        luaunit.assertEquals(items[i].item, "None")
        luaunit.assertEquals(items[i].amount, 0)
    end
end

function items_tests.item_list_test_out_of_range_error(items, item_name)
    luaunit.assertError(pkmn.item_list_sptr.add, items, item_name, 0)
    luaunit.assertError(pkmn.item_list_sptr.add, items, item_name, 100)
    luaunit.assertError(pkmn.item_list_sptr.remove, items, item_name, 0)
    luaunit.assertError(pkmn.item_list_sptr.remove, items, item_name, 100)
end

function items_tests.item_bag_test_invalid_items(bag, item_names)
    for i = 1, #item_names
    do
        luaunit.assertError(pkmn.item_bag_sptr.add, bag, item_names[i], 1)
    end
end

function items_tests.item_list_test_invalid_items(items, item_names)
    for i = 1, #item_names
    do
        luaunit.assertError(pkmn.item_list_sptr.add, items, item_names[i], 1)
    end
end

function items_tests.item_list_test_add_remove(items, item_names)
    luaunit.assertEquals(items:get_num_items(), 0)
    luaunit.assertEquals(#item_names, 8)

    items[1] = pkmn.item_slot(item_names[1], 50)
    items[2] = pkmn.item_slot(item_names[2], 40)
    items[3] = pkmn.item_slot(item_names[3], 30)

    luaunit.assertEquals(item_names[1], items[1].item)
    luaunit.assertEquals(50, items[1].amount)
    luaunit.assertEquals(item_names[2], items[2].item)
    luaunit.assertEquals(40, items[2].amount)
    luaunit.assertEquals(item_names[3], items[3].item)
    luaunit.assertEquals(30, items[3].amount)
    luaunit.assertEquals(3, items:get_num_items())

    luaunit.assertError(pkmn.item_list_sptr.set_item, 10, pkmn.item_slot("Ultra Ball", 5))
    luaunit.assertError(pkmn.item_list_sptr.set_item, 1, pkmn.item_slot("None", 5))

    luaunit.assertEquals(item_names[1], items[1].item)
    luaunit.assertEquals(50, items[1].amount)
    luaunit.assertEquals(item_names[2], items[2].item)
    luaunit.assertEquals(40, items[2].amount)
    luaunit.assertEquals(item_names[3], items[3].item)
    luaunit.assertEquals(30, items[3].amount)
    luaunit.assertEquals(3, items:get_num_items())

    items[2] = pkmn.item_slot("None", 0)

    luaunit.assertEquals(item_names[1], items[1].item)
    luaunit.assertEquals(50, items[1].amount)
    luaunit.assertEquals(item_names[3], items[2].item)
    luaunit.assertEquals(30, items[2].amount)
    luaunit.assertEquals("None", items[3].item)
    luaunit.assertEquals(0, items[3].amount)
    luaunit.assertEquals(2, items:get_num_items())

    items[1] = pkmn.item_slot("None", 0)
    items[1] = pkmn.item_slot("None", 0)

    luaunit.assertEquals("None", items[1].item)
    luaunit.assertEquals(0, items[1].amount)
    luaunit.assertEquals("None", items[2].item)
    luaunit.assertEquals(0, items[2].amount)
    luaunit.assertEquals("None", items[3].item)
    luaunit.assertEquals(0, items[3].amount)
    luaunit.assertEquals(0, items:get_num_items())

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
    luaunit.assertEquals(items:get_num_items(), 3)

    items:add(item_names[3], 15)

    luaunit.assertEquals(items[1].item, item_names[1])
    luaunit.assertEquals(items[1].amount, 30)
    luaunit.assertEquals(items[2].item, item_names[2])
    luaunit.assertEquals(items[2].amount, 99)
    luaunit.assertEquals(items[3].item, item_names[3])
    luaunit.assertEquals(items[3].amount, 16)
    luaunit.assertEquals(items:get_num_items(), 3)

    items:remove(item_names[2], 20)

    luaunit.assertEquals(items[1].item, item_names[1])
    luaunit.assertEquals(items[1].amount, 30)
    luaunit.assertEquals(items[2].item, item_names[2])
    luaunit.assertEquals(items[2].amount, 79)
    luaunit.assertEquals(items[3].item, item_names[3])
    luaunit.assertEquals(items[3].amount, 16)
    luaunit.assertEquals(items:get_num_items(), 3)

    items:move(0, 1)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 79)
    luaunit.assertEquals(items[2].item, item_names[1])
    luaunit.assertEquals(items[2].amount, 30)
    luaunit.assertEquals(items[3].item, item_names[3])
    luaunit.assertEquals(items[3].amount, 16)
    luaunit.assertEquals(items:get_num_items(), 3)

    items:remove(item_names[1], 30)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 79)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 16)
    luaunit.assertEquals(items[3].item, "None")
    luaunit.assertEquals(items[3].amount, 0)
    luaunit.assertEquals(items:get_num_items(), 2)

    items:add(item_names[4], 90)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 79)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 16)
    luaunit.assertEquals(items[3].item, item_names[4])
    luaunit.assertEquals(items[3].amount, 90)
    luaunit.assertEquals(items:get_num_items(), 3)

    items:add(item_names[5], 2)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 79)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 16)
    luaunit.assertEquals(items[3].item, item_names[4])
    luaunit.assertEquals(items[3].amount, 90)
    luaunit.assertEquals(items[4].item, item_names[5])
    luaunit.assertEquals(items[4].amount, 2)
    luaunit.assertEquals(items:get_num_items(), 4)

    items:remove(item_names[2], 30)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 49)
    luaunit.assertEquals(items[2].item, item_names[3])
    luaunit.assertEquals(items[2].amount, 16)
    luaunit.assertEquals(items[3].item, item_names[4])
    luaunit.assertEquals(items[3].amount, 90)
    luaunit.assertEquals(items[4].item, item_names[5])
    luaunit.assertEquals(items[4].amount, 2)
    luaunit.assertEquals(items:get_num_items(), 4)

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
    luaunit.assertEquals(items:get_num_items(), 5)

    items:remove(item_names[3], 16)

    luaunit.assertEquals(items[1].item, item_names[2])
    luaunit.assertEquals(items[1].amount, 49)
    luaunit.assertEquals(items[2].item, item_names[4])
    luaunit.assertEquals(items[2].amount, 90)
    luaunit.assertEquals(items[3].item, item_names[5])
    luaunit.assertEquals(items[3].amount, 2)
    luaunit.assertEquals(items[4].item, item_names[6])
    luaunit.assertEquals(items[4].amount, 12)
    luaunit.assertEquals(items:get_num_items(), 4)

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
    luaunit.assertEquals(items:get_num_items(), 5)

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
    luaunit.assertEquals(items:get_num_items(), 6)

    items:remove(item_names[5], 2)
    items:remove(item_names[2], 49)
    items:remove(item_names[8], 6)
    items:remove(item_names[6], 12)
    items:remove(item_names[4], 90)
    items:remove(item_names[7], 65)

    luaunit.assertEquals(items:get_num_items(), 0)
end

return items_tests
