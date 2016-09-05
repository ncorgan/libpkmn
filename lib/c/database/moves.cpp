/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/moves.h>

#include <pkmn/database/move_entry.hpp>

pkmn_error_t pkmn_database_move_type(
    const char* move_name,
    const char* game_name,
    char* move_type_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::move_entry(
                move_name, game_name
            ).get_type(),

            move_type_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_move_description(
    const char* move_name,
    const char* game_name,
    char* move_description_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::move_entry(
                move_name, game_name
            ).get_description(),

            move_description_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_move_target(
    const char* move_name,
    const char* game_name,
    char* move_target_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::move_entry(
                move_name, game_name
            ).get_target(),

            move_target_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_move_damage_class(
    const char* move_name,
    const char* game_name,
    char* move_damage_class_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::move_entry(
                move_name, game_name
            ).get_damage_class(),

            move_damage_class_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_move_base_power(
    const char* move_name,
    const char* game_name,
    int* base_power_out
) {
    PKMN_CPP_TO_C(
        *base_power_out = pkmn::database::move_entry(
                              move_name, game_name
                          ).get_base_power();
    )
}

pkmn_error_t pkmn_database_move_pp(
    const char* move_name,
    const char* game_name,
    int num_pp_ups,
    int* pp_out
) {
    PKMN_CPP_TO_C(
        *pp_out = pkmn::database::move_entry(
                      move_name, game_name
                  ).get_pp(num_pp_ups);
    )
}

pkmn_error_t pkmn_database_move_accuracy(
    const char* move_name,
    const char* game_name,
    float* accuracy_out
) {
    PKMN_CPP_TO_C(
        *accuracy_out = pkmn::database::move_entry(
                            move_name, game_name
                        ).get_accuracy();
    )
}

pkmn_error_t pkmn_database_move_priority(
    const char* move_name,
    const char* game_name,
    int* priority_out
) {
    PKMN_CPP_TO_C(
        *priority_out = pkmn::database::move_entry(
                            move_name, game_name
                        ).get_priority();
    )
}

pkmn_error_t pkmn_database_move_effect(
    const char* move_name,
    const char* game_name,
    char* move_effect_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::move_entry(
                move_name, game_name
            ).get_effect(),

            move_effect_out,
            buffer_len,
            actual_strlen_out
        );
    )
}
