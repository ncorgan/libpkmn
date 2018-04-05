/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_POKEMON_BOX_GBIMPL_IPP
#define PKMN_POKEMON_BOX_GBIMPL_IPP

#include "exception_internal.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <pksav/gen1/text.h>
#include <pksav/gen2/text.h>
#include <pksav/math/endian.h>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>
#include <type_traits>

#define NATIVE_LIST_RCAST (reinterpret_cast<list_type*>(_native))
#define NUM_LIST_SPECIES ((sizeof(NATIVE_LIST_RCAST->species)/sizeof(NATIVE_LIST_RCAST->species[0]))-1)

namespace pkmn {

    POKEMON_BOX_GBIMPL_TEMPLATE
    POKEMON_BOX_GBIMPL_CLASS::pokemon_box_gbimpl(
        int game_id
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new list_type);
        std::memset(_native, 0, sizeof(list_type));
        std::memset(NATIVE_LIST_RCAST->nicknames, 0x50, sizeof(NATIVE_LIST_RCAST->nicknames));
        std::memset(NATIVE_LIST_RCAST->otnames, 0x50, sizeof(NATIVE_LIST_RCAST->otnames));
        NATIVE_LIST_RCAST->species[NUM_LIST_SPECIES] = 0xFF;
        _our_mem = true;

