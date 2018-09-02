#!/usr/bin/ruby
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "minitest/autorun"

class PokemonPartyTest < MiniTest::Test
    @@GAMES = ["Red", "Blue", "Yellow",
               "Gold", "Silver", "Crystal",
               "Ruby", "Sapphire", "Emerald",
               "FireRed", "LeafGreen",
               "Colosseum", "XD"]

    def _test_empty_pokemon_party(party, game)
        party.each do |pokemon|
            assert_equal(PKMN::Species::NONE, pokemon.species)
            assert_equal(game, pokemon.game)

            pokemon.moves.each do |move|
                assert_equal(PKMN::Move::NONE, move.move)
                assert_equal(0, move.pp)
            end
        end
    end

    def _test_setting_pokemon(party, game)
        original_first = party[0]
        original_second = party[1]

        bulbasaur = PKMN::Pokemon.new(PKMN::Species::BULBASAUR, game, "", 5)
        charmander = PKMN::Pokemon.new(PKMN::Species::CHARMANDER, game, "", 5)
        squirtle = PKMN::Pokemon.new(PKMN::Species::SQUIRTLE, game, "", 5)

        # Make sure we can't set them at invalid indices.
        assert_raises IndexError do
            party[-1] = bulbasaur
        end
        assert_raises IndexError do
            party[6] = bulbasaur
        end

        party[0] = bulbasaur
        assert_equal(1, party.num_pokemon)
        assert_equal(PKMN::Species::BULBASAUR, party[0].species)
        party[1] = charmander
        assert_equal(2, party.num_pokemon)
        assert_equal(PKMN::Species::CHARMANDER, party[1].species)

        # Replace one of the new ones.
        party[0] = squirtle
        assert_equal(2, party.num_pokemon)
        assert_equal(PKMN::Species::SQUIRTLE, party[0].species)

        # Copy a Pokémon already part of the party.
        party[2] = party[1]
        assert_equal(3, party.num_pokemon)

        # We should always be able to clear the last contiguous Pokémon.
        party[2] = original_first
        assert_equal(2, party.num_pokemon)
        assert_equal(PKMN::Species::NONE, party[2].species)

        # Put it back.
        party[2] = party[1]
        assert_equal(3, party.num_pokemon)

        # Check that Pokémon cannot be placed non-contiguously.
        assert_raises ArgumentError do
            party[1] = original_first
        end
        assert_equal(3, party.num_pokemon)
        assert_equal(PKMN::Species::CHARMANDER, party[1].species)

        assert_raises IndexError do
            party[4] = charmander
        end
        assert_equal(3, party.num_pokemon)
        assert_equal(PKMN::Species::NONE, party[4].species)

        # Now check everything we've created. Each variable should have
        # the same Pokémon underneath, even if the pointer has changed.
        assert_equal(PKMN::Species::SQUIRTLE, party[0].species)
        assert_equal(PKMN::Species::CHARMANDER, party[1].species)
        assert_equal(PKMN::Species::CHARMANDER, party[2].species)
        assert_equal(PKMN::Species::NONE, original_first.species)
        assert_equal(PKMN::Species::NONE, original_second.species)
        assert_equal(PKMN::Species::BULBASAUR, bulbasaur.species)
        assert_equal(PKMN::Species::CHARMANDER, charmander.species)
        assert_equal(PKMN::Species::SQUIRTLE, squirtle.species)
    end

    def _pokemon_party_test(game)
        party = PKMN::PokemonParty.new(game)
        assert_equal(game, party.game)
        assert_equal(6, party.length)
        assert_equal(0, party.num_pokemon)

        _test_empty_pokemon_party(party, game)
        _test_setting_pokemon(party, game)
    end

     @@GAMES.each do |game|
        define_method("test_pokemon_party_#{game}") do
            _pokemon_party_test(PKMN::string_to_game(game))
        end
    end
end
