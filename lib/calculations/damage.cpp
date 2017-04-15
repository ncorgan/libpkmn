/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>
#include <pkmn/calculations/damage.hpp>

#include "../database/database_common.hpp"

namespace pkmn { namespace calculations {

    pkmn::database::sptr _db;

    PKMN_CONSTEXPR_OR_INLINE int _damage(
        float level,
        float power,
        float attack,
        float defense,
        float modifier
    ) {
        return ((((((2.0f * level) / 5.0f) + 2.0f) * power * (attack / defense)) / 50.0f) + 2.0f) * modifier;
    }

    PKMN_CONSTEXPR_OR_INLINE float _gen1_critical_hit_chance(
        float speed,
        bool rate_increased,
        bool high_rate_move
    ) {
        return (speed / 512.0f) /
               (rate_increased ? 4.0f : 1.0f) *
               (high_rate_move ? 8.0f : 1.0f);
    }

    float gen1_critical_hit_chance(
        int speed,
        bool rate_increased,
        bool high_rate_move
    ) {
        // Validate input parameters.
        if(speed < 0) {
            throw std::out_of_range("speed must be > 0.");
        }

        return _gen1_critical_hit_chance(
                   float(speed),
                   rate_increased,
                   high_rate_move
               );
    }

    PKMN_CONSTEXPR_OR_INLINE float _gen1_critical_hit_modifier(
        float attacker_level
    ) {
        return ((2.0f * attacker_level) + 5.0f) / (attacker_level + 5.0f);
    }

    float gen1_critical_hit_modifier(
        int attacker_level
    ) {
        // Validate input parameters (allow 255 for glitch Pokémon).
        if(attacker_level < 1 or attacker_level > 255) {
            throw pkmn::range_error("attacker_level", 1, 100);
        }

        return _gen1_critical_hit_modifier(float(attacker_level));
    }

    int damage(
        int attacker_level,
        int move_base_power,
        int attack_stat,
        int defense_stat,
        float modifier
    ) {
        // Validate input parameters (allow 255 for glitch Pokémon).
        if(attacker_level < 1 or attacker_level > 255) {
            throw pkmn::range_error("attacker_level", 1, 100);
        }
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

    float type_damage_modifier(
        int generation,
        const std::string &attacking_type,
        const std::string &defending_type1,
        const std::string &defending_type2
    ) {
        // Validate generation.
        if(generation < 1 or generation > 6) {
            throw pkmn::range_error("generation", 1, 6);
        }

        // Validate types.
        if(generation < 2) {
            if((attacking_type == "Dark" or attacking_type == "Steel") or
               (defending_type1 == "Dark" or defending_type1 == "Steel") or
               (defending_type2 == "Dark" or defending_type2 == "Steel")
            ) {
                throw std::invalid_argument("The Dark and Steel types are not available before Generation II.");
            }
        }
        if(generation != 3) {
            if((attacking_type == "Shadow") or
               (defending_type1 == "Shadow") or
               (defending_type2 == "Shadow")
            ) {
                throw std::invalid_argument("The Shadow type is only in Generation III.");
            }
        }
        if(generation < 6) {
            if((attacking_type == "Fairy") or
               (defending_type1 == "Fairy") or
               (defending_type2 == "Fairy")
            ) {
                throw std::invalid_argument("The Fairy type is not available before Generation VI.");
            }
        }

        pkmn::database::get_connection(_db);

        float damage_modifier1 = 0.0f;
        float damage_modifier2 = 0.0f;
        const char* query = "";

        if(generation == 1) {
            query = "SELECT damage_factor FROM gen1_type_efficacy WHERE damage_type_id="
                    "(SELECT type_id FROM type_names WHERE name=?) AND target_type_id="
                    "(SELECT type_id FROM type_names WHERE name=?)";
        } else {
            query = "SELECT damage_factor FROM type_efficacy WHERE damage_type_id="
                    "(SELECT type_id FROM type_names WHERE name=?) AND target_type_id="
                    "(SELECT type_id FROM type_names WHERE name=?)";
        }

        // Before Generation VI, Ghost and Dark did 0.5x damage against Steel.
        if(generation < 5 and
           ((attacking_type == "Dark" or attacking_type == "Ghost") and
             defending_type1 == "Steel"))
        {
            damage_modifier1 = 0.5f;
        } else {
            damage_modifier1 = float(pkmn::database::query_db_bind2<int, const std::string&, const std::string&>(
                                   _db, query, attacking_type, defending_type1
                               )) / 100.0f;
        }
        if(generation < 5 and
           ((attacking_type == "Dark" or attacking_type == "Ghost") and
             defending_type2 == "Steel"))
        {
            damage_modifier2 = 0.5f;
        } else if(defending_type2 == "None") {
            damage_modifier2 = 1.0f;
        } else {
            damage_modifier2 = float(pkmn::database::query_db_bind2<int, const std::string&, const std::string&>(
                                   _db, query, attacking_type, defending_type2
                               )) / 100.0f;
        }

        return (damage_modifier1 * damage_modifier2);
    }

}}
