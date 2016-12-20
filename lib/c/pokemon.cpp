/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/pokemon.h>

pkmn_error_t pkmn_pokemon_make(
    pkmn_pokemon_handle_t* handle_ptr,
    const char* species,
    const char* game,
    const char* form,
    int level
) {
    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::make(
                                        species,
                                        game,
                                        form,
                                        level
                                    );

        (*handle_ptr) = new pkmn_pokemon_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_pokemon_from_file(
    pkmn_pokemon_handle_t* handle_ptr,
    const char* filepath
) {
    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::from_file(
                                      filepath
                                  );

        (*handle_ptr) = new pkmn_pokemon_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_pokemon_free(
    pkmn_pokemon_handle_t* handle_ptr
) {
    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_pokemon_strerror(
    pkmn_pokemon_handle_t handle
) {
    return handle->last_error.c_str();
}

pkmn_error_t pkmn_pokemon_get_species(
    pkmn_pokemon_handle_t handle,
    char* species_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_species(),
            species_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_get_form(
    pkmn_pokemon_handle_t handle,
    char* form_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_form(),
            form_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_get_game(
    pkmn_pokemon_handle_t handle,
    char* game_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_game(),
            game_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_get_database_entry(
    pkmn_pokemon_handle_t handle,
    pkmn_database_pokemon_entry_t* database_entry_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_pokemon_entry_cpp_to_c(
            handle->cpp->get_database_entry(),
            database_entry_out
        )
    )
}

pkmn_error_t pkmn_pokemon_get_nickname(
    pkmn_pokemon_handle_t handle,
    char* nickname_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_nickname(),
            nickname_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_set_nickname(
    pkmn_pokemon_handle_t handle,
    const char* nickname
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_nickname(nickname)
    )
}

pkmn_error_t pkmn_pokemon_get_held_item(
    pkmn_pokemon_handle_t handle,
    char* held_item_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_held_item().get_name(),
            held_item_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_get_trainer_name(
    pkmn_pokemon_handle_t handle,
    char* trainer_name_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_trainer_name(),
            trainer_name_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_set_trainer_name(
    pkmn_pokemon_handle_t handle,
    const char* trainer_name
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_trainer_name(trainer_name)
    )
}

pkmn_error_t pkmn_pokemon_get_trainer_public_id(
    pkmn_pokemon_handle_t handle,
    uint16_t* trainer_public_id_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *trainer_public_id_out = handle->cpp->get_trainer_public_id();
    )
}

pkmn_error_t pkmn_pokemon_get_trainer_secret_id(
    pkmn_pokemon_handle_t handle,
    uint16_t* trainer_secret_id_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *trainer_secret_id_out = handle->cpp->get_trainer_secret_id();
    )
}

pkmn_error_t pkmn_pokemon_get_trainer_id(
    pkmn_pokemon_handle_t handle,
    uint32_t* trainer_id_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *trainer_id_out = handle->cpp->get_trainer_id();
    )
}

pkmn_error_t pkmn_pokemon_set_trainer_public_id(
    pkmn_pokemon_handle_t handle,
    uint16_t trainer_public_id
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_trainer_public_id(trainer_public_id);
    )
}

pkmn_error_t pkmn_pokemon_set_trainer_secret_id(
    pkmn_pokemon_handle_t handle,
    uint16_t trainer_secret_id
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_trainer_secret_id(trainer_secret_id);
    )
}

pkmn_error_t pkmn_pokemon_set_trainer_id(
    pkmn_pokemon_handle_t handle,
    uint32_t trainer_id
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_trainer_id(trainer_id);
    )
}

pkmn_error_t pkmn_pokemon_get_trainer_gender(
    pkmn_pokemon_handle_t handle,
    char* trainer_gender_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_trainer_gender(),
            trainer_gender_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_set_trainer_gender(
    pkmn_pokemon_handle_t handle,
    const char* trainer_gender
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_trainer_gender(trainer_gender)
    )
}

pkmn_error_t pkmn_pokemon_get_ability(
    pkmn_pokemon_handle_t handle,
    char* ability_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_ability(),
            ability_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_set_ability(
    pkmn_pokemon_handle_t handle,
    const char* ability
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_ability(ability)
    )
}

