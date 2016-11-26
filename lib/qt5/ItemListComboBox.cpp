/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt5/ItemListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt5 {

    ItemListComboBox::ItemListComboBox(
        const QString &game,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> items = pkmn::database::get_item_list(
                                             game.toStdString()
                                         );
        for(auto iter = items.begin(); iter != items.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt5/moc_ItemListComboBox.cpp"
