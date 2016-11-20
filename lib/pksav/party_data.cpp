/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "party_data.hpp"

#include <pkmn/calculations/stats.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <pksav/common/stats.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <boost/config.hpp>

namespace pksav {

    void gen1_pc_pokemon_to_party_data(
        const pksav_gen1_pc_pokemon_t* pc,
        pksav_gen1_pokemon_party_data_t* party_data_out
    ) {
        pkmn::database::pokemon_entry entry(
            pksav_bigendian16(pc->species), 1
        );
        std::map<std::string, int> base_stats = entry.get_base_stats();

        uint32_t exp = 0;
        pksav_from_base256(
            pc->exp,
            3,
            &exp
        );

        party_data_out->level = uint8_t(entry.get_level_at_experience(int(exp)));

        /*
         * PKSav TODO: get_gb_IV and get_IV, make raw a const pointer
         */

        uint8_t IV_hp;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_HP,
            &IV_hp
        );
        party_data_out->max_hp = pksav_bigendian16(uint16_t(
                                     pkmn::calculations::get_gb_stat(
                                         "HP", party_data_out->level,
                                         base_stats.at("HP"),
                                         pksav_bigendian16(pc->ev_hp),
                                         IV_hp
                                     )
                                 ));

        uint8_t IV_attack;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_ATTACK,
            &IV_attack
        );
        party_data_out->atk = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Attack",
                                      party_data_out->level,
                                      base_stats.at("Attack"),
                                      pksav_bigendian16(pc->ev_atk),
                                      IV_attack
                                  )
                              ));

        uint8_t IV_defense;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_DEFENSE,
            &IV_defense
        );
        party_data_out->def = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Defense",
                                      party_data_out->level,
                                      base_stats.at("Defense"),
                                      pksav_bigendian16(pc->ev_def),
                                      IV_defense
                                  )
                              ));

        uint8_t IV_speed;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_SPEED,
            &IV_speed
        );
        party_data_out->spd = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Speed",
                                      party_data_out->level,
                                      base_stats.at("Speed"),
                                      pksav_bigendian16(pc->ev_spd),
                                      IV_speed
                                  )
                              ));

        uint8_t IV_special;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_SPECIAL,
            &IV_special
        );
        party_data_out->spd = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Special",
                                      party_data_out->level,
                                      base_stats.at("Special"),
                                      pksav_bigendian16(pc->ev_spcl),
                                      IV_special
                                  )
                              ));
    }

    void gen2_pc_pokemon_to_party_data(
        const pksav_gen2_pc_pokemon_t* pc,
        pksav_gen2_pokemon_party_data_t* party_data_out
    ) {
        pkmn::database::pokemon_entry entry(
            pksav_bigendian16(pc->species), 4
        );
        std::map<std::string, int> base_stats = entry.get_base_stats();

        /*
         * PKSav TODO: get_gb_IV and get_IV, make raw a const pointer
         */

        uint8_t IV_hp;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_HP,
            &IV_hp
        );
        party_data_out->current_hp = pksav_bigendian16(uint16_t(
                                         pkmn::calculations::get_gb_stat(
                                             "HP", pc->level,
                                             base_stats.at("HP"),
                                             pksav_bigendian16(pc->ev_hp),
                                             IV_hp
                                         )
                                     ));
        party_data_out->max_hp = party_data_out->current_hp;

        uint8_t IV_attack;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_ATTACK,
            &IV_attack
        );
        party_data_out->atk = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Attack", pc->level,
                                      base_stats.at("Attack"),
                                      pksav_bigendian16(pc->ev_atk),
                                      IV_attack
                                  )
                              ));

        uint8_t IV_defense;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_DEFENSE,
            &IV_defense
        );
        party_data_out->def = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Defense", pc->level,
                                      base_stats.at("Defense"),
                                      pksav_bigendian16(pc->ev_def),
                                      IV_defense
                                  )
                              ));

        uint8_t IV_speed;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_SPEED,
            &IV_speed
        );
        party_data_out->spd = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Speed", pc->level,
                                      base_stats.at("Speed"),
                                      pksav_bigendian16(pc->ev_spd),
                                      IV_speed
                                  )
                              ));

        uint8_t IV_spatk;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_SPATK,
            &IV_spatk
        );
        party_data_out->spatk = pksav_bigendian16(uint16_t(
                                    pkmn::calculations::get_gb_stat(
                                        "Special Attack", pc->level,
                                        base_stats.at("Special Attack"),
                                        pksav_bigendian16(pc->ev_spcl),
                                        IV_spatk
                                    )
                                ));

        uint8_t IV_spdef;
        pksav_get_gb_IV(
            const_cast<uint16_t*>(&pc->iv_data),
            PKSAV_STAT_SPDEF,
            &IV_spdef
        );
        party_data_out->spdef = pksav_bigendian16(uint16_t(
                                    pkmn::calculations::get_gb_stat(
                                        "Special Defense", pc->level,
                                        base_stats.at("Special Defense"),
                                        pksav_bigendian16(pc->ev_spcl),
                                        IV_spdef
                                    )
                                ));
    }

}
