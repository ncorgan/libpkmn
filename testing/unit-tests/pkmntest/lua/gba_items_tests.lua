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

gba_items_tests.all_pocket_items = {
    "Potion", "Mach Bike", "Great Ball", "TM01",
    "Aspear Berry", "Wailmer Pail", "Master Ball", "HM04"
}
gba_items_tests.wrong_game_all_pocket_items = {
    "Pink Bow", "Black Sludge",
    "Ein File S", "Gonzap's Key",
    "GS Ball", "Poffin Items",
    "TM51",
    "Berry", "Occa Berry"
}

function gba_items_tests.test_item_pocket(item_pocket, game)
    local expected_capacity = 0
    if game == "Ruby" or game == "Sapphire"
    then
        expected_capacity = 20
    elseif game == "Emerald"
    then
        expected_capacity = 30
    else
        expected_capacity = 42
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#item_pocket, expected_capacity)
    luaunit.assertEquals(item_pocket:get_name(), "Items")
    luaunit.assertEquals(item_pocket:get_game(), game)
    luaunit.assertEquals(item_pocket:get_capacity(), expected_capacity)
    luaunit.assertEquals(item_pocket:get_num_items(), 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(item_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(item_pocket, "Potion")

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
    items_tests.item_list_test_add_remove(
        item_pocket,
        {"Potion", "Orange Mail", "Lava Cookie", "Stardust",
         "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"}
    )

    local valid_items = item_pocket:get_valid_items()
    luaunit.assertTrue(#valid_items > 0)
end

function gba_items_tests.test_key_item_pocket(key_item_pocket, game)
    local expected_capacity = 0
    if game == "Ruby" or game == "Sapphire"
    then
        expected_capacity = 20
    else
        expected_capacity = 30
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#key_item_pocket, expected_capacity)
    luaunit.assertEquals(key_item_pocket:get_name(), "Key Items")
    luaunit.assertEquals(key_item_pocket:get_game(), game)
    luaunit.assertEquals(key_item_pocket:get_capacity(), expected_capacity)
    luaunit.assertEquals(key_item_pocket:get_num_items(), 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(key_item_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(key_item_pocket, "Basement Key")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {"Potion", "Master Ball", "HM01", "Razz Berry"}
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

    if game == "Ruby" or game == "Sapphire"
    then
        items_tests.item_list_test_invalid_items(
            key_item_pocket,
            {"Helix Fossil", "Tea", "Ruby"}
        )
    end
    if game ~= "Emerald"
    then
        items_tests.item_list_test_invalid_items(
            key_item_pocket,
            {"Magma Emblem", "Old Sea Map"}
        )
    end

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    items_tests.item_list_test_add_remove(
        key_item_pocket,
        {"Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
         "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"}
    )

    local valid_items = key_item_pocket:get_valid_items()
    luaunit.assertTrue(#valid_items > 0)
end

function gba_items_tests.test_ball_pocket(ball_pocket, game)
    local expected_capacity = 0
    if game == "FireRed" or game == "LeafGreen"
    then
        expected_capacity = 13
    else
        expected_capacity = 16
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#ball_pocket, expected_capacity)
    luaunit.assertEquals(ball_pocket:get_name(), "Poké Balls")
    luaunit.assertEquals(ball_pocket:get_game(), game)
    luaunit.assertEquals(ball_pocket:get_capacity(), expected_capacity)
    luaunit.assertEquals(ball_pocket:get_num_items(), 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(ball_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(ball_pocket, "Master Ball")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        ball_pocket,
        {"Potion", "Bicycle", "HM01", "Razz Berry"}
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        ball_pocket,
        {"Moon Ball", "Heal Ball", "Dream Ball"}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    items_tests.item_list_test_add_remove(
        ball_pocket,
        {"Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
         "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"}
    )

    local valid_items = ball_pocket:get_valid_items()
    luaunit.assertTrue(#valid_items > 0)
end

function gba_items_tests.test_tmhm_pocket(tmhm_pocket, game)
    local expected_capacity = 0
    local pocket_name = ""
    if game == "FireRed" or game == "LeafGreen"
    then
        expected_capacity = 58
        pocket_name = "TM Case"
    else
        expected_capacity = 64
        pocket_name = "TMs & HMs"
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#tmhm_pocket, expected_capacity)
    luaunit.assertEquals(tmhm_pocket:get_name(), pocket_name)
    luaunit.assertEquals(tmhm_pocket:get_game(), game)
    luaunit.assertEquals(tmhm_pocket:get_capacity(), expected_capacity)
    luaunit.assertEquals(tmhm_pocket:get_num_items(), 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(tmhm_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(tmhm_pocket, "TM01")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        tmhm_pocket,
        {"Potion", "Bicycle", "Great Ball", "Razz Berry"}
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        tmhm_pocket,
        {"TM51"}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    items_tests.item_list_test_add_remove(
        tmhm_pocket,
        {"TM01", "HM01", "TM02", "HM02",
         "TM03", "HM03", "TM04", "HM04"}
    )

    local valid_items = tmhm_pocket:get_valid_items()
    luaunit.assertTrue(#valid_items > 0)
end

function gba_items_tests.test_berry_pocket(berry_pocket, game)
    local expected_capacity = 0
    local pocket_name = ""
    if game == "FireRed" or game == "LeafGreen"
    then
        expected_capacity = 43
        pocket_name = "Berry Pouch"
    else
        expected_capacity = 46
        pocket_name = "Berries"
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#berry_pocket, expected_capacity)
    luaunit.assertEquals(berry_pocket:get_name(), pocket_name)
    luaunit.assertEquals(berry_pocket:get_game(), game)
    luaunit.assertEquals(berry_pocket:get_capacity(), expected_capacity)
    luaunit.assertEquals(berry_pocket:get_num_items(), 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(berry_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(berry_pocket, "Razz Berry")

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        berry_pocket,
        {"Potion", "Bicycle", "Great Ball", "HM02"}
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        berry_pocket,
        {"Berry", "Occa Berry", "Roseli Berry"}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    items_tests.item_list_test_add_remove(
        berry_pocket,
        {"Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
         "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"}
    )

    local valid_items = berry_pocket:get_valid_items()
    luaunit.assertTrue(#valid_items > 0)
end

function gba_items_tests.test_pc(pc, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(#pc, 50)
    luaunit.assertEquals(pc:get_name(), "PC")
    luaunit.assertEquals(pc:get_game(), game)
    luaunit.assertEquals(pc:get_capacity(), 50)
    luaunit.assertEquals(pc:get_num_items(), 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(pc)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(pc, "Potion")

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        pc,
        gba_items_tests.wrong_game_all_pocket_items
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    items_tests.item_list_test_add_remove(
        pc,
        gba_items_tests.all_pocket_items
    )

    local valid_items = pc:get_valid_items()
    luaunit.assertTrue(#valid_items > 0)
end

function gba_items_tests.test_item_bag(bag, game)
    local tmhm_pocket_name = ""
    local berry_pocket_name = ""
    if game == "FireRed" or game == "LeafGreen"
    then
        tmhm_pocket_name = "TM Case"
        berry_pocket_name = "Berry Pouch"
    else
        tmhm_pocket_name = "TMs & HMs"
        berry_pocket_name = "Berries"
    end
    
    -- Check unchanging and initial values.
    luaunit.assertEquals(bag:get_game(), game)

    local pockets = bag:get_pockets()
    luaunit.assertEquals(#pockets, 5)
    luaunit.assertTrue(pockets:has_key("Items"))
    luaunit.assertTrue(pockets:has_key("Key Items"))
    luaunit.assertTrue(pockets:has_key("Poké Balls"))
    luaunit.assertTrue(pockets:has_key(tmhm_pocket_name))
    luaunit.assertTrue(pockets:has_key(berry_pocket_name))

    gba_items_tests.test_item_pocket(bag["Items"], game)
    gba_items_tests.test_key_item_pocket(bag["Key Items"], game)
    gba_items_tests.test_ball_pocket(bag["Poké Balls"], game)
    gba_items_tests.test_tmhm_pocket(bag[tmhm_pocket_name], game)
    gba_items_tests.test_berry_pocket(bag[berry_pocket_name], game)

    -- Make sure adding items through the bag adds to the proper pocket.
    luaunit.assertEquals(bag["Items"]:get_num_items(), 0)
    luaunit.assertEquals(bag["Key Items"]:get_num_items(), 0)
    luaunit.assertEquals(bag["Poké Balls"]:get_num_items(), 0)
    luaunit.assertEquals(bag[tmhm_pocket_name]:get_num_items(), 0)
    luaunit.assertEquals(bag[berry_pocket_name]:get_num_items(), 0)

    for i = 1, #gba_items_tests.all_pocket_items
    do
        bag:add(gba_items_tests.all_pocket_items[i], 5)
    end

    luaunit.assertEquals(bag["Items"][1].item, "Potion")
    luaunit.assertEquals(bag["Items"][1].amount, 5)
    luaunit.assertEquals(bag["Items"][2].item, "None")
    luaunit.assertEquals(bag["Items"][2].amount, 0)

    luaunit.assertEquals(bag["Key Items"][1].item, "Mach Bike")
    luaunit.assertEquals(bag["Key Items"][1].amount, 5)
    luaunit.assertEquals(bag["Key Items"][2].item, "Wailmer Pail")
    luaunit.assertEquals(bag["Key Items"][2].amount, 5)
    luaunit.assertEquals(bag["Key Items"][3].item, "None")
    luaunit.assertEquals(bag["Key Items"][3].amount, 0)

    luaunit.assertEquals(bag["Poké Balls"][1].item, "Great Ball")
    luaunit.assertEquals(bag["Poké Balls"][1].amount, 5)
    luaunit.assertEquals(bag["Poké Balls"][2].item, "Master Ball")
    luaunit.assertEquals(bag["Poké Balls"][2].amount, 5)
    luaunit.assertEquals(bag["Poké Balls"][3].item, "None")
    luaunit.assertEquals(bag["Poké Balls"][3].amount, 0)

    luaunit.assertEquals(bag[tmhm_pocket_name][1].item, "TM01")
    luaunit.assertEquals(bag[tmhm_pocket_name][1].amount, 5)
    luaunit.assertEquals(bag[tmhm_pocket_name][2].item, "HM04")
    luaunit.assertEquals(bag[tmhm_pocket_name][2].amount, 5)
    luaunit.assertEquals(bag[tmhm_pocket_name][3].item, "None")
    luaunit.assertEquals(bag[tmhm_pocket_name][3].amount, 0)

    luaunit.assertEquals(bag[berry_pocket_name][1].item, "Aspear Berry")
    luaunit.assertEquals(bag[berry_pocket_name][1].amount, 5)
    luaunit.assertEquals(bag[berry_pocket_name][2].item, "None")
    luaunit.assertEquals(bag[berry_pocket_name][2].amount, 0)

    -- Make sure removing items through the bag removes from the proper pocket.
    for i = 1, #gba_items_tests.all_pocket_items
    do
        bag:remove(gba_items_tests.all_pocket_items[i], 5)
    end

    luaunit.assertEquals(bag["Items"][1].item, "None")
    luaunit.assertEquals(bag["Items"][1].amount, 0)
    luaunit.assertEquals(bag["Items"][2].item, "None")
    luaunit.assertEquals(bag["Items"][2].amount, 0)

    luaunit.assertEquals(bag["Key Items"][1].item, "None")
    luaunit.assertEquals(bag["Key Items"][1].amount, 0)
    luaunit.assertEquals(bag["Key Items"][2].item, "None")
    luaunit.assertEquals(bag["Key Items"][2].amount, 0)
    luaunit.assertEquals(bag["Key Items"][3].item, "None")
    luaunit.assertEquals(bag["Key Items"][3].amount, 0)

    luaunit.assertEquals(bag["Poké Balls"][1].item, "None")
    luaunit.assertEquals(bag["Poké Balls"][1].amount, 0)
    luaunit.assertEquals(bag["Poké Balls"][2].item, "None")
    luaunit.assertEquals(bag["Poké Balls"][2].amount, 0)
    luaunit.assertEquals(bag["Poké Balls"][3].item, "None")
    luaunit.assertEquals(bag["Poké Balls"][3].amount, 0)

    luaunit.assertEquals(bag[tmhm_pocket_name][1].item, "None")
    luaunit.assertEquals(bag[tmhm_pocket_name][1].amount, 0)
    luaunit.assertEquals(bag[tmhm_pocket_name][2].item, "None")
    luaunit.assertEquals(bag[tmhm_pocket_name][2].amount, 0)
    luaunit.assertEquals(bag[tmhm_pocket_name][3].item, "None")
    luaunit.assertEquals(bag[tmhm_pocket_name][3].amount, 0)

    luaunit.assertEquals(bag[berry_pocket_name][1].item, "None")
    luaunit.assertEquals(bag[berry_pocket_name][1].amount, 0)
    luaunit.assertEquals(bag[berry_pocket_name][2].item, "None")
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
    local item_pocket = pkmn.item_list("Items", "Ruby")
    gba_items_tests.test_item_pocket(item_pocket, "Ruby")
end

function test_ruby_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", "Ruby")
    gba_items_tests.test_key_item_pocket(key_item_pocket, "Ruby")
end

function test_ruby_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", "Ruby")
    gba_items_tests.test_ball_pocket(ball_pocket, "Ruby")
end

function test_ruby_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TMs & HMs", "Ruby")
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, "Ruby")
end

function test_ruby_berry_pocket()
    local berry_pocket = pkmn.item_list("Berries", "Ruby")
    gba_items_tests.test_berry_pocket(berry_pocket, "Ruby")
end

function test_ruby_pc()
    local pc = pkmn.item_list("PC", "Ruby")
    gba_items_tests.test_pc(pc, "Ruby")
end

function test_ruby_item_bag()
    local item_bag = pkmn.item_bag("Ruby")
    gba_items_tests.test_item_bag(item_bag, "Ruby")
end

--
-- Sapphire
--

function test_sapphire_item_pocket()
    local item_pocket = pkmn.item_list("Items", "Sapphire")
    gba_items_tests.test_item_pocket(item_pocket, "Sapphire")
end

function test_sapphire_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", "Sapphire")
    gba_items_tests.test_key_item_pocket(key_item_pocket, "Sapphire")
end

function test_sapphire_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", "Sapphire")
    gba_items_tests.test_ball_pocket(ball_pocket, "Sapphire")
end

function test_sapphire_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TMs & HMs", "Sapphire")
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, "Sapphire")
end

function test_sapphire_berry_pocket()
    local berry_pocket = pkmn.item_list("Berries", "Sapphire")
    gba_items_tests.test_berry_pocket(berry_pocket, "Sapphire")
end

function test_sapphire_pc()
    local pc = pkmn.item_list("PC", "Sapphire")
    gba_items_tests.test_pc(pc, "Sapphire")
end

function test_sapphire_item_bag()
    local item_bag = pkmn.item_bag("Sapphire")
    gba_items_tests.test_item_bag(item_bag, "Sapphire")
end

--
-- Emerald
--

function test_emerald_item_pocket()
    local item_pocket = pkmn.item_list("Items", "Emerald")
    gba_items_tests.test_item_pocket(item_pocket, "Emerald")
end

function test_emerald_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", "Emerald")
    gba_items_tests.test_key_item_pocket(key_item_pocket, "Emerald")
end

function test_emerald_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", "Emerald")
    gba_items_tests.test_ball_pocket(ball_pocket, "Emerald")
end

function test_emerald_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TMs & HMs", "Emerald")
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, "Emerald")
end

function test_emerald_berry_pocket()
    local berry_pocket = pkmn.item_list("Berries", "Emerald")
    gba_items_tests.test_berry_pocket(berry_pocket, "Emerald")
end

function test_emerald_pc()
    local pc = pkmn.item_list("PC", "Emerald")
    gba_items_tests.test_pc(pc, "Emerald")
end

function test_emerald_item_bag()
    local item_bag = pkmn.item_bag("Emerald")
    gba_items_tests.test_item_bag(item_bag, "Emerald")
end

--
-- FireRed
--

function test_firered_item_pocket()
    local item_pocket = pkmn.item_list("Items", "FireRed")
    gba_items_tests.test_item_pocket(item_pocket, "FireRed")
end

function test_firered_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", "FireRed")
    gba_items_tests.test_key_item_pocket(key_item_pocket, "FireRed")
end

function test_firered_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", "FireRed")
    gba_items_tests.test_ball_pocket(ball_pocket, "FireRed")
end

function test_firered_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TM Case", "FireRed")
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, "FireRed")
end

function test_firered_berry_pocket()
    local berry_pocket = pkmn.item_list("Berry Pouch", "FireRed")
    gba_items_tests.test_berry_pocket(berry_pocket, "FireRed")
end

function test_firered_pc()
    local pc = pkmn.item_list("PC", "FireRed")
    gba_items_tests.test_pc(pc, "FireRed")
end

function test_firered_item_bag()
    local item_bag = pkmn.item_bag("FireRed")
    gba_items_tests.test_item_bag(item_bag, "FireRed")
end

--
-- LeafGreen
--

function test_leafgreen_item_pocket()
    local item_pocket = pkmn.item_list("Items", "LeafGreen")
    gba_items_tests.test_item_pocket(item_pocket, "LeafGreen")
end

function test_leafgreen_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", "LeafGreen")
    gba_items_tests.test_key_item_pocket(key_item_pocket, "LeafGreen")
end

function test_leafgreen_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", "LeafGreen")
    gba_items_tests.test_ball_pocket(ball_pocket, "LeafGreen")
end

function test_leafgreen_tmhm_pocket()
    local tmhm_pocket = pkmn.item_list("TM Case", "LeafGreen")
    gba_items_tests.test_tmhm_pocket(tmhm_pocket, "LeafGreen")
end

function test_leafgreen_berry_pocket()
    local berry_pocket = pkmn.item_list("Berry Pouch", "LeafGreen")
    gba_items_tests.test_berry_pocket(berry_pocket, "LeafGreen")
end

function test_leafgreen_pc()
    local pc = pkmn.item_list("PC", "LeafGreen")
    gba_items_tests.test_pc(pc, "LeafGreen")
end

function test_leafgreen_item_bag()
    local item_bag = pkmn.item_bag("LeafGreen")
    gba_items_tests.test_item_bag(item_bag, "LeafGreen")
end
