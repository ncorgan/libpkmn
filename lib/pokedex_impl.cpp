/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils/misc.hpp"
#include "pokedex_impl.hpp"
#include "pokedex_gbimpl.hpp"
#include "pokedex_gbaimpl.hpp"

#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"

#include "pksav/pksav_call.hpp"

#include "types/mutex_helpers.hpp"

#include <pkmn/exception.hpp>

#include <pksav/common/pokedex.h>

#include <boost/assert.hpp>

namespace pkmn
{
    static const std::vector<size_t> GENERATION_POKEDEX_SIZES =
    {
        0, 151, 251, 386, 493, 649, 721, 807
    };

    pokedex::sptr pokedex::make(pkmn::e_game game)
    {
        int game_id = pkmn::database::game_enum_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);

        pokedex::sptr ret;

        switch(generation)
        {
            case 1:
                ret = std::make_shared<pokedex_gen1impl>(game_id);
                break;

            case 2:
                ret = std::make_shared<pokedex_gen2impl>(game_id);
                break;

            case 3:
                if(game_is_gamecube(game_id))
                {
                    throw pkmn::feature_not_in_game_error(
                              "Pokédex",
                              game
                          );
                }
                else
                {
                    ret = std::make_shared<pokedex_gbaimpl>(game_id);
                }
                break;

            case 4:
            case 5:
            case 6:
            case 7:
                throw pkmn::unimplemented_error();

            default:
                throw std::invalid_argument("Invalid game.");
        }

        return ret;
    }

    pokedex_impl::pokedex_impl(
        int game_id
    ): _game_id(game_id),
       _generation(pkmn::database::game_id_to_generation(game_id)),
       _num_pokemon(GENERATION_POKEDEX_SIZES.at(_generation)),
       _all_seen(),
       _dirty_seen(true),
       _all_caught(),
       _dirty_caught(true),
       _p_native(nullptr)
    {}

    pkmn::e_game pokedex_impl::get_game()
    {
        return pkmn::database::game_id_to_enum(_game_id);
    }

    void pokedex_impl::set_has_seen(
        pkmn::e_species species,
        bool has_seen_value
    )
    {
        pkmn::lock_guard<pokedex_impl> lock(*this);

        _set_has_seen(
            static_cast<int>(species),
            has_seen_value
        );

        // If a Pokémon has not been seen, then it cannot have been caught.
        if(!has_seen_value && has_caught(species))
        {
            set_has_caught(species, false);
        }

        // Next time get_all_seen() is called, the vector will update.
        _dirty_seen = true;
    }

    const std::vector<pkmn::e_species>& pokedex_impl::get_all_seen()
    {
        pkmn::lock_guard<pokedex_impl> lock(*this);

        if(_dirty_seen)
        {
            _update_all_seen();
            _dirty_seen = false;
        }

        return _all_seen;
    }

    int pokedex_impl::get_num_seen()
    {
        pkmn::lock_guard<pokedex_impl> lock(*this);

        if(_dirty_seen)
        {
            _update_all_seen();
            _dirty_seen = false;
        }

        return int(_all_seen.size());
    }

    void pokedex_impl::set_has_caught(
        pkmn::e_species species,
        bool has_caught_value
    )
    {
        pkmn::lock_guard<pokedex_impl> lock(*this);

        _set_has_caught(
            static_cast<int>(species),
            has_caught_value
        );

        // If a Pokémon has been caught, then it must have been seen.
        if(has_caught_value && !has_seen(species))
        {
            set_has_seen(species, true);
        }

        // Next time get_all_caught() is called, the vector will update.
        _dirty_caught = true;
    }

    const std::vector<pkmn::e_species>& pokedex_impl::get_all_caught()
    {
        pkmn::lock_guard<pokedex_impl> lock(*this);

        if(_dirty_caught)
        {
            _update_all_caught();
            _dirty_caught = false;
        }

        return _all_caught;
    }

    int pokedex_impl::get_num_caught()
    {
        pkmn::lock_guard<pokedex_impl> lock(*this);

        if(_dirty_caught)
        {
            _update_all_caught();
            _dirty_caught = false;
        }

        return int(_all_caught.size());
    }

    void* pokedex_impl::get_native()
    {
        pkmn::lock_guard<pokedex_impl> lock(*this);

        return _p_native;
    }

    void pokedex_impl::_update_member_vector_with_pksav(
        const uint8_t* native_list,
        std::vector<pkmn::e_species>& member_vector
    )
    {
        BOOST_ASSERT(native_list != nullptr);

        member_vector.clear();

        for(uint16_t pokedex_num = 1; pokedex_num <= _num_pokemon; ++pokedex_num)
        {
            bool is_bit_present = false;
            PKSAV_CALL(
                pksav_get_pokedex_bit(
                    native_list,
                    pokedex_num,
                    &is_bit_present
                );
            )

            if(is_bit_present)
            {
                member_vector.emplace_back(static_cast<pkmn::e_species>(pokedex_num));
            }
        }
    }
}
