/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/move_entry.h>

#include <pkmn/database/move_entry.hpp>

#include <cstdlib>
#include <cstring>

pkmn_error_t pkmn_database_get_move_entry(
    const char* move_name,
    const char* move_game,
    pkmn_database_move_entry_t* move_entry_out
) {
    PKMN_CPP_TO_C(
        pkmn::database::move_entry move_entry_cpp(
                                       move_name,
                                       move_game
                                   );

        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_name(),
            &move_entry_out->name
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_game(),
            &move_entry_out->game
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_description(),
            &move_entry_out->description
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_target(),
            &move_entry_out->target
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_damage_class(),
            &move_entry_out->damage_class
        );

        move_entry_out->base_power = move_entry_cpp.get_base_power();
        for(int i = 0; i < 4; ++i) {
            move_entry_out->pp[i] = move_entry_cpp.get_pp(i);
        }
        move_entry_out->accuracy = move_entry_cpp.get_accuracy();

        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_effect(),
            &move_entry_out->effect
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_contest_type(),
            &move_entry_out->contest_type
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_contest_effect(),
            &move_entry_out->contest_effect
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_super_contest_effect(),
            &move_entry_out->super_contest_effect
        );
    )
}

pkmn_error_t pkmn_database_move_entry_free(
    pkmn_database_move_entry_t* move_entry
) {
    std::free(move_entry->name);
    move_entry->name = NULL;

    std::free(move_entry->game);
    move_entry->game = NULL;

    std::free(move_entry->description);
    move_entry->description = NULL;

    std::free(move_entry->target);
    move_entry->target = NULL;

    std::free(move_entry->damage_class);
    move_entry->damage_class = NULL;

    move_entry->base_power = 0;
    for(int i = 0; i < 4; ++i) {
        move_entry->pp[i] = 0;
    }
    move_entry->accuracy = 0.0f;

    std::free(move_entry->effect);
    move_entry->effect = NULL;

    std::free(move_entry->contest_type);
    move_entry->contest_type = NULL;

    std::free(move_entry->contest_effect);
    move_entry->contest_effect = NULL;

    std::free(move_entry->super_contest_effect);
    move_entry->super_contest_effect = NULL;

    return PKMN_ERROR_NONE;
}
