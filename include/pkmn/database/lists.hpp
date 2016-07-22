/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_LISTS_HPP
#define PKMN_DATABASE_LISTS_HPP

#include <pkmn/config.hpp>

#include <string>
#include <vector>

namespace pkmn { namespace database {

    PKMN_API void get_ability_list(
        int generation,
        std::vector<std::string> &abilities_out
    );

    PKMN_API void get_game_list(
        int generation,
        bool include_previous,
        std::vector<std::string> &games_out
    );

    PKMN_API void get_item_list(
        const std::string &game,
        std::vector<std::string> &games_out
    );

    PKMN_API void get_location_list(
        const std::string &game,
        bool whole_generation,
        std::vector<std::string> &locations_out
    );

    PKMN_API void get_move_list(
        const std::string &game,
        std::vector<std::string> &moves_out
    );

    PKMN_API void get_nature_list(
        std::vector<std::string> &natures_out
    );

    PKMN_API void get_pokemon_list(
        int generation,
        bool include_previous,
        std::vector<std::string> &pokemon_out
    );

    PKMN_API void get_region_list(
        std::vector<std::string> &regions_out
    );

    PKMN_API void get_ribbon_list(
        int generation,
        std::vector<std::string> &ribbons_out
    );

    PKMN_API void get_super_training_medal_list(
        std::vector<std::string> &super_training_medals_out
    );

    PKMN_API void get_type_list(
        const std::string &game,
        std::vector<std::string> &types_out
    );
}}

#endif /* PKMN_DATABASE_LISTS_HPP */
