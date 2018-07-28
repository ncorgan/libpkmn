/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "utils/misc.hpp"
#include "pokemon_gbaimpl.hpp"
#include "pokemon_gcnimpl.hpp"

#include "conversions/gen3_conversions.hpp"

#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "libpkmgc_includes.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include "pksav/lib/gba/checksum.h"

#include "types/rng.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/personality.hpp>
#include <pkmn/calculations/shininess.hpp>
#include <pkmn/enums/enum_to_string.hpp>
#include <pkmn/utils/paths.hpp>

#include <pkmn/qt/Spinda.hpp>

#include <pkmn/database/item_entry.hpp>

#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gba/language.h>
#include <pksav/gba/ribbons.h>
#include <pksav/gba/text.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <ctime>
#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn
{
    BOOST_STATIC_CONSTEXPR int MEW_ID    = 151;
    BOOST_STATIC_CONSTEXPR int UNOWN_ID  = 201;
    BOOST_STATIC_CONSTEXPR int DEOXYS_ID = 386;

    pokemon_gbaimpl::pokemon_gbaimpl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        std::memset(&_pksav_pokemon, 0, sizeof(_pksav_pokemon));

        // Set block pointers
        _p_growth_block  = &_pksav_pokemon.pc_data.blocks.growth;
        _p_attacks_block = &_pksav_pokemon.pc_data.blocks.attacks;
        _p_effort_block  = &_pksav_pokemon.pc_data.blocks.effort;
        _p_misc_block    = &_pksav_pokemon.pc_data.blocks.misc;

        _p_native_pc    = &_pksav_pokemon.pc_data;
        _p_native_party = &_pksav_pokemon.party_data;

        pkmn::rng<uint32_t> rng;
        _pksav_pokemon.pc_data.personality = rng.rand();
        _pksav_pokemon.pc_data.ot_id.id = pksav_littleendian32(DEFAULT_TRAINER_ID);

        PKSAV_CALL(
            pksav_gba_export_text(
                boost::algorithm::to_upper_copy(
                    _database_entry.get_species_name()
                ).c_str(),
                _pksav_pokemon.pc_data.nickname,
                PKSAV_GBA_POKEMON_NICKNAME_LENGTH
            );
        )

        set_language(pkmn::e_language::ENGLISH);

        PKSAV_CALL(
            pksav_gba_export_text(
                DEFAULT_TRAINER_NAME.c_str(),
                _pksav_pokemon.pc_data.otname,
                PKSAV_GBA_POKEMON_OTNAME_LENGTH
            );
        )

        _p_growth_block->species = pksav_littleendian16(uint16_t(_database_entry.get_pokemon_index()));
        _p_growth_block->exp = pksav_littleendian32(uint32_t(
                           _database_entry.get_experience_at_level(level)
                       ));
        _p_growth_block->friendship = uint8_t(_database_entry.get_base_friendship());

        set_location_met("Fateful encounter", false);
        set_level_met(level);
        set_original_game(get_game());
        set_ball(pkmn::e_ball::POKE_BALL);

        _p_misc_block->iv_egg_ability = rng.rand();
        _p_misc_block->iv_egg_ability &= ~PKSAV_GBA_POKEMON_EGG_MASK;
        if(_pksav_pokemon.pc_data.personality % 2)
        {
            _p_misc_block->iv_egg_ability |= PKSAV_GBA_POKEMON_ABILITY_MASK;
        }
        else
        {
            _p_misc_block->iv_egg_ability &= ~PKSAV_GBA_POKEMON_ABILITY_MASK;
        }

        // Only do this by default for Mew or Deoxys, who won't obey without
        // this flag. For any others, this will cause them to falsely be
        // flagged as a fateful encounter if traded up to Generation IV.
        if((_database_entry.get_species_id() == MEW_ID) or
           (_database_entry.get_species_id() == DEOXYS_ID))
        {
            _p_misc_block->ribbons_obedience |= PKSAV_GBA_POKEMON_OBEDIENCE_MASK;
        }

        // Populate abstractions
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_p_misc_block->iv_egg_ability);
        _init_contest_stat_map(&_p_effort_block->contest_stats);
        _init_markings_map(&_pksav_pokemon.pc_data.markings);
        set_level(level);
        _init_default_moves_for_level();

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _p_growth_block->species = pksav_littleendian16(uint16_t(
                                   _database_entry.get_pokemon_index()
                               ));

            _set_unown_personality_from_form();
        }

        _register_attributes();
    }

    pokemon_gbaimpl::pokemon_gbaimpl(
        const struct pksav_gba_pc_pokemon* p_pc_pokemon,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(p_pc_pokemon->blocks.growth.species),
           game_id
       )
    {
        BOOST_ASSERT(p_pc_pokemon != nullptr);

        _pksav_pokemon.pc_data = *p_pc_pokemon;
        _populate_party_data();

        // Set block pointers
        _p_growth_block  = &_pksav_pokemon.pc_data.blocks.growth;
        _p_attacks_block = &_pksav_pokemon.pc_data.blocks.attacks;
        _p_effort_block  = &_pksav_pokemon.pc_data.blocks.effort;
        _p_misc_block    = &_pksav_pokemon.pc_data.blocks.misc;

        _p_native_pc    = &_pksav_pokemon.pc_data;
        _p_native_party = &_pksav_pokemon.party_data;

        // Populate abstractions
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_p_misc_block->iv_egg_ability);
        _init_contest_stat_map(&_p_effort_block->contest_stats);
        _init_markings_map(&_pksav_pokemon.pc_data.markings);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_personality_from_form();
        }

        _register_attributes();
    }

    pokemon_gbaimpl::pokemon_gbaimpl(
        const struct pksav_gba_party_pokemon* p_party_pokemon,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(p_party_pokemon->pc_data.blocks.growth.species),
           game_id
       )
    {
        BOOST_ASSERT(p_party_pokemon != nullptr);

        _pksav_pokemon = *p_party_pokemon;

        // Set block pointers
        _p_growth_block  = &_pksav_pokemon.pc_data.blocks.growth;
        _p_attacks_block = &_pksav_pokemon.pc_data.blocks.attacks;
        _p_effort_block  = &_pksav_pokemon.pc_data.blocks.effort;
        _p_misc_block    = &_pksav_pokemon.pc_data.blocks.misc;

        _p_native_pc    = &_pksav_pokemon.pc_data;
        _p_native_party = &_pksav_pokemon.party_data;

        // Populate abstractions
        _update_ribbons_map();
        _update_EV_map();
        _init_modern_IV_map(&_p_misc_block->iv_egg_ability);
        _init_contest_stat_map(&_p_effort_block->contest_stats);
        _init_markings_map(&_pksav_pokemon.pc_data.markings);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_personality_from_form();
        }

        _register_attributes();
    }

    pokemon::sptr pokemon_gbaimpl::to_game(pkmn::e_game game)
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        pkmn::pokemon::sptr ret;

        int game_id = pkmn::database::game_enum_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);
        switch(generation)
        {
            case 3:
                if(game_is_gamecube(game_id))
                {
                    if(game_id == COLOSSEUM_ID)
                    {
                        LibPkmGC::Colosseum::Pokemon colosseum_pokemon;
                        pkmn::conversions::gba_party_pokemon_to_gcn(
                            &_pksav_pokemon,
                            &colosseum_pokemon
                        );
                        ret = std::make_shared<pokemon_gcnimpl>(
                                  &colosseum_pokemon,
                                  game_id
                              );
                    }
                    else
                    {
                        LibPkmGC::XD::Pokemon xd_pokemon;
                        pkmn::conversions::gba_party_pokemon_to_gcn(
                            &_pksav_pokemon,
                            &xd_pokemon
                        );
                        ret = std::make_shared<pokemon_gcnimpl>(
                                  &xd_pokemon,
                                  game_id
                              );
                    }
                }
                else
                {
                    ret = std::make_shared<pokemon_gbaimpl>(&_pksav_pokemon, game_id);
                    ret->set_level_met(get_level());
                    ret->set_original_game(get_game());
                }
                break;

            case 4:
            case 5:
            case 6:
                throw pkmn::unimplemented_error();

            default:
                throw std::invalid_argument("Generation III Pokémon can only be converted to Generation III-VI.");
        }

        return ret;
    }

    void pokemon_gbaimpl::export_to_file(
        const std::string& filepath
    )
    {
        std::string extension = fs::extension(filepath);
        if(extension == ".3gpkm")
        {
            boost::lock_guard<pokemon_gbaimpl> lock(*this);

            std::ofstream ofile(filepath, std::ios::binary);
            ofile.write(static_cast<const char*>(get_native_pc_data()), sizeof(struct pksav_gba_pc_pokemon));
            ofile.close();
        }
        else
        {
            throw std::invalid_argument("GBA Pokémon can only be saved to .3gpkm files.");
        }
    }

    void pokemon_gbaimpl::set_form(
        const std::string& form
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _database_entry.set_form(form);

        _p_growth_block->species = pksav_littleendian16(uint16_t(_database_entry.get_pokemon_index()));

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_personality_from_form();
        }
    }

    bool pokemon_gbaimpl::is_egg()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return bool(_p_misc_block->iv_egg_ability & PKSAV_GBA_POKEMON_EGG_MASK);
    }

    void pokemon_gbaimpl::set_is_egg(
        bool is_egg
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        if(is_egg)
        {
            _p_misc_block->iv_egg_ability |= PKSAV_GBA_POKEMON_EGG_MASK;
        }
        else
        {
            _p_misc_block->iv_egg_ability &= ~PKSAV_GBA_POKEMON_EGG_MASK;
        }
    }

    std::string pokemon_gbaimpl::get_condition()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        std::string ret = "None";

        // Check the mask. We won't distinguish between sleep states for different
        // numbers of turns.

        const pksav::condition_mask_bimap_t& condition_mask_bimap = pksav::get_condition_mask_bimap();
        for(const auto& condition_mask_iter: condition_mask_bimap.right)
        {
            if(_pksav_pokemon.party_data.condition & condition_mask_iter.first)
            {
                ret = condition_mask_iter.second;
                break;
            }
        }

        return ret;
    }

    void pokemon_gbaimpl::set_condition(
        const std::string& condition
    )
    {
        const pksav::condition_mask_bimap_t& condition_mask_bimap = pksav::get_condition_mask_bimap();
        pkmn::enforce_value_in_map_keys(
            "Condition",
            condition,
            condition_mask_bimap.left
        );

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _pksav_pokemon.party_data.condition = 0;

        if(condition == "Asleep")
        {
            // Sleep is stored as the number of turns asleep, so set a random value.
            _pksav_pokemon.party_data.condition = pksav_littleendian32(pkmn::rng<uint32_t>().rand(1, 7));
        }
        else
        {
            _pksav_pokemon.party_data.condition = pksav_littleendian32(condition_mask_bimap.left.at(condition));
        }
    }

    std::string pokemon_gbaimpl::get_nickname()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        // Add an extra character for the null terminator.
        char nickname[PKSAV_GBA_POKEMON_NICKNAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gba_import_text(
                _pksav_pokemon.pc_data.nickname,
                nickname,
                PKSAV_GBA_POKEMON_NICKNAME_LENGTH
            );
        )

        return std::string(nickname);
    }

    void pokemon_gbaimpl::set_nickname(
        const std::string& nickname
    )
    {
        pkmn::enforce_string_length(
            "Nickname",
            nickname,
            1,
            PKSAV_GBA_POKEMON_NICKNAME_LENGTH
        );

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        PKSAV_CALL(
            pksav_gba_export_text(
                nickname.c_str(),
                _pksav_pokemon.pc_data.nickname,
                PKSAV_GBA_POKEMON_NICKNAME_LENGTH
            );
        )
    }

    pkmn::e_gender pokemon_gbaimpl::get_gender()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return pkmn::calculations::modern_pokemon_gender(
                   get_species(),
                   pksav_littleendian32(_pksav_pokemon.pc_data.personality)
               );
    }

    void pokemon_gbaimpl::set_gender(
        pkmn::e_gender gender
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _set_modern_gender(
            &_pksav_pokemon.pc_data.personality,
            gender
        );

        // No need to check Unown case, it's genderless.
    }

    bool pokemon_gbaimpl::is_shiny()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return pkmn::calculations::modern_shiny(
                   pksav_littleendian32(_pksav_pokemon.pc_data.personality),
                   pksav_littleendian32(_pksav_pokemon.pc_data.ot_id.id)
               );
    }

    void pokemon_gbaimpl::set_shininess(
        bool value
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _set_modern_shininess(
            &_pksav_pokemon.pc_data.personality,
            &_pksav_pokemon.pc_data.ot_id.id,
            value
        );

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_personality();
        }
    }

    pkmn::e_item pokemon_gbaimpl::get_held_item()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return pkmn::e_item(
                   pkmn::database::item_index_to_id(
                       pksav_littleendian16(_p_growth_block->held_item),
                       _database_entry.get_game_id()
                   )
               );
    }

    void pokemon_gbaimpl::set_held_item(
        pkmn::e_item held_item
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        // Make sure item is valid and holdable
        pkmn::database::item_entry item(
            held_item,
            get_game()
        );

        if(!item.holdable() && (held_item != pkmn::e_item::NONE))
        {
            throw std::invalid_argument("This item is not holdable.");
        }

        _p_growth_block->held_item = pksav_littleendian16(uint16_t(item.get_item_index()));
    }

    pkmn::e_nature pokemon_gbaimpl::get_nature()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        const pksav::nature_bimap_t& nature_bimap = pksav::get_nature_bimap();

        enum pksav_nature nature = static_cast<enum pksav_nature>(get_personality() % 25);

        // All values 0-24 should be represented in this bimap.
        auto nature_iter = nature_bimap.right.find(nature);
        BOOST_ASSERT(nature_iter != nature_bimap.right.end());

        return nature_iter->second;
    }

    void pokemon_gbaimpl::set_nature(
        pkmn::e_nature nature
    )
    {
        const pksav::nature_bimap_t& nature_bimap = pksav::get_nature_bimap();
        pkmn::enforce_value_in_map_keys(
            "Nature",
            nature,
            nature_bimap.left
        );

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        // Nature is derived from personality, so we need to find a new
        // one that preserves all other values.
        set_personality(
            pkmn::calculations::generate_personality(
                get_species(),
                get_original_trainer_id(),
                is_shiny(),
                get_ability(),
                get_gender(),
                nature
            )
        );
    }

    int pokemon_gbaimpl::get_pokerus_duration()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return _get_pokerus_duration(&_p_misc_block->pokerus);
    }

    void pokemon_gbaimpl::set_pokerus_duration(
        int duration
    )
    {
        pkmn::enforce_bounds("Duration", duration, 0, 15);

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _set_pokerus_duration(
            &_p_misc_block->pokerus,
            duration
        );
    }

    std::string pokemon_gbaimpl::get_original_trainer_name()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        // Add an extra character for the null terminator.
        char otname[PKSAV_GBA_POKEMON_OTNAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gba_import_text(
                _pksav_pokemon.pc_data.otname,
                otname,
                PKSAV_GBA_POKEMON_OTNAME_LENGTH
            );
        )

        return std::string(otname);
    }

    void pokemon_gbaimpl::set_original_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            PKSAV_GBA_POKEMON_OTNAME_LENGTH
        );

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        PKSAV_CALL(
            pksav_gba_export_text(
                trainer_name.c_str(),
                _pksav_pokemon.pc_data.otname,
                PKSAV_GBA_POKEMON_OTNAME_LENGTH
            );
        )
    }

    uint16_t pokemon_gbaimpl::get_original_trainer_public_id()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return pksav_littleendian16(_pksav_pokemon.pc_data.ot_id.pid);
    }

    uint16_t pokemon_gbaimpl::get_original_trainer_secret_id()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return pksav_littleendian16(_pksav_pokemon.pc_data.ot_id.sid);
    }

    uint32_t pokemon_gbaimpl::get_original_trainer_id()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return pksav_littleendian32(_pksav_pokemon.pc_data.ot_id.id);
    }

    void pokemon_gbaimpl::set_original_trainer_public_id(
        uint16_t public_id
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _pksav_pokemon.pc_data.ot_id.pid = pksav_littleendian16(public_id);
    }

    void pokemon_gbaimpl::set_original_trainer_secret_id(
        uint16_t secret_id
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _pksav_pokemon.pc_data.ot_id.sid = pksav_littleendian16(secret_id);
    }

    void pokemon_gbaimpl::set_original_trainer_id(
        uint32_t id
    )
    {
        _pksav_pokemon.pc_data.ot_id.id = pksav_littleendian32(id);
    }

    pkmn::e_gender pokemon_gbaimpl::get_original_trainer_gender()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return (_p_misc_block->origin_info & PKSAV_GBA_POKEMON_OTGENDER_MASK)
                   ? pkmn::e_gender::FEMALE
                   : pkmn::e_gender::MALE;
    }

    void pokemon_gbaimpl::set_original_trainer_gender(
        pkmn::e_gender gender
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        if(gender == pkmn::e_gender::MALE)
        {
            _p_misc_block->origin_info &= ~PKSAV_GBA_POKEMON_OTGENDER_MASK;
        }
        else if(gender == pkmn::e_gender::FEMALE)
        {
            _p_misc_block->origin_info |= PKSAV_GBA_POKEMON_OTGENDER_MASK;
        }
        else
        {
            throw std::invalid_argument("gender: valid values \"Male\", \"Female\"");
        }
    }

    pkmn::e_language pokemon_gbaimpl::get_language()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        pkmn::e_language ret = pkmn::e_language::ENGLISH;

        enum pksav_gba_language language_as_enum = static_cast<enum pksav_gba_language>(
                                                       pksav_littleendian16(
                                                          _pksav_pokemon.pc_data.language
                                                       )
                                                   );

        const pksav::gba_language_bimap_t& gba_language_bimap = pksav::get_gba_language_bimap();

        // Allow for other values in case of a corrupted save.
        auto gba_language_bimap_iter = gba_language_bimap.right.find(language_as_enum);
        if(gba_language_bimap_iter != gba_language_bimap.right.end())
        {
            ret = gba_language_bimap_iter->second;
        }

        return ret;
    }

    void pokemon_gbaimpl::set_language(
        pkmn::e_language language
    )
    {
        const pksav::gba_language_bimap_t& gba_language_bimap = pksav::get_gba_language_bimap();
        pkmn::enforce_value_in_map_keys(
            "Language",
            language,
            gba_language_bimap.left
        );

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _pksav_pokemon.pc_data.language = pksav_littleendian16(static_cast<uint16_t>(
                                     gba_language_bimap.left.at(language)
                                 ));
    }

    int pokemon_gbaimpl::get_current_trainer_friendship()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return _p_growth_block->friendship;
    }

    void pokemon_gbaimpl::set_current_trainer_friendship(
        int friendship
    )
    {
        pkmn::enforce_bounds("Friendship", friendship, 0, 255);

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _p_growth_block->friendship = uint8_t(friendship);
    }

    pkmn::e_ability pokemon_gbaimpl::get_ability()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        pkmn::e_ability ret;

        pkmn::ability_pair_t abilities = _database_entry.get_abilities();
        if(abilities.second == pkmn::e_ability::NONE)
        {
            ret = abilities.first;
        }
        else
        {
            bool has_second_ability = bool(_p_misc_block->iv_egg_ability & PKSAV_GBA_POKEMON_ABILITY_MASK);

            ret = has_second_ability ? abilities.second : abilities.first;
        }

        return ret;
    }

    void pokemon_gbaimpl::set_ability(pkmn::e_ability ability)
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _set_ability(ability);

        // Ability is derived from personality, so we need to find a new
        // one that preserves all other values.
        _pksav_pokemon.pc_data.personality = pksav_littleendian32(
            pkmn::calculations::generate_personality(
                get_species(),
                get_original_trainer_id(),
                is_shiny(),
                ability,
                get_gender(),
                get_nature()
            ));
    }

    pkmn::e_ball pokemon_gbaimpl::get_ball()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        // Account for corrupted data.
        pkmn::e_ball ball = pkmn::e_ball(PKSAV_GBA_POKEMON_BALL(_p_misc_block->origin_info));
        if((ball < pkmn::e_ball::NONE) || (ball > pkmn::e_ball::PREMIER_BALL))
        {
            ball = pkmn::e_ball::INVALID;
        }

        return ball;
    }

    void pokemon_gbaimpl::set_ball(
        pkmn::e_ball ball
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        // Try and instantiate an item_entry to validate the ball.
        (void)pkmn::database::item_entry(
                  pkmn::database::ball_to_item(ball),
                  get_game()
              );

        _p_misc_block->origin_info &= ~PKSAV_GBA_POKEMON_BALL_MASK;
        _p_misc_block->origin_info |= (uint16_t(ball) << PKSAV_GBA_POKEMON_BALL_OFFSET);
    }


    int pokemon_gbaimpl::get_level_met()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return (_p_misc_block->origin_info & PKSAV_GBA_POKEMON_LEVEL_MET_MASK);
    }

    void pokemon_gbaimpl::set_level_met(
        int level
    )
    {
        pkmn::enforce_bounds("Level met", level, 0, 100);

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _p_misc_block->origin_info &= ~PKSAV_GBA_POKEMON_LEVEL_MET_MASK;
        _p_misc_block->origin_info |= uint16_t(level);
    }

    std::string pokemon_gbaimpl::get_location_met(
        bool as_egg
    )
    {
        if(as_egg)
        {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation III.");
        }
        else
        {
            boost::lock_guard<pokemon_gbaimpl> lock(*this);

            return pkmn::database::location_index_to_name(
                       _p_misc_block->met_location,
                       _database_entry.get_game_id()
                   );
        }
    }

    void pokemon_gbaimpl::set_location_met(
        const std::string& location,
        bool as_egg
    )
    {
        if(as_egg)
        {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation III.");
        }
        else
        {
            boost::lock_guard<pokemon_gbaimpl> lock(*this);

            _p_misc_block->met_location = uint8_t(pkmn::database::location_name_to_index(
                                              location,
                                              _database_entry.get_game_id()
                                          ));
        }
    }

    pkmn::e_game pokemon_gbaimpl::get_original_game()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return pkmn::database::game_index_to_enum(
                   PKSAV_GBA_POKEMON_ORIGIN_GAME(_p_misc_block->origin_info)
               );
    }

    void pokemon_gbaimpl::set_original_game(pkmn::e_game game)
    {
        int generation = pkmn::database::game_enum_to_generation(game);
        if(generation != 3)
        {
            throw std::invalid_argument("Game must be from Generation III.");
        }

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _p_misc_block->origin_info &= ~PKSAV_GBA_POKEMON_ORIGIN_GAME_MASK;
        uint16_t game_index = uint16_t(pkmn::database::game_enum_to_index(game));

        _p_misc_block->origin_info |= (game_index << PKSAV_GBA_POKEMON_ORIGIN_GAME_OFFSET);
    }

    uint32_t pokemon_gbaimpl::get_personality()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return pksav_littleendian32(_pksav_pokemon.pc_data.personality);
    }

    void pokemon_gbaimpl::set_personality(
        uint32_t personality
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _pksav_pokemon.pc_data.personality = pksav_littleendian32(personality);
        _set_ability_from_personality();

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_personality();
        }
    }

    int pokemon_gbaimpl::get_experience()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return int(pksav_littleendian32(_p_growth_block->exp));
    }

    void pokemon_gbaimpl::set_experience(
        int experience
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        int max_experience = _database_entry.get_experience_at_level(100);
        pkmn::enforce_bounds("Experience", experience, 0, max_experience);

        _p_growth_block->exp = pksav_littleendian32(uint32_t(experience));
        _pksav_pokemon.party_data.level = uint8_t(_database_entry.get_level_at_experience(experience));

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gbaimpl::get_level()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return int(_pksav_pokemon.party_data.level);
    }

    void pokemon_gbaimpl::set_level(
        int level
    )
    {
        pkmn::enforce_bounds("Level", level, 1, 100);

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _pksav_pokemon.party_data.level = uint8_t(level);
        _p_growth_block->exp = pksav_littleendian32(uint32_t(_database_entry.get_experience_at_level(level)));

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_gbaimpl::set_IV(
        pkmn::e_stat stat,
        int value
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _set_modern_IV(
            stat,
            value,
            &_p_misc_block->iv_egg_ability
        );
    }

    void pokemon_gbaimpl::set_marking(
        const std::string& marking,
        bool value
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _set_marking(
            marking,
            value,
            &_pksav_pokemon.pc_data.markings
        );
    }

    // TODO: will these be needed in other files? Conversion to NDS perhaps?
    // If so, move to pksav/enum_maps

    static const std::unordered_map<std::string, enum pksav_gba_ribbon_mask> gba_ribbons = boost::assign::map_list_of
        ("Champion", PKSAV_GBA_CHAMPION_RIBBON_MASK)
        ("Winning",  PKSAV_GBA_WINNING_RIBBON_MASK)
        ("Victory",  PKSAV_GBA_VICTORY_RIBBON_MASK)
        ("Artist",   PKSAV_GBA_ARTIST_RIBBON_MASK)
        ("Effort",   PKSAV_GBA_EFFORT_RIBBON_MASK)
        ("Marine",   PKSAV_GBA_MARINE_RIBBON_MASK)
        ("Land",     PKSAV_GBA_LAND_RIBBON_MASK)
        ("Sky",      PKSAV_GBA_SKY_RIBBON_MASK)
        ("Country",  PKSAV_GBA_COUNTRY_RIBBON_MASK)
        ("National", PKSAV_GBA_NATIONAL_RIBBON_MASK)
        ("Earth",    PKSAV_GBA_EARTH_RIBBON_MASK)
        ("World",    PKSAV_GBA_WORLD_RIBBON_MASK)
    ;

    static const std::unordered_map<std::string, enum pksav_gba_contest_ribbon_level> gba_contest_ribbon_levels = boost::assign::map_list_of
        ("",       PKSAV_GBA_CONTEST_RIBBON_NONE)
        ("Normal", PKSAV_GBA_CONTEST_RIBBON_NORMAL)
        ("Super",  PKSAV_GBA_CONTEST_RIBBON_SUPER)
        ("Hyper",  PKSAV_GBA_CONTEST_RIBBON_HYPER)
        ("Master", PKSAV_GBA_CONTEST_RIBBON_MASTER)
    ;

    static const std::unordered_map<std::string, enum pksav_gba_contest_ribbons_mask> gba_contest_ribbon_masks = boost::assign::map_list_of
        ("Cool",   PKSAV_GBA_COOL_RIBBONS_MASK)
        ("Beauty", PKSAV_GBA_BEAUTY_RIBBONS_MASK)
        ("Cute",   PKSAV_GBA_CUTE_RIBBONS_MASK)
        ("Smart",  PKSAV_GBA_SMART_RIBBONS_MASK)
        ("Tough",  PKSAV_GBA_TOUGH_RIBBONS_MASK)
    ;

    static const std::unordered_map<std::string, enum pksav_gba_contest_ribbons_offset> gba_contest_ribbon_offsets = boost::assign::map_list_of
        ("Cool",   PKSAV_GBA_COOL_RIBBONS_OFFSET)
        ("Beauty", PKSAV_GBA_BEAUTY_RIBBONS_OFFSET)
        ("Cute",   PKSAV_GBA_CUTE_RIBBONS_OFFSET)
        ("Smart",  PKSAV_GBA_SMART_RIBBONS_OFFSET)
        ("Tough",  PKSAV_GBA_TOUGH_RIBBONS_OFFSET)
    ;

    void pokemon_gbaimpl::set_ribbon(
        const std::string& ribbon,
        bool value
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        if(_ribbons.find(ribbon) == _ribbons.end())
        {
            throw std::invalid_argument("Invalid ribbon.");
        }

        if(gba_ribbons.find(ribbon) != gba_ribbons.end())
        {
            _set_ribbon<uint32_t, enum pksav_gba_ribbon_mask>(
                ribbon,
                value,
                &_p_misc_block->ribbons_obedience,
                gba_ribbons
            );
        }
        else
        {
            _set_contest_ribbon(
                ribbon,
                value
            );
        }
    }

    void pokemon_gbaimpl::set_contest_stat(
        const std::string& stat,
        int value
    )
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _set_contest_stat(
            stat,
            value,
            &_p_effort_block->contest_stats
        );
    }

    void pokemon_gbaimpl::set_move(
        pkmn::e_move move,
        int index
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        // This will throw an error if the move is invalid.
        pkmn::database::move_entry entry(
            move,
            get_game()
        );
        _moves[index] = {move, entry.get_pp(0)};

        _p_attacks_block->moves[index] = pksav_littleendian16(uint16_t(entry.get_move_id()));
        _p_attacks_block->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gbaimpl::set_move_pp(
        int index,
        int pp
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        std::vector<int> PPs;
        pkmn::database::move_entry entry(_moves[index].move, get_game());
        for(int num_PP_ups = 0; num_PP_ups < 4; ++num_PP_ups)
        {
            PPs.emplace_back(entry.get_pp(num_PP_ups));
        }

        pkmn::enforce_bounds("PP", pp, 0, PPs.back());

        _moves[index].pp = pp;
        _p_attacks_block->move_pps[index] = uint8_t(pp);

        // Set the PP Up mask to the minimum value that will accommodate the given PP.
        _p_growth_block->pp_up &= ~uint8_t(3 << (index*2));
        for(uint8_t num_PP_ups = 0; num_PP_ups < 4; ++num_PP_ups)
        {
            if(pp <= PPs[num_PP_ups])
            {
                _p_growth_block->pp_up |= uint8_t(num_PP_ups << (index*2));
                break;
            }
        }
    }

    void pokemon_gbaimpl::set_EV(
        pkmn::e_stat stat,
        int value
    )
    {
        pkmn::enforce_value_in_vector(
            "Stat",
            stat,
            pkmn::MODERN_STATS
        );
        pkmn::enforce_EV_bounds(stat, value, true);

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        switch(stat)
        {
            case pkmn::e_stat::HP:
                _p_effort_block->ev_hp = uint8_t(value);
                break;

            case pkmn::e_stat::ATTACK:
                _p_effort_block->ev_atk = uint8_t(value);
                break;

            case pkmn::e_stat::DEFENSE:
                _p_effort_block->ev_def = uint8_t(value);
                break;

            case pkmn::e_stat::SPEED:
                _p_effort_block->ev_spd = uint8_t(value);
                break;

            case pkmn::e_stat::SPECIAL_ATTACK:
                _p_effort_block->ev_spatk = uint8_t(value);
                break;

            default:
                _p_effort_block->ev_spdef = uint8_t(value);
                break;
        }

        _update_EV_map();
        _populate_party_data();
    }

    int pokemon_gbaimpl::get_current_hp()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return pksav_littleendian16(_pksav_pokemon.party_data.current_hp);
    }

    void pokemon_gbaimpl::set_current_hp(
        int hp
    )
    {
        pkmn::enforce_bounds(
            "Current HP",
            hp,
            0,
            _stats[pkmn::e_stat::HP]
        );

        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        _pksav_pokemon.party_data.current_hp = pksav_littleendian16(static_cast<uint16_t>(hp));
    }

    std::string pokemon_gbaimpl::get_sprite_filepath()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        std::string ret;

