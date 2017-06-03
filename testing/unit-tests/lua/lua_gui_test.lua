--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")
local os = require("os")
local pkmn = require("pkmn")

utils = {}

-- http://stackoverflow.com/a/30960054
function utils.is_windows()
    local shared_lib_ext = package.cpath:match("%p[\\|/]?%p(%a+)")
    return (shared_lib_ext == "dll")
end

function utils.concat_paths(path1, path2, path3)
    if utils.is_windows()
    then
        if path3 == nil
        then
            return string.format("%s\\%s", path1, path2)
        else
            return string.format("%s\\%s\\%s", path1, path2, path3)
        end
    else
        if path3 == nil
        then
            return string.format("%s/%s", path1, path2)
        else
            return string.format("%s/%s/%s", path1, path2, path3)
        end
    end
end

-- http://stackoverflow.com/a/4991602
function utils.file_exists(name)
    local f=io.open(name,"r")
    if f~=nil then io.close(f) return true else return false end
end

function test_spinda()
    local sprite_path       = utils.concat_paths(pkmn.paths.get_tmp_dir(), "spinda_0.png")
    local sprite_path_shiny = utils.concat_paths(pkmn.paths.get_tmp_dir(), "spinda_1.png")

    for generation = 3, 5
    do
        pkmn.gui.generate_spinda_sprite_at_filepath(
            generation,
            0x88888888,
            false,
            sprite_path
        )
        luaunit.assertTrue(utils.file_exists(sprite_path))
        os.remove(sprite_path)

        pkmn.gui.generate_spinda_sprite_at_filepath(
            generation,
            0x88888888,
            true,
            sprite_path_shiny
        )
        luaunit.assertTrue(utils.file_exists(sprite_path_shiny))
        os.remove(sprite_path_shiny)
    end
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
