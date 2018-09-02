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

#include <pkmn-c/item_list.h>

#include <cstdio>

enum pkmn_error pkmn_pokedex_init(
    enum pkmn_game game,
    struct pkmn_pokedex* p_pokedex_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_pokedex_out,
                                "p_pokedex_out"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::pokedex::sptr cpp = pkmn::pokedex::make(
                                          static_cast<pkmn::e_game>(game)
                                      );

            pkmn::c::init_pokedex(
                cpp,
                p_pokedex_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_pokedex_free(
    struct pkmn_pokedex* p_pokedex
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokedex,
                                "p_pokedex"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::delete_pointer_and_set_to_null(
                reinterpret_cast<pkmn::c::pokedex_internal_t**>(&p_pokedex->p_internal)
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

const char* pkmn_pokedex_strerror(
    const struct pkmn_pokedex* p_pokedex
)
{
    return pkmn::c::strerror<struct pkmn_pokedex, pkmn::pokedex>(p_pokedex);
}

enum pkmn_error pkmn_pokedex_has_seen(
    const struct pkmn_pokedex* p_pokedex,
    enum pkmn_species species,
    bool* p_has_seen_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokedex,
                                "p_pokedex"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokedex_internal_ptr(p_pokedex);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_has_seen_out,
                    "p_has_seen_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_has_seen_out = p_internal->cpp->has_seen(
                                      static_cast<pkmn::e_species>(species)
                                  );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokedex_set_has_seen_species(
    const struct pkmn_pokedex* p_pokedex,
    enum pkmn_species species,
    bool has_seen
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokedex,
                                "p_pokedex"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokedex_internal_ptr(p_pokedex);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_has_seen(
                static_cast<pkmn::e_species>(species),
                has_seen
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokedex_get_all_seen(
    const struct pkmn_pokedex* p_pokedex,
    struct pkmn_species_enum_list* p_all_seen_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokedex,
                                "p_pokedex"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokedex_internal_ptr(p_pokedex);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_all_seen_out,
                    "p_all_seen_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::species_enum_list_cpp_to_c(
                    p_internal->cpp->get_all_seen(),
                    p_all_seen_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokedex_get_num_seen(
    const struct pkmn_pokedex* p_pokedex,
    size_t* p_num_seen_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokedex,
                                "p_pokedex"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokedex_internal_ptr(p_pokedex);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_num_seen_out,
                    "p_num_seen_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_num_seen_out = static_cast<size_t>(
                                      p_internal->cpp->get_num_seen()
                                  );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokedex_has_caught(
    const struct pkmn_pokedex* p_pokedex,
    enum pkmn_species species,
    bool* p_has_caught_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokedex,
                                "p_pokedex"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokedex_internal_ptr(p_pokedex);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_has_caught_out,
                    "p_has_caught_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_has_caught_out = p_internal->cpp->has_caught(
                                        static_cast<pkmn::e_species>(species)
                                    );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokedex_set_has_caught_species(
    const struct pkmn_pokedex* p_pokedex,
    enum pkmn_species species,
    bool has_caught
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokedex,
                                "p_pokedex"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokedex_internal_ptr(p_pokedex);
        BOOST_ASSERT(p_internal != nullptr);

        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->set_has_caught(
                    static_cast<pkmn::e_species>(species),
                    has_caught
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokedex_get_all_caught(
    const struct pkmn_pokedex* p_pokedex,
    struct pkmn_species_enum_list* p_all_caught_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokedex,
                                "p_pokedex"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokedex_internal_ptr(p_pokedex);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_all_caught_out,
                    "p_all_caught_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::species_enum_list_cpp_to_c(
                    p_internal->cpp->get_all_caught(),
                    p_all_caught_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokedex_get_num_caught(
    const struct pkmn_pokedex* p_pokedex,
    size_t* p_num_caught_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokedex,
                                "p_pokedex"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokedex_internal_ptr(p_pokedex);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_num_caught_out,
                    "p_num_caught_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_num_caught_out = static_cast<size_t>(
                                      p_internal->cpp->get_num_caught()
                                  );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}
