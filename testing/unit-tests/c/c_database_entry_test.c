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
    struct pkmn_database_item_entry* item_entry
) {
    TEST_ASSERT_NULL(item_entry->name);
    TEST_ASSERT_NULL(item_entry->game);
    TEST_ASSERT_NULL(item_entry->category);
    TEST_ASSERT_NULL(item_entry->pocket);
    TEST_ASSERT_NULL(item_entry->description);
    TEST_ASSERT_EQUAL(item_entry->cost, 0);
    TEST_ASSERT_FALSE(item_entry->holdable);
    TEST_ASSERT_EQUAL(item_entry->fling_power, 0);
    TEST_ASSERT_NULL(item_entry->fling_effect);
}

static void item_entry_test() {
    struct pkmn_database_item_entry item_entry = {
        .name = NULL,
        .game = NULL,
        .category = NULL,
        .pocket = NULL,
        .description = NULL,
        .cost = 0,
        .holdable = false,
        .fling_power = 0,
        .fling_effect = NULL
    };

    /*
     * Make sure expected errors are returned and there is no effect on
     * the output upon failure.
     */
    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            "Not an item",
            "SoulSilver",
            &item_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_item_entry_uninitialized(
        &item_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            "Fast Ball",
            "Not a game",
            &item_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_item_entry_uninitialized(
        &item_entry
    );
    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            "Fast Ball",
            "Red",
            &item_entry
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    assert_item_entry_uninitialized(
        &item_entry
    );

    TEST_ASSERT_EQUAL(
        pkmn_database_get_item_entry(
            "Fast Ball",
            "SoulSilver",
            &item_entry
        ),
        PKMN_ERROR_NONE
    );

    // Make sure fields are properly populated
    TEST_ASSERT_EQUAL_STRING(item_entry.name, "Fast Ball");
    TEST_ASSERT_EQUAL_STRING(item_entry.game, "SoulSilver");
    TEST_ASSERT_EQUAL_STRING(item_entry.category, "Apricorn balls");
    TEST_ASSERT_EQUAL_STRING(item_entry.pocket, "Poké Balls");
    TEST_ASSERT_NOT_NULL(item_entry.description);
    TEST_ASSERT_EQUAL(item_entry.cost, 300);
    TEST_ASSERT(item_entry.holdable);
    TEST_ASSERT_EQUAL(item_entry.fling_power, 0);
    TEST_ASSERT_EQUAL_STRING(item_entry.fling_effect, "None");

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
    struct pkmn_database_move_entry* move_entry
) {
    TEST_ASSERT_NULL(move_entry->name);
    TEST_ASSERT_NULL(move_entry->game);
    TEST_ASSERT_NULL(move_entry->description);
    TEST_ASSERT_NULL(move_entry->target);
    TEST_ASSERT_NULL(move_entry->damage_class);
    TEST_ASSERT_EQUAL(move_entry->base_power, 0);
    for(int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL(move_entry->pp[i], 0);
    }
    TEST_ASSERT_EQUAL_FLOAT(move_entry->accuracy, 0.0f);
    TEST_ASSERT_NULL(move_entry->effect);
    TEST_ASSERT_NULL(move_entry->contest_type);
    TEST_ASSERT_NULL(move_entry->contest_effect);
    TEST_ASSERT_NULL(move_entry->super_contest_effect);
}

static void move_entry_test() {
    struct pkmn_database_move_entry move_entry = {
        .name = NULL,
        .game = NULL,
        .description = NULL,
        .target = NULL,
        .damage_class = NULL,
        .base_power = 0,
        .pp = {0,0,0,0},
        .accuracy = 0.0f,
        .effect = NULL,
        .contest_type = NULL,
        .contest_effect = NULL,
        .super_contest_effect = NULL
    };

    /*
     * Make sure expected errors are returned and there is no effect on
     * the output upon failure.
     */
    TEST_ASSERT_EQUAL(
        pkmn_database_get_move_entry(
            "Not a move",
            "Silver",
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
            "Not a game",
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
            "Red",
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
            "Silver",
            &move_entry
        ),
        PKMN_ERROR_NONE
    );

    // Make sure fields are properly populated
    TEST_ASSERT_EQUAL_STRING(move_entry.name, "Octazooka");
    TEST_ASSERT_EQUAL_STRING(move_entry.game, "Silver");
    TEST_ASSERT_NOT_NULL(move_entry.description);
    TEST_ASSERT_EQUAL_STRING(move_entry.target, "Selected Pokémon");
    TEST_ASSERT_EQUAL_STRING(move_entry.damage_class, "Special");
    TEST_ASSERT_EQUAL(move_entry.base_power, 65);
    TEST_ASSERT_EQUAL(move_entry.pp[0], 10);
    TEST_ASSERT_EQUAL(move_entry.pp[1], 12);
    TEST_ASSERT_EQUAL(move_entry.pp[2], 14);
    TEST_ASSERT_EQUAL(move_entry.pp[3], 16);
    TEST_ASSERT_EQUAL_FLOAT(move_entry.accuracy, 0.85f);
    TEST_ASSERT_NOT_NULL(move_entry.effect);
    TEST_ASSERT_EQUAL_STRING(move_entry.contest_type, "None");
    TEST_ASSERT_EQUAL_STRING(move_entry.contest_effect, "None");
    TEST_ASSERT_EQUAL_STRING(move_entry.super_contest_effect, "None");

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
    struct pkmn_database_pokemon_entry* pokemon_entry
) {
    TEST_ASSERT_NULL(pokemon_entry->name);
    TEST_ASSERT_NULL(pokemon_entry->game);
    TEST_ASSERT_NULL(pokemon_entry->species);
    TEST_ASSERT_NULL(pokemon_entry->pokedex_entry);
    TEST_ASSERT_NULL(pokemon_entry->form);
    TEST_ASSERT_EQUAL_FLOAT(pokemon_entry->height, 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(pokemon_entry->weight, 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(pokemon_entry->chance_male, 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(pokemon_entry->chance_female, 0.0f);
    TEST_ASSERT_FALSE(pokemon_entry->has_gender_differences);
    TEST_ASSERT_EQUAL(pokemon_entry->base_friendship, 0);
    TEST_ASSERT_NULL(pokemon_entry->types.first);
    TEST_ASSERT_NULL(pokemon_entry->types.second);
    TEST_ASSERT_NULL(pokemon_entry->abilities.first);
    TEST_ASSERT_NULL(pokemon_entry->abilities.second);
    TEST_ASSERT_NULL(pokemon_entry->hidden_ability);
    TEST_ASSERT_NULL(pokemon_entry->egg_groups.first);
    TEST_ASSERT_NULL(pokemon_entry->egg_groups.second);
    for(int i = 0; i < 7; ++i) {
        TEST_ASSERT_EQUAL(pokemon_entry->base_stats[i], 0);
        TEST_ASSERT_EQUAL(pokemon_entry->EV_yields[i], 0);
    }
    TEST_ASSERT_EQUAL(pokemon_entry->experience_yield, 0);
    TEST_ASSERT_NULL(pokemon_entry->levelup_moves.levelup_moves);
    TEST_ASSERT_EQUAL(pokemon_entry->levelup_moves.length, 0);
    TEST_ASSERT_NULL(pokemon_entry->tm_hm_moves.strings);
    TEST_ASSERT_EQUAL(pokemon_entry->tm_hm_moves.length, 0);
    TEST_ASSERT_NULL(pokemon_entry->egg_moves.strings);
    TEST_ASSERT_EQUAL(pokemon_entry->egg_moves.length, 0);
    TEST_ASSERT_NULL(pokemon_entry->tutor_moves.strings);
    TEST_ASSERT_EQUAL(pokemon_entry->tutor_moves.length, 0);
    TEST_ASSERT_NULL(pokemon_entry->forms.strings);
    TEST_ASSERT_EQUAL(pokemon_entry->forms.length, 0);
    TEST_ASSERT_NULL(pokemon_entry->evolutions.strings);
    TEST_ASSERT_EQUAL(pokemon_entry->evolutions.length, 0);
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
        .name = NULL,
        .game = NULL,
        .species = NULL,
        .pokedex_entry = NULL,
        .form = NULL,
        .height = 0.0f,
        .weight = 0.0f,
        .chance_male = 0.0f,
        .chance_female = 0.0f,
        .has_gender_differences = false,
        .types = {
            .first = NULL,
            .second = NULL
        },
        .abilities = {
            .first = NULL,
            .second = NULL
        },
        .hidden_ability = NULL,
        .egg_groups = {
            .first = NULL,
            .second = NULL
        },
        .base_stats = {0,0,0,0,0,0,0},
        .EV_yields = {0,0,0,0,0,0,0},
        .experience_yield = 0,
        .levelup_moves = {
            .levelup_moves = NULL,
            .length = 0
        },
        .tm_hm_moves = {
            .strings = NULL,
            .length = 0
        },
        .egg_moves = {
            .strings = NULL,
            .length = 0
        },
        .tutor_moves = {
            .strings = NULL,
            .length = 0
        },
        .forms = {
            .strings = NULL,
            .length = 0
        },
        .evolutions = {
            .strings = NULL,
            .length = 0
        }
    };

    /*
     * Make sure expected errors are returned and there is no effect on
     * the output upon failure.
     */
    TEST_ASSERT_EQUAL(
        pkmn_database_get_pokemon_entry(
            "Invalid",
            "Black 2",
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
            "Castform",
            "Not a game",
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
            "Invalid",
            "Black 2",
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
            "Castform",
            "Black 2",
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
            "Stunfisk",
            "Black 2",
            "",
            &pokemon_entry
        ),
        PKMN_ERROR_NONE
    );

    TEST_ASSERT_EQUAL_STRING(pokemon_entry.name, "Stunfisk");
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.game, "Black 2");
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.form, "Standard");
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.species, "Trap");
    TEST_ASSERT_NOT_NULL(pokemon_entry.pokedex_entry);
    TEST_ASSERT_EQUAL_FLOAT(pokemon_entry.height, 0.7f);
    TEST_ASSERT_EQUAL_FLOAT(pokemon_entry.weight, 11.0f);
    TEST_ASSERT_EQUAL_FLOAT(pokemon_entry.chance_male, 0.5f);
    TEST_ASSERT_EQUAL_FLOAT(pokemon_entry.chance_female, 0.5f);
    TEST_ASSERT_FALSE(pokemon_entry.has_gender_differences);
    TEST_ASSERT_EQUAL(pokemon_entry.base_friendship, 70);
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.types.first, "Ground");
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.types.second, "Electric");
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.abilities.first, "Static");
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.abilities.second, "Limber");
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.hidden_ability, "Sand Veil");
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.egg_groups.first, "Water 1");
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.egg_groups.second, "Amorphous");
    TEST_ASSERT_EQUAL(pokemon_entry.base_stats[PKMN_STAT_HP], 109);
    TEST_ASSERT_EQUAL(pokemon_entry.base_stats[PKMN_STAT_ATTACK], 66);
    TEST_ASSERT_EQUAL(pokemon_entry.base_stats[PKMN_STAT_DEFENSE], 84);
    TEST_ASSERT_EQUAL(pokemon_entry.base_stats[PKMN_STAT_SPEED], 32);
    TEST_ASSERT_EQUAL(pokemon_entry.base_stats[PKMN_STAT_SPECIAL], -1);
    TEST_ASSERT_EQUAL(pokemon_entry.base_stats[PKMN_STAT_SPATK], 81);
    TEST_ASSERT_EQUAL(pokemon_entry.base_stats[PKMN_STAT_SPDEF], 99);
    TEST_ASSERT_EQUAL(pokemon_entry.EV_yields[PKMN_STAT_HP], 2);
    TEST_ASSERT_EQUAL(pokemon_entry.EV_yields[PKMN_STAT_ATTACK], 0);
    TEST_ASSERT_EQUAL(pokemon_entry.EV_yields[PKMN_STAT_DEFENSE], 0);
    TEST_ASSERT_EQUAL(pokemon_entry.EV_yields[PKMN_STAT_SPEED], 0);
    TEST_ASSERT_EQUAL(pokemon_entry.EV_yields[PKMN_STAT_SPECIAL], -1);
    TEST_ASSERT_EQUAL(pokemon_entry.EV_yields[PKMN_STAT_SPATK], 0);
    TEST_ASSERT_EQUAL(pokemon_entry.EV_yields[PKMN_STAT_SPDEF], 0);
    TEST_ASSERT_EQUAL(pokemon_entry.experience_yield, 165);

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

    TEST_ASSERT_NOT_NULL(pokemon_entry.levelup_moves.levelup_moves);
    TEST_ASSERT(pokemon_entry.levelup_moves.length > 0);
    TEST_ASSERT_NOT_NULL(pokemon_entry.tm_hm_moves.strings);
    TEST_ASSERT(pokemon_entry.tm_hm_moves.length > 0);
    TEST_ASSERT_NOT_NULL(pokemon_entry.egg_moves.strings);
    TEST_ASSERT(pokemon_entry.egg_moves.length > 0);
    TEST_ASSERT_NOT_NULL(pokemon_entry.tutor_moves.strings);
    TEST_ASSERT(pokemon_entry.tutor_moves.length > 0);
    TEST_ASSERT_NOT_NULL(pokemon_entry.forms.strings);
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
            "Venusaur",
            "Ruby",
            "",
            &pokemon_entry
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.form, "Standard");

    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_set_form(
            &pokemon_entry,
            "Mega"
        ),
        PKMN_ERROR_INVALID_ARGUMENT
    );
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.form, "Standard");

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
            "Venusaur",
            "Omega Ruby",
            "",
            &pokemon_entry
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.form, "Standard");

    TEST_ASSERT_EQUAL(
        pkmn_database_pokemon_entry_set_form(
            &pokemon_entry,
            "Mega"
        ),
        PKMN_ERROR_NONE
    );
    TEST_ASSERT_EQUAL_STRING(pokemon_entry.form, "Mega");

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
