/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/pokemon.h>

#include <pkmn/database/pokemon_entry.hpp>

pkmn_error_t pkmn_database_pokemon_species(
    const char* pokemon_name,
    const char* game_name,
    char* pokemon_species_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, ""
            ).get_species(),

            pokemon_species_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_pokedex_entry(
    const char* pokemon_name,
    const char* game_name,
    char* pokemon_pokedex_entry_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, ""
            ).get_pokedex_entry(),

            pokemon_pokedex_entry_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_height(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    float* height_out
) {
    PKMN_CPP_TO_C(
        *height_out = pkmn::database::pokemon_entry(
                          pokemon_name, game_name,
                          form_name
                      ).get_height();
    )
}

pkmn_error_t pkmn_database_pokemon_weight(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    float* weight_out
) {
    PKMN_CPP_TO_C(
        *weight_out = pkmn::database::pokemon_entry(
                          pokemon_name, game_name,
                          form_name
                      ).get_weight();
    )
}

pkmn_error_t pkmn_database_pokemon_chance_male(
    const char* pokemon_name,
    const char* game_name,
    float* chance_male_out
) {
    PKMN_CPP_TO_C(
        *chance_male_out = pkmn::database::pokemon_entry(
                               pokemon_name, game_name, ""
                           ).get_chance_male();
    )
}

pkmn_error_t pkmn_database_pokemon_chance_female(
    const char* pokemon_name,
    const char* game_name,
    float* chance_female_out
) {
    PKMN_CPP_TO_C(
        *chance_female_out = pkmn::database::pokemon_entry(
                                 pokemon_name, game_name, ""
                             ).get_chance_female();
    )
}

pkmn_error_t pkmn_database_pokemon_has_gender_differences(
    const char* pokemon_name,
    const char* game_name,
    bool* has_gender_differences_out
) {
    PKMN_CPP_TO_C(
        *has_gender_differences_out = pkmn::database::pokemon_entry(
                                          pokemon_name, game_name, ""
                                      ).has_gender_differences();
    )
}

pkmn_error_t pkmn_database_pokemon_base_happiness(
    const char* pokemon_name,
    const char* game_name,
    int* base_happiness_out
) {
    PKMN_CPP_TO_C(
        *base_happiness_out = pkmn::database::pokemon_entry(
                                  pokemon_name, game_name, ""
                              ).get_base_happiness();
    )
}

pkmn_error_t pkmn_database_pokemon_types(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_pair_t* types_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_pair_std_string_to_string_pair(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, form_name
            ).get_types(),
            types_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_abilities(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_pair_t* abilities_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_pair_std_string_to_string_pair(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, form_name
            ).get_abilities(),
            abilities_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_hidden_ability(
    const char* pokemon_name,
    const char* game_name,
    char* pokemon_hidden_ability_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, ""
            ).get_hidden_ability(),

            pokemon_hidden_ability_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_egg_groups(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_pair_t* egg_groups_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_pair_std_string_to_string_pair(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, form_name
            ).get_egg_groups(),
            egg_groups_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_base_stat(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    const char* stat_name,
    int* base_stat_out
) {
    PKMN_CPP_TO_C(
        *base_stat_out = pkmn::database::pokemon_entry(
                             pokemon_name, game_name,
                             form_name
                         ).get_base_stats().at(stat_name);
    )
}

pkmn_error_t pkmn_database_pokemon_EV_yield(
    const char* pokemon_name,
    const char* game_name,
    const char* stat_name,
    int* EV_yield_out
) {
    PKMN_CPP_TO_C(
        *EV_yield_out = pkmn::database::pokemon_entry(
                            pokemon_name, game_name, ""
                        ).get_EV_yields().at(stat_name);
    )
}

pkmn_error_t pkmn_database_pokemon_experience_yield(
    const char* pokemon_name,
    const char* game_name,
    int* experience_yield_out
) {
    PKMN_CPP_TO_C(
        *experience_yield_out = pkmn::database::pokemon_entry(
                                    pokemon_name, game_name, ""
                                ).get_experience_yield();
    )
}

pkmn_error_t pkmn_database_pokemon_experience_at_level(
    const char* pokemon_name,
    const char* game_name,
    int level,
    int* experience_out
) {
    PKMN_CPP_TO_C(
        *experience_out = pkmn::database::pokemon_entry(
                              pokemon_name, game_name, ""
                          ).get_experience_at_level(level);
    )
}

pkmn_error_t pkmn_database_pokemon_level_at_experience(
    const char* pokemon_name,
    const char* game_name,
    int experience,
    int* level_out
) {
    PKMN_CPP_TO_C(
        *level_out = pkmn::database::pokemon_entry(
                         pokemon_name, game_name, ""
                     ).get_level_at_experience(experience);
    )
}

pkmn_error_t pkmn_database_pokemon_levelup_moves(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_levelup_moves_t* levelup_moves_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::pkmn_levelup_moves_cpp_to_c(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, form_name
            ).get_levelup_moves(),

            levelup_moves_out,
            list_length_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_tm_hm_moves(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_list_t* tm_hm_moves_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::pkmn_move_list_to_string_list(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, form_name
            ).get_tm_hm_moves(),

            tm_hm_moves_out,
            list_length_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_egg_moves(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_list_t* egg_moves_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::pkmn_move_list_to_string_list(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, form_name
            ).get_egg_moves(),

            egg_moves_out,
            list_length_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_tutor_moves(
    const char* pokemon_name,
    const char* game_name,
    const char* form_name,
    pkmn_string_list_t* tutor_moves_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::pkmn_move_list_to_string_list(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, form_name
            ).get_tutor_moves(),

            tutor_moves_out,
            list_length_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_forms(
    const char* pokemon_name,
    const char* game_name,
    pkmn_string_list_t* forms_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, ""
            ).get_forms(),

            forms_out,
            list_length_out
        );
    )
}

pkmn_error_t pkmn_database_pokemon_evolutions(
    const char* pokemon_name,
    const char* game_name,
    pkmn_string_list_t* evolutions_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::pkmn_pokemon_entries_to_string_list(
            pkmn::database::pokemon_entry(
                pokemon_name, game_name, ""
            ).get_evolutions(),

            evolutions_out,
            list_length_out
        );
    )
}
