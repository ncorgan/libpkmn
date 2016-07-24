/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"
#include "../database/database_common.hpp"

#include <pkmn/calculations/gender.hpp>

#include <boost/assign.hpp>
#include <boost/config.hpp>

#include <stdexcept>

namespace pkmn { namespace calculations {

    static pkmn::database::sptr _db;

    // No need to instantiate these strings at runtime if we can avoid it
    static BOOST_CONSTEXPR const char* GENDERLESS   = "Genderless";
    static BOOST_CONSTEXPR const char* MALE         = "Male";
    static BOOST_CONSTEXPR const char* FEMALE       = "Female";
    static BOOST_CONSTEXPR const char* gender_rate_query = \
        "SELECT gender_rate FROM pokemon_species WHERE id="
        "(SELECT pokemon_species_id FROM pokemon_species_names "
        "WHERE name=?)";

    /*
     * The database stores gender rates oddly, so this table
     * converts them to actual probabilities.
     */
    static const std::map<int, float> _veekun_gender_rates = boost::assign::map_list_of
        (-1, 0.0f)
        (0,  1.0f)
        (1,  0.875f)
        (2,  0.75f)
        (4,  0.5f)
        (6,  0.25f)
        (8,  0.0f)
    ;

    std::string gen2_pokemon_gender(
        const std::string &species,
        int IV_attack
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        // Input validation
        if(IV_attack < 0 or IV_attack > 15) {
            throw std::out_of_range("IV_attack: valid range 0-15");
        }

        int gender_rate_from_db = pkmn::database::query_db_bind1<int, const std::string&>(
                                      _db, gender_rate_query, species
                                  );

        float chance_female = (gender_rate_from_db == -1)
                                   ? 0.0f : _veekun_gender_rates.at(gender_rate_from_db);

        if(gender_rate_from_db == -1) {
            return GENDERLESS;
        } else if(pkmn_floats_close(chance_female, 0.0)) {
            return MALE;
        } else if(pkmn_floats_close(chance_female, 0.125)) {
            return (IV_attack < 2) ? FEMALE : MALE;
        } else if(pkmn_floats_close(chance_female, 0.25)) {
            return (IV_attack < 4) ? FEMALE : MALE;
        } else if(pkmn_floats_close(chance_female, 0.5)) {
            return (IV_attack < 7) ? FEMALE : MALE;
        } else if(pkmn_floats_close(chance_female, 0.75)) {
            return (IV_attack < 12) ? FEMALE : MALE;
        } else {
            return FEMALE;
        }
    }

    std::string modern_pokemon_gender(
        const std::string &species,
        uint32_t personality
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        int gender_rate_from_db = pkmn::database::query_db_bind1<int, const std::string&>(
                                      _db, gender_rate_query, species
                                  );

        float chance_female = (gender_rate_from_db == -1)
                                   ? 0.0f : _veekun_gender_rates.at(gender_rate_from_db);

        uint8_t truncated_pid = uint8_t(personality & 0xFF);
        if(gender_rate_from_db == -1) {
            return GENDERLESS;
        } else if(pkmn_floats_close(chance_female, 0.0)) {
            return MALE;
        } else if(pkmn_floats_close(chance_female, 0.125)) {
            return (truncated_pid < 31) ? FEMALE : MALE;
        } else if(pkmn_floats_close(chance_female, 0.25)) {
            return (truncated_pid < 64) ? FEMALE : MALE;
        } else if(pkmn_floats_close(chance_female, 0.5)) {
            return (truncated_pid < 127) ? FEMALE : MALE;
        } else if(pkmn_floats_close(chance_female, 0.75)) {
            return (truncated_pid < 191) ? FEMALE : MALE;
        } else {
            return FEMALE;
        }
    }

}}
