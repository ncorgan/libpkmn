/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt5/SuperTrainingMedalListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt5 {

    SuperTrainingMedalListComboBox::SuperTrainingMedalListComboBox(
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> super_training_medals = pkmn::database::get_super_training_medal_list();
        for(auto iter = super_training_medals.begin(); iter != super_training_medals.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt5/moc_SuperTrainingMedalListComboBox.cpp"
