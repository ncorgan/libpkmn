--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")
local pokemon_tests = require("pokemon_tests")

local gba_pokemon_tests = {}

gba_pokemon_tests.MARKINGS = {"Circle", "Triangle", "Square", "Heart"}
gba_pokemon_tests.CONTEST_TYPES = {"Cool", "Beauty", "Cute", "Smart", "Tough"}
gba_pokemon_tests.CONTEST_LEVELS = {"", " Super", " Hyper", " Master"}
gba_pokemon_tests.RIBBONS = {"Champion", "Winning", "Victory", "Artist",
                             "Effort", "Marine", "Land", "Sky",
                             "Country", "National", "Earth", "World"}
gba_pokemon_tests.STATS = {"HP", "Attack", "Defense", "Speed",
                           "Special Attack", "Special Defense"}

function gba_pokemon_tests.unown_form_test(game)
    local unown_forms = pkmn.database.pokemon_entry("Unown", "Omega Ruby", ""):get_forms()
    for i = 1, #unown_forms
    do
        local unown = pkmn.pokemon("Unown", game, unown_forms[i], 5)
        luaunit.assertEquals(unown:get_form(), unown_forms[i])
    end

    -- Make sure setting the personality properly sets the form.
    local unown = pkmn.pokemon("Unown", game, "A", 5)
    unown:set_personality(0x4C07DE71)
    luaunit.assertEquals(unown:get_form(), "B")

    for i = 1, #unown_forms
    do
        unown:set_form(unown_forms[i])
        luaunit.assertEquals(unown:get_form(), unown_forms[i])
        local form_from_personality = pkmn.calculations.gen3_unown_form(
                                          unown:get_personality()
                                      )
        luaunit.assertEquals(form_from_personality, unown_forms[i])
    end
end

