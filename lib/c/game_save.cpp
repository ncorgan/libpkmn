/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/game_save.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

static const std::map<std::string, pkmn_game_save_type_t> game_save_types = boost::assign::map_list_of
    ("None", PKMN_GAME_SAVE_TYPE_NONE)
    ("Red/Blue/Yellow", PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW)
    ("Gold/Silver", PKMN_GAME_SAVE_TYPE_GOLD_SILVER)
    ("Crystal", PKMN_GAME_SAVE_TYPE_CRYSTAL)
    ("Ruby/Sapphire", PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE)
    ("Emerald", PKMN_GAME_SAVE_TYPE_EMERALD)
    ("FireRed/LeafGreen", PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN)
    ("Colosseum/XD", PKMN_GAME_SAVE_TYPE_COLOSSEUM_XD)
;

static const std::map<std::string, pkmn_gender_t> genders = boost::assign::map_list_of
    ("Male", PKMN_MALE)
    ("Female", PKMN_FEMALE)
;

pkmn_error_t pkmn_game_save_detect_type(
    const char* filepath,
    pkmn_game_save_type_t* game_save_type_out
) {
    PKMN_CHECK_NULL_PARAM(filepath);
    PKMN_CHECK_NULL_PARAM(game_save_type_out);

    PKMN_CPP_TO_C(
        *game_save_type_out = game_save_types.at(
                                  pkmn::game_save::detect_type(filepath)
                              );
    )
}

