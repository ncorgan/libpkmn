--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_party_tests = {}

function pokemon_party_tests.test_empty_pokemon_party(party, game)
    luaunit.assertEquals(party.game, game)
    luaunit.assertEquals(#party, 6)

    for party_index = 1, #party
    do
        luaunit.assertEquals(party[party_index].species, "None")
        luaunit.assertEquals(party[party_index].game, game)

        for move_index = 1, #party[party_index].moves
        do
            luaunit.assertEquals(party[party_index].moves[move_index].move, "None")
            luaunit.assertEquals(party[party_index].moves[move_index].pp, 0)
        end
    end
end

function pokemon_party_tests.test_setting_pokemon(party)
    local game = party.game

    local original_first = party[1]
    local original_second = party[2]

    -- Create Pokémon and place in party. The original variables should
    -- still have the same underlying Pokémon.
    local bulbasaur = pkmn.pokemon("Bulbasaur", game, "", 5)
    local charmander = pkmn.pokemon("Charmander", game, "", 5)
    local squirtle = pkmn.pokemon("Squirtle", game, "", 5)

    party[1] = bulbasaur
    luaunit.assertEquals(party.num_pokemon, 1)
    luaunit.assertEquals(party[1].species, "Bulbasaur")
    party[2] = charmander
    luaunit.assertEquals(party.num_pokemon, 2)
    luaunit.assertEquals(party[2].species, "Charmander")

    -- Replace one of the new ones.
    party[1] = squirtle
    luaunit.assertEquals(party.num_pokemon, 2)
    luaunit.assertEquals(party[1].species, "Squirtle")

    -- Make sure we can't copy a Pokémon to itself.
    luaunit.assertError(party.set_pokemon, 2, party[2])
    luaunit.assertEquals(party.num_pokemon, 2)

    -- Copy a Pokémon whose memory is already part of the party.
    party[3] = party[2]
    luaunit.assertEquals(party.num_pokemon, 3)
    luaunit.assertEquals(party[3].species, "Charmander")

    -- We should be able to clear the last contiguous Pokémon.
    party[3] = original_first
    luaunit.assertEquals(party.num_pokemon, 2)
    luaunit.assertEquals(party[3].species, "None")

    -- Put it back.
    party[3] = party[2]
    luaunit.assertEquals(party.num_pokemon, 3)
    luaunit.assertEquals(party[3].species, "Charmander")

    -- Check that Pokémon cannot be placed non-contiguously.
    luaunit.assertError(party.set_pokemon, party, 2, original_first)
    luaunit.assertEquals(party.num_pokemon, 3)
    luaunit.assertEquals(party[2].species, "Charmander")

    luaunit.assertError(party.set_pokemon, 5, bulbasaur)
    luaunit.assertEquals(party.num_pokemon, 3)
    luaunit.assertEquals(party[5].species, "None")

    -- Now check everything we've created. Each variable should have
    -- the same underlying Pokémon.
    luaunit.assertEquals(party[1].species, "Squirtle")
    luaunit.assertEquals(party[2].species, "Charmander")
    luaunit.assertEquals(party[3].species, "Charmander")
    luaunit.assertEquals(original_first.species, "None")
    luaunit.assertEquals(original_second.species, "None")
    luaunit.assertEquals(bulbasaur.species, "Bulbasaur")
    luaunit.assertEquals(charmander.species, "Charmander")
    luaunit.assertEquals(squirtle.species, "Squirtle")
end

function pokemon_party_tests.test_pokemon_party(party, game)
    pokemon_party_tests.test_empty_pokemon_party(party, game)
    pokemon_party_tests.test_setting_pokemon(party)
end

return pokemon_party_tests
