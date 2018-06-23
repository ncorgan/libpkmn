/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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

#include <pkmn-c/pokemon_party.h>

enum pkmn_error pkmn_pokemon_party_init(
    enum pkmn_game game,
    struct pkmn_pokemon_party* p_pokemon_party_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party_out);

    PKMN_CPP_TO_C(
        const pkmn::c::game_bimap_t& game_bimap = pkmn::c::get_game_bimap();

        pkmn::enforce_value_in_map_keys(
            "Game",
            game,
            game_bimap.right
        );

        pkmn::pokemon_party::sptr cpp = pkmn::pokemon_party::make(
                                            game_bimap.right.at(game)
                                        );

        pkmn::c::init_pokemon_party(
            cpp,
            p_pokemon_party_out
        );
    )
}

enum pkmn_error pkmn_pokemon_party_free(
    struct pkmn_pokemon_party* p_pokemon_party
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);

    p_pokemon_party->game = PKMN_GAME_NONE;
    p_pokemon_party->capacity = 0;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_party_internal_t**>(&p_pokemon_party->p_internal)
        );
    )
}

const char* pkmn_pokemon_party_strerror(
    const struct pkmn_pokemon_party* p_pokemon_party
)
{
    if(!p_pokemon_party)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_party_internal_t* p_internal = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
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

enum pkmn_error pkmn_pokemon_party_get_num_pokemon(
    const struct pkmn_pokemon_party* p_pokemon_party,
    size_t* p_num_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);
    pkmn_pokemon_party_internal_t* p_internal = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_num_pokemon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_num_pokemon_out = p_internal->cpp->get_num_pokemon();
    )
}

enum pkmn_error pkmn_pokemon_party_get_pokemon(
    const struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);
    pkmn_pokemon_party_internal_t* p_internal = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokemon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokemon(
            p_internal->cpp->get_pokemon(int(position)),
            p_pokemon_out
        );
    )
}

enum pkmn_error pkmn_pokemon_party_set_pokemon(
    const struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);
    pkmn_pokemon_party_internal_t* p_internal = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
    PKMN_CHECK_NULL_WRAPPER_PARAM_WITH_HANDLE(p_pokemon, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn_pokemon_internal_t* new_pokemon_p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

        p_internal->cpp->set_pokemon(
            int(position),
            new_pokemon_p_internal->cpp
        );
    )
}

enum pkmn_error pkmn_pokemon_party_as_list(
    const struct pkmn_pokemon_party* p_pokemon_party,
    struct pkmn_pokemon_list* p_pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_party);
    pkmn_pokemon_party_internal_t* p_internal = POKEMON_PARTY_INTERNAL_RCAST(p_pokemon_party->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokemon_list_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::pokemon_list_cpp_to_c(
            p_internal->cpp->as_vector(),
            p_pokemon_list_out
        );
    )
}
