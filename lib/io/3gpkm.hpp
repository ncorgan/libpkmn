/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_IO_3GPKM_HPP
#define PKMN_IO_3GPKM_HPP

#include <pkmn/pokemon.hpp>

#include <string>
#include <vector>

namespace pkmn { namespace io {

    bool vector_is_valid_3gpkm(
        const std::vector<uint8_t> &buffer,
        int* game_id_out
    );

    pkmn::pokemon::sptr load_3gpkm(
        const std::vector<uint8_t> &buffer
    );

    pkmn::pokemon::sptr load_3gpkm(
        const std::string &filepath
    );

    void save_3gpkm(
        pkmn::pokemon::sptr libpkmn_pokemon,
        const std::string &filepath
    );

}}

#endif /* PKMN_IO_3GPKM_HPP */
