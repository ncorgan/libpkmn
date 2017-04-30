/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_box_gcnimpl.hpp"
#include "pokemon_gcnimpl.hpp"

#include <stdexcept>

#define GC_RCAST   reinterpret_cast<LibPkmGC::GC::PokemonBox*>(_native)
#define COLO_RCAST reinterpret_cast<LibPkmGC::Colosseum::PokemonBox*>(_native)
#define XD_RCAST   reinterpret_cast<LibPkmGC::XD::PokemonBox*>(_native)

BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;

namespace pkmn {

    pokemon_box_gcnimpl::pokemon_box_gcnimpl(
        int game_id
    ): pokemon_box_impl(game_id)
    {
        if(_game_id == COLOSSEUM) {
            _native = reinterpret_cast<void*>(new LibPkmGC::Colosseum::PokemonBox);
        } else {
            _native = reinterpret_cast<void*>(new LibPkmGC::XD::PokemonBox);
        }
        _our_mem = true;

        _from_native();
    }

    pokemon_box_gcnimpl::pokemon_box_gcnimpl(
        int game_id,
        LibPkmGC::GC::PokemonBox* native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_box_gcnimpl::~pokemon_box_gcnimpl() {
        if(_our_mem) {
            if(_game_id == COLOSSEUM) {
                delete COLO_RCAST;
            } else {
                delete XD_RCAST;
            }
        }
    }

    std::string pokemon_box_gcnimpl::get_name() {
        return GC_RCAST->name->toUTF8();
    }

    void pokemon_box_gcnimpl::set_name(
        const std::string &name
    ) {
        if(name.size() > 8) {
            throw std::invalid_argument("Generation III box names have a maximum length of 8.");
        }

        GC_RCAST->name->fromUTF8(name.c_str());
    }

    int pokemon_box_gcnimpl::get_num_pokemon() {
        int num_pokemon = 0;
        for(int i = 0; i < get_capacity(); ++i) {
            if(GC_RCAST->pkm[i]->species > LibPkmGC::NoSpecies) {
                ++num_pokemon;
            }
        }

        return num_pokemon;
    }

    int pokemon_box_gcnimpl::get_capacity() {
        return 30;
    }

    void pokemon_box_gcnimpl::_from_native() {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        for(int i = 0; i < capacity; ++i) {
            _pokemon_list[i] = pkmn::make_shared<pokemon_gcnimpl>(
                                   dynamic_cast<LibPkmGC::GC::Pokemon*>(GC_RCAST->pkm[i]),
                                   _game_id
                               );
        }
    }
}
