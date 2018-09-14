/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_POKEMON_PARTY_GBIMPL_IPP
#define PKMN_POKEMON_PARTY_GBIMPL_IPP

#include "exception_internal.hpp"
#include "pksav/pksav_call.hpp"
#include "types/mutex_helpers.hpp"

#include <pksav/gen1/text.h>
#include <pksav/gen2/text.h>

#include <cstring>
#include <stdexcept>

namespace pkmn {

    POKEMON_PARTY_GBIMPL_TEMPLATE
    POKEMON_PARTY_GBIMPL_CLASS::pokemon_party_gbimpl(
        int game_id,
        list_type* p_native
    ): pokemon_party_impl(game_id)
    {
        if(p_native != nullptr)
        {
            _pksav_party = *p_native;
        }
        else
        {
            std::memset(
                &_pksav_party,
                0,
                sizeof(_pksav_party)
            );
            std::memset(
                _pksav_party.nicknames,
                0x50,
                sizeof(_pksav_party.nicknames)
            );
            std::memset(
                _pksav_party.otnames,
                0x50,
                sizeof(_pksav_party.otnames)
            );
        }

        _p_native = &_pksav_party;

        _from_native();
    }

    POKEMON_PARTY_GBIMPL_TEMPLATE
    int POKEMON_PARTY_GBIMPL_CLASS::get_num_pokemon()
    {
        pkmn::lock_guard<POKEMON_PARTY_GBIMPL_CLASS> lock(*this);

        return int(_pksav_party.count);
    }

    POKEMON_PARTY_GBIMPL_TEMPLATE
    void POKEMON_PARTY_GBIMPL_CLASS::_set_pokemon(
        int index,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        int num_pokemon = get_num_pokemon();
        int max_index = std::min<int>(PARTY_SIZE-1, num_pokemon);

        pkmn::enforce_bounds("Party index", index, 0, max_index);

        if((index < (num_pokemon-1)) && (new_pokemon->get_species() == pkmn::e_species::NONE))
        {
            throw std::invalid_argument("Parties store Pokémon contiguously.");
        }

        pkmn::lock_guard<POKEMON_PARTY_GBIMPL_CLASS> lock(*this);

        // Make sure no one else is using the new Pokémon variable.
        libpkmn_pokemon_type* p_new_pokemon = dynamic_cast<libpkmn_pokemon_type*>(
                                                  new_pokemon.get()
                                              );
        BOOST_ASSERT(p_new_pokemon != nullptr);
        pkmn::lock_guard<libpkmn_pokemon_type> new_pokemon_lock(*p_new_pokemon);

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.
        rcast_equal<pksav_party_pokemon_type>(
            new_pokemon->get_native(),
            &_pksav_party.party[index]
        );
        _pokemon_list[index] = std::make_shared<libpkmn_pokemon_type>(
                                   &_pksav_party.party[index],
                                   _game_id
                               );

        // Don't set empty names.
        std::string new_pokemon_nickname = new_pokemon->get_nickname();
        std::string new_pokemon_trainer_name = new_pokemon->get_original_trainer_name();
        if(new_pokemon_nickname.empty())
        {
            new_pokemon_nickname = "None";
        }
        if(new_pokemon_trainer_name.empty())
        {
            new_pokemon_trainer_name = "None";
        }

        _pokemon_list[index]->set_nickname(new_pokemon_nickname);
        _pokemon_list[index]->set_original_trainer_name(new_pokemon_trainer_name);

        // In Generation II, whether or not a Pokémon is in an egg is
        // stored in the list that stores it, not the Pokémon struct itself.
        if(std::is_same<list_type, struct pksav_gen2_pokemon_party>::value and new_pokemon->is_egg())
        {
            _pksav_party.species[index] = GEN2_EGG_ID;
        }
        else
        {
            _pksav_party.species[index] = uint8_t(new_pokemon->get_database_entry().get_pokemon_index());
        }

        // Update the number of Pokémon in the party if needed.
        pkmn::e_species new_species = new_pokemon->get_species();
        if((index == num_pokemon) && (new_species != pkmn::e_species::NONE))
        {
            ++(_pksav_party.count);
        }
        else if((index == (num_pokemon-1)) && (new_species == pkmn::e_species::NONE))
        {
            --(_pksav_party.count);
        }

        if(_generation == 1)
        {
            PKSAV_CALL(
                pksav_gen1_export_text(
                    new_pokemon->get_nickname().c_str(),
                    _pksav_party.nicknames[index],
                    PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
                );
            )
            PKSAV_CALL(
                pksav_gen1_export_text(
                    new_pokemon->get_original_trainer_name().c_str(),
                    _pksav_party.otnames[index],
                    PKSAV_GEN1_POKEMON_OTNAME_LENGTH
                );
            )
        }
        else
        {
            PKSAV_CALL(
                pksav_gen2_export_text(
                    new_pokemon->get_nickname().c_str(),
                    _pksav_party.nicknames[index],
                    PKSAV_GEN2_POKEMON_NICKNAME_LENGTH
                );
            )
            PKSAV_CALL(
                pksav_gen2_export_text(
                    new_pokemon->get_original_trainer_name().c_str(),
                    _pksav_party.otnames[index],
                    PKSAV_GEN2_POKEMON_OTNAME_LENGTH
                );
            )
        }

        // If this party is part of a save, set the Pokédex to have both
        // seen and caught the Pokémon.
        if(_pokedex.get())
        {
            // TODO: use new Pokémon function
            pkmn::e_species species = pkmn::e_species(new_pokemon->get_database_entry().get_species_id());

            if((species != pkmn::e_species::NONE) && !new_pokemon->is_egg())
            {
                _pokedex->set_has_seen(species, true);
                _pokedex->set_has_caught(species, true);
            }
        }
    }

