/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_MISC_COMMON_HPP
#define INCLUDED_PKMN_MISC_COMMON_HPP

#include <pkmn/config.hpp>
#include <pkmn/enums/stat.hpp>

#include <boost/config.hpp>

#include <algorithm>
#include <string>
#include <type_traits>
#include <vector>

namespace pkmn {

    // This needs to be a macro because array parameters are decayed into
    // pointers.
    #define RAW_ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

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
        const void* src_ptr,
        void* dst_ptr
    )
    {
        *reinterpret_cast<ptr_type*>(dst_ptr) = *reinterpret_cast<const ptr_type*>(src_ptr);
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

    static const std::vector<pkmn::e_stat> GEN1_STATS =
    {
        pkmn::e_stat::HP,
        pkmn::e_stat::ATTACK,
        pkmn::e_stat::DEFENSE,
        pkmn::e_stat::SPEED,
        pkmn::e_stat::SPECIAL
    };
    static const std::vector<pkmn::e_stat> GEN2_STATS =
    {
        pkmn::e_stat::HP,
        pkmn::e_stat::ATTACK,
        pkmn::e_stat::DEFENSE,
        pkmn::e_stat::SPEED,
        pkmn::e_stat::SPECIAL,
        pkmn::e_stat::SPECIAL_ATTACK,
        pkmn::e_stat::SPECIAL_DEFENSE
    };
    static const std::vector<pkmn::e_stat> MODERN_STATS =
    {
        pkmn::e_stat::HP,
        pkmn::e_stat::ATTACK,
        pkmn::e_stat::DEFENSE,
        pkmn::e_stat::SPEED,
        pkmn::e_stat::SPECIAL_ATTACK,
        pkmn::e_stat::SPECIAL_DEFENSE
    };
    static const std::vector<std::string> STAT_NAMES_FROM_ENUM =
    {
        "None",
        "HP",
        "Attack",
        "Defense",
        "Speed",
        "Special",
        "Special Attack",
        "Special Defense"
    };

    BOOST_STATIC_CONSTEXPR int COLOSSEUM_ID = 19;
    BOOST_STATIC_CONSTEXPR int XD_ID        = 20;

    static PKMN_CONSTEXPR_OR_INLINE bool game_is_gamecube(
        int game_id
    )
    {
        return (game_id == COLOSSEUM_ID or game_id == XD_ID);
    }

    template <typename enum_type>
    inline typename std::enable_if<std::is_enum<enum_type>::value, void>::type decrement_enum(
        enum_type& r_enum_value
    )
    {
        r_enum_value = static_cast<enum_type>(static_cast<typename std::underlying_type<enum_type>::type>(r_enum_value) - 1);
    }

    template <typename enum_type>
    inline typename std::enable_if<std::is_enum<enum_type>::value, void>::type increment_enum(
        enum_type& r_enum_value
    )
    {
        r_enum_value = static_cast<enum_type>(static_cast<typename std::underlying_type<enum_type>::type>(r_enum_value) + 1);
    }
}

#endif /* INCLUDED_PKMN_MISC_COMMON_HPP */
