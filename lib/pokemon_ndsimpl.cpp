/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils/misc.hpp"
#include "pokemon_ndsimpl.hpp"
#include "database/id_to_index.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include <pkmn/calculations/shininess.hpp>

#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include <pksav/common/gen3_ribbons.h>
#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gen4/text.h>
#include <pksav/gen5/text.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <ctime>
#include <random>
#include <stdexcept>

#define NDS_PC_RCAST    reinterpret_cast<pksav_nds_pc_pokemon_t*>(_native_pc)
#define NDS_PARTY_RCAST reinterpret_cast<pksav_nds_pokemon_party_data_t*>(_native_party)

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int DIAMOND = 12;
    BOOST_STATIC_CONSTEXPR int PLATINUM = 14;
    BOOST_STATIC_CONSTEXPR int HEARTGOLD = 15;
    BOOST_STATIC_CONSTEXPR int SOULSILVER = 16;

    pokemon_ndsimpl::pokemon_ndsimpl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry)),
       _gen4(database_entry.get_game_id() >= DIAMOND and database_entry.get_game_id() <= SOULSILVER),
       _plat(database_entry.get_game_id() == PLATINUM),
       _hgss(database_entry.get_game_id() == HEARTGOLD or database_entry.get_game_id() == SOULSILVER)
    {
        _native_pc = reinterpret_cast<void*>(new pksav_nds_pc_pokemon_t);
        std::memset(_native_pc, 0, sizeof(pksav_nds_pc_pokemon_t));
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_nds_pokemon_party_data_t);
        std::memset(_native_party, 0, sizeof(pksav_nds_pokemon_party_data_t));
        _our_party_mem = true;

        // Set _block pointers
        _blockA = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC = &NDS_PC_RCAST->blocks.blockC;
        _blockD = &NDS_PC_RCAST->blocks.blockD;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_blockB->iv_isegg_isnicknamed);
        _init_contest_stat_map(&_blockA->contest_stats);
        _init_markings_map(&_blockA->markings);
        set_level(level);
        _update_moves(-1);
    }

    pokemon_ndsimpl::pokemon_ndsimpl(
        pksav_nds_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(pc->blocks.blockA.species),
           game_id
       ),
       _gen4(game_id >= DIAMOND and game_id <= SOULSILVER),
       _plat(game_id == PLATINUM),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_nds_pokemon_party_data_t);
        pksav::nds_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_nds_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_nds_pokemon_party_data_t*>(_native_party)
        );
        _our_party_mem = true;

        // Set _block pointers
        _blockA = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC = &NDS_PC_RCAST->blocks.blockC;
        _blockD = &NDS_PC_RCAST->blocks.blockD;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_blockB->iv_isegg_isnicknamed);
        _init_contest_stat_map(&_blockA->contest_stats);
        _init_markings_map(&_blockA->markings);
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_ndsimpl::pokemon_ndsimpl(
        pksav_nds_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(party->pc_data.blocks.blockA.species),
           game_id
       ),
       _gen4(game_id >= DIAMOND and game_id <= SOULSILVER),
       _plat(game_id == PLATINUM),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
    {
        _native_pc = reinterpret_cast<void*>(&party->pc_data;
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Set _block pointers
        _blockA = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC = &NDS_PC_RCAST->blocks.blockC;
        _blockD = &NDS_PC_RCAST->blocks.blockD;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_blockB->iv_isegg_isnicknamed);
        _init_contest_stat_map(&_blockA->contest_stats);
        _init_markings_map(&_blockA->markings);
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_ndsimpl::pokemon_ndsimpl(
        const pksav_nds_pc_pokemon_t &pc,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(pc.blocks.blockA.species),
           game_id
       ),
       _gen4(game_id >= DIAMOND and game_id <= SOULSILVER),
       _plat(game_id == PLATINUM),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
    {
        _native_pc = reinterpret_cast<void*>(new pksav_nds_pc_pokemon_t);
        *NDS_PC_RCAST = pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_nds_pokemon_party_data_t);
        pksav::nds_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_nds_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_nds_pokemon_party_data_t*>(_native_party)
        );
        _our_party_mem = true;

        // Set _block pointers
        _blockA = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC = &NDS_PC_RCAST->blocks.blockC;
        _blockD = &NDS_PC_RCAST->blocks.blockD;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_blockB->iv_isegg_isnicknamed);
        _init_contest_stat_map(&_blockA->contest_stats);
        _init_markings_map(&_blockA->markings);
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_ndsimpl::~pokemon_ndsimpl() {
        if(_our_pc_mem) {
            delete NDS_PC_RCAST;
        }
        if(_our_party_mem) {
            delete NDS_PARTY_RCAST;
        }
    }

    std::string pokemon_ndsimpl::get_nickname() {
        char nickname[11] = {0};
        if(_gen4) {
            PKSAV_CALL(
                pksav_text_from_gen4(
                    _blockC->nickname,
                    nickname,
                    10
                );
            )
        } else {
            PKSAV_CALL(
                pksav_text_from_gen5(
                    _blockC->nickname,
                    nickname,
                    10
                );
            )
        }

        return std::string(nickname);
    }

    // TODO: isnicknamed flag
    void pokemon_ndsimpl::set_nickname(
        const std::string &nickname
    ) {
        if(nickname.size() < 1 or nickname.size() > 10) {
            throw std::invalid_argument(
                      "The nickname length must be 1-10."
                  );
        }

        if(_gen4) {
            PKSAV_CALL(
                pksav_text_to_gen4(
                    nickname.c_str(),
                    _blockC->nickname,
                    10
                );
            )
        } else {
            PKSAV_CALL(
                pksav_text_to_gen5(
                    nickname.c_str(),
                    _blockC->nickname,
                    10
                );
            )
        }
    }

    bool pokemon_ndsimpl::is_shiny() {
        pokemon_scoped_lock lock(this);

        return pkmn::calculations::modern_shiny(
                   pksav_littleendian32(NDS_PC_RCAST->personality),
                   pksav_littleendian32(_blockA->ot_id.id)
               );
    }

    void pokemon_ndsimpl::set_shininess(
        bool value
    ) {
        pokemon_scoped_lock lock(this);

        _set_modern_shininess(
            &NDS_PC_RCAST->personality,
            &_blockA->ot_id.id,
            value
        );
    }

    void pokemon_ndsimpl::set_held_item(
        const std::string &held_item
    ) {
        // Make sure item is valid and holdable
        pkmn::database::item_entry item(
            held_item,
            get_game()
        );

        if(not item.holdable()) {
            throw std::invalid_argument("This item is not holdable.");
        }

        pokemon_scoped_lock lock(this);

        _blockA->held_item = pksav_littleendian16(uint16_t(item.get_item_index()));

        _update_held_item();
    }

    std::string pokemon_ndsimpl::get_original_trainer_name() {
        char otname[8] = {0};
        if(_gen4) {
            PKSAV_CALL(
                pksav_text_from_gen4(
                    _blockD->otname,
                    otname,
                    7
                );
            )
        } else {
            PKSAV_CALL(
                pksav_text_from_gen5(
                    _blockD->otname,
                    otname,
                    7
                );
            )
        }

        return std::string(otname);
    }

    void pokemon_ndsimpl::set_original_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() < 1 or trainer_name.size() > 7) {
            throw std::invalid_argument(
                      "The trainer name name length must be 1-7."
                  );
        }

        if(_gen4) {
            PKSAV_CALL(
                pksav_text_to_gen4(
                    trainer_name.c_str(),
                    _blockD->otname,
                    7
                );
            )
        } else {
            PKSAV_CALL(
                pksav_text_to_gen5(
                    trainer_name.c_str(),
                    _blockD->otname,
                    7
                );
            )
        }
    }

    uint16_t pokemon_ndsimpl::get_original_trainer_public_id() {
        return pksav_littleendian16(_blockA->ot_id.pid);
    }

    uint16_t pokemon_ndsimpl::get_original_trainer_secret_id() {
        return pksav_littleendian16(_blockA->ot_id.sid);
    }

    uint32_t pokemon_ndsimpl::get_original_trainer_id() {
        return pksav_littleendian32(_blockA->ot_id.id);
    }

    void pokemon_ndsimpl::set_original_trainer_public_id(
        uint16_t public_id
    ) {
        _blockA->ot_id.pid = pksav_littleendian16(public_id);
    }

    void pokemon_ndsimpl::set_original_trainer_secret_id(
        uint16_t secret_id
    ) {
        _blockA->ot_id.sid = pksav_littleendian16(secret_id);
    }

    void pokemon_ndsimpl::set_original_trainer_id(
        uint32_t id
    ) {
        _blockA->ot_id.id = pksav_littleendian32(id);
    }

    std::string pokemon_ndsimpl::get_original_trainer_gender() {
        return (_blockD->metlevel_otgender & PKSAV_NDS_OTGENDER_MASK) ? "Female"
                                                                      : "Male";
    }

    void pokemon_ndsimpl::set_original_trainer_gender(
        const std::string &gender
    ) {
        if(gender == "Male") {
            _blockD->metlevel_otgender &= ~PKSAV_NDS_OTGENDER_MASK;
        } else if(gender == "Female") {
            _blockD->metlevel_otgender |= PKSAV_NDS_OTGENDER_MASK;
        } else {
            throw std::invalid_argument("gender: valid values \"Male\", \"Female\"");
        }
    }

    int pokemon_ndsimpl::get_current_trainer_friendship() {
        pokemon_scoped_lock lock(this);

        return _blockA->friendship;
    }

    void pokemon_ndsimpl::set_current_trainer_friendship(
        int friendship
    ) {
        if(friendship < 0 or friendship > 255) {
            pkmn::throw_out_of_range("friendship", 0, 255);
        }

        pokemon_scoped_lock lock(this);

        _blockA->friendship = uint8_t(friendship);
    }

    std::string pokemon_ndsimpl::get_ability() {
        return pkmn::database::ability_id_to_name(_blockA->ability);
    }

    void pokemon_ndsimpl::set_ability(
        const std::string &ability
    ) {
        std::pair<std::string, std::string> abilities = _database_entry.get_abilities();

        if(ability == "None") {
            throw std::invalid_argument("The ability cannot be set to None.");
        } else if(ability == abilities.first || ability == abilities.second) {
            _blockA->ability = uint8_t(pkmn::database::ability_name_to_id(ability));
        } else {
            std::string error_message;
            if(abilities.second == "None") {
                error_message = str(boost::format("ability: valid values \"%s\"")
                                    % abilities.first.c_str());
            } else {
                error_message = str(boost::format("ability: valid values \"%s\", \"%s\"")
                                    % abilities.first.c_str()
                                    % abilities.second.c_str());
            }

            throw std::invalid_argument(error_message.c_str());
        }
    }

    std::string pokemon_ndsimpl::get_ball() {
        return pkmn::database::ball_id_to_name(
                   _hgss ? _blockD->ball_hgss
                         : _blockD->ball
               );
    }

    void pokemon_ndsimpl::set_ball(
        const std::string &ball
    ) {
        (_hgss ? _blockD->ball_hgss : _blockD->ball) = uint8_t(pkmn::database::ball_name_to_id(
                                                                   ball
                                                               ));
    }

    int pokemon_ndsimpl::get_level_met() {
        pokemon_scoped_lock lock(this);

        return (_blockD->metlevel_otgender & PKSAV_NDS_LEVELMET_MASK);
    }

    void pokemon_ndsimpl::set_level_met(
        int level
    ) {
        if(level < 0 or level > 100) {
            pkmn::throw_out_of_range("Level caught", 0, 100);
        }

        pokemon_scoped_lock lock(this);

        _blockD->metlevel_otgender &= ~PKSAV_NDS_LEVELMET_MASK;
        _blockD->metlevel_otgender |= uint16_t(level);
    }

    std::string pokemon_ndsimpl::get_location_met(
        bool as_egg
    ) {
        return pkmn::database::location_index_to_name(
                   pksav_littleendian16(_plat ? as_egg ? _blockB->eggmet_plat
                                                       : _blockB->met_plat
                                              : as_egg ? _blockD->eggmet_dp
                                                       : _blockD->met_dp
                                       ),
                   _database_entry.get_game_id()
               );
    }

    void pokemon_ndsimpl::set_location_met(
        const std::string &location,
        bool as_egg
    ) {
        uint16_t* field = _plat ? as_egg ? &_blockB->eggmet_plat
                                         : &_blockB->met_plat
                                : as_egg ? &_blockD->eggmet_dp
                                         : &_blockD->met_dp;

        *field = pksav_littleendian16(uint16_t(
                     pkmn::database::location_name_to_index(
                         location,
                         _database_entry.get_game_id()
                     )
                 ));
    }

    std::string pokemon_ndsimpl::get_original_game() {
        return pkmn::database::game_index_to_name(_blockC->hometown);
    }

    void pokemon_ndsimpl::set_original_game(
        const std::string &game
    ) {
        _blockC->hometown = uint8_t(pkmn::database::game_name_to_index(game));
    }

    uint32_t pokemon_ndsimpl::get_personality() {
        return pksav_littleendian32(NDS_PC_RCAST->personality);
    }

    // TODO: automatically update personality-based stuff
    void pokemon_ndsimpl::set_personality(
        uint32_t personality
    ) {
        NDS_PC_RCAST->personality = pksav_littleendian32(personality);
    }

    int pokemon_ndsimpl::get_experience() {
        return int(pksav_littleendian32(_blockA->exp));
    }

    void pokemon_ndsimpl::set_experience(
        int experience
    ) {
        int max_experience = _database_entry.get_experience_at_level(100);

        if(experience < 0 or experience > max_experience) {
            pkmn::throw_out_of_range("experience", 0, max_experience);
        }

        _blockA->exp = pksav_littleendian32(uint32_t(experience));
        NDS_PARTY_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_ndsimpl::get_level() {
        return int(NDS_PARTY_RCAST->level);
    }

    void pokemon_ndsimpl::set_level(
        int level
    ) {
        if(level < 0 or level > 100) {
            pkmn::throw_out_of_range("level", 0, 100);
        }

        NDS_PARTY_RCAST->level = uint8_t(level);
        _blockA->exp = uint32_t(_database_entry.get_experience_at_level(level));

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_ndsimpl::set_IV(
        const std::string &stat,
        int value
    ) {
        _set_modern_IV(
            stat,
            value,
            &_blockB->iv_isegg_isnicknamed
        );
    }

    void pokemon_ndsimpl::set_marking(
        const std::string &marking,
        bool value
    ) {
        _set_marking(
            marking,
            value,
            &_blockA->markings
        );
    }

    void pokemon_ndsimpl::set_ribbon(
        PKMN_UNUSED(const std::string &ribbon),
        PKMN_UNUSED(bool value)
    ) {
        throw pkmn::unimplemented_error();
    }

    void pokemon_ndsimpl::set_contest_stat(
        const std::string &stat,
        int value
    ) {
        _set_contest_stat(
            stat,
            value,
            &_blockA->contest_stats
        );
    }

    void pokemon_ndsimpl::set_move(
        const std::string &move,
        int index
    ) {
        if(index < 0 or index > 3) {
            pkmn::throw_out_of_range("index", 0, 3);
        }

        // This will throw an error if the move is invalid
        _moves[index].move = pkmn::database::move_entry(
                                 move,
                                 get_game()
                             );
        _moves[index].pp = _moves[index].move.get_pp(0);

        _blockB->moves[index] = uint8_t(_moves[index].move.get_move_id());
        _blockB->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_ndsimpl::_populate_party_data() {
        pksav::nds_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_nds_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_nds_pokemon_party_data_t*>(_native_party)
        );
    }

    void pokemon_ndsimpl::set_EV(
        const std::string &stat,
        int value
    ) {
        if(not pkmn_string_is_modern_stat(stat.c_str())) {
            throw std::invalid_argument("Invalid stat.");
        } else if(not pkmn::EV_in_bounds(value, true)) {
            throw std::out_of_range("Invalid stat.");
        }

        if(stat == "HP") {
            _blockA->ev_hp = uint8_t(value);
        } else if(stat == "Attack") {
            _blockA->ev_atk = uint8_t(value);
        } else if(stat == "Defense") {
            _blockA->ev_def = uint8_t(value);
        } else if(stat == "Speed") {
            _blockA->ev_spd = uint8_t(value);
        } else if(stat == "Special Attack") {
            _blockA->ev_spatk = uint8_t(value);
        } else {
            _blockA->ev_spdef = uint8_t(value);
        }

        _update_EV_map();
        _populate_party_data();
    }

    void pokemon_ndsimpl::_update_moves(
        int index
    ) {
        _moves.reserve(4);
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        pksav_littleendian16(_blockB->moves[index]),
                        _database_entry.get_game_id()
                    ),
                    pksav_littleendian16(_blockB->move_pps[index])
                );
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_ndsimpl::_update_held_item() {
        if(int(pksav_littleendian16(_blockA->held_item)) != _held_item.get_item_index()) {
            _held_item = pkmn::database::item_entry(
                             pksav_littleendian16(_blockA->held_item),
                             _database_entry.get_game_id()
                         );
        }
    }

    void pokemon_ndsimpl::_update_ribbons_map() {
        throw pkmn::unimplemented_error();
    }

    void pokemon_ndsimpl::_update_EV_map() {
        _EVs["HP"]              = int(_blockA->ev_hp);
        _EVs["Attack"]          = int(_blockA->ev_atk);
        _EVs["Defense"]         = int(_blockA->ev_def);
        _EVs["Speed"]           = int(_blockA->ev_spd);
        _EVs["Special Attack"]  = int(_blockA->ev_spatk);
        _EVs["Special Defense"] = int(_blockA->ev_spdef);
    }

    void pokemon_ndsimpl::_update_stat_map() {
        _stats["HP"]              = int(pksav_littleendian16(NDS_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_littleendian16(NDS_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_littleendian16(NDS_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_littleendian16(NDS_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_littleendian16(NDS_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_littleendian16(NDS_PARTY_RCAST->spdef));
    }
}
