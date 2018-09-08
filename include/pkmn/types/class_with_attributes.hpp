/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CLASS_WITH_ATTRIBUTES_HPP
#define PKMN_CLASS_WITH_ATTRIBUTES_HPP

#include <pkmn/types/attribute_engine.hpp>

namespace pkmn {

    /*!
     * @brief A base class that gives derived classes the ability to get and
     *        set numeric, string, and boolean attributes.
     *
     * Note that some attributes are read-only.
     */
    class class_with_attributes
    {
        public:
            class_with_attributes() = default;
            ~class_with_attributes() = default;

            class_with_attributes(const class_with_attributes&) = default;
            class_with_attributes(class_with_attributes&&) = default;

            class_with_attributes& operator=(const class_with_attributes&) = default;
            class_with_attributes& operator=(class_with_attributes&&) = default;

            /*!
             * @brief Query the numeric attribute with the given name.
             *
             * \param attribute_name The name of the attribute to query
             * \throws std::invalid_argument If the given attribute name doesn't exist
             */
            inline int get_numeric_attribute(
                const std::string& attribute_name
            )
            {
                return _numeric_attribute_engine.get_attribute(attribute_name);
            }

            /*!
             * @brief Sets the numeric attribute with the given name to the given
             *        value.
             *
             * Along with the errors listed below, the attribute setter can throw
             * any of its own errors based on value range, etc.
             *
             * \param attribute_name The name of the attribute whose value to set
             * \param value The new value for the attribute
             * \throws std::invalid_argument If the given attribute name doesn't exist
             * \throws std::invalid_argument If the given attribute is valid but read-only
             */
            inline void set_numeric_attribute(
                const std::string& attribute_name,
                int value
            )
            {
                _numeric_attribute_engine.set_attribute(attribute_name, value);
            }

            /*!
             * @brief Returns the full list of numeric attributes.
             *
             * All attributes can be read, so all values returned from this
             * function are valid inputs for get_numeric_attribute(). Some
             * attributes are read-only and are thus invalid inputs for
             * set_numeric_attribute().
             */
            inline std::vector<std::string> get_numeric_attribute_names()
            {
                return _numeric_attribute_engine.get_attribute_names();
            }

            /*!
             * @brief Query the string attribute with the given name.
             *
             * \param attribute_name The name of the attribute to query
             * \throws std::invalid_argument If the given attribute name doesn't exist
             */
            inline std::string get_string_attribute(
                const std::string& attribute_name
            )
            {
                return _string_attribute_engine.get_attribute(attribute_name);
            }

            /*!
             * @brief Sets the string attribute with the given name to the given
             *        value.
             *
             * Along with the errors listed below, the attribute setter can throw
             * any of its own errors based on value range, etc.
             *
             * \param attribute_name The name of the attribute whose value to set
             * \param value The new value for the attribute
             * \throws std::invalid_argument If the given attribute name doesn't exist
             * \throws std::invalid_argument If the given attribute is valid but read-only
             */
            inline void set_string_attribute(
                const std::string& attribute_name,
                const std::string& value
            )
            {
                _string_attribute_engine.set_attribute(attribute_name, value);
            }

            /*!
             * @brief Returns the full list of string attributes.
             *
             * All attributes can be read, so all values returned from this
             * function are valid inputs for get_string_attribute(). Some
             * attributes are read-only and are thus invalid inputs for
             * set_string_attribute().
             */
            inline std::vector<std::string> get_string_attribute_names()
            {
                return _string_attribute_engine.get_attribute_names();
            }

            /*!
             * @brief Query the boolean attribute with the given name.
             *
             * \param attribute_name The name of the attribute to query
             * \throws std::invalid_argument If the given attribute name doesn't exist
             */
            inline bool get_boolean_attribute(
                const std::string& attribute_name
            )
            {
                return _boolean_attribute_engine.get_attribute(attribute_name);
            }

            /*!
             * @brief Sets the boolean attribute with the given name to the given
             *        value.
             *
             * Along with the errors listed below, the attribute setter can throw
             * any of its own errors based on value range, etc.
             *
             * \param attribute_name The name of the attribute whose value to set
             * \param value The new value for the attribute
             * \throws std::invalid_argument If the given attribute name doesn't exist
             * \throws std::invalid_argument If the given attribute is valid but read-only
             */
            inline void set_boolean_attribute(
                const std::string& attribute_name,
                bool value
            )
            {
                _boolean_attribute_engine.set_attribute(attribute_name, value);
            }

            /*!
             * @brief Returns the full list of boolean attributes.
             *
             * All attributes can be read, so all values returned from this
             * function are valid inputs for get_boolean_attribute(). Some
             * attributes are read-only and are thus invalid inputs for
             * set_boolean_attribute().
             */
            inline std::vector<std::string> get_boolean_attribute_names()
            {
                return _boolean_attribute_engine.get_attribute_names();
            }

        protected:
            pkmn::attribute_engine<int> _numeric_attribute_engine;
            pkmn::attribute_engine<std::string> _string_attribute_engine;
            pkmn::attribute_engine<bool> _boolean_attribute_engine;
    };
}

#endif /* PKMN_CLASS_WITH_ATTRIBUTES_HPP */
