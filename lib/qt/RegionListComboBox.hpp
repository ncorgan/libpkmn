/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_REGIONLISTCOMBOBOX_HPP
#define PKMN_QT_REGIONLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#ifdef PKMN_QT4
#include <QtGui/QComboBox>
#include <QtCore/QString>
#else
#include <QComboBox>
#include <QString>
#endif

namespace pkmn { namespace qt {

    /*!
     * @brief A ComboBox populated with a list (sorted by their first generation)
     *        of regions in the Pokémon world.
     */
    class PKMN_API RegionListComboBox: public QComboBox {
        Q_OBJECT

        public:
            RegionListComboBox(
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_REGIONLISTCOMBOBOX_HPP */
