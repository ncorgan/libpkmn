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

#include <pkmn-c/pokemon_box.h>

enum pkmn_error pkmn_pokemon_box_init(
    enum pkmn_game game,
    struct pkmn_pokemon_box* p_pokemon_box_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_pokemon_box_out,
                                "p_pokemon_box_out"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::pokemon_box::sptr cpp = pkmn::pokemon_box::make(
                                              static_cast<pkmn::e_game>(game)
                                          );

            pkmn::c::init_pokemon_box(
                cpp,
                p_pokemon_box_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_box_free(
    struct pkmn_pokemon_box* p_pokemon_box
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon_box,
                                "p_pokemon_box"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::delete_pointer_and_set_to_null(
                reinterpret_cast<pkmn::c::pokemon_box_internal_t**>(&p_pokemon_box->p_internal)
            );

            std::memset(p_pokemon_box, 0, sizeof(*p_pokemon_box));
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

const char* pkmn_pokemon_box_strerror(
    const struct pkmn_pokemon_box* p_pokemon_box
)
{
    return pkmn::c::strerror<struct pkmn_pokemon_box, pkmn::pokemon_box>(p_pokemon_box);
}

enum pkmn_error pkmn_pokemon_box_get_name(
    const struct pkmn_pokemon_box* p_pokemon_box,
    char* p_name_buffer_out,
    size_t name_buffer_length,
    size_t* p_actual_name_length_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon_box,
                                "p_pokemon_box"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_box_internal_ptr(p_pokemon_box);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_name_buffer_out,
                    "p_name_buffer_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_cpp_to_c(
                    p_internal->cpp->get_name(),
                    p_name_buffer_out,
                    name_buffer_length,
                    p_actual_name_length_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_box_set_name(
    const struct pkmn_pokemon_box* p_pokemon_box,
    const char* p_name
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon_box,
                                "p_pokemon_box"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_box_internal_ptr(p_pokemon_box);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_name,
                    "p_name",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->set_name(p_name);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_box_get_num_pokemon(
    const struct pkmn_pokemon_box* p_pokemon_box,
    size_t* p_num_pokemon_out
)
{
    return pkmn::c::get_num_pokemon_in_container<struct pkmn_pokemon_box, pkmn::pokemon_box>(
               p_pokemon_box,
               p_num_pokemon_out,
               "p_pokemon_box"
           );
}

enum pkmn_error pkmn_pokemon_box_get_pokemon(
    const struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* p_pokemon_out
)
{
    return pkmn::c::get_pokemon_in_container<struct pkmn_pokemon_box, pkmn::pokemon_box>(
               p_pokemon_box,
               position,
               p_pokemon_out,
               "p_pokemon_box"
           );
}

enum pkmn_error pkmn_pokemon_box_set_pokemon(
    const struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* p_pokemon
)
{
    return pkmn::c::set_pokemon_in_container<struct pkmn_pokemon_box, pkmn::pokemon_box>(
               p_pokemon_box,
               position,
               p_pokemon,
               "p_pokemon_box"
           );
}

enum pkmn_error pkmn_pokemon_box_as_list(
    const struct pkmn_pokemon_box* p_pokemon_box,
    struct pkmn_pokemon_list* p_pokemon_list_out
)
{
    return pkmn::c::pokemon_container_as_list<struct pkmn_pokemon_box, pkmn::pokemon_box>(
               p_pokemon_box,
               p_pokemon_list_out,
               "p_pokemon_box"
           );
}
