/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_HPP
#define PKMN_POKEDEX_HPP

#include <pkmn/config.hpp>

#include <memory>
#include <string>
#include <vector>

namespace pkmn
{
    /*!
     * @brief A class that represents an in-game Pokédex, which tracks what Pokémon
     *        have been seen and caught.
     */
    class PKMN_API pokedex
    {
        public:
            typedef std::shared_ptr<pokedex> sptr;

            /*!
             * @brief Factory function to instantiate a Pokédex.
             *
             * By default, the Pokédex will have zero Pokémon seen and caught.
             *
             * \param game Which game this Pokédex corresponds to
             * \throws pkmn::feature_not_in_game_error If game is a Gamecube game
             */
            static sptr make(
                const std::string& game
            );

            //! Returns the game this Pokédex corresponds to.
            virtual std::string get_game() = 0;

            /*!
             * @brief Returns whether a given Pokémon is registered as seen.
             *
             * \param species The Pokémon species to query
             * \throws std::invalid_argument If the species is invalid or is not
             *                               present in the given game
             */
            virtual bool has_seen(
                const std::string& species
            ) = 0;

            /*!
             * @brief Set whether a given Pokémon has been seen.
             *
             * \param species The Pokémon species to set
             * \param has_seen_value If the Pokémon is set to be seen
             * \throws std::invalid_argument If the species is invalid or is not
             *                               present in the given game
             */
            virtual void set_has_seen(
                const std::string& species,
                bool has_seen_value
            ) = 0;

            /*!
             * @brief Returns a list of all Pokémon registered as seen, ordered
             *        by National Pokédex number.
             */
            virtual const std::vector<std::string>& get_all_seen() = 0;

            /*!
             * @brief Returns the number of Pokémon registered as seen.
             */
            virtual int get_num_seen() = 0;

            /*!
             * @brief Returns whether a given Pokémon is registered as caught.
             *
             * \param species The Pokémon species to query
             * \throws std::invalid_argument If the species is invalid or is not
             *                               present in the given game
             */
            virtual bool has_caught(
                const std::string& species
            ) = 0;

            /*!
             * @brief Set whether a given Pokémon has been caught.
             *
             * \param species The Pokémon species to set
             * \param has_caught_value If the Pokémon is set to be caught
             * \throws std::invalid_argument If the species is invalid or is not
             *                               present in the given game
             */
            virtual void set_has_caught(
                const std::string& species,
                bool has_caught_value
            ) = 0;

            /*!
             * @brief Returns a list of all Pokémon registered as caught, ordered
             *        by National Pokédex number.
             */
            virtual const std::vector<std::string>& get_all_caught() = 0;

            /*!
             * @brief Returns the number of Pokémon registered as caught.
             */
            virtual int get_num_caught() = 0;

            #ifndef __DOXYGEN__
            pokedex() {};
            virtual ~pokedex() {};
            virtual void* get_native() = 0;
            #endif
    };
}

#endif /* PKMN_POKEDEX_HPP */
