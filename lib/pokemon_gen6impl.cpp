/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "misc_common.hpp"
#include "pokemon_gen6impl.hpp"
#include "pokemon_gcnimpl.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include "types/datetime_internal.hpp"
#include "types/rng.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/shininess.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/utils/paths.hpp>

#include <pkmn/database/item_entry.hpp>

#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gen6/ribbons.h>
#include <pksav/gen6/text.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <ctime>
#include <fstream>
#include <stdexcept>

#define GEN6_PC_RCAST    (reinterpret_cast<pksav_gen6_pc_pokemon_t*>(_native_pc))
#define GEN6_PARTY_RCAST (reinterpret_cast<pksav_gen6_pokemon_party_data_t*>(_native_party))

namespace fs = boost::filesystem;

namespace pkmn
{
    pokemon_gen6impl::pokemon_gen6impl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gen6_pc_pokemon_t);
        std::memset(_native_pc, 0, sizeof(pksav_gen6_pc_pokemon_t));
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen6_pokemon_party_data_t);
        std::memset(_native_party, 0, sizeof(pksav_gen6_pokemon_party_data_t));
        _our_party_mem = true;

        (void)level;

        // Set block pointers.
        _blockA_ptr = &GEN6_PC_RCAST->blocks.blockA;
        _blockB_ptr = &GEN6_PC_RCAST->blocks.blockB;
        _blockC_ptr = &GEN6_PC_RCAST->blocks.blockC;
        _blockD_ptr = &GEN6_PC_RCAST->blocks.blockD;

        // Populate abstractions.
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_blockB_ptr->iv_isegg_isnicknamed);
        _init_contest_stat_map(&_blockA_ptr->contest_stats);
        _init_markings_map(&_blockA_ptr->markings);
        _populate_party_data();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen6impl::pokemon_gen6impl(
        pksav_gen6_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(pc->blocks.blockA.species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_gen6_pokemon_party_data_t);
        _our_party_mem = true;

        // Set block pointers.
        _blockA_ptr = &GEN6_PC_RCAST->blocks.blockA;
        _blockB_ptr = &GEN6_PC_RCAST->blocks.blockB;
        _blockC_ptr = &GEN6_PC_RCAST->blocks.blockC;
        _blockD_ptr = &GEN6_PC_RCAST->blocks.blockD;

        // Populate abstractions.
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_blockB_ptr->iv_isegg_isnicknamed);
        _init_contest_stat_map(&_blockA_ptr->contest_stats);
        _init_markings_map(&_blockA_ptr->markings);
        _populate_party_data();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen6impl::pokemon_gen6impl(
        pksav_gen6_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(party->pc.blocks.blockA.species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Set block pointers.
        _blockA_ptr = &GEN6_PC_RCAST->blocks.blockA;
        _blockB_ptr = &GEN6_PC_RCAST->blocks.blockB;
        _blockC_ptr = &GEN6_PC_RCAST->blocks.blockC;
        _blockD_ptr = &GEN6_PC_RCAST->blocks.blockD;

        // Populate abstractions.
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_blockB_ptr->iv_isegg_isnicknamed);
        _init_contest_stat_map(&_blockA_ptr->contest_stats);
        _init_markings_map(&_blockA_ptr->markings);
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen6impl::pokemon_gen6impl(
        const pksav_gen6_pc_pokemon_t& pc,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(pc.blocks.blockA.species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(new pksav_gen6_pc_pokemon_t);
        *GEN6_PC_RCAST = pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen6_pokemon_party_data_t);
        _our_party_mem = true;

        // Set block pointers.
        _blockA_ptr = &GEN6_PC_RCAST->blocks.blockA;
        _blockB_ptr = &GEN6_PC_RCAST->blocks.blockB;
        _blockC_ptr = &GEN6_PC_RCAST->blocks.blockC;
        _blockD_ptr = &GEN6_PC_RCAST->blocks.blockD;

        // Populate abstractions.
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_blockB_ptr->iv_isegg_isnicknamed);
        _init_contest_stat_map(&_blockA_ptr->contest_stats);
        _init_markings_map(&_blockA_ptr->markings);
        _populate_party_data();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen6impl::pokemon_gen6impl(
        const pksav_gen6_party_pokemon_t& party,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(party.pc.blocks.blockA.species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(new pksav_gen6_pc_pokemon_t);
        *GEN6_PC_RCAST = party.pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen6_pokemon_party_data_t);
        *GEN6_PARTY_RCAST = party.party_data;
        _our_party_mem = true;

        // Set block pointers.
        _blockA_ptr = &GEN6_PC_RCAST->blocks.blockA;
        _blockB_ptr = &GEN6_PC_RCAST->blocks.blockB;
        _blockC_ptr = &GEN6_PC_RCAST->blocks.blockC;
        _blockD_ptr = &GEN6_PC_RCAST->blocks.blockD;

        // Populate abstractions.
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_blockB_ptr->iv_isegg_isnicknamed);
        _init_contest_stat_map(&_blockA_ptr->contest_stats);
        _init_markings_map(&_blockA_ptr->markings);
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen6impl::~pokemon_gen6impl()
    {
        if(_our_pc_mem)
        {
            delete GEN6_PC_RCAST;
        }
        if(_our_party_mem)
        {
            delete GEN6_PARTY_RCAST;
        }
    }

    pokemon::sptr pokemon_gen6impl::to_game(
        const std::string& game
    )
    {
        (void)game;

        throw pkmn::unimplemented_error();

        return pkmn::pokemon::sptr();
    }

    void pokemon_gen6impl::export_to_file(
        const std::string& filepath
    )
    {
        std::string extension = fs::extension(filepath);
        if(extension == ".pk6")
        {
            boost::lock_guard<pokemon_gen6impl> lock(*this);

            std::ofstream ofile(filepath, std::ios::binary);
            ofile.write(static_cast<const char*>(get_native_pc_data()), sizeof(pksav_gen6_pc_pokemon_t));
            ofile.close();
        }
        else
        {
            throw std::invalid_argument("Generation VI Pokémon can only be saved to .pk6 files.");
        }
    }

    void pokemon_gen6impl::set_form(
        const std::string& form
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _database_entry.set_form(form);
    }

    bool pokemon_gen6impl::is_egg()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return false;
    }

    void pokemon_gen6impl::set_is_egg(
        bool is_egg
    )
    {
        (void)is_egg; 

        boost::lock_guard<pokemon_gen6impl> lock(*this);
    }

    std::string pokemon_gen6impl::get_condition()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        std::string ret = "None";

        return ret;
    }

    void pokemon_gen6impl::set_condition(
        const std::string& condition
    )
    {
        (void)condition;
    }

    std::string pokemon_gen6impl::get_nickname()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        char nickname[13] = {0};

        PKSAV_CALL(
            pksav_text_from_gen6(
                _blockB_ptr->nickname,
                nickname,
                sizeof(nickname)-1
            );
        )

        return std::string(nickname);
    }

    void pokemon_gen6impl::set_nickname(
        const std::string& nickname
    )
    {
        pkmn::enforce_string_length(
            "Nickname",
            nickname,
            1,
            12
        );

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        PKSAV_CALL(
            pksav_text_to_gen6(
                nickname.c_str(),
                _blockB_ptr->nickname,
                nickname.size()
            );
        )
    }

    std::string pokemon_gen6impl::get_gender()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return pkmn::calculations::modern_pokemon_gender(
                   _database_entry.get_name(),
                   pksav_littleendian32(_blockA_ptr->personality)
               );
    }

    void pokemon_gen6impl::set_gender(
        const std::string& gender
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _set_modern_gender(
            &_blockA_ptr->personality,
            gender
        );
    }

    bool pokemon_gen6impl::is_shiny()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return pkmn::calculations::modern_shiny(
                   pksav_littleendian32(_blockA_ptr->personality),
                   pksav_littleendian32(_blockA_ptr->ot_id.id)
               );
    }

    void pokemon_gen6impl::set_shininess(
        bool value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _set_modern_shininess(
            &_blockA_ptr->personality,
            value
        );
    }

    std::string pokemon_gen6impl::get_held_item()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return pkmn::database::item_index_to_name(
                   pksav_littleendian16(_blockA_ptr->held_item),
                   _database_entry.get_game_id()
               );
    }

    void pokemon_gen6impl::set_held_item(
        const std::string& held_item
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

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _blockA_ptr->held_item = pksav_littleendian16(uint16_t(item.get_item_index()));
    }

    std::string pokemon_gen6impl::get_trainer_name()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        char trainer_name[13] = {0};

        PKSAV_CALL(
            pksav_text_from_gen6(
                _blockD_ptr->otname,
                trainer_name,
                sizeof(trainer_name)-1
            );
        )

        return std::string(trainer_name);
    }

    void pokemon_gen6impl::set_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            12
        );

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        PKSAV_CALL(
            pksav_text_to_gen6(
                trainer_name.c_str(),
                _blockD_ptr->otname,
                trainer_name.size()
            );
        )
    }

    uint16_t pokemon_gen6impl::get_trainer_public_id()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return pksav_littleendian16(_blockA_ptr->ot_id.pid);
    }

    uint16_t pokemon_gen6impl::get_trainer_secret_id()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return pksav_littleendian16(_blockA_ptr->ot_id.sid);
    }

    uint32_t pokemon_gen6impl::get_trainer_id()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return pksav_littleendian32(_blockA_ptr->ot_id.id);
    }

    void pokemon_gen6impl::set_trainer_public_id(
        uint16_t public_id
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _blockA_ptr->ot_id.pid = pksav_littleendian16(public_id);
    }

    void pokemon_gen6impl::set_trainer_secret_id(
        uint16_t secret_id
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _blockA_ptr->ot_id.sid = pksav_littleendian16(secret_id);
    }

    void pokemon_gen6impl::set_trainer_id(
        uint32_t id
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _blockA_ptr->ot_id.id = pksav_littleendian32(id);
    }

    std::string pokemon_gen6impl::get_trainer_gender()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return "";
    }

    void pokemon_gen6impl::set_trainer_gender(
        const std::string& gender
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)gender;
    }

    pkmn::datetime pokemon_gen6impl::get_date_met(
        bool as_egg
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        pkmn::datetime ret;

        if(as_egg)
        {
            ret = pkmn::pksav_date_to_libpkmn_datetime(&_blockD_ptr->eggmet_date);
        }
        else
        {
            ret = pkmn::pksav_date_to_libpkmn_datetime(&_blockD_ptr->met_date);
        }

        return ret;
    }

    void pokemon_gen6impl::set_date_met(
        const pkmn::datetime &date,
        bool as_egg
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        if(as_egg)
        {
            pkmn::libpkmn_datetime_to_pksav_date(
                date,
                &_blockD_ptr->eggmet_date
            );
        }
        else
        {
            pkmn::libpkmn_datetime_to_pksav_date(
                date,
                &_blockD_ptr->met_date
            );
        }
    }

    int pokemon_gen6impl::get_friendship()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    void pokemon_gen6impl::set_friendship(
        int friendship
    )
    {
        pkmn::enforce_bounds("Friendship", friendship, 0, 255);

        boost::lock_guard<pokemon_gen6impl> lock(*this);
    }

    static const std::vector<std::string> NATURES =
    {
        "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
        "Bold", "Docile", "Relaxed", "Impish", "Lax",
        "Timid", "Hasty", "Serious", "Jolly", "Naive",
        "Modest", "Mild", "Quiet", "Bashful", "Rash",
        "Calm", "Gentle", "Sassy", "Careful", "Quirky"
    };

    std::string pokemon_gen6impl::get_nature()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return NATURES.at(_blockA_ptr->nature);
    }

    void pokemon_gen6impl::set_nature(
        const std::string &nature
    )
    {
        pkmn::enforce_value_in_vector(
            "Nature",
            nature,
            NATURES
        );

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _blockA_ptr->nature = uint8_t(std::distance(
                                          NATURES.begin(),
                                          std::find(
                                              NATURES.begin(),
                                              NATURES.end(),
                                              nature
                                          )
                                     ));

        // Update the personality to match.
        _set_nature(
            &_blockA_ptr->personality,
            nature
        );
    }

    std::string pokemon_gen6impl::get_ability()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return pkmn::database::ability_id_to_name(
                   _blockA_ptr->ability
               );
    }

    void pokemon_gen6impl::set_ability(
        const std::string& ability
    )
    {
        if(ability == "None")
        {
            throw std::invalid_argument("Invalid ability.");
        }

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        std::pair<std::string, std::string> abilities = _database_entry.get_abilities();
        std::string hidden_ability = _database_entry.get_hidden_ability();

        if(ability == abilities.first)
        {
            _blockA_ptr->ability = uint8_t(pkmn::database::ability_name_to_id(ability));
            _blockA_ptr->ability_num = 0;
        }
        else if(ability == abilities.second)
        {
            _blockA_ptr->ability = uint8_t(pkmn::database::ability_name_to_id(ability));
            _blockA_ptr->ability_num = 1;
        }
        else if(ability == hidden_ability)
        {
            _blockA_ptr->ability = uint8_t(pkmn::database::ability_name_to_id(ability));
            _blockA_ptr->ability_num = 0;
        }
        else
        {
            // TODO: smarter error message
            throw std::invalid_argument("Invalid ability.");
        }
    }

    std::string pokemon_gen6impl::get_ball()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return pkmn::database::ball_id_to_name(
                   _blockD_ptr->ball
               );
    }

    void pokemon_gen6impl::set_ball(
        const std::string& ball
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        // Try and instantiate an item_entry to validate the ball.
        (void)pkmn::database::item_entry(ball, get_game());

        _blockD_ptr->ball = uint8_t(pkmn::database::ball_name_to_id(ball));
    }


    int pokemon_gen6impl::get_level_met()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    void pokemon_gen6impl::set_level_met(
        int level
    )
    {
        pkmn::enforce_bounds("Level met", level, 0, 100);
    }

    std::string pokemon_gen6impl::get_location_met(
        bool as_egg
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        std::string ret;
        int game_id = _database_entry.get_game_id();

        if(as_egg)
        {
            ret = pkmn::database::location_index_to_name(
                      pksav_littleendian16(_blockD_ptr->eggmet_location),
                      game_id
                  );
        }
        else
        {
            ret = pkmn::database::location_index_to_name(
                      pksav_littleendian16(_blockD_ptr->met_location),
                      game_id
                  );
        }

        return ret;
    }

    void pokemon_gen6impl::set_location_met(
        const std::string& location,
        bool as_egg
    )
    {
        // Validate location.
        uint16_t location_index = pksav_littleendian16(
                                      uint16_t(pkmn::database::location_name_to_index(
                                                   location,
                                                   _database_entry.get_game_id()
                                               ))
                                  );

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        if(as_egg)
        {
            _blockD_ptr->eggmet_location = location_index;
        }
        else
        {
            _blockD_ptr->met_location = location_index;
        }
    }

    std::string pokemon_gen6impl::get_original_game()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        std::string ret;

        // Colosseum and XD are stored with the same index.
        if(_blockD_ptr->ot_game == 15)
        {
            ret = "Colosseum/XD";
        }
        else
        {
            ret = pkmn::database::game_index_to_name(_blockD_ptr->ot_game);
        }

        return ret;
    }

    void pokemon_gen6impl::set_original_game(
        const std::string& game
    )
    {
        std::string internal_game;
        if(game == "Colosseum/XD")
        {
            internal_game = "Colosseum";
        }
        else
        {
            internal_game = game;
        }

        int generation = pkmn::database::game_name_to_generation(internal_game);
        if((generation < 3) or (generation > _generation))
        {
            throw std::invalid_argument("Invalid game.");
        }

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _blockD_ptr->ot_game = uint8_t(pkmn::database::game_name_to_index(internal_game));
    }

    uint32_t pokemon_gen6impl::get_personality()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return pksav_littleendian32(_blockA_ptr->personality);
    }

    void pokemon_gen6impl::set_personality(
        uint32_t personality
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _blockA_ptr->personality = pksav_littleendian32(personality);
    }

    int pokemon_gen6impl::get_experience()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return int(pksav_littleendian32(_blockA_ptr->exp));
    }

    void pokemon_gen6impl::set_experience(
        int experience
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        int max_experience = _database_entry.get_experience_at_level(100);
        pkmn::enforce_bounds("Experience", experience, 0, max_experience);

        _blockA_ptr->exp = pksav_littleendian32(uint32_t(experience));
        GEN6_PARTY_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gen6impl::get_level()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return GEN6_PARTY_RCAST->level;
    }

    void pokemon_gen6impl::set_level(
        int level
    )
    {
        pkmn::enforce_bounds("Level", level, 1, 100);

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        GEN6_PARTY_RCAST->level = uint8_t(level);
        _blockA_ptr->exp = pksav_littleendian32(uint32_t(
                               _database_entry.get_experience_at_level(level)
                           ));

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_gen6impl::set_IV(
        const std::string& stat,
        int value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _set_modern_IV(
            stat,
            value,
            &_blockB_ptr->iv_isegg_isnicknamed
        );
    }

    void pokemon_gen6impl::set_marking(
        const std::string& marking,
        bool value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _set_marking(
            marking,
            value,
            &_blockA_ptr->markings
        );
    }

    void pokemon_gen6impl::set_ribbon(
        const std::string& ribbon,
        bool value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        if(_ribbons.find(ribbon) == _ribbons.end())
        {
            throw std::invalid_argument("Invalid ribbon.");
        }

        if(pksav::GEN6_RIBBONS1.left.count(ribbon) > 0)
        {
            _set_ribbon(
                ribbon,
                value,
                &_blockA_ptr->ribbons[0],
                pksav::GEN6_RIBBONS1.left
            );
        }
        else if(pksav::GEN6_RIBBONS2.left.count(ribbon) > 0)
        {
            _set_ribbon(
                ribbon,
                value,
                &_blockA_ptr->ribbons[1],
                pksav::GEN6_RIBBONS2.left
            );
        }
        else if(pksav::GEN6_RIBBONS3.left.count(ribbon) > 0)
        {
            _set_ribbon(
                ribbon,
                value,
                &_blockA_ptr->ribbons[2],
                pksav::GEN6_RIBBONS3.left
            );
        }
        else if(pksav::GEN6_RIBBONS4.left.count(ribbon) > 0)
        {
            _set_ribbon(
                ribbon,
                value,
                &_blockA_ptr->ribbons[3],
                pksav::GEN6_RIBBONS4.left
            );
        }
        else if(pksav::GEN6_RIBBONS5.left.count(ribbon) > 0)
        {
            _set_ribbon(
                ribbon,
                value,
                &_blockA_ptr->ribbons[4],
                pksav::GEN6_RIBBONS5.left
            );
        }
        else if(pksav::GEN6_RIBBONS6.left.count(ribbon) > 0)
        {
            _set_ribbon(
                ribbon,
                value,
                &_blockA_ptr->ribbons[5],
                pksav::GEN6_RIBBONS6.left
            );
        }
    }

    void pokemon_gen6impl::set_contest_stat(
        const std::string& stat,
        int value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _set_contest_stat(
            stat,
            value,
            &_blockA_ptr->contest_stats
        );
    }

    void pokemon_gen6impl::set_move(
        const std::string& move,
        int index
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        // This will throw an error if the move is invalid.
        pkmn::database::move_entry entry(
            move,
            get_game()
        );

        _moves[index].move = entry.get_name();
        _moves[index].pp   = entry.get_pp(0);

        _blockB_ptr->moves[index] = pksav_littleendian16(uint16_t(entry.get_move_id()));
        _blockB_ptr->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gen6impl::set_EV(
        const std::string& stat,
        int value
    )
    {
        pkmn::enforce_value_in_vector(
            "Stat",
            stat,
            pkmn::MODERN_STATS
        );
        pkmn::enforce_EV_bounds(stat, value, true);

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        if(stat == "HP")
        {
            _blockA_ptr->ev_hp = uint8_t(value);
        }
        else if(stat == "Attack")
        {
            _blockA_ptr->ev_atk = uint8_t(value);
        }
        else if(stat == "Defense")
        {
            _blockA_ptr->ev_def = uint8_t(value);
        }
        else if(stat == "Speed")
        {
            _blockA_ptr->ev_spd = uint8_t(value);
        }
        else if(stat == "Special Attack")
        {
            _blockA_ptr->ev_spatk = uint8_t(value);
        }
        else
        {
            _blockA_ptr->ev_spdef = uint8_t(value);
        }

        _update_EV_map();
        _populate_party_data();
    }

    int pokemon_gen6impl::get_current_hp()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return int(pksav_littleendian16(GEN6_PARTY_RCAST->current_hp));
    }

    void pokemon_gen6impl::set_current_hp(
        int hp
    )
    {
        pkmn::enforce_bounds(
            "Current HP",
            hp,
            0,
            _stats["HP"]
        );

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        GEN6_PARTY_RCAST->current_hp = pksav_littleendian16(uint16_t(hp));
    }

    void pokemon_gen6impl::_populate_party_data()
    {
        pksav::gen6_pc_pokemon_to_party_data(
            _database_entry,
            GEN6_PC_RCAST,
            GEN6_PARTY_RCAST
        );

        _update_stat_map();
    }

    void pokemon_gen6impl::_update_moves(
        int index
    )
    {
        _moves.resize(4);
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        pksav_littleendian16(_blockB_ptr->moves[index]),
                        _database_entry.get_game_id()
                    ).get_name(),
                    pksav_littleendian16(_blockB_ptr->move_pps[index])
                );
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gen6impl::_update_markings_map()
    {
        _markings["Circle"]   = bool(_blockA_ptr->markings & PKSAV_MARKING_CIRCLE);
        _markings["Triangle"] = bool(_blockA_ptr->markings & PKSAV_MARKING_TRIANGLE);
        _markings["Square"]   = bool(_blockA_ptr->markings & PKSAV_MARKING_SQUARE);
        _markings["Heart"]    = bool(_blockA_ptr->markings & PKSAV_MARKING_HEART);
        _markings["Star"]     = bool(_blockA_ptr->markings & PKSAV_MARKING_STAR);
        _markings["Diamond"]  = bool(_blockA_ptr->markings & PKSAV_MARKING_DIAMOND);
    }

    void pokemon_gen6impl::_update_ribbons_map()
    {
        for(const auto& ribbon1_iter: pksav::GEN6_RIBBONS1.left)
        {
            _ribbons[ribbon1_iter.first] = bool(_blockA_ptr->ribbons[0] & ribbon1_iter.second);
        }
        for(const auto& ribbon2_iter: pksav::GEN6_RIBBONS2.left)
        {
            _ribbons[ribbon2_iter.first] = bool(_blockA_ptr->ribbons[1] & ribbon2_iter.second);
        }
        for(const auto& ribbon3_iter: pksav::GEN6_RIBBONS3.left)
        {
            _ribbons[ribbon3_iter.first] = bool(_blockA_ptr->ribbons[2] & ribbon3_iter.second);
        }
        for(const auto& ribbon4_iter: pksav::GEN6_RIBBONS4.left)
        {
            _ribbons[ribbon4_iter.first] = bool(_blockA_ptr->ribbons[3] & ribbon4_iter.second);
        }
        for(const auto& ribbon5_iter: pksav::GEN6_RIBBONS5.left)
        {
            _ribbons[ribbon5_iter.first] = bool(_blockA_ptr->ribbons[4] & ribbon5_iter.second);
        }
        for(const auto& ribbon6_iter: pksav::GEN6_RIBBONS6.left)
        {
            _ribbons[ribbon6_iter.first] = bool(_blockA_ptr->ribbons[5] & ribbon6_iter.second);
        }
    }

    void pokemon_gen6impl::_update_EV_map()
    {
        _EVs["HP"]              = int(_blockA_ptr->ev_hp);
        _EVs["Attack"]          = int(_blockA_ptr->ev_atk);
        _EVs["Defense"]         = int(_blockA_ptr->ev_def);
        _EVs["Speed"]           = int(_blockA_ptr->ev_spd);
        _EVs["Special Attack"]  = int(_blockA_ptr->ev_spatk);
        _EVs["Special Defense"] = int(_blockA_ptr->ev_spdef);
    }

    void pokemon_gen6impl::_update_stat_map()
    {
        _stats["HP"]              = int(pksav_littleendian16(GEN6_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_littleendian16(GEN6_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_littleendian16(GEN6_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_littleendian16(GEN6_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_littleendian16(GEN6_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_littleendian16(GEN6_PARTY_RCAST->spdef));
    }
}
