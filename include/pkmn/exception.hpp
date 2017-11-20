/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_EXCEPTION_HPP
#define PKMN_EXCEPTION_HPP

#include <pkmn/config.hpp>

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(disable: 4275) // An exported class was derived from a class that was not exported.
#endif

namespace pkmn {

    //! An exception thrown when a user tries to use an optional feature not built into in this installation.
    class PKMN_API feature_not_in_build_error: public std::runtime_error
    {
        public:
            //! Throw the exception for the given feature.
            feature_not_in_build_error(
                const std::string &feature
            );
    };

    //! An exception thrown when a given functionality does not exist in the given game.
    /*!
     * As the series progressed, features were added, so when these newer features are
     * requested for older games, this is the exception thrown.
     *
     * For examples, contests and ribbons were introduced in Generation III, so calling
     * any function involving ribbons in Generation I-II will throw this exception.
     */
    class PKMN_API feature_not_in_game_error: public std::runtime_error {
        public:
            //! Throw the exception with the given message.
            feature_not_in_game_error(
                const std::string &msg
            );
            //! Throw the exception for the given feature and game.
            feature_not_in_game_error(
                const std::string &feature,
                const std::string &game
            );
    };

    //! An exception thrown when a non-success error code is returned from PKSav.
    /*!
     * LibPKMN uses PKSav, a C library, under the hood for game save parsing, and this exception
     * is used to propagate its error codes into exceptions.
     *
     * PKSav error codes can be found
     * <a href="https://github.com/ncorgan/pksav/blob/master/include/pksav/error.h">here</a>.
     */
    class PKMN_API pksav_error: public std::runtime_error {
        public:
            //! Throw an exception for the given PKSav error code.
            pksav_error(
                int pksav_error_code
            );
    };

    //! An exception thrown when an exposed function is not implemented for a given game.
    class unimplemented_error: public std::runtime_error {
        public:
            unimplemented_error():
                std::runtime_error("Currently unimplemented.")
            {}
    };

    //! Throw a std::invalid_argument, specifying all valid arguments in the error message.
    /*!
     * \param field the variable whose value is invalid
     * \param valid_values a list of valid values for the field
     */
    template <typename T>
    PKMN_INLINE void throw_invalid_argument(
        const std::string& field,
        const std::vector<T>& valid_values
    ) {
        std::ostringstream err_msg;
        if(std::is_floating_point<T>::value) {
            err_msg.precision(2);
            err_msg << std::fixed;
        }

        err_msg << field;
        err_msg << ": valid values ";
        for(auto iter = valid_values.begin(); iter != valid_values.end(); ++iter) {
            if(iter != valid_values.begin()) {
                err_msg << ", ";
            }

            err_msg << (*iter);
        }
        err_msg << ".";

        throw std::invalid_argument(err_msg.str().c_str());
    }

    //! Throw a std::out_of_range if the given value is outside the provided range.
    /*!
     * \param field the variable whose value is invalid
     * \param value value to check
     * \param min the minimum value
     * \param max the maximum value
     */
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
            err_msg << min;
            err_msg << "-";
            err_msg << max;
            err_msg << ".";

            if(std::is_floating_point<T>::value)
            {
                err_msg.precision(old_precision);
            }

            throw std::out_of_range(err_msg.str().c_str());
        }
    }

}

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(default: 4275)
#endif

#endif /* PKMN_EXCEPTION_HPP */
