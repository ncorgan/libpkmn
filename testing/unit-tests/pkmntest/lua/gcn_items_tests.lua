--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local items_tests = require("items_tests")

local gcn_items_tests = {}

gcn_items_tests.colosseum_all_pocket_items =
{
    pkmn.item.POTION, pkmn.item.EIN_FILE_S,
    pkmn.item.GREAT_BALL, pkmn.item.TM01,
    pkmn.item.TM02, pkmn.item.ASPEAR_BERRY,
    pkmn.item.JOY_SCENT, pkmn.item.VIVID_SCENT
}
gcn_items_tests.xd_all_pocket_items =
{
    pkmn.item.POTION, pkmn.item.GONZAPS_KEY,
    pkmn.item.GREAT_BALL, pkmn.item.TM01,
    pkmn.item.TM02, pkmn.item.ASPEAR_BERRY,
    pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01
}
gcn_items_tests.colosseum_wrong_game_all_pocket_items =
{
    pkmn.item.PINK_BOW, pkmn.item.BLACK_SLUDGE,
    pkmn.item.GS_BALL, pkmn.item.GONZAPS_KEY, pkmn.item.POFFIN_CASE,
    pkmn.item.TM51, pkmn.item.HM01,
    pkmn.item.BERRY, pkmn.item.OCCA_BERRY
}
gcn_items_tests.xd_wrong_game_all_pocket_items =
{
    pkmn.item.PINK_BOW, pkmn.item.BLACK_SLUDGE,
    pkmn.item.GS_BALL, pkmn.item.EIN_FILE_S, pkmn.item.POFFIN_CASE,
    pkmn.item.TM51, pkmn.item.HM01,
    pkmn.item.BERRY, pkmn.item.OCCA_BERRY
}

