/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_LOCATIONLISTCOMBOBOX_HPP
#define PKMN_QT_LOCATIONLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#ifdef PKMN_ENABLE_QT
#include <QComboBox>
#include <QString>
#else
#error Qt support is not enabled in this build of LIbPKMN.
#endif

namespace pkmn { namespace qt {

    /*!
     * @brief A ComboBox populated with an alphabetized list of locations available in the given
     *        game (or generation).
     */
    class PKMN_API LocationListComboBox: public QComboBox
    {
        Q_OBJECT

        public:
            /*!
             * @brief Constructor.
             *
             * Note: even if wholeGeneration is set to true, Game Boy Advance locations will not appear in
             * a list of Gamecube locations, and vice versa.
             *
             * \param game which game
             * \param wholeGeneration include locations from all games in this generation
             * \param parent parent widget
             * \throws std::invalid_argument if the given game is invalid
             */
            LocationListComboBox(
                const QString& game,
                bool wholeGeneration,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_LOCATIONLISTCOMBOBOX_HPP */
