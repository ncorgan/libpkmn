/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt5/RibbonListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt5 {

    RibbonListComboBox::RibbonListComboBox(
        int generation,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> ribbons = pkmn::database::get_ribbon_list(
                                               generation
                                           );
        for(auto iter = ribbons.begin(); iter != ribbons.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt5/moc_RibbonListComboBox.cpp"
