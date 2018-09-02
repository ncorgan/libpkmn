/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/misc.hpp"

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/daycare.h>

enum pkmn_error pkmn_daycare_init(
    enum pkmn_game game,
    struct pkmn_daycare* p_daycare_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_daycare_out,
                                "p_daycare_out"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::daycare::sptr cpp = pkmn::daycare::make(
                                          static_cast<pkmn::e_game>(game)
                                      );

            pkmn::c::init_daycare(
                cpp,
                p_daycare_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_daycare_free(
    struct pkmn_daycare* p_daycare
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_daycare,
                                "p_daycare"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::delete_pointer_and_set_to_null(
                reinterpret_cast<pkmn::c::daycare_internal_t**>(&p_daycare->p_internal)
            );

            std::memset(p_daycare, 0, sizeof(*p_daycare));
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

const char* pkmn_daycare_strerror(
    struct pkmn_daycare* p_daycare
)
{
    return pkmn::c::strerror<struct pkmn_daycare, pkmn::daycare>(p_daycare);
}

enum pkmn_error pkmn_daycare_get_levelup_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    struct pkmn_pokemon* p_levelup_pokemon_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_daycare,
                                "p_daycare"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_daycare_internal_ptr(p_daycare);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_levelup_pokemon_out,
                    "p_levelup_pokemon_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_pokemon(
                    p_internal->cpp->get_levelup_pokemon(
                        static_cast<int>(index)
                    ),
                    p_levelup_pokemon_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_daycare_set_levelup_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    const struct pkmn_pokemon* p_pokemon
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_daycare,
                                "p_daycare"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_daycare_internal_ptr(p_daycare);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_wrapper_param(
                    p_pokemon,
                    "p_pokemon",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                auto* p_pokemon_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
                BOOST_ASSERT(p_pokemon_internal != nullptr);

                p_internal->cpp->set_levelup_pokemon(
                    static_cast<int>(index),
                    p_pokemon_internal->cpp
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_daycare_get_levelup_pokemon_as_list(
    const struct pkmn_daycare* p_daycare,
    struct pkmn_pokemon_list* p_levelup_pokemon_list_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_daycare,
                                "p_daycare"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_daycare_internal_ptr(p_daycare);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_levelup_pokemon_list_out,
                    "p_levelup_pokemon_list_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::pokemon_list_cpp_to_c(
                    p_internal->cpp->get_levelup_pokemon_as_vector(),
                    p_levelup_pokemon_list_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_daycare_get_breeding_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    struct pkmn_pokemon* p_breeding_pokemon_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_daycare,
                                "p_daycare"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_daycare_internal_ptr(p_daycare);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_breeding_pokemon_out,
                    "p_breeding_pokemon_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_pokemon(
                    p_internal->cpp->get_breeding_pokemon(
                        static_cast<int>(index)
                    ),
                    p_breeding_pokemon_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_daycare_set_breeding_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    const struct pkmn_pokemon* p_pokemon
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_daycare,
                                "p_daycare"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_daycare_internal_ptr(p_daycare);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_wrapper_param(
                    p_pokemon,
                    "p_pokemon",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                auto* p_pokemon_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
                BOOST_ASSERT(p_pokemon_internal != nullptr);

                p_internal->cpp->set_breeding_pokemon(
                    static_cast<int>(index),
                    p_pokemon_internal->cpp
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_daycare_get_breeding_pokemon_as_list(
    const struct pkmn_daycare* p_daycare,
    struct pkmn_pokemon_list* p_breeding_pokemon_list_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_daycare,
                                "p_daycare"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_daycare_internal_ptr(p_daycare);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_breeding_pokemon_list_out,
                    "p_breeding_pokemon_list_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::pokemon_list_cpp_to_c(
                    p_internal->cpp->get_breeding_pokemon_as_vector(),
                    p_breeding_pokemon_list_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_daycare_get_egg(
    const struct pkmn_daycare* p_daycare,
    struct pkmn_pokemon* p_egg_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_daycare,
                                "p_daycare"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_daycare_internal_ptr(p_daycare);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_egg_out,
                    "p_egg_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_pokemon(
                    p_internal->cpp->get_egg(),
                    p_egg_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}
