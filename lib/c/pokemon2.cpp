/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_enum_maps.hpp"
#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "private_exports.hpp"

#include <pkmn-c/pokemon2.h>

#include <pkmn/exception.hpp>

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <cstdio>

#define INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_pokemon_internal_t*>(ptr))

// The caller is expected to be exception-safe.
void init_pokemon(
    pkmn::pokemon::sptr cpp_pokemon,
    pkmn_pokemon2_t* pokemon_ptr
)
{
    BOOST_ASSERT(pokemon_ptr);
    BOOST_ASSERT(cpp_pokemon.get());

    pokemon_ptr->_internal = new pkmn_pokemon_internal_t;
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    internal_ptr->cpp = cpp_pokemon;
    internal_ptr->last_error = "None";
    internal_ptr->generation = pkmn::priv::game_name_to_generation(
                                   internal_ptr->cpp->get_game()
                               );

    pkmn::c::string_cpp_to_c_alloc(
        internal_ptr->cpp->get_species(),
        &pokemon_ptr->species
    );
    pkmn::c::string_cpp_to_c_alloc(
        internal_ptr->cpp->get_game(),
        &pokemon_ptr->game
    );
}

pkmn_trainer_id2_t pkmn_pokemon2_default_trainer_id()
{
    return {pkmn::pokemon::DEFAULT_TRAINER_ID};
}

const char* pkmn_pokemon2_default_trainer_name()
{
    static const std::string DEFAULT_TRAINER_NAME = pkmn::pokemon::DEFAULT_TRAINER_NAME;
    return DEFAULT_TRAINER_NAME.c_str();
}

pkmn_error_t pkmn_pokemon_init(
    const char* species,
    const char* game,
    const char* form,
    int level,
    pkmn_pokemon2_t* pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(species);
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(form);
    PKMN_CHECK_NULL_PARAM(pokemon_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::make(
                                      species,
                                      game,
                                      form,
                                      level
                                  );

        init_pokemon(
            cpp,
            pokemon_out
        );
    )
}

pkmn_error_t pkmn_pokemon2_init_from_file(
    const char* filepath,
    pkmn_pokemon2_t* pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(filepath);
    PKMN_CHECK_NULL_PARAM(pokemon_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::from_file(filepath);

        init_pokemon(
            cpp,
            pokemon_out
        );
    )
}

pkmn_error_t pkmn_pokemon2_free(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);

    pokemon_ptr->species = nullptr;
    pokemon_ptr->game = nullptr;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_internal_t**>(&pokemon_ptr->_internal)
        );
    )
}

const char* pkmn_pokemon2_strerror(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    if(!pokemon_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
        if(!internal_ptr)
        {
            return nullptr;
        }

        boost::mutex::scoped_lock lock(internal_ptr->error_mutex);
        return internal_ptr->last_error.c_str();
    }
    catch(...)
    {
        return nullptr;
    }
}

pkmn_error_t pkmn_pokemon2_to_game(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* game,
    pkmn_pokemon2_t* new_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(new_pokemon_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::pokemon::sptr new_pokemon_cpp = internal_ptr->cpp->to_game(game);

        init_pokemon(
            new_pokemon_cpp,
            new_pokemon_out
        );
    )
}

pkmn_error_t pkmn_pokemon2_export_to_file(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* filepath
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(filepath, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->export_to_file(filepath);
    )
}

// Pokémon info

