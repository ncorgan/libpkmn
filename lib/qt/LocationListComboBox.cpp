/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/LocationListComboBox.hpp>

#include <pkmn/database/lists.hpp>
#include <pkmn/enums/enum_to_string.hpp>

namespace pkmn { namespace qt {

    LocationListComboBox::LocationListComboBox(
        const QString& game,
        bool wholeGeneration,
        QWidget* parent
    ): QComboBox(parent)
    {
        pkmn::e_game game_enum = pkmn::string_to_game(game.toStdString());

        std::vector<std::string> locations = pkmn::database::get_location_name_list(
                                                 game_enum,
                                                 wholeGeneration
                                             );
        for(const std::string& location: locations)
        {
            addItem(QString::fromStdString(location));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_LocationListComboBox.cpp"
