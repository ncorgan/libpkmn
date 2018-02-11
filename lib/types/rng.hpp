/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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

#include <limits>

namespace pkmn {

template <typename T>
class rng {
    public:
        inline rng() {
            _rng.seed(uint32_t(
                boost::posix_time::microsec_clock::universal_time().time_of_day().total_microseconds() %
                std::numeric_limits<uint32_t>::max()
            ));
        }

        inline T rand(
            T min = std::numeric_limits<T>::min(),
            T max = std::numeric_limits<T>::max()
        ) {
            return boost::random::uniform_int_distribution<T>(min, max)(_rng);
        }

    private:
        boost::random::mt19937 _rng;
};

}

#endif /* INCLUDED_PKMN_RNG_HPP */
