/*
 * p_Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "enum_maps.hpp"
#include "error_internal.hpp"
#include "exception_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/daycare.h>

enum pkmn_error pkmn_daycare_init(
    enum pkmn_game game,
    struct pkmn_daycare* p_daycare_out
)
{
    PKMN_CHECK_NULL_PARAM(p_daycare_out);

    PKMN_CPP_TO_C(
        const pkmn::c::game_bimap_t& game_bimap = pkmn::c::get_game_bimap();

        pkmn::enforce_value_in_map_keys(
            "Game",
            game,
            game_bimap.right
        );

        pkmn::daycare::sptr cpp = pkmn::daycare::make(game_bimap.right.at(game));

        pkmn::c::init_daycare(
            cpp,
            p_daycare_out
        );
    )
}

enum pkmn_error pkmn_daycare_free(
    struct pkmn_daycare* p_daycare
)
{
    PKMN_CHECK_NULL_PARAM(p_daycare);

    p_daycare->game = PKMN_GAME_NONE;
    p_daycare->can_breed_pokemon = false;
    p_daycare->levelup_pokemon_capacity = 0;
    p_daycare->breeding_pokemon_capacity = 0;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_daycare_internal_t**>(&p_daycare->p_internal)
        );
    )
}

const char* pkmn_daycare_strerror(
    struct pkmn_daycare* p_daycare
)
{
    if(!p_daycare)
    {
        return nullptr;
    }

    try
    {
        pkmn_daycare_internal_t* p_internal = DAYCARE_INTERNAL_RCAST(p_daycare->p_internal);
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

enum pkmn_error pkmn_daycare_get_levelup_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    struct pkmn_pokemon* p_levelup_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_daycare);
    pkmn_daycare_internal_t* p_internal = DAYCARE_INTERNAL_RCAST(p_daycare->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_levelup_pokemon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokemon(
            p_internal->cpp->get_levelup_pokemon(
                static_cast<int>(index)
            ),
            p_levelup_pokemon_out
        );
    )
}

enum pkmn_error pkmn_daycare_set_levelup_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    const struct pkmn_pokemon* p_pokemon
)
{
    PKMN_CHECK_NULL_PARAM(p_daycare);
    pkmn_daycare_internal_t* p_internal = DAYCARE_INTERNAL_RCAST(p_daycare->p_internal);
    PKMN_CHECK_NULL_WRAPPER_PARAM_WITH_HANDLE(p_pokemon, p_internal);

    pkmn_pokemon_internal_t* p_pokemon_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_levelup_pokemon(
            static_cast<int>(index),
            p_pokemon_internal->cpp
        );
    )
}

enum pkmn_error pkmn_daycare_get_levelup_pokemon_as_list(
    const struct pkmn_daycare* p_daycare,
    struct pkmn_pokemon_list* p_levelup_pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_daycare);
    pkmn_daycare_internal_t* p_internal = DAYCARE_INTERNAL_RCAST(p_daycare->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_levelup_pokemon_list_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::pokemon_list_cpp_to_c(
            p_internal->cpp->get_levelup_pokemon_as_vector(),
            p_levelup_pokemon_list_out
        );
    )
}

enum pkmn_error pkmn_daycare_get_breeding_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    struct pkmn_pokemon* p_breeding_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_daycare);
    pkmn_daycare_internal_t* p_internal = DAYCARE_INTERNAL_RCAST(p_daycare->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_breeding_pokemon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokemon(
            p_internal->cpp->get_breeding_pokemon(
                static_cast<int>(index)
            ),
            p_breeding_pokemon_out
        );
    )
}

enum pkmn_error pkmn_daycare_set_breeding_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    const struct pkmn_pokemon* p_pokemon
)
{
    PKMN_CHECK_NULL_PARAM(p_daycare);
    pkmn_daycare_internal_t* p_internal = DAYCARE_INTERNAL_RCAST(p_daycare->p_internal);
    PKMN_CHECK_NULL_WRAPPER_PARAM_WITH_HANDLE(p_pokemon, p_internal);

    pkmn_pokemon_internal_t* p_pokemon_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_breeding_pokemon(
            static_cast<int>(index),
            p_pokemon_internal->cpp
        );
    )
}

enum pkmn_error pkmn_daycare_get_breeding_pokemon_as_list(
    const struct pkmn_daycare* p_daycare,
    struct pkmn_pokemon_list* p_breeding_pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_daycare);
    pkmn_daycare_internal_t* p_internal = DAYCARE_INTERNAL_RCAST(p_daycare->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_breeding_pokemon_list_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::pokemon_list_cpp_to_c(
            p_internal->cpp->get_breeding_pokemon_as_vector(),
            p_breeding_pokemon_list_out
        );
    )
}

enum pkmn_error pkmn_daycare_get_egg(
    const struct pkmn_daycare* p_daycare,
    struct pkmn_pokemon* p_egg_out
)
{
    PKMN_CHECK_NULL_PARAM(p_daycare);
    pkmn_daycare_internal_t* p_internal = DAYCARE_INTERNAL_RCAST(p_daycare->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_egg_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokemon(
            p_internal->cpp->get_egg(),
            p_egg_out
        );
    )
}
