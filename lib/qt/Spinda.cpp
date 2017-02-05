/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/Spinda.hpp>
#include <pkmn/utils/paths.hpp>

namespace pkmn { namespace qt {

    bool GenerateSpindaImageAtPath(
        int generation,
        uint32_t personality,
        const QString &filePath
    ) {
        QImage outputImage;

        GenerateSpindaImage(
            generation,
            personality,
            &outputImage
        );

        return outputImage.save(filePath);
    }

    void GenerateSpindaImage(
        int generation,
        uint32_t personality,
        QImage* imageOut
    ) {
        (void)generation;
        (void)personality;
        (void)imageOut;
    }

}}
