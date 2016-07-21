/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/form.hpp>

namespace pkmn { namespace calculations {

    std::string gen2_unown_form(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    ) {
        uint8_t alphabet_num = ((uint8_t(IV_attack) << 5)
                             | (uint8_t(IV_defense) << 3)
                             | (uint8_t(IV_speed) << 1)
                             | (uint8_t(IV_special) >> 1)) / 10;

        std::string ret = "?";
        ret[0] = char(alphabet_num + 'A');
        return ret;
    }

    std::string gen3_unown_form(
        uint32_t personality
    ) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&personality);

        uint8_t combined = (((bytes[3] & 0x3) << 6)
                         |  ((bytes[2] & 0x3) << 4)
                         |  ((bytes[1] & 0x3) << 2)
                         |   (bytes[0] & 0x3));

        uint8_t alphabet_num = (combined % 28);

        std::string ret = "?";
        ret[0] = char(alphabet_num + 'A');
        return ret;
    }

}}
