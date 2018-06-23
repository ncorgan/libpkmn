/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_ITEMLISTCOMBOBOX_HPP
#define PKMN_QT_ITEMLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/game.hpp>

#ifdef PKMN_ENABLE_QT
#include <QComboBox>
#else
#error Qt support is not enabled in this build of LibPKMN.
#endif

namespace pkmn { namespace qt {

    /*!
     * @brief A ComboBox populated with an alphabetized list of
     *        items available in the given game.
     */
    class PKMN_API ItemListComboBox: public QComboBox
    {
        Q_OBJECT

        public:
            /*!
             * @brief Constructor.
             *
             * \param game which game
             * \param parent parent widget
             * \throws std::invalid_argument if the given game is invalid
             */
            ItemListComboBox(
                pkmn::e_game game,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_ITEMLISTCOMBOBOX_HPP */
