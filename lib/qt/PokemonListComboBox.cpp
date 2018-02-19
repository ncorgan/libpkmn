/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/PokemonListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt {

    PokemonListComboBox::PokemonListComboBox(
        int generation,
        bool includePrevious,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> pokemon_list = pkmn::database::get_pokemon_list(
                                                    generation,
                                                    includePrevious
                                                );
        for(const std::string& pokemon: pokemon_list)
        {
            addItem(QString::fromStdString(pokemon));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_PokemonListComboBox.cpp"
