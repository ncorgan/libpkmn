/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <unity.h>

#include <pkmntest-c/util.h>

#include <pkmn-c.h>

#include <string.h>

static void test_gen1_pokemon_forms(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon =
    {
        .species = NULL,
        .game = NULL,
        ._internal = NULL
    };

    int generation = game_to_generation(game);

    // Check that Mega forms only work in their given games.
    static const char* gen1_pokemon_with_xy_mega_forms[] =
    {
        "Venusaur", "Blastoise", "Alakazam", "Gengar", "Kangaskhan", "Pinsir", "Gyarados", "Aerodactyl",
        NULL
    };
    for(size_t pokemon_index = 0;
        gen1_pokemon_with_xy_mega_forms[pokemon_index];
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen1_pokemon_with_xy_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
    }
    static const char* gen1_pokemon_with_oras_mega_forms[] =
    {
        "Beedrill", "Pidgeot", "Slowbro",
        NULL
    };
    for(size_t pokemon_index = 0;
        gen1_pokemon_with_oras_mega_forms[pokemon_index];
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen1_pokemon_with_oras_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire"))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
    }

    // Cosplay Pikachu should only work for OR/AS.
    struct pkmn_database_pokemon_entry oras_pikachu_entry;
    error = pkmn_database_get_pokemon_entry(
                "Pikachu",
                "Omega Ruby",
                "",
                &oras_pikachu_entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT(oras_pikachu_entry.forms.length > 0);

    for(size_t form_index = 1; form_index < oras_pikachu_entry.forms.length; ++form_index)
    {
        TEST_ASSERT_NOT_NULL(oras_pikachu_entry.forms.strings[form_index]);

        error = pkmn_pokemon_init(
                    "Pikachu",
                    game,
                    oras_pikachu_entry.forms.strings[form_index],
                    5,
                    &pokemon
                );
        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire"))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
    }

    error = pkmn_database_pokemon_entry_free(&oras_pikachu_entry);
    PKMN_TEST_ASSERT_SUCCESS(error);

    static const char* gen1_pokemon_with_two_mega_forms[] =
    {
        "Charizard", "Mewtwo",
        NULL
    };
    for(size_t pokemon_index = 0;
        gen1_pokemon_with_two_mega_forms[pokemon_index];
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen1_pokemon_with_two_mega_forms[pokemon_index],
                    game,
                    "Mega X",
                    50,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon._internal);
        }

        error = pkmn_pokemon_init(
                    gen1_pokemon_with_two_mega_forms[pokemon_index],
                    game,
                    "Mega Y",
                    50,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
    }
}

