--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_tests = {}

pokemon_tests.GAME_TO_GENERATION = {
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

pokemon_tests.STARTERS = {
    "Bulbasaur", "Chikorita", "Treecko", "Turtwig", "Snivy", "Chespin"
}

pokemon_tests.GEN1_POKEMON_WITH_XY_MEGA_FORMS = {
    "Venusaur", "Blastoise", "Alakazam", "Gengar",
    "Kangaskhan", "Pinsir", "Gyarados", "Aerodactyl"
}

pokemon_tests.GEN1_POKEMON_WITH_ORAS_MEGA_FORMS = {
    "Beedrill", "Pidgeot", "Slowbro"
}

pokemon_tests.GEN2_POKEMON_WITH_XY_MEGA_FORMS = {
    "Ampharos", "Scizor", "Heracross", "Houndoom", "Tyranitar"
}

pokemon_tests.GEN3_POKEMON_WITH_XY_MEGA_FORMS = {
    "Blaziken", "Gardevoir", "Mawile", "Aggron", "Medicham",
    "Manectric", "Banette", "Absol", "Latias", "Latios", "Garchomp"
}

pokemon_tests.GEN3_POKEMON_WITH_ORAS_MEGA_FORMS = {
    "Sceptile", "Swampert", "Sableye", "Sharpedo", "Camerupt",
    "Altaria", "Glalie", "Salamence", "Metagross", "Rayquaza"
}

function pokemon_tests.gen1_forms_test(game)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    -- Check that Mega forms only work in their given games.
    for i = 1, #pokemon_tests.GEN1_POKEMON_WITH_XY_MEGA_FORMS
    do
        if generation >= 6
        then
            pkmn.pokemon(
                pokemon_tests.GEN1_POKEMON_WITH_XY_MEGA_FORMS[i],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN1_POKEMON_WITH_XY_MEGA_FORMS[i],
                game,
                "Mega",
                100
            )
        end
    end

    for i = 1, #pokemon_tests.GEN1_POKEMON_WITH_ORAS_MEGA_FORMS
    do
        if game == "Omega Ruby" or game == "Alpha Sapphire"
        then
            pkmn.pokemon(
                pokemon_tests.GEN1_POKEMON_WITH_ORAS_MEGA_FORMS[i],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN1_POKEMON_WITH_ORAS_MEGA_FORMS[i],
                game,
                "Mega",
                100
            )
        end
    end

    -- Cosplay Pikachu should only work in OR/AS.
    cosplay_pikachu_forms = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", ""):get_forms()
    for i = 2, #cosplay_pikachu_forms
    do
        if game == "Omega Ruby" or game == "Alpha Sapphire"
        then
            pkmn.pokemon(
                "Pikachu",
                game,
                cosplay_pikachu_forms[i],
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                "Pikachu",
                game,
                cosplay_pikachu_forms[i],
                100
            )
        end
    end

    -- Hardcode Mega X/Y cases.
    local species = {"Charizard", "Mewtwo"}
    local forms = {"Mega X", "Mega Y"}
    for i = 1, 2
    do
        for j = 1, 2
        do
            if game == "Omega Ruby" or game == "Alpha Sapphire"
            then
                pkmn.pokemon(
                    species[i],
                    game,
                    forms[j],
                    100
                )
            else
                luaunit.assertError(
                    pkmn.pokemon,
                    species[i],
                    game,
                    forms[j],
                    100
                )
            end
        end
    end
end

function pokemon_tests.gen2_forms_test(game)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    -- Check that Mega forms only work in their given games.
    for i = 1, #pokemon_tests.GEN2_POKEMON_WITH_XY_MEGA_FORMS
    do
        if generation >= 6
        then
            pkmn.pokemon(
                pokemon_tests.GEN2_POKEMON_WITH_XY_MEGA_FORMS[i],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN2_POKEMON_WITH_XY_MEGA_FORMS[i],
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
    for i = 1, #letters
    do
        pkmn.pokemon("Unown", game, letters:sub(i,i), 10)
    end

    if generation > 2
    then
        pkmn.pokemon("Unown", game, "!", 10)
        pkmn.pokemon("Unown", game, "?", 10)
    else
        luaunit.assertError(pkmn.pokemon, "Unown", game, "!", 10)
        luaunit.assertError(pkmn.pokemon, "Unown", game, "?", 10)
    end
end

function pokemon_tests.gen3_forms_test(game)
    local gamecube = (game == "Colosseum" or game == "XD")
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    -- Check that Mega forms only work in their given games.
    for i = 1, #pokemon_tests.GEN3_POKEMON_WITH_XY_MEGA_FORMS
    do
        if generation >= 6
        then
            pkmn.pokemon(
                pokemon_tests.GEN3_POKEMON_WITH_XY_MEGA_FORMS[i],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN3_POKEMON_WITH_XY_MEGA_FORMS[i],
                game,
                "Mega",
                100
            )
        end
    end

    for i = 1, #pokemon_tests.GEN3_POKEMON_WITH_ORAS_MEGA_FORMS
    do
        if game == "Omega Ruby" or game == "Alpha Sapphire"
        then
            pkmn.pokemon(
                pokemon_tests.GEN3_POKEMON_WITH_ORAS_MEGA_FORMS[i],
                game,
                "Mega",
                100
            )
        else
            luaunit.assertError(
                pkmn.pokemon,
                pokemon_tests.GEN3_POKEMON_WITH_ORAS_MEGA_FORMS[i],
                game,
                "Mega",
                100
            )
        end
    end

    -- Castform should always work.
    local castform_forms = pkmn.database.pokemon_entry("Castform", "Omega Ruby", ""):get_forms()
    for i = 1, #castform_forms
    do
        pkmn.pokemon("Castform", game, castform_forms[i], 30)
    end

    -- Primal Reversion should only work in OR/AS.
    local species = {"Groudon", "Kyogre"}
    for i = 1, 2
    do
        pkmn.pokemon(species[i], game, "", 70)
        if game == "Omega Ruby" or game == "Alpha Sapphire"
        then
            pkmn.pokemon(species, game, "Primal Reversion", 70)
        else
            luaunit.assertError(pkmn.pokemon, species, game, "Primal Reversion", 70)
        end
    end

    -- In Generation III, Deoxys's form is game-specific.
    if generation == 3
    then
        if game == "Ruby" or game == "Sapphire" or game == "Colosseum" or game == "XD"
        then
            pkmn.pokemon("Deoxys", game, "Normal", 70)
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Normal", 70)
        end

        if game == "FireRed"
        then
            pkmn.pokemon("Deoxys", game, "Attack", 70)
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Attack", 70)
        end

        if game == "LeafGreen"
        then
            pkmn.pokemon("Deoxys", game, "Defense", 70)
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Defense", 70)
        end

        if game == "Emerald"
        then
            pkmn.pokemon("Deoxys", game, "Speed", 70)
        else
            luaunit.assertError(pkmn.pokemon, "Deoxys", game, "Speed", 70)
        end
    else
        local deoxys_forms = pkmn.database.pokemon_entry("Deoxys", "Omega Ruby", ""):get_forms()
        for i = 1, #deoxys_forms
        do
            pkmn.pokemon("Deoxys", game, deoxys_forms[i], 70)
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

function pokemon_tests.invalid_starters_test(game)
    local generation = pokemon_tests.GAME_TO_GENERATION[game]

    for i = generation+1, 6
    do
        luaunit.assertError(pkmn.pokemon, pokemon_tests.STARTERS[i], game, "", 5)
    end
end

function pokemon_tests.invalid_pokemon_test(game)
    pokemon_tests.forms_test(game)
    pokemon_tests.invalid_starters_test(game)
end

function pokemon_tests.gender_test(game)
    -- Single-gender Pokémon
    local nidorina = pkmn.pokemon("Nidorina", game, "", 50)
    luaunit.assertEquals(nidorina:get_gender(), "Female")
    nidorina:set_gender("Female")
    luaunit.assertError(nidorina.set_gender, nidorina, "Male")
    luaunit.assertError(nidorina.set_gender, nidorina, "Genderless")

    local nidorino = pkmn.pokemon("Nidorino", game, "", 50)
    luaunit.assertEquals(nidorino:get_gender(), "Male")
    nidorino:set_gender("Male")
    luaunit.assertError(nidorino.set_gender, nidorino, "Female")
    luaunit.assertError(nidorino.set_gender, nidorino, "Genderless")

    local magnemite = pkmn.pokemon("Magnemite", game, "", 50)
    luaunit.assertEquals(magnemite:get_gender(), "Genderless")
    magnemite:set_gender("Genderless")
    luaunit.assertError(magnemite.set_gender, magnemite, "Male")
    luaunit.assertError(magnemite.set_gender, magnemite, "Female")

    local mixed_pokemon = {
        "Charmander", -- 87.5% male
        "Growlithe",  -- 75% male
        "Pidgey",     -- 50% male
        "Vulpix"      -- 25% male
    }
    for i = 1, #mixed_pokemon
    do
        local pokemon = pkmn.pokemon(
                            mixed_pokemon[i],
                            game,
                            "",
                            50
                        )
        pokemon:set_gender("Female")
        luaunit.assertEquals(pokemon:get_gender(), "Female")
        pokemon:set_gender("Male")
        luaunit.assertEquals(pokemon:get_gender(), "Male")
        luaunit.assertError(pokemon.set_gender, pokemon, "Genderless")
    end
end

return pokemon_tests