pkmn_error_t pkmn_game_save_from_file(
    pkmn_game_save_handle_t* handle_ptr,
    const char* filepath
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM(filepath);

    PKMN_CPP_TO_C(
        pkmn::game_save::sptr cpp = pkmn::game_save::from_file(
                                       filepath
                                    );

        (*handle_ptr) = new pkmn_game_save_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_game_save_free(
    pkmn_game_save_handle_t* handle_ptr
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM((*handle_ptr));

    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_game_save_strerror(
    pkmn_game_save_handle_t handle
) {
    if(!handle) {
        return NULL;
    }

    try {
        boost::mutex::scoped_lock lock(handle->error_mutex);
        return handle->last_error.c_str();
    } catch(...) {
        return NULL;
    }
}

pkmn_error_t pkmn_game_save_get_filepath(
    pkmn_game_save_handle_t handle,
    char* filepath_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(filepath_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_game_save_handle_t>(
                   handle,
                   handle->cpp->get_filepath(),
                   filepath_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_game_save_save(
    pkmn_game_save_handle_t handle
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->save();
    )
}

pkmn_error_t pkmn_game_save_save_as(
    pkmn_game_save_handle_t handle,
    const char* filepath
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(filepath, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->save_as(filepath);
    )
}

pkmn_error_t pkmn_game_save_get_game(
    pkmn_game_save_handle_t handle,
    char* game_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_game_save_handle_t>(
                   handle,
                   handle->cpp->get_game(),
                   game_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_game_save_get_trainer_name(
    pkmn_game_save_handle_t handle,
    char* trainer_name_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_name_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_game_save_handle_t>(
                   handle,
                   handle->cpp->get_trainer_name(),
                   trainer_name_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_game_save_set_trainer_name(
    pkmn_game_save_handle_t handle,
    const char* trainer_name
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_name, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_trainer_name(trainer_name);
    )
}

pkmn_error_t pkmn_game_save_get_trainer_id(
    pkmn_game_save_handle_t handle,
    uint32_t* trainer_id_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_id_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *trainer_id_out = handle->cpp->get_trainer_id();
    )
}

pkmn_error_t pkmn_game_save_set_trainer_id(
    pkmn_game_save_handle_t handle,
    uint32_t trainer_id
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_trainer_id(trainer_id);
    )
}

pkmn_error_t pkmn_game_save_get_trainer_public_id(
    pkmn_game_save_handle_t handle,
    uint16_t* trainer_public_id_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_public_id_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *trainer_public_id_out = handle->cpp->get_trainer_public_id();
    )
}

pkmn_error_t pkmn_game_save_set_trainer_public_id(
    pkmn_game_save_handle_t handle,
    uint16_t trainer_public_id
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_trainer_public_id(trainer_public_id);
    )
}

pkmn_error_t pkmn_game_save_get_trainer_secret_id(
    pkmn_game_save_handle_t handle,
    uint16_t* trainer_secret_id_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_secret_id_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *trainer_secret_id_out = handle->cpp->get_trainer_secret_id();
    )
}

pkmn_error_t pkmn_game_save_set_trainer_secret_id(
    pkmn_game_save_handle_t handle,
    uint16_t trainer_secret_id
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_trainer_secret_id(trainer_secret_id);
    )
}

pkmn_error_t pkmn_game_save_get_trainer_gender(
    pkmn_game_save_handle_t handle,
    pkmn_gender_t* gender_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(gender_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *gender_out = genders.at(handle->cpp->get_trainer_gender());
    )
}

pkmn_error_t pkmn_game_save_set_trainer_gender(
    pkmn_game_save_handle_t handle,
    pkmn_gender_t gender
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        switch(gender) {
            case PKMN_MALE:
                handle->cpp->set_trainer_gender("Male");
                break;

            case PKMN_FEMALE:
                handle->cpp->set_trainer_gender("Female");
                break;

            default:
                throw std::invalid_argument("gender: valid values PKMN_MALE, PKMN_FEMALE");
        }
    )
}

pkmn_error_t pkmn_game_save_get_rival_name(
    pkmn_game_save_handle_t handle,
    char* rival_name_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(rival_name_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_game_save_handle_t>(
                   handle,
                   handle->cpp->get_rival_name(),
                   rival_name_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_game_save_set_rival_name(
    pkmn_game_save_handle_t handle,
    const char* rival_name
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(rival_name, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_rival_name(rival_name);
    )
}

pkmn_error_t pkmn_game_save_get_money(
    pkmn_game_save_handle_t handle,
    int* money_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(money_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *money_out = handle->cpp->get_money();
    )
}

pkmn_error_t pkmn_game_save_set_money(
    pkmn_game_save_handle_t handle,
    int money
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_money(money);
    )
}

pkmn_error_t pkmn_game_save_get_pokemon_party(
    pkmn_game_save_handle_t handle,
    pkmn_pokemon_party_handle_t* pokemon_party_handle_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_party_handle_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        (*pokemon_party_handle_out) = new pkmn_pokemon_party_t;
        (*pokemon_party_handle_out)->cpp = handle->cpp->get_pokemon_party();
        (*pokemon_party_handle_out)->last_error = "None";
    )
}

pkmn_error_t pkmn_game_save_get_pokemon_pc(
    pkmn_game_save_handle_t handle,
    pkmn_pokemon_pc_handle_t* pokemon_pc_handle_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_pc_handle_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        (*pokemon_pc_handle_out) = new pkmn_pokemon_pc_t;
        (*pokemon_pc_handle_out)->cpp = handle->cpp->get_pokemon_pc();
        (*pokemon_pc_handle_out)->last_error = "None";
    )
}

pkmn_error_t pkmn_game_save_get_item_bag(
    pkmn_game_save_handle_t handle,
    pkmn_item_bag_handle_t* item_bag_handle_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_bag_handle_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        (*item_bag_handle_out) = new pkmn_item_bag_t;
        (*item_bag_handle_out)->cpp = handle->cpp->get_item_bag();
        (*item_bag_handle_out)->last_error = "None";
    )
}

pkmn_error_t pkmn_game_save_get_item_pc(
    pkmn_game_save_handle_t handle,
    pkmn_item_list_handle_t* item_pc_handle_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_pc_handle_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        // This call may fail, so do it before allocating memory.
        pkmn::item_list::sptr item_pc = handle->cpp->get_item_pc();

        (*item_pc_handle_out) = new pkmn_item_list_t;
        (*item_pc_handle_out)->cpp = handle->cpp->get_item_pc();
        (*item_pc_handle_out)->last_error = "None";
    )
}
