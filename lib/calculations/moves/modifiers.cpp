/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/moves/modifiers.hpp>

#include <pkmn/enums/enum_to_string.hpp>

#include "exception_internal.hpp"
#include "database/database_common.hpp"

#include <boost/format.hpp>

namespace pkmn { namespace calculations {

    float type_damage_modifier(
        int generation,
        pkmn::e_type attacking_type,
        pkmn::e_type defending_type
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
            if((attacking_type == pkmn::e_type::DARK) ||
               (attacking_type == pkmn::e_type::STEEL) ||
               (defending_type == pkmn::e_type::DARK) ||
               (defending_type == pkmn::e_type::STEEL))
            {
                throw std::invalid_argument("The Dark and Steel types are not available before Generation II.");
            }
        }
        if(generation != 3)
        {
            if((attacking_type == pkmn::e_type::SHADOW) ||
               (defending_type == pkmn::e_type::SHADOW)
            )
            {
                throw std::invalid_argument("The Shadow type is only in Generation III.");
            }
        }
        if(generation != 4)
        {
            if((attacking_type == pkmn::e_type::QUESTION_MARK) ||
               (defending_type == pkmn::e_type::QUESTION_MARK)
            )
            {
                throw std::invalid_argument("The ??? type is only in Generation IV.");
            }
        }
        if(generation < 6)
        {
            if((attacking_type == pkmn::e_type::FAIRY) ||
               (defending_type == pkmn::e_type::FAIRY)
            )
            {
                throw std::invalid_argument("The Fairy type is not available before Generation VI.");
            }
        }

        float damage_modifier = 0.0f;
        const char* query = "";

        if(generation == 1)
        {
            query = "SELECT damage_factor FROM gen1_type_efficacy WHERE "
                    "damage_type_id=? AND target_type_id=?";
        }
        else
        {
            query = "SELECT damage_factor FROM type_efficacy WHERE "
                    "damage_type_id=? AND target_type_id=?";
        }

        // Hardcode cases specific enough to not be worth putting in the database.

        // Before Generation VI, Ghost and Dark did 0.5x damage against Steel.
        if((generation <= 5) &&
           (((attacking_type == pkmn::e_type::DARK) || (attacking_type == pkmn::e_type::GHOST)) &&
             (defending_type == pkmn::e_type::STEEL)))
        {
            damage_modifier = 0.5f;
        }
        else
        {
            std::string error_message = str(boost::format("Invalid type(s): %s, %s")
                                            % pkmn::type_to_string(attacking_type).c_str()
                                            % pkmn::type_to_string(defending_type).c_str());

            damage_modifier = float(pkmn::database::query_db_bind2<int, int, int>(
                                   query,
                                   static_cast<int>(attacking_type),
                                   static_cast<int>(defending_type),
                                   error_message
                               )) / 100.0f;
        }

        return damage_modifier;
    }

}}
