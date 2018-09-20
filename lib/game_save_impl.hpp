/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_IMPL_HPP
#define PKMN_GAME_SAVE_IMPL_HPP

#include <pkmn/game_save.hpp>

#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <cstring>
#include <vector>

BOOST_STATIC_CONSTEXPR int MONEY_MAX_VALUE = 999999;

namespace pkmn {

    class game_save_impl: public game_save,
                          private boost::noncopyable,
                          public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            game_save_impl() {};
            game_save_impl(
                const std::string& filepath,
                std::vector<uint8_t>&& raw
            );

            virtual ~game_save_impl() {};

            std::string get_filepath() final;

            void save() final;

            pkmn::e_game get_game() final;

            const pkmn::pokedex::sptr& get_pokedex() final;

            const pkmn::pokemon_party::sptr& get_pokemon_party() final;

            const pkmn::pokemon_pc::sptr& get_pokemon_pc() final;

            const pkmn::item_bag::sptr& get_item_bag() final;

            const pkmn::item_list::sptr& get_item_pc() final;

        protected:
            std::string _filepath;

            pkmn::pokedex::sptr _pokedex;
            pkmn::pokemon_party::sptr _pokemon_party;
            pkmn::pokemon_pc::sptr _pokemon_pc;
            pkmn::item_bag::sptr _item_bag;
            pkmn::item_list::sptr _item_pc;

            int _game_id;

            std::vector<uint8_t> _raw;

            template <typename gb_pokedex_type>
            void save_gb_pokedex(
                gb_pokedex_type* p_save_pokedex,
                size_t num_bytes
            )
            {
                BOOST_ASSERT(p_save_pokedex != nullptr);

                const gb_pokedex_type* p_pokedex_copy =
                    static_cast<gb_pokedex_type*>(
                        _pokedex->get_native()
                    );

                std::memcpy(
                    p_save_pokedex->p_seen,
                    p_pokedex_copy->p_seen,
                    num_bytes
                );
                std::memcpy(
                    p_save_pokedex->p_owned,
                    p_pokedex_copy->p_owned,
                    num_bytes
                );
            }
    };

}

#endif /* PKMN_GAME_SAVE_IMPL_HPP */
