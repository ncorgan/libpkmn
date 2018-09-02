/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>
#include <pkmn/calculations/moves/natural_gift.hpp>

#include "exception_internal.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

namespace pkmn { namespace calculations {

    natural_gift natural_gift_stats(
        pkmn::e_item item,
        int generation
    )
    {
        pkmn::enforce_bounds(
            "Generation",
            generation,
            4,
            6
        );

        natural_gift ret;

        static const std::string gen_4_5_query =
            "SELECT type_id,gen_4_5_power FROM natural_gift_stats WHERE item_id=?";

        static const std::string gen_6_query =
            "SELECT type_id,gen_6_power FROM natural_gift_stats WHERE item_id=?";

        const std::string query = (generation <= 5) ? gen_4_5_query : gen_6_query;

        SQLite::Statement stmt(
            pkmn::database::get_connection(),
            query
        );
        stmt.bind(1, static_cast<int>(item));

        if(stmt.executeStep())
        {
            ret.type = static_cast<pkmn::e_type>(int(stmt.getColumn(0)));
            ret.base_power = stmt.getColumn(1);
        }
        else
        {
            std::ostringstream stream;
            stream << "Invalid SQLite query: \"" << query << "\"" << std::endl
                   << " * Value 1 = " << static_cast<int>(item);

            pkmn::throw_internal_error<std::invalid_argument>(stream.str());
        }

        return ret;
    }

}}
