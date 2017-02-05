--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local bit32 = require("bit32")
local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pokemon_tests = require("pokemon_tests")

local gen1_pokemon_tests = {}

function gen1_pokemon_tests.friendship_test(game)
    local pikachu = pkmn.pokemon("Pikachu", game, "", 5)

    if game == "Yellow"
    then
        pikachu:set_friendship(123)
        luaunit.assertEquals(pikachu:get_friendship(), 123)

        -- Also check a non-Pikachu.
        local mewtwo = pkmn.pokemon("Mewtwo", game, "", 70)
        luaunit.assertError(mewtwo.set_friendship, mewtwo, 123)
        luaunit.assertError(mewtwo.get_friendship, mewtwo)
    else
        luaunit.assertError(pikachu.set_friendship, pikachu, 123)
        luaunit.assertError(pikachu.get_friendship, pikachu)
    end
end

function gen1_pokemon_tests.check_stat_map(stat_map)
    luaunit.assertTrue(stat_map:has_key("HP"))
    luaunit.assertTrue(stat_map:has_key("Attack"))
    luaunit.assertTrue(stat_map:has_key("Defense"))
    luaunit.assertTrue(stat_map:has_key("Speed"))
    luaunit.assertTrue(stat_map:has_key("Special"))
    luaunit.assertFalse(stat_map:has_key("Special Attack"))
    luaunit.assertFalse(stat_map:has_key("Special Defense"))
end

