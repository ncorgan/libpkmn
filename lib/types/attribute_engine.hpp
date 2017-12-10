/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_ATTRIBUTE_ENGINE_HPP
#define INCLUDED_PKMN_ATTRIBUTE_ENGINE_HPP

#include <pkmn/config.hpp>

#include <exception>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace pkmn
{
    template <typename T>
    using getter_fcn = std::function<T(void)>;

    template <typename T>
    using setter_fcn = std::function<void(T)>;

    template <typename T>
    struct attribute_fcn_pair_t
    {
        getter_fcn<T> getter;
        setter_fcn<T> setter;
    };

    template <typename T>
    using attribute_fcn_map_t = std::unordered_map<std::string, attribute_fcn_pair_t<T>>;

    class attribute_engine
    {
        public:
            attribute_engine() {}
            ~attribute_engine() {};

            attribute_engine(const attribute_engine&) = default;
            attribute_engine(attribute_engine&&) = default;

            attribute_engine& operator=(const attribute_engine&) = default;
            attribute_engine& operator=(attribute_engine&&) = default;

            void register_numeric_attribute_fcns(
                const std::string& attribute_name,
                const getter_fcn<int>& getter,
                const setter_fcn<int>& setter
            )
            {
                _register_attribute_fcns(
                    attribute_name,
                    getter,
                    setter,
                    _numeric_attribute_map
                );
            }

            std::vector<std::string> get_numeric_attribute_names()
            {
                return _get_attribute_names(_numeric_attribute_map);
            }

            int get_numeric_attribute(
                const std::string& attribute_name
            )
            {
                return _get_attribute(
                           attribute_name,
                           _numeric_attribute_map
                       );
            }

            void set_numeric_attribute(
                const std::string& attribute_name,
                int value
            )
            {
                _set_attribute(
                    attribute_name,
                    value,
                    _numeric_attribute_map
                );
            }

            void register_string_attribute_fcns(
                const std::string& attribute_name,
                const getter_fcn<std::string>& getter,
                const setter_fcn<std::string>& setter
            )
            {
                _register_attribute_fcns(
                    attribute_name,
                    getter,
                    setter,
                    _string_attribute_map
                );
            }

            std::vector<std::string> get_string_attribute_names()
            {
                return _get_attribute_names(_string_attribute_map);
            }

            std::string get_string_attribute(
                const std::string& attribute_name
            )
            {
                return _get_attribute(
                           attribute_name,
                           _string_attribute_map
                       );
            }

            void set_string_attribute(
                const std::string& attribute_name,
                const std::string& value
            )
            {
                _set_attribute(
                    attribute_name,
                    value,
                    _string_attribute_map
                );
            }

        private:
            attribute_fcn_map_t<int> _numeric_attribute_map;
            attribute_fcn_map_t<std::string> _string_attribute_map;

            template <typename T>
            void _register_attribute_fcns(
                const std::string& attribute_name,
                const getter_fcn<T>& getter,
                const setter_fcn<T>& setter,
                attribute_fcn_map_t<T>& attribute_fcn_map
            )
            {
                attribute_fcn_pair_t<T> attribute_fcn_pair = {getter, setter};

                attribute_fcn_map.emplace(
                    attribute_name,
                    attribute_fcn_pair
                );
            }

            template <typename T>
            std::vector<std::string> _get_attribute_names(
                const attribute_fcn_map_t<T>& attribute_fcn_map
            )
            {
                std::vector<std::string> ret;
                for(const auto& attribute_iter: attribute_fcn_map)
                {
                    ret.emplace_back(attribute_iter.first);
                }

                return ret;
            }

            template <typename T>
            T _get_attribute(
                const std::string& attribute_name,
                const attribute_fcn_map_t<T>& attribute_fcn_map
            )
            {
                if(attribute_fcn_map.count(attribute_name) == 0)
                {
                    throw std::invalid_argument("Invalid attribute.");
                }

                const attribute_fcn_pair_t<T>& attribute_fcn_pair = attribute_fcn_map.at(attribute_name);
                if(attribute_fcn_pair.getter)
                {
                    return attribute_fcn_pair.getter();
                }
                else
                {
                    std::string error_message = "The attribute \"";
                    error_message += attribute_name;
                    error_message += "\" is write-only.";

                    throw std::invalid_argument(error_message);
                }
            }

            template <typename T>
            void _set_attribute(
                const std::string& attribute_name,
                const T& value,
                const attribute_fcn_map_t<T>& attribute_fcn_map
            )
            {
                if(attribute_fcn_map.count(attribute_name) == 0)
                {
                    throw std::invalid_argument("Invalid attribute.");
                }

                const attribute_fcn_pair_t<T>& attribute_fcn_pair = attribute_fcn_map.at(attribute_name);
                if(attribute_fcn_pair.setter)
                {
                    attribute_fcn_pair.setter(value);
                }
                else
                {
                    std::string error_message = "The attribute \"";
                    error_message += attribute_name;
                    error_message += "\" is read-only.";

                    throw std::invalid_argument(error_message);
                }
            }
    };
}

#endif /* INCLUDED_PKMN_ATTRIBUTE_ENGINE_HPP */
