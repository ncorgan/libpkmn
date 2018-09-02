/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/ItemListComboBox.hpp>

#include <pkmn/database/lists.hpp>
#include <pkmn/enums/enum_to_string.hpp>

namespace pkmn { namespace qt {

    ItemListComboBox::ItemListComboBox(
        const QString& game,
        QWidget* parent
    ): QComboBox(parent)
    {
        pkmn::e_game game_enum = pkmn::string_to_game(game.toStdString());

        std::vector<std::string> items = pkmn::database::get_item_name_list(game_enum);
        for(const std::string& item: items)
        {
            addItem(QString::fromStdString(item));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_ItemListComboBox.cpp"
