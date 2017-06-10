/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/pokemon.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <cstdio>
#include <unordered_map>

static const std::unordered_map<std::string, int> GAME_GENERATIONS = boost::assign::map_list_of
    ("Red", 1)("Blue", 1)("Yellow", 1)
    ("Gold", 2)("Silver", 2)("Crystal", 2)
    ("Ruby", 3)("Sapphire", 3)("Emerald", 3)("FireRed", 3)("LeafGreen", 3)
    ("Colosseum", 3)("XD", 3)
;

typedef boost::bimap<std::string, pkmn_gender_t> gender_bimap_t;
static const gender_bimap_t GENDER_BIMAP = boost::assign::list_of<gender_bimap_t::relation>
    ("Male",       PKMN_MALE)
    ("Female",     PKMN_FEMALE)
    ("Genderless", PKMN_GENDERLESS)
;

pkmn_error_t pkmn_pokemon_make(
    pkmn_pokemon_handle_t* handle_ptr,
    const char* species,
    const char* game,
    const char* form,
    int level
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM(species);
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(form);

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
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM(filepath);

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
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM((*handle_ptr));

    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_pokemon_strerror(
    pkmn_pokemon_handle_t handle
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

pkmn_error_t pkmn_pokemon_get_species(
    pkmn_pokemon_handle_t handle,
    char* species_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(species_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_species(),
                    species_out,
                    buffer_len
               );
    )
}

pkmn_error_t pkmn_pokemon_get_game(
    pkmn_pokemon_handle_t handle,
    char* game_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_game(),
                    game_out,
                    buffer_len
               );
    )
}

pkmn_error_t pkmn_pokemon_get_form(
    pkmn_pokemon_handle_t handle,
    char* form_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(form_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_form(),
                    form_out,
                    buffer_len
               );
    )
}

pkmn_error_t pkmn_pokemon_set_form(
    pkmn_pokemon_handle_t handle,
    const char* form
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(form, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_form(form);
    )
}

pkmn_error_t pkmn_pokemon_get_database_entry(
    pkmn_pokemon_handle_t handle,
    pkmn_database_pokemon_entry_t* database_entry_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(database_entry_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_pokemon_entry_cpp_to_c(
            handle->cpp->get_database_entry(),
            database_entry_out
        )
    )
}

pkmn_error_t pkmn_pokemon_get_trainer_info(
    pkmn_pokemon_handle_t handle,
    pkmn_trainer_info_t* trainer_info_out
)
{
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_info_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        std::strncpy(
            trainer_info_out->nickname,
            handle->cpp->get_nickname().c_str(),
            sizeof(trainer_info_out->nickname)
        );
        std::strncpy(
            trainer_info_out->trainer_name,
            handle->cpp->get_trainer_name().c_str(),
            sizeof(trainer_info_out->trainer_name)
        );
        trainer_info_out->trainer_id.id = handle->cpp->get_trainer_id();
        trainer_info_out->trainer_gender = GENDER_BIMAP.left.at(handle->cpp->get_trainer_gender());
    )
}

pkmn_error_t pkmn_pokemon_set_trainer_info(
    pkmn_pokemon_handle_t handle,
    const pkmn_trainer_info_t* trainer_info
)
{
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_info, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        /*
         * Preserve what's here in case we need to restore it if not every call
         * succeeds.
         */
        std::string current_nickname = handle->cpp->get_nickname();
        std::string current_trainer_name = handle->cpp->get_trainer_name();
        uint32_t current_trainer_id = handle->cpp->get_trainer_id();
        std::string current_trainer_gender = handle->cpp->get_trainer_gender();

        try
        {
            handle->cpp->set_nickname(trainer_info->nickname);
            handle->cpp->set_trainer_name(trainer_info->trainer_name);
            handle->cpp->set_trainer_id(trainer_info->trainer_id.id);
            if(GAME_GENERATIONS.at(handle->cpp->get_game()) >= 2)
            {
                handle->cpp->set_trainer_gender(GENDER_BIMAP.right.at(trainer_info->trainer_gender));
            }
        }
        catch(const std::exception&)
        {
            handle->cpp->set_nickname(current_nickname);
            handle->cpp->set_trainer_name(current_trainer_name);
            handle->cpp->set_trainer_id(current_trainer_id);
            if(GAME_GENERATIONS.at(handle->cpp->get_game()) >= 2)
            {
                handle->cpp->set_trainer_gender(current_trainer_gender);
            }

            throw;
        }
        catch(...)
        {
            handle->cpp->set_nickname(current_nickname);
            handle->cpp->set_trainer_name(current_trainer_name);
            handle->cpp->set_trainer_id(current_trainer_id);
            if(GAME_GENERATIONS.at(handle->cpp->get_game()) >= 2)
            {
                handle->cpp->set_trainer_gender(current_trainer_gender);
            }

            return PKMN_ERROR_UNKNOWN_ERROR;
        }
    )
}

