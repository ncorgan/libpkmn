--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_party_tests = {}
pokemon_party_tests.pokemon_party = {}

-- Stupid hacky functions to be able to test indexing and attributes

function pokemon_party_tests.pokemon_party.get_pokemon(party, index)
    local pokemon = party[index]
end

function pokemon_party_tests.pokemon_party.set_pokemon(party, index, pokemon)
    party[index] = pokemon
end

-- Actual test functions

function pokemon_party_tests.test_empty_pokemon_party(party, game)
    luaunit.assertEquals(party.game, game)
    luaunit.assertEquals(#party, 6)

    for party_index = 1, #party
    do
        luaunit.assertEquals(party[party_index].species, pkmn.species.NONE)
        luaunit.assertEquals(party[party_index].game, game)

        for move_index = 1, #party[party_index].moves
        do
            luaunit.assertEquals(party[party_index].moves[move_index].move, pkmn.move.NONE)
            luaunit.assertEquals(party[party_index].moves[move_index].pp, 0)
        end
    end

    -- Make sure trying to access a Pokémon at an invalid index fails.
    luaunit.assertError(
        pokemon_party_tests.pokemon_party.get_pokemon,
        party,
        0
    )
    luaunit.assertError(
        pokemon_party_tests.pokemon_party.get_pokemon,
        party,
        #party+1
    )
end

function pokemon_party_tests.test_setting_pokemon(party)
    local game = party.game

    local original_first = party[1]
    local original_second = party[2]

    -- Make sure we can't set Pokémon at invalid indices.
    luaunit.assertError(
        pokemon_party_tests.pokemon_party.set_pokemon,
        party,
        0,
        original_first
    )
    luaunit.assertError(
        pokemon_party_tests.pokemon_party.set_pokemon,
        party,
        #party+1,
        original_second
    )

    -- Create Pokémon and place in party. The original variables should
    -- still have the same underlying Pokémon.
    local bulbasaur = pkmn.pokemon(pkmn.species.BULBASAUR, game, "", 5)
    local charmander = pkmn.pokemon(pkmn.species.CHARMANDER, game, "", 5)
    local squirtle = pkmn.pokemon(pkmn.species.SQUIRTLE, game, "", 5)

    party[1] = bulbasaur
    luaunit.assertEquals(party.num_pokemon, 1)
    luaunit.assertEquals(party[1].species, pkmn.species.BULBASAUR)
    party[2] = charmander
    luaunit.assertEquals(party.num_pokemon, 2)
    luaunit.assertEquals(party[2].species, pkmn.species.CHARMANDER)

    -- Replace one of the new ones.
    party[1] = squirtle
    luaunit.assertEquals(party.num_pokemon, 2)
    luaunit.assertEquals(party[1].species, pkmn.species.SQUIRTLE)

    -- Copy a Pokémon already part of the party.
    party[3] = party[2]
    luaunit.assertEquals(party.num_pokemon, 3)
    luaunit.assertEquals(party[3].species, pkmn.species.CHARMANDER)

    -- We should be able to clear the last contiguous Pokémon.
    party[3] = original_first
    luaunit.assertEquals(party.num_pokemon, 2)
    luaunit.assertEquals(party[3].species, pkmn.species.NONE)

    -- Put it back.
    party[3] = party[2]
    luaunit.assertEquals(party.num_pokemon, 3)
    luaunit.assertEquals(party[3].species, pkmn.species.CHARMANDER)

    -- Check that Pokémon cannot be placed non-contiguously.
    luaunit.assertError(
        pokemon_party_tests.pokemon_party.set_pokemon,
        party,
        2,
        original_first
    )
    luaunit.assertEquals(party.num_pokemon, 3)
    luaunit.assertEquals(party[2].species, pkmn.species.CHARMANDER)

    luaunit.assertError(
        pokemon_party_tests.pokemon_party.set_pokemon,
        party,
        5,
        bulbasaur
    )
    luaunit.assertEquals(party.num_pokemon, 3)
    luaunit.assertEquals(party[5].species, pkmn.species.NONE)

    -- Now check everything we've created. Each variable should have
    -- the same underlying Pokémon.
    luaunit.assertEquals(party[1].species, pkmn.species.SQUIRTLE)
    luaunit.assertEquals(party[2].species, pkmn.species.CHARMANDER)
    luaunit.assertEquals(party[3].species, pkmn.species.CHARMANDER)
    luaunit.assertEquals(original_first.species, pkmn.species.NONE)
    luaunit.assertEquals(original_second.species, pkmn.species.NONE)
    luaunit.assertEquals(bulbasaur.species, pkmn.species.BULBASAUR)
    luaunit.assertEquals(charmander.species, pkmn.species.CHARMANDER)
    luaunit.assertEquals(squirtle.species, pkmn.species.SQUIRTLE)
end

function pokemon_party_tests.test_pokemon_party(party, game)
    pokemon_party_tests.test_empty_pokemon_party(party, game)
    pokemon_party_tests.test_setting_pokemon(party)
end

return pokemon_party_tests
