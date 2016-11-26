/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt5/GameListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt5 {

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
        for(auto iter = games.begin(); iter != games.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt5/moc_GameListComboBox.cpp"
