/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

    /*!
     * @brief Returns an alphabetized list of abilities available in the given generation.
     *
     * \param generation which generation
     * \throws pkmn::range_error if generation is not [3-6]
     */
    PKMN_API std::vector<std::string> get_ability_list(
        int generation
    );

    /*!
     * @brief Returns a list, sorted by version group, of games in (or up to) the given generation.
     *
     * \param generation which generation
     * \param include_previous whether or not to include games in generations earlier than the given one
     * \throws pkmn::range_error if generation is not [1-6]
     */
    PKMN_API std::vector<std::string> get_game_list(
        int generation,
        bool include_previous
    );

    /*!
     * @brief Returns an alphabetized list of Shadow Pokémon available in the specified Gamecube game.
     *
     * \param colosseum return Pokémon from Colosseum, XD otherwise
     */
    PKMN_API std::vector<std::string> get_gamecube_shadow_pokemon_list(
        bool colosseum
    );

    /*!
     * @brief Returns an alphabetized list of items available in the given game.
     *
     * \param game which game
     * \throws std::invalid_argument if the given game is invalid
     */
    PKMN_API std::vector<std::string> get_item_list(
        const std::string &game
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
    PKMN_API std::vector<std::string> get_location_list(
        const std::string &game,
        bool whole_generation
    );

    /*!
     * @brief Returns a list of moves (sorted by in-game index) of moves available in the given game.
     *
     * \param game which game
     */
    PKMN_API std::vector<std::string> get_move_list(
        const std::string &game
    );

    /*!
     * @brief Returns a list of natures sorted by in-game index.
     */
    PKMN_API std::vector<std::string> get_nature_list();

    /*!
     * @brief Returns a list (sorted by National Pokédex number) of Pokémon introduced in
     *        (or up to) the given generation.
     *
     * \param generation which generation
     * \param include_previous include Pokémon from previous generations.
     * \throws pkmn::range_error if generation is not [1-6]
     */
    PKMN_API std::vector<std::string> get_pokemon_list(
        int generation,
        bool include_previous
    );

    /*!
     * @brief Returns a list (sorted by their first generation) of regions in the Pokémon world.
     */
    PKMN_API std::vector<std::string> get_region_list();

    /*!
     * @brief Returns a list (ordered as in-game) of ribbons available in the given generation.
     *
     * \param generation which generation
     * \throws pkmn::range_error if generation is not [3-6]
     */
    PKMN_API std::vector<std::string> get_ribbon_list(
        int generation
    );

    /*!
     * @brief Returns a list (as presented in-game) of Super Training Medals in Generation VI.
     */
    PKMN_API std::vector<std::string> get_super_training_medal_list();

    /*!
     * @brief Returns a list of types available in the given game.
     *
     * \param game which game
     * \throws std::invalid_argument if the given game is invalid
     */
    PKMN_API std::vector<std::string> get_type_list(
        const std::string &game
    );
}}

#endif /* PKMN_DATABASE_LISTS_HPP */
