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

namespace pkmn {

    POKEMON_BOX_GBIMPL_TEMPLATE
    POKEMON_BOX_GBIMPL_CLASS::pokemon_box_gbimpl(
        int game_id,
        const list_type* p_native
    ): pokemon_box_impl(game_id)
    {
        if(p_native != nullptr)
        {
            _pksav_box = *p_native;
        }
        else
        {
            std::memset(
                &_pksav_box,
                0,
                sizeof(_pksav_box)
            );
            std::memset(
                _pksav_box.nicknames,
                0x50,
                sizeof(_pksav_box.nicknames)
            );
            std::memset(
                _pksav_box.otnames,
                0x50,
                sizeof(_pksav_box.otnames)
            );

            _pksav_box.species[get_capacity()-1] = 0xFF;
        }

        _p_native = &_pksav_box;

        _from_native();
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
        const std::string& name
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
                PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH
            );

            boost::lock_guard<POKEMON_BOX_GBIMPL_CLASS> lock(*this);

            _box_name = name;
        }
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    int POKEMON_BOX_GBIMPL_CLASS::get_num_pokemon()
    {
        boost::lock_guard<POKEMON_BOX_GBIMPL_CLASS> lock(*this);

        return int(_pksav_box.count);
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    int POKEMON_BOX_GBIMPL_CLASS::get_capacity()
    {
        return int(sizeof(_pksav_box.entries)/sizeof(_pksav_box.entries[0]));
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    void POKEMON_BOX_GBIMPL_CLASS::set_pokemon(
        int index,
        const pkmn::pokemon::sptr& new_pokemon
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
        else if((index < (num_pokemon-1)) && (new_pokemon->get_species() == pkmn::e_species::NONE))
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

        // Make sure no one else is using the new Pokémon variable.
        libpkmn_pokemon_type* p_new_pokemon = dynamic_cast<libpkmn_pokemon_type*>(
                                                  actual_new_pokemon.get()
                                              );
        BOOST_ASSERT(p_new_pokemon != nullptr);
        boost::lock_guard<libpkmn_pokemon_type> new_pokemon_lock(*p_new_pokemon);

        // Copy the underlying memory to the box. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.
        //
        // Note: as we control the implementation, we know the PC data points
        // to the whole Pokémon data structure.
        rcast_equal<pksav_pc_pokemon_type>(
            actual_new_pokemon->get_native_pc_data(),
            &_pksav_box.entries[index]
        );
        _pokemon_list[index] = std::make_shared<libpkmn_pokemon_type>(
                                   &_pksav_box.entries[index],
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
        pkmn::e_species new_species = actual_new_pokemon->get_species();
        if((index == num_pokemon) && (new_species != pkmn::e_species::NONE))
        {
            ++(_pksav_box.count);
        }
        else if((index == (num_pokemon-1)) && (new_species == pkmn::e_species::NONE))
        {
            --(_pksav_box.count);
        }

        // In Generation II, whether or not a Pokémon is in an egg is
        // stored in the list that stores it, not the Pokémon struct itself.
        if(std::is_same<list_type, struct pksav_gen2_pokemon_box>::value and actual_new_pokemon->is_egg())
        {
            _pksav_box.species[index] = GEN2_EGG_ID;
        }
        else
        {
            _pksav_box.species[index] = uint8_t(actual_new_pokemon->get_database_entry().get_pokemon_index());
        }

        if(_generation == 1)
        {
            PKSAV_CALL(
                pksav_gen1_export_text(
                    actual_new_pokemon->get_nickname().c_str(),
                    _pksav_box.nicknames[index],
                    10
                );
            )
            PKSAV_CALL(
                pksav_gen1_export_text(
                    actual_new_pokemon->get_original_trainer_name().c_str(),
                    _pksav_box.otnames[index],
                    7
                );
            )
        }
        else
        {
            PKSAV_CALL(
                pksav_gen2_export_text(
                    actual_new_pokemon->get_nickname().c_str(),
                    _pksav_box.nicknames[index],
                    10
                );
            )
            PKSAV_CALL(
                pksav_gen2_export_text(
                    actual_new_pokemon->get_original_trainer_name().c_str(),
                    _pksav_box.otnames[index],
                    7
                );
            )
        }

        // If this box is part of a save, set the Pokédex to have both
        // seen and caught the Pokémon.
        if(_pokedex.get())
        {
            pkmn::e_species species = new_pokemon->get_species();

            if((species != pkmn::e_species::NONE) && !new_pokemon->is_egg())
            {
                _pokedex->set_has_seen(species, true);
                _pokedex->set_has_caught(species, true);
            }
        }
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    std::string POKEMON_BOX_GBIMPL_CLASS::get_wallpaper()
    {
        throw pkmn::feature_not_in_game_error("Box wallpaper", "Generation I-II");
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    void POKEMON_BOX_GBIMPL_CLASS::set_wallpaper(const std::string&)
    {
        throw pkmn::feature_not_in_game_error("Box wallpaper", "Generation I-II");
    }

    POKEMON_BOX_GBIMPL_TEMPLATE
    void POKEMON_BOX_GBIMPL_CLASS::_from_native()
    {
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
        if((num_pokemon > 0) && (_pksav_box.entries[num_pokemon-1].species == 0))
        {
            for(int i = 0; i < num_pokemon; ++i)
            {
                if(_pksav_box.entries[i].species == 0)
                {
                    _pksav_box.count = i;
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
            if((i >= num_pokemon) && (_pksav_box.entries[i].species > 0))
            {
                _pksav_box.species[i] = 0;
                std::memset(&_pksav_box.entries[i], 0, sizeof(pksav_pc_pokemon_type));
                std::memset(_pksav_box.nicknames[i], 0x50, sizeof(_pksav_box.nicknames[i]));
                std::memset(_pksav_box.otnames[i], 0x50, sizeof(_pksav_box.otnames[i]));
            }

            _pokemon_list[i] = std::make_shared<libpkmn_pokemon_type>(
                                   &_pksav_box.entries[i],
                                   _game_id
                               );

            // In Generation II, whether or not a Pokémon is in an egg is
            // stored in the list that stores it, not the Pokémon struct itself.
            if(std::is_same<list_type, struct pksav_gen2_pokemon_box>::value)
            {
                _pokemon_list[i]->set_is_egg(
                    (_pksav_box.species[i] == GEN2_EGG_ID)
                );
            }

            PKSAV_CALL(
                pksav_gen1_import_text(
                    _pksav_box.nicknames[i],
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
                    _pksav_box.otnames[i],
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

    POKEMON_BOX_GBIMPL_TEMPLATE
    void POKEMON_BOX_GBIMPL_CLASS::_to_native()
    {
        BOOST_ASSERT(static_cast<size_t>(get_capacity()) == _pokemon_list.size());

        std::memset(
            _pksav_box.otnames,
            0x50,
            sizeof(_pksav_box.otnames)
        );
        std::memset(
            _pksav_box.nicknames,
            0x50,
            sizeof(_pksav_box.nicknames)
        );

        for(size_t pokemon_index = 0;
            pokemon_index < _pokemon_list.size();
            ++pokemon_index)
        {
            pkmn::rcast_equal<pksav_pc_pokemon_type>(
                _pokemon_list[pokemon_index]->get_native_pc_data(),
                &_pksav_box.entries[pokemon_index]
            );

            // In Generation II, whether or not a Pokémon is in an egg is
            // stored in the list that stores it, not the Pokémon struct itself.
            if(std::is_same<list_type, struct pksav_gen2_pokemon_box>::value &&
               _pokemon_list[pokemon_index]->is_egg())
            {
                _pksav_box.species[pokemon_index] = GEN2_EGG_ID;
            }
            else
            {
                _pksav_box.species[pokemon_index] =
                    static_cast<uint8_t>(
                        _pokemon_list[pokemon_index]->get_database_entry().get_pokemon_index()
                    );
            }

            // Even though this function is meant to update the native representation,
            // the native count should have been updated along the way.
            if(pokemon_index < _pksav_box.count)
            {
                if(std::is_same<list_type, struct pksav_gen1_pokemon_box>::value)
                {
                    PKSAV_CALL(
                        pksav_gen1_export_text(
                            _pokemon_list[pokemon_index]->get_original_trainer_name().c_str(),
                            _pksav_box.otnames[pokemon_index],
                            PKSAV_GEN1_POKEMON_OTNAME_LENGTH
                        );
                    )
                    PKSAV_CALL(
                        pksav_gen1_export_text(
                            _pokemon_list[pokemon_index]->get_nickname().c_str(),
                            _pksav_box.nicknames[pokemon_index],
                            PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
                        );
                    )
                }
                else
                {
                    PKSAV_CALL(
                        pksav_gen2_export_text(
                            _pokemon_list[pokemon_index]->get_original_trainer_name().c_str(),
                            _pksav_box.otnames[pokemon_index],
                            PKSAV_GEN2_POKEMON_OTNAME_LENGTH
                        );
                    )
                    PKSAV_CALL(
                        pksav_gen2_export_text(
                            _pokemon_list[pokemon_index]->get_nickname().c_str(),
                            _pksav_box.nicknames[pokemon_index],
                            PKSAV_GEN2_POKEMON_NICKNAME_LENGTH
                        );
                    )
                }
            }
        }
    }
}

#endif /* PKMN_POKEMON_BOX_GBIMPL_IPP */
