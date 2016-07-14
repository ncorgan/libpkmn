/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_UTILS_PATHS_HPP
#define PKMN_UTILS_PATHS_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn {

    PKMN_API std::string get_database_path();

    PKMN_API std::string get_images_dir();

    PKMN_API std::string get_tmp_dir();

}
#endif /* PKMN_UTILS_PATHS_HPP */
