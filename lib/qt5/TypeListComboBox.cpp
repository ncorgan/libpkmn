/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt5/TypeListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt5 {

    TypeListComboBox::TypeListComboBox(
        const QString &game,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> types = pkmn::database::get_type_list(
                                             game.toStdString()
                                         );
        for(auto iter = types.begin(); iter != types.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt5/moc_TypeListComboBox.cpp"
