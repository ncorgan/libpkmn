/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/pokemon.h>

#include <boost/assign.hpp>

#include <map>

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
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_species(),
                    species_out,
                    buffer_len
               );
    )
}

pkmn_error_t pkmn_pokemon_get_form(
    pkmn_pokemon_handle_t handle,
    char* form_out,
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_form(),
                    form_out,
                    buffer_len
               );
    )
}

pkmn_error_t pkmn_pokemon_get_game(
    pkmn_pokemon_handle_t handle,
    char* game_out,
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_game(),
                    game_out,
                    buffer_len
               );
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
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_nickname(),
                    nickname_out,
                    buffer_len
               );
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

pkmn_error_t pkmn_pokemon_is_shiny(
    pkmn_pokemon_handle_t handle,
    bool* is_shiny_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *is_shiny_out = handle->cpp->is_shiny();
    )
}

pkmn_error_t pkmn_pokemon_set_shininess(
    pkmn_pokemon_handle_t handle,
    bool value
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_shininess(value);
    )
}

pkmn_error_t pkmn_pokemon_get_held_item(
    pkmn_pokemon_handle_t handle,
    char* held_item_out,
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_held_item().get_name(),
                    held_item_out,
                    buffer_len
               );
    )
}

pkmn_error_t pkmn_pokemon_set_held_item(
    pkmn_pokemon_handle_t handle,
    const char* held_item
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_held_item(held_item);
    )
}

pkmn_error_t pkmn_pokemon_get_trainer_name(
    pkmn_pokemon_handle_t handle,
    char* trainer_name_out,
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_trainer_name(),
                    trainer_name_out,
                    buffer_len
               );
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

static const std::map<std::string, pkmn_gender_t> genders = boost::assign::map_list_of
    ("Male", PKMN_MALE)
    ("Female", PKMN_FEMALE)
    ("Genderless", PKMN_GENDERLESS)
;

pkmn_error_t pkmn_pokemon_get_trainer_gender(
    pkmn_pokemon_handle_t handle,
    pkmn_gender_t* trainer_gender_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *trainer_gender_out = genders.at(handle->cpp->get_trainer_gender());
    )
}

pkmn_error_t pkmn_pokemon_set_trainer_gender(
    pkmn_pokemon_handle_t handle,
    pkmn_gender_t trainer_gender
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        switch(trainer_gender) {
            case PKMN_MALE:
                handle->cpp->set_trainer_gender("Male");
                break;

            case PKMN_FEMALE:
                handle->cpp->set_trainer_gender("Female");
                break;

            default:
                throw std::invalid_argument("Invalid gender.");
        };
    )
}

pkmn_error_t pkmn_pokemon_get_friendship(
    pkmn_pokemon_handle_t handle,
    int* friendship_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *friendship_out = handle->cpp->get_friendship();
    )
}

pkmn_error_t pkmn_pokemon_set_friendship(
    pkmn_pokemon_handle_t handle,
    int friendship
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_friendship(friendship);
    )
}

pkmn_error_t pkmn_pokemon_get_ability(
    pkmn_pokemon_handle_t handle,
    char* ability_out,
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_ability(),
                    ability_out,
                    buffer_len
               );
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
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_ball(),
                    ball_out,
                    buffer_len
               );
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

pkmn_error_t pkmn_pokemon_get_level_met(
    pkmn_pokemon_handle_t handle,
    int* level_met_out
) {
    PKMN_CPP_TO_C(
        *level_met_out = handle->cpp->get_level_met();
    )
}

pkmn_error_t pkmn_pokemon_set_level_met(
    pkmn_pokemon_handle_t handle,
    int level_met
) {
    PKMN_CPP_TO_C(
        handle->cpp->set_level_met(level_met);
    )
}

pkmn_error_t pkmn_pokemon_get_location_met(
    pkmn_pokemon_handle_t handle,
    bool as_egg,
    char* location_met_out,
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_location_met(as_egg),
                    location_met_out,
                    buffer_len
               );
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
    size_t buffer_len
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_original_game(),
                    original_game_out,
                    buffer_len
               );
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

pkmn_error_t pkmn_pokemon_get_marking_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* marking_names_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_map_keys_to_string_list<bool>(
            handle->cpp->get_markings(),
            marking_names_out
        );
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

pkmn_error_t pkmn_pokemon_get_ribbon_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* ribbon_names_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_map_keys_to_string_list<bool>(
            handle->cpp->get_ribbons(),
            ribbon_names_out
        );
    )
}

pkmn_error_t pkmn_pokemon_has_ribbon(
    pkmn_pokemon_handle_t handle,
    const char* ribbon,
    bool* has_ribbon
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *has_ribbon = handle->cpp->get_ribbons().at(ribbon);
    )
}

pkmn_error_t pkmn_pokemon_set_ribbon(
    pkmn_pokemon_handle_t handle,
    const char* ribbon,
    bool value
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_ribbon(ribbon, value);
    )
}

pkmn_error_t pkmn_pokemon_get_moves(
    pkmn_pokemon_handle_t handle,
    pkmn_move_slots_t* move_slots_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_move_slots_cpp_to_c(
            handle->cpp->get_moves(),
            move_slots_out
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

pkmn_error_t pkmn_pokemon_get_contest_stat_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* contest_stat_names_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_map_keys_to_string_list<int>(
            handle->cpp->get_contest_stats(),
            contest_stat_names_out
        );
    )
}

pkmn_error_t pkmn_pokemon_get_contest_stat(
    pkmn_pokemon_handle_t handle,
    const char* stat,
    int* contest_stat_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *contest_stat_out = handle->cpp->get_contest_stats().at(stat);
    )
}

pkmn_error_t pkmn_pokemon_set_contest_stat(
    pkmn_pokemon_handle_t handle,
    const char* stat,
    int contest_stat
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_contest_stat(stat, contest_stat);
    )
}

pkmn_error_t pkmn_pokemon_get_EV_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* EV_names_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_map_keys_to_string_list<int>(
            handle->cpp->get_EVs(),
            EV_names_out
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

pkmn_error_t pkmn_pokemon_get_IV_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* IV_names_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_map_keys_to_string_list<int>(
            handle->cpp->get_IVs(),
            IV_names_out
        );
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

pkmn_error_t pkmn_pokemon_get_stat_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* stat_names_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_map_keys_to_string_list<int>(
            handle->cpp->get_stats(),
            stat_names_out
        );
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
