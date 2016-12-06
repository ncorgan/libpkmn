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

#include <cstring>

namespace pksav {

    void gen1_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const pksav_gen1_pc_pokemon_t* pc,
        pksav_gen1_pokemon_party_data_t* party_data_out
    ) {
        std::memset(party_data_out, 0, sizeof(*party_data_out));

        std::map<std::string, int> base_stats = entry.get_base_stats();

        uint32_t exp = 0;
        pksav_from_base256(
            pc->exp,
            3,
            &exp
        );

        party_data_out->level = uint8_t(entry.get_level_at_experience(int(exp)));

        uint8_t IV_hp = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

        uint8_t IV_attack = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

        uint8_t IV_defense = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

        uint8_t IV_speed = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

        uint8_t IV_special = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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
        const pkmn::database::pokemon_entry &entry,
        const pksav_gen2_pc_pokemon_t* pc,
        pksav_gen2_pokemon_party_data_t* party_data_out
    ) {
        std::memset(party_data_out, 0, sizeof(*party_data_out));

        std::map<std::string, int> base_stats = entry.get_base_stats();

        uint8_t IV_hp = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

        uint8_t IV_attack = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

        uint8_t IV_defense = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

        uint8_t IV_speed = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

        uint8_t IV_spatk = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

        uint8_t IV_spdef = 0;
        pksav_get_gb_IV(
            &pc->iv_data,
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

    // TODO: nature modifiers
    void gba_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const pksav_gba_pc_pokemon_t* pc,
        pksav_gba_pokemon_party_data_t* party_data_out
    ) {
        std::memset(party_data_out, 0, sizeof(*party_data_out));

        std::map<std::string, int> base_stats = entry.get_base_stats();

        const pksav_gba_pokemon_growth_t* growth = &pc->blocks.growth;
        const pksav_gba_pokemon_effort_t* effort = &pc->blocks.effort;
        const pksav_gba_pokemon_misc_t* misc = &pc->blocks.misc;

        party_data_out->level = uint8_t(entry.get_level_at_experience(int(growth->exp)));

        uint8_t IV_hp = 0;
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_HP,
            &IV_hp
        );
        party_data_out->max_hp = pksav_littleendian16(uint16_t(
                                     pkmn::calculations::get_modern_stat(
                                         "HP",
                                         int(party_data_out->level),
                                         1.0f,
                                         base_stats.at("HP"),
                                         effort->ev_hp,
                                         IV_hp
                                     )
                                 ));
        party_data_out->current_hp = party_data_out->max_hp;

        uint8_t IV_attack = 0;
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_ATTACK,
            &IV_attack
        );
        party_data_out->atk = pksav_littleendian16(uint16_t(
                                  pkmn::calculations::get_modern_stat(
                                      "Attack",
                                      int(party_data_out->level),
                                      1.0f,
                                      base_stats.at("Attack"),
                                      effort->ev_atk,
                                      IV_attack
                                  )
                              ));

        uint8_t IV_defense = 0;
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_DEFENSE,
            &IV_defense
        );
        party_data_out->def = pksav_littleendian16(uint16_t(
                                  pkmn::calculations::get_modern_stat(
                                      "Defense",
                                      int(party_data_out->level),
                                      1.0f,
                                      base_stats.at("Defense"),
                                      effort->ev_def,
                                      IV_defense
                                  )
                              ));

        uint8_t IV_speed = 0;
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPEED,
            &IV_speed
        );
        party_data_out->spd = pksav_littleendian16(uint16_t(
                                  pkmn::calculations::get_modern_stat(
                                      "Speed",
                                      int(party_data_out->level),
                                      1.0f,
                                      base_stats.at("Speed"),
                                      effort->ev_spd,
                                      IV_speed
                                  )
                              ));

        uint8_t IV_spatk = 0;
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPATK,
            &IV_spatk
        );
        party_data_out->spatk = pksav_littleendian16(uint16_t(
                                    pkmn::calculations::get_modern_stat(
                                        "Special Attack",
                                        int(party_data_out->level),
                                        1.0f,
                                        base_stats.at("Special Attack"),
                                        effort->ev_spatk,
                                        IV_spatk
                                    )
                                ));

        uint8_t IV_spdef = 0;
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPDEF,
            &IV_spdef
        );
        party_data_out->spdef = pksav_littleendian16(uint16_t(
                                    pkmn::calculations::get_modern_stat(
                                        "Special Defense",
                                        int(party_data_out->level),
                                        1.0f,
                                        base_stats.at("Special Defense"),
                                        effort->ev_spdef,
                                        IV_spdef
                                    )
                                ));
    }

    // TODO: nature modifiers
    void nds_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const pksav_nds_pc_pokemon_t* pc,
        pksav_nds_pokemon_party_data_t* party_data_out
    ) {
        std::memset(party_data_out, 0, sizeof(*party_data_out));

        std::map<std::string, int> base_stats = entry.get_base_stats();

        const pksav_nds_pokemon_blockA_t* blockA = &pc->blocks.blockA;
        const pksav_nds_pokemon_blockB_t* blockB = &pc->blocks.blockB;

        party_data_out->level = uint8_t(entry.get_level_at_experience(int(blockA->exp)));

        uint8_t IV_hp = 0;
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_HP,
            &IV_hp
        );
        party_data_out->max_hp = pksav_littleendian16(uint16_t(
                                     pkmn::calculations::get_modern_stat(
                                         "HP",
                                         int(party_data_out->level),
                                         1.0f,
                                         base_stats.at("HP"),
                                         blockA->ev_hp,
                                         IV_hp
                                     )
                                 ));
        party_data_out->current_hp = party_data_out->max_hp;

        uint8_t IV_attack = 0;
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_ATTACK,
            &IV_attack
        );
        party_data_out->atk = pksav_littleendian16(uint16_t(
                                  pkmn::calculations::get_modern_stat(
                                      "Attack",
                                      int(party_data_out->level),
                                      1.0f,
                                      base_stats.at("Attack"),
                                      blockA->ev_atk,
                                      IV_attack
                                  )
                              ));

        uint8_t IV_defense = 0;
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_DEFENSE,
            &IV_defense
        );
        party_data_out->def = pksav_littleendian16(uint16_t(
                                  pkmn::calculations::get_modern_stat(
                                      "Defense",
                                      int(party_data_out->level),
                                      1.0f,
                                      base_stats.at("Defense"),
                                      blockA->ev_def,
                                      IV_defense
                                  )
                              ));

        uint8_t IV_speed = 0;
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_SPEED,
            &IV_speed
        );
        party_data_out->spd = pksav_littleendian16(uint16_t(
                                  pkmn::calculations::get_modern_stat(
                                      "Speed",
                                      int(party_data_out->level),
                                      1.0f,
                                      base_stats.at("Speed"),
                                      blockA->ev_spd,
                                      IV_speed
                                  )
                              ));

        uint8_t IV_spatk = 0;
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_SPATK,
            &IV_spatk
        );
        party_data_out->spatk = pksav_littleendian16(uint16_t(
                                    pkmn::calculations::get_modern_stat(
                                        "Special Attack",
                                        int(party_data_out->level),
                                        1.0f,
                                        base_stats.at("Special Attack"),
                                        blockA->ev_spatk,
                                        IV_spatk
                                    )
                                ));

        uint8_t IV_spdef = 0;
        pksav_get_IV(
            &blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_SPDEF,
            &IV_spdef
        );
        party_data_out->spdef = pksav_littleendian16(uint16_t(
                                    pkmn::calculations::get_modern_stat(
                                        "Special Defense",
                                        int(party_data_out->level),
                                        1.0f,
                                        base_stats.at("Special Defense"),
                                        blockA->ev_spdef,
                                        IV_spdef
                                    )
                                ));
    }
}
