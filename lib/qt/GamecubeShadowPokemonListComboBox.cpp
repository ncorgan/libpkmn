/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/qt/GamecubeShadowPokemonListComboBox.hpp>

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace qt {

    GamecubeShadowPokemonListComboBox::GamecubeShadowPokemonListComboBox(
        bool colosseum,
        QWidget* parent
    ): QComboBox(parent)
    {
        std::vector<std::string> shadow_pokemon = pkmn::database::get_gamecube_shadow_pokemon_name_list(colosseum);
        for(const std::string& gamecube_shadow_pokemon: shadow_pokemon)
        {
            addItem(QString::fromStdString(gamecube_shadow_pokemon));
        }

        setEditable(false);
    }

}}

// Generated at build time
#include "../include/pkmn/qt/moc_GamecubeShadowPokemonListComboBox.cpp"