#ifdef PKMN_ENABLE_QT
        BOOST_STATIC_CONSTEXPR int SPINDA_ID = 327;

        if(_database_entry.get_species_id() == SPINDA_ID) {
            bool shiny = is_shiny();
            fs::path spinda_sprite_filepath(pkmn::get_tmp_dir());
            spinda_sprite_filepath /= str(boost::format("spinda_%d_%d_%u.png")
                                          % _generation
                                          % (shiny ? 1 : 0)
                                          % pksav_littleendian32(_pksav_pokemon.pc_data.personality));

            pkmn::qt::GenerateSpindaSpriteAtFilepath(
                3,
                pksav_littleendian32(_pksav_pokemon.pc_data.personality),
                shiny,
                spinda_sprite_filepath.string()
            );

            ret = spinda_sprite_filepath.string();
        } else {
#endif
            ret = pokemon_impl::get_sprite_filepath();
#ifdef PKMN_ENABLE_QT
        }
#endif
        return ret;
    }

    void* pokemon_gbaimpl::get_native_pc_data()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        pksav_gba_set_pokemon_checksum(
            &_pksav_pokemon.pc_data
        );

        return _p_native_pc;
    }

    bool pokemon_gbaimpl::get_is_obedient()
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        return bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_POKEMON_OBEDIENCE_MASK);
    }

    void pokemon_gbaimpl::set_is_obedient(bool is_obedient)
    {
        boost::lock_guard<pokemon_gbaimpl> lock(*this);

        if(is_obedient)
        {
            _p_misc_block->ribbons_obedience |= PKSAV_GBA_POKEMON_OBEDIENCE_MASK;
        }
        else
        {
            _p_misc_block->ribbons_obedience &= ~PKSAV_GBA_POKEMON_OBEDIENCE_MASK;
        }
    }

    void pokemon_gbaimpl::_set_contest_ribbon(
        const std::string& ribbon,
        bool value
    ) {
        std::vector<std::string> ribbon_parts;
        boost::split(ribbon_parts, ribbon, boost::is_any_of(" "));

        // Validate input (which should already have been validated)
        if((ribbon_parts.size() == 0 or ribbon_parts.size() > 2) or
           (gba_contest_ribbon_masks.count(ribbon_parts.at(0)) == 0) or
           (ribbon_parts.size() == 2 and
            gba_contest_ribbon_levels.count(ribbon_parts.at(1)) == 0)
          )
        {
            throw std::invalid_argument("Invalid ribbon.");
        }

        uint16_t mask = uint16_t(gba_contest_ribbon_masks.at(ribbon_parts.at(0)));
        uint16_t offset = uint16_t(gba_contest_ribbon_offsets.at(ribbon_parts.at(0)));
        uint16_t level = 0;
        if(ribbon_parts.size() == 2) {
            level = uint16_t(gba_contest_ribbon_levels.at(ribbon_parts.at(1)));
        }

        uint16_t current_level = (_p_misc_block->ribbons_obedience & mask) >> offset;
        if(value) {
            if(level == 0) {
                ++level;
            }
            level = std::max<uint16_t>(level, current_level);
        } else {
            if(level > 0) {
                --level;
            }
            level = std::min<uint16_t>(level, current_level);
        }

        _p_misc_block->ribbons_obedience &= ~mask;
        _p_misc_block->ribbons_obedience |= (level << offset);

        _update_ribbons_map();
    }

    void pokemon_gbaimpl::_populate_party_data() {
        pksav::gba_pc_pokemon_to_party_data(
            _database_entry,
            &_pksav_pokemon.pc_data,
            &_pksav_pokemon.party_data
        );

        _update_stat_map();
    }

    void pokemon_gbaimpl::_update_moves(
        int index
    )
    {
        _moves.resize(4);
        switch(index)
        {
            case 0:
            case 1:
            case 2:
            case 3:
                // TODO: check move validity
                _moves[index] = pkmn::move_slot(
                    static_cast<pkmn::e_move>(
                        pksav_littleendian16(_p_attacks_block->moves[index])
                    ),
                    _p_attacks_block->move_pps[index]
                );
                break;

            default:
                for(int i = 0; i < 4; ++i)
                {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gbaimpl::_update_markings_map() {
        _markings["Circle"]   = bool(_pksav_pokemon.pc_data.markings & PKSAV_MARKING_CIRCLE);
        _markings["Triangle"] = bool(_pksav_pokemon.pc_data.markings & PKSAV_MARKING_TRIANGLE);
        _markings["Square"]   = bool(_pksav_pokemon.pc_data.markings & PKSAV_MARKING_SQUARE);
        _markings["Heart"]    = bool(_pksav_pokemon.pc_data.markings & PKSAV_MARKING_HEART);
    }

    void pokemon_gbaimpl::_update_ribbons_map() {
        uint32_t cool_ribbons = _p_misc_block->ribbons_obedience & PKSAV_GBA_COOL_RIBBONS_MASK;
        _ribbons["Cool"]        = bool(cool_ribbons >= PKSAV_GBA_CONTEST_RIBBON_NORMAL);
        _ribbons["Cool Super"]  = bool(cool_ribbons >= PKSAV_GBA_CONTEST_RIBBON_SUPER);
        _ribbons["Cool Hyper"]  = bool(cool_ribbons >= PKSAV_GBA_CONTEST_RIBBON_HYPER);
        _ribbons["Cool Master"] = bool(cool_ribbons >= PKSAV_GBA_CONTEST_RIBBON_MASTER);

        uint32_t beauty_ribbons = _p_misc_block->ribbons_obedience & PKSAV_GBA_BEAUTY_RIBBONS_MASK;
        beauty_ribbons >>= PKSAV_GBA_BEAUTY_RIBBONS_OFFSET;
        _ribbons["Beauty"]        = bool(beauty_ribbons >= PKSAV_GBA_CONTEST_RIBBON_NORMAL);
        _ribbons["Beauty Super"]  = bool(beauty_ribbons >= PKSAV_GBA_CONTEST_RIBBON_SUPER);
        _ribbons["Beauty Hyper"]  = bool(beauty_ribbons >= PKSAV_GBA_CONTEST_RIBBON_HYPER);
        _ribbons["Beauty Master"] = bool(beauty_ribbons >= PKSAV_GBA_CONTEST_RIBBON_MASTER);

        uint32_t cute_ribbons = _p_misc_block->ribbons_obedience & PKSAV_GBA_CUTE_RIBBONS_MASK;
        cute_ribbons >>= PKSAV_GBA_CUTE_RIBBONS_OFFSET;
        _ribbons["Cute"]        = bool(cute_ribbons >= PKSAV_GBA_CONTEST_RIBBON_NORMAL);
        _ribbons["Cute Super"]  = bool(cute_ribbons >= PKSAV_GBA_CONTEST_RIBBON_SUPER);
        _ribbons["Cute Hyper"]  = bool(cute_ribbons >= PKSAV_GBA_CONTEST_RIBBON_HYPER);
        _ribbons["Cute Master"] = bool(cute_ribbons >= PKSAV_GBA_CONTEST_RIBBON_MASTER);

        uint32_t smart_ribbons = _p_misc_block->ribbons_obedience & PKSAV_GBA_SMART_RIBBONS_MASK;
        smart_ribbons >>= PKSAV_GBA_SMART_RIBBONS_OFFSET;
        _ribbons["Smart"]        = bool(smart_ribbons >= PKSAV_GBA_CONTEST_RIBBON_NORMAL);
        _ribbons["Smart Super"]  = bool(smart_ribbons >= PKSAV_GBA_CONTEST_RIBBON_SUPER);
        _ribbons["Smart Hyper"]  = bool(smart_ribbons >= PKSAV_GBA_CONTEST_RIBBON_HYPER);
        _ribbons["Smart Master"] = bool(smart_ribbons >= PKSAV_GBA_CONTEST_RIBBON_MASTER);

        uint32_t tough_ribbons = _p_misc_block->ribbons_obedience & PKSAV_GBA_TOUGH_RIBBONS_MASK;
        tough_ribbons >>= PKSAV_GBA_TOUGH_RIBBONS_OFFSET;
        _ribbons["Tough"]        = bool(tough_ribbons >= PKSAV_GBA_CONTEST_RIBBON_NORMAL);
        _ribbons["Tough Super"]  = bool(tough_ribbons >= PKSAV_GBA_CONTEST_RIBBON_SUPER);
        _ribbons["Tough Hyper"]  = bool(tough_ribbons >= PKSAV_GBA_CONTEST_RIBBON_HYPER);
        _ribbons["Tough Master"] = bool(tough_ribbons >= PKSAV_GBA_CONTEST_RIBBON_MASTER);

        _ribbons["Champion"] = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_CHAMPION_RIBBON_MASK);
        _ribbons["Winning"]  = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_WINNING_RIBBON_MASK);
        _ribbons["Victory"]  = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_VICTORY_RIBBON_MASK);
        _ribbons["Artist"]   = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_ARTIST_RIBBON_MASK);
        _ribbons["Effort"]   = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_EFFORT_RIBBON_MASK);
        _ribbons["Marine"]   = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_MARINE_RIBBON_MASK);
        _ribbons["Land"]     = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_LAND_RIBBON_MASK);
        _ribbons["Sky"]      = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_SKY_RIBBON_MASK);
        _ribbons["Country"]  = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_COUNTRY_RIBBON_MASK);
        _ribbons["National"] = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_NATIONAL_RIBBON_MASK);
        _ribbons["Earth"]    = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_EARTH_RIBBON_MASK);
        _ribbons["World"]    = bool(_p_misc_block->ribbons_obedience & PKSAV_GBA_WORLD_RIBBON_MASK);
    }

    void pokemon_gbaimpl::_update_EV_map()
    {
        _EVs[pkmn::e_stat::HP]              = int(_p_effort_block->ev_hp);
        _EVs[pkmn::e_stat::ATTACK]          = int(_p_effort_block->ev_atk);
        _EVs[pkmn::e_stat::DEFENSE]         = int(_p_effort_block->ev_def);
        _EVs[pkmn::e_stat::SPEED]           = int(_p_effort_block->ev_spd);
        _EVs[pkmn::e_stat::SPECIAL_ATTACK]  = int(_p_effort_block->ev_spatk);
        _EVs[pkmn::e_stat::SPECIAL_DEFENSE] = int(_p_effort_block->ev_spdef);
    }

    void pokemon_gbaimpl::_update_stat_map() {
        _stats[pkmn::e_stat::HP]              = int(pksav_littleendian16(_pksav_pokemon.party_data.max_hp));
        _stats[pkmn::e_stat::ATTACK]          = int(pksav_littleendian16(_pksav_pokemon.party_data.atk));
        _stats[pkmn::e_stat::DEFENSE]         = int(pksav_littleendian16(_pksav_pokemon.party_data.def));
        _stats[pkmn::e_stat::SPEED]           = int(pksav_littleendian16(_pksav_pokemon.party_data.spd));
        _stats[pkmn::e_stat::SPECIAL_ATTACK]  = int(pksav_littleendian16(_pksav_pokemon.party_data.spatk));
        _stats[pkmn::e_stat::SPECIAL_DEFENSE] = int(pksav_littleendian16(_pksav_pokemon.party_data.spdef));
    }

    void pokemon_gbaimpl::_set_ability(pkmn::e_ability ability)
    {
        pkmn::ability_pair_t abilities = _database_entry.get_abilities();
        if(ability == pkmn::e_ability::NONE)
        {
            throw std::invalid_argument("The ability cannot be set to None.");
        }
        else if(ability == abilities.first)
        {
            _p_misc_block->iv_egg_ability &= ~PKSAV_GBA_POKEMON_ABILITY_MASK;
        }
        else if(ability == abilities.second)
        {
            BOOST_ASSERT(abilities.second != pkmn::e_ability::NONE);
            _p_misc_block->iv_egg_ability |= PKSAV_GBA_POKEMON_ABILITY_MASK;
        }
        else
        {
            std::string error_message;
            if(abilities.second == pkmn::e_ability::NONE)
            {
                error_message = str(boost::format("ability: valid values \"%s\"")
                                    % pkmn::ability_to_string(abilities.first).c_str());
            }
            else
            {
                error_message = str(boost::format("ability: valid values \"%s\", \"%s\"")
                                    % pkmn::ability_to_string(abilities.first).c_str()
                                    % pkmn::ability_to_string(abilities.second).c_str());
            }

            throw std::invalid_argument(error_message.c_str());
        }
    }

    void pokemon_gbaimpl::_set_unown_form_from_personality()
    {
        _database_entry.set_form(
            pkmn::calculations::gen3_unown_form(
                pksav_littleendian32(
                    _pksav_pokemon.pc_data.personality
                )
            )
        );
    }

    void pokemon_gbaimpl::_set_unown_personality_from_form() {
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

        uint8_t* pid_as_bytes = reinterpret_cast<uint8_t*>(&_pksav_pokemon.pc_data.personality);
        for(size_t i = 0; i < 4; ++i) {
            pid_as_bytes[i] &= ~0x3;
            pid_as_bytes[i] |= ((num & (0x3 << (2*i))) >> (2*i));
        }
    }

    void pokemon_gbaimpl::_register_attributes()
    {
        using std::placeholders::_1;

        _boolean_attribute_engine.register_attribute_fcns(
            "Obedient?",
            std::bind(&pokemon_gbaimpl::get_is_obedient, this),
            std::bind(&pokemon_gbaimpl::set_is_obedient, this, _1)
        );
    }
}
