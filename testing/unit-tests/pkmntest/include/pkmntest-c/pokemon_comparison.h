/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_C_POKEMON_COMPARISON_H
#define PKMNTEST_C_POKEMON_COMPARISON_H

#include <pkmntest-c/config.h>

#include <pkmn-c/pokemon2.h>
#include <pkmn-c/types/string_types.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef pkmn_error_t (*pokemon_int_getter_fcn_t)(pkmn_pokemon2_t*,int*);
typedef pkmn_error_t (*pokemon_uint32_getter_fcn_t)(pkmn_pokemon2_t*,uint32_t*);
typedef pkmn_error_t (*pokemon_string_getter_fcn_t)(pkmn_pokemon2_t*,char*,size_t,size_t*);
typedef pkmn_error_t (*pokemon_bool_getter_fcn_t)(pkmn_pokemon2_t*,bool*);
typedef pkmn_error_t (*pokemon_int_buffer_getter_fcn_t)(pkmn_pokemon2_t*,int*,size_t,size_t*);
typedef pkmn_error_t (*pokemon_bool_buffer_getter_fcn_t)(pkmn_pokemon2_t*,bool*,size_t,size_t*);

PKMNTEST_C_API void get_random_pokemon(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_string_list_t* item_list_ptr, // Can be NULL
    const char* species, // Can be NULL
    const char* game
);

PKMNTEST_C_API void compare_pokemon(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr
);

PKMNTEST_C_API void compare_pokemon_ints(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    const char* field,
    pokemon_int_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_uint32s(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    const char* field,
    pokemon_uint32_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_strings(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    const char* field,
    pokemon_string_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_bools(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    const char* field,
    pokemon_bool_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_int_buffers(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    size_t buffer_size,
    const char* field,
    pokemon_int_buffer_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_bool_buffers(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    size_t buffer_size,
    const char* field,
    pokemon_bool_buffer_getter_fcn_t getter_fcn
);

PKMNTEST_C_API void compare_pokemon_original_trainer_info(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr
);

PKMNTEST_C_API void compare_pokemon_moves(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr
);

PKMNTEST_C_API void compare_pokemon_locations_met(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    bool as_egg
);

PKMNTEST_C_API void compare_pokemon_ribbons(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr
);

#endif /* PKMNTEST_C_POKEMON_COMPARISON_H */