pkmn_error_t pkmn_pokemon_get_ball(
    pkmn_pokemon_handle_t handle,
    char* ball_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_ball(),
            ball_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_set_ball(
    pkmn_pokemon_handle_t handle,
    const char* ball
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_ball(ball)
    )
}

pkmn_error_t pkmn_pokemon_get_level_caught(
    pkmn_pokemon_handle_t handle,
    int* level_caught_out
) {
    PKMN_CPP_TO_C(
        *level_caught_out = handle->cpp->get_level_caught();
    )
}

pkmn_error_t pkmn_pokemon_set_level_caught(
    pkmn_pokemon_handle_t handle,
    int level_caught
) {
    PKMN_CPP_TO_C(
        handle->cpp->set_level_caught(level_caught);
    )
}

pkmn_error_t pkmn_pokemon_get_location_met(
    pkmn_pokemon_handle_t handle,
    bool as_egg,
    char* location_met_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_location_met(as_egg),
            location_met_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_set_location_met(
    pkmn_pokemon_handle_t handle,
    const char* location_met,
    bool as_egg
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_location_met(
            location_met,
            as_egg
        )
    )
}

pkmn_error_t pkmn_pokemon_get_original_game(
    pkmn_pokemon_handle_t handle,
    char* original_game_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_original_game(),
            original_game_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_pokemon_set_original_game(
    pkmn_pokemon_handle_t handle,
    const char* original_game
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_original_game(original_game)
    )
}

pkmn_error_t pkmn_pokemon_get_personality(
    pkmn_pokemon_handle_t handle,
    uint32_t* personality_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *personality_out = handle->cpp->get_personality();
    )
}

pkmn_error_t pkmn_pokemon_set_personality(
    pkmn_pokemon_handle_t handle,
    uint32_t personality
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_personality(personality);
    )
}

pkmn_error_t pkmn_pokemon_get_experience(
    pkmn_pokemon_handle_t handle,
    int* experience_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *experience_out = handle->cpp->get_experience();
    )
}

pkmn_error_t pkmn_pokemon_set_experience(
    pkmn_pokemon_handle_t handle,
    int experience
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_experience(experience);
    )
}

pkmn_error_t pkmn_pokemon_get_level(
    pkmn_pokemon_handle_t handle,
    int* level_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *level_out = handle->cpp->get_level();
    )
}

pkmn_error_t pkmn_pokemon_set_level(
    pkmn_pokemon_handle_t handle,
    int level
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_level(level);
    )
}

pkmn_error_t pkmn_pokemon_has_marking(
    pkmn_pokemon_handle_t handle,
    const char* marking,
    bool* has_marking
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *has_marking = handle->cpp->get_markings().at(marking);
    )
}

pkmn_error_t pkmn_pokemon_set_marking(
    pkmn_pokemon_handle_t handle,
    const char* marking,
    bool value
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_marking(marking, value);
    )
}

pkmn_error_t pkmn_pokemon_get_moves(
    pkmn_pokemon_handle_t handle,
    pkmn_move_slots_t* move_slots_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_move_slots_cpp_to_c(
            handle->cpp->get_moves(),
            move_slots_out,
            list_length_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_move(
    pkmn_pokemon_handle_t handle,
    const char* move,
    int index
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_move(
            move,
            index
        );
    )
}

pkmn_error_t pkmn_pokemon_get_EV(
    pkmn_pokemon_handle_t handle,
    const char* stat,
    int* EV_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *EV_out = handle->cpp->get_EVs().at(stat);
    )
}

pkmn_error_t pkmn_pokemon_set_EV(
    pkmn_pokemon_handle_t handle,
    const char* stat,
    int EV
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_EV(stat, EV);
    )
}

pkmn_error_t pkmn_pokemon_get_IV(
    pkmn_pokemon_handle_t handle,
    const char* stat,
    int* IV_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *IV_out = handle->cpp->get_IVs().at(stat);
    )
}

pkmn_error_t pkmn_pokemon_set_IV(
    pkmn_pokemon_handle_t handle,
    const char* stat,
    int IV
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_IV(stat, IV);
    )
}

pkmn_error_t pkmn_pokemon_get_stat(
    pkmn_pokemon_handle_t handle,
    const char* stat_name,
    int* stat_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *stat_out = handle->cpp->get_stats().at(stat_name);
    )
}
