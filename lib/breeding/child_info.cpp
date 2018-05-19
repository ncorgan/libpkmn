/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "utils/misc.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

#include <pkmn/breeding/child_info.hpp>
#include <pkmn/breeding/compatibility.hpp>

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
        const pkmn::pokemon::sptr& pokemon1,
        const pkmn::pokemon::sptr& pokemon2
    )
    {
        if(!are_pokemon_compatible(pokemon1, pokemon2))
        {
            throw std::invalid_argument(
                      "The two given Pokémon are not compatible for breeding."
                  );
        }

        std::vector<std::string> possible_child_species;

        const pkmn::database::pokemon_entry& pokemon1_entry = pokemon1->get_database_entry();
        const pkmn::database::pokemon_entry& pokemon2_entry = pokemon2->get_database_entry();

        bool is_pokemon1_nidoran_f = (pokemon1_entry.get_species_id() == NIDORAN_F_SPECIES_ID);
        bool is_pokemon2_nidoran_f = (pokemon2_entry.get_species_id() == NIDORAN_F_SPECIES_ID);

        bool is_pokemon1_nidoran_m = (pokemon1_entry.get_species_id() == NIDORAN_M_SPECIES_ID);
        bool is_pokemon2_nidoran_m = (pokemon2_entry.get_species_id() == NIDORAN_M_SPECIES_ID);

        bool is_pokemon1_ditto = (pokemon1_entry.get_species_id() == DITTO_SPECIES_ID);
        bool is_pokemon2_ditto = (pokemon2_entry.get_species_id() == DITTO_SPECIES_ID);

        bool is_pokemon1_volbeat = (pokemon1_entry.get_species_id() == VOLBEAT_SPECIES_ID);
        bool is_pokemon2_volbeat = (pokemon2_entry.get_species_id() == VOLBEAT_SPECIES_ID);

        bool is_pokemon1_illumise = (pokemon1_entry.get_species_id() == ILLUMISE_SPECIES_ID);
        bool is_pokemon2_illumise = (pokemon2_entry.get_species_id() == ILLUMISE_SPECIES_ID);

        if(is_pokemon1_nidoran_f || is_pokemon2_nidoran_f)
        {
            // Trust the library to properly return the symbols.
            static const std::vector<std::string> NIDORAN_F_POSSIBLE_SPECIES =
            {
                pkmn::database::species_id_to_name(NIDORAN_F_SPECIES_ID),
                pkmn::database::species_id_to_name(NIDORAN_M_SPECIES_ID),
            };

            possible_child_species = NIDORAN_F_POSSIBLE_SPECIES;
        }
        else if((is_pokemon1_nidoran_m && is_pokemon2_ditto) ||
                (is_pokemon1_ditto & is_pokemon2_nidoran_m))
        {
            possible_child_species.emplace_back(
                pkmn::database::species_id_to_name(NIDORAN_M_SPECIES_ID)
            );

            if(pkmn::database::game_id_to_generation(pokemon1_entry.get_game_id()) >= 5)
            {
                // Output should be sorted by species ID
                possible_child_species.insert(
                    possible_child_species.begin(),
                    pkmn::database::species_id_to_name(NIDORAN_F_SPECIES_ID)
                );
            }
        }
        else if((is_pokemon1_volbeat && is_pokemon2_ditto) ||
                (is_pokemon1_ditto & is_pokemon2_volbeat))
        {
            possible_child_species.emplace_back("Volbeat");
            if(pkmn::database::game_id_to_generation(pokemon1_entry.get_game_id()) >= 5)
            {
                possible_child_species.emplace_back("Illumise");
            }
        }
        else if(is_pokemon1_illumise || is_pokemon2_illumise)
        {
            possible_child_species = {"Volbeat", "Illumise"};
        }
        else if(is_pokemon1_ditto)
        {
            possible_child_species.emplace_back(
                get_earliest_species_in_evolutionary_line(
                    pokemon2_entry.get_species_id()
                )
            );
        }
        else if(is_pokemon2_ditto)
        {
            possible_child_species.emplace_back(
                get_earliest_species_in_evolutionary_line(
                    pokemon1_entry.get_species_id()
                )
            );
        }
        else
        {
            // At this point, we should be able to guarantee that one
            // Pokémon is female and one is male. In this case, the
            // child matches the mother's species.
            if(pokemon1->get_gender() == "Female")
            {
                possible_child_species.emplace_back(
                    get_earliest_species_in_evolutionary_line(
                        pokemon1_entry.get_species_id()
                    )
                );
            }
            else
            {
                possible_child_species.emplace_back(
                    get_earliest_species_in_evolutionary_line(
                        pokemon2_entry.get_species_id()
                    )
                );
            }
        }

        BOOST_ASSERT(!possible_child_species.empty());
        return possible_child_species;
    }

}}