function gen1_pokemon_tests.pokemon_test(game)
    local species = "Charmander"
    local pokemon = pkmn.pokemon(species, game, "", 30)

    --
    -- Check starting values, and confirm that we can't query values
    -- that didn't exist in Generation I.
    --
    luaunit.assertEquals(pokemon:get_species(), species)
    luaunit.assertEquals(pokemon:get_form(), "Standard")
    luaunit.assertEquals(pokemon:get_game(), game)
    luaunit.assertEquals(pokemon:get_nickname(), string.upper(species))

    luaunit.assertError(pokemon.get_gender, pokemon)
    luaunit.assertError(pokemon.is_shiny, pokemon)
    luaunit.assertError(pokemon.get_held_item, pokemon)

    luaunit.assertEquals(pokemon:get_trainer_name(), pkmn.LIBPKMN_OT_NAME)
    luaunit.assertEquals(pokemon:get_trainer_public_id(), bit32.band(pkmn.LIBPKMN_OT_ID, 0xFFFF))

    luaunit.assertError(pokemon.get_trainer_secret_id, pokemon)

    luaunit.assertEquals(pokemon:get_trainer_id(), bit32.band(pkmn.LIBPKMN_OT_ID, 0xFFFF))
    luaunit.assertEquals(pokemon:get_trainer_gender(), "Male")

    luaunit.assertError(pokemon.get_friendship, pokemon)
    luaunit.assertError(pokemon.get_ability, pokemon)
    luaunit.assertError(pokemon.get_ball, pokemon)
    luaunit.assertError(pokemon.get_level_met, pokemon)
    luaunit.assertError(pokemon.get_location_met, pokemon, true)
    luaunit.assertError(pokemon.get_location_met, pokemon, false)
    luaunit.assertError(pokemon.get_original_game, pokemon)
    luaunit.assertError(pokemon.get_personality, pokemon)

    luaunit.assertEquals(pokemon:get_experience(), pokemon:get_database_entry():get_experience_at_level(30))
    luaunit.assertEquals(pokemon:get_level(), 30)

    luaunit.assertError(pokemon.get_markings, pokemon)
    luaunit.assertError(pokemon.get_ribbons, pokemon)
    luaunit.assertError(pokemon.get_contest_stats, pokemon)

    local move_slots = pokemon:get_moves()
    luaunit.assertEquals(#move_slots, 4)
    for i = 1, #move_slots
    do
        luaunit.assertEquals(move_slots[i].move:get_name(), "None")
        luaunit.assertEquals(move_slots[i].pp, 0)
    end

    gen1_pokemon_tests.check_stat_map(pokemon:get_EVs())
    gen1_pokemon_tests.check_stat_map(pokemon:get_IVs())
    gen1_pokemon_tests.check_stat_map(pokemon:get_stats())

    --
    -- Make sure the getters and setters agree. Also make sure it fails when
    -- expected.
    --
    luaunit.assertError(pokemon.set_nickname, pokemon, "")
    luaunit.assertError(pokemon.set_nickname, pokemon, "Too long nickname")

    pokemon:set_nickname("foobarbaz")
    luaunit.assertEquals(pokemon:get_nickname(), "foobarbaz")

    luaunit.assertError(pokemon.set_shininess, pokemon, true)
    luaunit.assertError(pokemon.set_shininess, pokemon, false)
    luaunit.assertError(pokemon.set_held_item, pokemon, "Potion")

    luaunit.assertError(pokemon.set_trainer_name, pokemon, "")
    luaunit.assertError(pokemon.set_trainer_name, pokemon, "Too long trainer name")

    pokemon:set_trainer_name("foobar")
    luaunit.assertEquals(pokemon:get_trainer_name(), "foobar")

    pokemon:set_trainer_id(10001)
    luaunit.assertEquals(pokemon:get_trainer_id(), 10001)
    luaunit.assertEquals(pokemon:get_trainer_public_id(), 10001)
    luaunit.assertError(pokemon.get_trainer_secret_id, pokemon)

    luaunit.assertError(pokemon.set_trainer_secret_id, 54321)
    luaunit.assertEquals(pokemon:get_trainer_id(), 10001)

    -- Make sure Lua+SWIG catches invalid values.
    luaunit.assertError(pokemon.set_trainer_id, pokemon, -1)
    luaunit.assertError(pokemon.set_trainer_id, pokemon, 0xFFFF+1)
    luaunit.assertError(pokemon.set_trainer_public_id, pokemon, -1)
    luaunit.assertError(pokemon.set_trainer_public_id, pokemon, 0xFFFF+1)

    luaunit.assertError(pokemon.set_trainer_gender, pokemon, "Male")
    luaunit.assertError(pokemon.set_friendship, pokemon, 123)
    luaunit.assertError(pokemon.set_ability, pokemon, "")
    luaunit.assertError(pokemon.set_ball, pokemon, "Great Ball")
    luaunit.assertError(pokemon.set_location_met, pokemon, "Victory Road", true)
    luaunit.assertError(pokemon.set_location_met, pokemon, "Victory Road", false)
    luaunit.assertError(pokemon.set_original_game, pokemon, "Blue")
    luaunit.assertError(pokemon.set_marking, pokemon, "Circle", true)
    luaunit.assertError(pokemon.set_ribbon, pokemon, "Cool", false)

    pokemon:set_experience(12345)
    luaunit.assertEquals(pokemon:get_experience(), 12345)

    pokemon:set_level(45)
    luaunit.assertEquals(pokemon:get_level(), 45)

    luaunit.assertError(pokemon.set_move, "Ember", 0)
    luaunit.assertError(pokemon.set_move, "Synthesis", 1)
    luaunit.assertEquals(pokemon:get_moves()[1].move:get_name(), "None")

    local move_names = {"Ember", "Flamethrower", "Slash", "Fire Blast"}
    for i = 1, #move_names
    do
        pokemon:set_move(move_names[i], i)
    end

    move_slots = pokemon:get_moves()
    for i = 1, #move_names
    do
        luaunit.assertEquals(move_slots[i].move:get_name(), move_names[i])
        luaunit.assertEquals(move_slots[i].pp, move_slots[i].move:get_pp(0))
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

-- Red

function test_gen1_red_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Red")
end

function test_gen1_red_friendship()
    gen1_pokemon_tests.friendship_test("Red")
end

function test_gen1_red_pokemon()
    gen1_pokemon_tests.pokemon_test("Red")
end

-- Blue

function test_gen1_blue_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Blue")
end

function test_gen1_blue_friendship()
    gen1_pokemon_tests.friendship_test("Blue")
end

function test_gen1_blue_pokemon()
    gen1_pokemon_tests.pokemon_test("Blue")
end

-- Yellow

function test_gen1_yellow_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Yellow")
end

function test_gen1_yellow_friendship()
    gen1_pokemon_tests.friendship_test("Yellow")
end

function test_gen1_yellow_pokemon()
    gen1_pokemon_tests.pokemon_test("Yellow")
end
