/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/misc.hpp"
#include "common/pokemon_container.hpp"

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/pokemon_party.h>

enum pkmn_error pkmn_pokemon_party_init(
    enum pkmn_game game,
    struct pkmn_pokemon_party* p_pokemon_party_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_pokemon_party_out,
                                "p_pokemon_party_out"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::pokemon_party::sptr cpp = pkmn::pokemon_party::make(
                                                static_cast<pkmn::e_game>(game)
                                            );

            pkmn::c::init_pokemon_party(
                cpp,
                p_pokemon_party_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_party_free(
    struct pkmn_pokemon_party* p_pokemon_party
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon_party,
                                "p_pokemon_party"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::delete_pointer_and_set_to_null(
                reinterpret_cast<pkmn::c::pokemon_party_internal_t**>(&p_pokemon_party->p_internal)
            );

            std::memset(p_pokemon_party, 0, sizeof(*p_pokemon_party));
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

const char* pkmn_pokemon_party_strerror(
    const struct pkmn_pokemon_party* p_pokemon_party
)
{
    return pkmn::c::strerror<struct pkmn_pokemon_party, pkmn::pokemon_party>(p_pokemon_party);
}

enum pkmn_error pkmn_pokemon_party_get_num_pokemon(
    const struct pkmn_pokemon_party* p_pokemon_party,
    size_t* p_num_pokemon_out
)
{
    return pkmn::c::get_num_pokemon_in_container<struct pkmn_pokemon_party, pkmn::pokemon_party>(
               p_pokemon_party,
               p_num_pokemon_out,
               "p_pokemon_party"
           );
}

enum pkmn_error pkmn_pokemon_party_get_pokemon(
    const struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon_out
)
{
    return pkmn::c::get_pokemon_in_container<struct pkmn_pokemon_party, pkmn::pokemon_party>(
               p_pokemon_party,
               position,
               p_pokemon_out,
               "p_pokemon_party"
           );
}

enum pkmn_error pkmn_pokemon_party_set_pokemon(
    const struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon
)
{
    return pkmn::c::set_pokemon_in_container<struct pkmn_pokemon_party, pkmn::pokemon_party>(
               p_pokemon_party,
               position,
               p_pokemon,
               "p_pokemon_party"
           );
}

enum pkmn_error pkmn_pokemon_party_as_list(
    const struct pkmn_pokemon_party* p_pokemon_party,
    struct pkmn_pokemon_list* p_pokemon_list_out
)
{
    return pkmn::c::pokemon_container_as_list<struct pkmn_pokemon_party, pkmn::pokemon_party>(
               p_pokemon_party,
               p_pokemon_list_out,
               "p_pokemon_party"
           );
}
