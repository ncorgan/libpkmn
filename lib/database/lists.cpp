/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/database/lists.hpp>

namespace pkmn { namespace database {

    std::vector<std::string> get_ability_list(
        int latest_generation
    ) {
        (void)latest_generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_game_list(
        int generation,
        bool include_previous
    ) {
        (void)generation;
        (void)include_previous;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_item_list(
        const std::string &game
    ) {
        (void)game;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_location_list(
        const std::string &game,
        bool whole_generation
    ) {
        (void)game;
        (void)whole_generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_move_list(
        int latest_generation
    ) {
        (void)latest_generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_nature_list() {
        return std::vector<std::string>();
    }

    std::vector<std::string> get_pokemon_list(
        int generation,
        bool include_previous
    ) {
        (void)generation;
        (void)include_previous;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_region_list() {
        return std::vector<std::string>();
    }

    std::vector<std::string> get_ribbon_list(
        int generation
    ) {
        (void)generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_super_training_medal_list() {
        return std::vector<std::string>();
    }

    std::vector<std::string> get_type_list(
        int generation
    ) {
        (void)generation;
        return std::vector<std::string>();
    }
}}
