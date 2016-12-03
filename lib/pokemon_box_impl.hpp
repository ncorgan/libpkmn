/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_IMPL_HPP
#define PKMN_POKEMON_BOX_IMPL_HPP

#include <pkmn/pokemon_box.hpp>

#include <boost/noncopyable.hpp>

#include <string>

namespace pkmn {

    class pokemon_box_impl: public pokemon_box, public boost::noncopyable {
        public:
            pokemon_box_impl() {}
            pokemon_box_impl(
                int game_id
            );

            virtual ~pokemon_box_impl() {}

            std::string get_game();

            pkmn::pokemon::sptr get_pokemon(
                int index
            );

            const pkmn::pokemon_list_t& as_vector();
            
            void* get_native();

        protected:
            pkmn::pokemon_list_t _pokemon_list;
            std::string _box_name;

            void* _native;
            bool _our_mem;

            int _game_id;

            virtual void _from_native() = 0;
    };

}

#endif /* PKMN_POKEMON_BOX_IMPL_HPP */
