--
-- Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local items_tests = require("items_tests")

local gba_items_tests = {}

gba_items_tests.all_pocket_items =
{
    pkmn.item.POTION, "Mach Bike", "Great Ball", "TM01",
    "Aspear Berry", "Wailmer Pail", "Master Ball", "HM04"
}
gba_items_tests.wrong_game_all_pocket_items =
{
    "Pink Bow", "Black Sludge",
    "Ein File S", "Gonzap's Key",
    "GS Ball", "Poffin Items",
    "TM51",
    "Berry", "Occa Berry"
}

function gba_items_tests.test_item_pocket(item_pocket, game)
    local expected_capacity = 0
    if game == pkmn.game.RUBY or game == pkmn.game.SAPPHIRE
    then
        expected_capacity = 20
    elseif game == pkmn.game.EMERALD
    then
        expected_capacity = 30
    else
        expected_capacity = 42
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#item_pocket, expected_capacity)
    luaunit.assertEquals(item_pocket.name, "Items")
    luaunit.assertEquals(item_pocket.game, game)
    luaunit.assertEquals(item_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(item_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(item_pocket, pkmn.item.POTION)

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {"Bicycle", "Master Ball", "HM01", "Razz Berry"}
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"}
    )

    -- Make sure we can't add items from Gamecube games.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {"Time Flute", "Poké Snack"}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        pkmn.item.POTION, "Orange Mail", "Lava Cookie", "Stardust",
        "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"
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

function gba_items_tests.test_key_item_pocket(key_item_pocket, game)
    local expected_capacity = 0
    if game == pkmn.game.RUBY or game == pkmn.game.SAPPHIRE
    then
        expected_capacity = 20
    else
        expected_capacity = 30
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#key_item_pocket, expected_capacity)
    luaunit.assertEquals(key_item_pocket.name, "Key Items")
    luaunit.assertEquals(key_item_pocket.game, game)
    luaunit.assertEquals(key_item_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(key_item_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(key_item_pocket, "Basement Key")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {pkmn.item.POTION, "Master Ball", "HM01", "Razz Berry"}
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"}
    )

    -- Make sure we can't add items from Gamecube games.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {"Ein File S", "Powerup Part",
         "Gonzap's Key", "Krane Memo 1"}
    )

    if game == pkmn.game.RUBY or game == pkmn.game.SAPPHIRE
    then
        items_tests.item_list_test_invalid_items(
            key_item_pocket,
            {"Helix Fossil", "Tea", pkmn.game.RUBY}
        )
    end
    if game ~= pkmn.game.EMERALD
    then
        items_tests.item_list_test_invalid_items(
            key_item_pocket,
            {"Magma Emblem", "Old Sea Map"}
        )
    end

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        "Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
        "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"
    }

    items_tests.item_list_test_setting_items(
        key_item_pocket,
        item_names
    )
    items_tests.item_list_test_add_remove(
        key_item_pocket,
        item_names
    )

    luaunit.assertTrue(#key_item_pocket.valid_items > 0)
end

function gba_items_tests.test_ball_pocket(ball_pocket, game)
    local expected_capacity = 0
    if game == pkmn.game.FIRERED or game == pkmn.game.LEAFGREEN
    then
        expected_capacity = 13
    else
        expected_capacity = 16
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#ball_pocket, expected_capacity)
    luaunit.assertEquals(ball_pocket.name, "Poké Balls")
    luaunit.assertEquals(ball_pocket.game, game)
    luaunit.assertEquals(ball_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(ball_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(ball_pocket, "Master Ball")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        ball_pocket,
        {pkmn.item.POTION, "Bicycle", "HM01", "Razz Berry"}
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        ball_pocket,
        {"Moon Ball", "Heal Ball", "Dream Ball"}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        "Master Ball", "Ultra Ball", "Great Ball", pkmn.item.POKE_BALL,
        "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"
    }

    items_tests.item_list_test_setting_items(
        ball_pocket,
        item_names
    )
    items_tests.item_list_test_add_remove(
        ball_pocket,
        item_names
    )

    luaunit.assertTrue(#ball_pocket.valid_items > 0)
end

function gba_items_tests.test_tmhm_pocket(tmhm_pocket, game)
    local expected_capacity = 0
    local pocket_name = ""
    if game == pkmn.game.FIRERED or game == pkmn.game.LEAFGREEN
    then
        expected_capacity = 58
        pocket_name = "TM Case"
    else
        expected_capacity = 64
        pocket_name = "TMs & HMs"
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#tmhm_pocket, expected_capacity)
    luaunit.assertEquals(tmhm_pocket.name, pocket_name)
    luaunit.assertEquals(tmhm_pocket.game, game)
    luaunit.assertEquals(tmhm_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(tmhm_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(tmhm_pocket, "TM01")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        tmhm_pocket,
        {pkmn.item.POTION, "Bicycle", "Great Ball", "Razz Berry"}
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        tmhm_pocket,
        {"TM51"}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        "TM01", "HM01", "TM02", "HM02",
        "TM03", "HM03", "TM04", "HM04"
    }

    items_tests.item_list_test_setting_items(
        tmhm_pocket,
        item_names
    )
    items_tests.item_list_test_add_remove(
        tmhm_pocket,
        item_names
    )

    luaunit.assertTrue(#tmhm_pocket.valid_items > 0)
end

function gba_items_tests.test_berry_pocket(berry_pocket, game)
    local expected_capacity = 0
    local pocket_name = ""
    if game == pkmn.game.FIRERED or game == pkmn.game.LEAFGREEN
    then
        expected_capacity = 43
        pocket_name = "Berry Pouch"
    else
        expected_capacity = 46
        pocket_name = "Berries"
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#berry_pocket, expected_capacity)
    luaunit.assertEquals(berry_pocket.name, pocket_name)
    luaunit.assertEquals(berry_pocket.game, game)
    luaunit.assertEquals(berry_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(berry_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(berry_pocket, "Razz Berry")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        berry_pocket,
        {pkmn.item.POTION, "Bicycle", "Great Ball", "HM02"}
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        berry_pocket,
        {"Berry", "Occa Berry", "Roseli Berry"}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local item_names =
    {
        "Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
        "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"
    }

    items_tests.item_list_test_setting_items(
        berry_pocket,
        item_names
    )
    items_tests.item_list_test_add_remove(
        berry_pocket,
        item_names
    )

    luaunit.assertTrue(#berry_pocket.valid_items > 0)
end

function gba_items_tests.test_pc(pc, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(#pc, 50)
    luaunit.assertEquals(pc.name, "PC")
    luaunit.assertEquals(pc.game, game)
    luaunit.assertEquals(pc.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(pc)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(pc, pkmn.item.POTION)

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        pc,
        gba_items_tests.wrong_game_all_pocket_items
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    items_tests.item_list_test_setting_items(
        pc,
        gba_items_tests.all_pocket_items
    )
    items_tests.item_list_test_add_remove(
        pc,
        gba_items_tests.all_pocket_items
    )

    luaunit.assertTrue(#pc.valid_items > 0)
end

function gba_items_tests.test_item_bag(bag, game)
    local tmhm_pocket_name = ""
    local berry_pocket_name = ""
    if game == pkmn.game.FIRERED or game == pkmn.game.LEAFGREEN
    then
        tmhm_pocket_name = "TM Case"
        berry_pocket_name = "Berry Pouch"
    else
        tmhm_pocket_name = "TMs & HMs"
        berry_pocket_name = "Berries"
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(bag.game, game)
    luaunit.assertEquals(#bag, 5)

    gba_items_tests.test_item_pocket(bag["Items"], game)
    gba_items_tests.test_key_item_pocket(bag["Key Items"], game)
    gba_items_tests.test_ball_pocket(bag["Poké Balls"], game)
    gba_items_tests.test_tmhm_pocket(bag[tmhm_pocket_name], game)
    gba_items_tests.test_berry_pocket(bag[berry_pocket_name], game)

    -- Make sure adding items through the bag adds to the proper pocket.
    luaunit.assertEquals(bag["Items"].num_items, 0)
    luaunit.assertEquals(bag["Key Items"].num_items, 0)
    luaunit.assertEquals(bag["Poké Balls"].num_items, 0)
    luaunit.assertEquals(bag[tmhm_pocket_name].num_items, 0)
    luaunit.assertEquals(bag[berry_pocket_name].num_items, 0)

    for i = 1, #gba_items_tests.all_pocket_items
    do
        bag:add(gba_items_tests.all_pocket_items[i], 5)
    end

    luaunit.assertEquals(bag["Items"][1].item, pkmn.item.POTION)
    luaunit.assertEquals(bag["Items"][1].amount, 5)
    luaunit.assertEquals(bag["Items"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][2].amount, 0)

    luaunit.assertEquals(bag["Key Items"][1].item, "Mach Bike")
    luaunit.assertEquals(bag["Key Items"][1].amount, 5)
    luaunit.assertEquals(bag["Key Items"][2].item, "Wailmer Pail")
    luaunit.assertEquals(bag["Key Items"][2].amount, 5)
    luaunit.assertEquals(bag["Key Items"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Key Items"][3].amount, 0)

    luaunit.assertEquals(bag["Poké Balls"][1].item, "Great Ball")
    luaunit.assertEquals(bag["Poké Balls"][1].amount, 5)
    luaunit.assertEquals(bag["Poké Balls"][2].item, "Master Ball")
    luaunit.assertEquals(bag["Poké Balls"][2].amount, 5)
    luaunit.assertEquals(bag["Poké Balls"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Poké Balls"][3].amount, 0)

    luaunit.assertEquals(bag[tmhm_pocket_name][1].item, "TM01")
    luaunit.assertEquals(bag[tmhm_pocket_name][1].amount, 5)
    luaunit.assertEquals(bag[tmhm_pocket_name][2].item, "HM04")
    luaunit.assertEquals(bag[tmhm_pocket_name][2].amount, 5)
    luaunit.assertEquals(bag[tmhm_pocket_name][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag[tmhm_pocket_name][3].amount, 0)

    luaunit.assertEquals(bag[berry_pocket_name][1].item, "Aspear Berry")
    luaunit.assertEquals(bag[berry_pocket_name][1].amount, 5)
    luaunit.assertEquals(bag[berry_pocket_name][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag[berry_pocket_name][2].amount, 0)

    -- Make sure removing items through the bag removes from the proper pocket.
    for i = 1, #gba_items_tests.all_pocket_items
    do
        bag:remove(gba_items_tests.all_pocket_items[i], 5)
    end

    luaunit.assertEquals(bag["Items"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][1].amount, 0)
    luaunit.assertEquals(bag["Items"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][2].amount, 0)

    luaunit.assertEquals(bag["Key Items"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Key Items"][1].amount, 0)
    luaunit.assertEquals(bag["Key Items"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Key Items"][2].amount, 0)
    luaunit.assertEquals(bag["Key Items"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Key Items"][3].amount, 0)

    luaunit.assertEquals(bag["Poké Balls"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Poké Balls"][1].amount, 0)
    luaunit.assertEquals(bag["Poké Balls"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Poké Balls"][2].amount, 0)
    luaunit.assertEquals(bag["Poké Balls"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Poké Balls"][3].amount, 0)

    luaunit.assertEquals(bag[tmhm_pocket_name][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag[tmhm_pocket_name][1].amount, 0)
    luaunit.assertEquals(bag[tmhm_pocket_name][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag[tmhm_pocket_name][2].amount, 0)
    luaunit.assertEquals(bag[tmhm_pocket_name][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag[tmhm_pocket_name][3].amount, 0)

    luaunit.assertEquals(bag[berry_pocket_name][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag[berry_pocket_name][1].amount, 0)
    luaunit.assertEquals(bag[berry_pocket_name][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag[berry_pocket_name][2].amount, 0)

    -- Make sure we can't add items from other generations.
    items_tests.item_bag_test_invalid_items(
        bag,
        gba_items_tests.wrong_game_all_pocket_items
    )
end

--
-- Ruby
--

function test_ruby_item_pocket()
    local item_pocket = pkmn.item_list("Items", pkmn.game.RUBY)
    gba_items_tests.test_item_pocket(item_pocket, pkmn.game.RUBY)
end

function test_ruby_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", pkmn.game.RUBY)
    gba_items_tests.test_key_item_pocket(key_item_pocket, pkmn.game.RUBY)
end

function test_ruby_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", pkmn.game.RUBY)
    gba_items_tests.test_ball_pocket(ball_pocket, pkmn.game.RUBY)
end

function test_ruby_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TMs & HMs", pkmn.game.RUBY)
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, pkmn.game.RUBY)
end

function test_ruby_berry_pocket()
    local berry_pocket = pkmn.item_list("Berries", pkmn.game.RUBY)
    gba_items_tests.test_berry_pocket(berry_pocket, pkmn.game.RUBY)
end

function test_ruby_pc()
    local pc = pkmn.item_list("PC", pkmn.game.RUBY)
    gba_items_tests.test_pc(pc, pkmn.game.RUBY)
end

function test_ruby_item_bag()
    local item_bag = pkmn.item_bag(pkmn.game.RUBY)
    gba_items_tests.test_item_bag(item_bag, pkmn.game.RUBY)
end

--
-- Sapphire
--

function test_sapphire_item_pocket()
    local item_pocket = pkmn.item_list("Items", pkmn.game.SAPPHIRE)
    gba_items_tests.test_item_pocket(item_pocket, pkmn.game.SAPPHIRE)
end

function test_sapphire_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", pkmn.game.SAPPHIRE)
    gba_items_tests.test_key_item_pocket(key_item_pocket, pkmn.game.SAPPHIRE)
end

function test_sapphire_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", pkmn.game.SAPPHIRE)
    gba_items_tests.test_ball_pocket(ball_pocket, pkmn.game.SAPPHIRE)
end

function test_sapphire_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TMs & HMs", pkmn.game.SAPPHIRE)
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, pkmn.game.SAPPHIRE)
end

function test_sapphire_berry_pocket()
    local berry_pocket = pkmn.item_list("Berries", pkmn.game.SAPPHIRE)
    gba_items_tests.test_berry_pocket(berry_pocket, pkmn.game.SAPPHIRE)
end

function test_sapphire_pc()
    local pc = pkmn.item_list("PC", pkmn.game.SAPPHIRE)
    gba_items_tests.test_pc(pc, pkmn.game.SAPPHIRE)
end

function test_sapphire_item_bag()
    local item_bag = pkmn.item_bag(pkmn.game.SAPPHIRE)
    gba_items_tests.test_item_bag(item_bag, pkmn.game.SAPPHIRE)
end

--
-- Emerald
--

function test_emerald_item_pocket()
    local item_pocket = pkmn.item_list("Items", pkmn.game.EMERALD)
    gba_items_tests.test_item_pocket(item_pocket, pkmn.game.EMERALD)
end

function test_emerald_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", pkmn.game.EMERALD)
    gba_items_tests.test_key_item_pocket(key_item_pocket, pkmn.game.EMERALD)
end

function test_emerald_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", pkmn.game.EMERALD)
    gba_items_tests.test_ball_pocket(ball_pocket, pkmn.game.EMERALD)
end

function test_emerald_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TMs & HMs", pkmn.game.EMERALD)
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, pkmn.game.EMERALD)
end

function test_emerald_berry_pocket()
    local berry_pocket = pkmn.item_list("Berries", pkmn.game.EMERALD)
    gba_items_tests.test_berry_pocket(berry_pocket, pkmn.game.EMERALD)
end

function test_emerald_pc()
    local pc = pkmn.item_list("PC", pkmn.game.EMERALD)
    gba_items_tests.test_pc(pc, pkmn.game.EMERALD)
end

function test_emerald_item_bag()
    local item_bag = pkmn.item_bag(pkmn.game.EMERALD)
    gba_items_tests.test_item_bag(item_bag, pkmn.game.EMERALD)
end

--
-- FireRed
--

function test_firered_item_pocket()
    local item_pocket = pkmn.item_list("Items", pkmn.game.FIRERED)
    gba_items_tests.test_item_pocket(item_pocket, pkmn.game.FIRERED)
end

function test_firered_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", pkmn.game.FIRERED)
    gba_items_tests.test_key_item_pocket(key_item_pocket, pkmn.game.FIRERED)
end

function test_firered_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", pkmn.game.FIRERED)
    gba_items_tests.test_ball_pocket(ball_pocket, pkmn.game.FIRERED)
end

function test_firered_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TM Case", pkmn.game.FIRERED)
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, pkmn.game.FIRERED)
end

function test_firered_berry_pocket()
    local berry_pocket = pkmn.item_list("Berry Pouch", pkmn.game.FIRERED)
    gba_items_tests.test_berry_pocket(berry_pocket, pkmn.game.FIRERED)
end

function test_firered_pc()
    local pc = pkmn.item_list("PC", pkmn.game.FIRERED)
    gba_items_tests.test_pc(pc, pkmn.game.FIRERED)
end

function test_firered_item_bag()
    local item_bag = pkmn.item_bag(pkmn.game.FIRERED)
    gba_items_tests.test_item_bag(item_bag, pkmn.game.FIRERED)
end

--
-- LeafGreen
--

function test_leafgreen_item_pocket()
    local item_pocket = pkmn.item_list("Items", pkmn.game.LEAFGREEN)
    gba_items_tests.test_item_pocket(item_pocket, pkmn.game.LEAFGREEN)
end

function test_leafgreen_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", pkmn.game.LEAFGREEN)
    gba_items_tests.test_key_item_pocket(key_item_pocket, pkmn.game.LEAFGREEN)
end

function test_leafgreen_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", pkmn.game.LEAFGREEN)
    gba_items_tests.test_ball_pocket(ball_pocket, pkmn.game.LEAFGREEN)
end

function test_leafgreen_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TM Case", pkmn.game.LEAFGREEN)
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, pkmn.game.LEAFGREEN)
end

function test_leafgreen_berry_pocket()
    local berry_pocket = pkmn.item_list("Berry Pouch", pkmn.game.LEAFGREEN)
    gba_items_tests.test_berry_pocket(berry_pocket, pkmn.game.LEAFGREEN)
end

function test_leafgreen_pc()
    local pc = pkmn.item_list("PC", pkmn.game.LEAFGREEN)
    gba_items_tests.test_pc(pc, pkmn.game.LEAFGREEN)
end

function test_leafgreen_item_bag()
    local item_bag = pkmn.item_bag(pkmn.game.LEAFGREEN)
    gba_items_tests.test_item_bag(item_bag, pkmn.game.LEAFGREEN)
end