        _from_native();
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    POKEMON_BOX_GBIMPL_CLASS::pokemon_box_gbimpl(
        int game_id,
        list_type* native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    POKEMON_BOX_GBIMPL_CLASS::~pokemon_box_gbimpl()
    {
        boost::lock_guard<POKEMON_BOX_GBIMPL_CLASS> lock(*this);

        if(_our_mem)
        {
            delete NATIVE_LIST_RCAST;
        }
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    std::string POKEMON_BOX_GBIMPL_CLASS::get_name()
    {
        if(std::is_same<list_type, struct pksav_gen1_pokemon_box>::value)
        {
            throw pkmn::feature_not_in_game_error("Box names", "Generation I");
        }
        else
        {
            boost::lock_guard<POKEMON_BOX_GBIMPL_CLASS> lock(*this);

            return _box_name;
        }
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    void POKEMON_BOX_GBIMPL_CLASS::set_name(
        const std::string &name
    )
    {
        if(std::is_same<list_type, struct pksav_gen1_pokemon_box>::value)
        {
            throw pkmn::feature_not_in_game_error("Box names", "Generation I");
        }
        else
        {
            pkmn::enforce_string_length(
                "Box name",
                name,
                0,
                8
            );

            boost::lock_guard<POKEMON_BOX_GBIMPL_CLASS> lock(*this);

            _box_name = name;
        }
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    int POKEMON_BOX_GBIMPL_CLASS::get_num_pokemon()
    {
        boost::lock_guard<POKEMON_BOX_GBIMPL_CLASS> lock(*this);

        return int(NATIVE_LIST_RCAST->count);
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    int POKEMON_BOX_GBIMPL_CLASS::get_capacity()
    {
        return int(sizeof(NATIVE_LIST_RCAST->entries)/sizeof(NATIVE_LIST_RCAST->entries[0]));
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    void POKEMON_BOX_GBIMPL_CLASS::set_pokemon(
        int index,
        pkmn::pokemon::sptr new_pokemon
    )
    {
        int num_pokemon = get_num_pokemon();
        int capacity = get_capacity();
        int max_index = std::min<int>(capacity-1, num_pokemon);

        pkmn::enforce_bounds("Box index", index, 0, max_index);

        if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data())
        {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        }
        else if(index < (num_pokemon-1) and new_pokemon->get_species() == "None")
        {
            throw std::invalid_argument("Generation I-II boxes store Pokémon contiguously.");
        }

        boost::lock_guard<POKEMON_BOX_GBIMPL_CLASS> lock(*this);

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

        // Copy the underlying memory to the box. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.

        // Make a copy of the current Pokémon in the given box slot so it can be preserved in an sptr
        // that owns its own memory.
        copy_box_pokemon<pksav_pc_pokemon_type, pksav_pokemon_party_data_type>(index);

        // Copy the new Pokémon's internals into the box's internals and create a new sptr.
        void* new_pokemon_native_pc_ptr = new_pokemon_impl_ptr->_native_pc;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_box_pokemon_impl_ptr->unlock();

        // Set the entry in the species list.
        NATIVE_LIST_RCAST->entries[index] = *reinterpret_cast<pksav_pc_pokemon_type*>(new_pokemon_native_pc_ptr);
        _pokemon_list[index] = std::make_shared<libpkmn_pokemon_type>(
                                   &NATIVE_LIST_RCAST->entries[index],
                                   _game_id
                               );

        // Don't set empty names.
        std::string new_pokemon_nickname = actual_new_pokemon->get_nickname();
        std::string new_pokemon_trainer_name = actual_new_pokemon->get_original_trainer_name();
        if(new_pokemon_nickname.size() == 0)
        {
            new_pokemon_nickname = "None";
        }
        if(new_pokemon_trainer_name.size() == 0)
        {
            new_pokemon_trainer_name = "None";
        }

        _pokemon_list[index]->set_nickname(new_pokemon_nickname);
        _pokemon_list[index]->set_original_trainer_name(new_pokemon_trainer_name);

        // Update the number of Pokémon in the box if needed.
        std::string new_species = actual_new_pokemon->get_species();
        if(index == num_pokemon and new_species != "None")
        {
            ++(NATIVE_LIST_RCAST->count);
        }
        else if(index == (num_pokemon-1) and new_species == "None")
        {
            --(NATIVE_LIST_RCAST->count);
        }

        // In Generation II, whether or not a Pokémon is in an egg is
        // stored in the list that stores it, not the Pokémon struct itself.
        if(std::is_same<list_type, struct pksav_gen2_pokemon_box>::value and actual_new_pokemon->is_egg())
        {
            NATIVE_LIST_RCAST->species[index] = GEN2_EGG_ID;
        }
        else
        {
            NATIVE_LIST_RCAST->species[index] = uint8_t(actual_new_pokemon->get_database_entry().get_pokemon_index());
        }

        if(_generation == 1)
        {
            PKSAV_CALL(
                pksav_gen1_export_text(
                    actual_new_pokemon->get_nickname().c_str(),
                    NATIVE_LIST_RCAST->nicknames[index],
                    10
                );
            )
            PKSAV_CALL(
                pksav_gen1_export_text(
                    actual_new_pokemon->get_original_trainer_name().c_str(),
                    NATIVE_LIST_RCAST->otnames[index],
                    7
                );
            )
        }
        else
        {
            PKSAV_CALL(
                pksav_gen2_export_text(
                    actual_new_pokemon->get_nickname().c_str(),
                    NATIVE_LIST_RCAST->nicknames[index],
                    10
                );
            )
            PKSAV_CALL(
                pksav_gen2_export_text(
                    actual_new_pokemon->get_original_trainer_name().c_str(),
                    NATIVE_LIST_RCAST->otnames[index],
                    7
                );
            )
        }

        // If this box is part of a save, set the Pokédex to have both
        // seen and caught the Pokémon.
        if(_pokedex.get())
        {
            std::string species = new_pokemon->get_species();

            if((species != "None") and (not new_pokemon->is_egg()))
            {
                _pokedex->set_has_seen(species, true);
                _pokedex->set_has_caught(species, true);
            }
        }
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    void POKEMON_BOX_GBIMPL_CLASS::_from_native()
    {
        boost::lock_guard<POKEMON_BOX_GBIMPL_CLASS> lock(*this);

        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        char nickname[11] = {0};
        char otname[8] = {0};

        int num_pokemon = get_num_pokemon();

        /*
         * Unfortuately, the count field may not be reliable, so we need to check
         * ourselves and fix it if it's wrong.
         */
        if(num_pokemon > 0 and NATIVE_LIST_RCAST->entries[num_pokemon-1].species == 0)
        {
            for(int i = 0; i < num_pokemon; ++i)
            {
                if(NATIVE_LIST_RCAST->entries[i].species == 0)
                {
                    NATIVE_LIST_RCAST->count = i;
                    break;
                }
            }
        }

        for(int i = 0; i < capacity; ++i)
        {
            /*
             * Memory is not necessarily zeroed-out past the num_pokemon point,
             * so we'll do it ourselves.
             */
            if(i >= num_pokemon and NATIVE_LIST_RCAST->entries[i].species > 0)
            {
                NATIVE_LIST_RCAST->species[i] = 0;
                std::memset(&NATIVE_LIST_RCAST->entries[i], 0, sizeof(pksav_pc_pokemon_type));
                std::memset(NATIVE_LIST_RCAST->nicknames[i], 0x50, sizeof(NATIVE_LIST_RCAST->nicknames[i]));
                std::memset(NATIVE_LIST_RCAST->otnames[i], 0x50, sizeof(NATIVE_LIST_RCAST->otnames[i]));
            }

            _pokemon_list[i] = std::make_shared<libpkmn_pokemon_type>(
                                   &NATIVE_LIST_RCAST->entries[i],
                                   _game_id
                               );

            // In Generation II, whether or not a Pokémon is in an egg is
            // stored in the list that stores it, not the Pokémon struct itself.
            if(std::is_same<list_type, struct pksav_gen2_pokemon_box>::value)
            {
                _pokemon_list[i]->set_is_egg(
                    (NATIVE_LIST_RCAST->species[i] == GEN2_EGG_ID)
                );
            }

            PKSAV_CALL(
                pksav_gen1_import_text(
                    NATIVE_LIST_RCAST->nicknames[i],
                    nickname,
                    10
                );
            )
            if(std::strlen(nickname) > 0)
            {
                _pokemon_list[i]->set_nickname(nickname);
            }

            PKSAV_CALL(
                pksav_gen1_import_text(
                    NATIVE_LIST_RCAST->otnames[i],
                    otname,
                    7
                );
            )
            if(std::strlen(otname) > 0)
            {
                _pokemon_list[i]->set_original_trainer_name(otname);
            }
        }
    }
}

#endif /* PKMN_POKEMON_BOX_GBIMPL_IPP */
