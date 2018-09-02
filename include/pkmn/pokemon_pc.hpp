/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_HPP
#define PKMN_POKEMON_PC_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon_box.hpp>

#include <pkmn/enums/game.hpp>

#include <memory>
#include <string>
#include <vector>

namespace pkmn
{
    /*!
     * @brief A list of Pokémon boxes in which a trainer stores Pokémon
     *
     * This class is an abstraction of the different in-game representations of
     * a Pokémon PC throughout the different games, providing a common API
     * for interacting with the PC.
     */
    class PKMN_API pokemon_pc: public std::enable_shared_from_this<pokemon_pc>
    {
        public:
            /*!
             * @brief The actual interface for using the pokemon_box class.
             *
             * The pokemon_pc class itself is abstract and thus cannot be
             * used directly.
             */
            typedef std::shared_ptr<pokemon_pc> sptr;

            /*!
             * @brief The factory function for instantiating a Pokémon PC.
             *
             * This function uses the game to determine which underlying
             * representation to use. This function will fail if given an
             * invalid game.
             *
             * \param game Which game the PC corresponds to
             * \throws std::invalid_argument If the given game is invalid
             */
            static sptr make(pkmn::e_game game);

            /*!
             * @brief Returns the game this box comes from.
             */
            virtual pkmn::e_game get_game()  = 0;

            /*!
             * @brief Returns the number of boxes in this PC.
             */
            virtual int get_num_boxes() = 0;

            /*!
             * @brief Returns the box at the given position.
             *
             * \param index The position of the box to return
             */
            virtual const pkmn::pokemon_box::sptr& get_box(
                int index
            ) = 0;

            /*!
             * @brief Returns a vector representation of the PC.
             */
            virtual const pkmn::pokemon_box_list_t& as_vector() = 0;

            /*!
             * @brief Returns a list of box names.
             *
             * Generation I boxes have no name, so for a Generation I PC,
             * this call will fail.
             *
             * \throws pkmn::feature_not_in_game_error If the PC is from Generation I
             */
            virtual const std::vector<std::string>& get_box_names() = 0;

            #ifndef __DOXYGEN__
            pokemon_pc() {}
            virtual ~pokemon_pc() {}
            virtual void* get_native() = 0;
            #endif
    };

}

#endif /* PKMN_POKEMON_PC_HPP */
