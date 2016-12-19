/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "misc_common.hpp"
#include "pokemon_gen1impl.hpp"

#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include "mem/scoped_lock.hpp"

#include <pkmn/exception.hpp>

#include <pksav/common/stats.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <ctime>
#include <random>
#include <stdexcept>
#include <unordered_map>

#define GEN1_PC_RCAST    reinterpret_cast<pksav_gen1_pc_pokemon_t*>(_native_pc)
#define GEN1_PARTY_RCAST reinterpret_cast<pksav_gen1_pokemon_party_data_t*>(_native_party)

static std::unordered_map<std::string, pksav_gen1_type_t> GEN1_TYPES = boost::assign::map_list_of
    ("Normal",   PKSAV_GEN1_TYPE_NORMAL)
    ("Fighting", PKSAV_GEN1_TYPE_FIGHTING)
    ("Flying",   PKSAV_GEN1_TYPE_FLYING)
    ("Poison",   PKSAV_GEN1_TYPE_POISON)
    ("Ground",   PKSAV_GEN1_TYPE_GROUND)
    ("Rock",     PKSAV_GEN1_TYPE_ROCK)
    ("Bug",      PKSAV_GEN1_TYPE_BUG)
    ("Ghost",    PKSAV_GEN1_TYPE_GHOST)
    ("Fire",     PKSAV_GEN1_TYPE_FIRE)
    ("Water",    PKSAV_GEN1_TYPE_WATER)
    ("Grass",    PKSAV_GEN1_TYPE_GRASS)
    ("Electric", PKSAV_GEN1_TYPE_ELECTRIC)
    ("Psychic",  PKSAV_GEN1_TYPE_PSYCHIC)
    ("Ice",      PKSAV_GEN1_TYPE_ICE)
    ("Dragon",   PKSAV_GEN1_TYPE_DRAGON)
;

namespace pkmn {

