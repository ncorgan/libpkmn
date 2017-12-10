/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CLASS_WITH_ATTRIBUTES_HPP
#define PKMN_CLASS_WITH_ATTRIBUTES_HPP

#include <pkmn/types/attribute_engine.hpp>

namespace pkmn {

    class class_with_attributes
    {
        public:
            #ifndef __DOXYGEN__
            class_with_attributes() {}
            ~class_with_attributes () {}
            #endif

            inline int get_numeric_attribute(
                const std::string& attribute_name
            )
            {
                return _numeric_attribute_engine.get_attribute(attribute_name);
            }

            inline void set_numeric_attribute(
                const std::string& attribute_name,
                int value
            )
            {
                _numeric_attribute_engine.set_attribute(attribute_name, value);
            }

            inline std::vector<std::string> get_numeric_attribute_names()
            {
                return _numeric_attribute_engine.get_attribute_names();
            }

            inline std::string get_string_attribute(
                const std::string& attribute_name
            )
            {
                return _string_attribute_engine.get_attribute(attribute_name);
            }

            inline void set_string_attribute(
                const std::string& attribute_name,
                const std::string& value
            )
            {
                _string_attribute_engine.set_attribute(attribute_name, value);
            }

            inline std::vector<std::string> get_string_attribute_names()
            {
                return _string_attribute_engine.get_attribute_names();
            }

        protected:
            pkmn::attribute_engine<int> _numeric_attribute_engine;
            pkmn::attribute_engine<std::string> _string_attribute_engine;
    };
}

#endif /* PKMN_CLASS_WITH_ATTRIBUTES_HPP */
