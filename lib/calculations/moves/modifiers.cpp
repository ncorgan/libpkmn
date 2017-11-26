/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>
#include <pkmn/calculations/moves/modifiers.hpp>

#include "../database/database_common.hpp"

namespace pkmn { namespace calculations {

    static pkmn::database::sptr _db;

    float type_damage_modifier(
        int generation,
        const std::string &attacking_type,
        const std::string &defending_type
    )
    {
        pkmn::enforce_bounds(
            "Generation",
            generation,
            1,
            6
        );

        // Validate types.
        if(generation < 2)
        {
            if((attacking_type == "Dark" or attacking_type == "Steel") or
               (defending_type == "Dark" or defending_type == "Steel")
            )
            {
                throw std::invalid_argument("The Dark and Steel types are not available before Generation II.");
            }
        }
        if(generation != 3)
        {
            if((attacking_type == "Shadow") or
               (defending_type == "Shadow")
            )
            {
                throw std::invalid_argument("The Shadow type is only in Generation III.");
            }
        }
        if(generation < 6)
        {
            if((attacking_type == "Fairy") or
               (defending_type == "Fairy")
            )
            {
                throw std::invalid_argument("The Fairy type is not available before Generation VI.");
            }
        }

        pkmn::database::get_connection(_db);

        float damage_modifier = 0.0f;
        const char* query = "";

        if(generation == 1)
        {
            query = "SELECT damage_factor FROM gen1_type_efficacy WHERE damage_type_id="
                    "(SELECT type_id FROM type_names WHERE name=?) AND target_type_id="
                    "(SELECT type_id FROM type_names WHERE name=?)";
        }
        else
        {
            query = "SELECT damage_factor FROM type_efficacy WHERE damage_type_id="
                    "(SELECT type_id FROM type_names WHERE name=?) AND target_type_id="
                    "(SELECT type_id FROM type_names WHERE name=?)";
        }

        // Hardcode cases specific enough to not be worth putting in the database.

        // Before Generation VI, Ghost and Dark did 0.5x damage against Steel.
        if(generation <= 5 and
           ((attacking_type == "Dark" or attacking_type == "Ghost") and
             defending_type == "Steel"))
        {
            damage_modifier = 0.5f;
        }
        else
        {
            damage_modifier = float(pkmn::database::query_db_bind2<int, const std::string&, const std::string&>(
                                   _db, query, attacking_type, defending_type
                               )) / 100.0f;
        }

        return damage_modifier;
    }

}}
