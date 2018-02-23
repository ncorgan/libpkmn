--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local bit32 = require("bit32")
local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_tests = {}

pokemon_tests.GAME_TO_GENERATION =
{
    ["Red"] = 1,
    ["Blue"] = 1,
    ["Yellow"] = 1,
    ["Gold"] = 2,
    ["Silver"] = 2,
    ["Crystal"] = 2,
    ["Ruby"] = 3,
    ["Sapphire"] = 3,
    ["Emerald"] = 3,
    ["FireRed"] = 3,
    ["LeafGreen"] = 3,
    ["Colosseum"] = 3,
    ["XD"] = 3
}

pokemon_tests.GEN1_POKEMON_WITH_XY_MEGA_FORMS =
{
    "Venusaur", "Blastoise", "Alakazam", "Gengar",
    "Kangaskhan", "Pinsir", "Gyarados", "Aerodactyl"
}

pokemon_tests.GEN1_POKEMON_WITH_ORAS_MEGA_FORMS =
{
    "Beedrill", "Pidgeot", "Slowbro"
}

pokemon_tests.GEN2_POKEMON_WITH_XY_MEGA_FORMS =
{
    "Ampharos", "Scizor", "Heracross", "Houndoom", "Tyranitar"
}

pokemon_tests.GEN3_POKEMON_WITH_XY_MEGA_FORMS =
{
    "Blaziken", "Gardevoir", "Mawile", "Aggron", "Medicham",
    "Manectric", "Banette", "Absol", "Latias", "Latios", "Garchomp"
}

pokemon_tests.GEN3_POKEMON_WITH_ORAS_MEGA_FORMS =
{
    "Sceptile", "Swampert", "Sableye", "Sharpedo", "Camerupt",
    "Altaria", "Glalie", "Salamence", "Metagross", "Rayquaza"
}

-- http://stackoverflow.com/a/4991602
function pokemon_tests.file_exists(name)
    local f=io.open(name,"r")
    if f~=nil then io.close(f) return true else return false end
end

-- Stupid hacky functions because Luaunit can't check setters.

function pokemon_tests.pokemon_set_gender(pokemon, gender)
    pokemon.gender = gender
end

function pokemon_tests.pokemon_set_ability(pokemon, ability)
    pokemon.ability = ability
end

-- Actual tests

function pokemon_tests.gen1_forms_test(game)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    -- Check that Mega forms only work in their given games.
    for pokemon_index = 1, #pokemon_tests.GEN1_POKEMON_WITH_XY_MEGA_FORMS
    do
        if generation >= 6
        then
            pkmn.pokemon(
                pokemon_tests.GEN1_POKEMON_WITH_XY_MEGA_FORMS[pokemon_index],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN1_POKEMON_WITH_XY_MEGA_FORMS[pokemon_test],
                game,
                "Mega",
                100
            )
        end
    end

    for pokemon_index = 1, #pokemon_tests.GEN1_POKEMON_WITH_ORAS_MEGA_FORMS
    do
        if game == "Omega Ruby" or game == "Alpha Sapphire"
        then
            pkmn.pokemon(
                pokemon_tests.GEN1_POKEMON_WITH_ORAS_MEGA_FORMS[pokemon_index],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN1_POKEMON_WITH_ORAS_MEGA_FORMS[pokemon_index],
                game,
                "Mega",
                100
            )
        end
    end

    -- Cosplay Pikachu should only work in OR/AS.
    cosplay_pikachu_forms = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "").forms
    for form_index = 2, #cosplay_pikachu_forms
    do
        if game == "Omega Ruby" or game == "Alpha Sapphire"
        then
            pkmn.pokemon(
                "Pikachu",
                game,
                cosplay_pikachu_forms[form_index],
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                "Pikachu",
                game,
                cosplay_pikachu_forms[form_index],
                100
            )
        end
    end

    -- Hardcode Mega X/Y cases.
    local species = {"Charizard", "Mewtwo"}
    local forms = {"Mega X", "Mega Y"}
    for species_index = 1, 2
    do
        for form_index = 1, 2
        do
            if game == "Omega Ruby" or game == "Alpha Sapphire"
            then
                pkmn.pokemon(
                    species[species_index],
                    game,
                    forms[form_index],
                    100
                )
            else
                luaunit.assertError(
                    pkmn.pokemon,
                    species[species_index],
                    game,
                    forms[form_index],
                    100
                )
            end
        end
    end
