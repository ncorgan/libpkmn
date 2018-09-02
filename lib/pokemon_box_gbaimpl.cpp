/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_box_gbaimpl.hpp"
#include "pokemon_gbaimpl.hpp"

#include "pksav/enum_maps.hpp"

#include <pkmn/exception.hpp>

#include <pksav/math/endian.h>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>

static const std::string DEFAULT_WALLPAPER = "Forest";

namespace pkmn {

    pokemon_box_gbaimpl::pokemon_box_gbaimpl(
        int game_id,
        const struct pksav_gba_pokemon_box* p_native
    ): pokemon_box_impl(game_id),
       _wallpaper(DEFAULT_WALLPAPER)
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
        }

        _p_native = &_pksav_box;

        _from_native();
    }

    std::string pokemon_box_gbaimpl::get_name()
    {
        boost::lock_guard<pokemon_box_gbaimpl> lock(*this);

        return _box_name;
    }

    void pokemon_box_gbaimpl::set_name(
        const std::string& name
    )
    {
        pkmn::enforce_string_length(
            "Box name",
            name,
            0,
            8
        );

        boost::lock_guard<pokemon_box_gbaimpl> lock(*this);

        _box_name = name;
    }

    int pokemon_box_gbaimpl::get_num_pokemon()
    {
        boost::lock_guard<pokemon_box_gbaimpl> lock(*this);

        int num_pokemon = 0;
        for(int i = 0; i < get_capacity(); ++i)
        {
            if(pksav_littleendian16(_pksav_box.entries[i].blocks.growth.species) > 0)
            {
                ++num_pokemon;
            }
        }

        return num_pokemon;
    }

    int pokemon_box_gbaimpl::get_capacity()
    {
        return int(sizeof(_pksav_box.entries)/sizeof(_pksav_box.entries[0]));
    }

    static std::vector<std::string> get_valid_gba_wallpaper_names(pkmn::e_game game)
    {
        std::vector<std::string> valid_wallpaper_names =
            pkmn::map_keys_to_vector(pksav::get_gba_box_wallpaper_bimap().left);

        std::vector<std::string> game_specific_wallpaper_names;

        if((game == pkmn::e_game::FIRERED) or (game == pkmn::e_game::LEAFGREEN))
        {
            game_specific_wallpaper_names =
                pkmn::map_keys_to_vector(pksav::get_gba_frlg_box_wallpaper_bimap().left);
        }
        else
        {
            game_specific_wallpaper_names =
                pkmn::map_keys_to_vector(pksav::get_gba_rse_box_wallpaper_bimap().left);
        }

        valid_wallpaper_names.insert(
            valid_wallpaper_names.end(),
            game_specific_wallpaper_names.begin(),
            game_specific_wallpaper_names.end()
        );

        return valid_wallpaper_names;
    }

    std::string pokemon_box_gbaimpl::get_wallpaper()
    {
        boost::lock_guard<pokemon_box_gbaimpl> lock(*this);

        BOOST_ASSERT(pkmn::does_vector_contain_value(
            get_valid_gba_wallpaper_names(get_game()),
            _wallpaper
        ));

        return _wallpaper;
    }

    void pokemon_box_gbaimpl::set_wallpaper(
        const std::string& wallpaper
    )
    {
        pkmn::enforce_value_in_vector(
            "Wallpaper",
            wallpaper,
            get_valid_gba_wallpaper_names(get_game())
        );

        boost::lock_guard<pokemon_box_gbaimpl> lock(*this);

        _wallpaper = wallpaper;
    }

    void pokemon_box_gbaimpl::_set_pokemon(
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

        boost::lock_guard<pokemon_box_gbaimpl> lock(*this);

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
        pokemon_gbaimpl* p_new_pokemon = dynamic_cast<pokemon_gbaimpl*>(
                                             actual_new_pokemon.get()
                                         );
        BOOST_ASSERT(p_new_pokemon != nullptr);
        boost::lock_guard<pokemon_gbaimpl> new_pokemon_lock(*p_new_pokemon);

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.
        //
        // Note: as we control the implementation, we know the PC data points
        // to the whole Pokémon data structure.
        rcast_equal<struct pksav_gba_pc_pokemon>(
            actual_new_pokemon->get_native_pc_data(),
            &_pksav_box.entries[index]
        );
        _pokemon_list[index] = std::make_shared<pokemon_gbaimpl>(
                                   &_pksav_box.entries[index],
                                   _game_id
                               );

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

    void pokemon_box_gbaimpl::_from_native()
    {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        for(int pokemon_index = 0; pokemon_index < capacity; ++pokemon_index)
        {
            _pokemon_list[pokemon_index] = std::make_shared<pokemon_gbaimpl>(
                                                &_pksav_box.entries[pokemon_index],
                                                _game_id
                                            );
        }
    }

    void pokemon_box_gbaimpl::_to_native()
    {
        BOOST_ASSERT(static_cast<size_t>(get_capacity()) == _pokemon_list.size());

        for(size_t pokemon_index = 0;
            pokemon_index < _pokemon_list.size();
            ++pokemon_index)
        {
            pkmn::rcast_equal<struct pksav_gba_pc_pokemon>(
                _pokemon_list[pokemon_index]->get_native_pc_data(),
                &_pksav_box.entries[pokemon_index]
            );
        }
    }
}
