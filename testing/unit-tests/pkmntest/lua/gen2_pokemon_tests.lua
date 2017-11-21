--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pokemon_tests = require("pokemon_tests")

local gen2_pokemon_tests = {}

function gen2_pokemon_tests.unown_forms_test(game)
    local letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    for i = 1, #letters
    do
        local unown = pkmn.pokemon("Unown", game, letters:sub(i,i), 5)
        luaunit.assertEquals(unown:get_form(), letters:sub(i,i))

        -- Make sure IVs are properly set.
        local IVs = unown:get_IVs()
        local form_from_IVs = pkmn.calculations.gen2_unown_form(
                                  IVs["Attack"],
                                  IVs["Defense"],
                                  IVs["Speed"],
                                  IVs["Special"]
                              )
        luaunit.assertEquals(unown:get_form(), form_from_IVs)
    end

    -- Make sure setting IVs properly changes the form.
    local unown = pkmn.pokemon("Unown", game, "A", 5)
    unown:set_IV("Attack", 10)
    unown:set_IV("Defense", 9)
    unown:set_IV("Speed", 1)
    unown:set_IV("Special", 14)
    luaunit.assertEquals(unown:get_form(), "G")

    -- Make sure setting the form properly changes IVs.
    for i = 1, #letters
    do
        unown:set_form(letters:sub(i,i))
        luaunit.assertEquals(unown:get_form(), letters:sub(i,i))

        -- Make sure IVs are properly set.
        local IVs = unown:get_IVs()
        local form_from_IVs = pkmn.calculations.gen2_unown_form(
                                  IVs["Attack"],
                                  IVs["Defense"],
                                  IVs["Speed"],
                                  IVs["Special"]
                              )
        luaunit.assertEquals(unown:get_form(), form_from_IVs)
    end
end

function gen2_pokemon_tests.check_stat_map(stat_map, is_stats)
    luaunit.assertTrue(stat_map:has_key("HP"))
    luaunit.assertTrue(stat_map:has_key("Attack"))
    luaunit.assertTrue(stat_map:has_key("Defense"))
    luaunit.assertTrue(stat_map:has_key("Speed"))
    if is_stats
    then
        luaunit.assertFalse(stat_map:has_key("Special"))
        luaunit.assertTrue(stat_map:has_key("Special Attack"))
        luaunit.assertTrue(stat_map:has_key("Special Defense"))
    else
        luaunit.assertTrue(stat_map:has_key("Special"))
        luaunit.assertFalse(stat_map:has_key("Special Attack"))
        luaunit.assertFalse(stat_map:has_key("Special Defense"))
    end
end

