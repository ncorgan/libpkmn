/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/RibbonListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt {

    RibbonListComboBox::RibbonListComboBox(
        int generation,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> ribbons = pkmn::database::get_ribbon_name_list(
                                               generation
                                           );
        for(const std::string& ribbon: ribbons)
        {
            addItem(QString::fromStdString(ribbon));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_RibbonListComboBox.cpp"
