#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "minitest/autorun"
require "os"
require "pathname"

class PokemonTest < MiniTest::Test
    @@GAME_GENERATIONS = {
        "Red" => 1,
        "Blue" => 1,
        "Yellow" => 1,
        "Gold" => 2,
        "Silver" => 2,
        "Crystal" => 2,
        "Ruby" => 3,
        "Sapphire" => 3,
        "Emerald" => 3,
        "FireRed" => 3,
        "LeafGreen" => 3,
        "Colosseum" => 3,
        "XD" => 3,
        "Diamond" => 4,
        "Pearl" => 4,
        "Platinum" => 4,
        "HeartGold" => 4,
        "SoulSilver" => 4,
        "Black" => 5,
        "White" => 5,
        "Black 2" => 5,
        "White 2" => 5,
        "X" => 6,
        "Y" => 6,
        "Omega Ruby" => 6,
        "Alpha Sapphire" => 6
    }

    def check_initial_values(pokemon)
    end
end
