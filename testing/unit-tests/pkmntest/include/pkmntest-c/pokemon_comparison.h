/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_C_POKEMON_COMPARISON_H
#define PKMNTEST_C_POKEMON_COMPARISON_H

#include <pkmntest-c/config.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/enums/game.h>
#include <pkmn-c/enums/item.h>
#include <pkmn-c/enums/species.h>
#include <pkmn-c/types/string_types.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum pkmn_error (*pokemon_int_getter_fcn_t)(const struct pkmn_pokemon*,int*);
typedef enum pkmn_error (*pokemon_uint32_getter_fcn_t)(const struct pkmn_pokemon*,uint32_t*);
typedef enum pkmn_error (*pokemon_string_getter_fcn_t)(const struct pkmn_pokemon*,char*,size_t,size_t*);
typedef enum pkmn_error (*pokemon_bool_getter_fcn_t)(const struct pkmn_pokemon*,bool*);
typedef enum pkmn_error (*pokemon_int_buffer_getter_fcn_t)(const struct pkmn_pokemon*,int*,size_t,size_t*);
typedef enum pkmn_error (*pokemon_bool_buffer_getter_fcn_t)(const struct pkmn_pokemon*,bool*,size_t,size_t*);

PKMNTEST_C_API void get_random_pokemon(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_item_enum_list* p_item_list, // Can be NULL
    enum pkmn_species species, // PKMN_SPECIES_NONE means function will choose one
    enum pkmn_game game
);

PKMNTEST_C_API void compare_pokemon(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2
);

PKMNTEST_C_API void compare_pokemon_ints(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2,
    const char* field,
    pokemon_int_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_uint32s(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2,
    const char* field,
    pokemon_uint32_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_strings(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2,
    const char* field,
    pokemon_string_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_bools(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2,
    const char* field,
    pokemon_bool_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_int_buffers(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2,
    size_t buffer_size,
    const char* field,
    pokemon_int_buffer_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_bool_buffers(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2,
    size_t buffer_size,
    const char* field,
    pokemon_bool_buffer_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_original_trainer_info(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2
);

PKMNTEST_C_API void compare_pokemon_moves(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2
);

PKMNTEST_C_API void compare_pokemon_locations_met(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2,
    bool as_egg
);

PKMNTEST_C_API void compare_pokemon_ribbons(
    const struct pkmn_pokemon* p_pokemon1,
    const struct pkmn_pokemon* p_pokemon2
);

#endif /* PKMNTEST_C_POKEMON_COMPARISON_H */
