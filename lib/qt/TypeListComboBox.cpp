/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/TypeListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt {

    TypeListComboBox::TypeListComboBox(
        pkmn::e_game game,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> types = pkmn::database::get_type_list(game);
        for(const std::string& type: types)
        {
            addItem(QString::fromStdString(type));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_TypeListComboBox.cpp"
