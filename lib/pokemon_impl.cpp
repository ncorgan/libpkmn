/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_impl.hpp"
#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"
#include "pokemon_gbaimpl.hpp"

#include "misc_common.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include <stdexcept>

namespace pkmn {

    const std::string pokemon::LIBPKMN_OT_NAME = "LibPKMN";

    pokemon::sptr pokemon::make(
        const std::string &species,
        const std::string &game,
        const std::string &form,
        int level
    ) {
        int game_id = pkmn::database::game_name_to_id(game);

        int pokemon_index = pkmn::database::pokemon_name_to_index(
                                species,
                                form,
                                game_id
                            );

        switch(pkmn::database::game_id_to_generation(game_id)) {
            case 1:
                return pkmn::make_shared<pokemon_gen1impl>(
                           pokemon_index,
                           game_id,
                           level
                       );

            case 2:
                return pkmn::make_shared<pokemon_gen2impl>(
                           pokemon_index,
                           game_id,
                           level
                       );

            case 3:
                if(pkmn::database::game_is_gamecube(game_id)) {
                    throw std::runtime_error("Currently unimplemented.");
                } else {
                    return pkmn::make_shared<pokemon_gbaimpl>(
                               pokemon_index,
                               game_id,
                               level
                           );
                }

            case 4:
            case 5:
            case 6:
                throw std::runtime_error("Currently unimplemented.");

            default:
                throw std::runtime_error("Invalid game.");
        }
    }

    pokemon_impl::pokemon_impl(
        int pokemon_index,
        int game_id
    ): pokemon(),
       _database_entry(pkmn::database::pokemon_entry(pokemon_index, game_id)),
       _held_item(pkmn::database::item_entry(0, game_id))
    {}

    std::string pokemon_impl::get_species() {
        return _database_entry.get_name();
    }

    std::string pokemon_impl::get_form() {
        return _database_entry.get_form();
    }

    std::string pokemon_impl::get_game() {
        return _database_entry.get_game();
    }

    const pkmn::database::pokemon_entry& pokemon_impl::get_database_entry() {
        return _database_entry;
    }

    const pkmn::database::item_entry& pokemon_impl::get_held_item() {
        _update_held_item();
        return _held_item;
    }

    const std::map<std::string, bool>& pokemon_impl::get_markings() {
        _update_markings_map();
        return _markings;
    }

    const std::map<std::string, bool>& pokemon_impl::get_ribbons() {
        _update_ribbons_map();
        return _ribbons;
    }

    const pkmn::move_slots_t& pokemon_impl::get_moves() {
        _update_moves(-1);
        return _moves;
    }

    const std::map<std::string, int>& pokemon_impl::get_EVs() {
        _update_EV_map();
        return _EVs;
    }

    const std::map<std::string, int>& pokemon_impl::get_IVs() {
        _update_IV_map();
        return _IVs;
    }

    const std::map<std::string, int>& pokemon_impl::get_stats() {
        _update_stat_map();
        return _stats;
    }

    void* pokemon_impl::get_native_pc_data() {
        return _native_pc;
    }

    void* pokemon_impl::get_native_party_data() {
        return _native_party;
    }

}
