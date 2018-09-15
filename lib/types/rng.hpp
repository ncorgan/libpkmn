/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_RNG_HPP
#define INCLUDED_PKMN_RNG_HPP

#include <pkmn/config.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

#include <limits>

#include <type_traits>

namespace pkmn {

// For some reason, the enable_fp addition is necessary.
// See: https://stackoverflow.com/a/25343967

template <typename T>
class rng
{
    public:
        inline rng()
        {
            _rng.seed(uint32_t(
                boost::posix_time::microsec_clock::universal_time().time_of_day().total_microseconds() %
                std::numeric_limits<uint32_t>::max()
            ));
        }

        template <bool enable_fp = true>
        typename std::enable_if<std::is_floating_point<T>::value && enable_fp, T>::type rand(
            T min = std::numeric_limits<T>::min(),
            T max = std::numeric_limits<T>::max()
        )
        {
            return boost::random::uniform_real_distribution<T>(min, max)(_rng);
        }

        template <bool enable_fp = true>
        typename std::enable_if<!std::is_floating_point<T>::value && enable_fp, T>::type rand(
            T min = std::numeric_limits<T>::min(),
            T max = std::numeric_limits<T>::max()
        )
        {
            return boost::random::uniform_int_distribution<T>(min, max)(_rng);
        }

    private:
        boost::random::mt19937 _rng;
};

}

#endif /* INCLUDED_PKMN_RNG_HPP */
