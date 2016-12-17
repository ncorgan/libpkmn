/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/pokemon_entry.h>

#include <pkmn/database/pokemon_entry.hpp>

#include <cstdlib>
#include <cstring>

pkmn_error_t pkmn_database_get_pokemon_entry(
    const char* species,
    const char* game,
    const char* form,
    pkmn_database_pokemon_entry_t* pokemon_entry_out
) {
    PKMN_CPP_TO_C(
        pkmn::database::pokemon_entry pokemon_entry_cpp(
                                          species,
                                          game,
                                          form
                                      );

        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_name(),
            &pokemon_entry_out->name
        );
        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_game(),
            &pokemon_entry_out->game
        );
        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_form(),
            &pokemon_entry_out->form
        );
        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_species(),
            &pokemon_entry_out->species
        );
        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_pokedex_entry(),
            &pokemon_entry_out->pokedex_entry
        );

        pokemon_entry_out->height = pokemon_entry_cpp.get_height();
        pokemon_entry_out->weight = pokemon_entry_cpp.get_weight();
        pokemon_entry_out->chance_male = pokemon_entry_cpp.get_chance_male();
        pokemon_entry_out->chance_female = pokemon_entry_cpp.get_chance_female();
        pokemon_entry_out->has_gender_differences = pokemon_entry_cpp.has_gender_differences();
        pokemon_entry_out->base_happiness = pokemon_entry_cpp.get_base_happiness();

        pkmn::std_pair_std_string_to_string_pair(
            pokemon_entry_cpp.get_types(),
            &pokemon_entry_out->types
        );
        pkmn::std_pair_std_string_to_string_pair(
            pokemon_entry_cpp.get_abilities(),
            &pokemon_entry_out->abilities
        );

        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_hidden_ability(),
            &pokemon_entry_out->hidden_ability
        );

        pkmn::std_pair_std_string_to_string_pair(
            pokemon_entry_cpp.get_egg_groups(),
            &pokemon_entry_out->egg_groups
        );

        std::map<std::string, int> base_stats = pokemon_entry_cpp.get_base_stats();
        pokemon_entry_out->base_stats[PKMN_STAT_HP] = base_stats.at("HP");
        pokemon_entry_out->base_stats[PKMN_STAT_ATTACK] = base_stats.at("Attack");
        pokemon_entry_out->base_stats[PKMN_STAT_DEFENSE] = base_stats.at("Defense");
        pokemon_entry_out->base_stats[PKMN_STAT_SPEED] = base_stats.at("Speed");
        if(base_stats.count("Special") > 0) {
            pokemon_entry_out->base_stats[PKMN_STAT_SPECIAL] = base_stats.at("Special");
            pokemon_entry_out->base_stats[PKMN_STAT_SPATK] = -1;
            pokemon_entry_out->base_stats[PKMN_STAT_SPDEF] = -1;
        } else {
            pokemon_entry_out->base_stats[PKMN_STAT_SPECIAL] = -1;
            pokemon_entry_out->base_stats[PKMN_STAT_SPATK] = base_stats.at("Special Attack");
            pokemon_entry_out->base_stats[PKMN_STAT_SPDEF] = base_stats.at("Special Defense");
        }

        std::map<std::string, int> EV_yields = pokemon_entry_cpp.get_EV_yields();
        pokemon_entry_out->EV_yields[PKMN_STAT_HP] = EV_yields.at("HP");
        pokemon_entry_out->EV_yields[PKMN_STAT_ATTACK] = EV_yields.at("Attack");
        pokemon_entry_out->EV_yields[PKMN_STAT_DEFENSE] = EV_yields.at("Defense");
        pokemon_entry_out->EV_yields[PKMN_STAT_SPEED] = EV_yields.at("Speed");
        if(base_stats.count("Special") > 0) {
            pokemon_entry_out->EV_yields[PKMN_STAT_SPECIAL] = EV_yields.at("Special");
            pokemon_entry_out->EV_yields[PKMN_STAT_SPATK] = -1;
            pokemon_entry_out->EV_yields[PKMN_STAT_SPDEF] = -1;
        } else {
            pokemon_entry_out->EV_yields[PKMN_STAT_SPECIAL] = -1;
            pokemon_entry_out->EV_yields[PKMN_STAT_SPATK] = EV_yields.at("Special Attack");
            pokemon_entry_out->EV_yields[PKMN_STAT_SPDEF] = EV_yields.at("Special Defense");
        }

        pokemon_entry_out->experience_yield = pokemon_entry_cpp.get_experience_yield();

        pkmn::pkmn_levelup_moves_cpp_to_c(
            pokemon_entry_cpp.get_levelup_moves(),
            &pokemon_entry_out->levelup_moves
        );

        pkmn::pkmn_move_list_to_string_list(
            pokemon_entry_cpp.get_tm_hm_moves(),
            &pokemon_entry_out->tm_hm_moves
        );
        pkmn::pkmn_move_list_to_string_list(
            pokemon_entry_cpp.get_egg_moves(),
            &pokemon_entry_out->egg_moves
        );
        pkmn::pkmn_move_list_to_string_list(
            pokemon_entry_cpp.get_tutor_moves(),
            &pokemon_entry_out->tutor_moves
        );

        pkmn::std_vector_std_string_to_string_list(
            pokemon_entry_cpp.get_forms(),
            &pokemon_entry_out->forms
        );

        pkmn::pkmn_pokemon_entries_to_string_list(
            pokemon_entry_cpp.get_evolutions(),
            &pokemon_entry_out->evolutions
        );
    )
}

