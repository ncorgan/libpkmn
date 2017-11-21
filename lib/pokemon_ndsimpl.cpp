/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "misc_common.hpp"
#include "pokemon_ndsimpl.hpp"
#include "database/database_common.hpp"
#include "database/id_to_index.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"
#include "types/datetime_internal.hpp"
#include "types/rng.hpp"

#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/nature.hpp>
#include <pkmn/calculations/shininess.hpp>
#include <pkmn/database/item_entry.hpp>

#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include <pksav/common/nds_ribbons.h>
#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gen4/text.h>
#include <pksav/gen5/text.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <ctime>
#include <iostream>
#include <random>
#include <stdexcept>

#define NDS_PC_RCAST    (reinterpret_cast<pksav_nds_pc_pokemon_t*>(_native_pc))
#define NDS_PARTY_RCAST (reinterpret_cast<pksav_nds_pokemon_party_data_t*>(_native_party))

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

        pkmn::rng<uint8_t> rng8;
        pkmn::rng<uint32_t> rng32;
        pkmn::datetime now = pkmn::current_datetime();

        // Set _block pointers
        _blockA = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC = &NDS_PC_RCAST->blocks.blockC;
        _blockD = &NDS_PC_RCAST->blocks.blockD;

        _set_default_nickname();

        // Populate fields
        set_personality(rng32.rand()); // Will set other fields
        NDS_PC_RCAST->isdecrypted_isegg |= PKSAV_NDS_PC_DATA_DECRYPTED_MASK;
        NDS_PC_RCAST->isdecrypted_isegg |= PKSAV_NDS_PARTY_DATA_DECRYPTED_MASK;

        _blockA->species = pksav_littleendian16(uint16_t(
                               _database_entry.get_pokemon_index()
                           ));
        _blockA->ot_id.id = pksav_littleendian32(
                                 pkmn::pokemon::DEFAULT_TRAINER_ID
                            );
        _blockA->friendship = uint8_t(_database_entry.get_base_friendship());
        // TODO: country
        _blockA->ev_hp = rng8.rand();
        _blockA->ev_atk = rng8.rand();
        _blockA->ev_def = rng8.rand();
        _blockA->ev_spd = rng8.rand();
        _blockA->ev_spatk = rng8.rand();
        _blockA->ev_spdef = rng8.rand();

        _blockB->iv_isegg_isnicknamed = rng32.rand();
        _blockB->iv_isegg_isnicknamed &= ~PKSAV_NDS_ISEGG_MASK;
        _blockB->iv_isegg_isnicknamed &= ~PKSAV_NDS_ISNICKNAMED_MASK;

        set_nickname(_default_nickname);
        set_original_game(_database_entry.get_game());
        set_trainer_name(pkmn::pokemon::DEFAULT_TRAINER_NAME);
        // TODO: Pokerus
        set_ball("Premier Ball");
        set_level_met(level);
        set_location_met("Faraway place", false);
        set_location_met("Faraway place", true);
        set_date_met(now, false);
        set_date_met(now, true);

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

        // Set block pointers
        _blockA = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC = &NDS_PC_RCAST->blocks.blockC;
        _blockD = &NDS_PC_RCAST->blocks.blockD;

        _set_default_nickname();

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
        _blockA = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC = &NDS_PC_RCAST->blocks.blockC;
        _blockD = &NDS_PC_RCAST->blocks.blockD;

        _set_default_nickname();

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

        _set_default_nickname();

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
        const pksav_nds_party_pokemon_t& party,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(party.pc.blocks.blockA.species),
           game_id
       ),
       _gen4(game_id >= DIAMOND and game_id <= SOULSILVER),
       _plat(game_id == PLATINUM),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
    {
        _native_pc = reinterpret_cast<void*>(new pksav_nds_pc_pokemon_t);
        *NDS_PC_RCAST = party.pc;
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_nds_pokemon_party_data_t);
        *NDS_PARTY_RCAST = party.party_data;
        _our_party_mem = false;

        // Set _block pointers
        _blockA = &NDS_PC_RCAST->blocks.blockA;
        _blockB = &NDS_PC_RCAST->blocks.blockB;
        _blockC = &NDS_PC_RCAST->blocks.blockC;
        _blockD = &NDS_PC_RCAST->blocks.blockD;

        _set_default_nickname();

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

    pokemon_ndsimpl::~pokemon_ndsimpl()
    {
        if(_our_pc_mem)
        {
            delete NDS_PC_RCAST;
        }
        if(_our_party_mem)
        {
            delete NDS_PARTY_RCAST;
        }
    }

    void pokemon_ndsimpl::set_form(
        const std::string &form
    )
    {
        (void)form;
    }

    std::string pokemon_ndsimpl::get_nickname()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        char nickname[11] = {0};
        if(_gen4)
        {
            PKSAV_CALL(
                pksav_text_from_gen4(
                    _blockC->nickname,
                    nickname,
                    10
                );
            )
        }
        else
        {
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
    )
    {
        pkmn::enforce_string_length(
            "Nickname",
            nickname,
            1,
            10
        );

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        if(_gen4)
        {
            PKSAV_CALL(
                pksav_text_to_gen4(
                    nickname.c_str(),
                    _blockC->nickname,
                    10
                );
            )
        }
        else
        {
            PKSAV_CALL(
                pksav_text_to_gen5(
                    nickname.c_str(),
                    _blockC->nickname,
                    10
                );
            )
        }

        if(nickname == _default_nickname)
        {
            _blockB->iv_isegg_isnicknamed |= PKSAV_NDS_ISNICKNAMED_MASK;
        }
        else
        {
            _blockB->iv_isegg_isnicknamed &= ~PKSAV_NDS_ISNICKNAMED_MASK;
        }
    }

    std::string pokemon_ndsimpl::get_gender()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pkmn::calculations::modern_pokemon_gender(
                   _database_entry.get_name(),
                   pksav_littleendian32(NDS_PC_RCAST->personality)
               );
    }

    void pokemon_ndsimpl::set_gender(
        const std::string &gender
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _set_modern_gender(
            &NDS_PC_RCAST->personality,
            gender
        );
    }

    bool pokemon_ndsimpl::is_shiny()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pkmn::calculations::modern_shiny(
                   pksav_littleendian32(NDS_PC_RCAST->personality),
                   pksav_littleendian32(_blockA->ot_id.id)
               );
    }

    void pokemon_ndsimpl::set_shininess(
        bool value
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _set_modern_shininess(
            &NDS_PC_RCAST->personality,
            value
        );
    }

    void pokemon_ndsimpl::set_held_item(
        const std::string &held_item
    )
    {
        // Make sure item is valid and holdable
        pkmn::database::item_entry item(
            held_item,
            get_game()
        );

        if(not item.holdable())
        {
            throw std::invalid_argument("This item is not holdable.");
        }

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _blockA->held_item = pksav_littleendian16(uint16_t(item.get_item_index()));

        _update_held_item();
    }

    std::string pokemon_ndsimpl::get_trainer_name()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        char otname[8] = {0};
        if(_gen4)
        {
            PKSAV_CALL(
                pksav_text_from_gen4(
                    _blockD->otname,
                    otname,
                    7
                );
            )
        }
        else
        {
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

    void pokemon_ndsimpl::set_trainer_name(
        const std::string &trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            7
        );

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        if(_gen4)
        {
            PKSAV_CALL(
                pksav_text_to_gen4(
                    trainer_name.c_str(),
                    _blockD->otname,
                    7
                );
            )
        }
        else
        {
            PKSAV_CALL(
                pksav_text_to_gen5(
                    trainer_name.c_str(),
                    _blockD->otname,
                    7
                );
            )
        }
    }

    uint16_t pokemon_ndsimpl::get_trainer_public_id()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pksav_littleendian16(_blockA->ot_id.pid);
    }

    uint16_t pokemon_ndsimpl::get_trainer_secret_id()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pksav_littleendian16(_blockA->ot_id.sid);
    }

    uint32_t pokemon_ndsimpl::get_trainer_id()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pksav_littleendian32(_blockA->ot_id.id);
    }

    void pokemon_ndsimpl::set_trainer_public_id(
        uint16_t public_id
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _blockA->ot_id.pid = pksav_littleendian16(public_id);
    }

    void pokemon_ndsimpl::set_trainer_secret_id(
        uint16_t secret_id
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _blockA->ot_id.sid = pksav_littleendian16(secret_id);
    }

    void pokemon_ndsimpl::set_trainer_id(
        uint32_t id
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _blockA->ot_id.id = pksav_littleendian32(id);
    }

    std::string pokemon_ndsimpl::get_trainer_gender()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return (_blockD->metlevel_otgender & PKSAV_NDS_OTGENDER_MASK) ? "Female"
                                                                      : "Male";
    }

    void pokemon_ndsimpl::set_trainer_gender(
        const std::string &gender
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        if(gender == "Male")
        {
            _blockD->metlevel_otgender &= ~PKSAV_NDS_OTGENDER_MASK;
        }
        else if(gender == "Female")
        {
            _blockD->metlevel_otgender |= PKSAV_NDS_OTGENDER_MASK;
        }
        else
        {
            throw std::invalid_argument("gender: valid values \"Male\", \"Female\"");
        }
    }

    pkmn::datetime pokemon_ndsimpl::get_date_met(
        bool as_egg
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pkmn::pksav_date_to_libpkmn_datetime(
                   as_egg ? &_blockD->eggmet_date : &_blockD->met_date
               );
    }

    void pokemon_ndsimpl::set_date_met(
        const pkmn::datetime &date,
        bool as_egg
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        pkmn::libpkmn_datetime_to_pksav_date(
            date,
            as_egg ? &_blockD->eggmet_date : &_blockD->met_date
        );
    }

    int pokemon_ndsimpl::get_friendship()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return _blockA->friendship;
    }

    void pokemon_ndsimpl::set_friendship(
        int friendship
    )
    {
        pkmn::enforce_bounds(
            "Friendship",
            friendship,
            0,
            255
        );

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _blockA->friendship = uint8_t(friendship);
    }

    std::string pokemon_ndsimpl::get_nature()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        if(_gen4)
        {
            return pkmn::calculations::nature(
                       pksav_littleendian32(NDS_PC_RCAST->personality)
                   );
        }
        else
        {
            return pkmn::database::nature_index_to_name(_blockB->nature);
        }
    }

    void pokemon_ndsimpl::set_nature(
        const std::string &nature
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _set_nature(
            &NDS_PC_RCAST->personality,
            nature
        );

        if(not _gen4)
        {
            _blockB->nature = uint8_t(pkmn::database::nature_name_to_index(nature));
        }
    }

    std::string pokemon_ndsimpl::get_ability()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pkmn::database::ability_id_to_name(_blockA->ability);
    }

    void pokemon_ndsimpl::set_ability(
        const std::string &ability
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        std::pair<std::string, std::string> abilities = _database_entry.get_abilities();
        std::string hidden_ability = _database_entry.get_hidden_ability();

        if(ability == "None")
        {
            throw std::invalid_argument("The ability cannot be set to None.");
        }
        else if(ability == abilities.first or ability == abilities.second) {
            _blockA->ability = uint8_t(pkmn::database::ability_name_to_id(ability));
        }
        else if(ability == hidden_ability)
        {
            _blockA->ability = uint8_t(pkmn::database::ability_name_to_id(ability));
            // TODO: add hidden ability flag to PKSav, set here
        }
        else
        {
            std::string error_message;
            if(abilities.second == "None")
            {
                error_message = str(boost::format("ability: valid values \"%s\"")
                                    % abilities.first.c_str());
            }
            else
            {
                error_message = str(boost::format("ability: valid values \"%s\", \"%s\"")
                                    % abilities.first.c_str()
                                    % abilities.second.c_str());
            }

            throw std::invalid_argument(error_message.c_str());
        }
    }

    std::string pokemon_ndsimpl::get_ball()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pkmn::database::ball_id_to_name(
                   _hgss ? _blockD->ball_hgss
                         : _blockD->ball
               );
    }

    void pokemon_ndsimpl::set_ball(
        const std::string &ball
    )
    {
        // Try to instantiate an item_entry to validate the ball.
        (void)pkmn::database::item_entry(ball, get_game());

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        (_hgss ? _blockD->ball_hgss : _blockD->ball) = uint8_t(pkmn::database::ball_name_to_id(
                                                                   ball
                                                               ));
    }

    int pokemon_ndsimpl::get_level_met()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return (_blockD->metlevel_otgender & PKSAV_NDS_LEVELMET_MASK);
    }

    void pokemon_ndsimpl::set_level_met(
        int level
    )
    {
        pkmn::enforce_bounds(
            "Level met",
            level,
            0,
            100
        );

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _blockD->metlevel_otgender &= ~PKSAV_NDS_LEVELMET_MASK;
        _blockD->metlevel_otgender |= uint16_t(level);
    }

    std::string pokemon_ndsimpl::get_location_met(
        bool as_egg
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

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
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

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

    std::string pokemon_ndsimpl::get_original_game()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pkmn::database::game_index_to_name(_blockC->hometown);
    }

    void pokemon_ndsimpl::set_original_game(
        const std::string &game
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        int generation = pkmn::database::game_name_to_generation(game);
        if(generation < 3 or generation > _generation) {
            throw std::invalid_argument("Invalid game.");
        }

        _blockC->hometown = uint8_t(pkmn::database::game_name_to_index(game));
    }

    uint32_t pokemon_ndsimpl::get_personality()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return pksav_littleendian32(NDS_PC_RCAST->personality);
    }

    // TODO: automatically update personality-based stuff
    void pokemon_ndsimpl::set_personality(
        uint32_t personality
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        NDS_PC_RCAST->personality = pksav_littleendian32(personality);

        if(not _gen4)
        {
            _blockB->nature = uint8_t(personality % 25);
        }
    }

    int pokemon_ndsimpl::get_experience()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return int(pksav_littleendian32(_blockA->exp));
    }

    void pokemon_ndsimpl::set_experience(
        int experience
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        int max_experience = _database_entry.get_experience_at_level(100);
        pkmn::enforce_bounds(
            "Experience",
            experience,
            0,
            max_experience
        );

        _blockA->exp = pksav_littleendian32(uint32_t(experience));
        NDS_PARTY_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_ndsimpl::get_level() {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        return int(NDS_PARTY_RCAST->level);
    }

    void pokemon_ndsimpl::set_level(
        int level
    )
    {
        pkmn::enforce_bounds(
            "Level",
            level,
            1,
            100
        );

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        NDS_PARTY_RCAST->level = uint8_t(level);
        _blockA->exp = pksav_littleendian32(uint32_t(
                           _database_entry.get_experience_at_level(level)
                       ));

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_ndsimpl::set_IV(
        const std::string &stat,
        int value
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _set_modern_IV(
            stat,
            value,
            &_blockB->iv_isegg_isnicknamed
        );
    }

    void pokemon_ndsimpl::set_marking(
        const std::string &marking,
        bool value
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _set_marking(
            marking,
            value,
            &_blockA->markings
        );
    }

    static const std::vector<std::string> contest_types = boost::assign::list_of
        ("Cool")("Beauty")("Cute")("Smart")("Tough")
    ;
    static const std::vector<std::string> hoenn_contest_levels = boost::assign::list_of
        ("")("Super")("Hyper")("Master")
    ;
    static const std::vector<std::string> sinnoh_contest_levels = boost::assign::list_of
        ("")("Great")("Ultra")("Master")
    ;

    static const std::map<std::string, pksav_nds_hoenn_ribbon_mask_t> hoenn_ribbons = boost::assign::map_list_of
        ("Hoenn Champion",          PKSAV_NDS_HOENN_CHAMPION_RIBBON_MASK)
        ("Hoenn Winning",           PKSAV_NDS_HOENN_WINNING_RIBBON_MASK)
        ("Hoenn Victory",           PKSAV_NDS_HOENN_VICTORY_RIBBON_MASK)
        ("Hoenn Artist",            PKSAV_NDS_HOENN_ARTIST_RIBBON_MASK)
        ("Hoenn Effort",            PKSAV_NDS_HOENN_EFFORT_RIBBON_MASK)
        ("Hoenn Battle Champion",   PKSAV_NDS_HOENN_BATTLE_CHAMPION_RIBBON_MASK)
        ("Hoenn Regional Champion", PKSAV_NDS_HOENN_REGIONAL_CHAMPION_RIBBON_MASK)
        ("Hoenn National Champion", PKSAV_NDS_HOENN_NATIONAL_CHAMPION_RIBBON_MASK)
        ("Hoenn Country",           PKSAV_NDS_HOENN_COUNTRY_RIBBON_MASK)
        ("Hoenn National",          PKSAV_NDS_HOENN_NATIONAL_RIBBON_MASK)
        ("Hoenn Earth",             PKSAV_NDS_HOENN_EARTH_RIBBON_MASK)
        ("Hoenn World",             PKSAV_NDS_HOENN_WORLD_RIBBON_MASK)
    ;

    static const std::map<std::string, pksav_nds_sinnoh_ribbon1_mask_t> sinnoh_ribbons1 = boost::assign::map_list_of
        ("Sinnoh Champ",   PKSAV_NDS_SINNOH_CHAMP_RIBBON_MASK)
        ("Ability",        PKSAV_NDS_SINNOH_ABILITY_RIBBON_MASK)
        ("Great Ability",  PKSAV_NDS_SINNOH_GREAT_ABILITY_RIBBON_MASK)
        ("Double Ability", PKSAV_NDS_SINNOH_DOUBLE_ABILITY_RIBBON_MASK)
        ("Multi Ability",  PKSAV_NDS_SINNOH_MULTI_ABILITY_RIBBON_MASK)
        ("Pair Ability",   PKSAV_NDS_SINNOH_PAIR_ABILITY_RIBBON_MASK)
        ("World Ability",  PKSAV_NDS_SINNOH_WORLD_ABILITY_RIBBON_MASK)
        ("Alert",          PKSAV_NDS_SINNOH_ALERT_RIBBON_MASK)
        ("Shock",          PKSAV_NDS_SINNOH_SHOCK_RIBBON_MASK)
        ("Downcast",       PKSAV_NDS_SINNOH_DOWNCAST_RIBBON_MASK)
        ("Careless",       PKSAV_NDS_SINNOH_CARELESS_RIBBON_MASK)
        ("Relax",          PKSAV_NDS_SINNOH_RELAX_RIBBON_MASK)
        ("Snooze",         PKSAV_NDS_SINNOH_SNOOZE_RIBBON_MASK)
        ("Smile",          PKSAV_NDS_SINNOH_SMILE_RIBBON_MASK)
        ("Gorgeous",       PKSAV_NDS_SINNOH_GORGEOUS_RIBBON_MASK)
        ("Royal",          PKSAV_NDS_SINNOH_ROYAL_RIBBON_MASK)
    ;

    static const std::map<std::string, pksav_nds_sinnoh_ribbon2_mask_t> sinnoh_ribbons2 = boost::assign::map_list_of
        ("Gorgeous Royal", PKSAV_NDS_SINNOH_GORGEOUS_ROYAL_RIBBON_MASK)
        ("Footprint",      PKSAV_NDS_SINNOH_FOOTPRINT_RIBBON_MASK)
        ("Record",         PKSAV_NDS_SINNOH_RECORD_RIBBON_MASK)
        ("History",        PKSAV_NDS_SINNOH_HISTORY_RIBBON_MASK)
        ("Legend",         PKSAV_NDS_SINNOH_LEGEND_RIBBON_MASK)
        ("Red",            PKSAV_NDS_SINNOH_RED_RIBBON_MASK)
        ("Green",          PKSAV_NDS_SINNOH_GREEN_RIBBON_MASK)
        ("Blue",           PKSAV_NDS_SINNOH_BLUE_RIBBON_MASK)
        ("Festival",       PKSAV_NDS_SINNOH_FESTIVAL_RIBBON_MASK)
        ("Carnival",       PKSAV_NDS_SINNOH_CARNIVAL_RIBBON_MASK)
        ("Classic",        PKSAV_NDS_SINNOH_CLASSIC_RIBBON_MASK)
        ("Premier",        PKSAV_NDS_SINNOH_PREMIER_RIBBON_MASK)
    ;

    static const std::map<std::string, pksav_nds_unova_ribbon_mask_t> unova_ribbons = boost::assign::map_list_of
        ("Gorgeous Royal", PKSAV_NDS_UNOVA_GORGEOUS_ROYAL_RIBBON_MASK)
        ("Footprint",      PKSAV_NDS_UNOVA_FOOTPRINT_RIBBON_MASK)
        ("Record",         PKSAV_NDS_UNOVA_RECORD_RIBBON_MASK)
        ("Event",          PKSAV_NDS_UNOVA_EVENT_RIBBON_MASK)
        ("Legend",         PKSAV_NDS_UNOVA_LEGEND_RIBBON_MASK)
        ("World Champion", PKSAV_NDS_UNOVA_WORLD_CHAMPION_RIBBON_MASK)
        ("Birthday",       PKSAV_NDS_UNOVA_BIRTHDAY_RIBBON_MASK)
        ("Special",        PKSAV_NDS_UNOVA_SPECIAL_RIBBON_MASK)
        ("Souvenir",       PKSAV_NDS_UNOVA_SOUVENIR_RIBBON_MASK)
        ("Wishing",        PKSAV_NDS_UNOVA_WISHING_RIBBON_MASK)
        ("Classic",        PKSAV_NDS_UNOVA_CLASSIC_RIBBON_MASK)
        ("Premier",        PKSAV_NDS_UNOVA_PREMIER_RIBBON_MASK)
    ;

    // Assume the ribbon name has been validated at this point.
    void pokemon_ndsimpl::_set_contest_ribbon(
        const std::string &ribbon,
        bool value
    )
    {
        std::vector<std::string> ribbon_parts;
        boost::split(ribbon_parts, ribbon, boost::is_any_of(" "));
        if(ribbon_parts.size() == 0 or ribbon_parts.size() > 3)
        {
            throw std::invalid_argument("Invalid ribbon name.");
        }

        bool hoenn = (ribbon_parts[0] == "Hoenn");
        if(hoenn)
        {
            ribbon_parts.erase(ribbon_parts.begin());
            if(ribbon_parts.size() != 2)
            {
                throw std::invalid_argument("Invalid ribbon name.");
            }
        }

        size_t contest_type_pos = 0;
        std::vector<std::string>::const_iterator contest_type_iter = std::find(contest_types.begin(), contest_types.end(), ribbon_parts[0]);
        if(contest_type_iter != contest_types.end())
        {
            contest_type_pos = size_t(std::distance(contest_types.begin(), contest_type_iter));
        }
        else
        {
            throw std::invalid_argument("Invalid ribbon name.");
        }

        size_t contest_level_pos = 0;
        if(ribbon_parts.size() == 2)
        {
            std::vector<std::string>::const_iterator contest_level_iter;
            if(hoenn)
            {
                contest_level_iter = std::find(hoenn_contest_levels.begin(), hoenn_contest_levels.end(), ribbon_parts[1]);
                contest_level_pos = size_t(std::distance(hoenn_contest_levels.begin(), contest_level_iter));
            }
            else
            {
                contest_level_iter = std::find(sinnoh_contest_levels.begin(), sinnoh_contest_levels.end(), ribbon_parts[1]);
                contest_level_pos = size_t(std::distance(sinnoh_contest_levels.begin(), contest_level_iter));
            }
        }

        uint32_t base_enum = uint32_t(4 * contest_type_pos);
        if(hoenn)
        {
            if(value)
            {
                for(uint32_t i = 0; i <= contest_level_pos; ++i)
                {
                    uint32_t mask = 1 << (base_enum + i);
                    _blockB->hoenn_ribbons |= mask;
                }
            }
            else
            {
                for(uint32_t i = 3; i >= contest_level_pos; ++i)
                {
                    uint32_t mask = 1 << (base_enum + i);
                    _blockB->hoenn_ribbons &= ~mask;
                }
            }
        }
        else
        {
            if(value)
            {
                for(uint32_t i = 0; i <= contest_level_pos; ++i)
                {
                    uint32_t mask = 1 << (base_enum + i);
                    _blockC->sinnoh_ribbons3 |= mask;
                }
            }
            else
            {
                for(uint32_t i = 3; i >= contest_level_pos; ++i)
                {
                    uint32_t mask = 1 << (base_enum + i);
                    _blockC->sinnoh_ribbons3 &= ~mask;
                }
            }
        }
    }

    void pokemon_ndsimpl::set_ribbon(
        const std::string &ribbon,
        bool value
    )
    {
        // TODO
        if(_ribbons.find(ribbon) == _ribbons.end())
        {
            throw std::invalid_argument("Invalid ribbon.");
        }

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        if(hoenn_ribbons.find(ribbon) != hoenn_ribbons.end())
        {
            _set_ribbon<uint32_t, pksav_nds_hoenn_ribbon_mask_t>(
                ribbon,
                value,
                &_blockB->hoenn_ribbons,
                hoenn_ribbons
            );
        }
        else if(sinnoh_ribbons1.find(ribbon) != sinnoh_ribbons1.end())
        {
            _set_ribbon<uint16_t, pksav_nds_sinnoh_ribbon1_mask_t>(
                ribbon,
                value,
                &_blockA->sinnoh_ribbons1,
                sinnoh_ribbons1
            );
        }
        else if(_gen4)
        {
            if(sinnoh_ribbons2.find(ribbon) != sinnoh_ribbons2.end())
            {
                _set_ribbon<uint16_t, pksav_nds_sinnoh_ribbon2_mask_t>(
                    ribbon,
                    value,
                    &_blockA->sinnoh_ribbons2,
                    sinnoh_ribbons2
                );
            }
        }
        else
        {
            if(unova_ribbons.find(ribbon) != unova_ribbons.end())
            {
                _set_ribbon<uint16_t, pksav_nds_unova_ribbon_mask_t>(
                    ribbon,
                    value,
                    &_blockA->unova_ribbons,
                    unova_ribbons
                );
            }
        }

        _update_ribbons_map();
    }

    void pokemon_ndsimpl::set_contest_stat(
        const std::string &stat,
        int value
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _set_contest_stat(
            stat,
            value,
            &_blockA->contest_stats
        );
    }

    void pokemon_ndsimpl::set_move(
        const std::string &move,
        int index
    )
    {
        pkmn::enforce_bounds(
            "Move index",
            index,
            0,
            3
        );

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        // Make sure the move is valid.
        pkmn::database::move_entry entry(move, get_game());

        _moves[index].move = entry.get_name();
        _moves[index].pp   = entry.get_pp(0);

        _blockB->moves[index] = pksav_littleendian16(uint16_t(entry.get_move_id()));
        _blockB->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_ndsimpl::_populate_party_data()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        pksav::nds_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_nds_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_nds_pokemon_party_data_t*>(_native_party)
        );
    }

    void pokemon_ndsimpl::set_EV(
        const std::string &stat,
        int value
    )
    {
        pkmn::enforce_value_in_vector(
            "Stat",
            stat,
            pkmn::MODERN_STATS
        );
        pkmn::enforce_EV_bounds(stat, value, true);

        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        if(stat == "HP")
        {
            _blockA->ev_hp = uint8_t(value);
        }
        else if(stat == "Attack")
        {
            _blockA->ev_atk = uint8_t(value);
        }
        else if(stat == "Defense")
        {
            _blockA->ev_def = uint8_t(value);
        }
        else if(stat == "Speed")
        {
            _blockA->ev_spd = uint8_t(value);
        }
        else if(stat == "Special Attack")
        {
            _blockA->ev_spatk = uint8_t(value);
        }
        else
        {
            _blockA->ev_spdef = uint8_t(value);
        }

        _update_EV_map();
        _populate_party_data();
    }

    void pokemon_ndsimpl::_set_default_nickname()
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _default_nickname = _database_entry.get_name();
        if(_gen4)
        {
            _default_nickname = boost::algorithm::to_upper_copy(
                                    _default_nickname
                                );
        }
    }

    void pokemon_ndsimpl::_update_moves(
        int index
    )
    {
        boost::lock_guard<pokemon_ndsimpl> lock(*this);

        _moves.resize(4);
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        pksav_littleendian16(_blockB->moves[index]),
                        _database_entry.get_game_id()
                    ).get_name(),
                    pksav_littleendian16(_blockB->move_pps[index])
                );
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_ndsimpl::_update_ribbons_map()
    {
        for(auto iter = hoenn_ribbons.begin(); iter != hoenn_ribbons.end(); ++iter)
        {
            _ribbons[iter->first] = (_blockB->hoenn_ribbons & iter->second);
        }
        for(auto iter = sinnoh_ribbons1.begin(); iter != sinnoh_ribbons1.end(); ++iter)
        {
            _ribbons[iter->first] = (_blockA->sinnoh_ribbons1 & iter->second);
        }
        if(_gen4)
        {
            for(auto iter = sinnoh_ribbons2.begin(); iter != sinnoh_ribbons2.end(); ++iter)
            {
                _ribbons[iter->first] = (_blockA->sinnoh_ribbons2 & iter->second);
            }
        }
        else
        {
            for(auto iter = unova_ribbons.begin(); iter != unova_ribbons.end(); ++iter)
            {
                _ribbons[iter->first] = (_blockA->unova_ribbons & iter->second);
            }
        }

        // Contest ribbons
        std::string hoenn_ribbon_name;
        std::string sinnoh_ribbon_name;
        for(uint32_t i = 0; i < contest_types.size(); ++i)
        {
            for(uint32_t j = 0; j < hoenn_contest_levels.size(); ++j)
            {
                hoenn_ribbon_name = "Hoenn " + contest_types[i];
                sinnoh_ribbon_name = contest_types[i];

                if(hoenn_contest_levels[j] != "")
                {
                    hoenn_ribbon_name  += " ";
                    sinnoh_ribbon_name += " ";
                }

                hoenn_ribbon_name  += hoenn_contest_levels[j];
                sinnoh_ribbon_name += sinnoh_contest_levels[j];

                uint32_t mask = 1 << (4 * i + j);
                _ribbons[hoenn_ribbon_name]  = _blockB->hoenn_ribbons & mask;
                _ribbons[sinnoh_ribbon_name] = _blockC->sinnoh_ribbons3 & mask;
            }
        }
    }

    void pokemon_ndsimpl::_update_EV_map()
    {
        _EVs["HP"]              = int(_blockA->ev_hp);
        _EVs["Attack"]          = int(_blockA->ev_atk);
        _EVs["Defense"]         = int(_blockA->ev_def);
        _EVs["Speed"]           = int(_blockA->ev_spd);
        _EVs["Special Attack"]  = int(_blockA->ev_spatk);
        _EVs["Special Defense"] = int(_blockA->ev_spdef);
    }

    void pokemon_ndsimpl::_update_stat_map()
    {
        _stats["HP"]              = int(pksav_littleendian16(NDS_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_littleendian16(NDS_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_littleendian16(NDS_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_littleendian16(NDS_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_littleendian16(NDS_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_littleendian16(NDS_PARTY_RCAST->spdef));
    }
}
