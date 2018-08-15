/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_COMMON_POKEMON_CONTAINER_HPP
#define PKMN_C_COMMON_POKEMON_CONTAINER_HPP

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/error.h>
#include <pkmn-c/pokemon.h>
#include <pkmn-c/types/pokemon_list.h>

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

namespace pkmn { namespace c {

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error get_num_pokemon_in_container(
        const c_struct_type* p_c_struct,
        size_t* p_num_pokemon_out,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            auto* p_internal = get_internal_ptr<c_struct_type, libpkmn_type>(
                                   p_c_struct
                               );
            BOOST_ASSERT(p_internal != nullptr);

            error = check_for_null_param(
                        p_num_pokemon_out,
                        "p_num_pokemon_out",
                        p_internal
                    );
            if(!error)
            {
                auto impl = [&]()
                {
                    *p_num_pokemon_out = p_internal->cpp->get_num_pokemon();
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error get_pokemon_in_container(
        const c_struct_type* p_c_struct,
        size_t position,
        struct pkmn_pokemon* p_pokemon_out,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            auto* p_internal = get_internal_ptr<c_struct_type, libpkmn_type>(
                                   p_c_struct
                               );
            BOOST_ASSERT(p_internal != nullptr);

            error = check_for_null_param(
                        p_pokemon_out,
                        "p_pokemon_out",
                        p_internal
                    );
            if(!error)
            {
                auto impl = [&]()
                {
                    init_pokemon(
                        p_internal->cpp->get_pokemon(static_cast<int>(position)),
                        p_pokemon_out
                    );
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error set_pokemon_in_container(
        const c_struct_type* p_c_struct,
        size_t position,
        const struct pkmn_pokemon* p_pokemon,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            auto* p_internal = get_internal_ptr<c_struct_type, libpkmn_type>(
                                   p_c_struct
                               );
            BOOST_ASSERT(p_internal != nullptr);

            error = check_for_null_param(
                        p_pokemon,
                        "p_pokemon",
                        p_internal
                    );
            if(!error)
            {
                auto impl = [&]()
                {
                    const auto* p_pokemon_internal = get_pokemon_internal_ptr(p_pokemon);

                    p_internal->cpp->set_pokemon(
                        static_cast<int>(position),
                        p_pokemon_internal->cpp
                    );
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error pokemon_container_as_list(
        const c_struct_type* p_c_struct,
        struct pkmn_pokemon_list* p_pokemon_list_out,
        const std::string& c_struct_param_name
    )
    {
        enum pkmn_error error = PKMN_ERROR_NONE;

        error = check_for_null_wrapper_param(
                    p_c_struct,
                    c_struct_param_name
                );
        if(!error)
        {
            auto* p_internal = get_internal_ptr<c_struct_type, libpkmn_type>(
                                   p_c_struct
                               );
            BOOST_ASSERT(p_internal != nullptr);

            error = check_for_null_param(
                        p_pokemon_list_out,
                        "p_pokemon_list_out",
                        p_internal
                    );
            if(!error)
            {
                auto impl = [&]()
                {
                    pkmn::c::pokemon_list_cpp_to_c(
                        p_internal->cpp->as_vector(),
                        p_pokemon_list_out
                    );
                };

                error = handle_exceptions(impl, p_internal);
            }
        }

        return error;
    }
}}

#endif /* PKMN_C_COMMON_POKEMON_CONTAINER_HPP */