end

function pokemon_tests.gen2_forms_test(game)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    -- Check that Mega forms only work in their given games.
    for pokemon_index = 1, #pokemon_tests.GEN2_POKEMON_WITH_XY_MEGA_FORMS
    do
        if generation >= 6
        then
            pkmn.pokemon(
                pokemon_tests.GEN2_POKEMON_WITH_XY_MEGA_FORMS[pokemon_index],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN2_POKEMON_WITH_XY_MEGA_FORMS[pokemon_index],
                game,
                "Mega",
                100
            )
        end
    end

    -- Check that Mega forms only work in their given games.
    if game == "Omega Ruby" or game == "Alpha Sapphire"
    then
        pkmn.pokemon("Steelix", game, "Mega", 100)
    else
        luaunit.assertError(pkmn.pokemon, "Steelix", game, "Mega", 100)
    end

    -- Spiky-eared Pikchu should only work in HG/SS.
    if game == "HeartGold" or game == "SoulSilver"
    then
        pkmn.pokemon("Pichu", game, "Spiky-eared", 100)
    else
        luaunit.assertError(pkmn.pokemon, "Pichu", game, "Spiky-eared", 100)
    end

    -- Unown's "!" and "?" forms aren't in Generation II.
    local letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    for letter_index = 1, #letters
    do
        local unown = pkmn.pokemon("Unown", game, letters:sub(letter_index,letter_index), 10)
        if game ~= "Colosseum" and game ~= "XD"
        then
            luaunit.assertTrue(pokemon_tests.file_exists(unown.icon_filepath))
            luaunit.assertTrue(pokemon_tests.file_exists(unown.sprite_filepath))
        end
    end

    if generation > 2
    then
        local unown = pkmn.pokemon("Unown", game, "!", 10)
        if game ~= "Colosseum" and game ~= "XD"
        then
            luaunit.assertTrue(pokemon_tests.file_exists(unown.icon_filepath))
            luaunit.assertTrue(pokemon_tests.file_exists(unown.sprite_filepath))
        end

        unown = pkmn.pokemon("Unown", game, "?", 10)
        if game ~= "Colosseum" and game ~= "XD"
        then
            luaunit.assertTrue(pokemon_tests.file_exists(unown.icon_filepath))
            luaunit.assertTrue(pokemon_tests.file_exists(unown.sprite_filepath))
        end
    else
        luaunit.assertError(pkmn.pokemon, "Unown", game, "!", 10)
        luaunit.assertError(pkmn.pokemon, "Unown", game, "?", 10)
    end
end

