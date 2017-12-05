/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_IMPL_HPP
#define PKMN_GAME_SAVE_IMPL_HPP

#include <pkmn/game_save.hpp>

#include <boost/noncopyable.hpp>

BOOST_STATIC_CONSTEXPR int MONEY_MAX_VALUE = 999999;

namespace pkmn {

    class game_save_impl: public game_save, boost::noncopyable {
        public:
            game_save_impl() {};
            explicit game_save_impl(
                const std::string &filepath
            );

            virtual ~game_save_impl() {};

            std::string get_filepath();

            void save();

            std::string get_game();

            virtual pkmn::pokedex::sptr get_pokedex() override;

            pkmn::pokemon_party::sptr get_pokemon_party();

            pkmn::pokemon_pc::sptr get_pokemon_pc();

            pkmn::item_bag::sptr get_item_bag();

            pkmn::item_list::sptr get_item_pc();

        protected:
            std::string _filepath;

            pkmn::pokedex::sptr _pokedex;
            pkmn::pokemon_party::sptr _pokemon_party;
            pkmn::pokemon_pc::sptr _pokemon_pc;
            pkmn::item_bag::sptr _item_bag;
            pkmn::item_list::sptr _item_pc;

            int _game_id;
    };

}

#endif /* PKMN_GAME_SAVE_IMPL_HPP */
