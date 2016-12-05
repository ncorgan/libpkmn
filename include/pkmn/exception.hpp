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

namespace pkmn {

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

    class sqlite_error: public std::invalid_argument {
        public:
            sqlite_error(
                const std::string &msg
            ): std::invalid_argument(msg)
            {}
    };

    class unimplemented_error: public std::runtime_error {
        public:
            unimplemented_error():
                std::runtime_error("Currently unimplemented.")
            {}
    };

}

#endif /* PKMN_EXCEPTION_HPP */
