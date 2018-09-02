/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/MoveListComboBox.hpp>

#include <pkmn/database/lists.hpp>
#include <pkmn/enums/enum_to_string.hpp>

namespace pkmn { namespace qt {

    MoveListComboBox::MoveListComboBox(
        const QString& game,
        QWidget* parent
    ): QComboBox(parent)
    {
        pkmn::e_game game_enum = pkmn::string_to_game(game.toStdString());

        std::vector<std::string> moves = pkmn::database::get_move_name_list(game_enum);
        for(const std::string& move: moves)
        {
            addItem(QString::fromStdString(move));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_MoveListComboBox.cpp"
