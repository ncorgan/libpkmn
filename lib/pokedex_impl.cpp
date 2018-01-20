/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "misc_common.hpp"
#include "pokedex_impl.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <pksav/common/pokedex.h>

#include <boost/assert.hpp>
#include <boost/format.hpp>
#include <boost/thread/lock_guard.hpp>

#include <cassert>

namespace pkmn
{
    static pkmn::database::sptr _db;

    static const std::vector<size_t> generation_pokedex_sizes =
    {
        0, 151, 251, 386, 493, 649, 721, 807
    };

    pokedex::sptr pokedex::make(
        const std::string& game
    )
    {
        int game_id = pkmn::database::game_name_to_id(game);
        if(game_is_gamecube(game_id))
        {
            throw pkmn::feature_not_in_game_error(
                      "Pokédex",
                      game
                  );
        }

        return pkmn::make_shared<pokedex_impl>(game_id);
    }

    pokedex_impl::pokedex_impl(
        int game_id,
        void* native_has_seen,
        void* native_has_caught
    ): _game_id(game_id),
       _generation(pkmn::database::game_id_to_generation(game_id)),
       _num_pokemon(generation_pokedex_sizes.at(_generation)),
       _dirty_seen(true),
       _dirty_caught(true)
    {
        BOOST_ASSERT(!native_has_seen == !native_has_caught);

        if(native_has_seen)
        {
            _native_has_seen = native_has_seen;
            _native_has_caught = native_has_caught;
            _our_mem = false;
        }
        else
        {
            size_t num_bytes = static_cast<size_t>(std::ceil(float(_num_pokemon) / 8.0f));
            _native_has_seen = reinterpret_cast<void*>(new uint8_t[num_bytes]);
            _native_has_caught = reinterpret_cast<void*>(new uint8_t[num_bytes]);

            std::memset(_native_has_seen, 0, num_bytes);
            std::memset(_native_has_caught, 0, num_bytes);
        }
    }

    pokedex_impl::~pokedex_impl()
    {
        if(_our_mem)
        {
            delete[] reinterpret_cast<uint8_t*>(_native_has_seen);
            delete[] reinterpret_cast<uint8_t*>(_native_has_caught);
        }
    }

    std::string pokedex_impl::get_game()
    {
        return pkmn::database::game_id_to_name(_game_id);
    }

    bool pokedex_impl::has_seen(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);
        bool ret = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                reinterpret_cast<uint8_t*>(_native_has_seen),
                uint16_t(species_id),
                &ret
            );
        )

        return ret;
    }

    void pokedex_impl::set_has_seen(
        const std::string& species,
        bool has_seen_value
    )
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);

        PKSAV_CALL(
            pksav_set_pokedex_bit(
                reinterpret_cast<uint8_t*>(_native_has_seen),
                uint16_t(species_id),
                has_seen_value
            );
        )

        // If a Pokémon has not been seen, then it cannot have been caught.
        if((not has_seen_value) and has_caught(species))
        {
            set_has_caught(species, false);
        }

        // Next time get_all_seen() is called, the vector will update.
        _dirty_seen = true;
    }

    const std::vector<std::string>& pokedex_impl::get_all_seen()
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        if(_dirty_seen)
        {
            _update_member_vector(
                reinterpret_cast<const uint8_t*>(_native_has_seen),
                _all_seen
            );
            _dirty_seen = false;
        }

        return _all_seen;
    }

    int pokedex_impl::get_num_seen()
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        if(_dirty_seen)
        {
            _update_member_vector(
                reinterpret_cast<const uint8_t*>(_native_has_seen),
                _all_seen
            );
            _dirty_seen = false;
        }

        return int(_all_seen.size());
    }

    bool pokedex_impl::has_caught(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);
        bool ret = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                reinterpret_cast<uint8_t*>(_native_has_caught),
                uint16_t(species_id),
                &ret
            );
        )

        return ret;
    }

    void pokedex_impl::set_has_caught(
        const std::string& species,
        bool has_caught_value
    )
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);

        PKSAV_CALL(
            pksav_set_pokedex_bit(
                reinterpret_cast<uint8_t*>(_native_has_caught),
                uint16_t(species_id),
                has_caught_value
            );
        )

        // If a Pokémon has been caught, then it must have been seen.
        if(has_caught_value and (not has_caught(species)))
        {
            set_has_seen(species, true);
        }

        // Next time get_all_caught() is called, the vector will update.
        _dirty_caught = true;
    }

    const std::vector<std::string>& pokedex_impl::get_all_caught()
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        if(_dirty_caught)
        {
            _update_member_vector(
                reinterpret_cast<const uint8_t*>(_native_has_caught),
                _all_caught
            );
            _dirty_caught = false;
        }

        return _all_caught;
    }

    int pokedex_impl::get_num_caught()
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        if(_dirty_caught)
        {
            _update_member_vector(
                reinterpret_cast<const uint8_t*>(_native_has_caught),
                _all_caught
            );
            _dirty_caught = false;
        }

        return int(_all_caught.size());
    }

    void* pokedex_impl::get_native_has_seen()
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        return _native_has_seen;
    }

    void* pokedex_impl::get_native_has_caught()
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        return _native_has_caught;
    }

    void pokedex_impl::_update_member_vector(
        const uint8_t* native_list,
        std::vector<std::string>& member_vector
    )
    {
        pkmn::database::get_connection(_db);

        member_vector.clear();

        std::string query_numbers;
        for(uint16_t i = 1; i <= _num_pokemon; ++i)
        {
            bool is_bit_present = false;
            PKSAV_CALL(
                pksav_get_pokedex_bit(
                    native_list,
                    i,
                    &is_bit_present
                );
            )

            if(is_bit_present)
            {
                if(not query_numbers.empty())
                {
                    query_numbers.append(",");
                }

                query_numbers.append(std::to_string(i));
            }
        }

        static boost::format query_format("SELECT name FROM pokemon_species_names WHERE local_language_id=9 "
                                          "AND pokemon_species_id IN (%s) ORDER BY pokemon_species_id");
        pkmn::database::query_db_list(
            _db,
            str(query_format % query_numbers).c_str(),
            member_vector
        );
    }
}
