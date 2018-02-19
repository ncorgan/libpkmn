/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_QT_ABILITYLISTCOMBOBOX_HPP
#define PKMN_QT_ABILITYLISTCOMBOBOX_HPP

#include <pkmn/config.hpp>

#ifdef PKMN_ENABLE_QT
#include <QComboBox>
#else
#error Qt support is not enabled in this build of LibPKMN.
#endif

namespace pkmn { namespace qt {

    /*!
     * @brief A ComboBox populated with an alphabetized list of all abilities
     *        available in the given generation.
     */
    class PKMN_API AbilityListComboBox: public QComboBox
    {
        Q_OBJECT

        public:
            /*!
             * @brief Constructor.
             *
             * \param generation which generation
             * \param parent parent widget
             * \throws std::out_of_range if generation is not [3-6]
             */
            AbilityListComboBox(
                int generation,
                QWidget* parent
            );

        signals:

        public slots:
    };

}}

#endif /* PKMN_QT_ABILITYLISTCOMBOBOX_HPP */