pkmn_error_t pkmn_pokemon_get_gender(
    pkmn_pokemon_handle_t handle,
    pkmn_gender_t* gender_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(gender_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *gender_out = GENDER_BIMAP.left.at(handle->cpp->get_gender());
    )
}

pkmn_error_t pkmn_pokemon_set_gender(
    pkmn_pokemon_handle_t handle,
    pkmn_gender_t gender
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        switch(gender) {
            case PKMN_MALE:
                handle->cpp->set_gender("Male");
                break;

            case PKMN_FEMALE:
                handle->cpp->set_gender("Female");
                break;

            case PKMN_GENDERLESS:
                handle->cpp->set_gender("Genderless");
                break;

            default:
                throw std::invalid_argument("Invalid gender.");
        };
    )
}

pkmn_error_t pkmn_pokemon_is_shiny(
    pkmn_pokemon_handle_t handle,
    bool* is_shiny_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(is_shiny_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *is_shiny_out = handle->cpp->is_shiny();
    )
}

pkmn_error_t pkmn_pokemon_set_shininess(
    pkmn_pokemon_handle_t handle,
    bool value
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_shininess(value);
    )
}

pkmn_error_t pkmn_pokemon_get_held_item(
    pkmn_pokemon_handle_t handle,
    char* held_item_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(held_item_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_held_item(),
                    held_item_out,
                    buffer_len
               );
    )
}

pkmn_error_t pkmn_pokemon_set_held_item(
    pkmn_pokemon_handle_t handle,
    const char* held_item
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(held_item, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_held_item(held_item);
    )
}

pkmn_error_t pkmn_pokemon_get_friendship(
    pkmn_pokemon_handle_t handle,
    int* friendship_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(friendship_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *friendship_out = handle->cpp->get_friendship();
    )
}

pkmn_error_t pkmn_pokemon_set_friendship(
    pkmn_pokemon_handle_t handle,
    int friendship
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_friendship(friendship);
    )
}

pkmn_error_t pkmn_pokemon_get_ability(
    pkmn_pokemon_handle_t handle,
    char* ability_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ability_out, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ability, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_ability(ability)
    )
}

pkmn_error_t pkmn_pokemon_get_ball(
    pkmn_pokemon_handle_t handle,
    char* ball_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ball_out, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ball, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_ball(ball)
    )
}

pkmn_error_t pkmn_pokemon_get_level_met(
    pkmn_pokemon_handle_t handle,
    int* level_met_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(level_met_out, handle);

    PKMN_CPP_TO_C(
        *level_met_out = handle->cpp->get_level_met();
    )
}

pkmn_error_t pkmn_pokemon_set_level_met(
    pkmn_pokemon_handle_t handle,
    int level_met
) {
    PKMN_CHECK_NULL_PARAM(handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(location_met_out, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(location_met, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(original_game_out, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(original_game, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_original_game(original_game)
    )
}

pkmn_error_t pkmn_pokemon_get_personality(
    pkmn_pokemon_handle_t handle,
    uint32_t* personality_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(personality_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *personality_out = handle->cpp->get_personality();
    )
}

pkmn_error_t pkmn_pokemon_set_personality(
    pkmn_pokemon_handle_t handle,
    uint32_t personality
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_personality(personality);
    )
}

pkmn_error_t pkmn_pokemon_get_experience(
    pkmn_pokemon_handle_t handle,
    int* experience_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(experience_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *experience_out = handle->cpp->get_experience();
    )
}

pkmn_error_t pkmn_pokemon_set_experience(
    pkmn_pokemon_handle_t handle,
    int experience
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_experience(experience);
    )
}

pkmn_error_t pkmn_pokemon_get_level(
    pkmn_pokemon_handle_t handle,
    int* level_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(level_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *level_out = handle->cpp->get_level();
    )
}

pkmn_error_t pkmn_pokemon_set_level(
    pkmn_pokemon_handle_t handle,
    int level
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_level(level);
    )
}

pkmn_error_t pkmn_pokemon_get_marking_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* marking_names_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(marking_names_out, handle);

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
    bool* has_marking_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(marking, handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(has_marking_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        const std::map<std::string, bool>& markings = handle->cpp->get_markings();
        if(markings.count(marking) == 0) {
            throw std::invalid_argument("Invalid stat.");
        }

        *has_marking_out = markings.at(marking);
    )
}

pkmn_error_t pkmn_pokemon_set_marking(
    pkmn_pokemon_handle_t handle,
    const char* marking,
    bool value
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(marking, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_marking(marking, value);
    )
}

pkmn_error_t pkmn_pokemon_get_ribbon_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* ribbon_names_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ribbon_names_out, handle);

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
    bool* has_ribbon_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ribbon, handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(has_ribbon_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        const std::map<std::string, bool>& ribbons = handle->cpp->get_ribbons();
        if(ribbons.count(ribbon) == 0) {
            throw std::invalid_argument("Invalid ribbon.");
        }

        *has_ribbon_out = ribbons.at(ribbon);
    )
}

