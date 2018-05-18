#!/usr/bin/ruby
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"
require "PKMNTest"

require "minitest/autorun"

class PokemonPCTest < PKMNTest
    @@GAMES = ["Red", "Blue", "Yellow",
               "Gold", "Silver", "Crystal",
               "Ruby", "Sapphire", "Emerald",
               "FireRed", "LeafGreen",
               "Colosseum", "XD"]

    def _pokemon_box_test_common(box)
        generation = @@GAME_GENERATIONS[box.game]
        if generation >= 2
            assert_raises ArgumentError do
                box.name = "ABCDEFGHI"
            end

            box.name = "ABCDEFGH"
            assert_equal("ABCDEFGH", box.name)
        else
            # The getter won't throw, but the setter will.
            assert_equal("", box.name)
            assert_raises RuntimeError do
                box.name = "ABCDEFGH"
            end
        end

        box.each do |pokemon|
            assert_equal("None", pokemon.species)
            assert_equal(box.game, pokemon.game)

            pokemon.moves.each do |move|
                assert_equal("None", move.move)
                assert_equal(0, move.pp)
            end
        end

        original_first = box[0]
        original_second = box[1]

        bulbasaur = PKMN::Pokemon.new("Bulbasaur", box.game, "", 5)
        charmander = PKMN::Pokemon.new("Charmander", box.game, "", 5)
        squirtle = PKMN::Pokemon.new("Squirtle", box.game, "", 5)

        # Make sure we can't set them at invalid indices.
        assert_raises IndexError do
            box[-1] = bulbasaur
        end
        assert_raises IndexError do
            box[box.length] = bulbasaur
        end

        box[0] = bulbasaur
        assert_equal(1, box.num_pokemon)
        assert_equal("Bulbasaur", box[0].species)
        box[1] = charmander
        assert_equal(2, box.num_pokemon)
        assert_equal("Charmander", box[1].species)

        # Replace one of the new ones.
        box[0] = squirtle
        assert_equal(2, box.num_pokemon)
        assert_equal("Squirtle", box[0].species)

        # Make sure we can't copy a Pokémon to itself.
        assert_raises ArgumentError do
            box[1] = box[1]
        end

        # Copy a Pokémon whose memory is already part of the box.
        box[2] = box[1]
        assert_equal(3, box.num_pokemon)

        # We should always be able to clear the last contiguous Pokémon.
        box[2] = original_first
        assert_equal(2, box.num_pokemon)
        assert_equal("None", box[2].species)

        # Put it back.
        box[2] = box[1]
        assert_equal(3, box.num_pokemon)

        # Check that Pokémon can be placed non-contiguously in the correct games.
        if generation <= 2
            assert_raises ArgumentError do
                box[1] = original_first
            end
            assert_equal(3, box.num_pokemon)
            assert_equal("Charmander", box[1].species)

            assert_raises IndexError do
                box[4] = charmander
            end
            assert_equal(3, box.num_pokemon)
            assert_equal("None", box[4].species)
        else
            box[1] = original_first
            assert_equal(2, box.num_pokemon)
            assert_equal("None", box[1].species)

            box[4] = bulbasaur
            assert_equal(3, box.num_pokemon)
            assert_equal("Bulbasaur", box[4].species)

            # Restore it to how it was
            box[1] = charmander
            box[4] = original_first
            assert_equal(3, box.num_pokemon)
            assert_equal("Charmander", box[1].species)
            assert_equal("None", box[4].species)
        end

        # Now check everything we've created. Each variable should have
        # the same Pokémon underneath, even if the pointer has changed.
        assert_equal("Squirtle", box[0].species)
        assert_equal("Charmander", box[1].species)
        assert_equal("Charmander", box[2].species)
        assert_equal("None", original_first.species)
        assert_equal("None", original_second.species)
        assert_equal("Bulbasaur", bulbasaur.species)
        assert_equal("Charmander", charmander.species)
        assert_equal("Squirtle", squirtle.species)
    end

    def _pokemon_box_test(game)
        box = PKMN::PokemonBox.new(game)
        assert_equal(game, box.game)
        assert_equal(0, box.num_pokemon)

        _pokemon_box_test_common(box)
    end

    def _pokemon_pc_test(game)
        pc = PKMN::PokemonPC.new(game)

        generation = @@GAME_GENERATIONS[game]

        pc.each do |box|
            _pokemon_box_test_common(box)
        end

        if generation >= 2
            (0..(pc.length-1)).each do |i|
                box_name = "BOX#{i}"
                pc[i].name = box_name
                assert_equal(pc.box_names[i], box_name)
            end
        else
            assert_equal(0, pc.box_names.length)
        end

        pc.each do |box|
            assert_equal("Squirtle", box[0].species)
            assert_equal("Charmander", box[1].species)
            assert_equal("Charmander", box[2].species)
        end
    end

    @@GAMES.each do |game|
        define_method("test_pokemon_box_#{game}") do
            _pokemon_box_test(game)
        end
    end

    @@GAMES.each do |game|
        define_method("test_pokemon_pc_#{game}") do
            _pokemon_pc_test(game)
        end
    end
end
