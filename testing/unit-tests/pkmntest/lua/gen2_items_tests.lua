--
-- Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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
    items_tests.item_list_test_out_of_range_error(item_pocket, pkmn.item.POTION)

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {"Bicycle", "Master Ball", "HM01"}
    )
    luaunit.assertEquals(item_pocket.num_items, 0)

    -- Start adding and removing stuff, and make sure the numbers are accurate.

    local item_names =
    {
        pkmn.item.POTION, "HP Up", "Wht Apricorn", "Lucky Egg",
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
    luaunit.assertEquals(#key_item_pocket, 25)
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
        {pkmn.item.POTION, "Master Ball", "HM01"}
    )
    luaunit.assertEquals(key_item_pocket.num_items, 0)

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        key_item_pocket,
        {"Mach Bike", "Jade Orb", "Light Stone", "Aqua Suit"}
    )
    luaunit.assertEquals(key_item_pocket.num_items, 0)

    -- Crystal-specific items
    if game == pkmn.game.CRYSTAL
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

    -- Make sure we can't add or remove more than a single item.
    luaunit.assertError(pkmn.item_list.add, key_item_pocket, "Bicycle", 5)
    luaunit.assertError(pkmn.item_list.remove, key_item_pocket, "Bicycle", 5)

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        "Bicycle", "Basement Key", "SecretPotion", "Mystery Egg",
        "Silver Wing", "Lost Item", "SquirtBottle", "Rainbow Wing"
    }
    for item_index = 1, #item_names
    do
        if item_index <= 4
        then
            key_item_pocket:add(item_names[item_index], 1)
        else
            key_item_pocket[item_index].item = item_names[item_index]
        end
    end

    key_item_pocket:remove(item_names[2], 1)
    key_item_pocket[2].item = "None"

    while key_item_pocket[1].item ~= "None"
    do
        key_item_pocket[1].item = "None"
    end

    luaunit.assertTrue(#key_item_pocket.valid_items > 0)
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
        {pkmn.item.POTION, "Bicycle", "HM01"}
    )
    luaunit.assertEquals(ball_pocket.num_items, 0)

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        "Great Ball", pkmn.item.POKE_BALL, "Park Ball", pkmn.item.FAST_BALL,
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
        {pkmn.item.POTION, "Master Ball", "Bicycle"}
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
    items_tests.item_list_test_out_of_range_error(pc, pkmn.item.POTION)

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        pkmn.item.POTION, "Bicycle", "Great Ball", "TM28",
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

    local items = {pkmn.item.POTION, "Bicycle", "Great Ball", "TM28",
                   "Berry", "SquirtBottle", "Friend Ball", "HM01"}

    -- Make sure adding items through the bag adds to the proper pockets.
    luaunit.assertEquals(bag["Items"].num_items, 0)
    luaunit.assertEquals(bag["KeyItems"].num_items, 0)
    luaunit.assertEquals(bag["Balls"].num_items, 0)
    luaunit.assertEquals(bag["TM/HM"].num_items, 0)
    for i = 1, #items
    do
        bag:add(items[i], 1)
    end

    luaunit.assertEquals(bag["Items"][1].item, pkmn.item.POTION)
    luaunit.assertEquals(bag["Items"][1].amount, 1)
    luaunit.assertEquals(bag["Items"][2].item, "Berry")
    luaunit.assertEquals(bag["Items"][2].amount, 1)
    luaunit.assertEquals(bag["Items"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][3].amount, 0)

    luaunit.assertEquals(bag["KeyItems"][1].item, "Bicycle")
    luaunit.assertEquals(bag["KeyItems"][1].amount, 1)
    luaunit.assertEquals(bag["KeyItems"][2].item, "SquirtBottle")
    luaunit.assertEquals(bag["KeyItems"][2].amount, 1)
    luaunit.assertEquals(bag["KeyItems"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["KeyItems"][3].amount, 0)

    luaunit.assertEquals(bag["Balls"][1].item, "Great Ball")
    luaunit.assertEquals(bag["Balls"][1].amount, 1)
    luaunit.assertEquals(bag["Balls"][2].item, "Friend Ball")
    luaunit.assertEquals(bag["Balls"][2].amount, 1)
    luaunit.assertEquals(bag["Balls"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Balls"][3].amount, 0)

    luaunit.assertEquals(bag["TM/HM"][1].item, "TM01")
    luaunit.assertEquals(bag["TM/HM"][1].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][2].item, "TM02")
    luaunit.assertEquals(bag["TM/HM"][2].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][28].item, "TM28")
    luaunit.assertEquals(bag["TM/HM"][28].amount, 1)
    luaunit.assertEquals(bag["TM/HM"][51].item, "HM01")
    luaunit.assertEquals(bag["TM/HM"][51].amount, 1)

    -- Make sure removing items through the bag removes from the proper pockets.
    for i = 1, #items
    do
        bag:remove(items[i], 1)
    end

    luaunit.assertEquals(bag["Items"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][1].amount, 0)
    luaunit.assertEquals(bag["Items"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][2].amount, 0)
    luaunit.assertEquals(bag["Items"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][3].amount, 0)

    luaunit.assertEquals(bag["KeyItems"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["KeyItems"][1].amount, 0)
    luaunit.assertEquals(bag["KeyItems"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["KeyItems"][2].amount, 0)
    luaunit.assertEquals(bag["KeyItems"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["KeyItems"][3].amount, 0)

    luaunit.assertEquals(bag["Balls"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Balls"][1].amount, 0)
    luaunit.assertEquals(bag["Balls"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Balls"][2].amount, 0)
    luaunit.assertEquals(bag["Balls"][3].item, pkmn.item.NONE)
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
    local items = pkmn.item_list("Items", pkmn.game.GOLD)
    gen2_items_tests.test_item_pocket(items, pkmn.game.GOLD)
end

function test_gold_key_item_pocket()
    local key_items = pkmn.item_list("KeyItems", pkmn.game.GOLD)
    gen2_items_tests.test_key_item_pocket(key_items, pkmn.game.GOLD)
end

function test_gold_ball_pocket()
    local balls = pkmn.item_list("Balls", pkmn.game.GOLD)
    gen2_items_tests.test_ball_pocket(balls, pkmn.game.GOLD)
end

function test_gold_tmhm_pocket()
    local tmhm = pkmn.item_list("TM/HM", pkmn.game.GOLD)
    gen2_items_tests.test_tmhm_pocket(tmhm, pkmn.game.GOLD)
end

function test_gold_pc()
    local pc = pkmn.item_list("PC", pkmn.game.GOLD)
    gen2_items_tests.test_pc(pc, pkmn.game.GOLD)
end

function test_gold_item_bag()
    local bag = pkmn.item_bag(pkmn.game.GOLD)
    gen2_items_tests.test_item_bag(bag, pkmn.game.GOLD)
end

--
-- Silver
--

function test_silver_item_pocket()
    local items = pkmn.item_list("Items", pkmn.game.SILVER)
    gen2_items_tests.test_item_pocket(items, pkmn.game.SILVER)
end

function test_silver_key_item_pocket()
    local key_items = pkmn.item_list("KeyItems", pkmn.game.SILVER)
    gen2_items_tests.test_key_item_pocket(key_items, pkmn.game.SILVER)
end

function test_silver_ball_pocket()
    local balls = pkmn.item_list("Balls", pkmn.game.SILVER)
    gen2_items_tests.test_ball_pocket(balls, pkmn.game.SILVER)
end

function test_silver_tmhm_pocket()
    local tmhm = pkmn.item_list("TM/HM", pkmn.game.SILVER)
    gen2_items_tests.test_tmhm_pocket(tmhm, pkmn.game.SILVER)
end

function test_silver_pc()
    local pc = pkmn.item_list("PC", pkmn.game.SILVER)
    gen2_items_tests.test_pc(pc, pkmn.game.SILVER)
end

function test_silver_item_bag()
    local bag = pkmn.item_bag(pkmn.game.SILVER)
    gen2_items_tests.test_item_bag(bag, pkmn.game.SILVER)
end

--
-- Crystal
--

function test_crystal_item_pocket()
    local items = pkmn.item_list("Items", pkmn.game.CRYSTAL)
    gen2_items_tests.test_item_pocket(items, pkmn.game.CRYSTAL)
end

function test_crystal_key_item_pocket()
    local key_items = pkmn.item_list("KeyItems", pkmn.game.CRYSTAL)
    gen2_items_tests.test_key_item_pocket(key_items, pkmn.game.CRYSTAL)
end

function test_crystal_ball_pocket()
    local balls = pkmn.item_list("Balls", pkmn.game.CRYSTAL)
    gen2_items_tests.test_ball_pocket(balls, pkmn.game.CRYSTAL)
end

function test_crystal_tmhm_pocket()
    local tmhm = pkmn.item_list("TM/HM", pkmn.game.CRYSTAL)
    gen2_items_tests.test_tmhm_pocket(tmhm, pkmn.game.CRYSTAL)
end

function test_crystal_pc()
    local pc = pkmn.item_list("PC", pkmn.game.CRYSTAL)
    gen2_items_tests.test_pc(pc, pkmn.game.CRYSTAL)
end

function test_crystal_item_bag()
    local bag = pkmn.item_bag(pkmn.game.CRYSTAL)
    gen2_items_tests.test_item_bag(bag, pkmn.game.CRYSTAL)
end
