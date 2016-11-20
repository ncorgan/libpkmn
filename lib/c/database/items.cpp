/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/items.h>

#include <pkmn/database/item_entry.hpp>

pkmn_error_t pkmn_database_item_category(
    const char* item_name,
    const char* game_name,
    char* item_category_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::item_entry(
                item_name, game_name
            ).get_category(),

            item_category_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_item_pocket(
    const char* item_name,
    const char* game_name,
    char* item_pocket_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::item_entry(
                item_name, game_name
            ).get_pocket(),

            item_pocket_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_item_description(
    const char* item_name,
    const char* game_name,
    char* item_description_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::item_entry(
                item_name, game_name
            ).get_description(),

            item_description_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_item_cost(
    const char* item_name,
    const char* game_name,
    int* item_cost_out
) {
    PKMN_CPP_TO_C(
        *item_cost_out = pkmn::database::item_entry(
                             item_name, game_name
                         ).get_cost();
    )
}

pkmn_error_t pkmn_database_item_holdable(
    const char* item_name,
    const char* game_name,
    bool* item_holdable_out
) {
    PKMN_CPP_TO_C(
        *item_holdable_out = pkmn::database::item_entry(
                                 item_name, game_name
                             ).holdable();
    )
}

pkmn_error_t pkmn_database_item_fling_power(
    const char* item_name,
    const char* game_name,
    int* item_fling_power_out
) {
    PKMN_CPP_TO_C(
        *item_fling_power_out = pkmn::database::item_entry(
                                    item_name, game_name
                                ).get_fling_power();
    )
}

pkmn_error_t pkmn_database_item_fling_effect(
    const char* item_name,
    const char* game_name,
    char* item_fling_effect_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::item_entry(
                item_name, game_name
            ).get_fling_effect(),

            item_fling_effect_out,
            buffer_len,
            actual_strlen_out
        );
    )
}
