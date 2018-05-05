/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "party_data.hpp"
#include "pksav_call.hpp"

#include <pkmn/calculations/stats.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <pksav/common/stats.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <cstring>

namespace pksav {

    void gen1_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const struct pksav_gen1_pc_pokemon* pc,
        struct pksav_gen1_pokemon_party_data* party_data_out
    ) {
        std::memset(party_data_out, 0, sizeof(*party_data_out));

        std::map<std::string, int> base_stats = entry.get_base_stats();

        size_t exp = 0;
        PKSAV_CALL(
            pksav_import_base256(
                pc->exp,
                3,
                &exp
            );
        )

        party_data_out->level = uint8_t(entry.get_level_at_experience(int(exp)));

        uint8_t IVs[PKSAV_NUM_GB_IVS] = {0};
        PKSAV_CALL(
            pksav_get_gb_IVs(
                &pc->iv_data,
                IVs,
                sizeof(IVs)
            );
        )

        party_data_out->max_hp = pksav_bigendian16(uint16_t(
                                     pkmn::calculations::get_gb_stat(
                                         "HP", party_data_out->level,
                                         base_stats.at("HP"),
                                         pksav_bigendian16(pc->ev_hp),
                                         IVs[PKSAV_GB_IV_HP]
                                     )
                                 ));
        party_data_out->atk = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Attack",
                                      party_data_out->level,
                                      base_stats.at("Attack"),
                                      pksav_bigendian16(pc->ev_atk),
                                      IVs[PKSAV_GB_IV_ATTACK]
                                  )
                              ));
        party_data_out->def = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Defense",
                                      party_data_out->level,
                                      base_stats.at("Defense"),
                                      pksav_bigendian16(pc->ev_def),
                                      IVs[PKSAV_GB_IV_DEFENSE]
                                  )
                              ));
        party_data_out->spd = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Speed",
                                      party_data_out->level,
                                      base_stats.at("Speed"),
                                      pksav_bigendian16(pc->ev_spd),
                                      IVs[PKSAV_GB_IV_SPEED]
                                  )
                              ));
        party_data_out->spd = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Special",
                                      party_data_out->level,
                                      base_stats.at("Special"),
                                      pksav_bigendian16(pc->ev_spcl),
                                      IVs[PKSAV_GB_IV_SPEED]
                                  )
                              ));
    }

    void gen2_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const struct pksav_gen2_pc_pokemon* pc,
        struct pksav_gen2_pokemon_party_data* party_data_out
    ) {
        std::memset(party_data_out, 0, sizeof(*party_data_out));

        std::map<std::string, int> base_stats = entry.get_base_stats();

        uint8_t IVs[PKSAV_NUM_GB_IVS] = {0};
        PKSAV_CALL(
            pksav_get_gb_IVs(
                &pc->iv_data,
                IVs,
                sizeof(IVs)
            );
        )
        party_data_out->current_hp = pksav_bigendian16(uint16_t(
                                         pkmn::calculations::get_gb_stat(
                                             "HP", pc->level,
                                             base_stats.at("HP"),
                                             pksav_bigendian16(pc->ev_hp),
                                             IVs[PKSAV_GB_IV_HP]
                                         )
                                     ));
        party_data_out->max_hp = party_data_out->current_hp;

        party_data_out->atk = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Attack", pc->level,
                                      base_stats.at("Attack"),
                                      pksav_bigendian16(pc->ev_atk),
                                      IVs[PKSAV_GB_IV_ATTACK]
                                  )
                              ));
        party_data_out->def = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Defense", pc->level,
                                      base_stats.at("Defense"),
                                      pksav_bigendian16(pc->ev_def),
                                      IVs[PKSAV_GB_IV_DEFENSE]
                                  )
                              ));
        party_data_out->spd = pksav_bigendian16(uint16_t(
                                  pkmn::calculations::get_gb_stat(
                                      "Speed", pc->level,
                                      base_stats.at("Speed"),
                                      pksav_bigendian16(pc->ev_spd),
                                      IVs[PKSAV_GB_IV_SPEED]
                                  )
                              ));
        party_data_out->spatk = pksav_bigendian16(uint16_t(
                                    pkmn::calculations::get_gb_stat(
                                        "Special Attack", pc->level,
                                        base_stats.at("Special Attack"),
                                        pksav_bigendian16(pc->ev_spcl),
                                        IVs[PKSAV_GB_IV_SPECIAL]
                                    )
                                ));
        party_data_out->spdef = pksav_bigendian16(uint16_t(
                                    pkmn::calculations::get_gb_stat(
                                        "Special Defense", pc->level,
                                        base_stats.at("Special Defense"),
                                        pksav_bigendian16(pc->ev_spcl),
                                        IVs[PKSAV_GB_IV_SPECIAL]
                                    )
                                ));
    }

    // TODO: nature modifiers
    void gba_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const struct pksav_gba_pc_pokemon* pc,
        struct pksav_gba_pokemon_party_data* party_data_out
    ) {
        std::memset(party_data_out, 0, sizeof(*party_data_out));

        std::map<std::string, int> base_stats = entry.get_base_stats();

        const struct pksav_gba_pokemon_growth_block* growth = &pc->blocks.growth;
        const struct pksav_gba_pokemon_effort_block* effort = &pc->blocks.effort;
        const struct pksav_gba_pokemon_misc_block* misc = &pc->blocks.misc;

        party_data_out->level = uint8_t(entry.get_level_at_experience(int(growth->exp)));

        uint8_t IVs[PKSAV_NUM_IVS] = {0};
        PKSAV_CALL(
            pksav_get_IVs(
                &misc->iv_egg_ability,
                IVs,
                sizeof(IVs)
            );
        )
        party_data_out->max_hp = pksav_littleendian16(uint16_t(
                                     pkmn::calculations::get_modern_stat(
                                         "HP",
                                         int(party_data_out->level),
                                         1.0f,
                                         base_stats.at("HP"),
                                         effort->ev_hp,
                                         IVs[PKSAV_IV_HP]
                                     )
                                 ));
        party_data_out->current_hp = party_data_out->max_hp;

        party_data_out->atk = pksav_littleendian16(uint16_t(
                                  pkmn::calculations::get_modern_stat(
                                      "Attack",
                                      int(party_data_out->level),
                                      1.0f,
                                      base_stats.at("Attack"),
                                      effort->ev_atk,
                                      IVs[PKSAV_IV_ATTACK]
                                  )
                              ));
        party_data_out->def = pksav_littleendian16(uint16_t(
                                  pkmn::calculations::get_modern_stat(
                                      "Defense",
                                      int(party_data_out->level),
                                      1.0f,
                                      base_stats.at("Defense"),
                                      effort->ev_def,
                                      IVs[PKSAV_IV_DEFENSE]
                                  )
                              ));
        party_data_out->spd = pksav_littleendian16(uint16_t(
                                  pkmn::calculations::get_modern_stat(
                                      "Speed",
                                      int(party_data_out->level),
                                      1.0f,
                                      base_stats.at("Speed"),
                                      effort->ev_spd,
                                      IVs[PKSAV_IV_SPEED]
                                  )
                              ));
        party_data_out->spatk = pksav_littleendian16(uint16_t(
                                    pkmn::calculations::get_modern_stat(
                                        "Special Attack",
                                        int(party_data_out->level),
                                        1.0f,
                                        base_stats.at("Special Attack"),
                                        effort->ev_spatk,
                                        IVs[PKSAV_IV_SPATK]
                                    )
                                ));
        party_data_out->spdef = pksav_littleendian16(uint16_t(
                                    pkmn::calculations::get_modern_stat(
                                        "Special Defense",
                                        int(party_data_out->level),
                                        1.0f,
                                        base_stats.at("Special Defense"),
                                        effort->ev_spdef,
                                        IVs[PKSAV_IV_SPDEF]
                                    )
                                ));
    }
}
