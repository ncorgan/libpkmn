/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT5_ABILITYLISTCOMBOBOX_HPP
#define PKMN_QT5_ABILITYLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn { namespace qt5 {

    class PKMN_API AbilityListComboBox: public QComboBox {
        Q_OBJECT

        public:
            AbilityListComboBox(
                qint32 generation,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT5_ABILITYLISTCOMBOBOX_HPP */