function gcn_items_tests.test_item_pocket(item_pocket, game)
    local expected_capacity = 0
    if game == pkmn.game.COLOSSEUM
    then
        expected_capacity = 20
    else
        expected_capacity = 30
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
    local wrong_pocket_items = {}
    if game == pkmn.game.COLOSSEUM
    then
        wrong_pocket_items =
        {
            pkmn.item.EIN_FILE_S, pkmn.item.GREAT_BALL,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY,
            pkmn.item.JOY_SCENT
        }
    else
        wrong_pocket_items =
        {
            pkmn.item.MIROR_RADAR, pkmn.item.GREAT_BALL,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY,
            pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01
        }
    end
    items_tests.item_list_test_invalid_items(
        item_pocket, wrong_pocket_items
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        item_pocket,
        {pkmn.item.PINK_BOW, pkmn.item.BLACK_SLUDGE,
         pkmn.item.BINDING_BAND, pkmn.item.BEEDRILLITE}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local items =
    {
        pkmn.item.POTION, pkmn.item.ORANGE_MAIL,
        pkmn.item.LAVA_COOKIE, pkmn.item.STARDUST,
        pkmn.item.SHADOW_MAIL, pkmn.item.PINK_SCARF,
        pkmn.item.ANTIDOTE, pkmn.item.GREEN_SHARD
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

function gcn_items_tests.test_key_item_pocket(key_item_pocket, game)
    local expected_capacity = 43
    local item_name = ""
    if game == pkmn.game.COLOSSEUM
    then
        item_name = pkmn.item.EIN_FILE_S
    else
        item_name = pkmn.item.MIROR_RADAR
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(#key_item_pocket, expected_capacity)
    luaunit.assertEquals(key_item_pocket.name, "Key Items")
    luaunit.assertEquals(key_item_pocket.game, game)
    luaunit.assertEquals(key_item_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(key_item_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(key_item_pocket, item_name)

    -- Make sure we can't add items from other pockets.
    local wrong_pocket_items = {}
    if game == pkmn.game.COLOSSEUM
    then
        wrong_pocket_items =
        {
            pkmn.item.POTION, pkmn.item.GREAT_BALL,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY, pkmn.item.JOY_SCENT
        }
    else
        wrong_pocket_items =
        {
            pkmn.item.POTION, pkmn.item.GREAT_BALL,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY,
            pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01
        }
    end
    items_tests.item_list_test_invalid_items(
        key_item_pocket, wrong_pocket_items
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        key_item_pocket,
        {pkmn.item.GS_BALL, pkmn.item.POFFIN_CASE,
         pkmn.item.DNA_SPLICERS, pkmn.item.AQUA_SUIT}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local items = {}
    if game == pkmn.game.COLOSSEUM
    then
        items =
        {
            pkmn.item.JAIL_KEY, pkmn.item.ELEVATOR_KEY,
            pkmn.item.SMALL_TABLET, pkmn.item.F_DISK,
            pkmn.item.R_DISK, pkmn.item.L_DISK,
            pkmn.item.D_DISK, pkmn.item.U_DISK
        }
    else
        items =
        {
            pkmn.item.KRANE_MEMO_1, pkmn.item.KRANE_MEMO_2,
            pkmn.item.KRANE_MEMO_3, pkmn.item.KRANE_MEMO_4,
            pkmn.item.KRANE_MEMO_5, pkmn.item.VOICE_CASE_1,
            pkmn.item.VOICE_CASE_2, pkmn.item.VOICE_CASE_3
        }
    end
    items_tests.item_list_test_setting_items(
        key_item_pocket,
        items
    )
    items_tests.item_list_test_add_remove(
        key_item_pocket,
        items
    )

    luaunit.assertTrue(#key_item_pocket.valid_items > 0)
    luaunit.assertEquals(
        #key_item_pocket.valid_items,
        #key_item_pocket.valid_item_names
    )
end

function gcn_items_tests.test_ball_pocket(ball_pocket, game)
    local expected_capacity = 16

    -- Check unchanging and initial values.
    luaunit.assertEquals(#ball_pocket, expected_capacity)
    luaunit.assertEquals(ball_pocket.name, "Poké Balls")
    luaunit.assertEquals(ball_pocket.game, game)
    luaunit.assertEquals(ball_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(ball_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(ball_pocket, pkmn.item.GREAT_BALL)

    -- Make sure we can't add items from other pockets.
    local wrong_pocket_items = {}
    if game == pkmn.game.COLOSSEUM
    then
        wrong_pocket_items =
        {
            pkmn.item.EIN_FILE_S, pkmn.item.POTION,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY, pkmn.item.JOY_SCENT
        }
    else
        wrong_pocket_items =
        {
            pkmn.item.MIROR_RADAR, pkmn.item.POTION,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY,
            pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01
        }
    end
    items_tests.item_list_test_invalid_items(
        ball_pocket, wrong_pocket_items
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        ball_pocket,
        {pkmn.item.MOON_BALL, pkmn.item.HEAL_BALL, pkmn.item.DREAM_BALL}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local items =
    {
        pkmn.item.MASTER_BALL, pkmn.item.ULTRA_BALL,
        pkmn.item.GREAT_BALL, pkmn.item.POKE_BALL,
        pkmn.item.SAFARI_BALL, pkmn.item.NET_BALL,
        pkmn.item.DIVE_BALL, pkmn.item.NEST_BALL
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

function gcn_items_tests.test_tm_pocket(tm_pocket, game)
    local expected_capacity = 64

    -- Check unchanging and initial values.
    luaunit.assertEquals(#tm_pocket, expected_capacity)
    luaunit.assertEquals(tm_pocket.name, "TMs")
    luaunit.assertEquals(tm_pocket.game, game)
    luaunit.assertEquals(tm_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(tm_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(tm_pocket, pkmn.item.TM01)

    -- Make sure we can't add items from other pockets.
    local wrong_pocket_items = {}
    if game == pkmn.game.COLOSSEUM
    then
        wrong_pocket_items =
        {
            pkmn.item.EIN_FILE_S, pkmn.item.POTION,
            pkmn.item.GREAT_BALL, pkmn.item.ORAN_BERRY, pkmn.item.JOY_SCENT
        }
    else
        wrong_pocket_items =
        {
            pkmn.item.MIROR_RADAR, pkmn.item.POTION,
            pkmn.item.GREAT_BALL, pkmn.item.ORAN_BERRY,
            pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01
        }
    end
    items_tests.item_list_test_invalid_items(
        tm_pocket, wrong_pocket_items
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        tm_pocket,
        {pkmn.item.TM51, pkmn.item.HM01}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local items =
    {
        pkmn.item.TM01, pkmn.item.TM02, pkmn.item.TM03, pkmn.item.TM04,
        pkmn.item.TM05, pkmn.item.TM06, pkmn.item.TM07, pkmn.item.TM08
    }

    items_tests.item_list_test_setting_items(
        tm_pocket,
        items
    )
    items_tests.item_list_test_add_remove(
        tm_pocket,
        items
    )

    luaunit.assertTrue(#tm_pocket.valid_items > 0)
    luaunit.assertEquals(
        #tm_pocket.valid_items,
        #tm_pocket.valid_item_names
    )
end

function gcn_items_tests.test_berry_pocket(berry_pocket, game)
    local expected_capacity = 46

    -- Check unchanging and initial values.
    luaunit.assertEquals(#berry_pocket, expected_capacity)
    luaunit.assertEquals(berry_pocket.name, "Berries")
    luaunit.assertEquals(berry_pocket.game, game)
    luaunit.assertEquals(berry_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(berry_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(berry_pocket, pkmn.item.ORAN_BERRY)

    -- Make sure we can't add items from other pockets.
    local wrong_pocket_items = {}
    if game == pkmn.game.COLOSSEUM
    then
        wrong_pocket_items =
        {
            pkmn.item.EIN_FILE_S, pkmn.item.POTION,
            pkmn.item.GREAT_BALL, pkmn.item.TM01, pkmn.item.JOY_SCENT
        }
    else
        wrong_pocket_items =
        {
            pkmn.item.MIROR_RADAR, pkmn.item.POTION,
            pkmn.item.GREAT_BALL, pkmn.item.TM01,
            pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01
        }
    end
    items_tests.item_list_test_invalid_items(
        berry_pocket, wrong_pocket_items
    )

    -- Make sure we can't add items from other generations.
    items_tests.item_list_test_invalid_items(
        berry_pocket,
        {pkmn.item.BERRY, pkmn.item.OCCA_BERRY, pkmn.item.ROSELI_BERRY}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local items =
    {
        pkmn.item.CHERI_BERRY, pkmn.item.RAZZ_BERRY,
        pkmn.item.LUM_BERRY, pkmn.item.PINAP_BERRY,
        pkmn.item.ASPEAR_BERRY, pkmn.item.IAPAPA_BERRY,
        pkmn.item.WIKI_BERRY, pkmn.item.APICOT_BERRY
    }

    items_tests.item_list_test_setting_items(
        berry_pocket,
        items
    )
    items_tests.item_list_test_add_remove(
        berry_pocket,
        items
    )

    luaunit.assertTrue(#berry_pocket.valid_items > 0)
    luaunit.assertEquals(
        #berry_pocket.valid_items,
        #berry_pocket.valid_item_names
    )
end

function gcn_items_tests.test_cologne_pocket(cologne_pocket, game)
    local expected_capacity = 3

    -- Check unchanging and initial values.
    luaunit.assertEquals(#cologne_pocket, expected_capacity)
    luaunit.assertEquals(cologne_pocket.name, "Colognes")
    luaunit.assertEquals(cologne_pocket.game, game)
    luaunit.assertEquals(cologne_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(cologne_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(cologne_pocket, pkmn.item.JOY_SCENT)

    -- Make sure we can't add items from other pockets.
    local wrong_pocket_items = {}
    if game == pkmn.game.COLOSSEUM
    then
        wrong_pocket_items =
        {
            pkmn.item.EIN_FILE_S, pkmn.item.POTION,
            pkmn.item.GREAT_BALL, pkmn.item.TM01, pkmn.item.ORAN_BERRY
        }
    else
        wrong_pocket_items =
        {
            pkmn.item.MIROR_RADAR, pkmn.item.POTION,
            pkmn.item.GREAT_BALL, pkmn.item.TM01,
            pkmn.item.ORAN_BERRY, pkmn.item.BATTLE_CD_01
        }
    end
    items_tests.item_list_test_invalid_items(
        cologne_pocket, wrong_pocket_items
    )

    -- Since this pocket can only have 3 items, we can't use our typical function, which requires 8.
    cologne_pocket:add(pkmn.item.JOY_SCENT, 3)
    cologne_pocket:add(pkmn.item.VIVID_SCENT, 3)
    cologne_pocket:add(pkmn.item.VIVID_SCENT, 3)
    cologne_pocket:remove(pkmn.item.VIVID_SCENT, 3)
    cologne_pocket:remove(pkmn.item.VIVID_SCENT, 1)

    luaunit.assertEquals(cologne_pocket[1].item, pkmn.item.JOY_SCENT)
    luaunit.assertEquals(cologne_pocket[1].amount, 3)
    luaunit.assertEquals(cologne_pocket[2].item, pkmn.item.VIVID_SCENT)
    luaunit.assertEquals(cologne_pocket[2].amount, 2)
    luaunit.assertEquals(cologne_pocket[3].item, pkmn.item.NONE)
    luaunit.assertEquals(cologne_pocket[3].amount, 0)

    cologne_pocket:remove(pkmn.item.JOY_SCENT, 3)
    cologne_pocket:remove(pkmn.item.VIVID_SCENT, 2)
    luaunit.assertEquals(cologne_pocket[1].item, pkmn.item.NONE)
    luaunit.assertEquals(cologne_pocket[1].amount, 0)
    luaunit.assertEquals(cologne_pocket[2].item, pkmn.item.NONE)
    luaunit.assertEquals(cologne_pocket[2].amount, 0)
    luaunit.assertEquals(cologne_pocket[3].item, pkmn.item.NONE)
    luaunit.assertEquals(cologne_pocket[3].amount, 0)

    luaunit.assertTrue(#cologne_pocket.valid_items > 0)
    luaunit.assertEquals(
        #cologne_pocket.valid_items,
        #cologne_pocket.valid_item_names
    )
end

function gcn_items_tests.test_battle_cd_pocket(battle_cd_pocket, game)
    local expected_capacity = 60

    -- Check unchanging and initial values.
    luaunit.assertEquals(#battle_cd_pocket, expected_capacity)
    luaunit.assertEquals(battle_cd_pocket.name, "Battle CDs")
    luaunit.assertEquals(battle_cd_pocket.game, game)
    luaunit.assertEquals(battle_cd_pocket.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(battle_cd_pocket)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(battle_cd_pocket, pkmn.item.TM01)

    -- Make sure we can't add items from other pockets.
    items_tests.item_list_test_invalid_items(
        battle_cd_pocket,
        {pkmn.item.MIROR_RADAR, pkmn.item.POTION,
         pkmn.item.GREAT_BALL, pkmn.item.ORAN_BERRY, pkmn.item.JOY_SCENT}
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local items =
    {
        pkmn.item.BATTLE_CD_01, pkmn.item.BATTLE_CD_02,
        pkmn.item.BATTLE_CD_03, pkmn.item.BATTLE_CD_04,
        pkmn.item.BATTLE_CD_05, pkmn.item.BATTLE_CD_06,
        pkmn.item.BATTLE_CD_07, pkmn.item.BATTLE_CD_08
    }

    items_tests.item_list_test_setting_items(
        battle_cd_pocket,
        items
    )
    items_tests.item_list_test_add_remove(
        battle_cd_pocket,
        items
    )

    luaunit.assertEquals(#battle_cd_pocket.valid_items, 60)
    luaunit.assertEquals(#battle_cd_pocket.valid_item_names, 60)
end

function gcn_items_tests.test_pc(pc, game)
    -- Check unchanging and initial values.
    luaunit.assertEquals(#pc, 235)
    luaunit.assertEquals(pc.name, "PC")
    luaunit.assertEquals(pc.game, game)
    luaunit.assertEquals(pc.num_items, 0)

    -- Make sure item slots start as correctly empty.
    items_tests.item_list_test_empty_slot(pc)

    -- Confirm errors are thrown when expected.
    items_tests.item_list_test_out_of_range_error(pc, pkmn.item.POTION)

    -- Make sure we can't add items from other generations.
    local wrong_game_all_pocket_items = {}
    if game == pkmn.game.COLOSSEUM
    then
        wrong_game_all_pocket_items = gcn_items_tests.colosseum_wrong_game_all_pocket_items
    else
        wrong_game_all_pocket_items = gcn_items_tests.xd_wrong_game_all_pocket_items
    end
    items_tests.item_list_test_invalid_items(
        pc,
        wrong_game_all_pocket_items
    )

    -- Start adding and removing stuff, and make sure the numbers are accurate.
    local all_pocket_items = {}
    if game == pkmn.game.COLOSSEUM
    then
        all_pocket_items = gcn_items_tests.colosseum_all_pocket_items
    else
        all_pocket_items = gcn_items_tests.xd_all_pocket_items
    end
    items_tests.item_list_test_add_remove(
        pc,
        all_pocket_items
    )

    luaunit.assertTrue(#pc.valid_items > 0)
    luaunit.assertEquals(
        #pc.valid_items,
        #pc.valid_item_names
    )
end

function gcn_items_tests.test_bag(bag, game)
    local num_pockets = ""
    if game == pkmn.game.COLOSSEUM
    then
        num_pockets = 6
    else
        num_pockets = 7
    end

    -- Check unchanging and initial values.
    luaunit.assertEquals(bag.game, game)
    luaunit.assertEquals(#bag, num_pockets)

    gcn_items_tests.test_item_pocket(bag["Items"], game)
    gcn_items_tests.test_key_item_pocket(bag["Key Items"], game)
    gcn_items_tests.test_ball_pocket(bag["Poké Balls"], game)
    gcn_items_tests.test_tm_pocket(bag["TMs"], game)
    gcn_items_tests.test_berry_pocket(bag["Berries"], game)
    gcn_items_tests.test_cologne_pocket(bag["Colognes"], game)
    if game == pkmn.game.XD
    then
        gcn_items_tests.test_battle_cd_pocket(bag["Battle CDs"], game)
    end

    -- Make sure adding items through the bag adds to the proper pocket.
    luaunit.assertEquals(bag["Items"].num_items, 0)
    luaunit.assertEquals(bag["Key Items"].num_items, 0)
    luaunit.assertEquals(bag["Poké Balls"].num_items, 0)
    luaunit.assertEquals(bag["TMs"].num_items, 0)
    luaunit.assertEquals(bag["Berries"].num_items, 0)
    luaunit.assertEquals(bag["Colognes"].num_items, 0)
    if game == pkmn.game.XD
    then
        luaunit.assertEquals(bag["Battle CDs"].num_items, 0)
    end

    local all_pocket_items = {}
    local wrong_game_all_pocket_items = {}
    local key_item = ""
    if game == pkmn.game.COLOSSEUM
    then
        all_pocket_items = gcn_items_tests.colosseum_all_pocket_items
        wrong_game_all_pocket_items = gcn_items_tests.colosseum_wrong_game_all_pocket_items
        key_item = pkmn.item.EIN_FILE_S
    else
        all_pocket_items = gcn_items_tests.xd_all_pocket_items
        wrong_game_all_pocket_items = gcn_items_tests.xd_wrong_game_all_pocket_items
        key_item = pkmn.item.GONZAPS_KEY
    end

    -- Make sure we can't add items from other generations.
    items_tests.item_bag_test_invalid_items(
        bag,
        wrong_game_all_pocket_items
    )

    for i = 1, #all_pocket_items
    do
        bag:add(all_pocket_items[i], 5)
    end

    luaunit.assertEquals(bag["Items"][1].item, pkmn.item.POTION)
    luaunit.assertEquals(bag["Items"][1].amount, 5)
    luaunit.assertEquals(bag["Items"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][2].amount, 0)

    luaunit.assertEquals(bag["Key Items"][1].item, key_item)
    luaunit.assertEquals(bag["Key Items"][1].amount, 5)
    luaunit.assertEquals(bag["Key Items"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Key Items"][2].amount, 0)

    luaunit.assertEquals(bag["Poké Balls"][1].item, pkmn.item.GREAT_BALL)
    luaunit.assertEquals(bag["Poké Balls"][1].amount, 5)
    luaunit.assertEquals(bag["Poké Balls"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Poké Balls"][2].amount, 0)

    luaunit.assertEquals(bag["TMs"][1].item, pkmn.item.TM01)
    luaunit.assertEquals(bag["TMs"][1].amount, 5)
    luaunit.assertEquals(bag["TMs"][2].item, pkmn.item.TM02)
    luaunit.assertEquals(bag["TMs"][2].amount, 5)
    luaunit.assertEquals(bag["TMs"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["TMs"][3].amount, 0)

    luaunit.assertEquals(bag["Berries"][1].item, pkmn.item.ASPEAR_BERRY)
    luaunit.assertEquals(bag["Berries"][1].amount, 5)
    luaunit.assertEquals(bag["Berries"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Berries"][2].amount, 0)

    luaunit.assertEquals(bag["Colognes"][1].item, pkmn.item.JOY_SCENT)
    luaunit.assertEquals(bag["Colognes"][1].amount, 5)
    if game == pkmn.game.COLOSSEUM
    then
        luaunit.assertEquals(bag["Colognes"][2].item, pkmn.item.VIVID_SCENT)
        luaunit.assertEquals(bag["Colognes"][2].amount, 5)
        luaunit.assertEquals(bag["Colognes"][3].item, pkmn.item.NONE)
        luaunit.assertEquals(bag["Colognes"][3].amount, 0)
    else
        luaunit.assertEquals(bag["Colognes"][2].item, pkmn.item.NONE)
        luaunit.assertEquals(bag["Colognes"][2].amount, 0)

        luaunit.assertEquals(bag["Battle CDs"][1].item, pkmn.item.BATTLE_CD_01)
        luaunit.assertEquals(bag["Battle CDs"][1].amount, 5)
        luaunit.assertEquals(bag["Battle CDs"][2].item, pkmn.item.NONE)
        luaunit.assertEquals(bag["Battle CDs"][2].amount, 0)
    end

    -- Make sure removing items through the bag removes from the proper pocket.
    for i = 1, #all_pocket_items
    do
        bag:remove(all_pocket_items[i], 5)
    end

    luaunit.assertEquals(bag["Items"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][1].amount, 0)
    luaunit.assertEquals(bag["Items"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Items"][2].amount, 0)

    luaunit.assertEquals(bag["Key Items"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Key Items"][1].amount, 0)
    luaunit.assertEquals(bag["Key Items"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Key Items"][2].amount, 0)

    luaunit.assertEquals(bag["Poké Balls"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Poké Balls"][1].amount, 0)
    luaunit.assertEquals(bag["Poké Balls"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Poké Balls"][2].amount, 0)

    luaunit.assertEquals(bag["TMs"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["TMs"][1].amount, 0)
    luaunit.assertEquals(bag["TMs"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["TMs"][2].amount, 0)
    luaunit.assertEquals(bag["TMs"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["TMs"][3].amount, 0)

    luaunit.assertEquals(bag["Berries"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Berries"][1].amount, 0)
    luaunit.assertEquals(bag["Berries"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Berries"][2].amount, 0)

    luaunit.assertEquals(bag["Colognes"][1].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Colognes"][1].amount, 0)
    luaunit.assertEquals(bag["Colognes"][2].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Colognes"][2].amount, 0)
    luaunit.assertEquals(bag["Colognes"][3].item, pkmn.item.NONE)
    luaunit.assertEquals(bag["Colognes"][3].amount, 0)
    if game == pkmn.game.XD
    then
        luaunit.assertEquals(bag["Colognes"][2].item, pkmn.item.NONE)
        luaunit.assertEquals(bag["Colognes"][2].amount, 0)

        luaunit.assertEquals(bag["Battle CDs"][1].item, pkmn.item.NONE)
        luaunit.assertEquals(bag["Battle CDs"][1].amount, 0)
        luaunit.assertEquals(bag["Battle CDs"][2].item, pkmn.item.NONE)
        luaunit.assertEquals(bag["Battle CDs"][2].amount, 0)
    end
end

--
-- Colosseum
--

function test_colosseum_item_pocket()
    local item_pocket = pkmn.item_list("Items", pkmn.game.COLOSSEUM)
    gcn_items_tests.test_item_pocket(item_pocket, pkmn.game.COLOSSEUM)
end

function test_colosseum_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", pkmn.game.COLOSSEUM)
    gcn_items_tests.test_key_item_pocket(key_item_pocket, pkmn.game.COLOSSEUM)
end

function test_colosseum_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", pkmn.game.COLOSSEUM)
    gcn_items_tests.test_ball_pocket(ball_pocket, pkmn.game.COLOSSEUM)
end

function test_colosseum_tm_pocket()
    local tm_pocket = pkmn.item_list("TMs", pkmn.game.COLOSSEUM)
    gcn_items_tests.test_tm_pocket(tm_pocket, pkmn.game.COLOSSEUM)
end

function test_colosseum_berry_pocket()
    local berry_pocket = pkmn.item_list("Berries", pkmn.game.COLOSSEUM)
    gcn_items_tests.test_berry_pocket(berry_pocket, pkmn.game.COLOSSEUM)
end

function test_colosseum_cologne_pocket()
    local cologne_pocket = pkmn.item_list("Colognes", pkmn.game.COLOSSEUM)
    gcn_items_tests.test_cologne_pocket(cologne_pocket, pkmn.game.COLOSSEUM)
end

function test_colosseum_pc()
    local pc = pkmn.item_list("PC", pkmn.game.COLOSSEUM)
    gcn_items_tests.test_pc(pc, pkmn.game.COLOSSEUM)
end

function test_colosseum_bag()
    local bag = pkmn.item_bag(pkmn.game.COLOSSEUM)
    gcn_items_tests.test_bag(bag, pkmn.game.COLOSSEUM)
end

--
-- XD
--

function test_xd_item_pocket()
    local item_pocket = pkmn.item_list("Items", pkmn.game.XD)
    gcn_items_tests.test_item_pocket(item_pocket, pkmn.game.XD)
end

function test_xd_key_item_pocket()
    local key_item_pocket = pkmn.item_list("Key Items", pkmn.game.XD)
    gcn_items_tests.test_key_item_pocket(key_item_pocket, pkmn.game.XD)
end

function test_xd_ball_pocket()
    local ball_pocket = pkmn.item_list("Poké Balls", pkmn.game.XD)
    gcn_items_tests.test_ball_pocket(ball_pocket, pkmn.game.XD)
end

function test_xd_tm_pocket()
    local tm_pocket = pkmn.item_list("TMs", pkmn.game.XD)
    gcn_items_tests.test_tm_pocket(tm_pocket, pkmn.game.XD)
end

function test_xd_berry_pocket()
    local berry_pocket = pkmn.item_list("Berries", pkmn.game.XD)
    gcn_items_tests.test_berry_pocket(berry_pocket, pkmn.game.XD)
end

function test_xd_cologne_pocket()
    local cologne_pocket = pkmn.item_list("Colognes", pkmn.game.XD)
    gcn_items_tests.test_cologne_pocket(cologne_pocket, pkmn.game.XD)
end

function test_xd_battle_cd_pocket()
    local battle_cd_pocket = pkmn.item_list("Battle CDs", pkmn.game.XD)
    gcn_items_tests.test_battle_cd_pocket(battle_cd_pocket, pkmn.game.XD)
end

function test_xd_pc()
    local pc = pkmn.item_list("PC", pkmn.game.XD)
    gcn_items_tests.test_pc(pc, pkmn.game.XD)
end

function test_xd_bag()
    local bag = pkmn.item_bag(pkmn.game.XD)
    gcn_items_tests.test_bag(bag, pkmn.game.XD)
end
