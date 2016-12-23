/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/MoveListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt {

    MoveListComboBox::MoveListComboBox(
        const QString &game,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> moves = pkmn::database::get_move_list(
                                             game.toStdString()
                                         );
        for(auto iter = moves.begin(); iter != moves.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_MoveListComboBox.cpp"
