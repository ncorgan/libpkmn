--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local pkmntest_utils = require("pkmntest_utils")

local luaunit = require("luaunit")

local LIBPKMN_TEST_FILES = pkmn.paths.getenv("LIBPKMN_TEST_FILES")
local TMP_DIR = pkmn.paths.get_tmp_dir()

local pokemon_io_tests = {}

-- Generation I

function pokemon_io_tests.test_loading_and_saving_pk1(game)
    local pk1_path = pkmntest_utils.concat_path(
                         TMP_DIR,
                         string.format("%s_%d.pk1", game, math.random(0, 0xFFFF))
                     )

    local item_list = pkmn.database.get_item_list(game)
    local move_list = pkmn.database.get_move_list(game)
    local pokemon_list = pkmn.database.get_pokemon_list(1, true)

    local random_pokemon = pkmntest_utils.get_random_pokemon(game, pokemon_list, move_list, item_list)
    random_pokemon:export_to_file(pk1_path)

    local imported_pokemon = pkmn.pokemon(pk1_path)
    pkmntest_utils.compare_pokemon(imported_pokemon, random_pokemon)

    os.remove(pk1_path)
end

function test_red_loading_and_saving_pk1()
    pokemon_io_tests.test_loading_and_saving_pk1(pkmn.game.RED)
end

function test_blue_loading_and_saving_pk1()
    pokemon_io_tests.test_loading_and_saving_pk1(pkmn.game.BLUE)
end

function test_yellow_loading_and_saving_pk1()
    pokemon_io_tests.test_loading_and_saving_pk1(pkmn.game.YELLOW)
end

-- Generation II

function pokemon_io_tests.test_loading_and_saving_pk2(game)
    local pk2_path = pkmntest_utils.concat_path(
                         TMP_DIR,
                         string.format("%s_%d.pk2", game, math.random(0, 0xFFFF))
                     )

    local item_list = pkmn.database.get_item_list(game)
    local move_list = pkmn.database.get_move_list(game)
    local pokemon_list = pkmn.database.get_pokemon_list(2, true)

    local random_pokemon = pkmntest_utils.get_random_pokemon(game, pokemon_list, move_list, item_list)
    random_pokemon:export_to_file(pk2_path)

    local imported_pokemon = pkmn.pokemon(pk2_path)
    pkmntest_utils.compare_pokemon(imported_pokemon, random_pokemon)

    os.remove(pk2_path)
end

function test_gold_loading_and_saving_pk2()
    pokemon_io_tests.test_loading_and_saving_pk2(pkmn.game.GOLD)
end

function test_silver_loading_and_saving_pk2()
    pokemon_io_tests.test_loading_and_saving_pk2(pkmn.game.SILVER)
end

function test_crystal_loading_and_saving_pk2()
    pokemon_io_tests.test_loading_and_saving_pk2(pkmn.game.CRYSTAL)
end

-- Generation III

function pokemon_io_tests.test_loading_and_saving_3gpkm(game)
    local _3gpkm_path = pkmntest_utils.concat_path(
                            TMP_DIR,
                            string.format("%s_%d.3gpkm", game, math.random(0, 0xFFFF))
                        )

    local item_list = pkmn.database.get_item_list(game)
    local move_list = pkmn.database.get_move_list(game)
    local pokemon_list = pkmn.database.get_pokemon_list(3, true)

    local random_pokemon = pkmntest_utils.get_random_pokemon(game, pokemon_list, move_list, item_list)
    random_pokemon:export_to_file(_3gpkm_path)

    local imported_pokemon = pkmn.pokemon(_3gpkm_path)
    pkmntest_utils.compare_pokemon(imported_pokemon, random_pokemon)

    os.remove(_3gpkm_path)
end

function test_ruby_loading_and_saving_3gpkm()
    pokemon_io_tests.test_loading_and_saving_3gpkm(pkmn.game.RUBY)
end

function test_sapphire_loading_and_saving_3gpkm()
    pokemon_io_tests.test_loading_and_saving_3gpkm(pkmn.game.SAPPHIRE)
end

function test_emerald_loading_and_saving_3gpkm()
    pokemon_io_tests.test_loading_and_saving_3gpkm(pkmn.game.EMERALD)
end

function test_firered_loading_and_saving_3gpkm()
    pokemon_io_tests.test_loading_and_saving_3gpkm(pkmn.game.FIRERED)
end

function test_leafgreen_loading_and_saving_3gpkm()
    pokemon_io_tests.test_loading_and_saving_3gpkm(pkmn.game.LEAFGREEN)
end

