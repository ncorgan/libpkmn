/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_SUPERTRAININGMEDALLISTCOMBOBOX_HPP
#define PKMN_QT_SUPERTRAININGMEDALLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <QComboBox>
#include <QString>

namespace pkmn { namespace qt {

    class PKMN_API SuperTrainingMedalListComboBox: public QComboBox {
        Q_OBJECT

        public:
            SuperTrainingMedalListComboBox(
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_SUPERTRAININGMEDALLISTCOMBOBOX_HPP */