    POKEMON_PARTY_GBIMPL_TEMPLATE
    void POKEMON_PARTY_GBIMPL_CLASS::_from_native()
    {
        pkmn::lock_guard<POKEMON_PARTY_GBIMPL_CLASS> lock(*this);

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(PARTY_SIZE);

        char nickname[PKSAV_GEN1_POKEMON_NICKNAME_LENGTH+1] = {0};
        char otname[PKSAV_GEN1_POKEMON_OTNAME_LENGTH+1] = {0};

        int num_pokemon = get_num_pokemon();

        for(int party_index = 0; party_index < PARTY_SIZE; ++party_index)
        {
            /*
             * Memory is not necessarily zeroed-out past the num_pokemon point,
             * so we'll do it ourselves.
             */
            if((party_index >= num_pokemon) && (_pksav_party.party[party_index].pc_data.species > 0))
            {
                _pksav_party.species[party_index] = 0;
                std::memset(
                    &_pksav_party.party[party_index],
                    0,
                    sizeof(_pksav_party.party[party_index])
                );
                std::memset(
                    _pksav_party.nicknames[party_index],
                    0x50,
                    sizeof(_pksav_party.nicknames[party_index])
                );
                std::memset(
                    _pksav_party.otnames[party_index],
                    0x50,
                    sizeof(_pksav_party.otnames[party_index])
                );
            }

            _pokemon_list[party_index] = std::make_shared<libpkmn_pokemon_type>(
                                             &_pksav_party.party[party_index],
                                             _game_id
                                         );

            // In Generation II, whether or not a Pokémon is in an egg is
            // stored in the list that stores it, not the Pokémon struct itself.
            if(std::is_same<list_type, struct pksav_gen2_pokemon_party>::value)
            {
                _pokemon_list[party_index]->set_is_egg(
                    (_pksav_party.species[party_index] == GEN2_EGG_ID)
                );
            }

            if(_generation == 1)
            {
                PKSAV_CALL(
                    pksav_gen1_import_text(
                        _pksav_party.nicknames[party_index],
                        nickname,
                        PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
                    );
                )
                if(std::strlen(nickname) > 0)
                {
                    _pokemon_list[party_index]->set_nickname(nickname);
                }

                PKSAV_CALL(
                    pksav_gen1_import_text(
                        _pksav_party.otnames[party_index],
                        otname,
                        PKSAV_GEN1_POKEMON_OTNAME_LENGTH
                    );
                )
                if(std::strlen(otname) > 0)
                {
                    _pokemon_list[party_index]->set_original_trainer_name(otname);
                }
            }
            else
            {
                PKSAV_CALL(
                    pksav_gen2_import_text(
                        _pksav_party.nicknames[party_index],
                        nickname,
                        PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
                    );
                )
                if(std::strlen(nickname) > 0)
                {
                    _pokemon_list[party_index]->set_nickname(nickname);
                }

                PKSAV_CALL(
                    pksav_gen2_import_text(
                        _pksav_party.otnames[party_index],
                        otname,
                        PKSAV_GEN1_POKEMON_OTNAME_LENGTH
                    );
                )
                if(std::strlen(otname) > 0)
                {
                    _pokemon_list[party_index]->set_original_trainer_name(otname);
                }
            }
        }
    }