pkmn_error_t pkmn_pokemon2_get_pokemon_info(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_pokemon_info_t* pokemon_info_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_info_ptr, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        // Put the values in a separate struct first so there are
        // no side effects if there's an error.
        pkmn_pokemon_info_t pokemon_info;

        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_form(),
            &pokemon_info.form
        );
        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_nickname(),
            &pokemon_info.nickname
        );
        pkmn::c::pokemon_entry_cpp_to_c(
            internal_ptr->cpp->get_database_entry(),
            &pokemon_info.database_entry
        );

        if(internal_ptr->generation >= 2)
        {
            std::string cpp_gender = internal_ptr->cpp->get_gender();
            BOOST_ASSERT(pkmn::c::GENDER_BIMAP.left.count(cpp_gender) > 0);
            pokemon_info.gender = pkmn::c::GENDER_BIMAP.left.at(cpp_gender);

            pokemon_info.is_egg = internal_ptr->cpp->is_egg();

            pkmn::c::string_cpp_to_c_alloc(
                internal_ptr->cpp->get_held_item(),
                &pokemon_info.held_item
            );
        }
        else
        {
            pokemon_info.gender = PKMN_MALE;
            pokemon_info.is_egg = false;

            pkmn::c::string_cpp_to_c_alloc(
                "None",
                &pokemon_info.held_item
            );
        }

        if(internal_ptr->generation >= 3)
        {
            pkmn::c::string_cpp_to_c_alloc(
                internal_ptr->cpp->get_ability(),
                &pokemon_info.ability
            );
            pokemon_info.personality = internal_ptr->cpp->get_personality();
        }
        else
        {
            pkmn::c::string_cpp_to_c_alloc(
                "None",
                &pokemon_info.ability
            );
            pokemon_info.personality = 0U;
        }

        *pokemon_info_ptr = std::move(pokemon_info);
    )
}

pkmn_error_t pkmn_pokemon2_set_form(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* form
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(form, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_form(form);
    )
}

pkmn_error_t pkmn_pokemon2_set_nickname(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* nickname
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(nickname, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_nickname(nickname);
    )
}

pkmn_error_t pkmn_pokemon2_set_gender(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_gender_t gender
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Gender",
            gender,
            pkmn::c::GENDER_BIMAP.right
        );

        internal_ptr->cpp->set_gender(
            pkmn::c::GENDER_BIMAP.right.at(gender)
        );
    )
}

pkmn_error_t pkmn_pokemon2_set_is_egg(
    pkmn_pokemon2_t* pokemon_ptr,
    bool is_egg
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_is_egg(is_egg);
    )
}

pkmn_error_t pkmn_pokemon2_set_held_item(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* held_item
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(held_item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_held_item(held_item);
    )
}

pkmn_error_t pkmn_pokemon2_set_ability(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* ability
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ability, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_ability(ability);
    )
}

pkmn_error_t pkmn_pokemon2_set_personality(
    pkmn_pokemon2_t* pokemon_ptr,
    uint32_t personality
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_personality(personality);
    )
}

// Pokémon origin info

pkmn_error_t pkmn_pokemon2_get_pokemon_origin_info(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_pokemon_origin_info_t* pokemon_origin_info_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_origin_info_ptr, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        // Put the values in a separate struct first so there are
        // no side effects if there's an error.
        pkmn_pokemon_origin_info_t pokemon_origin_info;

        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_original_trainer_name(),
            &pokemon_origin_info.trainer_info.name
        );
        pokemon_origin_info.trainer_info.id.id = internal_ptr->cpp->get_original_trainer_id();

        if(internal_ptr->generation >= 2)
        {
            std::string cpp_gender = internal_ptr->cpp->get_original_trainer_gender();
            BOOST_ASSERT(pkmn::c::GENDER_BIMAP.left.count(cpp_gender) > 0);
            pokemon_origin_info.trainer_info.gender = pkmn::c::GENDER_BIMAP.left.at(cpp_gender);

            pkmn::c::string_cpp_to_c_alloc(
                internal_ptr->cpp->get_location_met(false),
                &pokemon_origin_info.location_met
            );

            pokemon_origin_info.level_met = internal_ptr->cpp->get_level_met();
        }
        else
        {
            pokemon_origin_info.trainer_info.gender = PKMN_MALE;

            pkmn::c::string_cpp_to_c_alloc(
                "None",
                &pokemon_origin_info.location_met
            );

            pokemon_origin_info.level_met = 0;
        }

        if(internal_ptr->generation >= 3)
        {
            pkmn::c::string_cpp_to_c_alloc(
                internal_ptr->cpp->get_ball(),
                &pokemon_origin_info.ball
            );
        }
        else
        {
            pkmn::c::string_cpp_to_c_alloc(
                "None",
                &pokemon_origin_info.ball
            );
        }

        if(internal_ptr->generation >= 4)
        {
            pkmn::c::string_cpp_to_c_alloc(
                internal_ptr->cpp->get_location_met(true),
                &pokemon_origin_info.location_met_as_egg
            );
        }
        else
        {
            pkmn::c::string_cpp_to_c_alloc(
                "None",
                &pokemon_origin_info.location_met_as_egg
            );
        }
    )
}