function pokemon_tests.gen3_forms_test(game)
    local gamecube = (game == "Colosseum" or game == "XD")
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    -- Check that Mega forms only work in their given games.
    for pokemon_index = 1, #pokemon_tests.GEN3_POKEMON_WITH_XY_MEGA_FORMS
    do
        if generation >= 6
        then
            pkmn.pokemon(
                pokemon_tests.GEN3_POKEMON_WITH_XY_MEGA_FORMS[pokemon_index],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN3_POKEMON_WITH_XY_MEGA_FORMS[pokemon_index],
                game,
                "Mega",
                100
            )
        end
    end

    for pokemon_index = 1, #pokemon_tests.GEN3_POKEMON_WITH_ORAS_MEGA_FORMS
    do
        if game == "Omega Ruby" or game == "Alpha Sapphire"
        then
            pkmn.pokemon(
                pokemon_tests.GEN3_POKEMON_WITH_ORAS_MEGA_FORMS[pokemon_index],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN3_POKEMON_WITH_ORAS_MEGA_FORMS[pokemon_index],
                game,
                "Mega",
                100
            )
        end
    end

    -- Castform should always work.
    local castform_forms = pkmn.database.pokemon_entry("Castform", "Omega Ruby", "").forms
    for form_index = 1, #castform_forms
    do
        local castform = pkmn.pokemon("Castform", game, castform_forms[form_index], 30)
        if game ~= "Colosseum" and game ~= "XD"
        then
            luaunit.assertTrue(pokemon_tests.file_exists(castform.icon_filepath))
            luaunit.assertTrue(pokemon_tests.file_exists(castform.sprite_filepath))
        end
    end

    -- Primal Reversion should only work in OR/AS.
    local species = {"Groudon", "Kyogre"}
    for species_index = 1, 2
    do
        local pokemon = pkmn.pokemon(species[species_index], game, "", 70)
        if game ~= "Colosseum" and game ~= "XD"
        then
            luaunit.assertTrue(pokemon_tests.file_exists(pokemon.icon_filepath))
            luaunit.assertTrue(pokemon_tests.file_exists(pokemon.sprite_filepath))
        end

        if game == "Omega Ruby" or game == "Alpha Sapphire"
        then
            pkmn.pokemon(species[species_index], game, "Primal Reversion", 70)
        else
            luaunit.assertError(pkmn.pokemon, species[species_index], game, "Primal Reversion", 70)
        end
    end

    -- In Generation III, Deoxys's form is game-specific.
    if generation == 3
    then
        if game == "Ruby" or game == "Sapphire" or game == "Colosseum" or game == "XD"
        then
            local deoxys = pkmn.pokemon("Deoxys", game, "Normal", 70)
            if game ~= "Colosseum" and game ~= "XD"
            then
                luaunit.assertTrue(pokemon_tests.file_exists(deoxys.icon_filepath))
                luaunit.assertTrue(pokemon_tests.file_exists(deoxys.sprite_filepath))
            end
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Normal", 70)
        end

        if game == "FireRed"
        then
            local deoxys = pkmn.pokemon("Deoxys", game, "Attack", 70)
            luaunit.assertTrue(pokemon_tests.file_exists(deoxys.icon_filepath))
            luaunit.assertTrue(pokemon_tests.file_exists(deoxys.sprite_filepath))
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Attack", 70)
        end

        if game == "LeafGreen"
        then
            local deoxys = pkmn.pokemon("Deoxys", game, "Defense", 70)
            luaunit.assertTrue(pokemon_tests.file_exists(deoxys.icon_filepath))
            luaunit.assertTrue(pokemon_tests.file_exists(deoxys.sprite_filepath))
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Defense", 70)
        end

        if game == "Emerald"
        then
            local deoxys = pkmn.pokemon("Deoxys", game, "Speed", 70)
            luaunit.assertTrue(pokemon_tests.file_exists(deoxys.icon_filepath))
            luaunit.assertTrue(pokemon_tests.file_exists(deoxys.sprite_filepath))
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Speed", 70)
        end
    else
        local deoxys_forms = pkmn.database.pokemon_entry("Deoxys", "Omega Ruby", "").forms
        for form_index = 1, #deoxys_forms
        do
            local deoxys = pkmn.pokemon("Deoxys", game, deoxys_forms[form_index], 70)
            luaunit.assertTrue(pokemon_tests.file_exists(deoxys.icon_filepath))
            luaunit.assertTrue(pokemon_tests.file_exists(deoxys.sprite_filepath))
        end
    end
end

function pokemon_tests.forms_test(game)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    if generation >= 1
    then
        pokemon_tests.gen1_forms_test(game)
    end
    if generation >= 2
    then
        pokemon_tests.gen2_forms_test(game)
    end
    if generation >= 3
    then
        pokemon_tests.gen3_forms_test(game)
    end
end

