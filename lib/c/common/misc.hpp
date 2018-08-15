/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_COMMON_MISC_HPP
#define PKMN_C_COMMON_MISC_HPP

#include "cpp_to_c.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

namespace pkmn { namespace c {

    template <typename c_struct_type, typename libpkmn_type>
    const char* strerror(const c_struct_type* p_c_struct)
    {
        const char* p_error = nullptr;

        if((p_c_struct != nullptr) && (p_c_struct->p_internal != nullptr))
        {
            try
            {
                using internal_type_t = internal_t<libpkmn_type>;

                internal_type_t* p_internal = static_cast<internal_type_t*>(
                                                  p_c_struct->p_internal
                                              );
                BOOST_ASSERT(p_internal != nullptr);

                boost::mutex::scoped_lock lock(p_internal->error_mutex);
                return p_internal->last_error.c_str();
            }
            catch(...) {}
        }

        return p_error;
    }

    template <typename pointer_type>
    inline void delete_pointer_and_set_to_null(pointer_type** p_pointer)
    {
        BOOST_ASSERT(p_pointer != nullptr);

        delete (*p_pointer);
        *p_pointer = nullptr;
    }

    template <typename pointer_type>
    inline void free_pointer_and_set_to_null(pointer_type** p_pointer)
    {
        BOOST_ASSERT(p_pointer != nullptr);

        std::free(*p_pointer);
        *p_pointer = nullptr;
    }
}}

#endif /* PKMN_C_COMMON_MISC_HPP */
