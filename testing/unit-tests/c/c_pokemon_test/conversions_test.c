/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/pokemon_comparison.h>
#include <pkmntest-c/util.h>

#include <pkmn.h>

#include <unity.h>

#include <string.h>

static const pkmn_pokemon2_t empty_pokemon =
{
    .species = NULL,
    .game = NULL,
    ._internal = NULL
};

static void conversions_test(
    const char* species,
    const char* origin_game,
    const char* dest_game
)
{
    TEST_ASSERT_NOT_NULL(origin_game);
    TEST_ASSERT_NOT_NULL(dest_game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int origin_generation = game_to_generation(origin_game);
    int dest_generation = game_to_generation(dest_game);
    int min_generation = (origin_generation < dest_generation) ? origin_generation
                                                               : dest_generation;

    pkmn_pokemon2_t first_pokemon = empty_pokemon;
    pkmn_pokemon2_t second_pokemon = empty_pokemon;

    get_random_pokemon(
        &first_pokemon,
        species,
        origin_game
    );
    TEST_ASSERT_NOT_NULL(first_pokemon._internal);

    error = pkmn_pokemon2_to_game(
                &first_pokemon,
                dest_game,
                &second_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(second_pokemon._internal);

    // Comparison


    TEST_ASSERT_EQUAL_STRING(first_pokemon.species, second_pokemon.species);
    TEST_ASSERT_EQUAL_STRING(dest_game, second_pokemon.game);

    // TODO: condition

    compare_pokemon_strings(
        &first_pokemon,
        &second_pokemon,
        "Form",
        pkmn_pokemon2_get_form
    );
    compare_pokemon_strings(
        &first_pokemon,
        &second_pokemon,
        "Nickname",
        pkmn_pokemon2_get_nickname
    );
    compare_pokemon_ints(
        &first_pokemon,
        &second_pokemon,
        "Experience",
        pkmn_pokemon2_get_experience
    );
    compare_pokemon_ints(
        &first_pokemon,
        &second_pokemon,
        "Experience",
        pkmn_pokemon2_get_level
    );

    compare_pokemon_original_trainer_info(
        &first_pokemon,
        &second_pokemon
    );
    compare_pokemon_moves(
        &first_pokemon,
        &second_pokemon
    );

    if(min_generation >= 2)
    {
        // TODO: gender

        compare_pokemon_bools(
            &first_pokemon,
            &second_pokemon,
            "Is shiny",
            pkmn_pokemon2_is_shiny
        );
        compare_pokemon_strings(
            &first_pokemon,
            &second_pokemon,
            "Held item",
            pkmn_pokemon2_get_held_item
        );
        compare_pokemon_ints(
            &first_pokemon,
            &second_pokemon,
            "Current trainer friendship",
            pkmn_pokemon2_get_current_trainer_friendship
        );

        int first_pokemon_level = 0;
        int second_pokemon_level_met = 0;

        error = pkmn_pokemon2_get_level(
                    &first_pokemon,
                    &first_pokemon_level
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon2_get_level_met(
                    &second_pokemon,
                    &second_pokemon_level_met
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(first_pokemon_level, second_pokemon_level_met);
    }
    if(min_generation >= 3)
    {
        compare_pokemon_strings(
            &first_pokemon,
            &second_pokemon,
            "Ability",
            pkmn_pokemon2_get_ability
        );
        compare_pokemon_strings(
            &first_pokemon,
            &second_pokemon,
            "Ball",
            pkmn_pokemon2_get_ball
        );
        compare_pokemon_strings(
            &first_pokemon,
            &second_pokemon,
            "Original game",
            pkmn_pokemon2_get_original_game
        );
        compare_pokemon_uint32s(
            &first_pokemon,
            &second_pokemon,
            "Personality",
            pkmn_pokemon2_get_personality
        );
        compare_pokemon_ints(
            &first_pokemon,
            &second_pokemon,
            "Pokérus duration",
            pkmn_pokemon2_get_pokerus_duration
        );

        if(origin_generation == dest_generation)
        {
            compare_pokemon_bool_buffers(
                &first_pokemon,
                &second_pokemon,
                PKMN_NUM_MARKINGS,
                "Markings",
                pkmn_pokemon2_get_markings
            );
            compare_pokemon_int_buffers(
                &first_pokemon,
                &second_pokemon,
                PKMN_NUM_CONTEST_STATS,
                "Contest stats",
                pkmn_pokemon2_get_contest_stats
            );
            compare_pokemon_ribbons(
                &first_pokemon,
                &second_pokemon
            );
        }
    }
}

void gen1_conversions_test()
{
    // Generation I -> Generation I
    conversions_test("Bulbasaur", "Red", "Yellow");

    // Generation I -> Generation II
    conversions_test("Squirtle", "Blue", "Gold");
}

void gen2_conversions_test()
{
    // Generation II -> Generation II
    conversions_test("Cyndaquil", "Gold", "Crystal");
    conversions_test("Totodile", "Crystal", "Gold");

    // Generation II -> Generation I
    conversions_test("Charmander", "Silver", "Blue");
}

void gba_conversions_test()
{
    // GBA -> GBA
    conversions_test("Torchic", "Ruby", "Sapphire");
    conversions_test("Mudkip", "Ruby", "Emerald");
    conversions_test("Treecko", "Ruby", "FireRed");
    conversions_test("Torchic", "Emerald", "Sapphire");
    conversions_test("Mudkip", "Emerald", "Emerald");
    conversions_test("Treecko", "Emerald", "FireRed");
    conversions_test("Charmander", "FireRed", "Sapphire");
    conversions_test("Squirtle", "FireRed", "Emerald");
    conversions_test("Bulbasaur", "FireRed", "FireRed");

    // GBA -> GCN
    conversions_test("Eevee", "Ruby", "Colosseum");
    conversions_test("Espeon", "Emerald", "Colosseum");
    conversions_test("Umbreon", "FireRed", "Colosseum");
    conversions_test("Eevee", "Ruby", "XD");
    conversions_test("Espeon", "Emerald", "XD");
    conversions_test("Umbreon", "FireRed", "XD");
}

void gcn_conversions_test()
{
    // GCN -> GBA
    conversions_test("Eevee", "Colosseum", "Sapphire");
    conversions_test("Espeon", "Colosseum", "Emerald");
    conversions_test("Umbreon", "Colosseum", "LeafGreen");
    conversions_test("Eevee", "XD", "Sapphire");
    conversions_test("Espeon", "XD", "Emerald");
    conversions_test("Umbreon", "XD", "LeafGreen");

    // GCN -> GCN
    conversions_test("Vaporeon", "Colosseum", "Colosseum");
    conversions_test("Jolteon", "Colosseum", "XD");
    conversions_test("Vaporeon", "XD", "XD");
    conversions_test("Jolteon", "XD", "Colosseum");
}
