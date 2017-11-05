#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "PokemonTest"

class Gen1FriendshipTest < PokemonTest
    def _test_gen1_friendship(game)
        pikachu = PKMN::Pokemon.new("Pikachu", game, "", 5)

        if game == "Yellow"
        then
            pikachu.friendship = 123
            assert_equal(123, pikachu.friendship)

            # Also check a non-Pikachu
            mewtwo = PKMN::Pokemon.new("Mewtwo", game, "", 70)
            assert_raises RuntimeError do
                mewtwo.friendship = 123
            end
            assert_raises RuntimeError do
                mewtwo.friendship
            end
        else
            assert_raises RuntimeError do
                pikachu.friendship = 123
            end
            assert_raises RuntimeError do
                pikachu.friendship
            end
        end
    end

    ["Red", "Blue", "Yellow"].each do |game|
        define_method("test_gen1_friendship_#{game}") do
            _test_gen1_friendship(game)
        end
    end
end
