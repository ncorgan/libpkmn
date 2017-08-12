#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "PokemonTest"

require "pathname"

class PokemonTestParams
    def initialize
        @valid_ball = ""
        @invalid_balls = []

        @valid_item = ""
        @invalid_items = []

        @expected_original_location = ""
        @valid_locations = []
        @invalid_locations = []

        @valid_moves = []
        @invalid_moves = []

        @valid_original_games = []
        @invalid_original_games = []

        # Getters
        def valid_ball
            @valid_ball
        end

        def invalid_balls
            @invalid_balls
        end

        def valid_item
            @valid_item
        end

        def invalid_items
            @invalid_items
        end

        def expected_original_location
            @expected_original_location
        end

        def valid_locations
            @valid_locations
        end

        def invalid_locations
            @invalid_locations
        end

        def valid_moves
            @valid_moves
        end

        def invalid_moves
            @invalid_moves
        end

        def valid_original_games
            @valid_original_games
        end

        def invalid_original_games
            @invalid_original_games
        end

        # Setters
        def valid_ball=(value)
            @valid_ball = value
        end

        def invalid_balls=(value)
            @invalid_balls = value
        end

        def valid_item=(value)
            @valid_item = value
        end

        def invalid_items=(value)
            @invalid_items = value
        end

        def expected_original_location=(value)
            @expected_original_location = value
        end

        def valid_locations=(value)
            @valid_locations = value
        end

        def invalid_locations=(value)
            @invalid_locations = value
        end

        def valid_moves=(value)
            @valid_moves = value
        end

        def invalid_moves=(value)
            @invalid_moves = value
        end

        def valid_original_games=(value)
            @valid_original_games = value
        end

        def invalid_original_games=(value)
            @invalid_original_games = value
        end
    end
end
