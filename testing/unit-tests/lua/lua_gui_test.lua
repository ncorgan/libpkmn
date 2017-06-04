--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local io = require("io")
local luaunit = require("luaunit")
local os = require("os")
local pkmn = require("pkmn")

local LIBPKMN_TEST_FILES = pkmn.paths.getenv("LIBPKMN_TEST_FILES")
local PKMN_TMP_DIR = pkmn.paths.get_tmp_dir()

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

function utils.compare_two_files(filepath1, filepath2)
    print(filepath1)
    luaunit.assertTrue(utils.file_exists(filepath1))
    local file1 = io.open(filepath1, "rb")
    luaunit.assertNotEquals(file1, null)

    print(filepath2)
    luaunit.assertTrue(utils.file_exists(filepath2))
    local file2 = io.open(filepath2, "rb")
    luaunit.assertNotEquals(file2, null)

    local data1 = file1:read("*all")
    local data2 = file2:read("*all")

    file1:close()
    file2:close()

    -- The next call would encompass this, but it makes the error checking
    -- more granular.
    luaunit.assertEquals(#data1, #data2)
    luaunit.assertEquals(data1, data2)
end

function test_spinda()
    if pkmn.buildinfo.get_qt_version() ~= "N/A"
    then
        local SPINDA_FORMAT = "spinda_%d_%d_%d.png"

        local personality       = 0x88888888
        local personality_shiny = 0xF81C8021

        for generation = 3, 5
        do
            local spinda_filename = string.format(SPINDA_FORMAT, generation, 0, personality)

            local test_files_spinda_filepath = utils.concat_paths(LIBPKMN_TEST_FILES, "spinda", spinda_filename)
            local test_spinda_filepath = utils.concat_paths(PKMN_TMP_DIR, spinda_filename)

            pkmn.gui.generate_spinda_sprite_at_filepath(
                generation,
                personality,
                false,
                test_spinda_filepath
            )
            luaunit.assertTrue(utils.file_exists(test_spinda_filepath))
            utils.compare_two_files(test_spinda_filepath, test_files_spinda_filepath)
            os.remove(test_spinda_filepath)

            spinda_filename = string.format(SPINDA_FORMAT, generation, 1, personality_shiny)

            test_files_spinda_filepath = utils.concat_paths(LIBPKMN_TEST_FILES, "spinda", spinda_filename)
            test_spinda_filepath = utils.concat_paths(PKMN_TMP_DIR, spinda_filename)

            pkmn.gui.generate_spinda_sprite_at_filepath(
                generation,
                personality_shiny,
                true,
                test_spinda_filepath
            )
            luaunit.assertTrue(utils.file_exists(test_spinda_filepath))
            utils.compare_two_files(test_spinda_filepath, test_files_spinda_filepath)
            os.remove(test_spinda_filepath)
        end
    else
        luaunit.assertError(pkmn.gui.generate_spinda_sprite_at_filepath, 0, 0, false, "")
    end
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
