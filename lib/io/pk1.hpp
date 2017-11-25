/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_IO_PK1_HPP
#define PKMN_IO_PK1_HPP

#include <pkmn/pokemon.hpp>

#include <cstdint>
#include <vector>

namespace pkmn { namespace io {

    bool vector_is_valid_pk1(
        const std::vector<uint8_t>& buffer
    );

    pkmn::pokemon::sptr load_pk1(
        const std::vector<uint8_t>& buffer
    );

    pkmn::pokemon::sptr load_pk1(
        const std::string& filepath
    );

}}

#endif /* PKMN_IO_PK1_HPP */