function pokemon_tests.gender_test(game)
    -- Single-gender Pokémon
    local nidorina = pkmn.pokemon("Nidorina", game, "", 50)
    luaunit.assertEquals(nidorina.gender, "Female")
    nidorina:set_gender("Female")
    luaunit.assertError(pokemon_tests.pokemon_set_gender, nidorina, "Male")
    luaunit.assertError(pokemon_tests.pokemon_set_gender, nidorina, "Genderless")

    local nidorino = pkmn.pokemon("Nidorino", game, "", 50)
    luaunit.assertEquals(nidorino.gender, "Male")
    nidorino:set_gender("Male")
    luaunit.assertError(pokemon_tests.pokemon_set_gender, nidorino, "Female")
    luaunit.assertError(pokemon_tests.pokemon_set_gender, nidorino, "Genderless")

    local magnemite = pkmn.pokemon("Magnemite", game, "", 50)
    luaunit.assertEquals(magnemite.gender, "Genderless")
    magnemite:set_gender("Genderless")
    luaunit.assertError(pokemon_tests.pokemon_set_gender, magnemite, "Male")
    luaunit.assertError(pokemon_tests.pokemon_set_gender, magnemite, "Female")

    local mixed_pokemon =
    {
        "Charmander", -- 87.5% male
        "Growlithe",  -- 75% male
        "Pidgey",     -- 50% male
        "Vulpix"      -- 25% male
    }
    for pokemon_index = 1, #mixed_pokemon
    do
        local pokemon = pkmn.pokemon(
                            mixed_pokemon[pokemon_index],
                            game,
                            "",
                            50
                        )
        pokemon:set_gender("Female")
        luaunit.assertEquals(pokemon.gender, "Female")
        pokemon:set_gender("Male")
        luaunit.assertEquals(pokemon.gender, "Male")
        luaunit.assertError(pokemon_tests.pokemon_set_gender, pokemon, "Genderless")
    end
end

