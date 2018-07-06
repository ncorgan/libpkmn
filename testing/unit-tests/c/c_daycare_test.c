/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn-c.h>

static const struct pkmn_pokemon empty_pokemon =
{
    .species = PKMN_SPECIES_NONE,
    .game = PKMN_GAME_NONE,
    .p_internal = NULL
};
static const struct pkmn_pokemon_list empty_pokemon_list =
{
    .p_pokemon = NULL,
    .length = 0
};

static void test_empty_daycare(
    const struct pkmn_daycare* p_daycare
)
{
    TEST_ASSERT_NOT_NULL(p_daycare);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_pokemon pokemon = empty_pokemon;
    struct pkmn_pokemon_list pokemon_list = empty_pokemon_list;

    // Levelup Pokémon

    // Test invalid indices.
    error = pkmn_daycare_get_levelup_pokemon(
                p_daycare,
                -1,
                &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    for(size_t levelup_pokemon_index = 0;
        levelup_pokemon_index < p_daycare->levelup_pokemon_capacity;
        ++levelup_pokemon_index)
    {
        error = pkmn_daycare_get_levelup_pokemon(
                    p_daycare,
                    levelup_pokemon_index,
                    &pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon.p_internal);

        TEST_ASSERT_EQUAL(PKMN_SPECIES_NONE, pokemon.species);
        TEST_ASSERT_EQUAL(p_daycare->game, pokemon.game);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(pokemon.p_internal);
    }

    error = pkmn_daycare_get_levelup_pokemon_as_list(
                p_daycare,
                &pokemon_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon_list.p_pokemon);
    TEST_ASSERT_EQUAL(
        p_daycare->levelup_pokemon_capacity,
        pokemon_list.length
    );

    for(size_t levelup_pokemon_index = 0;
        levelup_pokemon_index < pokemon_list.length;
        ++levelup_pokemon_index)
    {
        TEST_ASSERT_EQUAL(
            PKMN_SPECIES_NONE,
            pokemon_list.p_pokemon[levelup_pokemon_index].species
        );
        TEST_ASSERT_EQUAL(
            p_daycare->game,
            pokemon_list.p_pokemon[levelup_pokemon_index].game
        );
    }

    error = pkmn_pokemon_list_free(&pokemon_list);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(pokemon_list.p_pokemon);
    TEST_ASSERT_EQUAL(0, pokemon_list.length);

    // Breeding Pokémon

    error = pkmn_daycare_get_egg(
                p_daycare,
                &pokemon
            );

    if(p_daycare->can_breed_pokemon)
    {
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon.p_internal);

        bool is_egg = false;
        error = pkmn_pokemon_is_egg(
                    &pokemon,
                    &is_egg
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(is_egg);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(pokemon.p_internal);

        // Test invalid indices.
        error = pkmn_daycare_get_breeding_pokemon(
                    p_daycare,
                    -1,
                    &pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

        for(size_t breeding_pokemon_index = 0;
            breeding_pokemon_index < p_daycare->breeding_pokemon_capacity;
            ++breeding_pokemon_index)
        {
            struct pkmn_pokemon pokemon = empty_pokemon;

            error = pkmn_daycare_get_breeding_pokemon(
                        p_daycare,
                        breeding_pokemon_index,
                        &pokemon
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            TEST_ASSERT_EQUAL(PKMN_SPECIES_NONE, pokemon.species);
            TEST_ASSERT_EQUAL(p_daycare->game, pokemon.game);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }

        error = pkmn_daycare_get_breeding_pokemon_as_list(
                    p_daycare,
                    &pokemon_list
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon_list.p_pokemon);
        TEST_ASSERT_EQUAL(
            p_daycare->breeding_pokemon_capacity,
            pokemon_list.length
        );

        for(size_t breeding_pokemon_index = 0;
            breeding_pokemon_index < pokemon_list.length;
            ++breeding_pokemon_index)
        {
            TEST_ASSERT_EQUAL(
                PKMN_SPECIES_NONE,
                pokemon_list.p_pokemon[breeding_pokemon_index].species
            );
            TEST_ASSERT_EQUAL(
                p_daycare->game,
                pokemon_list.p_pokemon[breeding_pokemon_index].game
            );
        }

        error = pkmn_pokemon_list_free(&pokemon_list);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(pokemon_list.p_pokemon);
        TEST_ASSERT_EQUAL(0, pokemon_list.length);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_pokemon_in_daycare(
    const struct pkmn_daycare* p_daycare
)
{
    TEST_ASSERT_NOT_NULL(p_daycare);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_pokemon daycare_pokemon = empty_pokemon;

    struct pkmn_pokemon venusaur = empty_pokemon;
    error = pkmn_pokemon_init(
                PKMN_SPECIES_VENUSAUR,
                p_daycare->game,
                "",
                50,
                &venusaur
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(p_daycare->p_internal);
    TEST_ASSERT_EQUAL(PKMN_SPECIES_VENUSAUR, venusaur.species);

    error = pkmn_daycare_set_levelup_pokemon(
                p_daycare,
                0,
                &venusaur
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_daycare_get_levelup_pokemon(
                p_daycare,
                0,
                &daycare_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(p_daycare->p_internal);
    TEST_ASSERT_EQUAL(PKMN_SPECIES_VENUSAUR, daycare_pokemon.species);

    error = pkmn_pokemon_free(&daycare_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(daycare_pokemon.p_internal);

    error = pkmn_pokemon_free(&venusaur);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(venusaur.p_internal);

    if(p_daycare->can_breed_pokemon)
    {
        // TODO: validate genders
        struct pkmn_pokemon blastoise = empty_pokemon;
        struct pkmn_pokemon marowak = empty_pokemon;

        enum pkmn_gender pokemon_gender = PKMN_GENDER_GENDERLESS;

        error = pkmn_pokemon_init(
                    PKMN_SPECIES_BLASTOISE,
                    p_daycare->game,
                    "",
                    50,
                    &blastoise
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(blastoise.p_internal);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_BLASTOISE, blastoise.species);

        error = pkmn_pokemon_set_gender(
                    &blastoise,
                    PKMN_GENDER_FEMALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_init(
                    PKMN_SPECIES_MAROWAK,
                    p_daycare->game,
                    "",
                    50,
                    &marowak
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(marowak.p_internal);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_MAROWAK, marowak.species);

        error = pkmn_pokemon_set_gender(
                    &marowak,
                    PKMN_GENDER_MALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_daycare_set_breeding_pokemon(
                    p_daycare,
                    0,
                    &blastoise
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_daycare_set_breeding_pokemon(
                    p_daycare,
                    1,
                    &marowak
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_free(&marowak);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(marowak.p_internal);

        error = pkmn_pokemon_free(&blastoise);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(blastoise.p_internal);

        // Check first Pokémon.

        error = pkmn_daycare_get_breeding_pokemon(
                    p_daycare,
                    0,
                    &daycare_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(daycare_pokemon.p_internal);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_BLASTOISE, daycare_pokemon.species);

        error = pkmn_pokemon_get_gender(
                    &daycare_pokemon,
                    &pokemon_gender
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, pokemon_gender);

        error = pkmn_pokemon_free(&daycare_pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(daycare_pokemon.p_internal);

        // Check second Pokémon.

        error = pkmn_daycare_get_breeding_pokemon(
                    p_daycare,
                    1,
                    &daycare_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(daycare_pokemon.p_internal);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_MAROWAK, daycare_pokemon.species);

        error = pkmn_pokemon_get_gender(
                    &daycare_pokemon,
                    &pokemon_gender
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, pokemon_gender);

        error = pkmn_pokemon_free(&daycare_pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(daycare_pokemon.p_internal);
    }
}

static void daycare_test_common(
    enum pkmn_game game,
    size_t expected_levelup_capacity,
    size_t expected_breeding_capacity
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_daycare daycare =
    {
        .game = PKMN_GAME_NONE,

        .can_breed_pokemon = false,

        .levelup_pokemon_capacity = 0,
        .breeding_pokemon_capacity = 0,

        .p_internal = NULL
    };

    error = pkmn_daycare_init(
                game,
                &daycare
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(daycare.p_internal);
    TEST_ASSERT_EQUAL(game, daycare.game);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_daycare_strerror(&daycare)
    );

    const bool expected_can_breed_pokemon = (expected_breeding_capacity > 0);

    TEST_ASSERT_EQUAL(
        expected_can_breed_pokemon,
        daycare.can_breed_pokemon
    );
    TEST_ASSERT_EQUAL(
        expected_levelup_capacity,
        daycare.levelup_pokemon_capacity
    );
    TEST_ASSERT_EQUAL(
        expected_breeding_capacity,
        daycare.breeding_pokemon_capacity
    );

    test_empty_daycare(&daycare);
    test_setting_pokemon_in_daycare(&daycare);

    error = pkmn_daycare_free(&daycare);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GAME_NONE, daycare.game);
    TEST_ASSERT_FALSE(daycare.can_breed_pokemon);
    TEST_ASSERT_EQUAL(0, daycare.levelup_pokemon_capacity);
    TEST_ASSERT_EQUAL(0, daycare.breeding_pokemon_capacity);
    TEST_ASSERT_NULL(daycare.p_internal);
}

// Red

static void red_daycare_test()
{
    daycare_test_common(PKMN_GAME_RED, 1, 0);
}

// Blue

static void blue_daycare_test()
{
    daycare_test_common(PKMN_GAME_BLUE, 1, 0);
}

// Yellow

static void yellow_daycare_test()
{
    daycare_test_common(PKMN_GAME_YELLOW, 1, 0);
}

// Gold

static void gold_daycare_test()
{
    daycare_test_common(PKMN_GAME_GOLD, 2, 2);
}

// Silver

static void silver_daycare_test()
{
    daycare_test_common(PKMN_GAME_SILVER, 2, 2);
}

// Crystal

static void crystal_daycare_test()
{
    daycare_test_common(PKMN_GAME_CRYSTAL, 2, 2);
}

/*
// Ruby

static void ruby_daycare_test()
{
    daycare_test_common("Ruby", 2, 2);
}

// Sapphire

static void sapphire_daycare_test()
{
    daycare_test_common("Sapphire", 2, 2);
}

// Emerald

static void emerald_daycare_test()
{
    daycare_test_common("Emerald", 2, 2);
}

// FireRed

static void firered_daycare_test()
{
    daycare_test_common("FireRed", 2, 2);
}

// LeafGreen

static void leafgreen_daycare_test()
{
    daycare_test_common("LeafGreen", 2, 2);
}
*/

// Colosseum

static void colosseum_daycare_test()
{
    daycare_test_common(PKMN_GAME_COLOSSEUM, 1, 0);
}

// XD

static void xd_daycare_test()
{
    daycare_test_common(PKMN_GAME_XD, 1, 0);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(red_daycare_test)
    PKMN_C_TEST(blue_daycare_test)
    PKMN_C_TEST(yellow_daycare_test)

    PKMN_C_TEST(gold_daycare_test)
    PKMN_C_TEST(silver_daycare_test)
    PKMN_C_TEST(crystal_daycare_test)

    /*
    PKMN_C_TEST(ruby_daycare_test)
    PKMN_C_TEST(sapphire_daycare_test)
    PKMN_C_TEST(emerald_daycare_test)
    PKMN_C_TEST(firered_daycare_test)
    PKMN_C_TEST(leafgreen_daycare_test)
    */

    PKMN_C_TEST(colosseum_daycare_test)
    PKMN_C_TEST(xd_daycare_test)
)
