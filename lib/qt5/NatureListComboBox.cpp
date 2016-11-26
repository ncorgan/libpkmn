/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt5/NatureListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt5 {

    NatureListComboBox::NatureListComboBox(
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> natures = pkmn::database::get_nature_list();
        for(auto iter = natures.begin(); iter != natures.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt5/moc_NatureListComboBox.cpp"
