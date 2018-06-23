/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/pokemon_comparison.h>
#include <pkmntest-c/util.h>

#include <pkmn-c.h>

#include <unity.h>

#include <string.h>

static const struct pkmn_pokemon empty_pokemon =
{
    .p_species = NULL,
    .game = PKMN_GAME_NONE,
    .p_internal = NULL
};

static void conversions_test(
    const char* species,
    enum pkmn_game origin_game,
    enum pkmn_game dest_game
)
{
    TEST_ASSERT_NOT_NULL(species);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int origin_generation = game_to_generation(origin_game);
    int dest_generation = game_to_generation(dest_game);
    int min_generation = (origin_generation < dest_generation) ? origin_generation
                                                               : dest_generation;

    struct pkmn_pokemon first_pokemon = empty_pokemon;
    struct pkmn_pokemon second_pokemon = empty_pokemon;

    get_random_pokemon(
        &first_pokemon,
        NULL, // p_item_list
        species,
        origin_game
    );
    TEST_ASSERT_NOT_NULL(first_pokemon.p_internal);

    error = pkmn_pokemon_to_game(
                &first_pokemon,
                dest_game,
                &second_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(second_pokemon.p_internal);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_strerror(&second_pokemon)
    );

    // Comparison

    TEST_ASSERT_EQUAL_STRING(first_pokemon.p_species, second_pokemon.p_species);
    TEST_ASSERT_EQUAL(dest_game, second_pokemon.game);

    // TODO: condition

    compare_pokemon_strings(
        &first_pokemon,
        &second_pokemon,
        "Form",
        pkmn_pokemon_get_form
    );
    compare_pokemon_strings(
        &first_pokemon,
        &second_pokemon,
        "Nickname",
        pkmn_pokemon_get_nickname
    );
    compare_pokemon_ints(
        &first_pokemon,
        &second_pokemon,
        "Experience",
        pkmn_pokemon_get_experience
    );
    compare_pokemon_ints(
        &first_pokemon,
        &second_pokemon,
        "Experience",
        pkmn_pokemon_get_level
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
            pkmn_pokemon_is_shiny
        );
        compare_pokemon_strings(
            &first_pokemon,
            &second_pokemon,
            "Held item",
            pkmn_pokemon_get_held_item
        );
        compare_pokemon_ints(
            &first_pokemon,
            &second_pokemon,
            "Current trainer friendship",
            pkmn_pokemon_get_current_trainer_friendship
        );

        int first_pokemon_level = 0;
        int second_pokemon_level_met = 0;

        error = pkmn_pokemon_get_level(
                    &first_pokemon,
                    &first_pokemon_level
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_get_level_met(
                    &second_pokemon,
                    &second_pokemon_level_met
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(first_pokemon_level, second_pokemon_level_met);
    }
    if(min_generation >= 3)
    {
        compare_pokemon_strings(
            &first_pokemon,
            &second_pokemon,
            "Ability",
            pkmn_pokemon_get_ability
        );
        compare_pokemon_strings(
            &first_pokemon,
            &second_pokemon,
            "Ball",
            pkmn_pokemon_get_ball
        );
        compare_pokemon_uint32s(
            &first_pokemon,
            &second_pokemon,
            "Personality",
            pkmn_pokemon_get_personality
        );
        compare_pokemon_ints(
            &first_pokemon,
            &second_pokemon,
            "Pokérus duration",
            pkmn_pokemon_get_pokerus_duration
        );

        if(origin_generation == dest_generation)
        {
            compare_pokemon_bool_buffers(
                &first_pokemon,
                &second_pokemon,
                PKMN_NUM_MARKINGS,
                "Markings",
                pkmn_pokemon_get_markings
            );
            compare_pokemon_int_buffers(
                &first_pokemon,
                &second_pokemon,
                PKMN_NUM_CONTEST_STATS,
                "Contest stats",
                pkmn_pokemon_get_contest_stats
            );
            compare_pokemon_ribbons(
                &first_pokemon,
                &second_pokemon
            );
        }
    }

    error = pkmn_pokemon_free(&second_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_pokemon_free(&first_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void gen1_conversions_test()
{
    // Generation I -> Generation I
    conversions_test("Bulbasaur", PKMN_GAME_RED, PKMN_GAME_YELLOW);

    // Generation I -> Generation II
    conversions_test("Squirtle", PKMN_GAME_BLUE, PKMN_GAME_GOLD);
}

void gen2_conversions_test()
{
    // Generation II -> Generation II
    conversions_test("Cyndaquil", PKMN_GAME_GOLD, PKMN_GAME_CRYSTAL);
    conversions_test("Totodile", PKMN_GAME_CRYSTAL, PKMN_GAME_GOLD);

    // Generation II -> Generation I
    conversions_test("Charmander", PKMN_GAME_SILVER, PKMN_GAME_BLUE);
}

void gba_conversions_test()
{
    // GBA -> GBA
    conversions_test("Torchic", PKMN_GAME_RUBY, PKMN_GAME_SAPPHIRE);
    conversions_test("Mudkip", PKMN_GAME_RUBY, PKMN_GAME_EMERALD);
    conversions_test("Treecko", PKMN_GAME_RUBY, PKMN_GAME_FIRERED);
    conversions_test("Torchic", PKMN_GAME_EMERALD, PKMN_GAME_SAPPHIRE);
    conversions_test("Mudkip", PKMN_GAME_EMERALD, PKMN_GAME_EMERALD);
    conversions_test("Treecko", PKMN_GAME_EMERALD, PKMN_GAME_FIRERED);
    conversions_test("Charmander", PKMN_GAME_FIRERED, PKMN_GAME_SAPPHIRE);
    conversions_test("Squirtle", PKMN_GAME_FIRERED, PKMN_GAME_EMERALD);
    conversions_test("Bulbasaur", PKMN_GAME_FIRERED, PKMN_GAME_FIRERED);

    // GBA -> GCN
    conversions_test("Eevee", PKMN_GAME_RUBY, PKMN_GAME_COLOSSEUM);
    conversions_test("Espeon", PKMN_GAME_EMERALD, PKMN_GAME_COLOSSEUM);
    conversions_test("Umbreon", PKMN_GAME_FIRERED, PKMN_GAME_COLOSSEUM);
    conversions_test("Eevee", PKMN_GAME_RUBY, PKMN_GAME_XD);
    conversions_test("Espeon", PKMN_GAME_EMERALD, PKMN_GAME_XD);
    conversions_test("Umbreon", PKMN_GAME_FIRERED, PKMN_GAME_XD);
}

void gcn_conversions_test()
{
    // GCN -> GBA
    conversions_test("Eevee", PKMN_GAME_COLOSSEUM, PKMN_GAME_SAPPHIRE);
    conversions_test("Espeon", PKMN_GAME_COLOSSEUM, PKMN_GAME_EMERALD);
    conversions_test("Umbreon", PKMN_GAME_COLOSSEUM, PKMN_GAME_LEAFGREEN);
    conversions_test("Eevee", PKMN_GAME_XD, PKMN_GAME_SAPPHIRE);
    conversions_test("Espeon", PKMN_GAME_XD, PKMN_GAME_EMERALD);
    conversions_test("Umbreon", PKMN_GAME_XD, PKMN_GAME_LEAFGREEN);

    // GCN -> GCN
    conversions_test("Vaporeon", PKMN_GAME_COLOSSEUM, PKMN_GAME_COLOSSEUM);
    conversions_test("Jolteon", PKMN_GAME_COLOSSEUM, PKMN_GAME_XD);
    conversions_test("Vaporeon", PKMN_GAME_XD, PKMN_GAME_XD);
    conversions_test("Jolteon", PKMN_GAME_XD, PKMN_GAME_COLOSSEUM);
}