pkmn_error_t pkmn_pokemon_set_ribbon(
    pkmn_pokemon_handle_t handle,
    const char* ribbon,
    bool value
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ribbon, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_ribbon(ribbon, value);
    )
}

pkmn_error_t pkmn_pokemon_get_moves(
    pkmn_pokemon_handle_t handle,
    pkmn_move_slots_t* move_slots_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(move_slots_out, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(move, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(contest_stat_names_out, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stat, handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(contest_stat_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        const std::map<std::string, int>& contest_stats = handle->cpp->get_contest_stats();
        if(contest_stats.count(stat) == 0) {
            throw std::invalid_argument("Invalid stat.");
        }

        *contest_stat_out = contest_stats.at(stat);
    )
}

pkmn_error_t pkmn_pokemon_set_contest_stat(
    pkmn_pokemon_handle_t handle,
    const char* stat,
    int contest_stat
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stat, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_contest_stat(stat, contest_stat);
    )
}

pkmn_error_t pkmn_pokemon_get_EV_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* EV_names_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(EV_names_out, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stat, handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(EV_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        const std::map<std::string, int>& EVs = handle->cpp->get_EVs();
        if(EVs.count(stat) == 0) {
            throw std::invalid_argument("Invalid stat.");
        }

        *EV_out = EVs.at(stat);
    )
}

pkmn_error_t pkmn_pokemon_set_EV(
    pkmn_pokemon_handle_t handle,
    const char* stat,
    int EV
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stat, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_EV(stat, EV);
    )
}

pkmn_error_t pkmn_pokemon_get_IV_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* IV_names_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(IV_names_out, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stat, handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(IV_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        const std::map<std::string, int>& IVs = handle->cpp->get_IVs();
        if(IVs.count(stat) == 0) {
            throw std::invalid_argument("Invalid stat.");
        }

        *IV_out = IVs.at(stat);
    )
}

pkmn_error_t pkmn_pokemon_set_IV(
    pkmn_pokemon_handle_t handle,
    const char* stat,
    int IV
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stat, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_IV(stat, IV);
    )
}

pkmn_error_t pkmn_pokemon_get_stat_names(
    pkmn_pokemon_handle_t handle,
    pkmn_string_list_t* stat_names_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stat_names_out, handle);

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
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stat_name, handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stat_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        const std::map<std::string, int>& stats = handle->cpp->get_stats();
        if(stats.count(stat_name) == 0) {
            throw std::invalid_argument("Invalid stat.");
        }

        *stat_out = stats.at(stat_name);
    )
}

pkmn_error_t pkmn_pokemon_get_icon_filepath(
    pkmn_pokemon_handle_t handle,
    char* icon_filepath_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(icon_filepath_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_icon_filepath(),
                    icon_filepath_out,
                    buffer_len
               );
    )
}

pkmn_error_t pkmn_pokemon_get_sprite_filepath(
    pkmn_pokemon_handle_t handle,
    char* sprite_filepath_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(sprite_filepath_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_pokemon_handle_t>(
                    handle,
                    handle->cpp->get_sprite_filepath(),
                    sprite_filepath_out,
                    buffer_len
               );
    )
}
