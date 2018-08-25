/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/pokemon_pc.h>

enum pkmn_error pkmn_pokemon_pc_init(
    const char* p_game,
    struct pkmn_pokemon_pc* p_pokemon_pc_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_game,
                                "p_game"
                            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_pokemon_pc_out,
                    "p_pokemon_pc_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::pokemon_pc::sptr cpp = pkmn::pokemon_pc::make(p_game);

            pkmn::c::init_pokemon_pc(
                cpp,
                p_pokemon_pc_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_pc_free(
    struct pkmn_pokemon_pc* p_pokemon_pc
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon_pc,
                                "p_pokemon_pc"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::free_pointer_and_set_to_null(&p_pokemon_pc->p_game);
            pkmn::c::delete_pointer_and_set_to_null(
                reinterpret_cast<pkmn::c::pokemon_pc_internal_t**>(&p_pokemon_pc->p_internal)
            );

            std::memset(p_pokemon_pc, 0, sizeof(*p_pokemon_pc));
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

const char* pkmn_pokemon_pc_strerror(
    const struct pkmn_pokemon_pc* p_pokemon_pc
)
{
    return pkmn::c::strerror<struct pkmn_pokemon_pc, pkmn::pokemon_pc>(p_pokemon_pc);
}

enum pkmn_error pkmn_pokemon_pc_get_box(
    const struct pkmn_pokemon_pc* p_pokemon_pc,
    size_t index,
    struct pkmn_pokemon_box* p_pokemon_box_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon_pc,
                                "p_pokemon_pc"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_pc_internal_ptr(p_pokemon_pc);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_pokemon_box_out,
                    "p_pokemon_box_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_pokemon_box(
                    p_internal->cpp->get_box(static_cast<int>(index)),
                    p_pokemon_box_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_pc_get_box_names(
    const struct pkmn_pokemon_pc* p_pokemon_pc,
    struct pkmn_string_list* p_box_names_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon_pc,
                                "p_pokemon_pc"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_pc_internal_ptr(p_pokemon_pc);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_box_names_out,
                    "p_box_names_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_list_cpp_to_c(
                    p_internal->cpp->get_box_names(),
                    p_box_names_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_pc_as_list(
    const struct pkmn_pokemon_pc* p_pokemon_pc,
    struct pkmn_pokemon_box_list* p_pokemon_box_list_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon_pc,
                                "p_pokemon_pc"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_pc_internal_ptr(p_pokemon_pc);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_pokemon_box_list_out,
                    "p_pokemon_box_list_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::pokemon_box_list_cpp_to_c(
                    p_internal->cpp->as_vector(),
                    p_pokemon_box_list_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}