pkmn_error_t pkmn_pokemon2_set_original_trainer_name(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* original_trainer_name
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(original_trainer_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_name(original_trainer_name);
    )
}

pkmn_error_t pkmn_pokemon2_set_original_trainer_id(
    pkmn_pokemon2_t* pokemon_ptr,
    uint32_t original_trainer_id
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_id(original_trainer_id);
    )
}

pkmn_error_t pkmn_pokemon2_set_original_trainer_public_id(
    pkmn_pokemon2_t* pokemon_ptr,
    uint16_t original_trainer_public_id
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_public_id(
            original_trainer_public_id
        );
    )
}

pkmn_error_t pkmn_pokemon2_set_original_trainer_secret_id(
    pkmn_pokemon2_t* pokemon_ptr,
    uint16_t original_trainer_secret_id
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_secret_id(
            original_trainer_secret_id
        );
    )
}

pkmn_error_t pkmn_pokemon2_set_original_game(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* game
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_game(game);
    )
}

pkmn_error_t pkmn_pokemon2_set_ball(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* ball
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ball, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_ball(ball);
    )
}

pkmn_error_t pkmn_pokemon2_set_location_met(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* location_met,
    bool as_egg
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(location_met, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_location_met(location_met, as_egg);
    )
}

pkmn_error_t pkmn_pokemon2_set_level_met(
    pkmn_pokemon2_t* pokemon_ptr,
    int level_met
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_level_met(level_met);
    )
}

// Moves

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_moves(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_move_slots_t* move_slots_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(move_slots_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::move_slots_cpp_to_c(
            internal_ptr->cpp->get_moves(),
            move_slots_out
        );
    )
}

pkmn_error_t pkmn_pokemon2_set_move(
    pkmn_pokemon2_t* pokemon_ptr,
    size_t index,
    const char* move
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(move, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_move(move, int(index));
    )
}

pkmn_error_t pkmn_pokemon2_set_move_pp(
    pkmn_pokemon2_t* pokemon_ptr,
    size_t index,
    int pp
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_move_pp(int(index), pp);
    )
}

// Stats

// The caller is expected to be exception-safe.
template <typename enum_type>
static void copy_stat_map_to_buffer(
    const std::map<std::string, int>& stat_map,
    const boost::bimap<std::string, enum_type>& stat_enum_bimap,
    int* stats_buffer_out,
    size_t stat_buffer_size,
    size_t actual_num_stats,
    size_t* actual_num_stats_out
)
{
    BOOST_ASSERT(stats_buffer_out);

    std::memset(
        stats_buffer_out,
        0,
        stat_buffer_size * sizeof(int)
    );

    for(size_t stat = 0; stat <= actual_num_stats; ++stat)
    {
        enum_type stat_enum = enum_type(stat);
        BOOST_ASSERT(stat_enum_bimap.right.count(stat_enum) > 0);

        stats_buffer_out[stat] = stat_map.at(stat_enum_bimap.right.at(stat_enum));
    }

    // Optional parameter
    if(actual_num_stats_out)
    {
        *actual_num_stats_out = actual_num_stats;
    }
}

pkmn_error_t pkmn_pokemon2_get_EVs(
    pkmn_pokemon2_t* pokemon_ptr,
    int* EVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_EVs_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(EVs_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_stat_map_to_buffer(
            internal_ptr->cpp->get_EVs(),
            pkmn::c::STAT_BIMAP,
            EVs_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            actual_num_EVs_out
        );
    )
}

