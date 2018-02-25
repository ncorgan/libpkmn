--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pkmntest_utils = require("pkmntest_utils")
local pokemon_tests = require("pokemon_tests")

local gen3_pokemon_tests = {}

gen3_pokemon_tests.MARKINGS = {"Circle", "Triangle", "Square", "Heart"}
gen3_pokemon_tests.CONTEST_TYPES = {"Cool", "Beauty", "Cute", "Smart", "Tough"}
gen3_pokemon_tests.CONTEST_LEVELS = {"Super", "Hyper", "Master"}
gen3_pokemon_tests.RIBBONS = {"Champion", "Winning", "Victory", "Artist",
                             "Effort", "Marine", "Land", "Sky",
                             "Country", "National", "Earth", "World"}
gen3_pokemon_tests.STATS = {"HP", "Attack", "Defense", "Speed",
                           "Special Attack", "Special Defense"}

function gen3_pokemon_tests.check_initial_ribbon_map(pokemon)
    for contest_type_index = 1, #gen3_pokemon_tests.CONTEST_TYPES
    do
        local contest_type = gen3_pokemon_tests.CONTEST_TYPES[contest_type_index]

        luaunit.assertTrue(pokemon.ribbons:has_key(contest_type))
        luaunit.assertFalse(pokemon.ribbons[contest_type])

        for contest_level_index = 1, #gen3_pokemon_tests.CONTEST_LEVELS
        do
            local contest_level = gen3_pokemon_tests.CONTEST_LEVELS[contest_level_index]
            local ribbon_name = contest_type .. " " .. contest_level

            luaunit.assertTrue(pokemon.ribbons:has_key(ribbon_name))
            luaunit.assertFalse(pokemon.ribbons[ribbon_name])
        end
    end

    for ribbon_index = 1, #gen3_pokemon_tests.RIBBONS
    do
        local ribbon_name = gen3_pokemon_tests.RIBBONS[ribbon_index]
        luaunit.assertTrue(pokemon.ribbons:has_key(ribbon_name))
        luaunit.assertFalse(pokemon.ribbons[ribbon_name])
    end
end

function gen3_pokemon_tests.test_contest_ribbons(pokemon)
    for contest_type_index = 1, #gen3_pokemon_tests.CONTEST_TYPES
    do
        local contest_type = gen3_pokemon_tests.CONTEST_TYPES[contest_type_index]
        local ribbon_name = contest_type
        local super_ribbon_name = contest_type .. " Super"
        local hyper_ribbon_name = contest_type .. " Hyper"
        local master_ribbon_name = contest_type .. " Master"

        pokemon.ribbons[hyper_ribbon_name] = true
        luaunit.assertTrue(pokemon.ribbons[ribbon_name])
        luaunit.assertTrue(pokemon.ribbons[super_ribbon_name])
        luaunit.assertTrue(pokemon.ribbons[hyper_ribbon_name])
        luaunit.assertFalse(pokemon.ribbons[master_ribbon_name])

        pokemon.ribbons[super_ribbon_name] = false
        luaunit.assertTrue(pokemon.ribbons[ribbon_name])
        luaunit.assertFalse(pokemon.ribbons[super_ribbon_name])
        luaunit.assertFalse(pokemon.ribbons[hyper_ribbon_name])
        luaunit.assertFalse(pokemon.ribbons[master_ribbon_name])
    end
end

function gen3_pokemon_tests.test_ribbons(pokemon)
    for ribbon_index = 1, #gen3_pokemon_tests.RIBBONS
    do
        local ribbon_name = gen3_pokemon_tests.RIBBONS[ribbon_index]

        luaunit.assertFalse(pokemon.ribbons[ribbon_name])
        pokemon.ribbons[ribbon_name] = true
        luaunit.assertTrue(pokemon.ribbons[ribbon_name])
    end
end

function gen3_pokemon_tests.unown_test(game)
    local unown_entry = pkmn.database.pokemon_entry("Unown", game, "")

    local unown = nil

    local unown_forms = unown_entry.forms
    for form_index = 1, #unown_forms
    do
        unown = pkmn.pokemon("Unown", game, unown_forms[form_index], 5)
        luaunit.assertEquals(unown.form, unown_forms[form_index])

        -- Make sure the personality properly set.
        local form_from_personality = pkmn.calculations.gen3_unown_form(unown.personality)
        luaunit.assertEquals(form_from_personality, unown.form)

        luaunit.assertTrue(pkmntest_utils.file_exists(unown.icon_filepath))
        luaunit.assertTrue(pkmntest_utils.file_exists(unown.sprite_filepath))
    end

    unown = pkmn.pokemon("Unown", game, "A", 5)

    -- Make sure setting the form properly changes the IVs.
    for form_index = 1, #unown_forms
    do
        unown.form = unown_forms[form_index]
        luaunit.assertEquals(unown.form, unown_forms[form_index])

        -- Make sure the personality properly set.
        local form_from_personality = pkmn.calculations.gen3_unown_form(unown.personality)
        luaunit.assertEquals(form_from_personality, unown.form)

        luaunit.assertTrue(pkmntest_utils.file_exists(unown.icon_filepath))
        luaunit.assertTrue(pkmntest_utils.file_exists(unown.sprite_filepath))
    end

    -- Make sure setting the personality properly sets the form.
    unown.personality = 0x4C07DE71
    luaunit.assertEquals(unown.form, "B")
end

