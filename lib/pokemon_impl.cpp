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
#include "io/pkm.hpp"

#include <pkmn/exception.hpp>

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

        pkmn::database::pokemon_entry database_entry(
                                          species,
                                          game,
                                          form
                                      );

        switch(pkmn::database::game_id_to_generation(game_id)) {
            case 1:
                return pkmn::make_shared<pokemon_gen1impl>(
                           std::move(database_entry),
                           level
                       );

            case 2:
                return pkmn::make_shared<pokemon_gen2impl>(
                           std::move(database_entry),
                           level
                       );

            case 3:
                if(game_is_gamecube(game_id)) {
                    throw pkmn::unimplemented_error();
                } else {
                    return pkmn::make_shared<pokemon_gbaimpl>(
                               std::move(database_entry),
                               level
                           );
                }

            case 4:
            case 5:
                return pkmn::make_shared<pokemon_ndsimpl>(
                           std::move(database_entry),
                           level
                       );

            case 6:
                throw pkmn::unimplemented_error();

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
            return pkmn::io::load_pkm(filepath);
        } else if(extension == ".pk6") {
            throw pkmn::unimplemented_error();
        } else {
            std::vector<uint8_t> buffer(size_t(fs::file_size(filepath)));
            PKMN_UNUSED(int game_id) = 0;

            if(pkmn::io::vector_is_valid_3gpkm(buffer, &game_id)) {
                return pkmn::io::load_3gpkm(buffer);
            } else if(pkmn::io::vector_is_valid_pkm(buffer, &game_id)) {
                return pkmn::io::load_pkm(buffer);
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
       _held_item(pkmn::database::item_entry(0, game_id)),
       _generation(pkmn::database::game_id_to_generation(game_id))
    {}

    pokemon_impl::pokemon_impl(
        pkmn::database::pokemon_entry&& database_entry
    ): pokemon(),
       _database_entry(database_entry),
       _held_item(pkmn::database::item_entry(0, database_entry.get_game_id())),
       _generation(pkmn::database::game_id_to_generation(database_entry.get_game_id()))
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
        if(_generation == 1) {
            throw pkmn::feature_not_in_game_error("Held items", "Generation I");
        }

        return _held_item;
    }

    const std::map<std::string, bool>& pokemon_impl::get_markings() {
        if(_generation < 3) {
            throw pkmn::feature_not_in_game_error("Markings", "Generation I-II");
        }

        return _markings;
    }

    const std::map<std::string, bool>& pokemon_impl::get_ribbons() {
        if(_generation < 3) {
            throw pkmn::feature_not_in_game_error("Ribbons", "Generation I-II");
        }

        return _ribbons;
    }

    const std::map<std::string, int>& pokemon_impl::get_contest_stats() {
        if(_generation < 3) {
            throw pkmn::feature_not_in_game_error("Contests", "Generation I-II");
        }

        return _contest_stats;
    }

    const pkmn::move_slots_t& pokemon_impl::get_moves() {
        return _moves;
    }

    const std::map<std::string, int>& pokemon_impl::get_EVs() {
        return _EVs;
    }

    const std::map<std::string, int>& pokemon_impl::get_IVs() {
        return _IVs;
    }

    const std::map<std::string, int>& pokemon_impl::get_stats() {
        return _stats;
    }

    void* pokemon_impl::get_native_pc_data() {
        pokemon_scoped_lock lock(this);

        return _native_pc;
    }

    void* pokemon_impl::get_native_party_data() {
        pokemon_scoped_lock lock(this);

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

        pokemon_scoped_lock lock(this);

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