    POKEMON_PARTY_GBIMPL_TEMPLATE
    void POKEMON_PARTY_GBIMPL_CLASS::_to_native()
    {
        BOOST_ASSERT(_pokemon_list.size() == PKSAV_GEN1_PARTY_NUM_POKEMON);

        std::memset(
            _pksav_party.otnames,
            0x50,
            sizeof(_pksav_party.otnames)
        );
        std::memset(
            _pksav_party.nicknames,
            0x50,
            sizeof(_pksav_party.nicknames)
        );

        for(size_t pokemon_index = 0;
            pokemon_index < _pokemon_list.size();
            ++pokemon_index)
        {
            pkmn::rcast_equal<pksav_party_pokemon_type>(
                _pokemon_list[pokemon_index]->get_native(),
                &_pksav_party.party[pokemon_index]
            );

            // In Generation II, whether or not a Pokémon is in an egg is
            // stored in the list that stores it, not the Pokémon struct itself.
            if(std::is_same<list_type, struct pksav_gen2_pokemon_party>::value &&
               _pokemon_list[pokemon_index]->is_egg())
            {
                _pksav_party.species[pokemon_index] = GEN2_EGG_ID;
            }
            else
            {
                _pksav_party.species[pokemon_index] =
                    static_cast<uint8_t>(
                        _pokemon_list[pokemon_index]->get_database_entry().get_pokemon_index()
                    );
            }

            // Even though this function is meant to update the native representation,
            // the native count should have been updated along the way.
            if(pokemon_index < _pksav_party.count)
            {
                if(std::is_same<list_type, struct pksav_gen1_pokemon_party>::value)
                {
                    PKSAV_CALL(
                        pksav_gen1_export_text(
                            _pokemon_list[pokemon_index]->get_original_trainer_name().c_str(),
                            _pksav_party.otnames[pokemon_index],
                            PKSAV_GEN1_POKEMON_OTNAME_LENGTH
                        );
                    )
                    PKSAV_CALL(
                        pksav_gen1_export_text(
                            _pokemon_list[pokemon_index]->get_nickname().c_str(),
                            _pksav_party.nicknames[pokemon_index],
                            PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
                        );
                    )
                }
                else
                {
                    PKSAV_CALL(
                        pksav_gen2_export_text(
                            _pokemon_list[pokemon_index]->get_original_trainer_name().c_str(),
                            _pksav_party.otnames[pokemon_index],
                            PKSAV_GEN2_POKEMON_OTNAME_LENGTH
                        );
                    )
                    PKSAV_CALL(
                        pksav_gen2_export_text(
                            _pokemon_list[pokemon_index]->get_nickname().c_str(),
                            _pksav_party.nicknames[pokemon_index],
                            PKSAV_GEN2_POKEMON_NICKNAME_LENGTH
                        );
                    )
                }
            }
        }
    }
}

#endif /* PKMN_POKEMON_PARTY_GBIMPL_IPP */
