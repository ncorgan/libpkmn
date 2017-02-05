/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/Spinda.hpp>

namespace pkmn { namespace qt {

    QString generateSpindaImage(
        int generation,
        uint32_t personality
    ) {
        (void)generation;
        (void)personality;
        return QString("");
    }

    void generateSpindaImage(
        int generation,
        uint32_t personality,
        QImage* imageOut
    ) {
        (void)generation;
        (void)personality;
        (void)imageOut;
    }

}}
