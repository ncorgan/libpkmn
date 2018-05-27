/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_HPP
#define PKMN_POKEMON_PARTY_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>

#include <memory>
#include <string>
#include <vector>

namespace pkmn
{
    /*!
     * @brief A list of the Pokémon a trainer carries with them.
     *
     * This class is an abstraction of the different in-game representations of
     * a Pokémon party throughout the different games, providing a common API
     * for interacting with the party.
     */
    class PKMN_API pokemon_party
    {
        public:
            /*!
             * @brief The actual interface for using the pokemon_party class.
             *
             * The pokemon_party class itself is abstract and thus cannot be
             * used directly.
             */
            typedef std::shared_ptr<pokemon_party> sptr;

            /*!
             * @brief The factory function for instantiating a Pokémon party.
             *
             * This function uses the game to determine which underlying
             * representation to use. This function will fail if given an
             * invalid game.
             *
             * \param game Which game the party corresponds to
             * \throws std::invalid_argument If the given game is invalid
             */
            static sptr make(
                const std::string& game
            );

            /*!
             * @brief Returns the game this party comes from.
             */
            virtual std::string get_game() = 0;

            /*!
             * @brief Returns the number of Pokémon currently in the party.
             *
             * This value will always be in the range [0-6].
             */
            virtual int get_num_pokemon() = 0;

            /*!
             * @brief Returns the Pokémon at the given position (0-based).
             *
             * \param index The position on the party whose Pokémon to return
             * \throws std::out_of_range If index is not in the range [0-5]
             */
            virtual const pkmn::pokemon::sptr& get_pokemon(
                int index
            ) = 0;

            /*!
             * @brief Copies the given Pokémon to the given position in the party (0-based).
             *
             * If the given Pokémon is not from the same game as the party, LibPKMN
             * will attempt to convert the Pokémon to the given game, and this
             * converted Pokémon will be added.
             *
             * Pokémon in parties are stored contiguously, so attempting to place
             * a Pokémon in a position past this point will result in an error.
             *
             * \param index The position on the team where to copy the Pokémon
             * \param new_pokemon The Pokémon to add to the party
             * \throws std::out_of_range If the index is outside the range [0-num_pokemon)
             * \throws std::invalid_argument If the new Pokémon is incompatible with the party's game
             */
            virtual void set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) = 0;

            /*!
             * @brief Returns a vector representation of the Pokémon party.
             */
            virtual const pkmn::pokemon_list_t& as_vector() = 0;

            #ifndef __DOXYGEN__
            pokemon_party() {}
            virtual ~pokemon_party() {}
            virtual void* get_native() = 0;
            #endif
    };

}

#endif /* PKMN_POKEMON_PARTY_HPP */
