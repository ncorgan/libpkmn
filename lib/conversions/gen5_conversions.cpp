/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "gen3_conversions.hpp"
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

    void gen5_pc_pokemon_to_gen6(
        const pksav_nds_pc_pokemon_t* from,
        pksav_gen6_pc_pokemon_t* to
    )
    {
        (void)from;
        (void)to;
    }

    void gen5_party_pokemon_to_gen6(
        const pksav_nds_party_pokemon_t* from,
        pksav_gen6_party_pokemon_t* to
    )
    {
        gen5_pc_pokemon_to_gen6(
            &from->pc,
            &to->pc
        );

        // TODO: party data
    }
}}