function test_outside_3gpkm()
    local CONTEST_LEVELS = {"Super", "Hyper", "Master"}
    local RIBBONS = {"Champion", "Winning", "Victory", "Artist",
                     "Effort", "Marine", "Land", "Sky",
                     "Country", "National", "Earth", "World"}
    --
    -- Test files in repo and compare to known values.
    --
    local _3gpkm_dir = pkmntest_utils.concat_path(LIBPKMN_TEST_FILES, "3gpkm")

    local mightyena = pkmn.pokemon(pkmntest_utils.concat_path(_3gpkm_dir, "MIGHTYENA.3gpkm"))
    luaunit.assertEquals(mightyena.species, pkmn.species.MIGHTYENA)
    luaunit.assertEquals(mightyena.game, pkmn.game.EMERALD)
    luaunit.assertEquals(mightyena.form, "Standard")
    luaunit.assertEquals(mightyena.nickname, "MIGHTYENA")
    luaunit.assertFalse(mightyena.is_shiny)
    luaunit.assertEquals(mightyena.held_item, pkmn.item.HEART_SCALE)
    luaunit.assertEquals(mightyena.original_trainer_name, "A")
    luaunit.assertEquals(mightyena.original_trainer_public_id, 61415)
    luaunit.assertEquals(mightyena.original_trainer_secret_id, 3417)
    luaunit.assertEquals(mightyena.original_trainer_id, 223997927)
    luaunit.assertEquals(mightyena.original_trainer_gender, pkmn.gender.FEMALE)
    luaunit.assertEquals(mightyena.current_trainer_friendship, 254)
    luaunit.assertEquals(mightyena.ability, pkmn.ability.INTIMIDATE)
    luaunit.assertEquals(mightyena.ball, pkmn.item.GREAT_BALL)
    luaunit.assertEquals(mightyena.level_met, 25)
    luaunit.assertEquals(mightyena.location_met, "Route 120")
    luaunit.assertEquals(mightyena.original_game, pkmn.game.EMERALD)
    luaunit.assertEquals(mightyena.personality, 3557601241)
    luaunit.assertEquals(mightyena.experience, 128734)
    luaunit.assertEquals(mightyena.level, 50)

    luaunit.assertEquals(#mightyena.markings, 4)
    for i = 1, #mightyena.markings.keys
    do
        luaunit.assertFalse(mightyena.markings[mightyena.markings.keys[i]])
    end

    luaunit.assertEquals(#mightyena.ribbons, 32)
    -- Contest ribbons
    for contest_type_index = 1, #mightyena.contest_stats.keys
    do
        if mightyena.contest_stats.keys[contest_type_index] ~= pkmn.contest_stat.FEEL
        then
            local contest_stat_name = pkmn.contest_stat_to_string(mightyena.contest_stats.keys[contest_type_index])

            luaunit.assertFalse(mightyena.ribbons[contest_stat_name])
            for contest_level_index = 1, #CONTEST_LEVELS
            do
                local ribbon_name = contest_stat_name .. " " .. CONTEST_LEVELS[contest_level_index]
                luaunit.assertFalse(mightyena.ribbons[ribbon_name])
            end
        end
    end
    -- Other ribbons
    for ribbon_index = 1, #RIBBONS
    do
        if RIBBONS[ribbon_index] == "Champion"
        then
            luaunit.assertTrue(mightyena.ribbons[RIBBONS[ribbon_index]])
        else
            luaunit.assertFalse(mightyena.ribbons[RIBBONS[ribbon_index]])
        end
    end

    luaunit.assertEquals(#mightyena.contest_stats, 6)
    for contest_type_index = 1, #mightyena.contest_stats.keys
    do
        luaunit.assertEquals(mightyena.contest_stats[mightyena.contest_stats.keys[contest_type_index]], 0)
    end

    local expected_mightyena_moves =
    {
        pkmn.move.CRUNCH,
        pkmn.move.STRENGTH,
        pkmn.move.SHADOW_BALL,
        pkmn.move.DOUBLE_EDGE
    }
    luaunit.assertEquals(#mightyena.moves, 4)
    for move_index = 1, 4
    do
        luaunit.assertEquals(mightyena.moves[move_index].move, expected_mightyena_moves[move_index])
    end

    luaunit.assertEquals(#mightyena.EVs, 6)
    luaunit.assertEquals(mightyena.EVs[pkmn.stat.HP], 30)
    luaunit.assertEquals(mightyena.EVs[pkmn.stat.ATTACK], 110)
    luaunit.assertEquals(mightyena.EVs[pkmn.stat.DEFENSE], 32)
    luaunit.assertEquals(mightyena.EVs[pkmn.stat.SPEED], 48)
    luaunit.assertEquals(mightyena.EVs[pkmn.stat.SPECIAL_ATTACK], 17)
    luaunit.assertEquals(mightyena.EVs[pkmn.stat.SPECIAL_DEFENSE], 83)

    luaunit.assertEquals(#mightyena.IVs, 6)
    luaunit.assertEquals(mightyena.IVs[pkmn.stat.HP], 26)
    luaunit.assertEquals(mightyena.IVs[pkmn.stat.ATTACK], 28)
    luaunit.assertEquals(mightyena.IVs[pkmn.stat.DEFENSE], 4)
    luaunit.assertEquals(mightyena.IVs[pkmn.stat.SPEED], 13)
    luaunit.assertEquals(mightyena.IVs[pkmn.stat.SPECIAL_ATTACK], 25)
    luaunit.assertEquals(mightyena.IVs[pkmn.stat.SPECIAL_DEFENSE], 26)

    luaunit.assertEquals(#mightyena.stats, 6)
    luaunit.assertEquals(mightyena.stats[pkmn.stat.HP], 146)
    luaunit.assertEquals(mightyena.stats[pkmn.stat.ATTACK], 122)
    luaunit.assertEquals(mightyena.stats[pkmn.stat.DEFENSE], 81)
    luaunit.assertEquals(mightyena.stats[pkmn.stat.SPEED], 87)
    luaunit.assertEquals(mightyena.stats[pkmn.stat.SPECIAL_ATTACK], 79)
    luaunit.assertEquals(mightyena.stats[pkmn.stat.SPECIAL_DEFENSE], 88)
end
