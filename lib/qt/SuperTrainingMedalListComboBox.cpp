/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/SuperTrainingMedalListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt {

    SuperTrainingMedalListComboBox::SuperTrainingMedalListComboBox(
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> super_training_medals =
            pkmn::database::get_super_training_medal_name_list();
        for(const std::string& super_training_medal: super_training_medals)
        {
            addItem(QString::fromStdString(super_training_medal));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_SuperTrainingMedalListComboBox.cpp"
