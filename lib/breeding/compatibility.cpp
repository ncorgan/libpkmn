/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database/database_common.hpp"
#include "utils/misc.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/breeding/compatibility.hpp>

#include <boost/assert.hpp>
#include <boost/config.hpp>

namespace pkmn { namespace breeding {

    static bool does_string_pair_contain_value(
        const std::pair<std::string, std::string>& string_pair,
        const std::string& value
    )
    {
        return ((string_pair.first == value) ||
                (string_pair.second == value));
    }

    static bool do_string_pairs_have_common_valid_value(
        const std::pair<std::string, std::string>& string_pair1,
        const std::pair<std::string, std::string>& string_pair2
    )
    {
        bool has_common_value = false;

        has_common_value |= (string_pair1.first == string_pair2.first);
        if(!has_common_value && (string_pair2.first != "None"))
        {
            has_common_value |= (string_pair1.first == string_pair2.second);
        }
        if(!has_common_value && (string_pair1.second != "None"))
        {
            has_common_value |= (string_pair1.second == string_pair2.first);
            if(!has_common_value && (string_pair2.second != "None"))
            {
                has_common_value |= (string_pair1.second == string_pair2.second);
            }
        }

        return has_common_value;
    }

    static bool are_species_valid_and_compatible(
        const pkmn::pokemon::sptr& pokemon1,
        const pkmn::pokemon::sptr& pokemon2
    )
    {
        bool are_compatible = true;

        int pokemon1_id = pokemon1->get_database_entry().get_species_id();
        int pokemon2_id = pokemon2->get_database_entry().get_species_id();

        are_compatible = (pokemon1_id >= 1) && (pokemon2_id >= 1);

        if(are_compatible)
        {
            BOOST_STATIC_CONSTEXPR int DITTO_SPECIES_ID = 132;

            are_compatible &= !((pokemon1_id == DITTO_SPECIES_ID) &&
                                (pokemon2_id == DITTO_SPECIES_ID));
        }
        if(are_compatible)
        {
            BOOST_STATIC_CONSTEXPR int NIDOQUEEN_SPECIES_ID = 31;
            BOOST_STATIC_CONSTEXPR int NIDOKING_SPECIES_ID  = 34;

            static const std::vector<int> NONBREEDABLE_SPECIES_IDS =
            {
                NIDOQUEEN_SPECIES_ID, NIDOKING_SPECIES_ID
            };

            are_compatible &= !(does_vector_contain_value(NONBREEDABLE_SPECIES_IDS, pokemon1_id) ||
                                does_vector_contain_value(NONBREEDABLE_SPECIES_IDS, pokemon2_id));
        }

        return are_compatible;
    }

    static bool are_egg_groups_valid_and_compatible(
        const pkmn::pokemon::sptr& pokemon1,
        const pkmn::pokemon::sptr& pokemon2
    )
    {
        bool are_compatible = true;

        const pkmn::database::pokemon_entry& pokemon1_entry = pokemon1->get_database_entry();
        const pkmn::database::pokemon_entry& pokemon2_entry = pokemon2->get_database_entry();

        std::pair<std::string, std::string> pokemon1_egg_groups = pokemon1_entry.get_egg_groups();
        std::pair<std::string, std::string> pokemon2_egg_groups = pokemon2_entry.get_egg_groups();

        bool is_pokemon1_ditto = (pokemon1_egg_groups.first == "Ditto");
        bool is_pokemon2_ditto = (pokemon2_egg_groups.first == "Ditto");

        // Pokémon in the "Undiscovered" group cannot breed.
        if(does_string_pair_contain_value(pokemon1_egg_groups, "Undiscovered") ||
           does_string_pair_contain_value(pokemon2_egg_groups, "Undiscovered"))
        {
            are_compatible = false;
        }
        else if(is_pokemon1_ditto && is_pokemon2_ditto)
        {
            // Two Ditto cannot breed.
            are_compatible = false;
        }
        else if(!is_pokemon1_ditto && !is_pokemon2_ditto)
        {
            are_compatible = do_string_pairs_have_common_valid_value(
                                 pokemon1_egg_groups,
                                 pokemon2_egg_groups
                             );
        }

        return are_compatible;
    }

    bool are_gen2_IVs_compatible(
        const pkmn::pokemon::sptr& pokemon1,
        const pkmn::pokemon::sptr& pokemon2
    )
    {
        const std::map<std::string, int>& pokemon1_IVs = pokemon1->get_IVs();
        const std::map<std::string, int>& pokemon2_IVs = pokemon2->get_IVs();

        return ((pokemon1_IVs.at("Defense") != pokemon2_IVs.at("Defense")) &&
                (pokemon1_IVs.at("Special") != pokemon2_IVs.at("Special")) &&
                (std::abs(pokemon1_IVs.at("Special") - pokemon2_IVs.at("Special"))) != 8);
    }

    bool are_pokemon_compatible(
        const pkmn::pokemon::sptr& pokemon1,
        const pkmn::pokemon::sptr& pokemon2
    )
    {
        bool are_compatible = true;

        const pkmn::database::pokemon_entry& pokemon1_entry = pokemon1->get_database_entry();
        const pkmn::database::pokemon_entry& pokemon2_entry = pokemon2->get_database_entry();

        int generation1 = pkmn::database::game_id_to_generation(pokemon1_entry.get_game_id());
        int generation2 = pkmn::database::game_id_to_generation(pokemon2_entry.get_game_id());

        if((generation1 == 1) || (generation2 == 1))
        {
            throw pkmn::feature_not_in_game_error(
                      "Breeding",
                      "Generation I"
                  );
        }

        are_compatible = (pokemon1_entry.get_game_id() == pokemon2_entry.get_game_id())
                       && !pokemon1->is_egg()
                       && !pokemon2->is_egg()
                       && are_species_valid_and_compatible(pokemon1, pokemon2)
                       && are_egg_groups_valid_and_compatible(pokemon1, pokemon2);
        if(generation1 == 2)
        {
            are_compatible &= are_gen2_IVs_compatible(pokemon1, pokemon2);
        }

        return are_compatible;
    }

}}
