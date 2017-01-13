/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_IMPL_HPP
#define PKMN_POKEMON_PC_IMPL_HPP

#include <pkmn/pokemon_pc.hpp>

#include <boost/noncopyable.hpp>

#include <string>

namespace pkmn {

    class pokemon_pc_impl: public pokemon_pc, public boost::noncopyable {
        public:
            pokemon_pc_impl() {}
            pokemon_pc_impl(
                int game_id
            );

            virtual ~pokemon_pc_impl() {}

            std::string get_game();

            pkmn::pokemon_box::sptr get_box(
                int index
            );

            const pkmn::pokemon_box_list_t& as_vector();

            const std::vector<std::string>& get_box_names();
            
            void* get_native();

        protected:
            pkmn::pokemon_box_list_t _box_list;
            std::vector<std::string> _box_names;

            void* _native;
            bool _our_mem;

            int _game_id, _generation;

            virtual void _from_native() = 0;
            virtual void _update_box_names() = 0;
    };

}

#endif /* PKMN_POKEMON_PC_IMPL_HPP */
