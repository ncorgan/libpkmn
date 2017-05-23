/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>

#include <boost/config.hpp>

#include <stdexcept>

namespace pkmn { namespace calculations {

    PKMN_CONSTEXPR_OR_INLINE char gen2_unown_letter(
        uint8_t IV_attack, uint8_t IV_defense,
        uint8_t IV_speed, uint8_t IV_special
    ) {
        return char((((IV_attack & 0x6) << 5) |
                     ((IV_defense & 0x6) << 3) |
                     ((IV_speed & 0x6) << 1) |
                     ((IV_special & 0x6) >> 1)) / 10) + 'A';
    }

    std::string gen2_unown_form(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    ) {
        if(not pkmn::IV_in_bounds(IV_attack, false)) {
            pkmn::throw_out_of_range("IV_attack", 0, 15);
        }
        if(not pkmn::IV_in_bounds(IV_defense, false)) {
            pkmn::throw_out_of_range("IV_defense", 0, 15);
        }
        if(not pkmn::IV_in_bounds(IV_speed, false)) {
            pkmn::throw_out_of_range("IV_speed", 0, 15);
        }
        if(not pkmn::IV_in_bounds(IV_special, false)) {
            pkmn::throw_out_of_range("IV_special", 0, 15);
        }

        std::string ret = "?";
        ret[0] = gen2_unown_letter(
                     uint8_t(IV_attack), uint8_t(IV_defense),
                     uint8_t(IV_speed),  uint8_t(IV_special)
                 );
        return ret;
    }

    PKMN_CONSTEXPR_OR_INLINE char gen3_unown_letter(
        const uint8_t* bytes
    ) {
        return ((((bytes[3] & 0x3) << 6) |
                 ((bytes[2] & 0x3) << 4) |
                 ((bytes[1] & 0x3) << 2) |
                  (bytes[0] & 0x3)) % 28) + 'A';
    }

    std::string gen3_unown_form(
        uint32_t personality
    ) {
        std::string ret = "?";
        ret[0] = gen3_unown_letter(
                     reinterpret_cast<const uint8_t*>(&personality)
                 );

        if(ret == "[") {
            return "?";
        } else if(ret == "\\") {
            return "!";
        } else {
            return ret;
        }
    }

}}
