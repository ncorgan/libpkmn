/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"

#include <pkmn/calculations/moves/damage.hpp>

namespace pkmn { namespace calculations {

    static PKMN_CONSTEXPR_OR_INLINE int _damage(
        float level,
        float power,
        float attack,
        float defense,
        float modifier
    ) {
        return int(((((((2.0f * level) / 5.0f) + 2.0f) * power * (attack / defense)) / 50.0f) + 2.0f) * modifier);
    }

    int damage(
        int attacker_level,
        int move_base_power,
        int attack_stat,
        int defense_stat,
        float modifier
    ) {
        pkmn::enforce_bounds(
            "Attacker level",
            attacker_level,
            1,
            255
        );
        if(move_base_power < 0) {
            throw std::out_of_range("move_base_power must be > 0.");
        }
        if(attack_stat < 0) {
            throw std::out_of_range("attack_stat must be > 0.");
        }
        if(defense_stat < 0) {
            throw std::out_of_range("defense_stat must be > 0.");
        }
        if(modifier < 0.0f) {
            throw std::out_of_range("modifier must be > 0.0f.");
        }

        return _damage(
                   float(attacker_level),
                   float(move_base_power),
                   float(attack_stat),
                   float(defense_stat),
                   modifier
               );
    }

}}
