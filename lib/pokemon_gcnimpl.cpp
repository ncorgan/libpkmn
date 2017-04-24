/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "misc_common.hpp"
#include "pokemon_gcnimpl.hpp"
#include "database/database_common.hpp"
#include "database/id_to_index.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include "types/rng.hpp"

#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/shininess.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/bimap.hpp>

#include <cstring>
#include <ctime>
#include <iostream>
#include <stdexcept>

#define GC_RCAST   reinterpret_cast<LibPkmGC::GC::Pokemon*>(_native_pc)
#define COLO_RCAST reinterpret_cast<LibPkmGC::Colosseum::Pokemon*>(_native_pc)
#define XD_RCAST   reinterpret_cast<LibPkmGC::XD::Pokemon*>(_native_pc)

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;
    BOOST_STATIC_CONSTEXPR int XD = 20;

    BOOST_STATIC_CONSTEXPR int UNOWN_ID = 201;

    typedef boost::bimap<LibPkmGC::Gender, std::string> gender_bimap_t;
    static const gender_bimap_t GENDER_BIMAP = boost::assign::list_of<gender_bimap_t::relation>
        (LibPkmGC::Male, "Male")
        (LibPkmGC::Female, "Female")
        (LibPkmGC::Genderless, "Genderless")
    ;

    pokemon_gcnimpl::pokemon_gcnimpl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        if(_database_entry.get_game_id() == COLOSSEUM) {
            _native_pc = reinterpret_cast<void*>(new LibPkmGC::Colosseum::Pokemon);
        } else {
            _native_pc = reinterpret_cast<void*>(new LibPkmGC::XD::Pokemon);
        }
        _our_pc_mem = true;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        set_level(level);
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }

        _native_party = nullptr;
        _our_party_mem = false;
    }

    pokemon_gcnimpl::pokemon_gcnimpl(
        LibPkmGC::GC::Pokemon* native,
        int game_id
    ): pokemon_impl(
           int(native->species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(native);
        _our_pc_mem = false;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }

        _native_party = nullptr;
        _our_party_mem = false;
    }

    pokemon_gcnimpl::pokemon_gcnimpl(
        const LibPkmGC::Colosseum::Pokemon &native
    ): pokemon_impl(
           int(native.species),
           COLOSSEUM
       )
    {
        _native_pc = reinterpret_cast<void*>(new LibPkmGC::Colosseum::Pokemon(native));
        _our_pc_mem = true;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }

        _native_party = nullptr;
        _our_party_mem = false;
    }

    pokemon_gcnimpl::pokemon_gcnimpl(
        const LibPkmGC::XD::Pokemon &native
    ): pokemon_impl(
           int(native.species),
           XD 
       )
    {
        _native_pc = reinterpret_cast<void*>(new LibPkmGC::XD::Pokemon(native));
        _our_pc_mem = true;

        // Populate abstractions
        _update_held_item();
        _update_ribbons_map();
        _update_EV_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }

        _native_party = nullptr;
        _our_party_mem = false;
    }

    pokemon_gcnimpl::~pokemon_gcnimpl() {
        if(_our_pc_mem) {
            if(_database_entry.get_game_id() == COLOSSEUM) {
                delete COLO_RCAST;
            } else {
                delete XD_RCAST;
            }
        }
    }

    void pokemon_gcnimpl::set_form(
        const std::string &form
    ) {
        _database_entry.set_form(form);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }
    }

    std::string pokemon_gcnimpl::get_nickname() {
        pokemon_scoped_lock lock(this);

        return GC_RCAST->name->toUTF8();
    }

    void pokemon_gcnimpl::set_nickname(
        const std::string &nickname
    ) {
        if(nickname.size() < 1 or nickname.size() > 10) {
            throw std::invalid_argument(
                      "The nickname length must be 1-10."
                  );
        }

        pokemon_scoped_lock lock(this);

        GC_RCAST->name->fromUTF8(nickname.c_str());
    }

    std::string pokemon_gcnimpl::get_gender() {
        pokemon_scoped_lock lock(this);

        return GENDER_BIMAP.left.at(GC_RCAST->getGender());
    }

    void pokemon_gcnimpl::set_gender(
        const std::string &gender
    ) {
        pokemon_scoped_lock lock(this);

        _set_modern_gender(
            &GC_RCAST->PID,
            gender
        );

        // Unown is genderless, so don't bother setting the form.
    }

    bool pokemon_gcnimpl::is_shiny() {
        pokemon_scoped_lock lock(this);

        return GC_RCAST->isShiny();
    }

    void pokemon_gcnimpl::set_shininess(
        bool value
    ) {
        pokemon_scoped_lock lock(this);

        // LibPkmGC stores trainer IDs in halves.
        uint32_t trainer_id = GC_RCAST->TID | (uint32_t(GC_RCAST->SID) << 16);

        _set_modern_shininess(
            &GC_RCAST->PID,
            &trainer_id,
            value
        );

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_personality();
        }
    }

    void pokemon_gcnimpl::set_held_item(
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

        GC_RCAST->heldItem = LibPkmGC::ItemIndex(item.get_item_index());

        _update_held_item();
    }

    std::string pokemon_gcnimpl::get_trainer_name() {
        pokemon_scoped_lock lock(this);

        return GC_RCAST->OTName->toUTF8();
    }

    void pokemon_gcnimpl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() < 1 or trainer_name.size() > 7) {
            throw std::invalid_argument(
                      "The trainer name length must be 1-7."
                  );
        }

        pokemon_scoped_lock lock(this);

        GC_RCAST->OTName->fromUTF8(trainer_name.c_str());
    }

    uint16_t pokemon_gcnimpl::get_trainer_public_id() {
        pokemon_scoped_lock lock(this);

        return GC_RCAST->TID;
    }

    uint16_t pokemon_gcnimpl::get_trainer_secret_id() {
        pokemon_scoped_lock lock(this);

        return GC_RCAST->SID;
    }

    uint32_t pokemon_gcnimpl::get_trainer_id() {
        pokemon_scoped_lock lock(this);

        return uint32_t(GC_RCAST->PID) | (uint32_t(GC_RCAST->SID) << 16);
    }

    void pokemon_gcnimpl::set_trainer_public_id(
        uint16_t public_id
    ) {
        pokemon_scoped_lock lock(this);

        GC_RCAST->TID = public_id;
    }

    void pokemon_gcnimpl::set_trainer_secret_id(
        uint16_t secret_id
    ) {
        pokemon_scoped_lock lock(this);

        GC_RCAST->SID = secret_id;
    }

    void pokemon_gcnimpl::set_trainer_id(
        uint32_t id
    ) {
        pokemon_scoped_lock lock(this);

        GC_RCAST->TID = uint16_t(id & 0xFFFF);
        GC_RCAST->SID = uint16_t(id >> 16);
    }

    std::string pokemon_gcnimpl::get_trainer_gender() {
        pokemon_scoped_lock lock(this);

        return GENDER_BIMAP.left.at(GC_RCAST->OTGender);
    }

    void pokemon_gcnimpl::set_trainer_gender(
        const std::string &gender
    ) {
        pokemon_scoped_lock lock(this);

        if(gender == "Male" or gender == "Female") {
            GC_RCAST->OTGender = GENDER_BIMAP.right.at(gender);
        } else {
            throw std::invalid_argument("gender: valid values \"Male\", \"Female\"");
        }
    }

    int pokemon_gcnimpl::get_friendship() {
        pokemon_scoped_lock lock(this);

        return GC_RCAST->friendship;
    }

    void pokemon_gcnimpl::set_friendship(
        int friendship
    ) {
        if(friendship < 0 or friendship > 255) {
            throw pkmn::range_error("friendship", 0 , 255);
        }

        pokemon_scoped_lock lock(this);

        GC_RCAST->friendship = LibPkmGC::u8(friendship);
    }

    std::string pokemon_gcnimpl::get_ability() {
        pokemon_scoped_lock lock(this);

        std::pair<std::string, std::string> abilities = _database_entry.get_abilities();
        if(abilities.second == "None") {
            return abilities.first;
        } else {
            return pkmn::database::ability_id_to_name(int(GC_RCAST->getAbility()));
        }
    }

    void pokemon_gcnimpl::set_ability(
        const std::string &ability
    ) {
        pokemon_scoped_lock lock(this);

        std::pair<std::string, std::string> abilities = _database_entry.get_abilities();
        if(ability == "None") {
            throw std::invalid_argument("The ability cannot be set to None.");
        } else if(ability == abilities.first) {
            GC_RCAST->setSecondAbilityFlag(false);
        } else if(ability == abilities.second) {
            GC_RCAST->setSecondAbilityFlag(true);
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

    std::string pokemon_gcnimpl::get_ball() {
        pokemon_scoped_lock lock(this);

        return pkmn::database::ball_id_to_name(int(GC_RCAST->ballCaughtWith));
    }

    void pokemon_gcnimpl::set_ball(
        const std::string &ball
    ) {
        // Try and instantiate an item_entry to validate the ball.
        pkmn::database::item_entry item(ball, get_game());

        pokemon_scoped_lock lock(this);

        GC_RCAST->ballCaughtWith = LibPkmGC::ItemIndex(item.get_item_index());
    }


    int pokemon_gcnimpl::get_level_met() {
        pokemon_scoped_lock lock(this);

        return GC_RCAST->levelMet;
    }

    void pokemon_gcnimpl::set_level_met(
        int level
    ) {
        if(level < 0 or level > 100) {
            throw pkmn::range_error("Level caught", 0, 100);
        }

        pokemon_scoped_lock lock(this);

        GC_RCAST->levelMet = LibPkmGC::u8(level);
    }

    std::string pokemon_gcnimpl::get_location_met(
        bool as_egg
    ) {
        if(as_egg) {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation III.");
        } else {
            pokemon_scoped_lock lock(this);

            // TODO: fix size of field in LibPkmGC
            return pkmn::database::location_index_to_name(
                       GC_RCAST->locationCaught,
                       _database_entry.get_game_id()
                   );
        }
    }

    void pokemon_gcnimpl::set_location_met(
        const std::string &location,
        bool as_egg
    ) {
        if(as_egg) {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation III.");
        } else {
            pokemon_scoped_lock lock(this);

            // TODO: fix size of field in LibPkmGC
            GC_RCAST->locationCaught = LibPkmGC::u8(pkmn::database::location_name_to_index(
                                           location,
                                           _database_entry.get_game_id()
                                       ));
        }
    }

    std::string pokemon_gcnimpl::get_original_game() {
        pokemon_scoped_lock lock(this);

        return pkmn::database::game_index_to_name(int(
                   GC_RCAST->version.game
               ));
    }

    void pokemon_gcnimpl::set_original_game(
        const std::string &game
    ) {
        pokemon_scoped_lock lock(this);

        int generation = pkmn::database::game_name_to_generation(game);
        if(generation != 3) {
            throw std::invalid_argument("Game must be from Generation III.");
        }

        GC_RCAST->version.game = LibPkmGC::GameIndex(
                                     pkmn::database::game_name_to_index(game)
                                 );
    }

    uint32_t pokemon_gcnimpl::get_personality() {
        pokemon_scoped_lock lock(this);

        return GC_RCAST->PID;
    }

    void pokemon_gcnimpl::set_personality(
        uint32_t personality
    ) {
        pokemon_scoped_lock lock(this);

        GC_RCAST->PID = personality;

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_personality();
        }
    }

    int pokemon_gcnimpl::get_experience() {
        pokemon_scoped_lock lock(this);

        return int(GC_RCAST->experience);
    }

    void pokemon_gcnimpl::set_experience(
        int experience
    ) {
        int max_experience = _database_entry.get_experience_at_level(100);

        if(experience < 0 or experience > max_experience) {
            throw pkmn::range_error("experience", 0, max_experience);
        }

        pokemon_scoped_lock lock(this);

        GC_RCAST->experience = LibPkmGC::u32(experience);
        GC_RCAST->updateLevelFromExp();

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gcnimpl::get_level() {
        pokemon_scoped_lock lock(this);

        return int(GC_RCAST->partyData.level);
    }

    void pokemon_gcnimpl::set_level(
        int level
    ) {
        if(level < 1 or level > 100) {
            throw pkmn::range_error("level", 1, 100);
        }

        pokemon_scoped_lock lock(this);

        GC_RCAST->partyData.level = LibPkmGC::u8(level);
        GC_RCAST->experience = GC_RCAST->getExpTable()[level];

        _update_stat_map();
    }

    void pokemon_gcnimpl::set_IV(
        const std::string &stat,
        int value
    ) {
        (void)stat;
        (void)value;
    }

    void pokemon_gcnimpl::set_marking(
        const std::string &marking,
        bool value
    ) {
        (void)marking;
        (void)value;
    }

    /*static const std::map<std::string, pksav_gen3_ribbon_mask_t> gba_ribbons = boost::assign::map_list_of
        ("Champion", PKSAV_GEN3_CHAMPION_RIBBON_MASK)
        ("Winning",  PKSAV_GEN3_WINNING_RIBBON_MASK)
        ("Victory",  PKSAV_GEN3_VICTORY_RIBBON_MASK)
        ("Artist",   PKSAV_GEN3_ARTIST_RIBBON_MASK)
        ("Effort",   PKSAV_GEN3_EFFORT_RIBBON_MASK)
        ("Marine",   PKSAV_GEN3_MARINE_RIBBON_MASK)
        ("Land",     PKSAV_GEN3_LAND_RIBBON_MASK)
        ("Sky",      PKSAV_GEN3_SKY_RIBBON_MASK)
        ("Country",  PKSAV_GEN3_COUNTRY_RIBBON_MASK)
        ("National", PKSAV_GEN3_NATIONAL_RIBBON_MASK)
        ("Earth",    PKSAV_GEN3_EARTH_RIBBON_MASK)
        ("World",    PKSAV_GEN3_WORLD_RIBBON_MASK)
    ;

    static const std::map<std::string, pksav_gen3_contest_ribbon_level_t> gba_contest_ribbon_levels = boost::assign::map_list_of
        ("",       PKSAV_GEN3_CONTEST_RIBBON_NONE)
        ("Normal", PKSAV_GEN3_CONTEST_RIBBON_NORMAL)
        ("Super",  PKSAV_GEN3_CONTEST_RIBBON_SUPER)
        ("Hyper",  PKSAV_GEN3_CONTEST_RIBBON_HYPER)
        ("Master", PKSAV_GEN3_CONTEST_RIBBON_MASTER)
    ;

    static const std::map<std::string, pksav_gen3_contest_ribbons_mask_t> gba_contest_ribbon_masks = boost::assign::map_list_of
        ("Cool",   PKSAV_GEN3_COOL_RIBBONS_MASK)
        ("Beauty", PKSAV_GEN3_BEAUTY_RIBBONS_MASK)
        ("Cute",   PKSAV_GEN3_CUTE_RIBBONS_MASK)
        ("Smart",  PKSAV_GEN3_SMART_RIBBONS_MASK)
        ("Tough",  PKSAV_GEN3_TOUGH_RIBBONS_MASK)
    ;

    static const std::map<std::string, pksav_gen3_contest_ribbons_offset_t> gba_contest_ribbon_offsets = boost::assign::map_list_of
        ("Cool",   PKSAV_GEN3_COOL_RIBBONS_OFFSET)
        ("Beauty", PKSAV_GEN3_BEAUTY_RIBBONS_OFFSET)
        ("Cute",   PKSAV_GEN3_CUTE_RIBBONS_OFFSET)
        ("Smart",  PKSAV_GEN3_SMART_RIBBONS_OFFSET)
        ("Tough",  PKSAV_GEN3_TOUGH_RIBBONS_OFFSET)
    ;*/

    void pokemon_gcnimpl::set_ribbon(
        const std::string &ribbon,
        bool value
    ) {
        if(_ribbons.find(ribbon) == _ribbons.end()) {
            throw std::invalid_argument("Invalid ribbon.");
        }

        pokemon_scoped_lock lock(this);

        (void)ribbon;
        (void)value;
    }

    void pokemon_gcnimpl::set_contest_stat(
        const std::string &stat,
        int value
    ) {
        (void)stat;
        (void)value;
    }

    void pokemon_gcnimpl::set_move(
        const std::string &move,
        int index
    ) {
        if(index < 0 or index > 3) {
            throw pkmn::range_error("index", 0, 3);
        }

        pokemon_scoped_lock lock(this);

        (void)move;
        (void)index;
    }

    void pokemon_gcnimpl::set_EV(
        const std::string &stat,
        int value
    ) {
        if(not pkmn_string_is_modern_stat(stat.c_str())) {
            throw std::invalid_argument("Invalid stat.");
        } else if(not pkmn_EV_in_bounds(value, true)) {
            throw pkmn::range_error(stat, 0, 255);
        }

        pokemon_scoped_lock lock(this);

        (void)value;
        if(stat == "HP") {
        } else if(stat == "Attack") {
        } else if(stat == "Defense") {
        } else if(stat == "Speed") {
        } else if(stat == "Special Attack") {
        } else {
        }

        _update_EV_map();
        _populate_party_data();
    }

    void pokemon_gcnimpl::_set_contest_ribbon(
        const std::string &ribbon,
        bool value
    ) {
        (void)ribbon;
        (void)value;

        _update_ribbons_map();
    }

    void pokemon_gcnimpl::_populate_party_data() {
        GC_RCAST->resetPartyData();
        _update_stat_map();
    }

    void pokemon_gcnimpl::_update_moves(
        int index
    ) {
        _moves.resize(4);
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                /*_moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        pksav_littleendian16(_attacks->moves[index]),
                        _database_entry.get_game_id()
                    ),
                    _attacks->move_pps[index]
                );*/
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gcnimpl::_update_held_item() {
    }

    void pokemon_gcnimpl::_update_markings_map() {
        _markings["Circle"]   = GC_RCAST->markings.circle;
        _markings["Triangle"] = GC_RCAST->markings.triangle;
        _markings["Square"]   = GC_RCAST->markings.square;
        _markings["Heart"]    = GC_RCAST->markings.heart;
    }

    void pokemon_gcnimpl::_update_ribbons_map() {
    }

    void pokemon_gcnimpl::_update_EV_map() {
        /*_EVs["HP"]              = int(_effort->ev_hp);
        _EVs["Attack"]          = int(_effort->ev_atk);
        _EVs["Defense"]         = int(_effort->ev_def);
        _EVs["Speed"]           = int(_effort->ev_spd);
        _EVs["Special Attack"]  = int(_effort->ev_spatk);
        _EVs["Special Defense"] = int(_effort->ev_spdef);*/
    }

    void pokemon_gcnimpl::_update_stat_map() {
        /*_stats["HP"]              = int(pksav_littleendian16(GBA_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_littleendian16(GBA_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_littleendian16(GBA_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_littleendian16(GBA_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_littleendian16(GBA_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_littleendian16(GBA_PARTY_RCAST->spdef));*/
    }

    void pokemon_gcnimpl::_set_unown_form_from_personality() {
        _database_entry.set_form(
            pkmn::calculations::gen3_unown_form(
                GC_RCAST->PID
            )
        );
    }

    void pokemon_gcnimpl::_set_unown_personality_from_form() {
        char as_char = _database_entry.get_form()[0];
        uint8_t num = 0;

        switch(as_char) {
            case '?':
                num = 26;
                break;

            case '!':
                num = 27;
                break;

            // We can assume the form is valid at this point.
            default:
                num = uint8_t(as_char - 'A');
                break;
        }

        uint8_t* pid_as_bytes = reinterpret_cast<uint8_t*>(&GC_RCAST->PID);
        for(size_t i = 0; i < 4; ++i) {
            pid_as_bytes[i] &= ~0x3;
            pid_as_bytes[i] |= ((num & (0x3 << (2*i))) >> (2*i));
        }
    }
}
