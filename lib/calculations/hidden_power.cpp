/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../database/database_common.hpp"

#include <pkmn/calculations/hidden_power.hpp>

#include <boost/config.hpp>
#include <boost/format.hpp>

#include <cmath>
#include <stdexcept>

namespace pkmn { namespace calculations {

    static BOOST_CONSTEXPR const char* stat_name_query = \
        "SELECT name FROM type_names WHERE local_language_id=9 AND type_id=?";

    // Most significant bit
    #define MSB(var) ((var >> 3) & 1)

    static PKMN_INLINE void _check_IV_bounds(
        int IV,
        const std::string &IV_name,
        bool modern
    ) {
        boost::format IV_error_format("IV_%s valid range: 0-%d");
        int upper_bound = (modern ? 31 : 15);

        if(IV < 0 or IV > upper_bound) {
            throw std::out_of_range(
                      str(IV_error_format % IV_name.c_str() % upper_bound)
                  );
        }
    }

    hidden_power_t gen2_hidden_power(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    ) {
        // Input validation
        _check_IV_bounds(IV_attack,  "attack",  false);
        _check_IV_bounds(IV_defense, "defense", false);
        _check_IV_bounds(IV_speed,   "speed",   false);
        _check_IV_bounds(IV_special, "special", false);

        uint8_t v = MSB(IV_special);
        uint8_t w = MSB(IV_speed);
        uint8_t x = MSB(IV_defense);
        uint8_t y = MSB(IV_attack);
        uint8_t Z = (IV_special % 4);

        hidden_power_t ret;
        ret.type = pkmn_db_query_bind1<std::string, int>(
                       stat_name_query,
                       (((IV_attack % 4) << 2) + (IV_defense % 4))
                   );
        ret.base_power = int(std::floor(float(((5 * (v + (w<<1) + (x<<2) + (y<<3)) + Z) / 2) + 31)));

        return ret;
    }

    // Least significant bit
    #define LSB(var)  (var & 1)
    // Second-least significant bit
    #define LSB2(var) ((var & 2) >> 1)

    hidden_power_t modern_hidden_power(
        int IV_HP,
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_spatk,
        int IV_spdef
    ) {
        // Input validation
        _check_IV_bounds(IV_HP,      "HP",      true);
        _check_IV_bounds(IV_attack,  "attack",  true);
        _check_IV_bounds(IV_defense, "defense", true);
        _check_IV_bounds(IV_speed,   "speed",   true);
        _check_IV_bounds(IV_spatk,   "spatk",   true);
        _check_IV_bounds(IV_spdef,   "spdef",   true);

        uint8_t a = LSB(IV_HP);
        uint8_t b = LSB(IV_attack);
        uint8_t c = LSB(IV_defense);
        uint8_t d = LSB(IV_speed);
        uint8_t e = LSB(IV_spatk);
        uint8_t f = LSB(IV_spdef);

        uint8_t u = LSB2(IV_HP);
        uint8_t v = LSB2(IV_attack);
        uint8_t w = LSB2(IV_defense);
        uint8_t x = LSB2(IV_speed);
        uint8_t y = LSB2(IV_spatk);
        uint8_t z = LSB2(IV_spdef);

        hidden_power_t ret;
        ret.type = pkmn_db_query_bind1<std::string, int>(
                       stat_name_query,
                       int(std::floor(float(((a + (b<<1) + (c<<2) + (d<<3) + (e<<4) + (f<<5)) * 15) / 63)))
                   );
        ret.base_power = int(std::floor(float((((u + (v<<1) + (w<<2) + (x<<3) + (y<<4) + (z<<5)) * 40) / 63) + 30)));

        return ret;
    }

}}
