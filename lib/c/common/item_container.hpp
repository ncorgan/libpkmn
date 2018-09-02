/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_COMMON_ITEM_CONTAINER_HPP
#define PKMN_C_COMMON_ITEM_CONTAINER_HPP

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/error.h>

#include <pkmn-c/enums/item.h>

#include <pkmn/enums/item.hpp>

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

namespace pkmn { namespace c {

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error add_item(
        const c_struct_type* p_c_struct,
        enum pkmn_item item,
        size_t amount,
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

            auto impl = [&]()
            {
                p_internal->cpp->add(
                    static_cast<pkmn::e_item>(item),
                    static_cast<int>(amount)
                );
            };

            error = handle_exceptions(impl, p_internal);
        }

        return error;
    }

    template <typename c_struct_type, typename libpkmn_type>
    enum pkmn_error remove_item(
        const c_struct_type* p_c_struct,
        enum pkmn_item item,
        size_t amount,
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

            auto impl = [&]()
            {
                p_internal->cpp->remove(
                    static_cast<pkmn::e_item>(item),
                    static_cast<int>(amount)
                );
            };

            error = handle_exceptions(impl, p_internal);
        }

        return error;
    }
}}

#endif /* PKMN_C_COMMON_ITEM_CONTAINER_HPP */
