/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_PC_HPP
#define CPP_WRAPPERS_POKEMON_PC_HPP

#include "exception_internal.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon_pc.hpp>

#include "swig/modules/cpp_wrappers/pokemon_box.hpp"

namespace pkmn { namespace swig {

    class pokemon_pc
    {
        public:
            pokemon_pc():
                _pokemon_pc(nullptr)
            {}

            pokemon_pc(
                const pkmn::pokemon_pc::sptr& cpp_pokemon_pc
            ): _pokemon_pc(cpp_pokemon_pc)
            {
                _populate_list();
            }

            pokemon_pc(
                const std::string& game
            ): _pokemon_pc(pkmn::pokemon_pc::make(game))
            {
                _populate_list();
            }

            pokemon_pc(
                const pokemon_pc& other
            ): _pokemon_pc(other._pokemon_pc),
               _pokemon_box_list(other._pokemon_box_list)
            {}

            bool operator==(
                const pokemon_pc& rhs
            ) const
            {
                return (_pokemon_pc == rhs._pokemon_pc);
            }

            PKMN_INLINE std::string get_game()
            {
                return _pokemon_pc->get_game();
            }

            PKMN_INLINE int get_num_boxes()
            {
                return _pokemon_pc->get_num_boxes();
            }

            PKMN_INLINE pkmn::swig::pokemon_box get_box(
                int index
            )
            {
                int num_boxes = _pokemon_pc->get_num_boxes();
                pkmn::enforce_bounds("Box index", index, 0, (num_boxes-1));

                return pkmn::swig::pokemon_box(_pokemon_pc->get_box(index));
            }

            PKMN_INLINE const std::vector<pkmn::swig::pokemon_box>& as_vector()
            {
                return _pokemon_box_list;
            }

            PKMN_INLINE const std::vector<std::string>& get_box_names()
            {
                return _pokemon_pc->get_box_names();
            }

        private:
            pkmn::pokemon_pc::sptr _pokemon_pc;

            std::vector<pkmn::swig::pokemon_box> _pokemon_box_list;

            void _populate_list()
            {
                const pkmn::pokemon_box_list_t& internal_vector = _pokemon_pc->as_vector();
                _pokemon_box_list.reserve(internal_vector.size());
                for(size_t i = 0; i < internal_vector.size(); ++i)
                {
                    _pokemon_box_list.emplace_back(pkmn::swig::pokemon_box(internal_vector[i]));
                }
            }
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_PC_HPP */