    pokemon_gen1impl::pokemon_gen1impl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry)),
       _yellow_pikachu_friendship(0)
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gen1_pc_pokemon_t);
        std::memset(_native_pc, 0, sizeof(pksav_gen1_pc_pokemon_t));
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen1_pokemon_party_data_t);
        std::memset(_native_party, 0, sizeof(pksav_gen1_pokemon_party_data_t));
        _our_party_mem = true;

        _nickname = boost::algorithm::to_upper_copy(
                        _database_entry.get_name()
                    );
        _trainer_name = LIBPKMN_OT_NAME;

        // Set internal members
        GEN1_PC_RCAST->species = uint8_t(_database_entry.get_pokemon_index());

        std::pair<std::string, std::string> types = _database_entry.get_types();
        GEN1_PC_RCAST->types[0] = uint8_t(GEN1_TYPES.at(types.first));
        GEN1_PC_RCAST->types[1] = (types.second == "None") ? GEN1_PC_RCAST->types[0]
                                                           : uint8_t(GEN1_TYPES.at(types.second));

        // TODO: catch rate

        GEN1_PC_RCAST->ot_id = pksav_bigendian16(uint16_t(LIBPKMN_OT_ID & 0xFFFF));

        // TODO: Use PKSav PRNG after refactor merged in
        std::srand((unsigned int)std::time(0));
        GEN1_PC_RCAST->ev_hp   = uint16_t(std::rand());
        GEN1_PC_RCAST->ev_atk  = uint16_t(std::rand());
        GEN1_PC_RCAST->ev_def  = uint16_t(std::rand());
        GEN1_PC_RCAST->ev_spd  = uint16_t(std::rand());
        GEN1_PC_RCAST->ev_spcl = uint16_t(std::rand());
        GEN1_PC_RCAST->iv_data = uint16_t(std::rand());

        // Populate abstractions
        _update_EV_map();
        _update_IV_map();
        _update_moves(-1);
        set_level(level);
    }

    pokemon_gen1impl::pokemon_gen1impl(
        pksav_gen1_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(pc->species, game_id),
       _yellow_pikachu_friendship(0)
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_gen1_pokemon_party_data_t);
        pksav::gen1_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_gen1_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gen1_pokemon_party_data_t*>(_native_party)
        );
        _our_party_mem = true;

        // Populate abstractions
        _update_EV_map();
        _update_IV_map();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen1impl::pokemon_gen1impl(
        pksav_gen1_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(party->pc.species, game_id),
       _yellow_pikachu_friendship(0)
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Populate abstractions
        _update_EV_map();
        _update_IV_map();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen1impl::~pokemon_gen1impl() {
        if(_our_pc_mem) {
            delete GEN1_PC_RCAST;
        }
        if(_our_party_mem) {
            delete GEN1_PARTY_RCAST;
        }
    }

    std::string pokemon_gen1impl::get_nickname() {
        return _nickname;
    }

    void pokemon_gen1impl::set_nickname(
        const std::string &nickname
    ) {
        if(nickname.size() < 1 or nickname.size() > 10) {
            throw std::invalid_argument(
                      "The nickname length must be 1-10."
                  );
        }

        _nickname = nickname;
    }

    void pokemon_gen1impl::set_held_item(
        PKMN_UNUSED(const std::string &held_item)
    ) {
        throw pkmn::feature_not_in_game_error("Held items", "Generation I");
    }

    std::string pokemon_gen1impl::get_trainer_name() {
        return _trainer_name;
    }

    void pokemon_gen1impl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() < 1 or trainer_name.size() > 7) {
            throw std::invalid_argument(
                      "The trainer name length must be 1-7."
                  );
        }

        _trainer_name = trainer_name;
    }

    uint16_t pokemon_gen1impl::get_trainer_public_id() {
        pokemon_scoped_lock lock(this);

        return pksav_bigendian16(GEN1_PC_RCAST->ot_id);
    }

    uint16_t pokemon_gen1impl::get_trainer_secret_id() {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation I");
    }

    uint32_t pokemon_gen1impl::get_trainer_id() {
        pokemon_scoped_lock lock(this);

        return uint32_t(pksav_bigendian16(GEN1_PC_RCAST->ot_id));
    }

    void pokemon_gen1impl::set_trainer_public_id(
        uint16_t public_id
    ) {
        GEN1_PC_RCAST->ot_id = pksav_bigendian16(public_id);
    }

    void pokemon_gen1impl::set_trainer_secret_id(
        PKMN_UNUSED(uint16_t secret_id)
    ) {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation I");
    }

    void pokemon_gen1impl::set_trainer_id(
        uint32_t id
    ) {
        if(id > 65535) {
            throw pkmn::range_error("id", 0, 65535);
        }

        pokemon_scoped_lock lock(this);

        GEN1_PC_RCAST->ot_id = pksav_bigendian16(uint16_t(id));
    }

    std::string pokemon_gen1impl::get_trainer_gender() {
        return "Male";
    }

    void pokemon_gen1impl::set_trainer_gender(
        PKMN_UNUSED(const std::string &gender)
    ) {
        throw pkmn::feature_not_in_game_error("All Generation I trainers are male.");
    }

    BOOST_STATIC_CONSTEXPR int YELLOW = 3;
    BOOST_STATIC_CONSTEXPR int PIKACHU = 25;

    int pokemon_gen1impl::get_friendship() {
        if(_database_entry.get_game_id() == YELLOW and
           _database_entry.get_species_id() == PIKACHU)
        {
            return _yellow_pikachu_friendship;
        } else {
            throw pkmn::feature_not_in_game_error("Friendship", "Generation I");
        }
    }

    void pokemon_gen1impl::set_friendship(
        int friendship
    ) {
        if(_database_entry.get_game_id() == YELLOW and
           _database_entry.get_species_id() == PIKACHU)
        {
            if(friendship < 0 or friendship > 255) {
                throw pkmn::range_error("friendship", 0, 255);
            } else {
                _yellow_pikachu_friendship = uint8_t(friendship);
            }
        } else {
            throw pkmn::feature_not_in_game_error("Friendship", "Generation I");
        }
    }

    std::string pokemon_gen1impl::get_ability() {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation I");
    }

    void pokemon_gen1impl::set_ability(
        PKMN_UNUSED(const std::string &ability)
    ) {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation I");
    }

    std::string pokemon_gen1impl::get_ball() {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation I.");
    }

    void pokemon_gen1impl::set_ball(
        PKMN_UNUSED(const std::string &ball)
    ) {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation I.");
    }

    int pokemon_gen1impl::get_level_caught() {
        throw pkmn::feature_not_in_game_error("Level caught is not recorded in Generation I.");
    }

    void pokemon_gen1impl::set_level_caught(
        PKMN_UNUSED(int level)
    ) {
        throw pkmn::feature_not_in_game_error("Level caught is not recorded in Generation I.");
    }

    std::string pokemon_gen1impl::get_location_met(
        PKMN_UNUSED(bool as_egg)
    ) {
        throw pkmn::feature_not_in_game_error("Location caught is not recorded in Generation I.");
    }

    void pokemon_gen1impl::set_location_met(
        PKMN_UNUSED(const std::string &location),
        PKMN_UNUSED(bool as_egg)
    ) {
        throw pkmn::feature_not_in_game_error("Location caught is not recorded in Generation I.");
    }

    std::string pokemon_gen1impl::get_original_game() {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation I.");
    }

    void pokemon_gen1impl::set_original_game(
        PKMN_UNUSED(const std::string &game)
    ) {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation I.");
    }

    uint32_t pokemon_gen1impl::get_personality() {
        throw pkmn::feature_not_in_game_error("Personality", "Generation I");
    }

    void pokemon_gen1impl::set_personality(
        PKMN_UNUSED(uint32_t personality)
    ) {
        throw pkmn::feature_not_in_game_error("Personality", "Generation I");
    }

    int pokemon_gen1impl::get_experience() {
        pokemon_scoped_lock lock(this);

        uint32_t ret = 0;
        PKSAV_CALL(
            pksav_from_base256(
                GEN1_PC_RCAST->exp,
                3,
                &ret
            );
        )

        return int(ret);
    }

    void pokemon_gen1impl::set_experience(
        int experience
    ) {
        int max_experience = _database_entry.get_experience_at_level(100);

        if(experience < 0 or experience > max_experience) {
            throw pkmn::range_error("experience", 0, max_experience);
        }

        pokemon_scoped_lock lock(this);

        PKSAV_CALL(
            pksav_to_base256(
                experience,
                GEN1_PC_RCAST->exp,
                3
            );
        )

        GEN1_PC_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));
        GEN1_PARTY_RCAST->level = GEN1_PC_RCAST->level;

        _calculate_stats();
        _update_stat_map();
    }

    int pokemon_gen1impl::get_level() {
        pokemon_scoped_lock lock(this);

        return int(GEN1_PARTY_RCAST->level);
    }

    void pokemon_gen1impl::set_level(
        int level
    ) {
        if(level < 2 or level > 100) {
            throw pkmn::range_error("level", 2, 100);
        }

        pokemon_scoped_lock lock(this);

        GEN1_PC_RCAST->level = GEN1_PARTY_RCAST->level = uint8_t(level);

        PKSAV_CALL(
            pksav_to_base256(
                uint32_t(_database_entry.get_experience_at_level(level)),
                GEN1_PC_RCAST->exp,
                3
            );
        )

        _calculate_stats();
        _update_stat_map();
    }

    void pokemon_gen1impl::set_marking(
        PKMN_UNUSED(const std::string &marking),
        PKMN_UNUSED(bool value)
    ) {
        throw pkmn::feature_not_in_game_error("Markings", "Generation I");
    }

    void pokemon_gen1impl::set_ribbon(
        PKMN_UNUSED(const std::string &ribbon),
        PKMN_UNUSED(bool value)
    ) {
        throw pkmn::feature_not_in_game_error("Ribbons", "Generation I");
    }

    void pokemon_gen1impl::set_move(
        const std::string &move,
        int index
    ) {
        if(index < 0 or index > 3) {
            throw pkmn::range_error("index", 0, 3);
        }

        // This will throw an error if the move is invalid
        _moves[index].move = pkmn::database::move_entry(
                                 move,
                                 get_game()
                             );
        _moves[index].pp = _moves[index].move.get_pp(0);

        pokemon_scoped_lock lock(this);

        GEN1_PC_RCAST->moves[index] = uint8_t(_moves[index].move.get_move_id());
        GEN1_PC_RCAST->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gen1impl::set_EV(
        const std::string &stat,
        int value
    ) {
        if(not pkmn_string_is_gen1_stat(stat.c_str())) {
            throw std::invalid_argument("Invalid stat.");
        } else if(not pkmn_EV_in_bounds(value, false)) {
            throw pkmn::range_error(stat, 0, 65535);
        }

        pokemon_scoped_lock lock(this);

        if(stat == "HP") {
            GEN1_PC_RCAST->ev_hp = pksav_bigendian16(uint16_t(value));
        } else if(stat == "Attack") {
            GEN1_PC_RCAST->ev_atk = pksav_bigendian16(uint16_t(value));
        } else if(stat == "Defense") {
            GEN1_PC_RCAST->ev_def = pksav_bigendian16(uint16_t(value));
        } else if(stat == "Speed") {
            GEN1_PC_RCAST->ev_spd = pksav_bigendian16(uint16_t(value));
        } else {
            GEN1_PC_RCAST->ev_spcl = pksav_bigendian16(uint16_t(value));
        }

        _update_EV_map();
        _calculate_stats();
    }

    void pokemon_gen1impl::set_IV(
        const std::string &stat,
        int value
    ) {
        if(not pkmn_string_is_gen1_stat(stat.c_str())) {
            throw std::invalid_argument("Invalid stat.");
        } else if(not pkmn_IV_in_bounds(value, false)) {
            throw pkmn::range_error(stat, 0, 15);
        }

        pokemon_scoped_lock lock(this);

        PKSAV_CALL(
            pksav_set_gb_IV(
                &GEN1_PC_RCAST->iv_data,
                pkmn_stats_to_pksav.at(stat),
                uint8_t(value)
            );
        )

        _update_IV_map();
        _calculate_stats();
    }

    void pokemon_gen1impl::_calculate_stats() {
        pksav::gen1_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_gen1_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gen1_pokemon_party_data_t*>(_native_party)
        );

        GEN1_PC_RCAST->current_hp = GEN1_PARTY_RCAST->max_hp;

        _update_stat_map();
    }

    void pokemon_gen1impl::_update_moves(
        int index
    ) {
        if(_moves.size() != 4) {
            _moves.resize(4);
        }
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        GEN1_PC_RCAST->moves[index],
                        _database_entry.get_game_id()
                    ),
                    (GEN1_PC_RCAST->move_pps[index] & PKSAV_GEN1_MOVE_PP_MASK)
                );
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gen1impl::_update_held_item() {
        throw pkmn::feature_not_in_game_error("Held items", "Generation I");
    }

    void pokemon_gen1impl::_update_markings_map() {
        throw pkmn::feature_not_in_game_error("Markings", "Generation I");
    }

    void pokemon_gen1impl::_update_ribbons_map() {
        throw pkmn::feature_not_in_game_error("Ribbons", "Generation I");
    }

    void pokemon_gen1impl::_update_EV_map() {
        _EVs["HP"]      = int(pksav_bigendian16(GEN1_PC_RCAST->ev_hp));
        _EVs["Attack"]  = int(pksav_bigendian16(GEN1_PC_RCAST->ev_atk));
        _EVs["Defense"] = int(pksav_bigendian16(GEN1_PC_RCAST->ev_def));
        _EVs["Speed"]   = int(pksav_bigendian16(GEN1_PC_RCAST->ev_spd));
        _EVs["Special"] = int(pksav_bigendian16(GEN1_PC_RCAST->ev_spcl));
    }

    void pokemon_gen1impl::_update_IV_map() {
        uint8_t IV = 0;

        PKSAV_CALL(
            pksav_get_gb_IV(
                &GEN1_PC_RCAST->iv_data,
                PKSAV_STAT_HP,
                &IV
            );
        )
        _IVs["HP"] = int(IV);

        PKSAV_CALL(
            pksav_get_gb_IV(
                &GEN1_PC_RCAST->iv_data,
                PKSAV_STAT_ATTACK,
                &IV
            );
        )
        _IVs["Attack"] = int(IV);

        PKSAV_CALL(
            pksav_get_gb_IV(
                &GEN1_PC_RCAST->iv_data,
                PKSAV_STAT_DEFENSE,
                &IV
            );
        )
        _IVs["Defense"] = int(IV);

        PKSAV_CALL(
            pksav_get_gb_IV(
                &GEN1_PC_RCAST->iv_data,
                PKSAV_STAT_SPEED,
                &IV
            );
        )
        _IVs["Speed"] = int(IV);

        PKSAV_CALL(
            pksav_get_gb_IV(
                &GEN1_PC_RCAST->iv_data,
                PKSAV_STAT_SPECIAL,
                &IV
            );
        )
        _IVs["Special"] = int(IV);
    }

    void pokemon_gen1impl::_update_stat_map() {
        _stats["HP"]      = int(pksav_bigendian16(GEN1_PARTY_RCAST->max_hp));
        _stats["Attack"]  = int(pksav_bigendian16(GEN1_PARTY_RCAST->atk));
        _stats["Defense"] = int(pksav_bigendian16(GEN1_PARTY_RCAST->def));
        _stats["Speed"]   = int(pksav_bigendian16(GEN1_PARTY_RCAST->spd));
        _stats["Special"] = int(pksav_bigendian16(GEN1_PARTY_RCAST->spcl));
    }
}