function gba_pokemon_tests.check_markings_map(markings_map)
    luaunit.assertEquals(#markings_map, 4)

    for i = 1, #gba_pokemon_tests.MARKINGS
    do
        luaunit.assertTrue(markings_map:has_key(gba_pokemon_tests.MARKINGS[i]))
        luaunit.assertFalse(markings_map[gba_pokemon_tests.MARKINGS[i]])
    end

    luaunit.assertFalse(markings_map:has_key("Star"))
    luaunit.assertFalse(markings_map:has_key("Diamond"))
end

function gba_pokemon_tests.check_ribbons_map(ribbons_map)
    luaunit.assertEquals(#ribbons_map, 32)

    -- Check contest ribbons.
    for i = 1, #gba_pokemon_tests.CONTEST_TYPES
    do
        for j = 1, #gba_pokemon_tests.CONTEST_LEVELS
        do
            local ribbon_name = string.format(
                                    "%s%s",
                                    gba_pokemon_tests.CONTEST_TYPES[i],
                                    gba_pokemon_tests.CONTEST_LEVELS[j]
                                )
            luaunit.assertTrue(ribbons_map:has_key(ribbon_name))
            luaunit.assertFalse(ribbons_map[ribbon_name])
        end
    end

    -- Check other ribbons.
    for i = 1, #gba_pokemon_tests.RIBBONS
    do
        luaunit.assertTrue(ribbons_map:has_key(gba_pokemon_tests.RIBBONS[i]))
        luaunit.assertFalse(ribbons_map[gba_pokemon_tests.RIBBONS[i]])
    end
end

function gba_pokemon_tests.check_contest_stats_map(contest_stats_map)
    luaunit.assertEquals(#contest_stats_map, 6)

    for i = 1, #gba_pokemon_tests.CONTEST_TYPES
    do
        luaunit.assertTrue(contest_stats_map:has_key(gba_pokemon_tests.CONTEST_TYPES[i]))
        luaunit.assertEquals(contest_stats_map[gba_pokemon_tests.CONTEST_TYPES[i]], 0)
    end

    luaunit.assertTrue(contest_stats_map:has_key("Feel"))
    luaunit.assertTrue(contest_stats_map["Feel"])
    luaunit.assertFalse(contest_stats_map:has_key("Sheen"))
end

function gba_pokemon_tests.check_stats_map(stats_map)
    local stats = {"HP", "Attack", "Defense", "Speed", "Special Attack", "Special Defense"}
    for i = 1, #stats
    do
        luaunit.assertTrue(stats_map:has_key(stats[i]))
    end

    luaunit.assertFalse(stats_map:has_key("Special"))
end

function gba_pokemon_tests.markings_test(pokemon)
    for i = 1, #gba_pokemon_tests.MARKINGS
    do
        luaunit.assertFalse(pokemon:get_markings()[gba_pokemon_tests.MARKINGS[i]])
        pokemon:set_marking(gba_pokemon_tests.MARKINGS[i], true)
        luaunit.assertTrue(pokemon:get_markings()[gba_pokemon_tests.MARKINGS[i]])
    end
end

function gba_pokemon_tests.ribbons_test(pokemon)
    -- Check contest ribbons.
    for i = 1, #gba_pokemon_tests.CONTEST_TYPES
    do
        for j = 1, #gba_pokemon_tests.CONTEST_LEVELS
        do
            local ribbon_name = string.format(
                                    "%s%s",
                                    gba_pokemon_tests.CONTEST_TYPES[i],
                                    gba_pokemon_tests.CONTEST_LEVELS[j]
                                )
            luaunit.assertFalse(pokemon:get_ribbons()[ribbon_name])
            pokemon:set_ribbon(ribbon_name, true)
            luaunit.assertTrue(pokemon:get_ribbons()[ribbon_name])
        end
    end

    -- Check other ribbons.
    for i = 1, #gba_pokemon_tests.RIBBONS
    do
        luaunit.assertFalse(pokemon:get_ribbons()[gba_pokemon_tests.RIBBONS[i]])
        pokemon:set_ribbon(gba_pokemon_tests.RIBBONS[i], true)
        luaunit.assertTrue(pokemon:get_ribbons()[gba_pokemon_tests.RIBBONS[i]])
    end
end

function gba_pokemon_tests.contest_stats_test(pokemon)
    for i = 1, #gba_pokemon_tests.CONTEST_TYPES
    do
        luaunit.assertEquals(pokemon:get_contest_stats()[gba_pokemon_tests.CONTEST_TYPES[i]], 0)
        local value = math.random(0, 255)
        pokemon:set_contest_stat(gba_pokemon_tests.CONTEST_TYPES[i], value)
        luaunit.assertEquals(pokemon:get_contest_stats()[gba_pokemon_tests.CONTEST_TYPES[i]], value)
    end
end

function gba_pokemon_tests.EVs_test(pokemon)
    for i = 1, #gba_pokemon_tests.STATS
    do
        local value = math.random(0, 255)
        pokemon:set_EV(gba_pokemon_tests.STATS[i], value)
        luaunit.assertEquals(pokemon:get_EVs()[gba_pokemon_tests.STATS[i]], value)
    end
end

function gba_pokemon_tests.IVs_test(pokemon)
    for i = 1, #gba_pokemon_tests.STATS
    do
        local value = math.random(0, 31)
        pokemon:set_IV(gba_pokemon_tests.STATS[i], value)
        luaunit.assertEquals(pokemon:get_IVs()[gba_pokemon_tests.STATS[i]], value)
    end
end

function gba_pokemon_tests.pokemon_test(game)
    local species = "Torchic"
    local pokemon = pkmn.pokemon(species, game, "", 30)

    --
    -- Check known starting values, and confirm that we can't query values
    -- that didn't exist in Generation III.
    --
    luaunit.assertEquals(pokemon:get_species(), species)
    luaunit.assertEquals(pokemon:get_form(), "Standard")
    luaunit.assertEquals(pokemon:get_game(), game)
    luaunit.assertEquals(pokemon:get_nickname(), string.upper(species))
    luaunit.assertEquals(pokemon:get_held_item():get_name(), "None")
    luaunit.assertEquals(pokemon:get_trainer_name(), pkmn.LIBPKMN_OT_NAME)
    luaunit.assertEquals(pokemon:get_trainer_public_id(), bit32.band(pkmn.LIBPKMN_OT_ID, 0xFFFF))

    luaunit.assertEquals(
        pokemon:get_trainer_secret_id(),
        bit32.rshift(bit32.band(pkmn.LIBPKMN_OT_ID, 0xFFFF0000), 16)
    )

    luaunit.assertEquals(pokemon:get_trainer_id(), pkmn.LIBPKMN_OT_ID)
    luaunit.assertEquals(pokemon:get_trainer_gender(), "Male")
    luaunit.assertEquals(pokemon:get_friendship(), pokemon:get_database_entry():get_base_friendship())
    luaunit.assertEquals(pokemon:get_ability(), "Blaze")
    luaunit.assertEquals(pokemon:get_ball(), "Premier Ball")
    luaunit.assertEquals(pokemon:get_level_met(), pokemon:get_level())

    luaunit.assertError(pokemon.get_location_met, pokemon, true)

    luaunit.assertEquals(pokemon:get_location_met(false), "Fateful encounter")
    luaunit.assertEquals(pokemon:get_original_game(), pokemon:get_game())

    luaunit.assertEquals(
        pokemon:get_experience(),
        pokemon:get_database_entry():get_experience_at_level(30)
    )
    luaunit.assertEquals(pokemon:get_level(), 30)

    gba_pokemon_tests.check_markings_map(pokemon:get_markings())
    gba_pokemon_tests.check_ribbons_map(pokemon:get_ribbons())
    gba_pokemon_tests.check_contest_stats_map(pokemon:get_contest_stats())

    local move_slots = pokemon:get_moves()
    luaunit.assertEquals(#move_slots, 4)
    for i = 1, #move_slots
    do
        luaunit.assertEquals(move_slots[i].move:get_name(), "None")
        luaunit.assertEquals(move_slots[i].pp, 0)
    end

    gba_pokemon_tests.check_stats_map(pokemon:get_EVs())
    gba_pokemon_tests.check_stats_map(pokemon:get_IVs())
    gba_pokemon_tests.check_stats_map(pokemon:get_stats())

    --
    -- Make sure the getters and setters agree. Also make sure it fails when
    -- expected.
    --

    luaunit.assertError(pokemon.set_nickname, pokemon, "")
    luaunit.assertError(pokemon.set_nickname, pokemon, "Too long nickname")

    pokemon:set_nickname("foobarbaz")
    luaunit.assertEquals(pokemon:get_nickname(), "foobarbaz")

    -- Setting shininess should affect personality.
    pokemon:set_shininess(false)
    luaunit.assertFalse(pokemon:is_shiny())
    local personality = pokemon:get_personality()
    pokemon:set_shininess(true)
    luaunit.assertTrue(pokemon:is_shiny())
    luaunit.assertNotEquals(pokemon:get_personality(), personality)

    luaunit.assertError(pokemon.set_held_item, pokemon, "Not an item")

    -- Not in this game
    luaunit.assertError(pokemon.set_held_item, pokemon, "Berry")

    -- Not holdable
    luaunit.assertError(pokemon.set_held_item, pokemon, "Mach Bike")

    pokemon:set_held_item("Razz Berry")
    luaunit.assertEquals(pokemon:get_held_item():get_name(), "Razz Berry")

    luaunit.assertError(pokemon.set_trainer_name, pokemon, "")
    luaunit.assertError(pokemon.set_trainer_name, pokemon, "Too long trainer name")

    pokemon:set_trainer_name("foobar")
    luaunit.assertEquals(pokemon:get_trainer_name(), "foobar")

    pokemon:set_trainer_id(0x1234ABCD)
    luaunit.assertEquals(pokemon:get_trainer_id(), 0x1234ABCD)
    luaunit.assertEquals(pokemon:get_trainer_public_id(), 0xABCD)
    luaunit.assertEquals(pokemon:get_trainer_secret_id(), 0x1234)

    pokemon:set_trainer_public_id(0x1A2B)
    luaunit.assertEquals(pokemon:get_trainer_id(), 0x12341A2B)
    luaunit.assertEquals(pokemon:get_trainer_public_id(), 0x1A2B)
    luaunit.assertEquals(pokemon:get_trainer_secret_id(), 0x1234)

    pokemon:set_trainer_secret_id(0x3C4D)
    luaunit.assertEquals(pokemon:get_trainer_id(), 0x3C4D1A2B)
    luaunit.assertEquals(pokemon:get_trainer_public_id(), 0x1A2B)
    luaunit.assertEquals(pokemon:get_trainer_secret_id(), 0x3C4D)

    -- Make sure Lua+SWIG catches invalid values.
    luaunit.assertError(pokemon.set_trainer_id, pokemon, -1)
    luaunit.assertError(pokemon.set_trainer_id, pokemon, 0xFFFFFFFF+1)
    luaunit.assertError(pokemon.set_trainer_public_id, pokemon, -1)
    luaunit.assertError(pokemon.set_trainer_public_id, pokemon, 0xFFFF+1)
    luaunit.assertError(pokemon.set_trainer_secret_id, pokemon, -1)
    luaunit.assertError(pokemon.set_trainer_secret_id, pokemon, 0xFFFF+1)

    pokemon:set_trainer_gender("Male")
    luaunit.assertEquals(pokemon:get_trainer_gender(), "Male")
    pokemon:set_trainer_gender("Female")
    luaunit.assertEquals(pokemon:get_trainer_gender(), "Female")
    luaunit.assertError(pokemon.set_trainer_error, pokemon, "Genderless")

    pokemon:set_friendship(123)
    luaunit.assertEquals(pokemon:get_friendship(), 123)
    luaunit.assertError(pokemon.set_friendship, pokemon, -1)
    luaunit.assertError(pokemon.set_friendship, pokemon, 256)

    pokemon:set_ability("Blaze")
    luaunit.assertEquals(pokemon:get_ability(), "Blaze")
    luaunit.assertError(pokemon.set_ability, pokemon, "None")
    luaunit.assertError(pokemon.set_ability, pokemon, "Torrent") -- Invalid
    luaunit.assertError(pokemon.set_ability, pokemon, "Speed Boost") -- Hidden ability
    luaunit.assertEquals(pokemon:get_ability(), "Blaze")

    pokemon:set_ball("Great Ball")
    luaunit.assertEquals(pokemon:get_ball(), "Great Ball")
    luaunit.assertError(pokemon.set_ball, pokemon, "Friend Ball") -- Not in Generation III
    luaunit.assertEquals(pokemon:get_ball(), "Great Ball")

    pokemon:set_level_met(67)
    luaunit.assertEquals(pokemon:get_level_met(), 67)
    luaunit.assertError(pokemon.get_level_met, pokemon, -1)
    luaunit.assertError(pokemon.get_level_met, pokemon, 101)

    local location = ""
    if game == "FireRed" or game == "LeafGreen"
    then
        location = "Viridian Forest"
    else
        location = "Petalburg Woods"
    end
    pokemon:set_location_met(location, false)
    luaunit.assertEquals(pokemon:get_location_met(false), location)
    luaunit.assertError(pokemon.set_location_met, location, true)

    pokemon:set_personality(0x7F3AB3A8)
    luaunit.assertEquals(pokemon:get_personality(), 0x7F3AB3A8)

    -- Make sure Lua+SWIG catches invalid values.
    luaunit.assertError(pokemon.set_personality, pokemon, -1)
    luaunit.assertError(pokemon.set_personality, pokemon, 0xFFFFFFFF+1)

    luaunit.assertError(pokemon.set_contest_stat, pokemon, "Cool", -1)
    luaunit.assertError(pokemon.set_contest_stat, pokemon, "Cool", 256)
    luaunit.assertError(pokemon.set_contest_stat, pokemon, "Not a stat", -1)

    gba_pokemon_tests.markings_test(pokemon)
    gba_pokemon_tests.ribbons_test(pokemon)
    gba_pokemon_tests.contest_stats_test(pokemon)
    gba_pokemon_tests.EVs_test(pokemon)
    gba_pokemon_tests.IVs_test(pokemon)
end

-- Ruby

function test_gba_ruby_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Ruby")
end

function test_gba_ruby_unown_form()
    gba_pokemon_tests.unown_form_test("Ruby")
end

function test_gba_ruby_pokemon()
    gba_pokemon_tests.pokemon_test("Ruby")
end

-- Sapphire

function test_gba_sapphire_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Sapphire")
end

function test_gba_sapphire_unown_form()
    gba_pokemon_tests.unown_form_test("Sapphire")
end

function test_gba_sapphire_pokemon()
    gba_pokemon_tests.pokemon_test("Sapphire")
end

-- Emerald

function test_gba_emerald_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("Emerald")
end

function test_gba_emerald_unown_form()
    gba_pokemon_tests.unown_form_test("Emerald")
end

function test_gba_emerald_pokemon()
    gba_pokemon_tests.pokemon_test("Emerald")
end

-- FireRed

function test_gba_firered_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("FireRed")
end

function test_gba_firered_unown_form()
    gba_pokemon_tests.unown_form_test("FireRed")
end

function test_gba_firered_pokemon()
    gba_pokemon_tests.pokemon_test("FireRed")
end

-- LeafGreen

function test_gba_leafgreen_invalid_pokemon()
    pokemon_tests.invalid_pokemon_test("LeafGreen")
end

function test_gba_leafgreen_unown_form()
    gba_pokemon_tests.unown_form_test("LeafGreen")
end

function test_gba_leafgreen_pokemon()
    gba_pokemon_tests.pokemon_test("LeafGreen")
end