pkmn_error_t pkmn_pokemon2_set_EV(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_stat_t stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Stat",
            stat,
            pkmn::c::STAT_BIMAP.right
        );

        internal_ptr->cpp->set_EV(
            pkmn::c::STAT_BIMAP.right.at(stat),
            value
        );
    )
}

pkmn_error_t pkmn_pokemon2_get_IVs(
    pkmn_pokemon2_t* pokemon_ptr,
    int* IVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_IVs_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(IVs_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_stat_map_to_buffer(
            internal_ptr->cpp->get_IVs(),
            pkmn::c::STAT_BIMAP,
            IVs_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            actual_num_IVs_out
        );
    )
}

pkmn_error_t pkmn_pokemon2_set_IV(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_stat_t stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Stat",
            stat,
            pkmn::c::STAT_BIMAP.right
        );

        internal_ptr->cpp->set_IV(
            pkmn::c::STAT_BIMAP.right.at(stat),
            value
        );
    )
}

pkmn_error_t pkmn_pokemon2_get_stats(
    pkmn_pokemon2_t* pokemon_ptr,
    int* stats_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stats_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_stat_map_to_buffer(
            internal_ptr->cpp->get_stats(),
            pkmn::c::STAT_BIMAP,
            stats_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            actual_num_stats_out
        );
    )
}

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_contest_stats(
    pkmn_pokemon2_t* pokemon_ptr,
    int* contest_stats_buffer_out,
    size_t contest_stat_buffer_size,
    size_t* actual_num_contest_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(contest_stats_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_stat_map_to_buffer(
            internal_ptr->cpp->get_contest_stats(),
            pkmn::c::CONTEST_STAT_BIMAP,
            contest_stats_buffer_out,
            contest_stat_buffer_size,
            PKMN_NUM_CONTEST_STATS,
            actual_num_contest_stats_out
        );
    )
}

pkmn_error_t pkmn_pokemon2_set_contest_stat(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_contest_stat_t contest_stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Contest stat",
            contest_stat,
            pkmn::c::CONTEST_STAT_BIMAP.right
        );

        internal_ptr->cpp->set_contest_stat(
            pkmn::c::CONTEST_STAT_BIMAP.right.at(contest_stat),
            value
        );
    )
}

pkmn_error_t pkmn_pokemon2_get_numeric_attribute(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* attribute_name,
    int* value_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *value_out = internal_ptr->cpp->get_numeric_attribute(attribute_name);
    )
}

pkmn_error_t pkmn_pokemon2_set_numeric_attribute(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* attribute_name,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_numeric_attribute(attribute_name, value);
    )
}

pkmn_error_t pkmn_pokemon2_get_string_attribute(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* attribute_name,
    char* value_out,
    size_t value_buffer_len,
    size_t* actual_value_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_string_attribute(attribute_name),
            value_out,
            value_buffer_len,
            actual_value_len_out
        );
    )
}

pkmn_error_t pkmn_pokemon2_set_string_attribute(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* attribute_name,
    const char* value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_string_attribute(attribute_name, value);
    )
}

pkmn_error_t pkmn_pokemon2_get_attribute_names(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_attribute_names_t* attribute_names_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_names_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_list_cpp_to_c(
            internal_ptr->cpp->get_numeric_attribute_names(),
            &attribute_names_out->numeric_attribute_names
        );
        pkmn::c::string_list_cpp_to_c(
            internal_ptr->cpp->get_string_attribute_names(),
            &attribute_names_out->string_attribute_names
        );
    )
}

// Filepaths

PKMN_C_API pkmn_error_t pkmn_pokemon2_get_filepaths(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_pokemon_filepaths_t* filepaths_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(filepaths_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_icon_filepath(),
            &filepaths_out->icon_filepath
        );
        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_sprite_filepath(),
            &filepaths_out->sprite_filepath
        );
    )
}
