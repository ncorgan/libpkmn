/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../exception_internal.hpp"
#include "../misc_common.hpp"
#include "../database/database_common.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/calculations/moves/hidden_power.hpp>

#include <boost/config.hpp>

#include <cmath>

namespace pkmn { namespace calculations {

    static pkmn::database::sptr _db;

    static BOOST_CONSTEXPR const char* stat_name_query = \
        "SELECT name FROM type_names WHERE local_language_id=9 AND type_id=?";

    // Most significant bit
    #define MSB(var) (((var) >> 3) & 1)

    /*
     * There is no Normal-type Hidden Power, so all type indices
     * are offset from normal.
     */
    PKMN_CONSTEXPR_OR_INLINE int gen2_hidden_power_type(
        int IV_attack, int IV_defense
    ) {
        return (4 * (IV_attack % 4) + (IV_defense % 4)) + 2;
    }

    PKMN_INLINE int gen2_hidden_power_base_power(
        uint8_t v, uint8_t w, uint8_t x,
        uint8_t y, uint8_t Z
    ) {
        return int(std::floor<int>(((5 * (v + (w<<1) + (x<<2) + (y<<3)) + Z) / 2) + 31));
    }

    hidden_power gen2_hidden_power(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        // Input validation
        pkmn::enforce_IV_bounds("Attack",  IV_attack,  false);
        pkmn::enforce_IV_bounds("Defense", IV_defense, false);
        pkmn::enforce_IV_bounds("Speed",   IV_speed,   false);
        pkmn::enforce_IV_bounds("Special", IV_special, false);

        uint8_t v = MSB(IV_special);
        uint8_t w = MSB(IV_speed);
        uint8_t x = MSB(IV_defense);
        uint8_t y = MSB(IV_attack);
        uint8_t Z = (IV_special % 4);

        return hidden_power(
                   pkmn::database::query_db_bind1<std::string, int>(
                       _db, stat_name_query,
                       gen2_hidden_power_type(IV_attack, IV_defense)
                   ),
                   gen2_hidden_power_base_power(v, w, x, y, Z)
               );
    }

    // Least significant bit
    #define LSB(var)  ((var) & 1)
    // Second-least significant bit
    #define LSB2(var) (((var) & 2) >> 1)

    PKMN_INLINE int modern_hidden_power_type(
        uint8_t a, uint8_t b, uint8_t c,
        uint8_t d, uint8_t e, uint8_t f
    ) {
         return int(std::floor<int>(((a + (b<<1) + (c<<2) + (d<<3) + (e<<4) + (f<<5)) * 15) / 63)) + 2;
    }

    PKMN_INLINE int modern_hidden_power_base_power(
        uint8_t u, uint8_t v, uint8_t w,
        uint8_t x, uint8_t y, uint8_t z
    ) {
        return int(std::floor<int>((((u + (v<<1) + (w<<2) + (x<<3) + (y<<4) + (z<<5)) * 40) / 63) + 30));
    }

    hidden_power modern_hidden_power(
        int IV_HP,
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_spatk,
        int IV_spdef
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        // Input validation
        pkmn::enforce_IV_bounds("HP",              IV_HP,      true);
        pkmn::enforce_IV_bounds("Attack",          IV_attack,  true);
        pkmn::enforce_IV_bounds("Defense",         IV_defense, true);
        pkmn::enforce_IV_bounds("Speed",           IV_speed,   true);
        pkmn::enforce_IV_bounds("Special Attack",  IV_spatk,   true);
        pkmn::enforce_IV_bounds("Special Defense", IV_spdef,   true);

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

        return hidden_power(
                   pkmn::database::query_db_bind1<std::string, int>(
                       _db, stat_name_query,
                       modern_hidden_power_type(a, b, c, d, e, f)
                   ),
                   modern_hidden_power_base_power(u, v, w, x, y, z)
               );
    }

}}
