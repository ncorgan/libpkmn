/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_EXCEPTION_HPP
#define PKMN_EXCEPTION_HPP

#include <pkmn/config.hpp>

#include <stdexcept>
#include <string>

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(disable: 4275) // An exported class was derived from a class that was not exported.
#endif

namespace pkmn {

    class PKMN_API feature_not_in_game_error: public std::runtime_error {
        public:
            feature_not_in_game_error(
                const std::string &msg
            );
            feature_not_in_game_error(
                const std::string &feature,
                const std::string &game
            );
    };

    class PKMN_API pksav_error: public std::runtime_error {
        public:
            pksav_error(
                int pksav_error_code
            );
    };

    class PKMN_API range_error: public std::out_of_range {
        public:
            range_error(
                const std::string &value,
                int min,
                int max
            );
    };

    class unimplemented_error: public std::runtime_error {
        public:
            unimplemented_error():
                std::runtime_error("Currently unimplemented.")
            {}
    };

}

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(default: 4275)
#endif

#endif /* PKMN_EXCEPTION_HPP */
