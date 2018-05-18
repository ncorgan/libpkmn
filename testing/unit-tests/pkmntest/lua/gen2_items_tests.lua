--
-- Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local items_tests = require("items_tests")

local gen2_items_tests = {}

gen2_items_tests.crystal_items = {"Clear Bell", "GS Ball", "Blue Card", "Egg Ticket"}
gen2_items_tests.wrong_generation_all_pocket_items = {"Black Sludge", "Poffin Items", "TM51", "Occa Berry"}

function gen2_items_tests.test_item_pocket(item_pocket, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(#item_pocket, 20)
    luaunit.assertEquals(item_pocket.name, "Items")
    luaunit.assertEquals(item_pocket.game, game)
    luaunit.assertEquals(item_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(item_pocket)

    -- Confirms errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(item_pocket, "Potion")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {"Bicycle", "Master Ball", "HM01"}
    )
    luaunit.assertEquals(item_pocket.num_items, 0)

    -- Start adding and removing stuff, and make sure the numbers are accurate.

    local item_names =
    {
        "Potion", "HP Up", "Wht Apricorn", "Lucky Egg",
        "Flower Mail", "Burn Heal", "PSNCureBerry", "Stick"
    }

    items_tests.item_list_test_setting_items(
        item_pocket,
        item_names
    )
    items_tests.item_list_test_add_remove(
        item_pocket,
        item_names
    )

    luaunit.assertTrue(#item_pocket.valid_items > 0)
end

function gen2_items_tests.test_key_item_pocket(key_item_pocket, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(#key_item_pocket, 26)
    luaunit.assertEquals(key_item_pocket.name, "KeyItems")
    luaunit.assertEquals(key_item_pocket.game, game)
    luaunit.assertEquals(key_item_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(key_item_pocket)

    -- Confirms errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(key_item_pocket, "Bicycle")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        key_item_pocket,
        {"Potion", "Master Ball", "HM01"}
    )
    luaunit.assertEquals(key_item_pocket.num_items, 0)

    -- Crystal-specific items
    if game == "Crystal"
    then
        for i = 1, #gen2_items_tests.crystal_items
        do
            key_item_pocket:add(gen2_items_tests.crystal_items[i], 1)
            key_item_pocket:remove(gen2_items_tests.crystal_items[i], 1)
        end
    else
        for i = 1, #gen2_items_tests.crystal_items
        do
            luaunit.assertError(pkmn.item_list.add, key_item_pocket, gen2_items_tests.crystal_items[i], 1)
            luaunit.assertError(pkmn.item_list.remove, key_item_pocket, gen2_items_tests.crystal_items[i], 1)
        end
    end
    luaunit.assertEquals(key_item_pocket.num_items, 0)

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        "Bicycle", "Basement Key", "SecretPotion", "Mystery Egg",
        "Silver Wing", "Lost Item", "SquirtBottle", "Rainbow Wing"
    }

    items_tests.item_list_test_setting_items(
        key_item_pocket,
        item_names
    )
    items_tests.item_list_test_add_remove(
        key_item_pocket,
        item_names
    )

    valid_items = key_item_pocket.valid_items
    luaunit.assertTrue(#valid_items > 0)
end

function gen2_items_tests.test_ball_pocket(ball_pocket, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(#ball_pocket, 12)
    luaunit.assertEquals(ball_pocket.name, "Balls")
    luaunit.assertEquals(ball_pocket.game, game)
    luaunit.assertEquals(ball_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(ball_pocket)

    -- Confirms errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(ball_pocket, "Master Ball")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        ball_pocket,
        {"Potion", "Bicycle", "HM01"}
    )
    luaunit.assertEquals(ball_pocket.num_items, 0)

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        "Great Ball", "Poké Ball", "Park Ball", "Fast Ball",
        "Master Ball", "Friend Ball", "Love Ball", "Level Ball"
    }

    items_tests.item_list_test_setting_items(
        ball_pocket,
        item_names
    )
    items_tests.item_list_test_add_remove(
        ball_pocket,
        item_names
    )

    local valid_items = ball_pocket.valid_items
    luaunit.assertTrue(#valid_items > 0)
end

function gen2_items_tests.test_tmhm_pocket(tmhm_pocket, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(#tmhm_pocket, 57)
    luaunit.assertEquals(tmhm_pocket.name, "TM/HM")
    luaunit.assertEquals(tmhm_pocket.game, game)
    luaunit.assertEquals(tmhm_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    for i = 1, 50
    do
        local name = string.format("TM%02d", i)
        luaunit.assertEquals(tmhm_pocket[i].item, name)
        luaunit.assertEquals(tmhm_pocket[i].amount, 0)
    end
    for i = 1, 7
    do
        local name = string.format("HM%02d", i)
        luaunit.assertEquals(tmhm_pocket[50+i].item, name)
        luaunit.assertEquals(tmhm_pocket[50+i].amount, 0)
    end

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(tmhm_pocket, "TM10")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        tmhm_pocket,
        {"Potion", "Master Ball", "Bicycle"}
    )
    luaunit.assertEquals(tmhm_pocket.num_items, 0)

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    for i = 1, 50
    do
        local name = string.format("TM%02d", i)
        tmhm_pocket:add(name, 50)
        luaunit.assertEquals(tmhm_pocket.num_items, i)
        luaunit.assertEquals(tmhm_pocket[i].item, name)
        luaunit.assertEquals(tmhm_pocket[i].amount, 50)
    end
    for i = 50, 1, -1
    do
        local name = string.format("TM%02d", i)
        tmhm_pocket:remove(name, 50)
        luaunit.assertEquals(tmhm_pocket.num_items, i-1)
        luaunit.assertEquals(tmhm_pocket[i].item, name)
        luaunit.assertEquals(tmhm_pocket[i].amount, 0)
    end
    for i = 1, 7
    do
        local name = string.format("HM%02d", i)
        tmhm_pocket:add(name, 1)
        luaunit.assertEquals(tmhm_pocket.num_items, i)
        luaunit.assertEquals(tmhm_pocket[50+i].item, name)
        luaunit.assertEquals(tmhm_pocket[50+i].amount, 1)
    end
    for i = 7, 1, -1
    do
        local name = string.format("HM%02d", i)
        tmhm_pocket:remove(name, 1)
        luaunit.assertEquals(tmhm_pocket.num_items, i-1)
        luaunit.assertEquals(tmhm_pocket[50+i].item, name)
        luaunit.assertEquals(tmhm_pocket[50+i].amount, 0)
    end

    local valid_items = tmhm_pocket.valid_items
    luaunit.assertEquals(#tmhm_pocket, 57)
end

function gen2_items_tests.test_pc(pc, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(#pc, 50)
    luaunit.assertEquals(pc.name, "PC")
    luaunit.assertEquals(pc.game, game)
    luaunit.assertEquals(pc.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(pc)

    -- Confirms errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(pc, "Potion")

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        "Potion", "Bicycle", "Great Ball", "TM28",
        "Berry", "SquirtBottle", "Friend Ball", "HM01"
    }

    items_tests.item_list_test_setting_items(
        pc,
        item_names
    )
    items_tests.item_list_test_add_remove(
        pc,
        item_names
    )

    local valid_items = pc.valid_items
    local full_item_list = pkmn.database.get_item_list(game)
    luaunit.assertEquals(#valid_items, #full_item_list)
end

function gen2_items_tests.test_item_bag(bag, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(bag.game, game)
    luaunit.assertEquals(#bag, 4)

    gen2_items_tests.test_item_pocket(bag["Items"], game)
    gen2_items_tests.test_key_item_pocket(bag["KeyItems"], game)
    gen2_items_tests.test_ball_pocket(bag["Balls"], game)
    gen2_items_tests.test_tmhm_pocket(bag["TM/HM"], game)

    local items = {"Potion", "Bicycle", "Great Ball", "TM28",
                   "Berry", "SquirtBottle", "Friend Ball", "HM01"}

    -- Make sure adding items through the bag adds to the proper pockets.
    luaunit.assertEquals(bag["Items"].num_items, 0)
    luaunit.assertEquals(bag["KeyItems"].num_items, 0)
    luaunit.assertEquals(bag["Balls"].num_items, 0)
    luaunit.assertEquals(bag["TM/HM"].num_items, 0)
    for i = 1, #items
    do
        bag:add(items[i], 5)
    end

    luaunit.assertEquals(bag["Items"][1].item, "Potion")
    luaunit.assertEquals(bag["Items"][1].amount, 5)
    luaunit.assertEquals(bag["Items"][2].item, "Berry")
    luaunit.assertEquals(bag["Items"][2].amount, 5)
    luaunit.assertEquals(bag["Items"][3].item, "None")
    luaunit.assertEquals(bag["Items"][3].amount, 0)

    luaunit.assertEquals(bag["KeyItems"][1].item, "Bicycle")
    luaunit.assertEquals(bag["KeyItems"][1].amount, 5)
    luaunit.assertEquals(bag["KeyItems"][2].item, "SquirtBottle")
    luaunit.assertEquals(bag["KeyItems"][2].amount, 5)
    luaunit.assertEquals(bag["KeyItems"][3].item, "None")
    luaunit.assertEquals(bag["KeyItems"][3].amount, 0)

    luaunit.assertEquals(bag["Balls"][1].item, "Great Ball")
    luaunit.assertEquals(bag["Balls"][1].amount, 5)
    luaunit.assertEquals(bag["Balls"][2].item, "Friend Ball")
    luaunit.assertEquals(bag["Balls"][2].amount, 5)
    luaunit.assertEquals(bag["Balls"][3].item, "None")
    luaunit.assertEquals(bag["Balls"][3].amount, 0)

    luaunit.assertEquals(bag["TM/HM"][1].item, "TM01")
    luaunit.assertEquals(bag["TM/HM"][1].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][2].item, "TM02")
    luaunit.assertEquals(bag["TM/HM"][2].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][28].item, "TM28")
    luaunit.assertEquals(bag["TM/HM"][28].amount, 5)
    luaunit.assertEquals(bag["TM/HM"][51].item, "HM01")
    luaunit.assertEquals(bag["TM/HM"][51].amount, 5)

    -- Make sure removing items through the bag removes from the proper pockets.
    for i = 1, #items
    do
        bag:remove(items[i], 5)
    end

    luaunit.assertEquals(bag["Items"][1].item, "None")
    luaunit.assertEquals(bag["Items"][1].amount, 0)
    luaunit.assertEquals(bag["Items"][2].item, "None")
    luaunit.assertEquals(bag["Items"][2].amount, 0)
    luaunit.assertEquals(bag["Items"][3].item, "None")
    luaunit.assertEquals(bag["Items"][3].amount, 0)

    luaunit.assertEquals(bag["KeyItems"][1].item, "None")
    luaunit.assertEquals(bag["KeyItems"][1].amount, 0)
    luaunit.assertEquals(bag["KeyItems"][2].item, "None")
    luaunit.assertEquals(bag["KeyItems"][2].amount, 0)
    luaunit.assertEquals(bag["KeyItems"][3].item, "None")
    luaunit.assertEquals(bag["KeyItems"][3].amount, 0)

    luaunit.assertEquals(bag["Balls"][1].item, "None")
    luaunit.assertEquals(bag["Balls"][1].amount, 0)
    luaunit.assertEquals(bag["Balls"][2].item, "None")
    luaunit.assertEquals(bag["Balls"][2].amount, 0)
    luaunit.assertEquals(bag["Balls"][3].item, "None")
    luaunit.assertEquals(bag["Balls"][3].amount, 0)

    luaunit.assertEquals(bag["TM/HM"][1].item, "TM01")
    luaunit.assertEquals(bag["TM/HM"][1].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][2].item, "TM02")
    luaunit.assertEquals(bag["TM/HM"][2].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][28].item, "TM28")
    luaunit.assertEquals(bag["TM/HM"][28].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][51].item, "HM01")
    luaunit.assertEquals(bag["TM/HM"][51].amount, 0)

    -- Make sure we can't add items from later generations.
    items_tests.item_bag_test_invalid_items(
        bag,
        gen2_items_tests.wrong_generation_all_pocket_items
    )
end

--
-- Gold
--

function test_gold_item_pocket()
    local items = pkmn.item_list("Items", "Gold")
    gen2_items_tests.test_item_pocket(items, "Gold")
end

function test_gold_key_item_pocket()
    local key_items = pkmn.item_list("KeyItems", "Gold")
    gen2_items_tests.test_key_item_pocket(key_items, "Gold")
end

function test_gold_ball_pocket()
    local balls = pkmn.item_list("Balls", "Gold")
    gen2_items_tests.test_ball_pocket(balls, "Gold")
end

function test_gold_tmhm_pocket()
    local tmhm = pkmn.item_list("TM/HM", "Gold")
    gen2_items_tests.test_tmhm_pocket(tmhm, "Gold")
end

function test_gold_pc()
    local pc = pkmn.item_list("PC", "Gold")
    gen2_items_tests.test_pc(pc, "Gold")
end

function test_gold_item_bag()
    local bag = pkmn.item_bag("Gold")
    gen2_items_tests.test_item_bag(bag, "Gold")
end

--
-- Silver
--

function test_silver_item_pocket()
    local items = pkmn.item_list("Items", "Silver")
    gen2_items_tests.test_item_pocket(items, "Silver")
end

function test_silver_key_item_pocket()
    local key_items = pkmn.item_list("KeyItems", "Silver")
    gen2_items_tests.test_key_item_pocket(key_items, "Silver")
end

function test_silver_ball_pocket()
    local balls = pkmn.item_list("Balls", "Silver")
    gen2_items_tests.test_ball_pocket(balls, "Silver")
end

function test_silver_tmhm_pocket()
    local tmhm = pkmn.item_list("TM/HM", "Silver")
    gen2_items_tests.test_tmhm_pocket(tmhm, "Silver")
end

function test_silver_pc()
    local pc = pkmn.item_list("PC", "Silver")
    gen2_items_tests.test_pc(pc, "Silver")
end

function test_silver_item_bag()
    local bag = pkmn.item_bag("Silver")
    gen2_items_tests.test_item_bag(bag, "Silver")
end

--
-- Crystal
--

function test_crystal_item_pocket()
    local items = pkmn.item_list("Items", "Crystal")
    gen2_items_tests.test_item_pocket(items, "Crystal")
end

function test_crystal_key_item_pocket()
    local key_items = pkmn.item_list("KeyItems", "Crystal")
    gen2_items_tests.test_key_item_pocket(key_items, "Crystal")
end

function test_crystal_ball_pocket()
    local balls = pkmn.item_list("Balls", "Crystal")
    gen2_items_tests.test_ball_pocket(balls, "Crystal")
end

function test_crystal_tmhm_pocket()
    local tmhm = pkmn.item_list("TM/HM", "Crystal")
    gen2_items_tests.test_tmhm_pocket(tmhm, "Crystal")
end

function test_crystal_pc()
    local pc = pkmn.item_list("PC", "Crystal")
    gen2_items_tests.test_pc(pc, "Crystal")
end

function test_crystal_item_bag()
    local bag = pkmn.item_bag("Crystal")
    gen2_items_tests.test_item_bag(bag, "Crystal")
end
