/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_ATTRIBUTE_MAPS_HPP
#define CPP_WRAPPERS_ATTRIBUTE_MAPS_HPP

#include "exception_internal.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/game_save.hpp>
#include <pkmn/pokemon.hpp>

#include <boost/assert.hpp>

namespace pkmn { namespace swig {

    template <typename sptr_type>
    class numeric_attribute_map
    {
        public:
            explicit numeric_attribute_map(
                const std::shared_ptr<sptr_type>& internal
            ): _internal(internal)
            {
                BOOST_ASSERT(_internal.get() != nullptr);
            }

            inline int get_attribute(const std::string& attribute)
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return _internal->get_numeric_attribute(attribute);
            }

            inline void set_attribute(
                const std::string& attribute,
                int value
            )
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                _internal->set_numeric_attribute(attribute, value);
            }

            inline std::vector<std::string> get_attribute_names()
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return _internal->get_numeric_attribute_names();
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return uintmax_t(_internal.get());
            }
#else
            inline bool operator==(const numeric_attribute_map& rhs) const
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return (_internal == rhs._internal);
            }

            inline bool operator!=(const numeric_attribute_map& rhs) const
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const std::shared_ptr<sptr_type>& _internal;
    };

    template <typename sptr_type>
    class string_attribute_map
    {
        public:
            explicit string_attribute_map(
                const std::shared_ptr<sptr_type>& internal
            ): _internal(internal)
            {
                BOOST_ASSERT(_internal.get() != nullptr);
            }

            inline std::string get_attribute(const std::string& attribute)
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return _internal->get_string_attribute(attribute);
            }

            inline void set_attribute(
                const std::string& attribute,
                const std::string& value
            )
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                _internal->set_string_attribute(attribute, value);
            }

            inline std::vector<std::string> get_attribute_names()
            {
                return _internal->get_string_attribute_names();
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return uintmax_t(_internal.get());
            }
#else
            inline bool operator==(const string_attribute_map& rhs) const
            {
                return (_internal == rhs._internal);
            }

            inline bool operator!=(const string_attribute_map& rhs) const
            {
                return !operator==(rhs);
            }
#endif

        private:
            const std::shared_ptr<sptr_type>& _internal;
    };

    template <typename sptr_type>
    class boolean_attribute_map
    {
        public:
            explicit boolean_attribute_map(
                const std::shared_ptr<sptr_type>& internal
            ): _internal(internal)
            {
                BOOST_ASSERT(_internal.get() != nullptr);
            }

            inline bool get_attribute(const std::string& attribute)
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return _internal->get_boolean_attribute(attribute);
            }

            inline void set_attribute(
                const std::string& attribute,
                bool value
            )
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                _internal->set_boolean_attribute(attribute, value);
            }

            inline std::vector<std::string> get_attribute_names()
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return _internal->get_boolean_attribute_names();
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return uintmax_t(_internal.get());
            }
#else
            inline bool operator==(const boolean_attribute_map& rhs) const
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return (_internal == rhs._internal);
            }

            inline bool operator!=(const boolean_attribute_map& rhs) const
            {
                BOOST_ASSERT(_internal.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const std::shared_ptr<sptr_type>& _internal;
    };

}}

#endif /* CPP_WRAPPERS_ATTRIBUTE_MAPS_HPP */
