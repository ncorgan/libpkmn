/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "misc_common.hpp"
#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"

#include "conversions/gb_conversions.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include "mem/scoped_lock.hpp"
#include "types/rng.hpp"

#include <pkmn/exception.hpp>

#include <pksav/common/stats.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>

#include <cstring>
#include <stdexcept>
#include <unordered_map>

#define GEN1_PC_RCAST    (reinterpret_cast<pksav_gen1_pc_pokemon_t*>(_native_pc))
#define GEN1_PARTY_RCAST (reinterpret_cast<pksav_gen1_pokemon_party_data_t*>(_native_party))

static const std::unordered_map<std::string, pksav_gen1_type_t> GEN1_TYPES = boost::assign::map_list_of
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
        _trainer_name = DEFAULT_TRAINER_NAME;

        // Set internal members
        GEN1_PC_RCAST->species = uint8_t(_database_entry.get_pokemon_index());

        std::pair<std::string, std::string> types = _database_entry.get_types();
        GEN1_PC_RCAST->types[0] = uint8_t(GEN1_TYPES.at(types.first));
        GEN1_PC_RCAST->types[1] = (types.second == "None") ? GEN1_PC_RCAST->types[0]
                                                           : uint8_t(GEN1_TYPES.at(types.second));

        // TODO: catch rate

        GEN1_PC_RCAST->ot_id = pksav_bigendian16(uint16_t(DEFAULT_TRAINER_ID & 0xFFFF));

        pkmn::rng<uint16_t> rng;
        GEN1_PC_RCAST->ev_hp   = rng.rand();
        GEN1_PC_RCAST->ev_atk  = rng.rand();
        GEN1_PC_RCAST->ev_def  = rng.rand();
        GEN1_PC_RCAST->ev_spd  = rng.rand();
        GEN1_PC_RCAST->ev_spcl = rng.rand();
        GEN1_PC_RCAST->iv_data = rng.rand();

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&GEN1_PC_RCAST->iv_data);
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
        _populate_party_data();
        _our_party_mem = true;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&GEN1_PC_RCAST->iv_data);
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
        _init_gb_IV_map(&GEN1_PC_RCAST->iv_data);
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen1impl::pokemon_gen1impl(
        const pksav_gen1_pc_pokemon_t& pc,
        int game_id
    ): pokemon_impl(pc.species, game_id),
       _yellow_pikachu_friendship(0)
    {
        _native_pc = reinterpret_cast<void*>(new pksav_gen1_pc_pokemon_t);
        *GEN1_PC_RCAST = pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen1_pokemon_party_data_t);
        _populate_party_data();
        _our_party_mem = true;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&GEN1_PC_RCAST->iv_data);
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen1impl::pokemon_gen1impl(
        const pksav_gen1_party_pokemon_t& party,
        int game_id
    ): pokemon_impl(party.pc.species, game_id),
       _yellow_pikachu_friendship(0)
    {
        _native_pc = reinterpret_cast<void*>(new pksav_gen1_pc_pokemon_t);
        *GEN1_PC_RCAST = party.pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen1_pokemon_party_data_t);
        *GEN1_PARTY_RCAST = party.party_data;
        _our_party_mem = true;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&GEN1_PC_RCAST->iv_data);
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

    pokemon::sptr pokemon_gen1impl::to_game(
        const std::string& game
    )
    {
        pkmn::pokemon::sptr ret;

        pksav_gen1_party_pokemon_t pksav_pokemon;
        pksav_pokemon.pc = *GEN1_PC_RCAST;
        pksav_pokemon.party_data = *GEN1_PARTY_RCAST;

        int game_id = pkmn::database::game_name_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);
        switch(generation)
        {
            case 1:
            {
                ret = pkmn::make_shared<pokemon_gen1impl>(pksav_pokemon, game_id);
                break;
            }

            case 2:
            {
                pksav_gen2_party_pokemon_t gen2_pksav_pokemon;
                pkmn::conversions::gen1_party_pokemon_to_gen2(
                    &pksav_pokemon,
                    &gen2_pksav_pokemon
                );
                ret = pkmn::make_shared<pokemon_gen2impl>(gen2_pksav_pokemon, game_id);
                ret->set_level_met(std::min<int>(63, get_level()));
                break;
            }

            default:
                throw std::invalid_argument("Generation I Pokémon can only be converted to Generation I-II.");
        }

        ret->set_nickname(get_nickname());
        ret->set_trainer_name(get_trainer_name());

        return ret;
    }

    void pokemon_gen1impl::set_form(
        const std::string &form
    ) {
        _database_entry.set_form(form);
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

    std::string pokemon_gen1impl::get_gender() {
        throw pkmn::feature_not_in_game_error("Pokémon gender", "Generation I");
    }

    void pokemon_gen1impl::set_gender(
        PKMN_UNUSED(const std::string &gender)
    ) {
        throw pkmn::feature_not_in_game_error("Pokémon gender", "Generation I");
    }

    bool pokemon_gen1impl::is_shiny() {
        throw pkmn::feature_not_in_game_error("Shininess", "Generation I");
    }

    void pokemon_gen1impl::set_shininess(
        PKMN_UNUSED(bool value)
    ) {
        throw pkmn::feature_not_in_game_error("Shininess", "Generation I");
    }

    std::string pokemon_gen1impl::get_held_item()
    {
        throw pkmn::feature_not_in_game_error("Held items", "Generation I");
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
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return pksav_bigendian16(GEN1_PC_RCAST->ot_id);
    }

    uint16_t pokemon_gen1impl::get_trainer_secret_id() {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation I");
    }

    uint32_t pokemon_gen1impl::get_trainer_id() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

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
    )
    {
        pkmn::enforce_gb_trainer_id_bounds(id);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

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
        int ret = 0;

        if(_database_entry.get_game_id() == YELLOW and
           _database_entry.get_species_id() == PIKACHU)
        {
            ret = _yellow_pikachu_friendship;
        } else {
            throw pkmn::feature_not_in_game_error("Friendship", "Generation I");
        }

        return ret;
    }

    void pokemon_gen1impl::set_friendship(
        int friendship
    )
    {
        if(_database_entry.get_game_id() == YELLOW and
           _database_entry.get_species_id() == PIKACHU)
        {
            pkmn::enforce_bounds("Friendship", friendship, 0, 255);

            _yellow_pikachu_friendship = uint8_t(friendship);
        }
        else
        {
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

    int pokemon_gen1impl::get_level_met() {
        throw pkmn::feature_not_in_game_error("Level caught is not recorded in Generation I.");
    }

    void pokemon_gen1impl::set_level_met(
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
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

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
        pkmn::enforce_bounds("Experience", experience, 0, max_experience);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        PKSAV_CALL(
            pksav_to_base256(
                experience,
                GEN1_PC_RCAST->exp,
                3
            );
        )

        GEN1_PC_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));
        GEN1_PARTY_RCAST->level = GEN1_PC_RCAST->level;

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gen1impl::get_level() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return int(GEN1_PARTY_RCAST->level);
    }

    void pokemon_gen1impl::set_level(
        int level
    )
    {
        pkmn::enforce_bounds("Level", level, 2, 100);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GEN1_PC_RCAST->level = GEN1_PARTY_RCAST->level = uint8_t(level);

        PKSAV_CALL(
            pksav_to_base256(
                uint32_t(_database_entry.get_experience_at_level(level)),
                GEN1_PC_RCAST->exp,
                3
            );
        )

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_gen1impl::set_IV(
        const std::string &stat,
        int value
    ) {
        _set_gb_IV(
            stat,
            value,
            &GEN1_PC_RCAST->iv_data
        );
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

    void pokemon_gen1impl::set_contest_stat(
        PKMN_UNUSED(const std::string &stat),
        PKMN_UNUSED(int value)
    ) {
        throw pkmn::feature_not_in_game_error("Contests", "Generation I");
    }

    void pokemon_gen1impl::set_move(
        const std::string &move,
        int index
    ) {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        // This will throw an error if the move is invalid.
        pkmn::database::move_entry entry(
            move,
            get_game()
        );
        _moves[index].move = entry.get_name();
        _moves[index].pp   = entry.get_pp(0);

        GEN1_PC_RCAST->moves[index] = uint8_t(entry.get_move_id());
        GEN1_PC_RCAST->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gen1impl::set_EV(
        const std::string &stat,
        int value
    )
    {
        if(not pkmn::string_is_gen1_stat(stat)) {
            pkmn::throw_invalid_argument("stat", pkmn::GEN1_STATS);
        }

        pkmn::enforce_EV_bounds(stat, value, false);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        if(stat == "HP")
        {
            GEN1_PC_RCAST->ev_hp = pksav_bigendian16(uint16_t(value));
        }
        else if(stat == "Attack")
        {
            GEN1_PC_RCAST->ev_atk = pksav_bigendian16(uint16_t(value));
        }
        else if(stat == "Defense")
        {
            GEN1_PC_RCAST->ev_def = pksav_bigendian16(uint16_t(value));
        }
        else if(stat == "Speed")
        {
            GEN1_PC_RCAST->ev_spd = pksav_bigendian16(uint16_t(value));
        }
        else
        {
            GEN1_PC_RCAST->ev_spcl = pksav_bigendian16(uint16_t(value));
        }

        _update_EV_map();
        _populate_party_data();
    }

    std::string pokemon_gen1impl::get_icon_filepath() {
        return _database_entry.get_icon_filepath(false);
    }

    std::string pokemon_gen1impl::get_sprite_filepath() {
        return _database_entry.get_sprite_filepath(false,false);
    }

    void pokemon_gen1impl::_populate_party_data() {
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
                    pkmn::database::move_id_to_name(
                        GEN1_PC_RCAST->moves[index], 1
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

    void pokemon_gen1impl::_update_EV_map() {
        _EVs["HP"]      = int(pksav_bigendian16(GEN1_PC_RCAST->ev_hp));
        _EVs["Attack"]  = int(pksav_bigendian16(GEN1_PC_RCAST->ev_atk));
        _EVs["Defense"] = int(pksav_bigendian16(GEN1_PC_RCAST->ev_def));
        _EVs["Speed"]   = int(pksav_bigendian16(GEN1_PC_RCAST->ev_spd));
        _EVs["Special"] = int(pksav_bigendian16(GEN1_PC_RCAST->ev_spcl));
    }

    void pokemon_gen1impl::_update_stat_map() {
        _stats["HP"]      = int(pksav_bigendian16(GEN1_PARTY_RCAST->max_hp));
        _stats["Attack"]  = int(pksav_bigendian16(GEN1_PARTY_RCAST->atk));
        _stats["Defense"] = int(pksav_bigendian16(GEN1_PARTY_RCAST->def));
        _stats["Speed"]   = int(pksav_bigendian16(GEN1_PARTY_RCAST->spd));
        _stats["Special"] = int(pksav_bigendian16(GEN1_PARTY_RCAST->spcl));
    }
}
