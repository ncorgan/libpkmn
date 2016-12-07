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
#include "pokemon_ndsimpl.hpp"

#include "misc_common.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "io/3gpkm.hpp"

#include <pksav/common/markings.h>

#include <boost/filesystem.hpp>

#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn {

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
                if(game_is_gamecube(game_id)) {
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
                return pkmn::make_shared<pokemon_ndsimpl>(
                           pokemon_index,
                           game_id,
                           level
                       );

            case 6:
                throw std::runtime_error("Currently unimplemented.");

            default:
                throw std::runtime_error("Invalid game.");
        }
    }

    pokemon::sptr pokemon::from_file(
        const std::string &filepath
    ) {
        // If an extension is given, assume a type. If not, try each.
        std::string extension = fs::extension(filepath);
        if(extension == ".3gpkm") {
            return pkmn::io::load_3gpkm(filepath);
        } else if(extension == ".pkm") {
            throw std::runtime_error("Currently unimplemented.");
        } else if(extension == ".pk6") {
            throw std::runtime_error("Currently unimplemented.");
        } else {
            std::vector<uint8_t> buffer(fs::file_size(filepath));
            PKMN_UNUSED(int game_id) = 0;

            if(pkmn::io::vector_is_valid_3gpkm(buffer, &game_id)) {
                return pkmn::io::load_3gpkm(buffer);
            } else {
                throw std::runtime_error("Invalid file.");
            }
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

    // Shared setters

    #define SET_MARKING(str,mask) \
        if(marking == str) { \
            if(value) { \
                *native |= mask; \
            } else { \
                *native &= ~mask; \
            } \
        }

    void pokemon_impl::_set_marking(
        const std::string &marking,
        bool value,
        uint8_t* native,
        bool gen3
    ) {
        if(_markings.find(marking) == _markings.end()) {
            throw std::invalid_argument("Invalid marking.");
        }

        SET_MARKING("Circle", PKSAV_MARKING_CIRCLE);
        SET_MARKING("Triangle", PKSAV_MARKING_TRIANGLE);
        SET_MARKING("Square", PKSAV_MARKING_SQUARE);
        SET_MARKING("Heart", PKSAV_MARKING_HEART);
        if(not gen3) {
            SET_MARKING("Star", PKSAV_MARKING_STAR);
            SET_MARKING("Diamond", PKSAV_MARKING_DIAMOND);
        }

        _update_markings_map();
    }

}
