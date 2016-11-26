/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt5/RegionListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt5 {

    RegionListComboBox::RegionListComboBox(
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> regions = pkmn::database::get_region_list();
        for(auto iter = regions.begin(); iter != regions.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt5/moc_RegionListComboBox.cpp"
