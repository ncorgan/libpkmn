/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_TYPELISTCOMBOBOX_HPP
#define PKMN_QT_TYPELISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#ifdef PKMN_ENABLE_QT
#include <QComboBox>
#include <QString>
#endif

namespace pkmn { namespace qt {

    /*!
     * @brief A ComboBox populated with a list of types available in the given game.
     */
    class PKMN_API TypeListComboBox: public QComboBox
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
            TypeListComboBox(
                const QString& game,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_TYPELISTCOMBOBOX_HPP */
