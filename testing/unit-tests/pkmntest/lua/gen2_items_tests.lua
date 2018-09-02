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

gen2_items_tests.crystal_items =
{
    pkmn.item.CLEAR_BELL, pkmn.item.GS_BALL,
    pkmn.item.BLUE_CARD, pkmn.item.EGG_TICKET
}
gen2_items_tests.wrong_generation_all_pocket_items =
{
    pkmn.item.BLACK_SLUDGE, pkmn.item.POFFIN_CASE,
    pkmn.item.TM51, pkmn.item.OCCA_BERRY
}

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
        {pkmn.item.BICYCLE, pkmn.item.MASTER_BALL, pkmn.item.HM01}
    )
    luaunit.assertEquals(item_pocket.num_items, 0)

    -- Start adding and removing stuff, and make sure the numbers are accurate.

    local items =
    {
        pkmn.item.POTION, pkmn.item.HP_UP,
        pkmn.item.WHITE_APRICORN, pkmn.item.LUCKY_EGG,
        pkmn.item.FLOWER_MAIL, pkmn.item.BURN_HEAL,
        pkmn.item.PSNCUREBERRY, pkmn.item.STICK
    }

    items_tests.item_list_test_setting_items(
        item_pocket,
        items
    )
    items_tests.item_list_test_add_remove(
        item_pocket,
        items
    )

    luaunit.assertTrue(#item_pocket.valid_items > 0)
    luaunit.assertEquals(
        #item_pocket.valid_items,
        #item_pocket.valid_item_names
    )
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
    items_tests.item_list_test_out_of_range_error(key_item_pocket, pkmn.item.BICYCLE)

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        key_item_pocket,
        {pkmn.item.POTION, pkmn.item.MASTER_BALL, pkmn.item.HM01}
    )
    luaunit.assertEquals(key_item_pocket.num_items, 0)

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        key_item_pocket,
        {pkmn.item.MACH_BIKE, pkmn.item.JADE_ORB,
         pkmn.item.LIGHT_STONE, pkmn.item.AQUA_SUIT}
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
    luaunit.assertError(pkmn.item_list.add, key_item_pocket, pkmn.item.BICYCLE, 5)
    luaunit.assertError(pkmn.item_list.remove, key_item_pocket, pkmn.item.BICYCLE, 5)

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local items =
    {
        pkmn.item.BICYCLE, pkmn.item.BASEMENT_KEY,
        pkmn.item.SECRETPOTION, pkmn.item.MYSTERY_EGG,
        pkmn.item.SILVER_WING, pkmn.item.LOST_ITEM,
        pkmn.item.SQUIRT_BOTTLE, pkmn.item.RAINBOW_WING
    }
    for item_index = 1, #items
    do
        if item_index <= 4
        then
            key_item_pocket:add(items[item_index], 1)
        else
            key_item_pocket[item_index].item = items[item_index]
        end
    end

    key_item_pocket:remove(items[2], 1)
    key_item_pocket[2].item = pkmn.item.NONE

    while key_item_pocket[1].item ~= pkmn.item.NONE
    do
        key_item_pocket[1].item = pkmn.item.NONE
    end

    luaunit.assertTrue(#key_item_pocket.valid_items > 0)
    luaunit.assertEquals(
        #key_item_pocket.valid_items,
        #key_item_pocket.valid_item_names
    )
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
    items_tests.item_list_test_out_of_range_error(ball_pocket, pkmn.item.MASTER_BALL)

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        ball_pocket,
        {pkmn.item.POTION, pkmn.item.BICYCLE, pkmn.item.HM01}
    )
    luaunit.assertEquals(ball_pocket.num_items, 0)

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local items =
    {
        pkmn.item.GREAT_BALL, pkmn.item.POKE_BALL,
        pkmn.item.PARK_BALL, pkmn.item.FAST_BALL,
        pkmn.item.MASTER_BALL, pkmn.item.FRIEND_BALL,
        pkmn.item.LOVE_BALL, pkmn.item.LEVEL_BALL
    }

    items_tests.item_list_test_setting_items(
        ball_pocket,
        items
    )
    items_tests.item_list_test_add_remove(
        ball_pocket,
        items
    )

    luaunit.assertTrue(#ball_pocket.valid_items > 0)
    luaunit.assertEquals(
        #ball_pocket.valid_items,
        #ball_pocket.valid_item_names
    )
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
        local expected_item = pkmn.item.TM01 + (i-1)

        luaunit.assertEquals(tmhm_pocket[i].item, expected_item)
        luaunit.assertEquals(tmhm_pocket[i].amount, 0)
    end
    for i = 1, 7
    do
        local expected_item = pkmn.item.HM01 + (i-1)

        luaunit.assertEquals(tmhm_pocket[50+i].item, expected_item)
        luaunit.assertEquals(tmhm_pocket[50+i].amount, 0)
    end

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(tmhm_pocket, pkmn.item.TM10)

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        tmhm_pocket,
        {pkmn.item.POTION, pkmn.item.MASTER_BALL, pkmn.item.BICYCLE}
    )
    luaunit.assertEquals(tmhm_pocket.num_items, 0)

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    for i = 1, 50
    do
        local expected_item = pkmn.item.TM01 + (i-1)

        tmhm_pocket:add(expected_item, 50)
        luaunit.assertEquals(tmhm_pocket.num_items, i)
        luaunit.assertEquals(tmhm_pocket[i].item, expected_item)
        luaunit.assertEquals(tmhm_pocket[i].amount, 50)
    end
    for i = 50, 1, -1
    do
        local expected_item = pkmn.item.TM01 + (i-1)

        tmhm_pocket:remove(expected_item, 50)
        luaunit.assertEquals(tmhm_pocket.num_items, i-1)
        luaunit.assertEquals(tmhm_pocket[i].item, expected_item)
        luaunit.assertEquals(tmhm_pocket[i].amount, 0)
    end
    for i = 1, 7
    do
        local expected_item = pkmn.item.HM01 + (i-1)

        tmhm_pocket:add(expected_item, 1)
        luaunit.assertEquals(tmhm_pocket.num_items, i)
        luaunit.assertEquals(tmhm_pocket[50+i].item, expected_item)
        luaunit.assertEquals(tmhm_pocket[50+i].amount, 1)
    end
    for i = 7, 1, -1
    do
        local expected_item = pkmn.item.HM01 + (i-1)

        tmhm_pocket:remove(expected_item, 1)
        luaunit.assertEquals(tmhm_pocket.num_items, i-1)
        luaunit.assertEquals(tmhm_pocket[50+i].item, expected_item)
        luaunit.assertEquals(tmhm_pocket[50+i].amount, 0)
    end

    luaunit.assertEquals(#tmhm_pocket.valid_items, 57)
    luaunit.assertEquals(#tmhm_pocket.valid_item_names, 57)
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
    local items =
    {
        pkmn.item.POTION, pkmn.item.BICYCLE,
        pkmn.item.GREAT_BALL, pkmn.item.TM28,
        pkmn.item.BERRY, pkmn.item.SQUIRT_BOTTLE,
        pkmn.item.FRIEND_BALL, pkmn.item.HM01
    }

    items_tests.item_list_test_setting_items(
        pc,
        items
    )
    items_tests.item_list_test_add_remove(
        pc,
        items
    )

    luaunit.assertEquals(
        #pc.valid_items,
        #pkmn.database.get_item_list(game)
    )
    luaunit.assertEquals(
        #pc.valid_items,
        #pc.valid_item_names
    )
end

function gen2_items_tests.test_item_bag(bag, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(bag.game, game)
    luaunit.assertEquals(#bag, 4)

    gen2_items_tests.test_item_pocket(bag["Items"], game)
    gen2_items_tests.test_key_item_pocket(bag["KeyItems"], game)
    gen2_items_tests.test_ball_pocket(bag["Balls"], game)
    gen2_items_tests.test_tmhm_pocket(bag["TM/HM"], game)

    local items =
    {
        pkmn.item.POTION, pkmn.item.BICYCLE,
        pkmn.item.GREAT_BALL, pkmn.item.TM28,
        pkmn.item.BERRY, pkmn.item.SQUIRT_BOTTLE,
        pkmn.item.FRIEND_BALL, pkmn.item.HM01
    }

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
    luaunit.assertEquals(bag["Items"][2].item, pkmn.item.BERRY)
    luaunit.assertEquals(bag["Items"][2].amount, 1)
    luaunit.assertEquals(bag["Items"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][3].amount, 0)

    luaunit.assertEquals(bag["KeyItems"][1].item, pkmn.item.BICYCLE)
    luaunit.assertEquals(bag["KeyItems"][1].amount, 1)
    luaunit.assertEquals(bag["KeyItems"][2].item, pkmn.item.SQUIRT_BOTTLE)
    luaunit.assertEquals(bag["KeyItems"][2].amount, 1)
    luaunit.assertEquals(bag["KeyItems"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["KeyItems"][3].amount, 0)

    luaunit.assertEquals(bag["Balls"][1].item, pkmn.item.GREAT_BALL)
    luaunit.assertEquals(bag["Balls"][1].amount, 1)
    luaunit.assertEquals(bag["Balls"][2].item, pkmn.item.FRIEND_BALL)
    luaunit.assertEquals(bag["Balls"][2].amount, 1)
    luaunit.assertEquals(bag["Balls"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Balls"][3].amount, 0)

    luaunit.assertEquals(bag["TM/HM"][1].item, pkmn.item.TM01)
    luaunit.assertEquals(bag["TM/HM"][1].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][2].item, pkmn.item.TM02)
    luaunit.assertEquals(bag["TM/HM"][2].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][28].item, pkmn.item.TM28)
    luaunit.assertEquals(bag["TM/HM"][28].amount, 1)
    luaunit.assertEquals(bag["TM/HM"][51].item, pkmn.item.HM01)
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

    luaunit.assertEquals(bag["TM/HM"][1].item, pkmn.item.TM01)
    luaunit.assertEquals(bag["TM/HM"][1].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][2].item, pkmn.item.TM02)
    luaunit.assertEquals(bag["TM/HM"][2].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][28].item, pkmn.item.TM28)
    luaunit.assertEquals(bag["TM/HM"][28].amount, 0)
    luaunit.assertEquals(bag["TM/HM"][51].item, pkmn.item.HM01)
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
