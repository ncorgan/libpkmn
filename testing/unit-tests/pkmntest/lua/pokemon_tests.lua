--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local bit32 = require("bit32")
local pkmn = require("pkmn")
local pkmntest_utils = require("pkmntest_utils")
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

-- Stupid hacky functions because Luaunit can't check setters.

function pokemon_tests.pokemon_set_ability(pokemon, ability)
    pokemon.ability = ability
end

function pokemon_tests.pokemon_set_ball(pokemon, ball)
    pokemon.ball = ball
end

function pokemon_tests.pokemon_set_condition(pokemon, condition)
    pokemon.condition = condition
end

function pokemon_tests.pokemon_set_current_hp(pokemon, current_hp)
    pokemon.current_hp = current_hp
end

function pokemon_tests.pokemon_set_contest_stat(pokemon, stat, value)
    pokemon.contest_stats[stat] = value
end

function pokemon_tests.pokemon_set_current_trainer_friendship(pokemon, current_trainer_friendship)
    pokemon.current_trainer_friendship = current_trainer_friendship
end

function pokemon_tests.pokemon_set_EV(pokemon, stat, value)
    pokemon.EVs[stat] = value
end

function pokemon_tests.pokemon_set_experience(pokemon, experience)
    pokemon.experience = experience
end

function pokemon_tests.pokemon_set_gender(pokemon, gender)
    pokemon.gender = gender
end

function pokemon_tests.pokemon_set_held_item(pokemon, held_item)
    pokemon.held_item = held_item
end

function pokemon_tests.pokemon_set_IV(pokemon, stat, value)
    pokemon.IVs[stat] = value
end

function pokemon_tests.pokemon_set_nickname(pokemon, nickname)
    pokemon.nickname = nickname
end

function pokemon_tests.pokemon_set_original_trainer_id(pokemon, original_trainer_id)
    pokemon.original_trainer_id = original_trainer_id
end

function pokemon_tests.pokemon_set_original_trainer_public_id(pokemon, original_trainer_public_id)
    pokemon.original_trainer_public_id = original_trainer_public_id
end

function pokemon_tests.pokemon_set_original_trainer_secret_id(pokemon, original_trainer_secret_id)
    pokemon.original_trainer_secret_id = original_trainer_secret_id
end

function pokemon_tests.pokemon_set_original_trainer_name(pokemon, original_trainer_name)
    pokemon.original_trainer_name = original_trainer_name
end

function pokemon_tests.pokemon_set_original_game(pokemon, original_game)
    pokemon.original_game = original_game
end

function pokemon_tests.pokemon_set_level(pokemon, level)
    pokemon.level = level
end

function pokemon_tests.pokemon_set_level_met(pokemon, level_met)
    pokemon.level_met = level_met
end

function pokemon_tests.pokemon_set_location_met(pokemon, location_met)
    pokemon.location_met = location_met
end

function pokemon_tests.pokemon_set_location_met_as_egg(pokemon, location_met_as_egg)
    pokemon.location_met_as_egg = location_met_as_egg
end

function pokemon_tests.pokemon_set_marking(pokemon, marking, has_marking)
    pokemon.markings[marking] = has_marking
end

function pokemon_tests.pokemon_set_move(pokemon, position, move)
    pokemon.moves[position].move = move
end

function pokemon_tests.pokemon_set_move_pp(pokemon, position, pp)
    pokemon.moves[position].pp = pp
end

