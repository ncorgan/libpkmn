/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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

    /*!
     * @brief Generate a Spinda sprite based on the given game and personality.
     *
     * Spinda's spot pattern is based on its hidden personality value. As this
     * value is 32 bits, this means there are 4294967296 possible spot combinations.
     * This function supports all games Generation III-V.
     *
     * \param generation Generation of Spinda sprite to generate (valid values: 3-5)
     * \param personality Spinda's personality
     * \param shiny Whether the Spinda is shiny
     * \param filepath Where to write the generated Spinda sprite
     * \throws pkmn::feature_not_in_build_error if Qt support is not enabled
     * \throws std::out_of_range if generation is not in the range [3-5]
     * \throws std::runtime_error if the write to the file fails
     */
    PKMN_API void GenerateSpindaSpriteAtFilepath(
        int generation,
        uint32_t personality,
        bool shiny,
        const std::string& filepath
    );

}}

#endif /* PKMN_QT_SPINDA_HPP */
