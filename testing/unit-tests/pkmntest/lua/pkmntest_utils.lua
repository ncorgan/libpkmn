--
-- Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local bit32 = require("bit32")
local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pkmntest_utils = {}

pkmntest_utils.GAME_TO_GENERATION =
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

-- http://stackoverflow.com/a/4991602
function pkmntest_utils.file_exists(name)
    local f=io.open(name,"r")
    if f~=nil then io.close(f) return true else return false end
end

pkmntest_utils.string = {}

-- http://lua-users.org/wiki/StringRecipes
function pkmntest_utils.string.starts(String,Start)
    return string.sub(String,1,string.len(Start))==Start
end

-- http://lua-users.org/wiki/StringRecipes
function pkmntest_utils.string.ends(String,End)
    return End=='' or string.sub(String,-string.len(End))==End
end

function pkmntest_utils.random_bool()
    return (math.random(0, 255) > 127)
end

function pkmntest_utils.random_string(len)
    ret = ""
    for i = 1, len
    do
        n = math.random(65, 90)
        if pkmntest_utils.random_bool()
        then
            n = math.random(97, 122)
        end

        ret = ret .. string.char(n)
    end

    return ret
end

-- Helper functions to make sure SWIG+Lua properly enforces bounds
-- for parameters only constrained by the size of a type.

function pkmntest_utils.enforce_uint16_param(var, func)
    luaunit.assertError(func, var, -1)
    luaunit.assertError(func, var, 0xFFFF+1)
end

function pkmntest_utils.enforce_uint32_param(var, func)
    luaunit.assertError(func, var, -1)
    luaunit.assertError(func, var, 0xFFFFFFFF+1)
end

-- http://stackoverflow.com/a/30960054
function pkmntest_utils.is_windows()
    local shared_lib_ext = package.cpath:match("%p[\\|/]?%p(%a+)")
    return (shared_lib_ext == "dll")
end

function pkmntest_utils.concat_path(path1, path2)
    if pkmntest_utils.is_windows()
    then
        return string.format("%s\\%s", path1, path2)
    else
        return string.format("%s/%s", path1, path2)
    end
end

function pkmntest_utils.concat_paths(path1, path2, path3)
    if pkmntest_utils.is_windows()
    then
        return string.format("%s\\%s\\%s", path1, path2, path3)
    else
        return string.format("%s/%s/%s", path1, path2, path3)
    end
end

function pkmntest_utils.compare_attributes(var1, var2)
    luaunit.assertEquals(var1.numeric_attributes.names, var2.numeric_attributes.names)
    local numeric_attribute_names = var1.numeric_attributes.names
    for attribute_index = 1, #numeric_attribute_names
    do
        luaunit.assertEquals(
            var1.numeric_attributes[numeric_attribute_names[attribute_index]],
            var2.numeric_attributes[numeric_attribute_names[attribute_index]]
        )
    end

    luaunit.assertEquals(var1.string_attributes.names, var2.string_attributes.names)
    local string_attribute_names = var1.string_attributes.names
    for attribute_index = 1, #string_attribute_names
    do
        luaunit.assertEquals(
            var1.string_attributes[string_attribute_names[attribute_index]],
            var2.string_attributes[string_attribute_names[attribute_index]]
        )
    end

    luaunit.assertEquals(var1.boolean_attributes.names, var2.boolean_attributes.names)
    local boolean_attribute_names = var1.boolean_attributes.names
    for attribute_index = 1, #boolean_attribute_names
    do
        luaunit.assertEquals(
            var1.boolean_attributes[boolean_attribute_names[attribute_index]],
            var2.boolean_attributes[boolean_attribute_names[attribute_index]]
        )
    end
end

