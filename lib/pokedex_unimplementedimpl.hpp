/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_UNIMPLEMENTEDIMPL_HPP
#define PKMN_POKEDEX_UNIMPLEMENTEDIMPL_HPP

#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/pokedex.hpp>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace pkmn
{
    class pokedex_unimplementedimpl: public pokedex
    {
        public:
            explicit pokedex_unimplementedimpl(
                int game_id
            ): _game_id(game_id)
            {}

            virtual ~pokedex_unimplementedimpl() = default;

            inline pkmn::e_game get_game() final
            {
                return pkmn::database::game_id_to_enum(_game_id);
            }

            inline bool has_seen(pkmn::e_species) final
            {
                return false;
            }

            inline void set_has_seen(pkmn::e_species, bool) final
            {
                const std::string error_message = "Pokédex ("
                                                + pkmn::database::game_id_to_name(_game_id)
                                                + ")";

                throw pkmn::unimplemented_error(error_message);
            }

            const std::vector<pkmn::e_species>& get_all_seen()
            {
                return _all_seen;
            }

            int get_num_seen() final
            {
                return 0;
            }

            inline bool has_caught(pkmn::e_species) final
            {
                return false;
            }

            inline void set_has_caught(pkmn::e_species, bool) final
            {
                const std::string error_message = "Pokédex ("
                                                + pkmn::database::game_id_to_name(_game_id)
                                                + ")";

                throw pkmn::unimplemented_error(error_message);
            }

            const std::vector<pkmn::e_species>& get_all_caught()
            {
                return _all_caught;
            }

            int get_num_caught() final
            {
                return 0;
            }

            void* get_native() final
            {
                return nullptr;
            }

        private:
            int _game_id;

            std::vector<pkmn::e_species> _all_seen;
            std::vector<pkmn::e_species> _all_caught;
    };
}

#endif /* PKMN_POKEDEX_UNIMPLEMENTEDIMPL_HPP */
