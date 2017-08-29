#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "os"
require "test/unit"

class RubyPathsTest < Test::Unit::TestCase
    def test_appdata_dir
        if OS.windows?
            appdata_dir = "C:\\Windows\\appdata\\dir"
        else
            appdata_dir = "/appdata/dir"
        end

        ENV["PKMN_APPDATA_DIR"] = appdata_dir
        assert_equal(appdata_dir, PKMN::Paths::get_appdata_dir())
    end

    def test_database_path
        #
        # When this unit test is run, the PKMN_DATABASE_PATH environment
        # variable is set to a valid value, so just make sure the call doesn't
        # fail.
        #
        PKMN::Paths::get_database_path()

        # Cause a failure with an existing file that isn't a database.
        assert_raise RuntimeError do
            ENV["PKMN_DATABASE_PATH"] = __FILE__
            PKMN::Paths::get_database_path()
        end

        # Cause an error with a non-existent file.
        if OS.windows?
            database_path = "C:\\libpkmn\\database\\path"
        else
            database_path = "/libpkmn/database/path"
        end
        assert_raise RuntimeError do
            ENV["PKMN_DATABASE_PATH"] = database_path
            PKMN::Paths::get_database_path()
        end
    end

    def test_images_dir
        if OS.windows?
            images_dir = "C:\\libpkmn\\images\\dir"
        else
            images_dir = "/libpkmn/images/dir"
        end

        ENV["PKMN_IMAGES_DIR"] = images_dir
        PKMN::Paths::get_images_dir()
    end

    def test_tmp_dir
        if OS.windows?
            tmp_dir = "C:\\libpkmn\\tmp\\dir"
        else
            tmp_dir = "/libpkmn/tmp/dir"
        end

        ENV["PKMN_TMP_DIR"] = tmp_dir
        PKMN::Paths::get_tmp_dir()
    end
end