function pokemon_tests.pokemon_set_pokerus_duration(pokemon, pokerus_duration)
    pokemon.pokerus_duration = pokerus_duration
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
            luaunit.assertTrue(pkmntest_utils.file_exists(unown.icon_filepath))
            luaunit.assertTrue(pkmntest_utils.file_exists(unown.sprite_filepath))
        end
    end

    if generation > 2
    then
        local unown = pkmn.pokemon("Unown", game, "!", 10)
        if game ~= "Colosseum" and game ~= "XD"
        then
            luaunit.assertTrue(pkmntest_utils.file_exists(unown.icon_filepath))
            luaunit.assertTrue(pkmntest_utils.file_exists(unown.sprite_filepath))
        end

        unown = pkmn.pokemon("Unown", game, "?", 10)
        if game ~= "Colosseum" and game ~= "XD"
        then
            luaunit.assertTrue(pkmntest_utils.file_exists(unown.icon_filepath))
            luaunit.assertTrue(pkmntest_utils.file_exists(unown.sprite_filepath))
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
            luaunit.assertTrue(pkmntest_utils.file_exists(castform.icon_filepath))
            luaunit.assertTrue(pkmntest_utils.file_exists(castform.sprite_filepath))
        end
    end

    -- Primal Reversion should only work in OR/AS.
    local species = {"Groudon", "Kyogre"}
    for species_index = 1, 2
    do
        local pokemon = pkmn.pokemon(species[species_index], game, "", 70)
        if game ~= "Colosseum" and game ~= "XD"
        then
            luaunit.assertTrue(pkmntest_utils.file_exists(pokemon.icon_filepath))
            luaunit.assertTrue(pkmntest_utils.file_exists(pokemon.sprite_filepath))
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
                luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.icon_filepath))
                luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.sprite_filepath))
            end
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Normal", 70)
        end

        if game == "FireRed"
        then
            local deoxys = pkmn.pokemon("Deoxys", game, "Attack", 70)
            luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.icon_filepath))
            luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.sprite_filepath))
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Attack", 70)
        end

        if game == "LeafGreen"
        then
            local deoxys = pkmn.pokemon("Deoxys", game, "Defense", 70)
            luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.icon_filepath))
            luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.sprite_filepath))
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Defense", 70)
        end

        if game == "Emerald"
        then
            local deoxys = pkmn.pokemon("Deoxys", game, "Speed", 70)
            luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.icon_filepath))
            luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.sprite_filepath))
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Speed", 70)
        end
    else
        local deoxys_forms = pkmn.database.pokemon_entry("Deoxys", "Omega Ruby", "").forms
        for form_index = 1, #deoxys_forms
        do
            local deoxys = pkmn.pokemon("Deoxys", game, deoxys_forms[form_index], 70)
            luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.icon_filepath))
            luaunit.assertTrue(pkmntest_utils.file_exists(deoxys.sprite_filepath))
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
    nidorina.gender = "Female"
    luaunit.assertError(pokemon_tests.pokemon_set_gender, nidorina, "Male")
    luaunit.assertError(pokemon_tests.pokemon_set_gender, nidorina, "Genderless")

    local nidorino = pkmn.pokemon("Nidorino", game, "", 50)
    luaunit.assertEquals(nidorino.gender, "Male")
    nidorino.gender = "Male"
    luaunit.assertError(pokemon_tests.pokemon_set_gender, nidorino, "Female")
    luaunit.assertError(pokemon_tests.pokemon_set_gender, nidorino, "Genderless")

    local magnemite = pkmn.pokemon("Magnemite", game, "", 50)
    luaunit.assertEquals(magnemite.gender, "Genderless")
    magnemite.gender = "Genderless"
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
        pokemon.gender = "Female"
        luaunit.assertEquals(pokemon.gender, "Female")
        pokemon.gender = "Male"
        luaunit.assertEquals(pokemon.gender, "Male")
        luaunit.assertError(pokemon_tests.pokemon_set_gender, pokemon, "Genderless")
    end
end

