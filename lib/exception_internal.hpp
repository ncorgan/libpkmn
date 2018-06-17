/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_EXCEPTION_INTERNAL_HPP
#define PKMN_EXCEPTION_INTERNAL_HPP

#include "utils/misc.hpp"
#include "utils/floating_point_comparison.hpp"

#include <pkmn/exception.hpp>

#include <pksav/common/stats.h>

#include <algorithm>
#include <exception>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace pkmn
{
    template <typename exception_type>
    void throw_internal_error(
        const std::string& message
    )
    {
        std::string exception_message = "Internal error: ";
        exception_message += message;

        throw exception_type(exception_message);
    }

    // Type-trait-related ugliness goes into these for outputting in a
    // stringstream.
    template <typename T>
    inline typename std::enable_if<!std::is_enum<T>::value, void>::type
    output_to_ostringstream(
        std::ostringstream& r_stream,
        T value
    )
    {
        r_stream << value;
    }

    template <typename T>
    inline typename std::enable_if<std::is_enum<T>::value, void>::type
    output_to_ostringstream(
        std::ostringstream& r_stream,
        T value
    )
    {
        r_stream << static_cast<typename std::underlying_type<T>::type>(value);
    }

    template <typename map_type, typename key_type>
    void enforce_value_in_map_keys(
        const std::string& field,
        const key_type& value,
        const map_type& map
    )
    {
        auto key_count = map.count(value);
        if(key_count == 0)
        {
            std::ostringstream err_msg;
            std::streamsize old_precision = 0;

            if(std::is_floating_point<key_type>::value)
            {
                old_precision = err_msg.precision();
                err_msg.precision(2);
                err_msg << std::fixed;
            }

            err_msg << field;
            err_msg << ": valid values ";
            for(auto iter = map.begin();
                iter != map.end();
                ++iter
            )
            {
                if(iter != map.begin())
                {
                    err_msg << ", ";
                }

                output_to_ostringstream(err_msg, (iter->first));
            }
            err_msg << ".";

            if(std::is_floating_point<key_type>::value)
            {
                err_msg.precision(old_precision);
            }

            throw std::invalid_argument(err_msg.str().c_str());
        }
    }

    template <typename T>
    void enforce_value_in_vector(
        const std::string& field,
        const T& value,
        const std::vector<T>& valid_values
    )
    {
        if(not does_vector_contain_value(valid_values, value))
        {
            std::ostringstream err_msg;
            std::streamsize old_precision = 0;

            if(std::is_floating_point<T>::value)
            {
                old_precision = err_msg.precision();
                err_msg.precision(2);
                err_msg << std::fixed;
            }

            err_msg << field;
            err_msg << ": valid values ";
            for(auto iter = valid_values.begin();
                iter != valid_values.end();
                ++iter
            )
            {
                if(iter != valid_values.begin())
                {
                    err_msg << ", ";
                }

                output_to_ostringstream(err_msg, (*iter));
            }
            err_msg << ".";

            if(std::is_floating_point<T>::value)
            {
                err_msg.precision(old_precision);
            }

            throw std::invalid_argument(err_msg.str().c_str());
        }
    }

    template <typename T>
    void enforce_bounds(
        const std::string& field,
        T value,
        T min,
        T max
    )
    {
        bool is_less_than_range = false;
        bool is_greater_than_range = false;

        if(std::is_floating_point<T>::value)
        {
            is_less_than_range = std::isless(value, min);
            is_greater_than_range = std::isgreater(value, max);
        }
        else
        {
            is_less_than_range = (value < min);
            is_greater_than_range = (value > max);
        }

        if(is_less_than_range or is_greater_than_range)
        {
            std::ostringstream err_msg;
            std::streamsize old_precision = 0;

            if(std::is_floating_point<T>::value)
            {
                old_precision = err_msg.precision();
                err_msg.precision(2);
                err_msg << std::fixed;
            }

            err_msg << field;
            err_msg << ": valid values ";
            output_to_ostringstream(err_msg, min);
            err_msg << "-";
            output_to_ostringstream(err_msg, max);
            err_msg << ".";

            if(std::is_floating_point<T>::value)
            {
                err_msg.precision(old_precision);
            }

            throw std::out_of_range(err_msg.str().c_str());
        }
    }

    enum class value_comparator
    {
        LT,
        LE,
        EQ,
        NE,
        GE,
        GT
    };

    template <typename T>
    void enforce_comparator(
        const std::string& field,
        T value,
        T compare_value,
        value_comparator comparator
    )
    {
        bool did_comparison_pass = false;
        std::string nice_comparator_name;

        switch(comparator)
        {
            case value_comparator::LT:
                nice_comparator_name = "<";
                did_comparison_pass = (value < compare_value);
                break;

            case value_comparator::LE:
                nice_comparator_name = "<=";
                did_comparison_pass = (value <= compare_value);
                break;

            case value_comparator::EQ:
                nice_comparator_name = "==";
                did_comparison_pass = (value == compare_value);
                break;

            case value_comparator::NE:
                nice_comparator_name = "!=";
                did_comparison_pass = (value != compare_value);
                break;

            case value_comparator::GE:
                nice_comparator_name = ">=";
                did_comparison_pass = (value >= compare_value);
                break;

            case value_comparator::GT:
                nice_comparator_name = ">";
                did_comparison_pass = (value > compare_value);
                break;
        }

        if(not did_comparison_pass)
        {
            std::ostringstream err_msg;

            err_msg << field;
            err_msg << ": must be ";
            err_msg << nice_comparator_name;
            err_msg << " ";
            output_to_ostringstream(err_msg, compare_value);
            err_msg << ".";

            throw std::out_of_range(err_msg.str().c_str());
        }
    }

    template <>
    inline void enforce_comparator<float>(
        const std::string& field,
        float value,
        float compare_value,
        value_comparator comparator
    )
    {
        bool did_comparison_pass = false;
        std::string nice_comparator_name;

        switch(comparator)
        {
            case value_comparator::LT:
                nice_comparator_name = "<";
                did_comparison_pass = fp_compare_less(value, compare_value);
                break;

            case value_comparator::LE:
                nice_comparator_name = "<=";
                did_comparison_pass = (not fp_compare_greater(value, compare_value));
                break;

            case value_comparator::EQ:
                nice_comparator_name = "==";
                did_comparison_pass = fp_compare_equal(value, compare_value);
                break;

            case value_comparator::NE:
                nice_comparator_name = "!=";
                did_comparison_pass = fp_compare_not_equal(value, compare_value);
                break;

            case value_comparator::GE:
                nice_comparator_name = ">=";
                did_comparison_pass = (not fp_compare_less(value, compare_value));
                break;

            case value_comparator::GT:
                nice_comparator_name = ">";
                did_comparison_pass = fp_compare_greater(value, compare_value);
                break;
        }

        if(not did_comparison_pass)
        {
            std::ostringstream err_msg;
            err_msg.precision(2);

            err_msg << field;
            err_msg << ": must be ";
            err_msg << nice_comparator_name;
            err_msg << " ";
            err_msg << compare_value;
            err_msg << ".";

            throw std::out_of_range(err_msg.str().c_str());
        }
    }

    template <>
    inline void enforce_comparator<double>(
        const std::string& field,
        double value,
        double compare_value,
        value_comparator comparator
    )
    {
        bool did_comparison_pass = false;
        std::string nice_comparator_name;

        switch(comparator)
        {
            case value_comparator::LT:
                nice_comparator_name = "<";
                did_comparison_pass = fp_compare_less(value, compare_value);
                break;

            case value_comparator::LE:
                nice_comparator_name = "<=";
                did_comparison_pass = (not fp_compare_greater(value, compare_value));
                break;

            case value_comparator::EQ:
                nice_comparator_name = "==";
                did_comparison_pass = fp_compare_equal(value, compare_value);
                break;

            case value_comparator::NE:
                nice_comparator_name = "!=";
                did_comparison_pass = fp_compare_not_equal(value, compare_value);
                break;

            case value_comparator::GE:
                nice_comparator_name = ">=";
                did_comparison_pass = (not fp_compare_less(value, compare_value));
                break;

            case value_comparator::GT:
                nice_comparator_name = ">";
                did_comparison_pass = fp_compare_greater(value, compare_value);
                break;
        }

        if(not did_comparison_pass)
        {
            std::ostringstream err_msg;
            err_msg.precision(2);

            err_msg << field;
            err_msg << ": must be ";
            err_msg << nice_comparator_name;
            err_msg << " ";
            err_msg << compare_value;
            err_msg << ".";

            throw std::out_of_range(err_msg.str().c_str());
        }
    }

    inline void enforce_string_length(
        const std::string& field,
        const std::string& value,
        size_t min_length,
        size_t max_length
    )
    {
        size_t len = value.size();
        if((len < min_length) || (len > max_length))
        {
            std::ostringstream err_msg;

            err_msg << field;
            err_msg << ": valid length ";
            err_msg << min_length;
            err_msg << "-";
            err_msg << max_length;
            err_msg << ".";

            throw std::invalid_argument(err_msg.str().c_str());
        }
    }

    inline void enforce_EV_bounds(
        const std::string& stat,
        int value,
        bool is_game_modern
    )
    {
        std::string field = stat + " EV";

        const int min_value = is_game_modern ? PKSAV_MIN_EV : PKSAV_MIN_GB_EV;
        const int max_value = is_game_modern ? PKSAV_MAX_EV : PKSAV_MAX_GB_EV;

        pkmn::enforce_bounds(
            field,
            value,
            min_value,
            max_value
        );
    }

    inline void enforce_IV_bounds(
        const std::string& stat,
        int value,
        bool is_game_modern
    )
    {
        std::string field = stat + " IV";

        const int min_value = is_game_modern ? PKSAV_MIN_IV : PKSAV_MIN_GB_IV;
        const int max_value = is_game_modern ? PKSAV_MAX_IV : PKSAV_MAX_GB_IV;

        pkmn::enforce_bounds(
            field,
            value,
            min_value,
            max_value
        );
    }

    inline void enforce_gb_trainer_id_bounds(
        uint32_t trainer_id
    )
    {
        pkmn::enforce_bounds(
            "Trainer ID",
            trainer_id,
            0U,
            uint32_t(std::numeric_limits<uint16_t>::max())
        );
    }
}

#endif /* PKMN_EXCEPTION_INTERNAL_HPP */
