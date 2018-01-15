/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_HPP
#define PKMN_GAME_SAVE_HPP

#include <pkmn/config.hpp>
#include <pkmn/item_bag.hpp>
#include <pkmn/item_list.hpp>
#include <pkmn/pokedex.hpp>
#include <pkmn/pokemon_party.hpp>
#include <pkmn/pokemon_pc.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace pkmn {

    class PKMN_API game_save {
        public:
            typedef pkmn::shared_ptr<game_save> sptr;

            /*!
             * @brief Returns what type of game save is at the given filepath.
             *
             * Possible return values:
             *  * "Red/Blue/Yellow"
             *  * "Gold/Silver"
             *  * "Crystal"
             *  * "Ruby/Sapphire/Emerald"
             *  * "Colosseum/XD"
             *
             * \param filepath The filepath to check
             */
            static std::string detect_type(
                const std::string &filepath
            );

            /*!
             * @brief Imports the save from the game save at the given file.
             *
             * \param filepath The filepath from which to import the save
             * \throws std::invalid_argument if the given filepath is invalid
             */
            static sptr from_file(
                const std::string &filepath
            );

            #ifndef __DOXYGEN__
            game_save() {};
            virtual ~game_save() {};
            #endif

            /*!
             * @brief Returns the path from which the given save was imported.
             */
            virtual std::string get_filepath() = 0;

            /*!
             * @brief Saves the game save at its current filepath.
             *
             * If the save operation fails, the save file will be in an unknown
             * state.
             *
             * \throws std::runtime_error if the save operation failed
             */
            virtual void save() = 0;

            /*!
             * @brief Saves the game save at the given filepath.
             *
             * No changes will be made to the file at the original path, and the
             * new filepath will be considered the current one.
             *
             * If the save operation fails, the save file will be in an unknown
             * state.
             *
             * \param filepath The new filepath for the game save
             */
            virtual void save_as(
                const std::string &filepath
            ) = 0;

            /*!
             * @brief Returns which game this save corresponds to.
             *
             * This is guaranteed to be valid within a given version group, but as
             * there is often no way to distinguish between saves within the same
             * version group, LibPKMN will attempt to use the filename to determine
             * the specific game.
             */
            virtual std::string get_game() = 0;

            /*!
             * @brief Returns the name of the player character.
             */
            virtual std::string get_trainer_name() = 0;

            /*!
             * @brief Sets the name of the player character.
             *
             * Valid player names are 1-7 characters.
             *
             * \param trainer_name The new trainer name
             * \throws std::invalid_argument if the given string is empty or longer than 7 characters
             */
            virtual void set_trainer_name(
                const std::string &trainer_name
            ) = 0;

            /*!
             * @brief Returns the player character's trainer ID.
             *
             * In Generation I-II games, trainer IDs are 16-bit, and the number returned here
             * will also fit in a 16-bit number. In Generation III and beyond, an extra 16 bits were
             * added. In either case, this function returns the full number.
             */
            virtual uint32_t get_trainer_id() = 0;

            /*!
             * @brief Sets the player character's trainer ID to the given value.
             *
             * In Generation I-II games, trainer IDs are 16-bit, and the number returned here
             * will also fit in a 16-bit number. In Generation III and beyond, an extra 16 bits were
             * added. To maintain a common interface, this function takes a 32-bit number as an input,
             * but if attempting to set a Generation I-II ID to a 32-bit number will fail.
             *
             * \param trainer_id The new trainer ID
             * \throws std::out_of_range if the ID is too large for the given game
             */
            virtual void set_trainer_id(
                uint32_t trainer_id
            ) = 0;

            /*!
             * @brief Returns the player character's public trainer ID.
             *
             * Trainer IDs are how the game determines if a given Pokémon was originally caught by
             * a given trainer. In Generation I-II games, trainer IDs are 16-bit. In Generation III
             * and beyond, another 16-bit "secret ID" was added, making the full ID 32 bits.
             *
             * For each generation, this function returns:
             *  * Generation I-II: the same as get_trainer_id()
             *  * Generation III+: the lower 16 bits of get_trainer_id()
             */
            virtual uint16_t get_trainer_public_id() = 0;

            /*!
             * @brief Sets the player character's public trainer ID.
             *
             * In Generation I-II, this is equivalent to calling get_trainer_id().
             *
             * \param trainer_public_id The new trainer public ID
             */
            virtual void set_trainer_public_id(
                uint16_t trainer_public_id
            ) = 0;

            /*!
             * @brief Returns the player character's secret trainer ID (Generation III+).
             *
             * Trainer IDs are how the game determines if a given Pokémon was originally caught by
             * a given trainer. In Generation I-II games, trainer IDs are 16-bit. In Generation III
             * and beyond, another 16-bit "secret ID" was added, making the full ID 32 bits; this is
             * the value returned.
             *
             * \throws pkmn::feature_not_in_game_error if the save is from Generation I-II
             */
            virtual uint16_t get_trainer_secret_id() = 0;

            /*!
             * @brief Sets the player character's secret trainer ID (Generation III+).
             *
             * Trainer IDs are how the game determines if a given Pokémon was originally caught by
             * a given trainer. In Generation I-II games, trainer IDs are 16-bit. In Generation III
             * and beyond, another 16-bit "secret ID" was added, making the full ID 32 bits; this is
             * the value set by this function.
             *
             * \param trainer_secret_id The new trainer secret ID
             * \throws pkmn::feature_not_in_game_error if the save is from Generation I-II
             */
            virtual void set_trainer_secret_id(
                uint16_t trainer_secret_id
            ) = 0;

            /*!
             * @brief Returns whether the player character is male or female.
             */
            virtual std::string get_trainer_gender() = 0;

            /*!
             * @brief Sets whether the player character is male or female.
             *
             * Pokémon Crystal was the first game that introduced a female playable character,
             * and Pokémon Colosseum and XD had a single male character. As such, using this function for
             * Gamecube saves or for games before Crystal will result in an error.
             *
             * Valid values: "Male", "Female"
             *
             * \param trainer_gender The new trainer gender
             * \throws std::invalid_argument if the given gender is not "Male" or "Female"
             * \throws pkmn::feature_not_in_game_error if the game only has one gender of playable character
             */
            virtual void set_trainer_gender(
                const std::string &trainer_gender
            ) = 0;

            /*!
             * @brief Returns the name of the rival character.
             *
             * Gamecube games have no rival character, so calling this function with those saves will result
             * in an error.
             *
             * \throws pkmn::feature_not_in_game_error if the save is for a Gamecube game
             */
            virtual std::string get_rival_name() = 0;

            /*!
             * @brief Sets the name of the rival character.
             *
             * Valid player names are 1-7 characters.
             *
             * Gamecube games have no rival character, so calling this function with those saves will result
             * in an error. The following games have set rival names that cannot be changed:
             *  * Ruby/Sapphire/Emerald
             *  * Black/White
             *  * X/Y
             *  * Omega Ruby/Alpha Sapphire
             *  * Sun/Moon
             *
             * \param rival_name The new rival name
             * \throws std::invalid_argument if the given rival name is empty or longer than 7 characters
             * \throws pkmn::feature_not_in_game_error if the game has no rival or the rival cannot be renamed
             */
            virtual void set_rival_name(
                const std::string &rival_name
            ) = 0;

            /*!
             * @brief Returns the amount of money the player is holding.
             *
             * The return value will be in the range [0-999999].
             */
            virtual int get_money() = 0;

            /*!
             * @brief Sets the amount of money the player is holding.
             *
             * Valid values are in the range [0-999999].
             *
             * \param money The new amount of money
             * \throws std::out_of_range if the input is not in the range [0-999999].
             */
            virtual void set_money(
                int money
            ) = 0;

            virtual pkmn::pokedex::sptr get_pokedex() = 0;

            /*!
             * @brief Returns the trainer's Pokémon party.
             */
            virtual pkmn::pokemon_party::sptr get_pokemon_party() = 0;

            /*!
             * @brief Returns the trainer's Pokémon PC.
             */
            virtual pkmn::pokemon_pc::sptr get_pokemon_pc() = 0;

            /*!
             * @brief Returns the trainer's item bag.
             */
            virtual pkmn::item_bag::sptr get_item_bag() = 0;

            /*!
             * @brief Returns the trainer's item PC (Generation I-III).
             *
             * \throws pkmn::feature_not_in_game_error if the save is from Generation IV+
             */
            virtual pkmn::item_list::sptr get_item_pc() = 0;
    };
}

#endif /* PKMN_GAME_SAVE_HPP */
