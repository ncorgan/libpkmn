#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(ruby_tests
    RubyBuildInfoTest
    RubyCalculationsTest
    RubyDatabaseEntryTest
    RubyListsTest
    RubyPathsTest
    RubyPokemonTest
    RubyPokemonPartyTest
    RubyPokemonPCTest
)

FOREACH(test ${ruby_tests})
    PKMN_ADD_RUBY_TEST(${test})
ENDFOREACH(test ${ruby_tests})