function gen3_pokemon_tests.common(game, species)
    local pokemon = pkmn.pokemon(species, game, "", 30)

    local test_params = nil
    local is_game_gamecube = (game == "Colosseum" or game == "XD")

    if is_game_gamecube
    then
        test_params =
        {
            valid_ball = "Great Ball",
            invalid_balls = {"Friend Ball", "Heal Ball"},

            valid_item = "Razz Berry",
            invalid_items = {"Berry", "Mach Bike"},

            expected_original_location = "Distant land",
            valid_locations = {"Phenac City", "Orre Colosseum"},
            invalid_locations = {"New Bark Town", "Twinleaf Town"},

            valid_moves = {"Swallow", "Flamethrower", "Return", "Fire Blast"},
            invalid_moves = {"Roost", "Flame Burst"},

            valid_original_games = {"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum/XD", "Colosseum", "XD"},
            invalid_original_games = {"Gold", "HeartGold"}
        }
    else
        test_params =
        {
            valid_ball = "Great Ball",
            invalid_balls = {"Friend Ball", "Heal Ball"},

            valid_item = "Razz Berry",
            invalid_items = {"Berry", "Mach Bike"},

            expected_original_location = "Fateful encounter",
            valid_locations = {"Petalburg Woods", "Viridian Forest"},
            invalid_locations = {"New Bark Town", "Twinleaf Town"},

            valid_moves = {"Swallow", "Flamethrower", "Return", "Fire Blast"},
            invalid_moves = {"Shadow Sky", "Roost", "Flame Burst"},

            valid_original_games = {"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum/XD", "Colosseum", "XD"},
            invalid_original_games = {"Gold", "HeartGold"}
        }
    end

    pokemon_tests.test_common(pokemon, test_params)
    gen3_pokemon_tests.check_initial_ribbon_map(pokemon)
    gen3_pokemon_tests.test_contest_ribbons(pokemon)
    gen3_pokemon_tests.test_ribbons(pokemon)

    -- Gender and personality are tied, so make sure they affect each other.

    pokemon.gender = "Female"
    luaunit.assertTrue(bit32.band(pokemon.personality, 0xFF) < 0xFF)
    pokemon.gender = "Male"
    luaunit.assertEquals(bit32.band(pokemon.personality, 0xFF), 0xFF)

    -- Setting shininess should affect personality

    pokemon.is_shiny = false
    local non_shiny_personality = pokemon.personality

    pokemon.is_shiny = true
    luaunit.assertNotEquals(pokemon.personality, non_shiny_personality)

    -- Shadow Pokémon should only work in Gamecube games.

    if is_game_gamecube
    then
        local shadow_species = nil
        if game == "Colosseum"
        then
            shadow_species = "Ledian"
        else
            shadow_species = "Ledyba"
        end

        local shadow_pokemon = pkmn.pokemon(shadow_species, game, "", 50)
        luaunit.assertEquals(shadow_pokemon.form, "Standard")
        shadow_pokemon.form = "Shadow"
        luaunit.assertEquals(shadow_pokemon.form, "Shadow")
    else
        luaunit.assertError(
            pkmn.pokemon,
            "Ledyba",
            game,
            "Shadow",
            50
        )
    end
end

-- Ruby

function test_ruby_pokemon()
    gen3_pokemon_tests.common("Ruby", "Torchic")
end

function test_ruby_forms()
    pokemon_tests.forms_test("Ruby")
end

function test_ruby_genders()
    pokemon_tests.gender_test("Ruby")
end

function test_ruby_unown()
    gen3_pokemon_tests.unown_test("Ruby")
end

-- Sapphire

function test_sapphire_pokemon()
    gen3_pokemon_tests.common("Sapphire", "Mudkip")
end

function test_sapphire_forms()
    pokemon_tests.forms_test("Sapphire")
end

function test_sapphire_genders()
    pokemon_tests.gender_test("Sapphire")
end

function test_sapphire_unown()
    gen3_pokemon_tests.unown_test("Sapphire")
end

-- Emerald

function test_emerald_pokemon()
    gen3_pokemon_tests.common("Emerald", "Treecko")
end

function test_emerald_forms()
    pokemon_tests.forms_test("Emerald")
end

function test_emerald_genders()
    pokemon_tests.gender_test("Emerald")
end

function test_emerald_unown()
    gen3_pokemon_tests.unown_test("Emerald")
end

-- FireRed

function test_firered_pokemon()
    gen3_pokemon_tests.common("FireRed", "Charmander")
end

function test_firered_forms()
    pokemon_tests.forms_test("FireRed")
end

function test_firered_genders()
    pokemon_tests.gender_test("FireRed")
end

function test_firered_unown()
    gen3_pokemon_tests.unown_test("FireRed")
end

-- LeafGreen

function test_leafgreen_pokemon()
    gen3_pokemon_tests.common("LeafGreen", "Bulbasaur")
end

function test_leafgreen_forms()
    pokemon_tests.forms_test("LeafGreen")
end

function test_leafgreen_genders()
    pokemon_tests.gender_test("LeafGreen")
end

function test_leafgreen_unown()
    gen3_pokemon_tests.unown_test("LeafGreen")
end

-- Colosseum

function test_colosseum_pokemon()
    gen3_pokemon_tests.common("Colosseum", "Espeon")
end

function test_colosseum_forms()
    pokemon_tests.forms_test("Colosseum")
end

function test_colosseum_genders()
    pokemon_tests.gender_test("Colosseum")
end

function test_colosseum_unown()
    gen3_pokemon_tests.unown_test("Colosseum")
end

-- XD

function test_xd_pokemon()
    gen3_pokemon_tests.common("XD", "Umbreon")
end

function test_xd_forms()
    pokemon_tests.forms_test("XD")
end

function test_xd_genders()
    pokemon_tests.gender_test("XD")
end

function test_xd_unown()
    gen3_pokemon_tests.unown_test("XD")
end

return gen3_pokemon_tests
