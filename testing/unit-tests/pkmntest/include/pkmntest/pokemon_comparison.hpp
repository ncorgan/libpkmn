/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_POKEMON_COMPARISON_HPP
#define PKMNTEST_POKEMON_COMPARISON_HPP

#include <pkmntest/config.hpp>

#include <pkmn/pokemon.hpp>

#include <pkmn/enums/game.hpp>
#include <pkmn/enums/item.hpp>
#include <pkmn/enums/species.hpp>

namespace pkmntest
{
    PKMNTEST_API pkmn::pokemon::sptr get_random_pokemon(
        pkmn::e_species species,
        pkmn::e_game game,
        const std::vector<pkmn::e_item>& item_list,
        const std::vector<pkmn::e_move>& move_list
    );

    PKMNTEST_API pkmn::pokemon::sptr get_random_pokemon(
        pkmn::e_game game,
        const std::vector<pkmn::e_species>& pokemon_list,
        const std::vector<pkmn::e_item>& item_list,
        const std::vector<pkmn::e_move>& move_list
    );

    PKMNTEST_API pkmn::pokemon::sptr get_random_pokemon(pkmn::e_game game);

    PKMNTEST_API void compare_pokemon(
        const pkmn::pokemon::sptr& pokemon1,
        const pkmn::pokemon::sptr& pokemon2
    );
}

#endif /* PKMNTEST_POKEMON_COMPARISON_HPP */