function pokemon_tests.check_initial_values(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

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

    luaunit.assertTrue(pkmntest_utils.file_exists(pokemon.icon_filepath))
    luaunit.assertTrue(pkmntest_utils.file_exists(pokemon.sprite_filepath))

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
        luaunit.assertEquals(pokemon.level_met, 30)
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
            bit32.rshift(bit32.band(pkmn.pokemon.DEFAULT_TRAINER_ID, 0xFFFF0000), 16)
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

function pokemon_tests.check_initial_maps(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

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
        luaunit.assertEquals(pokemon.EVs[EV_names[EV_index]], 0)
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

    luaunit.assertEquals(#pokemon.stats, #pokemon.stats:keys())

    luaunit.assertTrue(pokemon.stats:has_key("HP"))
    luaunit.assertTrue(pokemon.stats:has_key("Attack"))
    luaunit.assertTrue(pokemon.stats:has_key("Defense"))
    luaunit.assertTrue(pokemon.stats:has_key("Speed"))
    if generation >= 2
    then
        luaunit.assertFalse(pokemon.stats:has_key("Special"))
        luaunit.assertTrue(pokemon.stats:has_key("Special Attack"))
        luaunit.assertTrue(pokemon.stats:has_key("Special Defense"))
    else
        luaunit.assertTrue(pokemon.stats:has_key("Special"))
        luaunit.assertFalse(pokemon.stats:has_key("Special Attack"))
        luaunit.assertFalse(pokemon.stats:has_key("Special Defense"))
    end

    local stat_names = pokemon.stats:keys()
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
        for marking_index = 1, #marking_names
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
        luaunit.assertEquals(#pokemon.markings, 0)
        luaunit.assertEquals(#pokemon.markings.keys, 0)

        luaunit.assertEquals(#pokemon.ribbons, 0)
        luaunit.assertEquals(#pokemon.ribbons.keys, 0)
    end
end

function pokemon_tests.test_image_filepaths(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    luaunit.assertTrue(pkmntest_utils.file_exists(pokemon.icon_filepath))

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
                luaunit.assertTrue(pkmntest_utils.file_exists(pokemon.sprite_filepath))
            end
        end
    else
        luaunit.assertTrue(pkmntest_utils.file_exists(pokemon.sprite_filepath))
    end
end

function pokemon_tests.test_setting_ability(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

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

function pokemon_tests.test_setting_ball(pokemon, valid_ball_name, invalid_ball_names)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    if generation >= 3
    then
        pokemon.ball = valid_ball_name
        luaunit.assertEquals(pokemon.ball, valid_ball_name)

        for ball_index = 1, #invalid_ball_names
        do
            luaunit.assertError(
                pokemon_tests.pokemon_set_ball,
                pokemon,
                invalid_ball_names[ball_index]
            )
        end
    else
        -- The getter shouldn't throw by convention, but the setter
        -- will.
        luaunit.assertEquals(pokemon.ball, "")
        luaunit.assertError(
            pokemon_tests.pokemon_set_ball,
            pokemon,
            "Great Ball"
        )
    end
end

function pokemon_tests.test_setting_condition(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    local conditions = {"None", "Asleep", "Poison", "Burn", "Frozen", "Paralysis"}
    if generation >= 3
    then
        conditions[#conditions] = "Bad Poison"
    end

    for condition_index = 1, #conditions
    do
        pokemon.condition = conditions[condition_index]
        luaunit.assertEquals(pokemon.condition, conditions[condition_index])
    end
end

function pokemon_tests.test_setting_friendship(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    if generation >= 2
    then
        local test_friendship = math.random(0, 255)

        pokemon.current_trainer_friendship = test_friendship
        luaunit.assertEquals(pokemon.current_trainer_friendship, test_friendship)

        luaunit.assertError(
            pokemon_tests.pokemon_set_current_trainer_friendship,
            pokemon,
            -1
        )
        luaunit.assertError(
            pokemon_tests.pokemon_set_current_trainer_friendship,
            pokemon,
            256
        )
    else
        -- The getter shouldn't throw by convention, but the setter
        -- will.
        luaunit.assertEquals(pokemon.current_trainer_friendship, 0)
        luaunit.assertError(
            pokemon_tests.pokemon_set_current_trainer_friendship,
            pokemon,
            0
        )
    end
end

function pokemon_tests.test_setting_item(pokemon, valid_item_name, invalid_item_names)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    if generation >= 2
    then
        pokemon.held_item = valid_item_name
        luaunit.assertEquals(pokemon.held_item, valid_item_name)

        for item_index = 1, #invalid_item_names
        do
            luaunit.assertError(
                pokemon_tests.pokemon_set_held_item,
                pokemon,
                invalid_item_names[item_index]
            )
        end
    else
        -- The getter shouldn't throw by convention, but the setter
        -- will.
        luaunit.assertEquals(pokemon.held_item, "")
        luaunit.assertError(
            pokemon_tests.pokemon_set_held_item,
            pokemon,
            "Great item"
        )
    end
end

function pokemon_tests.test_setting_levels(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    luaunit.assertError(
        pokemon_tests.pokemon_set_level,
        pokemon,
        -1
    )
    luaunit.assertError(
        pokemon_tests.pokemon_set_level,
        pokemon,
        101
    )

    local level = 50
    pokemon.level = level
    luaunit.assertEquals(pokemon.level, level)
    luaunit.assertEquals(
        pokemon.database_entry:get_level_at_experience(pokemon.experience),
        level
    )

    local experience = 123456
    pokemon.experience = experience
    luaunit.assertEquals(pokemon.experience, experience)
    luaunit.assertTrue(
        pokemon.database_entry:get_experience_at_level(pokemon.level-1) < experience
    )
    luaunit.assertTrue(
        pokemon.database_entry:get_experience_at_level(pokemon.level) <= experience
    )

    if generation == 1
    then
        luaunit.assertError(
            pokemon_tests.pokemon_set_level,
            pokemon,
            1
        )

        -- The getter shouldn't throw by convention, but the setter will.
        luaunit.assertEquals(pokemon.level_met, 0)
        luaunit.assertError(
            pokemon_tests.pokemon_set_level_met,
            pokemon,
            10
        )
    elseif generation == 2
    then
        local level_met = 5
        pokemon.level_met = level_met
        luaunit.assertEquals(pokemon.level_met, level_met)

        luaunit.assertError(
            pokemon_tests.pokemon_set_level,
            pokemon,
            1
        )
        luaunit.assertError(
            pokemon_tests.pokemon_set_level_met,
            pokemon,
            101
        )
    else
        local level_met = 5
        pokemon.level_met = level_met
        luaunit.assertEquals(pokemon.level_met, level_met)

        luaunit.assertError(
            pokemon_tests.pokemon_set_level,
            pokemon,
            0
        )
        luaunit.assertError(
            pokemon_tests.pokemon_set_level_met,
            pokemon,
            101
        )
    end
end

function pokemon_tests.test_setting_location_met(
    pokemon,
    expected_original_location,
    valid_locations,
    invalid_locations
)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    if generation == 1
    then
        -- The getters shouldn't throw by convention, but the setters will.
        luaunit.assertEquals(pokemon.location_met, "")
        luaunit.assertEquals(pokemon.location_met_as_egg, "")

        luaunit.assertError(
            pokemon_tests.pokemon_set_location_met,
            pokemon,
            valid_locations[1]
        )
        luaunit.assertError(
            pokemon_tests.pokemon_set_location_met_as_egg,
            pokemon,
            valid_locations[1]
        )
    elseif generation <= 3
    then
        luaunit.assertEquals(pokemon.location_met, expected_original_location)
        for location_index = 1, #valid_locations
        do
            pokemon.location_met = valid_locations[location_index]
            luaunit.assertEquals(pokemon.location_met, valid_locations[location_index])
        end

        for location_index = 1, #invalid_locations
        do
            luaunit.assertError(
                pokemon_tests.pokemon_set_location_met,
                pokemon,
                invalid_locations[location_index]
            )
        end

        -- The setter shouldn't throw by convention, but the setter will.
        luaunit.assertEquals(pokemon.location_met_as_egg, "")
        luaunit.assertError(
            pokemon_tests.pokemon_set_location_met_as_egg,
            pokemon,
            valid_locations[1]
        )
    else
        luaunit.assertEquals(pokemon.location_met, expected_original_location)
        luaunit.assertEquals(pokemon.location_met_as_egg, expected_original_location)

        for location_index = 1, #valid_locations
        do
            pokemon.location_met = valid_locations[location_index]
            luaunit.assertEquals(pokemon.location_met, valid_locations[location_index])

            pokemon.location_met_as_egg = valid_locations[location_index]
            luaunit.assertEquals(pokemon.location_met_as_egg, valid_locations[location_index])
        end

        for location_index = 1, #invalid_locations
        do
            luaunit.assertError(
                pokemon_tests.pokemon_set_location_met,
                pokemon,
                invalid_locations[location_index]
            )
            luaunit.assertError(
                pokemon_tests.pokemon_set_location_met_as_egg,
                pokemon,
                invalid_locations[location_index]
            )
        end
    end
end

function pokemon_tests.test_setting_markings(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    if generation >= 3
    then
        marking_names = pokemon.markings.keys
        for marking_index = 1, #marking_names
        do
            luaunit.assertFalse(pokemon.markings[marking_names[marking_index]])
            pokemon.markings[marking_names[marking_index]] = true
            luaunit.assertTrue(pokemon.markings[marking_names[marking_index]])
        end
    else
        luaunit.assertEquals(#pokemon.markings, 0)
        luaunit.assertError(
            pokemon_tests.pokemon_set_marking,
            pokemon,
            "Circle",
            true
        )
    end
end

function pokemon_tests.test_setting_moves(pokemon, valid_move_names, invalid_move_names)
    luaunit.assertEquals(#valid_move_names, 4)

    for move_index = 1, #valid_move_names
    do
        pokemon.moves[move_index].move = valid_move_names[move_index]
        luaunit.assertEquals(pokemon.moves[move_index].move, valid_move_names[move_index])

        local valid_move_entry = pkmn.database.move_entry(valid_move_names[move_index], pokemon.game)

        pokemon.moves[move_index].pp = 0
        luaunit.assertEquals(pokemon.moves[move_index].pp, 0)

        local max_pp = valid_move_entry:get_pp(3)
        pokemon.moves[move_index].pp = max_pp
        luaunit.assertEquals(pokemon.moves[move_index].pp, max_pp)
    end
    for move_index = 1, #invalid_move_names
    do
        luaunit.assertError(
            pokemon_tests.pokemon_set_move,
            pokemon,
            1,
            invalid_move_names[move_index]
        )
    end

    luaunit.assertError(
        pokemon_tests.pokemon_set_move,
        pokemon,
        0,
        valid_move_names[1]
    )
    luaunit.assertError(
        pokemon_tests.pokemon_set_move,
        pokemon,
        6,
        valid_move_names[1]
    )
    luaunit.assertError(
        pokemon_tests.pokemon_set_move_pp,
        pokemon,
        0,
        0
    )
    luaunit.assertError(
        pokemon_tests.pokemon_set_move_pp,
        pokemon,
        5,
        0
    )
end

function pokemon_tests.test_setting_original_game(pokemon, valid_games, invalid_games)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    if generation >= 3
    then
        for game_index = 1, #valid_games
        do
            pokemon.original_game = valid_games[game_index]

            if valid_games[game_index] == "Colosseum" or valid_games[game_index] == "XD"
            then
                luaunit.assertEquals(pokemon.original_game, "Colosseum/XD")
            else
                luaunit.assertEquals(pokemon.original_game, valid_games[game_index])
            end
        end

        for game_index = 1, #invalid_games
        do
            luaunit.assertError(
                pokemon_tests.pokemon_set_original_game,
                pokemon,
                invalid_games[game_index]
            )
        end
    else
        -- The getter shouldn't throw by convention, but the setter will.
        luaunit.assertEquals(pokemon.original_game, "")
        luaunit.assertError(
            pokemon_tests.pokemon_set_original_game,
            pokemon,
            pokemon.game
        )
    end
end

function pokemon_tests.test_setting_personality(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    if generation >= 3
    then
        -- Apparently, 0xFFFFFFFF is too large for math.random
        local test_personality = bit32.bor(math.random(0, 0xFFFF), bit32.lshift(math.random(0, 0xFFFF), 16))

        pokemon.personality = test_personality
        luaunit.assertEquals(pokemon.personality, test_personality)

        -- Make sure SWIG+Lua properly enforces the uint32_t bounds.
        pkmntest_utils.enforce_uint32_param(
            pokemon_tests.pokemon_set_personality,
            pokemon
        )
    else
        -- The getter shouldn't throw by convention, but the setter will.
        luaunit.assertEquals(pokemon.personality, 0)
        luaunit.assertError(
            pokemon_tests.pokemon_set_personality,
            pokemon,
            0
        )
    end
end

function pokemon_tests.test_setting_pokerus(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    if generation >= 2
    then
        local test_pokerus_duration = math.random(0, 15)

        pokemon.pokerus_duration = test_pokerus_duration
        luaunit.assertEquals(pokemon.pokerus_duration, test_pokerus_duration)

        luaunit.assertError(
            pokemon_tests.pokemon_set_pokerus_duration,
            pokemon,
            -1
        )
        luaunit.assertError(
            pokemon_tests.pokemon_set_pokerus_duration,
            pokemon,
            16
        )
    else
        -- The getter shouldn't throw by convention, but the setter will.
        luaunit.assertEquals(pokemon.pokerus_duration, 0)
        luaunit.assertError(
            pokemon_tests.pokemon_set_pokerus_duration,
            pokemon,
            0
        )
    end
end

function pokemon_tests.test_setting_stats(pokemon)
    -- Test setting bounds for setting the current HP.

    luaunit.assertError(
        pokemon_tests.pokemon_set_current_hp,
        pokemon,
        -1
    )
    luaunit.assertError(
        pokemon_tests.pokemon_set_current_hp,
        pokemon,
        pokemon.stats["HP"] + 1
    )

    pokemon.current_hp = 0
    luaunit.assertEquals(pokemon.current_hp, 0)

    pokemon.current_hp = pokemon.stats["HP"] - 1
    luaunit.assertEquals(pokemon.current_hp, pokemon.stats["HP"] - 1)

    pokemon.current_hp = pokemon.stats["HP"]
    luaunit.assertEquals(pokemon.current_hp, pokemon.stats["HP"])

    -- Set the HP stat to lower than the current HP, and make sure
    -- it's updated.
    local current_hp = pokemon.current_hp
    pokemon.EVs["HP"] = 0
    pokemon.IVs["HP"] = 0
    luaunit.assertTrue(pokemon.current_hp <= current_hp)

    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    if generation >= 3
    then
        local contest_stat_names = pokemon.contest_stats.keys
        for contest_stat_index = 1, #contest_stat_names
        do
            local stat_value = math.random(0, 255)
            pokemon.contest_stats[contest_stat_names[contest_stat_index]] = stat_value
            luaunit.assertEquals(pokemon.contest_stats[contest_stat_names[contest_stat_index]], stat_value)
        end
    end
end

function pokemon_tests.test_setting_trainer_info(pokemon)
    local generation = pokemon_tests.GAME_TO_GENERATION[pokemon.game]

    luaunit.assertError(
        pokemon_tests.pokemon_set_nickname,
        pokemon,
        ""
    )
    luaunit.assertError(
        pokemon_tests.pokemon_set_nickname,
        pokemon,
        "Too long nickname"
    )

    pokemon.nickname = "foobarbaz"
    luaunit.assertEquals(pokemon.nickname, "foobarbaz")

    luaunit.assertError(
        pokemon_tests.pokemon_set_original_trainer_name,
        pokemon,
        ""
    )
    luaunit.assertError(
        pokemon_tests.pokemon_set_original_trainer_name,
        pokemon,
        "Too long trainer name"
    )

    pokemon.original_trainer_name = "foobar"
    luaunit.assertEquals(pokemon.original_trainer_name, "foobar")

    if generation >= 2
    then
        pokemon.original_trainer_gender = "Male"
        luaunit.assertEquals(pokemon.original_trainer_gender, "Male")

        pokemon.original_trainer_gender = "Female"
        luaunit.assertEquals(pokemon.original_trainer_gender, "Female")

        luaunit.assertError(
            pokemon_tests.pokemon_set_original_trainer_gender,
            pokemon,
            "Genderless"
        )
    else
        luaunit.assertError(
            pokemon_tests.pokemon_set_original_trainer_gender,
            pokemon,
            "Male"
        )
    end

    if generation >= 3
    then
        pokemon.original_trainer_id = 0x1234ABCD
        luaunit.assertEquals(pokemon.original_trainer_id, 0x1234ABCD)
        luaunit.assertEquals(pokemon.original_trainer_public_id, 0xABCD)
        luaunit.assertEquals(pokemon.original_trainer_secret_id, 0x1234)

        pokemon.original_trainer_public_id = 0x1A2B
        luaunit.assertEquals(pokemon.original_trainer_id, 0x12341A2B)
        luaunit.assertEquals(pokemon.original_trainer_public_id, 0x1A2B)
        luaunit.assertEquals(pokemon.original_trainer_secret_id, 0x1234)

        pokemon.original_trainer_secret_id = 0x3C4D
        luaunit.assertEquals(pokemon.original_trainer_id, 0x3C4D1A2B)
        luaunit.assertEquals(pokemon.original_trainer_public_id, 0x1A2B)
        luaunit.assertEquals(pokemon.original_trainer_secret_id, 0x3C4D)
    else
        luaunit.assertError(
            pokemon_tests.pokemon_set_original_trainer_id,
            pokemon,
            0xFFFF+1
        )

        -- The getter shouldn't throw by convention, but the setter will.
        luaunit.assertEquals(pokemon.original_trainer_secret_id, 0)
        luaunit.assertError(
            pokemon_tests.pokemon_set_original_trainer_secret_id,
            pokemon,
            0xFFFF
        )

        pokemon.original_trainer_id = 0xABCD
        luaunit.assertEquals(pokemon.original_trainer_id, 0xABCD)
        luaunit.assertEquals(pokemon.original_trainer_public_id, 0xABCD)

        pokemon.original_trainer_public_id = 0x9876
        luaunit.assertEquals(pokemon.original_trainer_id, 0x9876)
        luaunit.assertEquals(pokemon.original_trainer_public_id, 0x9876)
    end

    -- Make sure SWIG+Lua properly enforces that numeric values fit
    -- inside the range of the corresponding C++ types.
    pkmntest_utils.enforce_uint16_param(
        pokemon_tests.pokemon_set_original_trainer_public_id,
        pokemon
    )
    pkmntest_utils.enforce_uint16_param(
        pokemon_tests.pokemon_set_original_trainer_secret_id,
        pokemon
    )
    pkmntest_utils.enforce_uint32_param(
        pokemon_tests.pokemon_set_original_trainer_id,
        pokemon
    )
end

function pokemon_tests.test_common(pokemon, test_params)
    pokemon_tests.check_initial_values(pokemon)
    pokemon_tests.check_initial_maps(pokemon)
    pokemon_tests.test_setting_ability(pokemon)
    pokemon_tests.test_setting_ball(
        pokemon,
        test_params.valid_ball,
        test_params.invalid_balls
    )
    pokemon_tests.test_setting_condition(pokemon)
    pokemon_tests.test_image_filepaths(pokemon)
    pokemon_tests.test_setting_friendship(pokemon)
    pokemon_tests.test_setting_item(
        pokemon,
        test_params.valid_item,
        test_params.invalid_items
    )
    pokemon_tests.test_setting_levels(pokemon)
    pokemon_tests.test_setting_location_met(
        pokemon,
        test_params.expected_original_location,
        test_params.valid_locations,
        test_params.invalid_locations
    )
    pokemon_tests.test_setting_markings(pokemon)
    pokemon_tests.test_setting_moves(
        pokemon,
        test_params.valid_moves,
        test_params.invalid_moves
    )
    pokemon_tests.test_setting_original_game(
        pokemon,
        test_params.valid_original_games,
        test_params.invalid_original_games
    )
    pokemon_tests.test_setting_personality(pokemon)
    pokemon_tests.test_setting_pokerus(pokemon)
    pokemon_tests.test_setting_stats(pokemon)
    pokemon_tests.test_setting_trainer_info(pokemon)
end

return pokemon_tests
