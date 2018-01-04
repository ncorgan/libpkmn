/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_ATTRIBUTE_MAPS_HPP
#define CPP_WRAPPERS_ATTRIBUTE_MAPS_HPP

#include "exception_internal.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon.hpp>

namespace pkmn { namespace swig {

    template <typename sptr_type>
    class numeric_attribute_map
    {
        public:
            numeric_attribute_map():
                _internal(nullptr)
            {}

            numeric_attribute_map(
                const pkmn::shared_ptr<sptr_type>& internal
            ): _internal(internal)
            {}

            inline int get_attribute(const std::string& attribute)
            {
                if(!_internal)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                return _internal->get_numeric_attribute(attribute);
            }

            inline void set_attribute(
                const std::string& attribute,
                int value
            )
            {
                if(!_internal)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _internal->set_numeric_attribute(attribute, value);
            }

            inline std::vector<std::string> get_attribute_names()
            {
                if(!_internal)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                return _internal->get_numeric_attribute_names();
            }

            inline uintmax_t cptr()
            {
                return uintmax_t(_internal.get());
            }

        private:
            pkmn::shared_ptr<sptr_type> _internal;
    };

    template <typename sptr_type>
    class string_attribute_map
    {
        public:
            string_attribute_map():
                _internal(nullptr)
            {}

            string_attribute_map(
                const pkmn::shared_ptr<sptr_type>& internal
            ): _internal(internal)
            {}

            inline std::string get_attribute(const std::string& attribute)
            {
                return _internal->get_string_attribute(attribute);
            }

            inline void set_attribute(
                const std::string& attribute,
                const std::string& value
            )
            {
                _internal->set_string_attribute(attribute, value);
            }

            inline std::vector<std::string> get_attribute_names()
            {
                return _internal->get_string_attribute_names();
            }

            inline uintmax_t cptr()
            {
                return uintmax_t(_internal.get());
            }

        private:
            pkmn::shared_ptr<sptr_type> _internal;
    };

}}

#endif /* CPP_WRAPPERS_ATTRIBUTE_MAPS_HPP */
