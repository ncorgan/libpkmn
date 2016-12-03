/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/pokemon.h>

pkmn_error_t pkmn_pokemon_make(
    pkmn_pokemon_handle_t* handle_ptr,
    const char* species,
    const char* game,
    const char* form,
    int level
) {
    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::make(
                                        species,
                                        game,
                                        form,
                                        level
                                    );

        (*handle_ptr) = new pkmn_pokemon_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_pokemon_free(
    pkmn_pokemon_handle_t* handle_ptr
) {
    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_pokemon_strerror(
    pkmn_pokemon_handle_t handle
) {
    return handle->last_error.c_str();
}

pkmn_error_t pkmn_pokemon_get_game(
    pkmn_pokemon_handle_t handle,
    char* game_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_game(),
            game_out,
            buffer_len,
            actual_strlen_out
        )
    )
}