function pkmntest_utils.get_specific_random_pokemon(game, species, form, move_list, item_list)
    local generation = pkmntest_utils.GAME_TO_GENERATION[game]

    local ret = pkmn.pokemon(species, game, "", math.random(2, 63)) -- To account for Gen II level met limitation

    for move_index = 1, 4
    do
        local move = ""
        repeat
            move = move_list[math.random(1, #move_list)]
        until string.find(move, "Shadow") == nil
        ret.moves[move_index].move = move
    end

    for EV_index = 1, #ret.EVs
    do
        ret.EVs[ret.EVs.keys[EV_index]] = math.random(0, 255)
    end
    for IV_index = 1, #ret.IVs
    do
        ret.IVs[ret.IVs.keys[IV_index]] = math.random(0, 15)
    end

    if generation >= 2
    then
        -- Keep going until one is holdable. Use this function since pcall can't
        -- take a variable setter...
        function pokemon_set_held_item(pokemon, held_item)
            pokemon.held_item = held_item
        end
        repeat
            pcall(pokemon_set_held_item, ret, item_list[math.random(1, #item_list)])
        until ret.held_item ~= "None" and string.find(ret.held_item, "None") == nil

        ret.pokerus_duration = math.random(0, 15)
    end
    if generation >= 3
    then
        for marking_index = 1, #ret.markings
        do
            ret.markings[ret.markings.keys[marking_index]] = pkmntest_utils.random_bool()
        end
        for ribbon_index = 1, #ret.ribbons
        do
            ret.ribbons[ret.ribbons.keys[ribbon_index]] = pkmntest_utils.random_bool()
        end
        for contest_stat_index = 1, #ret.contest_stats
        do
            ret.contest_stats[ret.contest_stats.keys[contest_stat_index]] = math.random(0, 255)
        end
    end

    return ret
end

function pkmntest_utils.get_random_pokemon(game, pokemon_list, move_list, item_list)
    local generation = pkmntest_utils.GAME_TO_GENERATION[game]

    -- Don't deal with Deoxys issues here.
    local species = ""
    repeat
        species = pokemon_list[math.random(1, #pokemon_list)]
    until (generation ~= 3 or species ~= "Deoxys")

    return pkmntest_utils.get_specific_random_pokemon(game, species, "", move_list, item_list)
end

function pkmntest_utils.compare_pokemon(pokemon1, pokemon2)
    local generation = pkmntest_utils.GAME_TO_GENERATION[pokemon1.game]

    -- There is no way to tell what game an imported Generation I-II
    -- Pokémon comes from, so LibPKMN defaults to a default valid game.
    if generation >= 3
    then
        luaunit.assertEquals(pokemon1.game, pokemon2.game)
    end

    luaunit.assertEquals(pokemon1.species, pokemon2.species)
    luaunit.assertEquals(pokemon1.form, pokemon2.form)
    luaunit.assertEquals(pokemon1.original_trainer_id, pokemon2.original_trainer_id)
    luaunit.assertEquals(pokemon1.experience, pokemon2.experience)
    luaunit.assertEquals(pokemon1.level, pokemon2.level)
    luaunit.assertEquals(pokemon1.nickname, pokemon2.nickname)
    luaunit.assertEquals(pokemon1.original_trainer_name, pokemon2.original_trainer_name)

    luaunit.assertEquals(pokemon1.EVs.keys, pokemon2.EVs.keys)
    for EV_index = 1, #pokemon1.EVs
    do
        luaunit.assertEquals(
            pokemon1.EVs[pokemon1.EVs.keys[EV_index]],
            pokemon2.EVs[pokemon2.EVs.keys[EV_index]]
        )
    end

    luaunit.assertEquals(pokemon1.IVs.keys, pokemon2.IVs.keys)
    for IV_index = 1, #pokemon1.IVs
    do
        luaunit.assertEquals(
            pokemon1.IVs[pokemon1.IVs.keys[IV_index]],
            pokemon2.IVs[pokemon2.IVs.keys[IV_index]]
        )
    end

    luaunit.assertEquals(pokemon1.stats:keys(), pokemon2.stats:keys())
    for stat_index = 1, #pokemon1.stats
    do
        luaunit.assertEquals(
            pokemon1.stats[pokemon1.stats:keys()[stat_index]],
            pokemon2.stats[pokemon2.stats:keys()[stat_index]]
        )
    end

    if pokemon1.game == pokemon2.game
    then
        luaunit.assertEquals(pokemon1.icon_filepath, pokemon2.icon_filepath)
        luaunit.assertEquals(pokemon1.sprite_filepath, pokemon2.sprite_filepath)
    end

    pkmntest_utils.compare_attributes(pokemon1, pokemon2)

    if generation >= 2
    then
        luaunit.assertEquals(pokemon1.original_trainer_gender, pokemon2.original_trainer_gender)
        luaunit.assertEquals(pokemon1.current_trainer_friendship, pokemon2.current_trainer_friendship)
        luaunit.assertEquals(pokemon1.gender, pokemon2.gender)
        luaunit.assertEquals(pokemon1.is_shiny, pokemon2.is_shiny)
        luaunit.assertEquals(pokemon1.held_item, pokemon2.held_item)
        luaunit.assertEquals(pokemon1.level_met, pokemon2.level_met)
        luaunit.assertEquals(pokemon1.location_met, pokemon2.location_met)
        luaunit.assertEquals(pokemon1.pokerus_duration, pokemon2.pokerus_duration)
    end
    if generation >= 3
    then
        luaunit.assertEquals(pokemon1.ability, pokemon2.ability)
        luaunit.assertEquals(pokemon1.ball, pokemon2.ball)
        luaunit.assertEquals(pokemon1.personality, pokemon2.personality)

        luaunit.assertEquals(pokemon1.markings.keys, pokemon2.markings.keys)
        for marking_index = 1, #pokemon1.markings
        do
            luaunit.assertEquals(
                pokemon1.markings[pokemon1.markings.keys[marking_index]],
                pokemon2.markings[pokemon2.markings.keys[marking_index]]
            )
        end

        luaunit.assertEquals(pokemon1.ribbons.keys, pokemon2.ribbons.keys)
        for ribbon_index = 1, #pokemon1.ribbons
        do
            luaunit.assertEquals(
                pokemon1.ribbons[pokemon1.ribbons.keys[ribbon_index]],
                pokemon2.ribbons[pokemon2.ribbons.keys[ribbon_index]]
            )
        end

        luaunit.assertEquals(pokemon1.contest_stats.keys, pokemon2.contest_stats.keys)
        for contest_stat_index = 1, #pokemon1.contest_stats
        do
            luaunit.assertEquals(
                pokemon1.contest_stats[pokemon1.contest_stats.keys[contest_stat_index]],
                pokemon2.contest_stats[pokemon2.contest_stats.keys[contest_stat_index]]
            )
        end
    end
    if generation >= 4
    then
        luaunit.assertEquals(pokemon1.location_met_as_egg, pokemon2.location_met_as_egg)
    end
end

return pkmntest_utils
