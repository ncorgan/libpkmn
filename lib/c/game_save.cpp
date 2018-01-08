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

#include <unordered_map>

#define ITEM_BAG_INTERNAL_RCAST(ptr) reinterpret_cast<pkmn_item_bag_internal_t*>(ptr)
#define ITEM_LIST_INTERNAL_RCAST(ptr) reinterpret_cast<pkmn_item_list_internal_t*>(ptr)

static const std::unordered_map<std::string, int> GAME_GENERATIONS = boost::assign::map_list_of
    ("Red", 1)("Blue", 1)("Yellow", 1)
    ("Gold", 2)("Silver", 2)("Crystal", 2)
    ("Ruby", 3)("Sapphire", 3)("Emerald", 3)("FireRed", 3)("LeafGreen", 3)
    ("Colosseum", 3)("XD", 3)
;

static const std::unordered_map<std::string, pkmn_game_save_type_t> GAME_SAVE_TYPES = boost::assign::map_list_of
    ("None", PKMN_GAME_SAVE_TYPE_NONE)
    ("Red/Blue/Yellow", PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW)
    ("Gold/Silver", PKMN_GAME_SAVE_TYPE_GOLD_SILVER)
    ("Crystal", PKMN_GAME_SAVE_TYPE_CRYSTAL)
    ("Ruby/Sapphire", PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE)
    ("Emerald", PKMN_GAME_SAVE_TYPE_EMERALD)
    ("FireRed/LeafGreen", PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN)
    ("Colosseum/XD", PKMN_GAME_SAVE_TYPE_COLOSSEUM_XD)
;


typedef boost::bimap<std::string, pkmn_gender_t> gender_bimap_t;
static const gender_bimap_t GENDER_BIMAP = boost::assign::list_of<gender_bimap_t::relation>
    ("Male",       PKMN_MALE)
    ("Female",     PKMN_FEMALE)
    ("Genderless", PKMN_GENDERLESS)
;

static const std::vector<std::string> MALE_ONLY_GAMES = boost::assign::list_of
    ("Red")("Blue")("Yellow")
    ("Gold")("Silver")
    ("Colosseum")("XD")
;

pkmn_error_t pkmn_game_save_detect_type(
    const char* filepath,
    pkmn_game_save_type_t* game_save_type_out
) {
    PKMN_CHECK_NULL_PARAM(filepath);
    PKMN_CHECK_NULL_PARAM(game_save_type_out);

    PKMN_CPP_TO_C(
        *game_save_type_out = GAME_SAVE_TYPES.at(
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
)
{
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM((*handle_ptr));

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(handle_ptr);
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
    size_t buffer_len,
    size_t* filepath_length_out
)
{
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(filepath_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::c::string_cpp_to_c(
            handle->cpp->get_filepath(),
            filepath_out,
            buffer_len,
            filepath_length_out
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
    size_t buffer_len,
    size_t* game_length_out
)
{
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::c::string_cpp_to_c(
            handle->cpp->get_game(),
            game_out,
            buffer_len,
            game_length_out
        );
    )
}

pkmn_error_t pkmn_game_save_get_trainer_info(
    pkmn_game_save_handle_t handle,
    pkmn_trainer_info_t* trainer_info_out
)
{
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_info_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        std::strncpy(
            trainer_info_out->trainer_name,
            handle->cpp->get_trainer_name().c_str(),
            sizeof(trainer_info_out->trainer_name)
        );
        trainer_info_out->trainer_id.id = handle->cpp->get_trainer_id();
        trainer_info_out->trainer_gender = GENDER_BIMAP.left.at(
                                               handle->cpp->get_trainer_gender()
                                           );
    )
}

pkmn_error_t pkmn_game_save_set_trainer_info(
    pkmn_game_save_handle_t handle,
    pkmn_trainer_info_t* trainer_info
)
{
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_info, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        /*
         * Preserve what's here in case we need to restore it if not every call
         * succeeds.
         */
        std::string game = handle->cpp->get_game();
        std::string current_trainer_name = handle->cpp->get_trainer_name();
        uint32_t current_trainer_id = handle->cpp->get_trainer_id();
        std::string current_trainer_gender = handle->cpp->get_trainer_gender();

        try
        {
            handle->cpp->set_trainer_name(trainer_info->trainer_name);
            handle->cpp->set_trainer_id(trainer_info->trainer_id.id);
            if(std::find(MALE_ONLY_GAMES.begin(), MALE_ONLY_GAMES.end(), game) == MALE_ONLY_GAMES.end())
            {
                handle->cpp->set_trainer_gender(GENDER_BIMAP.right.at(trainer_info->trainer_gender));
            }
        }
        catch(const std::exception&)
        {
            handle->cpp->set_trainer_name(current_trainer_name);
            handle->cpp->set_trainer_id(current_trainer_id);
            if(std::find(MALE_ONLY_GAMES.begin(), MALE_ONLY_GAMES.end(), game) == MALE_ONLY_GAMES.end())
            {
                handle->cpp->set_trainer_gender(current_trainer_gender);
            }

            throw;
        }
        catch(...)
        {
            handle->cpp->set_trainer_name(current_trainer_name);
            handle->cpp->set_trainer_id(current_trainer_id);
            if(std::find(MALE_ONLY_GAMES.begin(), MALE_ONLY_GAMES.end(), game) == MALE_ONLY_GAMES.end())
            {
                handle->cpp->set_trainer_gender(current_trainer_gender);
            }

            return PKMN_ERROR_UNKNOWN_ERROR;
        }
    )
}

pkmn_error_t pkmn_game_save_get_rival_name(
    pkmn_game_save_handle_t handle,
    char* rival_name_out,
    size_t buffer_len,
    size_t* rival_name_length_out
)
{
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(rival_name_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::c::string_cpp_to_c(
            handle->cpp->get_rival_name(),
            rival_name_out,
            buffer_len,
            rival_name_length_out
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
    pkmn_item_bag_t* item_bag_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_bag_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        item_bag_out->_internal = reinterpret_cast<void*>(new pkmn_item_bag_internal_t);
        ITEM_BAG_INTERNAL_RCAST(item_bag_out->_internal)->cpp = handle->cpp->get_item_bag();
        init_item_bag(item_bag_out);
    )
}

pkmn_error_t pkmn_game_save_get_item_pc(
    pkmn_game_save_handle_t handle,
    pkmn_item_list_t* item_pc_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_pc_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        item_pc_out->_internal = reinterpret_cast<void*>(new pkmn_item_list_internal_t);
        ITEM_LIST_INTERNAL_RCAST(item_pc_out->_internal)->cpp = handle->cpp->get_item_pc();
        init_item_list(item_pc_out);
    )
}