pkmn_error_t pkmn_database_pokemon_entry_set_form(
    pkmn_database_pokemon_entry_t* pokemon_entry,
    const char* form
) {
    // Just to see if it exists
    PKMN_CPP_TO_C(
        pkmn::database::pokemon_entry test_entry(
                                          pokemon_entry->name,
                                          pokemon_entry->game,
                                          form
                                      );

        pkmn_database_pokemon_entry_free(pokemon_entry);
        pkmn_database_get_pokemon_entry(
            test_entry.get_name().c_str(),
            test_entry.get_game().c_str(),
            form,
            pokemon_entry
        );
    )
}

pkmn_error_t pkmn_database_pokemon_entry_free(
    pkmn_database_pokemon_entry_t* pokemon_entry
) {
    std::free(pokemon_entry->name);
    pokemon_entry->name = NULL;

    std::free(pokemon_entry->game);
    pokemon_entry->game = NULL;

    std::free(pokemon_entry->form);
    pokemon_entry->form = NULL;

    std::free(pokemon_entry->species);
    pokemon_entry->species = NULL;

    std::free(pokemon_entry->pokedex_entry);
    pokemon_entry->pokedex_entry = NULL;

    pokemon_entry->has_gender_differences = false;
    pokemon_entry->base_happiness = 0;

    pkmn_string_pair_free(&pokemon_entry->types);
    pkmn_string_pair_free(&pokemon_entry->abilities);

    std::free(pokemon_entry->hidden_ability);

    pkmn_string_pair_free(&pokemon_entry->egg_groups);

    for(int i = 0; i < 7; ++i) {
        pokemon_entry->base_stats[i] = 0;
        pokemon_entry->EV_yields[i] = 0;
    }

    pokemon_entry->experience_yield = 0;

    pkmn_levelup_moves_free(&pokemon_entry->levelup_moves);

    pkmn_string_list_free(&pokemon_entry->tm_hm_moves);
    pkmn_string_list_free(&pokemon_entry->egg_moves);
    pkmn_string_list_free(&pokemon_entry->tutor_moves);
    pkmn_string_list_free(&pokemon_entry->forms);
    pkmn_string_list_free(&pokemon_entry->evolutions);

    return PKMN_ERROR_NONE;
}
