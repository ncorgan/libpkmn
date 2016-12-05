/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_gen2impl.hpp"
#include "database/index_to_string.hpp"

#include "pksav/party_data.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen2/time_of_day.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <boost/format.hpp>

#include <cstring>
#include <ctime>
#include <random>
#include <stdexcept>

#define GEN2_PC_RCAST    reinterpret_cast<pksav_gen2_pc_pokemon_t*>(_native_pc)
#define GEN2_PARTY_RCAST reinterpret_cast<pksav_gen2_pokemon_party_data_t*>(_native_party)

namespace pkmn {

    pokemon_gen2impl::pokemon_gen2impl(
        int pokemon_index,
        int game_id,
        int level
    ): pokemon_impl(pokemon_index, game_id)
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gen2_pc_pokemon_t);
        std::memset(_native_pc, 0, sizeof(pksav_gen2_pc_pokemon_t));
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen2_pokemon_party_data_t);
        std::memset(_native_party, 0, sizeof(pksav_gen2_pokemon_party_data_t));
        _our_party_mem = true;

        // Set internal members
        GEN2_PC_RCAST->species = uint8_t(pokemon_index);

        GEN2_PC_RCAST->ot_id = pksav_bigendian16(uint16_t(LIBPKMN_OT_ID & 0xFFFF));

        // TODO: Use PKSav PRNG after refactor merged in
        time_t now = 0;
        std::srand((unsigned int)std::time(&now));
        GEN2_PC_RCAST->ev_hp   = uint16_t(std::rand());
        GEN2_PC_RCAST->ev_atk  = uint16_t(std::rand());
        GEN2_PC_RCAST->ev_def  = uint16_t(std::rand());
        GEN2_PC_RCAST->ev_spd  = uint16_t(std::rand());
        GEN2_PC_RCAST->ev_spcl = uint16_t(std::rand());
        GEN2_PC_RCAST->iv_data = uint16_t(std::rand());

        GEN2_PC_RCAST->friendship = uint8_t(_database_entry.get_base_happiness());

        // TODO: rest of caught_data
        pksav_gen2_set_caught_data_time_field(
            &now,
            &GEN2_PC_RCAST->caught_data
        );

        // Populate abstractions
        _update_held_item();
        _update_EV_map();
        _update_IV_map();
        set_level(level);
    }

    pokemon_gen2impl::pokemon_gen2impl(
        pksav_gen2_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(pc->species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_gen2_pokemon_party_data_t);
        pksav::gen2_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_gen2_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gen2_pokemon_party_data_t*>(_native_party)
        );
        _our_party_mem = true;

        // Populate abstractions
        _update_held_item();
        _update_EV_map();
        _update_IV_map();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen2impl::pokemon_gen2impl(
        pksav_gen2_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(party->pc.species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Populate abstractions
        _update_held_item();
        _update_EV_map();
        _update_IV_map();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen2impl::~pokemon_gen2impl() {
        if(_our_pc_mem) {
            delete GEN2_PC_RCAST;
        }
        if(_our_party_mem) {
            delete GEN2_PARTY_RCAST;
        }
    }

    std::string pokemon_gen2impl::get_nickname() {
        return _nickname;
    }

    void pokemon_gen2impl::set_nickname(
        const std::string &nickname
    ) {
        if(nickname.size() < 1 or nickname.size() > 10) {
            throw std::invalid_argument(
                      "The nickname length must be 0-10."
                  );
        }

        _nickname = nickname;
    }

    std::string pokemon_gen2impl::get_trainer_name() {
        return _trainer_name;
    }

    void pokemon_gen2impl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() < 1 or trainer_name.size() > 7) {
            throw std::invalid_argument(
                      "The trainer name name length must be 0-7."
                  );
        }

        _trainer_name = trainer_name;
    }

    uint16_t pokemon_gen2impl::get_trainer_public_id() {
        return pksav_bigendian16(GEN2_PC_RCAST->ot_id);
    }

    uint16_t pokemon_gen2impl::get_trainer_secret_id() {
        throw std::runtime_error("Generation II has no secret trainer ID.");
    }

    uint32_t pokemon_gen2impl::get_trainer_id() {
        return uint32_t(pksav_bigendian16(GEN2_PC_RCAST->ot_id));
    }

    void pokemon_gen2impl::set_trainer_public_id(
        uint16_t public_id
    ) {
        GEN2_PC_RCAST->ot_id = pksav_bigendian16(public_id);
    }

    void pokemon_gen2impl::set_trainer_secret_id(
        PKMN_UNUSED(uint16_t secret_id)
    ) {
        throw std::runtime_error("Generation II has no secret trainer ID.");
    }

    void pokemon_gen2impl::set_trainer_id(
        uint32_t id
    ) {
        if(id > 65535) {
            throw std::runtime_error("id: valid values 0-65535");
        }

        GEN2_PC_RCAST->ot_id = pksav_bigendian16(uint16_t(id));
    }

    std::string pokemon_gen2impl::get_trainer_gender() {
        return (GEN2_PC_RCAST->caught_data & PKSAV_GEN2_OT_GENDER_MASK) ? "Female"
                                                                        : "Male";
    }

    void pokemon_gen2impl::set_trainer_gender(
        const std::string &gender
    ) {
        if(gender == "Male") {
            GEN2_PC_RCAST->caught_data &= ~PKSAV_GEN2_OT_GENDER_MASK;
        } else if(gender == "Female") {
            GEN2_PC_RCAST->caught_data |= PKSAV_GEN2_OT_GENDER_MASK;
        } else {
            throw std::invalid_argument("gender: valid values \"Male\", \"Female\"");
        }
    }

    std::string pokemon_gen2impl::get_ability() {
        throw std::runtime_error("There are no abilities in Generation II.");
    }

    void pokemon_gen2impl::set_ability(
        PKMN_UNUSED(const std::string &ability)
    ) {
        throw std::runtime_error("There are no abilities in Generation II.");
    }

    std::string pokemon_gen2impl::get_ball() {
        throw std::runtime_error("A Pokémon's ball is not recorded in Generation II.");
    }

    void pokemon_gen2impl::set_ball(
        PKMN_UNUSED(const std::string &ball)
    ) {
        throw std::runtime_error("A Pokémon's ball is not recorded in Generation II.");
    }

    std::string pokemon_gen2impl::get_location_caught() {
        return pkmn::database::location_index_to_name(
                   (GEN2_PC_RCAST->caught_data & PKSAV_GEN2_LOCATION_MASK),
                   _database_entry.get_game_id()
               );
    }

    void pokemon_gen2impl::set_location_caught(
        const std::string &location
    ) {
        uint16_t location_index = uint16_t(pkmn::database::location_name_to_index(
                                               location,
                                               _database_entry.get_game_id()
                                           ));
        GEN2_PC_RCAST->caught_data &= ~PKSAV_GEN2_LOCATION_MASK;
        GEN2_PC_RCAST->caught_data |= location_index;
    }

    std::string pokemon_gen2impl::get_original_game() {
        throw std::runtime_error("Original game is not recorded in Generation II.");
    }

    void pokemon_gen2impl::set_original_game(
        PKMN_UNUSED(const std::string &game)
    ) {
        throw std::runtime_error("Original game is not recorded in Generation II.");
    }

    uint32_t pokemon_gen2impl::get_personality() {
        throw std::runtime_error("There is no personality in Generation II.");
    }

    void pokemon_gen2impl::set_personality(
        PKMN_UNUSED(uint32_t personality)
    ) {
        throw std::runtime_error("There is no personality in Generation II.");
    }

    int pokemon_gen2impl::get_experience() {
        uint32_t ret = 0;
        pksav_from_base256(
            GEN2_PC_RCAST->exp,
            3,
            &ret
        );

        return int(ret);
    }

    void pokemon_gen2impl::set_experience(
        int experience
    ) {
        int max_experience = _database_entry.get_experience_at_level(100);

        if(experience < 0 or experience > max_experience) {
            throw std::out_of_range(
                      str(boost::format(
                              "experience: valid range 0-%d"
                          ) % max_experience)
                  );
        }

        pksav_to_base256(
            experience,
            GEN2_PC_RCAST->exp,
            3
        );

        GEN2_PC_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));

        _calculate_stats();
        _update_stat_map();
    }

    int pokemon_gen2impl::get_level() {
        return int(GEN2_PC_RCAST->level);
    }

    void pokemon_gen2impl::set_level(
        int level
    ) {
        if(level < 2 or level > 100) {
            throw std::out_of_range(
                      "level: valid range 2-100"
                  );
        }

        GEN2_PC_RCAST->level = uint8_t(level);

        pksav_to_base256(
            uint32_t(_database_entry.get_experience_at_level(level)),
            GEN2_PC_RCAST->exp,
            3
        );

        _calculate_stats();
        _update_stat_map();
    }

    void pokemon_gen2impl::set_marking(
        PKMN_UNUSED(const std::string &marking),
        PKMN_UNUSED(bool value)
    ) {
        throw std::runtime_error("There are no markings in Generation II.");
    }

    void pokemon_gen2impl::set_ribbon(
        PKMN_UNUSED(const std::string &ribbon),
        PKMN_UNUSED(bool value)
    ) {
        throw std::runtime_error("There are no ribbons in Generation II.");
    }

    void pokemon_gen2impl::set_move(
        const std::string &move,
        int index
    ) {
        if(index < 0 or index > 3) {
            throw std::out_of_range("index: valid values 0-3");
        }

        // This will throw an error if the move is invalid
        _moves[index].move = pkmn::database::move_entry(
                                 move,
                                 get_game()
                             );
        _moves[index].pp = _moves[index].move.get_pp(0);

        GEN2_PC_RCAST->moves[index] = uint8_t(_moves[index].move.get_move_id());
        GEN2_PC_RCAST->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gen2impl::_calculate_stats() {
        pksav::gen2_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_gen2_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gen2_pokemon_party_data_t*>(_native_party)
        );
    }

    void pokemon_gen2impl::_update_moves(
        int index
    ) {
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        GEN2_PC_RCAST->moves[index],
                        _database_entry.get_game_id()
                    ),
                    (GEN2_PC_RCAST->move_pps[index] & PKSAV_GEN2_MOVE_PP_MASK)
                );
                break;

            default:
                for(size_t i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gen2impl::_update_held_item() {
        if(int(GEN2_PC_RCAST->held_item) != _held_item.get_item_index()) {
            _held_item = pkmn::database::item_entry(
                             GEN2_PC_RCAST->held_item,
                             _database_entry.get_game_id()
                         );
        }
    }

    void pokemon_gen2impl::_update_markings_map() {
        throw std::runtime_error("There are no markings in Generation II.");
    }

    void pokemon_gen2impl::_update_ribbons_map() {
        throw std::runtime_error("There are no ribbons in Generation II.");
    }

    void pokemon_gen2impl::_update_EV_map() {
        _EVs["HP"]      = int(pksav_bigendian16(GEN2_PC_RCAST->ev_hp));
        _EVs["Attack"]  = int(pksav_bigendian16(GEN2_PC_RCAST->ev_atk));
        _EVs["Defense"] = int(pksav_bigendian16(GEN2_PC_RCAST->ev_def));
        _EVs["Speed"]   = int(pksav_bigendian16(GEN2_PC_RCAST->ev_spd));
        _EVs["Special"] = int(pksav_bigendian16(GEN2_PC_RCAST->ev_spcl));
    }

    void pokemon_gen2impl::_update_IV_map() {
        uint8_t IV = 0;

        pksav_get_gb_IV(
            &GEN2_PC_RCAST->iv_data,
            PKSAV_STAT_HP,
            &IV
        );
        _IVs["HP"] = int(IV);

        pksav_get_gb_IV(
            &GEN2_PC_RCAST->iv_data,
            PKSAV_STAT_ATTACK,
            &IV
        );
        _IVs["Attack"] = int(IV);

        pksav_get_gb_IV(
            &GEN2_PC_RCAST->iv_data,
            PKSAV_STAT_DEFENSE,
            &IV
        );
        _IVs["Defense"] = int(IV);

        pksav_get_gb_IV(
            &GEN2_PC_RCAST->iv_data,
            PKSAV_STAT_SPEED,
            &IV
        );
        _IVs["Speed"] = int(IV);

        pksav_get_gb_IV(
            &GEN2_PC_RCAST->iv_data,
            PKSAV_STAT_SPECIAL,
            &IV
        );
        _IVs["Special"] = int(IV);
    }

    void pokemon_gen2impl::_update_stat_map() {
        _stats["HP"]              = int(pksav_bigendian16(GEN2_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_bigendian16(GEN2_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_bigendian16(GEN2_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_bigendian16(GEN2_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_bigendian16(GEN2_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_bigendian16(GEN2_PARTY_RCAST->spdef));
    }
}
