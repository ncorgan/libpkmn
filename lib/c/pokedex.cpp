/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "enum_maps.hpp"
#include "error_internal.hpp"
#include "exception_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_list.h>

#include <cstdio>

enum pkmn_error pkmn_pokedex_init(
    enum pkmn_game game,
    struct pkmn_pokedex* p_pokedex_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokedex_out);

    PKMN_CPP_TO_C(
        const pkmn::c::game_bimap_t& game_bimap = pkmn::c::get_game_bimap();

        pkmn::enforce_value_in_map_keys(
            "Game",
            game,
            game_bimap.right
        );

        pkmn::pokedex::sptr cpp = pkmn::pokedex::make(game_bimap.right.at(game));

        pkmn::c::init_pokedex(
            cpp,
            p_pokedex_out
        );
    )
}

enum pkmn_error pkmn_pokedex_free(
    struct pkmn_pokedex* p_pokedex
)
{
    PKMN_CHECK_NULL_PARAM(p_pokedex);

    p_pokedex->game = PKMN_GAME_NONE;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokedex_internal_t**>(&p_pokedex->p_internal)
        );
    )
}

const char* pkmn_pokedex_strerror(
    const struct pkmn_pokedex* p_pokedex
)
{
    if(!p_pokedex)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokedex_internal_t* p_internal = POKEDEX_INTERNAL_RCAST(p_pokedex->p_internal);
        if(!p_internal)
        {
            return nullptr;
        }

        boost::mutex::scoped_lock lock(p_internal->error_mutex);
        return p_internal->last_error.c_str();
    }
    catch(...)
    {
        return nullptr;
    }
}

enum pkmn_error pkmn_pokedex_has_seen(
    const struct pkmn_pokedex* p_pokedex,
    const char* p_species,
    bool* p_has_seen_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_pokedex);
    pkmn_pokedex_internal_t* p_internal = POKEDEX_INTERNAL_RCAST(p_pokedex->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_species, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_has_seen_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_has_seen_out = p_internal->cpp->has_seen(p_species);
    )
}

enum pkmn_error pkmn_pokedex_set_has_seen_species(
    const struct pkmn_pokedex* p_pokedex,
    const char* p_species,
    bool has_seen
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_pokedex);
    pkmn_pokedex_internal_t* p_internal = POKEDEX_INTERNAL_RCAST(p_pokedex->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_species, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_has_seen(p_species, has_seen);
    )
}

enum pkmn_error pkmn_pokedex_get_all_seen(
    const struct pkmn_pokedex* p_pokedex,
    struct pkmn_string_list* p_all_seen_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_pokedex);
    pkmn_pokedex_internal_t* p_internal = POKEDEX_INTERNAL_RCAST(p_pokedex->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_all_seen_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_list_cpp_to_c(
            p_internal->cpp->get_all_seen(),
            p_all_seen_out
        );
    )
}

enum pkmn_error pkmn_pokedex_get_num_seen(
    const struct pkmn_pokedex* p_pokedex,
    size_t* p_num_seen_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_pokedex);
    pkmn_pokedex_internal_t* p_internal = POKEDEX_INTERNAL_RCAST(p_pokedex->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_num_seen_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_num_seen_out = p_internal->cpp->get_num_seen();
    )
}

enum pkmn_error pkmn_pokedex_has_caught(
    const struct pkmn_pokedex* p_pokedex,
    const char* p_species,
    bool* p_has_caught_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_pokedex);
    pkmn_pokedex_internal_t* p_internal = POKEDEX_INTERNAL_RCAST(p_pokedex->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_species, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_has_caught_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_has_caught_out = p_internal->cpp->has_caught(p_species);
    )
}

enum pkmn_error pkmn_pokedex_set_has_caught_species(
    const struct pkmn_pokedex* p_pokedex,
    const char* p_species,
    bool has_caught
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_pokedex);
    pkmn_pokedex_internal_t* p_internal = POKEDEX_INTERNAL_RCAST(p_pokedex->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_species, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_has_caught(p_species, has_caught);
    )
}

enum pkmn_error pkmn_pokedex_get_all_caught(
    const struct pkmn_pokedex* p_pokedex,
    struct pkmn_string_list* p_all_caught_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_pokedex);
    pkmn_pokedex_internal_t* p_internal = POKEDEX_INTERNAL_RCAST(p_pokedex->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_all_caught_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_list_cpp_to_c(
            p_internal->cpp->get_all_caught(),
            p_all_caught_out
        );
    )
}

enum pkmn_error pkmn_pokedex_get_num_caught(
    const struct pkmn_pokedex* p_pokedex,
    size_t* p_num_caught_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_pokedex);
    pkmn_pokedex_internal_t* p_internal = POKEDEX_INTERNAL_RCAST(p_pokedex->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_num_caught_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_num_caught_out = p_internal->cpp->get_num_caught();
    )
}