static void test_gen2_pokemon_forms(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon =
    {
        .species = NULL,
        .game = NULL,
        ._internal = NULL
    };

    int generation = game_to_generation(game);

    // Check that Mega forms only work in their given games.
    static const char* gen2_pokemon_with_xy_mega_forms[] =
    {
        "Ampharos", "Scizor", "Heracross", "Houndoom", "Tyranitar",
        NULL
    };
    for(size_t pokemon_index = 0;
        gen2_pokemon_with_xy_mega_forms[pokemon_index];
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen2_pokemon_with_xy_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon._internal);
        }
    }

    error = pkmn_pokemon_init(
                "Steelix",
                game,
                "Mega",
                100,
                &pokemon
            );
    if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire"))
    {
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon._internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon._internal);

    // Spiky-eared Pichu should only work in HG/SS.
    error = pkmn_pokemon_init(
                "Pichu",
                game,
                "Spiky-eared",
                100,
                &pokemon
            );
    if(!strcmp(game, "HeartGold") || !strcmp(game, "SoulSilver"))
    {
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon._internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon._internal);

    // Unown's "!" and "?" forms aren't in Generation II.
    struct pkmn_database_pokemon_entry unown_entry;
    error = pkmn_database_get_pokemon_entry(
                "Unown",
                "Omega Ruby",
                "",
                &unown_entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(28, unown_entry.forms.length);
    for(size_t i = 0; i < 26; ++i)
    {
        TEST_ASSERT_NOT_NULL(unown_entry.forms.strings[i]);
        error = pkmn_pokemon_init(
                    "Unown",
                    game,
                    unown_entry.forms.strings[i],
                    10,
                    &pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon._internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(pokemon._internal);
    }

    error = pkmn_database_pokemon_entry_free(&unown_entry);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_pokemon_init(
                "Unown",
                game,
                "!",
                10,
                &pokemon
            );
    if(generation >= 3)
    {
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon._internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon._internal);

    error = pkmn_pokemon_init(
                "Unown",
                game,
                "?",
                10,
                &pokemon
            );
    if(generation >= 3)
    {
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon._internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon._internal);
}

static void test_gen3_pokemon_forms(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon =
    {
        .species = NULL,
        .game = NULL,
        ._internal = NULL
    };

    int generation = game_to_generation(game);

    // Check that Mega forms only work in their given games.
    static const char* gen3_pokemon_with_xy_mega_forms[] =
    {
        "Blaziken", "Gardevoir", "Mawile", "Aggron", "Medicham",
        "Manectric", "Banette", "Absol", "Latias", "Latios", "Garchomp",
        NULL
    };
    for(size_t pokemon_index = 0;
        gen3_pokemon_with_xy_mega_forms[pokemon_index];
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen3_pokemon_with_xy_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon._internal);
    }
    static const char* gen3_pokemon_with_oras_mega_forms[] =
    {
        "Sceptile", "Swampert", "Sableye", "Sharpedo", "Camerupt",
        "Altaria", "Glalie", "Salamence", "Metagross", "Rayquaza",
        NULL
    };
    for(size_t pokemon_index = 0;
        gen3_pokemon_with_oras_mega_forms[pokemon_index];
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen3_pokemon_with_oras_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire"))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon._internal);
    }

    // Castform should always work.
    struct pkmn_database_pokemon_entry castform_entry;
    error = pkmn_database_get_pokemon_entry(
                "Castform",
                "Omega Ruby",
                "",
                &castform_entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(4, castform_entry.forms.length);
    for(size_t form_index = 0; form_index < 4; ++form_index)
    {
        TEST_ASSERT_NOT_NULL(castform_entry.forms.strings[form_index]);
        error = pkmn_pokemon_init(
                    "Castform",
                    game,
                    castform_entry.forms.strings[form_index],
                    30,
                    &pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon._internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(pokemon._internal);
    }

    error = pkmn_database_pokemon_entry_free(&castform_entry);
    PKMN_TEST_ASSERT_SUCCESS(error);

    // Primal Reversion forms should only work in OR/AS.
    static const char* primal_version_pokemon[] = {"Groudon", "Kyogre"};
    for(size_t pokemon_index = 0; pokemon_index < 2; ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    primal_version_pokemon[pokemon_index],
                    game,
                    "Standard",
                    70,
                    &pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon._internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_init(
                    primal_version_pokemon[pokemon_index],
                    game,
                    "Primal Reversion",
                    70,
                    &pokemon
                );
        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire"))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon._internal);
    }

    // In Generation III, Deoxys's form is game-specific.
    if(generation == 3)
    {
        error = pkmn_pokemon_init(
                    "Deoxys",
                    game,
                    "Normal",
                    70,
                    &pokemon
                );
        if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire") ||
           !strcmp(game, "Colosseum") || !strcmp(game, "XD"))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon._internal);

        error = pkmn_pokemon_init(
                    "Deoxys",
                    game,
                    "Attack",
                    70,
                    &pokemon
                );
        if(!strcmp(game, "FireRed"))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon._internal);

        error = pkmn_pokemon_init(
                    "Deoxys",
                    game,
                    "Defense",
                    70,
                    &pokemon
                );
        if(!strcmp(game, "LeafGreen"))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon._internal);

        error = pkmn_pokemon_init(
                    "Deoxys",
                    game,
                    "Speed",
                    70,
                    &pokemon
                );
        if(!strcmp(game, "Emerald"))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon._internal);
    }
    else
    {
        struct pkmn_database_pokemon_entry deoxys_entry;
        error = pkmn_database_get_pokemon_entry(
                    "Deoxys",
                    game,
                    "",
                    &deoxys_entry
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(4, deoxys_entry.forms.length);

        for(size_t form_index = 0; form_index < 4; ++form_index)
        {
            TEST_ASSERT_NOT_NULL(deoxys_entry.forms.strings[form_index]);

            error = pkmn_pokemon_init(
                        "Deoxys",
                        game,
                        deoxys_entry.forms.strings[form_index],
                        70,
                        &pokemon
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon._internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon._internal);
        }

        error = pkmn_database_pokemon_entry_free(&deoxys_entry);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
}

void red_form_test()
{
    test_gen1_pokemon_forms("Red");
}

void blue_form_test()
{
    test_gen1_pokemon_forms("Blue");
}

void yellow_form_test()
{
    test_gen1_pokemon_forms("Yellow");
}

void gold_form_test()
{
    test_gen1_pokemon_forms("Gold");
    test_gen2_pokemon_forms("Gold");
}

void silver_form_test()
{
    test_gen1_pokemon_forms("Silver");
    test_gen2_pokemon_forms("Silver");
}

void crystal_form_test()
{
    test_gen1_pokemon_forms("Crystal");
    test_gen2_pokemon_forms("Crystal");
}

void ruby_form_test()
{
    test_gen1_pokemon_forms("Ruby");
    test_gen2_pokemon_forms("Ruby");
    test_gen3_pokemon_forms("Ruby");
}

void sapphire_form_test()
{
    test_gen1_pokemon_forms("Sapphire");
    test_gen2_pokemon_forms("Sapphire");
    test_gen3_pokemon_forms("Sapphire");
}

void emerald_form_test()
{
    test_gen1_pokemon_forms("Emerald");
    test_gen2_pokemon_forms("Emerald");
    test_gen3_pokemon_forms("Emerald");
}

void firered_form_test()
{
    test_gen1_pokemon_forms("FireRed");
    test_gen2_pokemon_forms("FireRed");
    test_gen3_pokemon_forms("FireRed");
}

void leafgreen_form_test()
{
    test_gen1_pokemon_forms("LeafGreen");
    test_gen2_pokemon_forms("LeafGreen");
    test_gen3_pokemon_forms("LeafGreen");
}

void colosseum_form_test()
{
    test_gen1_pokemon_forms("Colosseum");
    test_gen2_pokemon_forms("Colosseum");
    test_gen3_pokemon_forms("Colosseum");
}

void xd_form_test()
{
    test_gen1_pokemon_forms("XD");
    test_gen2_pokemon_forms("XD");
    test_gen3_pokemon_forms("XD");
}
