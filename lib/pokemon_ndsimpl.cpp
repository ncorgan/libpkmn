/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "misc_common.hpp"
#include "pokemon_ndsimpl.hpp"
#include "database/id_to_index.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "pksav/party_data.hpp"

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
        int pokemon_index,
        int game_id,
        int level
    ): pokemon_impl(pokemon_index, game_id),
       _gen4(game_id >= DIAMOND and game_id <= SOULSILVER),
       _plat(game_id == PLATINUM),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
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
        _update_markings_map();
        _update_ribbons_map();
        _update_EV_map();
        _update_IV_map();
        set_level(level);
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
        _blockA  = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC  = &NDS_PC_RCAST->blocks.blockC;
        _blockD    = &NDS_PC_RCAST->blocks.blockD;

        // Populate abstractions
        _update_held_item();
        _update_markings_map();
        _update_ribbons_map();
        _update_EV_map();
        _update_IV_map();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_ndsimpl::pokemon_ndsimpl(
        pksav_nds_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(party->pc.blocks.blockA.species),
           game_id
       ),
       _gen4(game_id >= DIAMOND and game_id <= SOULSILVER),
       _plat(game_id == PLATINUM),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Set _block pointers
        _blockA  = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC  = &NDS_PC_RCAST->blocks.blockC;
        _blockD    = &NDS_PC_RCAST->blocks.blockD;

        // Populate abstractions
        _update_held_item();
        _update_markings_map();
        _update_ribbons_map();
        _update_EV_map();
        _update_IV_map();
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
        _blockA  = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC  = &NDS_PC_RCAST->blocks.blockC;
        _blockD    = &NDS_PC_RCAST->blocks.blockD;

        // Populate abstractions
        _update_held_item();
        _update_markings_map();
        _update_ribbons_map();
        _update_EV_map();
        _update_IV_map();
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
            pksav_text_from_gen4(
                _blockC->nickname,
                nickname,
                10
            );
        } else {
            pksav_text_from_gen5(
                _blockC->nickname,
                nickname,
                10
            );
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
            pksav_text_to_gen4(
                nickname.c_str(),
                _blockC->nickname,
                10
            );
        } else {
            pksav_text_to_gen5(
                nickname.c_str(),
                _blockC->nickname,
                10
            );
        }
    }

    std::string pokemon_ndsimpl::get_trainer_name() {
        char otname[8] = {0};
        if(_gen4) {
            pksav_text_from_gen4(
                _blockD->otname,
                otname,
                7
            );
        } else {
            pksav_text_from_gen5(
                _blockD->otname,
                otname,
                7
            );
        }

        return std::string(otname);
    }

    void pokemon_ndsimpl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() < 1 or trainer_name.size() > 7) {
            throw std::invalid_argument(
                      "The trainer name name length must be 1-7."
                  );
        }

        if(_gen4) {
            pksav_text_to_gen4(
                trainer_name.c_str(),
                _blockD->otname,
                7
            );
        } else {
            pksav_text_to_gen5(
                trainer_name.c_str(),
                _blockD->otname,
                7
            );
        }
    }

    uint16_t pokemon_ndsimpl::get_trainer_public_id() {
        return pksav_littleendian16(_blockA->ot_id.pid);
    }

    uint16_t pokemon_ndsimpl::get_trainer_secret_id() {
        return pksav_littleendian16(_blockA->ot_id.sid);
    }

    uint32_t pokemon_ndsimpl::get_trainer_id() {
        return pksav_littleendian32(_blockA->ot_id.id);
    }

    void pokemon_ndsimpl::set_trainer_public_id(
        uint16_t public_id
    ) {
        _blockA->ot_id.pid = pksav_littleendian16(public_id);
    }

    void pokemon_ndsimpl::set_trainer_secret_id(
        uint16_t secret_id
    ) {
        _blockA->ot_id.sid = pksav_littleendian16(secret_id);
    }

    void pokemon_ndsimpl::set_trainer_id(
        uint32_t id
    ) {
        _blockA->ot_id.id = pksav_littleendian32(id);
    }

    std::string pokemon_ndsimpl::get_trainer_gender() {
        return (_blockD->metlevel_otgender & PKSAV_NDS_OTGENDER_MASK) ? "Female"
                                                                      : "Male";
    }

    void pokemon_ndsimpl::set_trainer_gender(
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

    // TODO: as_egg
    std::string pokemon_ndsimpl::get_location_caught() {
        return pkmn::database::location_index_to_name(
                   pksav_littleendian16(_plat ? _blockB->met_plat
                                              : _blockD->met_dp
                                       ),
                   _database_entry.get_game_id()
               );
    }

    void pokemon_ndsimpl::set_location_caught(
        const std::string &location
    ) {
        (_plat ? _blockB->met_plat : _blockD->met_dp) = pksav_littleendian16(uint16_t(
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
            throw std::out_of_range(
                      str(boost::format(
                              "experience: valid range 0-%d"
                          ) % max_experience)
                  );
        }

        _blockA->exp = uint32_t(experience);
        NDS_PARTY_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));

        _calculate_stats();
        _update_stat_map();
    }

    int pokemon_ndsimpl::get_level() {
        return int(NDS_PARTY_RCAST->level);
    }

    void pokemon_ndsimpl::set_level(
        int level
    ) {
        if(level < 0 or level > 100) {
            throw std::out_of_range(
                      "level: valid range 0-100"
                  );
        }

        NDS_PARTY_RCAST->level = uint8_t(level);
        _blockA->exp = uint32_t(_database_entry.get_experience_at_level(level));

        _calculate_stats();
        _update_stat_map();
    }

    void pokemon_ndsimpl::set_marking(
        const std::string &marking,
        bool value
    ) {
        _set_marking(
            marking,
            value,
            &_blockA->markings,
            true
        );
    }

    /*static const std::map<std::string, pksav_gen3_ribbon_mask_t> nds_ribbons = boost::assign::map_list_of
        ("Champion", PKSAV_GEN3_CHAMPION_RIBBON_MASK)
        ("Winning",  PKSAV_GEN3_WINNING_RIBBON_MASK)
        ("Victory",  PKSAV_GEN3_VICTORY_RIBBON_MASK)
        ("Artist",   PKSAV_GEN3_ARTIST_RIBBON_MASK)
        ("blockC",   PKSAV_GEN3_blockC_RIBBON_MASK)
        ("Marine",   PKSAV_GEN3_MARINE_RIBBON_MASK)
        ("Land",     PKSAV_GEN3_LAND_RIBBON_MASK)
        ("Sky",      PKSAV_GEN3_SKY_RIBBON_MASK)
        ("Country",  PKSAV_GEN3_COUNTRY_RIBBON_MASK)
        ("National", PKSAV_GEN3_NATIONAL_RIBBON_MASK)
        ("Earth",    PKSAV_GEN3_EARTH_RIBBON_MASK)
        ("World",    PKSAV_GEN3_WORLD_RIBBON_MASK)
    ;

    static const std::map<std::string, pksav_gen3_contest_ribbon_level_t> nds_contest_ribbon_levels = boost::assign::map_list_of
        ("",       PKSAV_GEN3_CONTEST_RIBBON_NONE)
        ("Normal", PKSAV_GEN3_CONTEST_RIBBON_NORMAL)
        ("Super",  PKSAV_GEN3_CONTEST_RIBBON_SUPER)
        ("Hyper",  PKSAV_GEN3_CONTEST_RIBBON_HYPER)
        ("Master", PKSAV_GEN3_CONTEST_RIBBON_MASTER)
    ;

    static const std::map<std::string, pksav_gen3_contest_ribbons_mask_t> nds_contest_ribbon_masks = boost::assign::map_list_of
        ("Cool",   PKSAV_GEN3_COOL_RIBBONS_MASK)
        ("Beauty", PKSAV_GEN3_BEAUTY_RIBBONS_MASK)
        ("Cute",   PKSAV_GEN3_CUTE_RIBBONS_MASK)
        ("Smart",  PKSAV_GEN3_SMART_RIBBONS_MASK)
        ("Tough",  PKSAV_GEN3_TOUGH_RIBBONS_MASK)
    ;

    static const std::map<std::string, pksav_gen3_contest_ribbons_offset_t> nds_contest_ribbon_offsets = boost::assign::map_list_of
        ("Cool",   PKSAV_GEN3_COOL_RIBBONS_OFFSET)
        ("Beauty", PKSAV_GEN3_BEAUTY_RIBBONS_OFFSET)
        ("Cute",   PKSAV_GEN3_CUTE_RIBBONS_OFFSET)
        ("Smart",  PKSAV_GEN3_SMART_RIBBONS_OFFSET)
        ("Tough",  PKSAV_GEN3_TOUGH_RIBBONS_OFFSET)
    ;*/

    void pokemon_ndsimpl::set_ribbon(
        const std::string &ribbon,
        bool value
    ) {
        (void)value;
        if(_ribbons.find(ribbon) == _ribbons.end()) {
            throw std::invalid_argument("Invalid ribbon.");
        }

        /*if(nds_ribbons.find(ribbon) != nds_ribbons.end()) {
            _set_ribbon<uint32_t, pksav_gen3_ribbon_mask_t>(
                ribbon,
                value,
                &_blockD->ribbons_obedience,
                nds_ribbons
            );
        } else {
            _set_contest_ribbon(
                ribbon,
                value
            );
        }*/
    }

    void pokemon_ndsimpl::set_move(
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

        _blockB->moves[index] = uint8_t(_moves[index].move.get_move_id());
        _blockB->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_ndsimpl::_calculate_stats() {
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
        } else if(not pkmn_EV_in_bounds(value, true)) {
            throw std::out_of_range("Invalid stat.");
        }

        if(stat == "HP") {
            _blockA->ev_hp = pksav_littleendian16(uint16_t(value));
        } else if(stat == "Attack") {
            _blockA->ev_atk = pksav_littleendian16(uint16_t(value));
        } else if(stat == "Defense") {
            _blockA->ev_def = pksav_littleendian16(uint16_t(value));
        } else if(stat == "Speed") {
            _blockA->ev_spd = pksav_littleendian16(uint16_t(value));
        } else if(stat == "Special Attack") {
            _blockA->ev_spatk = pksav_littleendian16(uint16_t(value));
        } else {
            _blockA->ev_spdef = pksav_littleendian16(uint16_t(value));
        }

        _update_EV_map();
        _calculate_stats();
    }

    void pokemon_ndsimpl::set_IV(
        const std::string &stat,
        int value
    ) {
        if(not pkmn_string_is_modern_stat(stat.c_str())) {
            throw std::invalid_argument("Invalid stat.");
        } else if(not pkmn_IV_in_bounds(value, true)) {
            throw std::out_of_range("Invalid stat.");
        }

        pksav_set_IV(
            &_blockB->iv_isegg_isnicknamed,
            pkmn_stats_to_pksav.at(stat),
            uint8_t(value)
        );

        _update_IV_map();
        _calculate_stats();
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
                for(size_t i = 0; i < 4; ++i) {
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

    void pokemon_ndsimpl::_update_markings_map() {
        _markings["Circle"]   = bool(_blockA->markings & PKSAV_MARKING_CIRCLE);
        _markings["Triangle"] = bool(_blockA->markings & PKSAV_MARKING_TRIANGLE);
        _markings["Square"]   = bool(_blockA->markings & PKSAV_MARKING_SQUARE);
        _markings["Heart"]    = bool(_blockA->markings & PKSAV_MARKING_HEART);
    }

    void pokemon_ndsimpl::_update_ribbons_map() {
        /*uint32_t cool_ribbons = _blockD->ribbons_obedience & PKSAV_GEN3_COOL_RIBBONS_MASK;
        _ribbons["Cool"]        = bool(cool_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Cool Super"]  = bool(cool_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Cool Hyper"]  = bool(cool_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Cool Master"] = bool(cool_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        uint32_t beauty_ribbons = _blockD->ribbons_obedience & PKSAV_GEN3_BEAUTY_RIBBONS_MASK;
        beauty_ribbons >>= PKSAV_GEN3_BEAUTY_RIBBONS_OFFSET;
        _ribbons["Beauty"]        = bool(beauty_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Beauty Super"]  = bool(beauty_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Beauty Hyper"]  = bool(beauty_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Beauty Master"] = bool(beauty_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        uint32_t cute_ribbons = _blockD->ribbons_obedience & PKSAV_GEN3_CUTE_RIBBONS_MASK;
        cute_ribbons >>= PKSAV_GEN3_CUTE_RIBBONS_OFFSET;
        _ribbons["Cute"]        = bool(cute_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Cute Super"]  = bool(cute_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Cute Hyper"]  = bool(cute_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Cute Master"] = bool(cute_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        uint32_t smart_ribbons = _blockD->ribbons_obedience & PKSAV_GEN3_SMART_RIBBONS_MASK;
        smart_ribbons >>= PKSAV_GEN3_SMART_RIBBONS_OFFSET;
        _ribbons["Smart"]        = bool(smart_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Smart Super"]  = bool(smart_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Smart Hyper"]  = bool(smart_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Smart Master"] = bool(smart_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        uint32_t tough_ribbons = _blockD->ribbons_obedience & PKSAV_GEN3_TOUGH_RIBBONS_MASK;
        tough_ribbons >>= PKSAV_GEN3_TOUGH_RIBBONS_OFFSET;
        _ribbons["Tough"]        = bool(tough_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_NORMAL);
        _ribbons["Tough Super"]  = bool(tough_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_SUPER);
        _ribbons["Tough Hyper"]  = bool(tough_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_HYPER);
        _ribbons["Tough Master"] = bool(tough_ribbons >= PKSAV_GEN3_CONTEST_RIBBON_MASTER);

        _ribbons["Champion"] = bool(_blockD->ribbons_obedience & PKSAV_GEN3_CHAMPION_RIBBON_MASK);
        _ribbons["Winning"]  = bool(_blockD->ribbons_obedience & PKSAV_GEN3_WINNING_RIBBON_MASK);
        _ribbons["Victory"]  = bool(_blockD->ribbons_obedience & PKSAV_GEN3_VICTORY_RIBBON_MASK);
        _ribbons["Artist"]   = bool(_blockD->ribbons_obedience & PKSAV_GEN3_ARTIST_RIBBON_MASK);
        _ribbons["blockC"]   = bool(_blockD->ribbons_obedience & PKSAV_GEN3_blockC_RIBBON_MASK);
        _ribbons["Marine"]   = bool(_blockD->ribbons_obedience & PKSAV_GEN3_MARINE_RIBBON_MASK);
        _ribbons["Land"]     = bool(_blockD->ribbons_obedience & PKSAV_GEN3_LAND_RIBBON_MASK);
        _ribbons["Sky"]      = bool(_blockD->ribbons_obedience & PKSAV_GEN3_SKY_RIBBON_MASK);
        _ribbons["Country"]  = bool(_blockD->ribbons_obedience & PKSAV_GEN3_COUNTRY_RIBBON_MASK);
        _ribbons["National"] = bool(_blockD->ribbons_obedience & PKSAV_GEN3_NATIONAL_RIBBON_MASK);
        _ribbons["Earth"]    = bool(_blockD->ribbons_obedience & PKSAV_GEN3_EARTH_RIBBON_MASK);
        _ribbons["World"]    = bool(_blockD->ribbons_obedience & PKSAV_GEN3_WORLD_RIBBON_MASK);*/
    }

    void pokemon_ndsimpl::_update_EV_map() {
        _EVs["HP"]              = int(_blockA->ev_hp);
        _EVs["Attack"]          = int(_blockA->ev_atk);
        _EVs["Defense"]         = int(_blockA->ev_def);
        _EVs["Speed"]           = int(_blockA->ev_spd);
        _EVs["Special Attack"]  = int(_blockA->ev_spatk);
        _EVs["Special Defense"] = int(_blockA->ev_spdef);
    }

    void pokemon_ndsimpl::_update_IV_map() {
        uint8_t IV = 0;

        pksav_get_IV(
            &_blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_HP,
            &IV
        );
        _IVs["HP"] = int(IV);

        pksav_get_IV(
            &_blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_ATTACK,
            &IV
        );
        _IVs["Attack"] = int(IV);

        pksav_get_IV(
            &_blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_DEFENSE,
            &IV
        );
        _IVs["Defense"] = int(IV);

        pksav_get_IV(
            &_blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_SPEED,
            &IV
        );
        _IVs["Speed"] = int(IV);

        pksav_get_IV(
            &_blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_SPATK,
            &IV
        );
        _IVs["Special Attack"] = int(IV);

        pksav_get_IV(
            &_blockB->iv_isegg_isnicknamed,
            PKSAV_STAT_SPDEF,
            &IV
        );
        _IVs["Special Defense"] = int(IV);
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
