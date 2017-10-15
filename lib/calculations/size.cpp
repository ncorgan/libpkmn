/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"

#include <pkmn/calculations/size.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/exception.hpp>

#include <boost/assign.hpp>

#include <cmath>
#include <unordered_map>

struct size_xyz_t {
    size_xyz_t(uint16_t _x, uint16_t _y, uint16_t _z):
        x(_x), y(_y), z(_z) {}

    uint16_t x;
    uint16_t y;
    uint16_t z;
};

static const std::unordered_map<uint16_t, size_xyz_t> XYZ = boost::assign::map_list_of<uint16_t, size_xyz_t>
    (9,     size_xyz_t(290,1,0))
    (109,   size_xyz_t(300,1,10))
    (309,   size_xyz_t(400,2,110))
    (709,   size_xyz_t(500,4,310))
    (2709,  size_xyz_t(600,20,710))
    (7709,  size_xyz_t(700,50,2710))
    (17709, size_xyz_t(800,100,7710))
    (32709, size_xyz_t(900,150,17710))
    (47709, size_xyz_t(1000,100,47710))
    (57709, size_xyz_t(1100,100,47710))
    (62709, size_xyz_t(1200,50,57710))
    (64709, size_xyz_t(1300,20,62710))
    (65209, size_xyz_t(1400,5,64710))
    (65409, size_xyz_t(1500,2,65210))
    (65535, size_xyz_t(1700,1,65510))
;

typedef std::pair<uint16_t, size_xyz_t> xyz_pair_t;

uint16_t get_s(
    uint16_t s_from_calculation
) {
    uint16_t ret = 0;

    // The loop won't catch this.
    if(s_from_calculation == 0) {
        ret = 9;
    } else {
        uint16_t last_value = 0;
        for(const xyz_pair_t& xyz_pair: XYZ) {
            if(s_from_calculation > last_value and s_from_calculation <= xyz_pair.first) {
                ret = xyz_pair.first;
            } else {
                last_value = xyz_pair.first;
            }
        }
    }

    return ret;
}

PKMN_INLINE static float round_float(float value) {
    return float(std::floor(value * 5 + 0.5) / 5);
}

namespace pkmn { namespace calculations {

    float pokemon_size(
        const std::string &species,
        uint32_t personality,
        int IV_HP,
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_spatk,
        int IV_spdef
    ) {
        // Input validation
        if(not pkmn::IV_in_bounds(IV_HP, true)) {
            pkmn::throw_out_of_range("IV_HP", 0, 31);
        }
        if(not pkmn::IV_in_bounds(IV_attack, true)) {
            pkmn::throw_out_of_range("IV_attack", 0, 31);
        }
        if(not pkmn::IV_in_bounds(IV_defense, true)) {
            pkmn::throw_out_of_range("IV_defense", 0, 31);
        }
        if(not pkmn::IV_in_bounds(IV_speed, true)) {
            pkmn::throw_out_of_range("IV_speed", 0, 31);
        }
        if(not pkmn::IV_in_bounds(IV_spatk, true)) {
            pkmn::throw_out_of_range("IV_spatk", 0, 31);
        }
        if(not pkmn::IV_in_bounds(IV_spdef, true)) {
            pkmn::throw_out_of_range("IV_spdef", 0, 31);
        }

        uint16_t h = uint16_t(pkmn::database::pokemon_entry(species, "HeartGold", "").get_height() * 10);

        uint16_t p1 = uint16_t(personality % 256);
        uint16_t p2 = uint16_t((personality / 256) % 256);
        uint16_t HP = uint16_t(IV_HP) % 16;
        uint16_t attack = uint16_t(IV_attack) % 16;
        uint16_t defense = uint16_t(IV_defense) % 16;
        uint16_t speed = uint16_t(IV_speed) % 16;
        uint16_t spatk = uint16_t(IV_spatk) % 16;
        uint16_t spdef = uint16_t(IV_spdef) % 16;

        uint16_t s = get_s(
                         ((((attack xor defense) * HP) xor p1) * 256) +
                         (((spatk xor spdef) * speed) xor p2)
                     );
        const size_xyz_t& xyz = XYZ.at(s);

        uint16_t size_in_mm = ((((s - xyz.z) / xyz.y) + xyz.x) * h) / 10;
        return round_float(size_in_mm / 1000.0f);
    }

}}
