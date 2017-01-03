--
-- Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")
local pkmn = require("pkmn")

-- http://stackoverflow.com/a/30960054
function windows()
    local shared_lib_ext = package.cpath:match("%p[\\|/]?%p(%a+)")
    return (shared_lib_ext == "dll")
end

function test_appdata_dir()
    local appdata_dir = ""
    if windows()
    then
        appdata_dir = "C:\\libpkmn\\appdata\\dir"
    else
        appdata_dir = "/libpkmn/appdata/dir"
    end

    pkmn.paths.setenv("PKMN_APPDATA_DIR", appdata_dir)
    luaunit.assertEquals(pkmn.paths.get_appdata_dir(), appdata_dir)

    -- Convenient time to test unsetenv.
    pkmn.paths.unsetenv("PKMN_APPDATA_DIR")
    luaunit.assertNotEquals(pkmn.paths.get_appdata_dir(), appdata_dir)
end

function test_database_path()
    --
    -- When this unit test is run, the PKMN_DATABASE_PATH environment variable
    -- is set to a valid value, so just make sure the call doesn't fail.
    --
    pkmn.paths.get_database_path()

    -- Cause a failure with an existing file that isn't a database
    pkmn.paths.setenv("PKMN_DATABASE_PATH", arg[0])
    luaunit.assertError(pkmn.paths.get_database_path)

    -- Cause a failure with a non-existent file.
    local database_path = ""
    if windows()
    then
        database_path = "C:\\libpkmn\\database\\path"
    else
        database_path = "/libpkmn/database/path"
    end

    pkmn.paths.setenv("PKMN_DATABASE_PATH", database_path)
    luaunit.assertError(pkmn.paths.get_database_path)
end

function test_images_dir()
    local images_dir = ""
    if windows()
    then
        images_dir = "C:\\libpkmn\\images\\dir"
    else
        images_dir = "/libpkmn/images/dir"
    end

    pkmn.paths.setenv("PKMN_IMAGES_DIR", images_dir)
    luaunit.assertEquals(pkmn.paths.get_images_dir(), images_dir)
end

-- Unimplemented
-- function test_tmp_dir()
-- end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
