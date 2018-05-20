/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "utils/floating_point_comparison.hpp"
#include "utils/misc.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

#include <pkmn/breeding/child_info.hpp>
#include <pkmn/breeding/compatibility.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <boost/assert.hpp>
#include <boost/config.hpp>

namespace pkmn { namespace breeding {

    BOOST_STATIC_CONSTEXPR int NIDORAN_F_SPECIES_ID = 29;
    BOOST_STATIC_CONSTEXPR int NIDORAN_M_SPECIES_ID = 32;
    BOOST_STATIC_CONSTEXPR int DITTO_SPECIES_ID     = 132;
    BOOST_STATIC_CONSTEXPR int VOLBEAT_SPECIES_ID   = 313;
    BOOST_STATIC_CONSTEXPR int ILLUMISE_SPECIES_ID  = 314;

    std::string get_earliest_species_in_evolutionary_line(int species_id)
    {
        static BOOST_CONSTEXPR const char* evolution_query =
            "SELECT evolves_from_species_id FROM pokemon_species WHERE id=?";

        int query_species_id = species_id;
        SQLite::Statement stmt(pkmn::database::get_connection(), evolution_query);
        stmt.bind(1, query_species_id);
        while(stmt.executeStep())
        {
            // The final query will be valid but return 0, which we can't use
            if(int(stmt.getColumn(0)) == 0)
            {
                break;
            }
            query_species_id = stmt.getColumn(0);
            stmt.reset();
            stmt.clearBindings();
            stmt.bind(1, query_species_id);
        }

        return pkmn::database::species_id_to_name(query_species_id);
    }

    std::vector<std::string> get_possible_child_species(
        const std::string& mother_species,
        const std::string& father_species,
        const std::string& game
    )
    {
        if(!are_pokemon_species_compatible(mother_species, father_species))
        {
            throw std::invalid_argument(
                      "The two given Pokémon are not compatible for breeding."
                  );
        }

        pkmn::database::pokemon_entry mother_entry(mother_species, game, "");
        if(fp_compare_equal(mother_entry.get_chance_male(), 1.0f))
        {
            std::string error_message(mother_species);
            error_message += " cannot be a mother.";
            throw std::invalid_argument(error_message);
        }

        pkmn::database::pokemon_entry father_entry(father_species, game, "");
        if(fp_compare_equal(father_entry.get_chance_female(), 1.0f))
        {
            std::string error_message(father_species);
            error_message += " cannot be a father.";
            throw std::invalid_argument(error_message);
        }

        std::vector<std::string> possible_child_species;

        bool is_mother_nidoran_f = (mother_entry.get_species_id() == NIDORAN_F_SPECIES_ID);
        bool is_mother_ditto     = (mother_entry.get_species_id() == DITTO_SPECIES_ID);
        bool is_mother_illumise  = (mother_entry.get_species_id() == ILLUMISE_SPECIES_ID);

        bool is_father_nidoran_m = (father_entry.get_species_id() == NIDORAN_M_SPECIES_ID);
        bool is_father_volbeat   = (father_entry.get_species_id() == VOLBEAT_SPECIES_ID);

        if(is_mother_nidoran_f)
        {
            // Trust the library to properly return the symbols.
            static const std::vector<std::string> NIDORAN_F_POSSIBLE_SPECIES =
            {
                pkmn::database::species_id_to_name(NIDORAN_F_SPECIES_ID),
                pkmn::database::species_id_to_name(NIDORAN_M_SPECIES_ID),
            };

            possible_child_species = NIDORAN_F_POSSIBLE_SPECIES;
        }
        else if(is_mother_ditto && is_father_nidoran_m)
        {
            possible_child_species.emplace_back(
                pkmn::database::species_id_to_name(NIDORAN_M_SPECIES_ID)
            );

            if(pkmn::database::game_name_to_generation(game) >= 5)
            {
                // Output should be sorted by species ID
                possible_child_species.insert(
                    possible_child_species.begin(),
                    pkmn::database::species_id_to_name(NIDORAN_F_SPECIES_ID)
                );
            }
        }
        else if(is_mother_ditto && is_father_volbeat)
        {
            possible_child_species.emplace_back("Volbeat");
            if(pkmn::database::game_name_to_generation(game) >= 5)
            {
                possible_child_species.emplace_back("Illumise");
            }
        }
        else if(is_mother_illumise)
        {
            possible_child_species = {"Volbeat", "Illumise"};
        }
        else if(is_mother_ditto)
        {
            possible_child_species.emplace_back(
                get_earliest_species_in_evolutionary_line(
                    father_entry.get_species_id()
                )
            );
        }
        else
        {
            possible_child_species.emplace_back(
                get_earliest_species_in_evolutionary_line(
                    mother_entry.get_species_id()
                )
            );
        }

        BOOST_ASSERT(!possible_child_species.empty());
        return possible_child_species;
    }

}}