function gen2_pokemon_tests.pokemon_test(game)
    local species = "Cyndaquil"
    local pokemon = pkmn.pokemon(species, game, "", 30)

    --
    -- Check starting values, and confirm that we can't query values
    -- that didn't exist in Generation II.
    --
    luaunit.assertEquals(pokemon:get_species(), species)
    luaunit.assertEquals(pokemon:get_form(), "Standard")
    luaunit.assertEquals(pokemon:get_game(), game)
    luaunit.assertEquals(pokemon:get_nickname(), string.upper(species))
    luaunit.assertEquals(pokemon:get_trainer_name(), pkmn.DEFAULT_TRAINER_NAME)
    luaunit.assertEquals(pokemon:get_trainer_public_id(), bit32.band(pkmn.DEFAULT_TRAINER_ID, 0xFFFF))

    luaunit.assertError(pokemon.get_trainer_secret_id, pokemon)

    luaunit.assertEquals(pokemon:get_trainer_id(), bit32.band(pkmn.DEFAULT_TRAINER_ID, 0xFFFF))
    luaunit.assertEquals(pokemon:get_trainer_gender(), "Male")
    luaunit.assertEquals(pokemon:get_friendship(), pokemon:get_database_entry():get_base_friendship())

    luaunit.assertEquals(pokemon:get_level_met(), pokemon:get_level())
    luaunit.assertEquals(pokemon:get_level(), 30)

    luaunit.assertError(pokemon.get_location_met, pokemon, true)

    luaunit.assertEquals(pokemon:get_location_met(false), "Special")

    luaunit.assertError(pokemon.get_original_game, pokemon)
    luaunit.assertError(pokemon.get_personality, pokemon)

    luaunit.assertEquals(
        pokemon:get_experience(),
        pokemon:get_database_entry():get_experience_at_level(30)
    )

    luaunit.assertError(pokemon.get_markings, pokemon)
    luaunit.assertError(pokemon.get_ribbons, pokemon)
    luaunit.assertError(pokemon.get_contest_stats, pokemon)

    local move_slots = pokemon:get_moves()
    luaunit.assertEquals(#move_slots, 4)

    for i = 1, #move_slots
    do
        luaunit.assertEquals(move_slots[i].move, "None")
        luaunit.assertEquals(move_slots[i].pp, 0)
    end

    gen2_pokemon_tests.check_stat_map(pokemon:get_EVs(), false)
    gen2_pokemon_tests.check_stat_map(pokemon:get_IVs(), false)
    gen2_pokemon_tests.check_stat_map(pokemon:get_stats(), true)

    luaunit.assertTrue(pokemon_tests.file_exists(pokemon:get_icon_filepath()))
    luaunit.assertTrue(pokemon_tests.file_exists(pokemon:get_sprite_filepath()))

    --
    -- Make sure the getters and setters agree. Also make sure it fails
    -- when expected.
    --
    luaunit.assertError(pokemon.set_nickname, pokemon, "")
    luaunit.assertError(pokemon.set_nickname, pokemon, "Too long nickname")

    pokemon:set_nickname("foobarbaz")
    luaunit.assertEquals(pokemon:get_nickname(), "foobarbaz")

    -- Gender affects IVs, so make sure the abstraction reflects that.
    pokemon:set_gender("Male")
    luaunit.assertEquals(pokemon:get_IVs()["Attack"], 15)
    pokemon:set_gender("Female")
    luaunit.assertTrue(pokemon:get_IVs()["Attack"] < 15)

    pokemon:set_IV("Attack", 0)
    luaunit.assertEquals(pokemon:get_gender(), "Female")
    pokemon:set_IV("Attack", 15)
    luaunit.assertEquals(pokemon:get_gender(), "Male")

    -- Shininess affects IVs, so make sure the abstraction reflects that. Also check filepaths.
    pokemon:set_shininess(false)
    luaunit.assertFalse(pokemon:is_shiny())
    luaunit.assertEquals(pokemon:get_IVs()["Attack"], 13)
    luaunit.assertTrue(pokemon_tests.file_exists(pokemon:get_sprite_filepath()))

    -- This will fail if "shiny" is anywhere in the filepath.
    luaunit.assertEquals(string.find(pokemon:get_sprite_filepath(), "shiny"), nil)

    pokemon:set_shininess(true)
    luaunit.assertTrue(pokemon:is_shiny())
    luaunit.assertTrue(pokemon_tests.file_exists(pokemon:get_sprite_filepath()))
    luaunit.assertNotEquals(string.find(pokemon:get_sprite_filepath(), "shiny"), nil)

    local IVs = pokemon:get_IVs()
    luaunit.assertEquals(IVs["Attack"], 15)
    luaunit.assertEquals(IVs["Defense"], 10)
    luaunit.assertEquals(IVs["Speed"], 10)
    luaunit.assertEquals(IVs["Special"], 10)

    luaunit.assertError(pokemon.set_held_item, pokemon, "Not an item")
    luaunit.assertEquals(pokemon:get_held_item(), "None")

    -- Valid item, not holdable
    luaunit.assertError(pokemon.set_held_item, pokemon, "Bicycle")
    luaunit.assertEquals(pokemon:get_held_item(), "None")

    pokemon:set_held_item("Berry")
    luaunit.assertEquals(pokemon:get_held_item(), "Berry")

    luaunit.assertError(pokemon.set_trainer_name, pokemon, "")
    luaunit.assertError(pokemon.set_trainer_name, pokemon, "Too long trainer name")

    pokemon:set_trainer_name("foobar")
    luaunit.assertEquals(pokemon:get_trainer_name(), "foobar")

    pokemon:set_trainer_id(10001)
    luaunit.assertEquals(pokemon:get_trainer_id(), 10001)
    luaunit.assertEquals(pokemon:get_trainer_public_id(), 10001)

    luaunit.assertError(pokemon.set_trainer_secret_id, pokemon, 54321)
    luaunit.assertEquals(pokemon:get_trainer_id(), 10001)

    -- Make sure Lua+SWIG catches invalid values.
    luaunit.assertError(pokemon.set_trainer_id, pokemon, -1)
    luaunit.assertError(pokemon.set_trainer_id, pokemon, 0xFFFF+1)
    luaunit.assertError(pokemon.set_trainer_public_id, pokemon, -1)
    luaunit.assertError(pokemon.set_trainer_public_id, pokemon, 0xFFFF+1)

    luaunit.assertError(pokemon.set_ability, pokemon, "")
    luaunit.assertError(pokemon.set_ball, pokemon, "Great Ball")
    luaunit.assertError(pokemon.set_location_met, pokemon, "Pallet Town", true)

    pokemon:set_location_met("Pallet Town", false)
    luaunit.assertEquals(pokemon:get_location_met(false), "Pallet Town")

    pokemon:set_trainer_gender("Female")
    luaunit.assertEquals(pokemon:get_trainer_gender(), "Female")
    luaunit.assertError(pokemon.set_trainer_gender, pokemon, "Genderless")

    pokemon:set_level_met(3)
    luaunit.assertEquals(pokemon:get_level_met(), 3)

    luaunit.assertError(pokemon.set_friendship, pokemon, -1)
    luaunit.assertError(pokemon.set_friendship, pokemon, 256)

    pokemon:set_friendship(123)
    luaunit.assertEquals(pokemon:get_friendship(), 123)

    luaunit.assertError(pokemon.set_original_game, pokemon, "Gold")
    luaunit.assertError(pokemon.set_marking, pokemon, "Circle", true)
    luaunit.assertError(pokemon.set_ribbon, pokemon, "Cool", false)

    pokemon:set_experience(12345)
    luaunit.assertEquals(pokemon:get_experience(), 12345)

    pokemon:set_level(45)
    luaunit.assertEquals(pokemon:get_level(), 45)

    luaunit.assertError(pokemon.set_move, pokemon, "Ember", 0)
    luaunit.assertError(pokemon.set_move, pokemon, "Bounce", 1)
    luaunit.assertEquals(pokemon:get_moves()[1].move, "None")

    local move_names = {"Ember", "Flamethrower", "Slash", "Fire Blast"}
    for i = 1, #move_names
    do
        pokemon:set_move(move_names[i], i)
    end

    move_slots = pokemon:get_moves()
    for i = 1, #move_slots
    do
        luaunit.assertEquals(move_slots[i].move, move_names[i])
        luaunit.assertEquals(
            move_slots[i].pp,
            pkmn.database.move_entry(move_slots[i].move, pokemon:get_game()):get_pp(0)
        )
    end

    luaunit.assertError(pokemon.set_EV, pokemon, "Not a stat", 1)
    luaunit.assertError(pokemon.set_EV, pokemon, "Attack", -1)
    luaunit.assertError(pokemon.set_EV, pokemon, "Attack", 65536)

    pokemon:set_EV("Attack", 12345)
    luaunit.assertEquals(pokemon:get_EVs()["Attack"], 12345)

    luaunit.assertError(pokemon.set_IV, pokemon, "Not a stat", 1)
    luaunit.assertError(pokemon.set_IV, pokemon, "Attack", -1)
    luaunit.assertError(pokemon.set_IV, pokemon, "Attack", 16)

    pokemon:set_IV("Attack", 12)
    luaunit.assertEquals(pokemon:get_IVs()["Attack"], 12)
end

-- Gold

function test_gen2_gold_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Gold")
end

function test_gen2_gold_gender()
    pokemon_tests.gender_test("Gold")
end

function test_gen2_gold_unown_forms()
    gen2_pokemon_tests.unown_forms_test("Gold")
end

function test_gen2_gold_pokemon()
    gen2_pokemon_tests.pokemon_test("Gold")
end

-- Silver

function test_gen2_silver_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Silver")
end

function test_gen2_silver_gender()
    pokemon_tests.gender_test("Silver")
end

function test_gen2_silver_unown_forms()
    gen2_pokemon_tests.unown_forms_test("Silver")
end

function test_gen2_silver_pokemon()
    gen2_pokemon_tests.pokemon_test("Silver")
end

-- Crystal

function test_gen2_crystal_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Crystal")
end

function test_gen2_crystal_gender()
    pokemon_tests.gender_test("Crystal")
end

function test_gen2_crystal_unown_forms()
    gen2_pokemon_tests.unown_forms_test("Crystal")
end

function test_gen2_crystal_pokemon()
    gen2_pokemon_tests.pokemon_test("Crystal")
end
