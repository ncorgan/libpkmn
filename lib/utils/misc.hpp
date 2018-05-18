/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_MISC_COMMON_HPP
#define INCLUDED_PKMN_MISC_COMMON_HPP

#include <pkmn/config.hpp>

#include <boost/config.hpp>

#include <algorithm>
#include <string>
#include <vector>

namespace pkmn {

    template <typename T>
    inline bool does_vector_contain_value(
        const std::vector<T>& vec,
        const T& value
    )
    {
        return (std::find(vec.begin(), vec.end(), value) != vec.end());
    }

    template <typename ptr_type>
    static void inline rcast_equal(
        void* src_ptr,
        void* dst_ptr
    )
    {
        *reinterpret_cast<ptr_type*>(dst_ptr) = *reinterpret_cast<ptr_type*>(src_ptr);
    }

    template <typename map_type, typename key_type = std::string>
    static std::vector<key_type> map_keys_to_vector(
        const map_type& map
    )
    {
        std::vector<key_type> ret;

        for(const auto& map_pair: map)
        {
            ret.emplace_back(map_pair.first);
        }

        return ret;
    }

    // Useful constants and maps

    static const std::vector<std::string> GEN1_STATS =
    {
        "HP", "Attack", "Defense", "Speed", "Special"
    };
    static const std::vector<std::string> GEN2_STATS =
    {
        "HP", "Attack", "Defense", "Speed", "Special",
        "Special Attack", "Special Defense"
    };
    static const std::vector<std::string> MODERN_STATS =
    {
        "HP", "Attack", "Defense", "Speed",
        "Special Attack", "Special Defense"
    };

    BOOST_STATIC_CONSTEXPR int COLOSSEUM_ID = 19;
    BOOST_STATIC_CONSTEXPR int XD_ID        = 20;

    static PKMN_CONSTEXPR_OR_INLINE bool game_is_gamecube(
        int game_id
    )
    {
        return (game_id == COLOSSEUM_ID or game_id == XD_ID);
    }
}

#endif /* INCLUDED_PKMN_MISC_COMMON_HPP */
