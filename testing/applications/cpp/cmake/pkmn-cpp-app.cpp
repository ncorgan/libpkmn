/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/utils/paths.hpp>

#include <iostream>

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "Database path: " << pkmn::get_database_path() << std::endl;

    return 0;
}