function pokemon_tests.check_initial_values(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    luaunit.assertEquals(pokemon.condition, "None")
    luaunit.assertEquals(
        pokemon.original_trainer_name,
        pkmn.pokemon.DEFAULT_TRAINER_NAME
    )
    luaunit.assertEquals(
        pokemon.original_trainer_public_id,
        bit32.band(pkmn.pokemon.DEFAULT_TRAINER_ID, 0xFFFF)
    )

    luaunit.assertEquals(
        pokemon.experience,
        pokemon.database_entry:get_experience_at_level(pokemon.level)
    )
    luaunit.assertEquals(pokemon.level, 30)

    luaunit.assertEquals(#pokemon.moves, 4)
    for move_index = 1, #pokemon.moves
    do
        luaunit.assertEquals(pokemon.moves[move_index].move, "None")
        luaunit.assertEquals(pokemon.moves[move_index].pp, 0)
    end

    luaunit.assertTrue(pokemon_tests.file_exists(pokemon.icon_filepath))
    luaunit.assertTrue(pokemon_tests.file_exists(pokemon.sprite_filepath))

    luaunit.assertEquals(pokemon.current_hp, pokemon.stats["HP"])

    -- Not in Generation I, but the default value matches either way.
    luaunit.assertEquals(pokemon.pokerus_duration, 0)

    if generation >= 2
    then
        luaunit.assertEquals(pokemon.held_item, "None")
        luaunit.assertEquals(pokemon.original_trainer_gender, "Male")
        luaunit.assertEquals(
            pokemon.current_trainer_friendship,
            pokemon.database_entry.base_friendship
        )
        luaunit.assertEquals(pokemon.level_met, pokemon.level)
        luaunit.assertEquals(pokemon.level_met, 0)
    else
        -- Default values since accessing a Lua variable shouldn't throw an exception.
        luaunit.assertEquals(pokemon.held_item, "")
        luaunit.assertEquals(pokemon.original_trainer_gender, "")
        luaunit.assertEquals(pokemon.current_trainer_friendship, 0)
        luaunit.assertEquals(pokemon.ball, "")
        luaunit.assertEquals(pokemon.original_game, "")
    end

    if generation >= 3
    then
        luaunit.assertEquals(
            pokemon.original_trainer_secret_id,
            bit32.rshift(bit32.band(pkmn.pokemon.DEFAULT_TRAINER_ID, 0xFFFF), 16)
        )
        luaunit.assertEquals(
            pokemon.original_trainer_id,
            pkmn.pokemon.DEFAULT_TRAINER_ID
        )
        luaunit.assertEquals(pokemon.ball, "Premier Ball")

        -- There is no distinction between Colosseum and XD in the game storage.
        if pokemon.game == "Colosseum" or pokemon.game == "XD"
        then
            luaunit.assertEquals(pokemon.original_game, "Colosseum/XD")
        else
            luaunit.assertEquals(pokemon.original_game, pokemon.game)
        end
    else
        luaunit.assertEquals(
            pokemon.original_trainer_id,
            bit32.band(pkmn.pokemon.DEFAULT_TRAINER_ID, 0xFFFF)
        )

        -- Default value since accessing a Lua variable shouldn't throw an exception.
        luaunit.assertEquals(pokemon.original_trainer_secret_id, 0)
    end

    if generation >= 5
    then
        luaunit.assertEquals(pokemon.nickname, pokemon.species)
    else
        luaunit.assertEquals(pokemon.nickname, string.upper(pokemon.species))
    end
end

function pokemon_tests.test_initial_maps(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    -- EVs

    luaunit.assertEquals(#pokemon.EVs, #pokemon.EVs.keys)

    luaunit.assertTrue(pokemon.EVs:has_key("HP"))
    luaunit.assertTrue(pokemon.EVs:has_key("Attack"))
    luaunit.assertTrue(pokemon.EVs:has_key("Defense"))
    luaunit.assertTrue(pokemon.EVs:has_key("Speed"))
    if generation >= 3
    then
        luaunit.assertFalse(pokemon.EVs:has_key("Special"))
        luaunit.assertTrue(pokemon.EVs:has_key("Special Attack"))
        luaunit.assertTrue(pokemon.EVs:has_key("Special Defense"))
    else
        luaunit.assertTrue(pokemon.EVs:has_key("Special"))
        luaunit.assertFalse(pokemon.EVs:has_key("Special Attack"))
        luaunit.assertFalse(pokemon.EVs:has_key("Special Defense"))
    end

    local EV_names = pokemon.EVs.keys
    for EV_index = 1, #EV_names
    do
        luaunit.assertTrue(pokemon.EVs[EV_names[EV_index]] >= 0)

        local max_EV = 0
        if generation >= 3
        then
            max_EV = 255
        else
            max_EV = 65535
        end
        luaunit.assertTrue(pokemon.EVs[EV_names[EV_index]] <= max_EV)
    end

    -- IVs

    luaunit.assertEquals(#pokemon.IVs, #pokemon.IVs.keys)

    luaunit.assertTrue(pokemon.IVs:has_key("HP"))
    luaunit.assertTrue(pokemon.IVs:has_key("Attack"))
    luaunit.assertTrue(pokemon.IVs:has_key("Defense"))
    luaunit.assertTrue(pokemon.IVs:has_key("Speed"))
    if generation >= 3
    then
        luaunit.assertFalse(pokemon.IVs:has_key("Special"))
        luaunit.assertTrue(pokemon.IVs:has_key("Special Attack"))
        luaunit.assertTrue(pokemon.IVs:has_key("Special Defense"))
    else
        luaunit.assertTrue(pokemon.IVs:has_key("Special"))
        luaunit.assertFalse(pokemon.IVs:has_key("Special Attack"))
        luaunit.assertFalse(pokemon.IVs:has_key("Special Defense"))
    end

    local IV_names = pokemon.IVs.keys
    for IV_index = 1, #IV_names
    do
        luaunit.assertTrue(pokemon.IVs[IV_names[IV_index]] >= 0)

        local max_IV = 0
        if generation >= 3
        then
            max_IV = 31
        else
            max_IV = 15
        end
        luaunit.assertTrue(pokemon.IVs[IV_names[IV_index]] <= max_IV)
    end

    -- Stats

    luaunit.assertEquals(#pokemon.stats, #pokemon.stats.keys)

    luaunit.assertTrue(pokemon.stats:has_key("HP"))
    luaunit.assertTrue(pokemon.stats:has_key("Attack"))
    luaunit.assertTrue(pokemon.stats:has_key("Defense"))
    luaunit.assertTrue(pokemon.stats:has_key("Speed"))
    if generation >= 3
    then
        luaunit.assertFalse(pokemon.stats:has_key("Special"))
        luaunit.assertTrue(pokemon.stats:has_key("Special Attack"))
        luaunit.assertTrue(pokemon.stats:has_key("Special Defense"))
    else
        luaunit.assertTrue(pokemon.stats:has_key("Special"))
        luaunit.assertFalse(pokemon.stats:has_key("Special Attack"))
        luaunit.assertFalse(pokemon.stats:has_key("Special Defense"))
    end

    local stat_names = pokemon.stats.keys
    for stat_index = 1, #stat_names
    do
        luaunit.assertTrue(pokemon.stats[stat_names[stat_index]] >= 0)
        luaunit.assertTrue(pokemon.stats[stat_names[stat_index]] <= 65535)
    end

    if generation >= 3
    then
        -- Markings

        luaunit.assertEquals(#pokemon.markings, #pokemon.markings.keys)

        luaunit.assertTrue(pokemon.markings:has_key("Circle"))
        luaunit.assertTrue(pokemon.markings:has_key("Triangle"))
        luaunit.assertTrue(pokemon.markings:has_key("Square"))
        luaunit.assertTrue(pokemon.markings:has_key("Heart"))
        if generation >= 4
        then
            luaunit.assertTrue(pokemon.markings:has_key("Star"))
            luaunit.assertTrue(pokemon.markings:has_key("Diamond"))
        else
            luaunit.assertFalse(pokemon.markings:has_key("Star"))
            luaunit.assertFalse(pokemon.markings:has_key("Diamond"))
        end

        local marking_names = pokemon.markings.keys
        for marking_index = 1, #markings
        do
            luaunit.assertFalse(pokemon.markings[marking_names[marking_index]])
        end

        -- Ribbons

        luaunit.assertEquals(#pokemon.ribbons, #pokemon.ribbons.keys)
        luaunit.assertTrue(#pokemon.ribbons > 0)

        local ribbon_names = pokemon.ribbons.keys
        for ribbon_index = 1, #ribbon_names
        do
            luaunit.assertTrue(pokemon.ribbons:has_key(ribbon_names[ribbon_index]))
            luaunit.assertFalse(pokemon.ribbons[ribbon_names[ribbon_index]])
        end
    else
        luaunit.assertEquals(#pokemon.markings)
        luaunit.assertEquals(#pokemon.markings.keys)

        luaunit.assertEquals(#pokemon.ribbons)
        luaunit.assertEquals(#pokemon.ribbons.keys)
    end
end

function pokemon_tests.test_image_filepaths(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    luaunit.assertTrue(pokemon_tests.file_exists(pokemon.icon_filepath))

    if generation >= 2
    then
        local genders = {"Male", "Female"}
        local is_shiny_bools = {true, false}

        for gender_index = 1, #genders
        do
            for is_shiny_index = 1, #is_shiny_bools
            do
                pokemon.gender = genders[gender_index]
                pokemon.is_shiny = is_shiny_bools[is_shiny_index]
                luaunit.assertTrue(pokemon_tests.file_exists(pokemon.sprite_filepath))
            end
        end
    else
        luaunit.assertTrue(pokemon_tests.file_exists(pokemon.sprite_filepath))
    end
end

function pokemon_tests.test_setting_ability(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    if generation >= 3
    then
        luaunit.assertNotEquals(pokemon.database_entry.abilities.first, "None")

        pokemon.ability = pokemon.database_entry.abilities.first
        luaunit.assertEquals(pokemon.ability, pokemon.database_entry.abilities.first)

        if pokemon.database_entry.abilities.second ~= "None"
        then
            pokemon.ability = pokemon.database_entry.abilities.second
            luaunit.assertEquals(pokemon.ability, pokemon.database_entry.abilities.second)
        end

        if generation >= 5
        then
            pokemon.ability = pokemon.database_entry.hidden_ability
            luaunit.assertEquals(pokemon.ability, pokemon.database_entry.hidden_ability)
        end

        luaunit.assertError(
            pokemon_tests.pokemon_set_ability,
            pokemon,
            "Not an ability"
        )
        luaunit.assertError(
            pokemon_tests.pokemon_set_ability,
            pokemon,
            "Wonder Guard"
        )
    else
        -- The getter has no error by convention.
        luaunit.assertEquals(pokemon.ability, "")

        luaunit.assertError(
            pokemon_tests.pokemon_set_ability,
            pokemon,
            "Wonder Guard"
        )
    end
end

return pokemon_tests
