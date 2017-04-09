/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_MEM_SCOPED_LOCK_HPP
#define INCLUDED_PKMN_MEM_SCOPED_LOCK_HPP

#include <pkmn/config.hpp>

#include <boost/thread/mutex.hpp>

namespace pkmn { namespace mem {

    template <typename T>
    class scoped_lock {
        public:
            PKMN_INLINE scoped_lock(
                T* impl
            ): _impl(impl)
            {
                _impl->_mem_mutex.try_lock();
            }

            PKMN_INLINE ~scoped_lock() {
                _impl->_mem_mutex.unlock();
            }

        private:
            T* _impl;
    };

}}

#endif /* INCLUDED_PKMN_MEM_SCOPED_LOCK_HPP */
