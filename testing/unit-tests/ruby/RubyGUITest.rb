#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "fileutils"
require "pathname"
require "test/unit"

class RubyGUITest < Test::Unit::TestCase

    @@PKMN_TMP_DIR = PKMN::Paths::get_tmp_dir()

    def test_spinda
        if PKMN::BuildInfo::QT_VERSION[0] != "N/A"
            spinda_format = "spinda_%d_%d_%u.png"

            personality = 0x88888888
            personality_shiny = 0xF81C8021

            (3..5).each do |generation|
                spinda_filename = spinda_format % [generation, 0, personality]

                test_files_spinda_filepath = File.join(
                                                 ENV["LIBPKMN_TEST_FILES"],
                                                 "spinda-qt%d" % [PKMN::BuildInfo::QT_VERSION[0]],
                                                 spinda_filename
                                             )
                test_spinda_filepath = File.join(@@PKMN_TMP_DIR, spinda_filename)

                PKMN::GUI::generate_spinda_sprite_at_filepath(
                    generation,
                    personality,
                    false,
                    test_spinda_filepath
                )
                assert(Pathname.new(test_spinda_filepath).exist?)
                assert(FileUtils.identical?(test_files_spinda_filepath, test_spinda_filepath))
                File.delete(test_spinda_filepath)

                spinda_filename = spinda_format % [generation, 1, personality_shiny]

                test_files_spinda_filepath = File.join(
                                                 ENV["LIBPKMN_TEST_FILES"],
                                                 "spinda-qt%d" % [PKMN::BuildInfo::QT_VERSION[0]],
                                                 spinda_filename
                                             )
                test_spinda_filepath = File.join(@@PKMN_TMP_DIR, spinda_filename)

                PKMN::GUI::generate_spinda_sprite_at_filepath(
                    generation,
                    personality_shiny,
                    true,
                    test_spinda_filepath
                )
                assert(Pathname.new(test_spinda_filepath).exist?)
                assert(FileUtils.identical?(test_files_spinda_filepath, test_spinda_filepath))
                File.delete(test_spinda_filepath)
            end
	end
    end
end
