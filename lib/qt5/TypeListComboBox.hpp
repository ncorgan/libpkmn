/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT5_TYPELISTCOMBOBOX_HPP
#define PKMN_QT5_TYPELISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <QComboBox>
#include <QString>

namespace pkmn { namespace qt5 {

    class PKMN_API TypeListComboBox: public QComboBox {
        Q_OBJECT

        public:
            TypeListComboBox(
                const QString &game,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT5_TYPELISTCOMBOBOX_HPP */
