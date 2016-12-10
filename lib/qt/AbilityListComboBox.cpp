/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/AbilityListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt {

    AbilityListComboBox::AbilityListComboBox(
        int generation,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> abilities = pkmn::database::get_ability_list(
                                                 generation
                                             );
        for(auto iter = abilities.begin(); iter != abilities.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_AbilityListComboBox.cpp"
