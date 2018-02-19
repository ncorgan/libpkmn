/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_RIBBONLISTCOMBOBOX_HPP
#define PKMN_QT_RIBBONLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#ifdef PKMN_QT4
#include <QtGui/QComboBox>
#else
#include <QComboBox>
#endif

namespace pkmn { namespace qt {

    /*!
     * @brief A ComboBox populated with a list (ordered as in-game) of ribbons
     *        available in the given generation.
     */
    class PKMN_API RibbonListComboBox: public QComboBox {
        Q_OBJECT

        public:
            /*!
             * @brief Constructor.
             *
             * \param generation which generation
             * \param parent parent widget
             * \throws std::out_of_range if generation is not [3-6]
             */
            RibbonListComboBox(
                int generation,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_RIBBONLISTCOMBOBOX_HPP */
