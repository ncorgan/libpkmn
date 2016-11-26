/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt5/LocationListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt5 {

    LocationListComboBox::LocationListComboBox(
        const QString &game,
        bool wholeGeneration,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> locations = pkmn::database::get_location_list(
                                                 game.toStdString(),
                                                 wholeGeneration
                                             );
        for(auto iter = locations.begin(); iter != locations.end(); ++iter) {
            addItem(QString::fromStdString(*iter));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt5/moc_LocationListComboBox.cpp"
