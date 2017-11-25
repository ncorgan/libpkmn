/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/GameListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt {

    GameListComboBox::GameListComboBox(
        int generation,
        bool includePrevious,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> games = pkmn::database::get_game_list(
                                             generation,
                                             includePrevious
                                         );
        for(const std::string& game: games) {
            addItem(QString::fromStdString(game));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_GameListComboBox.cpp"
