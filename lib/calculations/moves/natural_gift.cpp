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
        const std::string& item_name,
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

        static BOOST_CONSTEXPR const char* gen_4_5_query =
            "SELECT type_id,gen_4_5_power FROM natural_gift_stats WHERE item_id="
            "(SELECT item_id FROM item_names WHERE name=?)";

        static BOOST_CONSTEXPR const char* gen_6_query =
            "SELECT type_id,gen_6_power FROM natural_gift_stats WHERE item_id="
            "(SELECT item_id FROM item_names WHERE name=?)";

        const char* query = (generation <= 5) ? gen_4_5_query : gen_6_query;

        SQLite::Statement stmt(
            pkmn::database::get_connection(),
            query
        );
        stmt.bind(1, item_name);

        if(stmt.executeStep())
        {
            ret.type = pkmn::database::type_id_to_name(stmt.getColumn(0));
            ret.base_power = stmt.getColumn(1);
        }
        else
        {
            std::ostringstream stream;
            stream << "Invalid SQLite query: \"" << query << "\"" << std::endl
                   << " * Value 1 = " << item_name;
            throw std::invalid_argument(stream.str());
        }

        return ret;
    }

}}
