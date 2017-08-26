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
class RubyBuildInfoTest < Test::Unit::TestCase
    def test_boost_version
        assert(PKMN::BuildInfo::BOOST_VERSION.length > 0)
    end

    def test_pksav_version
        assert(PKMN::BuildInfo::PKSAV_VERSION.length > 0)
    end

    def test_qt_version
        assert(PKMN::BuildInfo::QT_VERSION.length > 0)
    end

    def test_sqlite3_version
        assert(PKMN::BuildInfo::SQLITE3_VERSION.length > 0)
    end

    def test_sqlitecpp_version
        assert(PKMN::BuildInfo::SQLITECPP_VERSION.length > 0)
    end

    def test_swig_version
        assert(PKMN::BuildInfo::SWIG_VERSION.length > 0)

        # Since this isn't actually a function call, make sure it's valid
        assert(PKMN::BuildInfo::SWIG_VERSION.to_f > 0.0)
    end
end
