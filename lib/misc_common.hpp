/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_MISC_COMMON_HPP
#define INCLUDED_PKMN_MISC_COMMON_HPP

#include <pkmn/config.hpp>

#include <boost/assign.hpp>
#include <boost/config.hpp>

#include <cmath>
#include <cstring>

namespace pkmn {

    PKMN_INLINE bool floats_close(
        float f1,
        float f2
    )
    {
        return (std::fabs(f1-f2) < 0.00001);
    }

    static const std::vector<std::string> GEN1_STATS = boost::assign::list_of
        ("HP")("Attack")("Defense")("Speed")("Special")
    ;
    static const std::vector<std::string> GEN2_STATS = boost::assign::list_of
        ("HP")("Attack")("Defense")("Speed")("Special")("Special Attack")("Special Defense")
    ;
    static const std::vector<std::string> MODERN_STATS = boost::assign::list_of
        ("HP")("Attack")("Defense")("Speed")("Special Attack")("Special Defense")
    ;

    PKMN_INLINE bool string_is_gen1_stat(
        const std::string& stat
    )
    {
        return (std::find(GEN1_STATS.begin(), GEN1_STATS.end(), stat) != GEN1_STATS.end());
    }

    PKMN_INLINE bool string_is_gen2_stat(
        const std::string& stat
    )
    {
        return (std::find(GEN2_STATS.begin(), GEN2_STATS.end(), stat) != GEN2_STATS.end());
    }

    PKMN_INLINE bool string_is_modern_stat(
        const std::string& stat
    )
    {
        return (std::find(MODERN_STATS.begin(), MODERN_STATS.end(), stat) != MODERN_STATS.end());
    }

    PKMN_CONSTEXPR_OR_INLINE bool EV_in_bounds(
        int EV, bool modern
    )
    {
        return (EV >= 0) and (EV <= (modern ? 255 : 65535));
    }

    PKMN_CONSTEXPR_OR_INLINE bool IV_in_bounds(
        int IV, bool modern
    )
    {
        return (IV >= 0) and (IV <= (modern ? 31 : 15));
    }

    BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;
    BOOST_STATIC_CONSTEXPR int XD = 20;

    BOOST_STATIC_CONSTEXPR bool game_is_gamecube(
        int game_id
    )
    {
        return (game_id == COLOSSEUM or game_id == XD);
    }

    template <typename ptr_type>
    static void inline rcast_equal(
        void* src_ptr,
        void* dst_ptr
    )
    {
        *reinterpret_cast<ptr_type*>(dst_ptr) = *reinterpret_cast<ptr_type*>(src_ptr);
    }

    template <typename map_type, typename key_type>
    std::vector<key_type> map_keys_to_vector(
        const map_type& map
    )
    {
        std::vector<key_type> ret;

        for(const auto& map_iter: map)
        {
            ret.emplace_back(map_iter.first);
        }

        std::sort(ret.begin(), ret.end());
        return ret;
    }
}

#endif /* INCLUDED_PKMN_MISC_COMMON_HPP */
