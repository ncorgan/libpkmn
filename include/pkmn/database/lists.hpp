/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_LISTS_HPP
#define PKMN_DATABASE_LISTS_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/ability.hpp>
#include <pkmn/enums/game.hpp>
#include <pkmn/enums/item.hpp>
#include <pkmn/enums/move.hpp>
#include <pkmn/enums/nature.hpp>
#include <pkmn/enums/species.hpp>
#include <pkmn/enums/type.hpp>

#include <string>
#include <vector>

namespace pkmn { namespace database {

    PKMN_API std::vector<pkmn::e_ability> get_ability_list(
        int generation
    );

    PKMN_API std::vector<std::string> get_ability_name_list(
        int generation
    );

    PKMN_API std::vector<pkmn::e_game> get_game_list(
        int generation,
        bool include_previous
    );

    PKMN_API std::vector<std::string> get_game_name_list(
        int generation,
        bool include_previous
    );

    PKMN_API std::vector<pkmn::e_species> get_gamecube_shadow_pokemon_list(
        bool colosseum
    );

    PKMN_API std::vector<std::string> get_gamecube_shadow_pokemon_name_list(
        bool colosseum
    );

    PKMN_API std::vector<pkmn::e_move> get_hm_move_list(
        pkmn::e_game game
    );

    PKMN_API std::vector<std::string> get_hm_move_name_list(
        pkmn::e_game game
    );

    PKMN_API std::vector<pkmn::e_item> get_item_list(
        pkmn::e_game game
    );

    PKMN_API std::vector<std::string> get_item_name_list(
        pkmn::e_game game
    );

    /*!
     * @brief Returns an alphabetized list of locations available in the given game (or generation).
     *
     * Note: even if whole_generation is set to true, Game Boy Advance locations will not appear in
     * a list of Gamecube locations, and vice versa.
     *
     * \param game which game
     * \param whole_generation include locations from all games in this generation
     * \throws std::invalid_argument if the given game is invalid
     */
    PKMN_API std::vector<std::string> get_location_name_list(
        pkmn::e_game game,
        bool whole_generation
    );

    PKMN_API std::vector<pkmn::e_move> get_move_list(pkmn::e_game game);

    PKMN_API std::vector<std::string> get_move_name_list(
        pkmn::e_game game
    );

    PKMN_API std::vector<pkmn::e_nature> get_nature_list();

    PKMN_API std::vector<std::string> get_nature_name_list();

    PKMN_API std::vector<pkmn::e_species> get_pokemon_list(
        int generation,
        bool include_previous
    );

    PKMN_API std::vector<std::string> get_pokemon_name_list(
        int generation,
        bool include_previous
    );

    /*!
     * @brief Returns a list (sorted by their first generation) of regions in the Pokémon world.
     */
    PKMN_API std::vector<std::string> get_region_name_list();

    /*!
     * @brief Returns a list (ordered as in-game) of ribbons available in the given generation.
     *
     * \param generation which generation
     * \throws std::out_of_range if generation is not [3-6]
     */
    PKMN_API std::vector<std::string> get_ribbon_name_list(
        int generation
    );

    /*!
     * @brief Returns a list (as presented in-game) of Super Training Medals in Generation VI.
     */
    PKMN_API std::vector<std::string> get_super_training_medal_name_list();

    PKMN_API std::vector<pkmn::e_move> get_tm_move_list(
        pkmn::e_game game
    );

    PKMN_API std::vector<std::string> get_tm_move_name_list(
        pkmn::e_game game
    );

    PKMN_API std::vector<pkmn::e_type> get_type_list(
        pkmn::e_game game
    );

    PKMN_API std::vector<std::string> get_type_name_list(
        pkmn::e_game game
    );
}}

#endif /* PKMN_DATABASE_LISTS_HPP */
