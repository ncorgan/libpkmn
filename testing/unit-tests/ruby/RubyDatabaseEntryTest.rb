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

# For these tests, just make sure the wrapper constant works as intended.
class RubyDatabaseEntryTest < Test::Unit::TestCase
    def test_item_entry
        # Make sure trying to create an invalid entry results in an error.
        assert_raise ArgumentError do
            PKMN::Database::ItemEntry.new("Not an item", "SoulSilver")
        end
        assert_raise ArgumentError do
            PKMN::Database::ItemEntry.new("Fast Ball", "Not a game")
        end
        assert_raise ArgumentError do
            PKMN::Database::ItemEntry.new("Fast Ball", "Red")
        end

        entry = PKMN::Database::ItemEntry.new("Fast Ball", "SoulSilver")

        assert_equal("Fast Ball", entry.name)
        assert_equal("SoulSilver", entry.game)
        assert_equal("Apricorn balls", entry.category)
        assert_equal("Poké Balls", entry.pocket)
    end
end
