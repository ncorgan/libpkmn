/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_GAMECUBESHADOWPOKEMONLISTCOMBOBOX_HPP
#define PKMN_QT_GAMECUBESHADOWPOKEMONLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#ifdef PKMN_QT4
#include <QtGui/QComboBox>
#else
#include <QComboBox>
#endif

namespace pkmn { namespace qt {

    /*!
     * @brief A ComboBox populated with a list, sorted by version group, of games
     *        in (or up to) the given generation.
     */
    class PKMN_API GamecubeShadowPokemonListComboBox: public QComboBox {
        Q_OBJECT

        public:
            /*!
             * @brief Constructor.
             *
             * \param colosseum show Pokémon from Colosseum
             * \param parent parent widget
             */
            GamecubeShadowPokemonListComboBox(
                bool colosseum,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_GAMECUBESHADOWPOKEMONLISTCOMBOBOX_HPP */
