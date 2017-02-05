/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_SPINDA_HPP
#define PKMN_QT_SPINDA_HPP

#include <pkmn/config.hpp>

#ifdef PKMN_QT4
#include <QtCore/QString>
#include <QtGui/QImage>
#else
#include <QImage>
#include <QString>
#endif

#include <cstdint>

namespace pkmn { namespace qt {

    PKMN_API bool GenerateSpindaImageAtPath(
        int generation,
        uint32_t personality,
        const QString &filePath
    );

    PKMN_API void GenerateSpindaImage(
        int generation,
        uint32_t personality,
        QImage* imageOut
    );

}}

#endif /* PKMN_QT_SPINDA_HPP */
