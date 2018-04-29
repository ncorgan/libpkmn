/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_HPP
#define PKMN_POKEMON_BOX_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>

#include <memory>
#include <string>
#include <vector>

namespace pkmn
{
    /*!
     * @brief A list of the Pokémon a trainer stores in a PC box.
     *
     * This class is an abstraction of the different in-game representations of
     * a Pokémon box throughout the different games, providing a common API
     * for interacting with the box.
     */
    class PKMN_API pokemon_box
    {
        public:
            /*!
             * @brief The actual interface for using the pokemon_box class.
             *
             * The pokemon_box class itself is abstract and thus cannot be
             * used directly.
             */
            typedef std::shared_ptr<pokemon_box> sptr;

            /*!
             * @brief The factory function for instantiating a Pokémon box.
             *
             * This function uses the game to determine which underlying
             * representation to use. This function will fail if given an
             * invalid game.
             *
             * \param game Which game the box corresponds to
             * \throws std::invalid_argument If the given game is invalid
             */
            static sptr make(
                const std::string& game
            );

            /*!
             * @brief Returns this box's name.
             *
             * Box names were only supported in Generation II, so this call
             * will fail for Generation I boxes.
             *
             * \throws pkmn::feature_not_in_game_error If the box is from a Generation I game
             */
            virtual std::string get_name() = 0;

            /*!
             * @brief Sets this box's name.
             *
             * Valid box names are of length [1-8].
             *
             * Box names were only supported in Generation II, so this call
             * will fail for Generation I boxes.
             *
             * \param name The new name for the box
             * \throws std::invalid_argument If the box name is not of length [1-8]
             * \throws pkmn::feature_not_in_game_error If the box is from a Generation I game
             */
            virtual void set_name(
                const std::string& name
            ) = 0;

            /*!
             * @brief Returns the game this box comes from.
             */
            virtual std::string get_game()  = 0;

            /*!
             * @brief Returns the number of Pokémon in this box.
             *
             * This number will always be of length [0-capacity].
             */
            virtual int get_num_pokemon() = 0;

            /*!
             * @brief Returns the number of Pokémon this box can hold.
             */
            virtual int get_capacity() = 0;

            /*!
             * @brief Returns the Pokémon at the given position (0-based).
             *
             * \param index The position on the box whose Pokémon to return
             * \throws std::out_of_range If index is not in the range [0-5]
             */
            virtual pkmn::pokemon::sptr get_pokemon(
                int index
            ) = 0;

            /*!
             * @brief Copies the given Pokémon to the given position in the box (0-based).
             *
             * If the given Pokémon is not from the same game as the box, LibPKMN
             * will attempt to convert the Pokémon to the given game, and this
             * converted Pokémon will be added.
             *
             * In Generations I-II, Pokémon in boxes are stored contiguously, so
             * attempting to place a Pokémon in a position past this point will result
             * in an error.
             *
             * \param index The position on the team where to copy the Pokémon
             * \param new_pokemon The Pokémon to add to the box
             * \throws std::out_of_range If the index is outside the range [0-num_pokemon)
             *                           for Generation I-II boxes
             * \throws std::out_of_range If the index is outside the range [0-capacity)
             * \throws std::invalid_argument If the new Pokémon is incompatible with the box's game
             */
            virtual void set_pokemon(
                int index,
                pkmn::pokemon::sptr new_pokemon
            ) = 0;

            /*!
             * @brief Returns a vector representation of the Pokémon box.
             */
            virtual const pkmn::pokemon_list_t& as_vector() = 0;

            #ifndef __DOXYGEN__
            pokemon_box() {}
            virtual ~pokemon_box() {}
            virtual void* get_native() = 0;
            #endif
    };

    typedef std::vector<pkmn::pokemon_box::sptr> pokemon_box_list_t;

}

#endif /* PKMN_POKEMON_BOX_HPP */
