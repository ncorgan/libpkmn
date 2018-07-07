/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_box_gcnimpl.hpp"
#include "pokemon_gcnimpl.hpp"

#include <pkmn/exception.hpp>

#include <boost/thread/lock_guard.hpp>

#include <stdexcept>

namespace pkmn
{
    pokemon_box_gcnimpl::pokemon_box_gcnimpl(
        int game_id,
        LibPkmGC::GC::PokemonBox* p_libpkmgc_native
    ): pokemon_box_impl(game_id)
    {
        if(p_libpkmgc_native != nullptr)
        {
            _libpkmgc_box_uptr.reset(p_libpkmgc_native->clone());
        }
        else
        {
            if(_game_id == COLOSSEUM_ID)
            {
                _libpkmgc_box_uptr.reset(new LibPkmGC::Colosseum::PokemonBox);
            }
            else
            {
                _libpkmgc_box_uptr.reset(new LibPkmGC::XD::PokemonBox);
            }
        }

        _from_native();
    }

    std::string pokemon_box_gcnimpl::get_name()
    {
        boost::lock_guard<pokemon_box_gcnimpl> lock(*this);

        return _libpkmgc_box_uptr->name->toUTF8();
    }

    void pokemon_box_gcnimpl::set_name(
        const std::string& name
    )
    {
        pkmn::enforce_string_length(
            "Box name",
            name,
            0,
            8
        );

        boost::lock_guard<pokemon_box_gcnimpl> lock(*this);

        _libpkmgc_box_uptr->name->fromUTF8(name.c_str());
    }

    int pokemon_box_gcnimpl::get_num_pokemon()
    {
        boost::lock_guard<pokemon_box_gcnimpl> lock(*this);

        int num_pokemon = 0;
        for(int i = 0; i < get_capacity(); ++i)
        {
            if(_libpkmgc_box_uptr->pkm[i]->species > LibPkmGC::NoSpecies)
            {
                ++num_pokemon;
            }
        }

        return num_pokemon;
    }

    int pokemon_box_gcnimpl::get_capacity()
    {
        return 30;
    }

    void pokemon_box_gcnimpl::set_pokemon(
        int index,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        int max_index = get_capacity();
        pkmn::enforce_bounds("Box index", index, 0, max_index);

        if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data())
        {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        }

        boost::lock_guard<pokemon_box_gcnimpl> lock(*this);

        // If the given Pokémon isn't from this box's game, convert it if we can.
        pkmn::pokemon::sptr actual_new_pokemon;
        if(_game_id == new_pokemon->get_database_entry().get_game_id())
        {
            actual_new_pokemon = new_pokemon;
        }
        else
        {
            actual_new_pokemon = new_pokemon->to_game(get_game());
        }

        pokemon_impl* new_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(actual_new_pokemon.get());
        pokemon_impl* old_box_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(_pokemon_list[index].get());

        // Make sure no one else is using the Pokémon variables.
        boost::lock_guard<pokemon_impl> new_pokemon_lock(*new_pokemon_impl_ptr);
        old_box_pokemon_impl_ptr->lock();

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.

        void* old_box_pokemon_native_ptr = old_box_pokemon_impl_ptr->_native_pc;
        void* new_pokemon_native_ptr = new_pokemon_impl_ptr->_native_pc;

        // Make a copy of the current Pokémon in the given party slot so it can be preserved in an sptr
        // that owns its own memory.
        LibPkmGC::GC::Pokemon* box_pokemon_copy_ptr = reinterpret_cast<LibPkmGC::GC::Pokemon*>(
                                                            old_box_pokemon_impl_ptr->_native_pc
                                                        )->clone();

        if(_game_id == COLOSSEUM_ID)
        {
            rcast_equal<LibPkmGC::Colosseum::Pokemon>(
                old_box_pokemon_native_ptr,
                dynamic_cast<LibPkmGC::Colosseum::Pokemon*>(box_pokemon_copy_ptr)
            );
            rcast_equal<LibPkmGC::Colosseum::Pokemon>(
                new_pokemon_native_ptr,
                old_box_pokemon_native_ptr
            );
        }
        else
        {
            rcast_equal<LibPkmGC::XD::Pokemon>(
                old_box_pokemon_native_ptr,
                dynamic_cast<LibPkmGC::XD::Pokemon*>(box_pokemon_copy_ptr)
            );
            rcast_equal<LibPkmGC::XD::Pokemon>(
                new_pokemon_native_ptr,
                old_box_pokemon_native_ptr
            );
        }

        old_box_pokemon_impl_ptr->_native_pc = reinterpret_cast<void*>(box_pokemon_copy_ptr);
        old_box_pokemon_impl_ptr->_our_pc_mem = true;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_box_pokemon_impl_ptr->unlock();

        _pokemon_list[index] = std::make_shared<pokemon_gcnimpl>(
                                   dynamic_cast<LibPkmGC::GC::Pokemon*>(
                                       _libpkmgc_box_uptr->pkm[index]
                                   ),
                                   _game_id
                               );
    }

    std::string pokemon_box_gcnimpl::get_wallpaper()
    {
        throw pkmn::feature_not_in_game_error("Box wallpaper", get_game());
    }

    void pokemon_box_gcnimpl::set_wallpaper(const std::string&)
    {
        throw pkmn::feature_not_in_game_error("Box wallpaper", get_game());
    }

    void pokemon_box_gcnimpl::_from_native()
    {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        for(int party_index = 0; party_index < capacity; ++party_index)
        {
            _pokemon_list[party_index] = std::make_shared<pokemon_gcnimpl>(
                                             dynamic_cast<LibPkmGC::GC::Pokemon*>(
                                                 _libpkmgc_box_uptr->pkm[party_index]
                                             ),
                                             _game_id
                                         );
        }
    }
}
