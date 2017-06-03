/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_SPINDA_HPP
#define PKMN_QT_SPINDA_HPP

#include <pkmn/config.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace qt {

    PKMN_API bool GenerateSpindaSpriteAtFilepath(
        int generation,
        uint32_t personality,
        bool shiny,
        const std::string& filepath
    );

}}

#endif /* PKMN_QT_SPINDA_HPP */
