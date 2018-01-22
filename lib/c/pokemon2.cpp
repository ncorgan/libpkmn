/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_enum_maps.hpp"
#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/pokemon2.h>

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

    // TODO: store and check generation
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

        std::string cpp_gender = internal_ptr->cpp->get_gender();
        BOOST_ASSERT(pkmn::c::GENDER_BIMAP.left.count(cpp_gender) > 0);
        pokemon_info.gender = pkmn::c::GENDER_BIMAP.left.at(cpp_gender);

        pokemon_info.is_egg = internal_ptr->cpp->is_egg();

        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_held_item(),
            &pokemon_info.held_item
        );
        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_ability(),
            &pokemon_info.ability
        );
        pkmn::c::pokemon_entry_cpp_to_c(
            internal_ptr->cpp->get_database_entry(),
            &pokemon_info.database_entry
        );
        pokemon_info.personality = internal_ptr->cpp->get_personality();

        *pokemon_info_ptr = std::move(pokemon_info);
    )
}

// Original trainer info

pkmn_error_t pkmn_pokemon2_get_original_trainer_info(
    pkmn_pokemon2_t* pokemon_ptr,
    pkmn_trainer_info2_t* trainer_info_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_info_ptr, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        // Put the values in a separate struct first so there are
        // no side effects if there's an error.
        pkmn_trainer_info2_t trainer_info;

        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_original_trainer_name(),
            &trainer_info.name
        );
        trainer_info.id.id = internal_ptr->cpp->get_original_trainer_id();

        // TODO: this may fail for Generation I at some point
        std::string cpp_gender = internal_ptr->cpp->get_original_trainer_gender();
        BOOST_ASSERT(pkmn::c::GENDER_BIMAP.left.count(cpp_gender) > 0);
        trainer_info.gender = pkmn::c::GENDER_BIMAP.left.at(cpp_gender);

        *trainer_info_ptr = std::move(trainer_info);
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
