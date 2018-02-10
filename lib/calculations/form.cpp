/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../exception_internal.hpp"
#include "../misc_common.hpp"

#include <pkmn/config.hpp>
#include <pkmn/calculations/form.hpp>

namespace pkmn { namespace calculations {

    inline char gen2_unown_letter(
        uint8_t IV_attack, uint8_t IV_defense,
        uint8_t IV_speed, uint8_t IV_special
    )
    {
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
    )
    {
        pkmn::enforce_IV_bounds("Attack",  IV_attack,  false);
        pkmn::enforce_IV_bounds("Defense", IV_defense, false);
        pkmn::enforce_IV_bounds("Speed",   IV_speed,   false);
        pkmn::enforce_IV_bounds("Special", IV_special, false);

        std::string ret = "?";
        ret[0] = gen2_unown_letter(
                     uint8_t(IV_attack), uint8_t(IV_defense),
                     uint8_t(IV_speed),  uint8_t(IV_special)
                 );

        return ret;
    }

    inline char gen3_unown_letter(
        const uint8_t* bytes
    )
    {
        return ((((bytes[3] & 0x3) << 6) |
                 ((bytes[2] & 0x3) << 4) |
                 ((bytes[1] & 0x3) << 2) |
                  (bytes[0] & 0x3)) % 28) + 'A';
    }

    std::string gen3_unown_form(
        uint32_t personality
    )
    {
        std::string ret = "?";
        ret[0] = gen3_unown_letter(
                     reinterpret_cast<const uint8_t*>(&personality)
                 );

        if(ret == "[")
        {
            ret = "?";
        }
        else if(ret == "\\")
        {
            ret = "!";
        }

        return ret;
    }

}}
