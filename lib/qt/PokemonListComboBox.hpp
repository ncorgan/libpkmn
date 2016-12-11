/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_POKEMONLISTCOMBOBOX_HPP
#define PKMN_QT_POKEMONLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#ifdef PKMN_QT4
#include <QtGui/QComboBox>
#include <QtCore/QString>
#else
#include <QComboBox>
#include <QString>
#endif

namespace pkmn { namespace qt {

    class PKMN_API PokemonListComboBox: public QComboBox {
        Q_OBJECT

        public:
            PokemonListComboBox(
                int generation,
                bool includePrevious,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_POKEMONLISTCOMBOBOX_HPP */
