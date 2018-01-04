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

    class pokemon_pc2
    {
        public:
            pokemon_pc2():
                _pokemon_pc(nullptr)
            {}

            pokemon_pc2(
                const pkmn::pokemon_pc::sptr& cpp_pokemon_pc
            ): _pokemon_pc(cpp_pokemon_pc)
            {
                _populate_list();
            }

            pokemon_pc2(
                const std::string& game
            ): _pokemon_pc(pkmn::pokemon_pc::make(game))
            {
                _populate_list();
            }

            bool operator==(
                const pokemon_pc2& rhs
            ) const
            {
                return (_pokemon_pc == rhs._pokemon_pc);
            }

            inline std::string get_game()
            {
                return _pokemon_pc->get_game();
            }

            inline int get_num_boxes()
            {
                return _pokemon_pc->get_num_boxes();
            }

            inline pkmn::swig::pokemon_box2 get_box(
                int index
            )
            {
                int num_boxes = _pokemon_pc->get_num_boxes();
                pkmn::enforce_bounds("Box index", index, 0, (num_boxes-1));

                return pkmn::swig::pokemon_box2(_pokemon_pc->get_box(index));
            }

            inline const std::vector<pkmn::swig::pokemon_box2>& as_vector()
            {
                return _pokemon_box_list;
            }

            // Copy the vector, since the const in the reference
            // is casted away.
            inline std::vector<std::string> get_box_names()
            {
                return _pokemon_pc->get_box_names();
            }

            inline uintmax_t cptr()
            {
                return uintmax_t(_pokemon_pc.get());
            }

        private:
            pkmn::pokemon_pc::sptr _pokemon_pc;

            std::vector<pkmn::swig::pokemon_box2> _pokemon_box_list;

            void _populate_list()
            {
                const pkmn::pokemon_box_list_t& internal_vector = _pokemon_pc->as_vector();
                _pokemon_box_list.reserve(internal_vector.size());
                for(size_t i = 0; i < internal_vector.size(); ++i)
                {
                    _pokemon_box_list.emplace_back(pkmn::swig::pokemon_box2(internal_vector[i]));
                }
            }
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_PC_HPP */
