/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "party_data.hpp"
#include "../database/database_common.hpp"

#include <pkmn/calculations/stats.hpp>

#include <pksav/common/stats.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <boost/config.hpp>

namespace pksav {

    static pkmn::database::sptr _db;

    static BOOST_CONSTEXPR const char* exp_to_level_query = \
        "SELECT experience.level FROM experience "
        "INNER JOIN pokemon_species "
        "ON experience.growth_rate_id=pokemon_species.growth_rate_id "
        "AND experience.growth_rate_id=pokemon_species.growth_rate_id "
        "WHERE (experience.experience<=? "
        "AND pokemon_species.id=(SELECT pokemon_species_id FROM pokemon "
        "WHERE id=(SELECT pokemon_id FROM pokemon_game_indices WHERE "
        "game_index=? AND version_id=?))) ORDER BY experience.level DESC";

    static void get_base_stats(
        bool gen1,
        int* base_stats_out
    ) {
        static BOOST_CONSTEXPR const char* gen1_query = \
            "SELECT base_stat FROM pokemon_stats WHERE stat_id "
            "IN (1,2,3,6,9) AND pokemon_id=(SELECT pokemon_id FROM "
            "pokemon_game_indices WHERE game_index=?) ORDER BY "
            "stat_id";

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT base_stat FROM pokemon_stats WHERE stat_id "
            "IN (1,2,3,6,4,5) AND pokemon_id=(SELECT pokemon_id FROM "
            "pokemon_game_indices WHERE game_index=?) ORDER BY "
            "stat_id";

        SQLite::Statement stmt(
            (*_db),
            (gen1 ? gen1_query : main_query)
        );

        for(int i = 0; i < (gen1 ? 6 : 5); ++i) {
            stmt.executeStep();
            base_stats_out[i] = stmt.getColumn(0);
        }
    }

    void gen1_pc_pokemon_to_party_data(
        const pksav_gen1_pc_pokemon_t* pc,
        pksav_gen1_pokemon_party_data_t* party_data_out
    ) {
        pkmn::database::get_connection(_db);

        size_t exp = pksav_from_base256(
                         pc->exp, 3
                     );

        party_data_out->level = uint8_t(pkmn::database::query_db_bind3<int, int, int, int>(
                                            _db, exp_to_level_query,
                                            int(exp), pc->species, 1
                                        ));

        int base_stats[5];
        get_base_stats(true, base_stats);

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
                                         base_stats[0],
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
                                      base_stats[1],
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
                                      base_stats[2],
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
                                      base_stats[3],
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
                                      base_stats[4],
                                      pksav_bigendian16(pc->ev_spcl),
                                      IV_special
                                  )
                              ));
    }

    void gen2_pc_pokemon_to_party_data(
        const pksav_gen2_pc_pokemon_t* pc,
        pksav_gen2_pokemon_party_data_t* party_data_out
    ) {
        pkmn::database::get_connection(_db);

        int base_stats[6];
        get_base_stats(false, base_stats);

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
                                             base_stats[0],
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
                                      base_stats[1],
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
                                      base_stats[2],
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
                                      base_stats[3],
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
                                        base_stats[4],
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
                                        base_stats[5],
                                        pksav_bigendian16(pc->ev_spcl),
                                        IV_spdef
                                    )
                                ));
    }

}
