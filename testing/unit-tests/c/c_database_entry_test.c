/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn-c/database.h>

#include <string.h>

#define STRBUFFER_LEN 1024

static void assert_item_entry_uninitialized(
    struct pkmn_database_item_entry* p_item_entry
)
{
    TEST_ASSERT_EQUAL(PKMN_ITEM_NONE, p_item_entry->item);
    TEST_ASSERT_EQUAL(PKMN_GAME_NONE, p_item_entry->game);
    TEST_ASSERT_NULL(p_item_entry->p_category);
    TEST_ASSERT_NULL(p_item_entry->p_pocket);
    TEST_ASSERT_NULL(p_item_entry->p_description);
    TEST_ASSERT_EQUAL(p_item_entry->cost, 0);
    TEST_ASSERT_FALSE(p_item_entry->holdable);
    TEST_ASSERT_EQUAL(p_item_entry->fling_power, 0);
    TEST_ASSERT_NULL(p_item_entry->p_fling_effect);
}

static void item_entry_test()
{
    struct pkmn_database_item_entry item_entry =
    {
        .item = PKMN_ITEM_NONE,
        .game = PKMN_GAME_NONE,
        .p_category = NULL,
        .p_pocket = NULL,
        .p_description = NULL,
        .cost = 0,
        .holdable = false,
        .fling_power = 0,
        .p_fling_effect = NULL
    };

    /*
     * Make sure expected errors are returned and there is no effect on
     * the output upon failure.
     */
    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            PKMN_ITEM_INVALID,
            PKMN_GAME_SOULSILVER,
            &item_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_item_entry_uninitialized(
        &item_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            PKMN_ITEM_FAST_BALL,
            PKMN_GAME_NONE,
            &item_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_item_entry_uninitialized(
        &item_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            PKMN_ITEM_FAST_BALL,
            PKMN_GAME_RED,
            &item_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_item_entry_uninitialized(
        &item_entry
    );

    PKMN_TEST_ASSERT_SUCCESS(
        pkmn_database_get_item_entry(
            PKMN_ITEM_FAST_BALL,
            PKMN_GAME_SOULSILVER,
            &item_entry
        )
    );

    // Make sure fields are properly populated
    TEST_ASSERT_EQUAL(PKMN_ITEM_FAST_BALL, item_entry.item);
    TEST_ASSERT_EQUAL(PKMN_GAME_SOULSILVER, item_entry.game);
    TEST_ASSERT_EQUAL_STRING("Apricorn balls", item_entry.p_category);
    TEST_ASSERT_EQUAL_STRING("Poké Balls", item_entry.p_pocket);
    TEST_ASSERT_NOT_NULL(item_entry.p_description);
    TEST_ASSERT_EQUAL(300, item_entry.cost);
    TEST_ASSERT(item_entry.holdable);
    TEST_ASSERT_EQUAL(0, item_entry.fling_power);
    TEST_ASSERT_EQUAL_STRING("None", item_entry.p_fling_effect);

    // Make sure freeing properly works
    TEST_ASSERT_EQUAL(
        pkmn_database_item_entry_free(&item_entry),
        PKMN_ERROR_NONE
    );
    assert_item_entry_uninitialized(
        &item_entry
    );
}

static void assert_move_entry_uninitialized(
    struct pkmn_database_move_entry* p_move_entry
)
{
    TEST_ASSERT_NULL(p_move_entry->p_name);
    TEST_ASSERT_EQUAL(PKMN_GAME_NONE, p_move_entry->game);
    TEST_ASSERT_NULL(p_move_entry->p_description);
    TEST_ASSERT_NULL(p_move_entry->p_target);
    TEST_ASSERT_NULL(p_move_entry->p_damage_class);
    TEST_ASSERT_EQUAL(p_move_entry->base_power, 0);
    for(int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL(p_move_entry->pp[i], 0);
    }
    TEST_ASSERT_EQUAL_FLOAT(p_move_entry->accuracy, 0.0f);
    TEST_ASSERT_NULL(p_move_entry->p_effect);
    TEST_ASSERT_NULL(p_move_entry->p_contest_type);
    TEST_ASSERT_NULL(p_move_entry->p_contest_effect);
    TEST_ASSERT_NULL(p_move_entry->p_super_contest_effect);
}

static void move_entry_test()
{
    struct pkmn_database_move_entry move_entry =
    {
        .p_name = NULL,
        .game = PKMN_GAME_NONE,
        .p_description = NULL,
        .p_target = NULL,
        .p_damage_class = NULL,
        .base_power = 0,
        .pp = {0,0,0,0},
        .accuracy = 0.0f,
        .p_effect = NULL,
        .p_contest_type = NULL,
        .p_contest_effect = NULL,
        .p_super_contest_effect = NULL
    };

    /*
     * Make sure expected errors are returned and there is no effect on
     * the output upon failure.
     */
    TEST_ASSERT_EQUAL(
        pkmn_database_get_move_entry(
            "Not a move",
            PKMN_GAME_SILVER,
            &move_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_move_entry_uninitialized(
        &move_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_move_entry(
            "Octazooka",
            PKMN_GAME_NONE,
            &move_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_move_entry_uninitialized(
        &move_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_move_entry(
            "Octazooka",
            PKMN_GAME_RED,
            &move_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_move_entry_uninitialized(
        &move_entry
    );

    TEST_ASSERT_EQUAL(
        pkmn_database_get_move_entry(
            "Octazooka",
            PKMN_GAME_SILVER,
            &move_entry
        ),
        PKMN_ERROR_NONE
    );

    // Make sure fields are properly populated
    TEST_ASSERT_EQUAL_STRING(move_entry.p_name, "Octazooka");
    TEST_ASSERT_EQUAL(move_entry.game, PKMN_GAME_SILVER);
    TEST_ASSERT_NOT_NULL(move_entry.p_description);
    TEST_ASSERT_EQUAL_STRING(move_entry.p_target, "Selected Pokémon");
    TEST_ASSERT_EQUAL_STRING(move_entry.p_damage_class, "Special");
    TEST_ASSERT_EQUAL(move_entry.base_power, 65);
    TEST_ASSERT_EQUAL(move_entry.pp[0], 10);
    TEST_ASSERT_EQUAL(move_entry.pp[1], 12);
    TEST_ASSERT_EQUAL(move_entry.pp[2], 14);
    TEST_ASSERT_EQUAL(move_entry.pp[3], 16);
    TEST_ASSERT_EQUAL_FLOAT(move_entry.accuracy, 0.85f);
    TEST_ASSERT_NOT_NULL(move_entry.p_effect);
    TEST_ASSERT_EQUAL_STRING(move_entry.p_contest_type, "None");
    TEST_ASSERT_EQUAL_STRING(move_entry.p_contest_effect, "None");
    TEST_ASSERT_EQUAL_STRING(move_entry.p_super_contest_effect, "None");

    // Make sure freeing properly works
    TEST_ASSERT_EQUAL(
        pkmn_database_move_entry_free(&move_entry),
        PKMN_ERROR_NONE
    );
    assert_move_entry_uninitialized(
        &move_entry
    );
}

static void assert_pokemon_entry_uninitialized(
    struct pkmn_database_pokemon_entry* p_pokemon_entry
)
{
    TEST_ASSERT_EQUAL(PKMN_SPECIES_NONE, p_pokemon_entry->name);
    TEST_ASSERT_EQUAL(PKMN_GAME_NONE, p_pokemon_entry->game);
    TEST_ASSERT_NULL(p_pokemon_entry->p_species);
    TEST_ASSERT_NULL(p_pokemon_entry->p_pokedex_entry);
    TEST_ASSERT_NULL(p_pokemon_entry->p_form);
    TEST_ASSERT_EQUAL_FLOAT(p_pokemon_entry->height, 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(p_pokemon_entry->weight, 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(p_pokemon_entry->chance_male, 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(p_pokemon_entry->chance_female, 0.0f);
    TEST_ASSERT_FALSE(p_pokemon_entry->has_gender_differences);
    TEST_ASSERT_EQUAL(p_pokemon_entry->base_friendship, 0);
    TEST_ASSERT_NULL(p_pokemon_entry->types.p_first);
    TEST_ASSERT_NULL(p_pokemon_entry->types.p_second);
    TEST_ASSERT_NULL(p_pokemon_entry->abilities.p_first);
    TEST_ASSERT_NULL(p_pokemon_entry->abilities.p_second);
    TEST_ASSERT_NULL(p_pokemon_entry->p_hidden_ability);
    TEST_ASSERT_NULL(p_pokemon_entry->egg_groups.p_first);
    TEST_ASSERT_NULL(p_pokemon_entry->egg_groups.p_second);
    for(int i = 0; i < 7; ++i) {
        TEST_ASSERT_EQUAL(p_pokemon_entry->base_stats[i], 0);
        TEST_ASSERT_EQUAL(p_pokemon_entry->EV_yields[i], 0);
    }
    TEST_ASSERT_EQUAL(p_pokemon_entry->experience_yield, 0);
    TEST_ASSERT_NULL(p_pokemon_entry->levelup_moves.p_levelup_moves);
    TEST_ASSERT_EQUAL(p_pokemon_entry->levelup_moves.length, 0);
    TEST_ASSERT_NULL(p_pokemon_entry->tm_hm_moves.pp_strings);
    TEST_ASSERT_EQUAL(p_pokemon_entry->tm_hm_moves.length, 0);
    TEST_ASSERT_NULL(p_pokemon_entry->egg_moves.pp_strings);
    TEST_ASSERT_EQUAL(p_pokemon_entry->egg_moves.length, 0);
    TEST_ASSERT_NULL(p_pokemon_entry->tutor_moves.pp_strings);
    TEST_ASSERT_EQUAL(p_pokemon_entry->tutor_moves.length, 0);
    TEST_ASSERT_NULL(p_pokemon_entry->forms.pp_strings);
    TEST_ASSERT_EQUAL(p_pokemon_entry->forms.length, 0);
    TEST_ASSERT_NULL(p_pokemon_entry->evolutions.pp_strings);
    TEST_ASSERT_EQUAL(p_pokemon_entry->evolutions.length, 0);
}

static bool file_exists(
    const char* filepath
) {
    FILE* file = fopen(filepath, "rb");
    if(file) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

static void pokemon_entry_test() {
    struct pkmn_database_pokemon_entry pokemon_entry = {
        .name = PKMN_SPECIES_NONE,
        .game = PKMN_GAME_NONE,
        .p_species = NULL,
        .p_pokedex_entry = NULL,
        .p_form = NULL,
        .height = 0.0f,
        .weight = 0.0f,
        .chance_male = 0.0f,
        .chance_female = 0.0f,
        .has_gender_differences = false,
        .types = {
            .p_first = NULL,
            .p_second = NULL
        },
        .abilities = {
            .p_first = NULL,
            .p_second = NULL
        },
        .p_hidden_ability = NULL,
        .egg_groups = {
            .p_first = NULL,
            .p_second = NULL
        },
        .base_stats = {0,0,0,0,0,0,0},
        .EV_yields = {0,0,0,0,0,0,0},
        .experience_yield = 0,
        .levelup_moves = {
            .p_levelup_moves = NULL,
            .length = 0
        },
        .tm_hm_moves = {
            .pp_strings = NULL,
            .length = 0
        },
        .egg_moves = {
            .pp_strings = NULL,
            .length = 0
        },
        .tutor_moves = {
            .pp_strings = NULL,
            .length = 0
        },
        .forms = {
            .pp_strings = NULL,
            .length = 0
        },
        .evolutions = {
            .pp_strings = NULL,
            .length = 0
        }
    };

    /*
     * Make sure expected errors are returned and there is no effect on
     * the output upon failure.
     */
    TEST_ASSERT_EQUAL(
        pkmn_database_get_pokemon_entry(
            PKMN_SPECIES_INVALID,
            PKMN_GAME_BLACK2,
            "Sunny",
            &pokemon_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_pokemon_entry_uninitialized(
        &pokemon_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_pokemon_entry(
            PKMN_SPECIES_CASTFORM,
            PKMN_GAME_NONE,
            "Sunny",
            &pokemon_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_pokemon_entry_uninitialized(
        &pokemon_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_pokemon_entry(
            PKMN_SPECIES_INVALID,
            PKMN_GAME_BLACK2,
            "Sunny",
            &pokemon_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_pokemon_entry_uninitialized(
        &pokemon_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_pokemon_entry(
            PKMN_SPECIES_CASTFORM,
            PKMN_GAME_BLACK2,
            "Not a form",
            &pokemon_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_pokemon_entry_uninitialized(
        &pokemon_entry
    );

    TEST_ASSERT_EQUAL(
        pkmn_database_get_pokemon_entry(
            PKMN_SPECIES_STUNFISK,
            PKMN_GAME_BLACK2,
            "",
            &pokemon_entry
        ),
        PKMN_ERROR_NONE
    );

    TEST_ASSERT_EQUAL(PKMN_SPECIES_STUNFISK, pokemon_entry.name);
    TEST_ASSERT_EQUAL(PKMN_GAME_BLACK2, pokemon_entry.game);
    TEST_ASSERT_EQUAL_STRING("Standard", pokemon_entry.p_form);
    TEST_ASSERT_EQUAL_STRING("Trap", pokemon_entry.p_species);
    TEST_ASSERT_NOT_NULL(pokemon_entry.p_pokedex_entry);
    TEST_ASSERT_EQUAL_FLOAT(0.7f, pokemon_entry.height);
    TEST_ASSERT_EQUAL_FLOAT(11.0f, pokemon_entry.weight);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, pokemon_entry.chance_male);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, pokemon_entry.chance_female);
    TEST_ASSERT_FALSE(pokemon_entry.has_gender_differences);
    TEST_ASSERT_EQUAL(70, pokemon_entry.base_friendship);
    TEST_ASSERT_EQUAL_STRING("Ground", pokemon_entry.types.p_first);
    TEST_ASSERT_EQUAL_STRING("Electric", pokemon_entry.types.p_second);
    TEST_ASSERT_EQUAL_STRING("Static", pokemon_entry.abilities.p_first);
    TEST_ASSERT_EQUAL_STRING("Limber", pokemon_entry.abilities.p_second);
    TEST_ASSERT_EQUAL_STRING("Sand Veil", pokemon_entry.p_hidden_ability);
    TEST_ASSERT_EQUAL_STRING("Water 1", pokemon_entry.egg_groups.p_first);
    TEST_ASSERT_EQUAL_STRING("Amorphous", pokemon_entry.egg_groups.p_second);
    TEST_ASSERT_EQUAL(109, pokemon_entry.base_stats[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(66, pokemon_entry.base_stats[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(84, pokemon_entry.base_stats[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(32, pokemon_entry.base_stats[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, pokemon_entry.base_stats[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(81, pokemon_entry.base_stats[PKMN_STAT_SPATK]);
    TEST_ASSERT_EQUAL(99, pokemon_entry.base_stats[PKMN_STAT_SPDEF]);
    TEST_ASSERT_EQUAL(2, pokemon_entry.EV_yields[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(0, pokemon_entry.EV_yields[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(0, pokemon_entry.EV_yields[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(0, pokemon_entry.EV_yields[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, pokemon_entry.EV_yields[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(0, pokemon_entry.EV_yields[PKMN_STAT_SPATK]);
    TEST_ASSERT_EQUAL(0, pokemon_entry.EV_yields[PKMN_STAT_SPDEF]);
    TEST_ASSERT_EQUAL(165, pokemon_entry.experience_yield);

    int experience = 0;
    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_experience_at_level(
            &pokemon_entry,
            50,
            &experience
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(experience, 125000);

    int level = 0;
    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_level_at_experience(
            &pokemon_entry,
            200000,
            &level
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL(level, 58);

    char icon_filepath[STRBUFFER_LEN] = {0};
    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_icon_filepath(
            &pokemon_entry,
            false,
            icon_filepath,
            STRBUFFER_LEN,
            NULL
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT(strlen(icon_filepath) > 0);
    TEST_ASSERT(file_exists(icon_filepath));

    char sprite_filepath[STRBUFFER_LEN] = {0};
    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_sprite_filepath(
            &pokemon_entry,
            false,
            false,
            sprite_filepath,
            STRBUFFER_LEN,
            NULL
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT(strlen(sprite_filepath) > 0);
    TEST_ASSERT(file_exists(sprite_filepath));

    TEST_ASSERT_NOT_NULL(pokemon_entry.levelup_moves.p_levelup_moves);
    TEST_ASSERT(pokemon_entry.levelup_moves.length > 0);
    TEST_ASSERT_NOT_NULL(pokemon_entry.tm_hm_moves.pp_strings);
    TEST_ASSERT(pokemon_entry.tm_hm_moves.length > 0);
    TEST_ASSERT_NOT_NULL(pokemon_entry.egg_moves.pp_strings);
    TEST_ASSERT(pokemon_entry.egg_moves.length > 0);
    TEST_ASSERT_NOT_NULL(pokemon_entry.tutor_moves.pp_strings);
    TEST_ASSERT(pokemon_entry.tutor_moves.length > 0);
    TEST_ASSERT_NOT_NULL(pokemon_entry.forms.pp_strings);
    TEST_ASSERT(pokemon_entry.forms.length > 0);
    // No evolutions, so no memory allocated
    TEST_ASSERT_EQUAL(pokemon_entry.evolutions.length, 0);

    // Make sure freeing properly works
    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_free(&pokemon_entry),
        PKMN_ERROR_NONE
    );
    assert_pokemon_entry_uninitialized(
        &pokemon_entry
    );

    // Test failing to set form
    TEST_ASSERT_EQUAL(
        pkmn_database_get_pokemon_entry(
            PKMN_SPECIES_VENUSAUR,
            PKMN_GAME_RUBY,
            "",
            &pokemon_entry
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.p_form, "Standard");

    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_set_form(
            &pokemon_entry,
            "Mega"
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.p_form, "Standard");

    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_free(&pokemon_entry),
        PKMN_ERROR_NONE
    );
    assert_pokemon_entry_uninitialized(
        &pokemon_entry
    );

    // Test setting form
    TEST_ASSERT_EQUAL(
        pkmn_database_get_pokemon_entry(
            PKMN_SPECIES_VENUSAUR,
            PKMN_GAME_OMEGA_RUBY,
            "",
            &pokemon_entry
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.p_form, "Standard");

    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_set_form(
            &pokemon_entry,
            "Mega"
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.p_form, "Mega");

    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_free(&pokemon_entry),
        PKMN_ERROR_NONE
    );
    assert_pokemon_entry_uninitialized(
        &pokemon_entry
    );
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(item_entry_test)
    PKMN_C_TEST(move_entry_test)
    PKMN_C_TEST(pokemon_entry_test)
)
