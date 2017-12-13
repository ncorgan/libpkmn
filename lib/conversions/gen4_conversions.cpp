/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "gen4_conversions.hpp"
#include "gen5_conversions.hpp"
#include "database/database_common.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/calculations/stats.hpp>

#include <pksav/common/condition.h>
#include <pksav/common/gen3_ribbons.h>
#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/nds/text.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/config.hpp>

namespace pkmn { namespace conversions {

    void gen4_pc_pokemon_to_gen5(
        const pksav_nds_pc_pokemon_t* from,
        pksav_nds_pc_pokemon_t* to
    )
    {
        (void)from;
        (void)to;
    }

    void gen4_party_pokemon_to_gen5(
        const pksav_nds_party_pokemon_t* from,
        pksav_nds_party_pokemon_t* to
    )
    {
        (void)from;
        (void)to;
    }

    void gen4_pc_pokemon_to_gen6(
        const pksav_nds_pc_pokemon_t* from,
        pksav_gen6_pc_pokemon_t* to
    )
    {
        (void)from;
        (void)to;

        pksav_nds_pc_pokemon_t gen5_pokemon;
        gen4_pc_pokemon_to_gen5(
            from,
            &gen5_pokemon
        );
        gen5_pc_pokemon_to_gen6(
            &gen5_pokemon,
            to
        );
    }

    void gen4_party_pokemon_to_gen6(
        const pksav_nds_party_pokemon_t* from,
        pksav_gen6_party_pokemon_t* to
    )
    {
        (void)from;
        (void)to;

        pksav_nds_party_pokemon_t gen5_pokemon;
        gen4_party_pokemon_to_gen5(
            from,
            &gen5_pokemon
        );
        gen5_party_pokemon_to_gen6(
            &gen5_pokemon,
            to
        );
    }
}}
