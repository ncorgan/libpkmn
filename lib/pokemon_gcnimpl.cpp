/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "utils/misc.hpp"
#include "pokemon_gcnimpl.hpp"
#include "pokemon_gbaimpl.hpp"

#include "conversions/gen3_conversions.hpp"

#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_index.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "pkmgc/enum_maps.hpp"
#include "pksav/enum_maps.hpp"

#include "types/rng.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/personality.hpp>
#include <pkmn/calculations/shininess.hpp>

#include <pkmn/database/item_entry.hpp>

#include <pkmn/enums/enum_to_string.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/bimap.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>
#include <unordered_map>

namespace pkmn
{
    BOOST_STATIC_CONSTEXPR int MEW_ID    = 151;
    BOOST_STATIC_CONSTEXPR int UNOWN_ID  = 201;
    BOOST_STATIC_CONSTEXPR int DEOXYS_ID = 386;

    static const std::unordered_map<std::string, LibPkmGC::ContestAchievementLevel> CONTEST_LEVEL_MAP =
    {
        {"",       LibPkmGC::NormalContestWon},
        {"Super",  LibPkmGC::SuperContestWon},
        {"Hyper",  LibPkmGC::HyperContestWon},
        {"Master", LibPkmGC::MasterContestWon}
    };

    pokemon_gcnimpl::pokemon_gcnimpl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        if(_database_entry.get_game_id() == COLOSSEUM_ID)
        {
            _libpkmgc_pokemon_uptr.reset(new LibPkmGC::Colosseum::Pokemon);
        }
        else
        {
            _libpkmgc_pokemon_uptr.reset(new LibPkmGC::XD::Pokemon);
        }

        pkmn::rng<LibPkmGC::u8> rng8;
        pkmn::rng<LibPkmGC::u32> rng32;

        _libpkmgc_pokemon_uptr->species = LibPkmGC::PokemonSpeciesIndex(_database_entry.get_pokemon_index());
        _libpkmgc_pokemon_uptr->heldItem = LibPkmGC::NoItem;
        _libpkmgc_pokemon_uptr->friendship = LibPkmGC::u8(_database_entry.get_base_friendship());
        _libpkmgc_pokemon_uptr->locationCaught = 0; // Met in a distant land
        _libpkmgc_pokemon_uptr->ballCaughtWith = LibPkmGC::PokeBall;
        _libpkmgc_pokemon_uptr->levelMet = LibPkmGC::u8(level);
        _libpkmgc_pokemon_uptr->OTGender = LibPkmGC::Male;
        _libpkmgc_pokemon_uptr->OTName->fromUTF8(pkmn::pokemon::DEFAULT_TRAINER_NAME.c_str());
        _libpkmgc_pokemon_uptr->name->fromUTF8(boost::algorithm::to_upper_copy(_database_entry.get_species_name()).c_str());
        _libpkmgc_pokemon_uptr->contestLuster = 0;
        _libpkmgc_pokemon_uptr->pokerusStatus = 0;
        _libpkmgc_pokemon_uptr->experience = LibPkmGC::u32(_database_entry.get_experience_at_level(level));
        _libpkmgc_pokemon_uptr->SID = LibPkmGC::u16(pkmn::pokemon::DEFAULT_TRAINER_ID >> 16);
        _libpkmgc_pokemon_uptr->TID = LibPkmGC::u16(pkmn::pokemon::DEFAULT_TRAINER_ID & 0xFFFF);

        _libpkmgc_pokemon_uptr->PID = rng32.rand();
        _set_ability_from_personality();

        _libpkmgc_pokemon_uptr->version.game = LibPkmGC::Colosseum_XD;
        _libpkmgc_pokemon_uptr->version.currentRegion = LibPkmGC::NTSC_U;
        _libpkmgc_pokemon_uptr->version.originalRegion = LibPkmGC::NTSC_U;
        _libpkmgc_pokemon_uptr->version.language = LibPkmGC::English;

        // Only do this by default for Mew or Deoxys, who won't obey without
        // this flag. For any others, this will cause them to falsely be
        // flagged as a fateful encounter if traded up to Generation IV.
        if((_database_entry.get_species_id() == MEW_ID) or
           (_database_entry.get_species_id() == DEOXYS_ID))
        {
            _libpkmgc_pokemon_uptr->obedient = true;
        }
        for(bool& r_has_special_ribbon: _libpkmgc_pokemon_uptr->specialRibbons)
        {
            r_has_special_ribbon = false;
        }
        _libpkmgc_pokemon_uptr->unimplementedRibbons = 0;

        for(LibPkmGC::PokemonMove& r_move: _libpkmgc_pokemon_uptr->moves)
        {
            r_move.move = LibPkmGC::NoMove;
            r_move.currentPPs = 0;
            r_move.nbPPUpsUsed = 0;
        }

        for(size_t i = 0; i < 6; ++i)
        {
            _libpkmgc_pokemon_uptr->EVs[i] = 0;
            _libpkmgc_pokemon_uptr->IVs[i] = rng8.rand(0,31);
        }

        for(size_t i = 0; i < 5; ++i)
        {
            _libpkmgc_pokemon_uptr->contestStats[i] = 0;
            _libpkmgc_pokemon_uptr->contestAchievements[i] = LibPkmGC::NoContestWon;
        }

        // The in-game storage doesn't distinguish between Colosseum and XD.
        set_original_game(pkmn::e_game::COLOSSEUM);

        // Populate abstractions
        _update_ribbons_map();
        _update_EV_map();
        _init_IV_map();
        _init_gcn_contest_stats_map();
        _init_markings_map();
        set_level(level);
        _init_default_moves_for_level();
        _populate_party_data();

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_personality_from_form();
        }

        _register_attributes();

        _p_native_pc = _libpkmgc_pokemon_uptr.get();
        _p_native_party = nullptr;
    }

    pokemon_gcnimpl::pokemon_gcnimpl(
        const LibPkmGC::GC::Pokemon* p_libpkmgc_pokemon,
        int game_id
    ): pokemon_impl(
           int(p_libpkmgc_pokemon->species),
           game_id
       )
    {
        _libpkmgc_pokemon_uptr.reset(p_libpkmgc_pokemon->clone());

        // Populate abstractions
        _update_ribbons_map();
        _update_EV_map();
        _init_IV_map();
        _init_gcn_contest_stats_map();
        _init_markings_map();
        _update_moves(-1);
        _update_stat_map();

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_personality_from_form();
        }

        _register_attributes();

        _p_native_pc = _libpkmgc_pokemon_uptr.get();
        _p_native_party = nullptr;
    }

    pokemon::sptr pokemon_gcnimpl::to_game(pkmn::e_game game)
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        pkmn::pokemon::sptr ret;

        int game_id = pkmn::database::game_enum_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);
        switch(generation)
        {
            case 3:
                if(game_is_gamecube(game_id))
                {
                    if(game_id == _database_entry.get_game_id())
                    {
                        ret = std::make_shared<pokemon_gcnimpl>(
                                  _libpkmgc_pokemon_uptr.get(),
                                  game_id
                              );
                    }
                    else if(game_id == COLOSSEUM_ID)
                    {
                        LibPkmGC::XD::Pokemon* p_xd_pokemon =
                            dynamic_cast<LibPkmGC::XD::Pokemon*>(
                                _libpkmgc_pokemon_uptr.get()
                            );
                        BOOST_ASSERT(p_xd_pokemon != nullptr);

                        LibPkmGC::Colosseum::Pokemon colosseum_pokemon(*p_xd_pokemon);
                        ret = std::make_shared<pokemon_gcnimpl>(
                                  &colosseum_pokemon,
                                  game_id
                              );
                    }
                    else
                    {
                        LibPkmGC::Colosseum::Pokemon* p_colosseum_pokemon =
                            dynamic_cast<LibPkmGC::Colosseum::Pokemon*>(
                                _libpkmgc_pokemon_uptr.get()
                            );
                        BOOST_ASSERT(p_colosseum_pokemon != nullptr);

                        LibPkmGC::XD::Pokemon xd_pokemon(*p_colosseum_pokemon);
                        ret = std::make_shared<pokemon_gcnimpl>(
                                  &xd_pokemon,
                                  game_id
                              );
                    }

                    ret->set_level_met(get_level());
                }
                else
                {
                    struct pksav_gba_party_pokemon pksav_pokemon;
                    pkmn::conversions::gcn_pokemon_to_gba_party(
                        _libpkmgc_pokemon_uptr.get(),
                        &pksav_pokemon
                    );

                    ret = std::make_shared<pokemon_gbaimpl>(&pksav_pokemon, game_id);
                }

                // Some Gamecube-specific indices don't match up. This will
                // also catch attempts to send over invalid items.
                if(_database_entry.get_game_id() != game_id)
                {
                    ret->set_held_item(get_held_item());
                }
                ret->set_original_game(get_original_game());
                break;

            case 4:
            case 5:
            case 6:
                throw pkmn::unimplemented_error();

            default:
                throw std::invalid_argument("Generation II Pokémon can only be converted to Generation III-VI.");
        }

        return ret;
    }

    void pokemon_gcnimpl::export_to_file(
        PKMN_UNUSED(const std::string& filepath)
    )
    {
        throw pkmn::feature_not_in_game_error("Exporting to file");
    }

    void pokemon_gcnimpl::set_form(
        const std::string& form
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        bool was_shadow = (_database_entry.get_form() == "Shadow");
        _database_entry.set_form(form);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }

        if(was_shadow and form != "Shadow")
        {
            _libpkmgc_pokemon_uptr->shadowPkmID = 0;
        }
        else if(form == "Shadow")
        {
            static const char* shadow_query = \
                "SELECT shadow_pokemon_id FROM shadow_pokemon WHERE species_id=? AND "
                "colosseum=?";

            _libpkmgc_pokemon_uptr->shadowPkmID = LibPkmGC::u16(
                                        pkmn::database::query_db_bind2<int, int, int>(
                                            shadow_query,
                                            _database_entry.get_species_id(),
                                            ((_database_entry.get_game_id() == COLOSSEUM_ID) ? 1 : 0)
                                        )
                                    );
        }
    }

    bool pokemon_gcnimpl::is_egg()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->isEgg();
    }

    void pokemon_gcnimpl::set_is_egg(
        bool is_egg
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->setEggFlag(is_egg);
    }

    pkmn::e_condition pokemon_gcnimpl::get_condition()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        pkmn::e_condition ret = pkmn::e_condition::NONE;
        LibPkmGC::PokemonStatus status = _libpkmgc_pokemon_uptr->partyData.status;

        static const pkmgc::condition_bimap_t& CONDITION_BIMAP = pkmgc::get_condition_bimap();

        // Allow for a corrupted save.
        if(CONDITION_BIMAP.right.count(status) > 0)
        {
            ret = CONDITION_BIMAP.right.at(status);
        }

        return ret;
    }

    void pokemon_gcnimpl::set_condition(
        pkmn::e_condition condition
    )
    {
        static const pkmgc::condition_bimap_t& CONDITION_BIMAP = pkmgc::get_condition_bimap();

        auto condition_iter = CONDITION_BIMAP.left.find(condition);

        if(condition_iter != CONDITION_BIMAP.left.end())
        {
            boost::lock_guard<pokemon_gcnimpl> lock(*this);

            _libpkmgc_pokemon_uptr->partyData.status = condition_iter->second;

            if(condition == pkmn::e_condition::ASLEEP)
            {
                // Sleep is stored as the number of turns asleep, so set a random value.
                _libpkmgc_pokemon_uptr->partyData.turnsOfSleepRemaining = pkmn::rng<LibPkmGC::s8>().rand(1, 7);
            }
        }
        else
        {
            throw std::invalid_argument("Invalid condition.");
        }
    }

    std::string pokemon_gcnimpl::get_nickname()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->name->toUTF8();
    }

    void pokemon_gcnimpl::set_nickname(
        const std::string& nickname
    )
    {
        pkmn::enforce_string_length(
            "Nickname",
            nickname,
            1,
            10
        );

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->name->fromUTF8(nickname.c_str());
    }

    pkmn::e_gender pokemon_gcnimpl::get_gender()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return pkmn::calculations::modern_pokemon_gender(
                   get_species(),
                   _libpkmgc_pokemon_uptr->PID
               );
    }

    void pokemon_gcnimpl::set_gender(
        pkmn::e_gender gender
    )
    {
        pkmn::enforce_value_in_vector(
            "Gender",
            gender,
            {pkmn::e_gender::MALE, pkmn::e_gender::FEMALE, pkmn::e_gender::GENDERLESS}
        );

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _set_modern_gender(
            &_libpkmgc_pokemon_uptr->PID,
            gender
        );

        // Unown is genderless, so don't bother setting the form.
    }

    bool pokemon_gcnimpl::is_shiny()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->isShiny();
    }

    void pokemon_gcnimpl::set_shininess(
        bool value
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        // LibPkmGC stores trainer IDs in halves.
        uint32_t trainer_id = _libpkmgc_pokemon_uptr->TID | (uint32_t(_libpkmgc_pokemon_uptr->SID) << 16);

        _set_modern_shininess(
            &_libpkmgc_pokemon_uptr->PID,
            &trainer_id,
            value
        );

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_personality();
        }
    }

    pkmn::e_item pokemon_gcnimpl::get_held_item()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return pkmn::e_item(
                   pkmn::database::item_index_to_id(
                       _libpkmgc_pokemon_uptr->heldItem,
                       _database_entry.get_game_id()
                   )
               );
    }

    void pokemon_gcnimpl::set_held_item(
        pkmn::e_item held_item
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        // Make sure item is valid and holdable
        pkmn::database::item_entry item(
            held_item,
            get_game()
        );

        if(!item.holdable() && (held_item != pkmn::e_item::NONE))
        {
            throw std::invalid_argument("This item is not holdable.");
        }

        _libpkmgc_pokemon_uptr->heldItem = LibPkmGC::ItemIndex(item.get_item_index());
    }

    pkmn::e_nature pokemon_gcnimpl::get_nature()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        const pksav::nature_bimap_t& nature_bimap = pksav::get_nature_bimap();

        // No need to reimplement this with LibPkmGC's types when the
        // indices match.
        enum pksav_nature nature = static_cast<enum pksav_nature>(_libpkmgc_pokemon_uptr->getNature());

        // All values 0-24 should be represented in this bimap.
        auto nature_iter = nature_bimap.right.find(nature);
        BOOST_ASSERT(nature_iter != nature_bimap.right.end());

        return nature_iter->second;
    }

    void pokemon_gcnimpl::set_nature(
        pkmn::e_nature nature
    )
    {
        const pksav::nature_bimap_t& nature_bimap = pksav::get_nature_bimap();
        pkmn::enforce_value_in_map_keys(
            "Nature",
            nature,
            nature_bimap.left
        );

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        // Nature is derived from personality, so we need to find a new
        // one that preserves all other values. Note that nature is not stored
        // in the LibPkmGC class, so we just set the personality.
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

    int pokemon_gcnimpl::get_pokerus_duration()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _get_pokerus_duration(&_libpkmgc_pokemon_uptr->pokerusStatus);
    }

    void pokemon_gcnimpl::set_pokerus_duration(
        int duration
    )
    {
        pkmn::enforce_bounds("Duration", duration, 0, 15);

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _set_pokerus_duration(
            &_libpkmgc_pokemon_uptr->pokerusStatus,
            duration
        );

        _libpkmgc_pokemon_uptr->partyData.pokerusDaysRemaining = LibPkmGC::s8(duration);
    }

    std::string pokemon_gcnimpl::get_original_trainer_name()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->OTName->toUTF8();
    }

    void pokemon_gcnimpl::set_original_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            7
        );

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->OTName->fromUTF8(trainer_name.c_str());
    }

    uint16_t pokemon_gcnimpl::get_original_trainer_public_id()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->TID;
    }

    uint16_t pokemon_gcnimpl::get_original_trainer_secret_id()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->SID;
    }

    uint32_t pokemon_gcnimpl::get_original_trainer_id()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return uint32_t(_libpkmgc_pokemon_uptr->TID) | (uint32_t(_libpkmgc_pokemon_uptr->SID) << 16);
    }

    void pokemon_gcnimpl::set_original_trainer_public_id(
        uint16_t public_id
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->TID = public_id;
    }

    void pokemon_gcnimpl::set_original_trainer_secret_id(
        uint16_t secret_id
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->SID = secret_id;
    }

    void pokemon_gcnimpl::set_original_trainer_id(
        uint32_t id
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->TID = uint16_t(id & 0xFFFF);
        _libpkmgc_pokemon_uptr->SID = uint16_t(id >> 16);
    }

    pkmn::e_gender pokemon_gcnimpl::get_original_trainer_gender()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        static const pkmgc::gender_bimap_t& GENDER_BIMAP = pkmgc::get_gender_bimap();
        BOOST_ASSERT(GENDER_BIMAP.right.count(_libpkmgc_pokemon_uptr->OTGender) > 0);

        return GENDER_BIMAP.right.at(_libpkmgc_pokemon_uptr->OTGender);
    }

    void pokemon_gcnimpl::set_original_trainer_gender(
        pkmn::e_gender gender
    )
    {
        pkmn::enforce_value_in_vector(
            "Gender",
            gender,
            {pkmn::e_gender::MALE, pkmn::e_gender::FEMALE}
        );

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        static const pkmgc::gender_bimap_t& GENDER_BIMAP = pkmgc::get_gender_bimap();
        BOOST_ASSERT(GENDER_BIMAP.left.count(gender) > 0);

        _libpkmgc_pokemon_uptr->OTGender = GENDER_BIMAP.left.at(gender);
    }

    pkmn::e_language pokemon_gcnimpl::get_language()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        pkmn::e_language ret = pkmn::e_language::ENGLISH;

        static const pkmgc::language_bimap_t& LANGUAGE_BIMAP = pkmgc::get_language_bimap();

        // Allow for other values in case of a corrupted save.
        auto language_bimap_iter = LANGUAGE_BIMAP.right.find(
                                       _libpkmgc_pokemon_uptr->version.language
                                   );
        if(language_bimap_iter != LANGUAGE_BIMAP.right.end())
        {
            ret = language_bimap_iter->second;
        }

        return ret;
    }

    void pokemon_gcnimpl::set_language(
        pkmn::e_language language
    )
    {
        static const pkmgc::language_bimap_t& LANGUAGE_BIMAP = pkmgc::get_language_bimap();

        pkmn::enforce_value_in_map_keys(
            "Language",
            language,
            LANGUAGE_BIMAP.left
        );

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->version.language = LANGUAGE_BIMAP.left.at(language);
    }

    int pokemon_gcnimpl::get_current_trainer_friendship()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->friendship;
    }

    void pokemon_gcnimpl::set_current_trainer_friendship(
        int friendship
    )
    {
        pkmn::enforce_bounds("Friendship", friendship, 0, 255);

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->friendship = LibPkmGC::u8(friendship);
    }

    pkmn::e_ability pokemon_gcnimpl::get_ability()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        pkmn::e_ability ret = pkmn::e_ability::NONE;

        pkmn::ability_pair_t abilities = _database_entry.get_abilities();
        if(abilities.second == pkmn::e_ability::NONE)
        {
            ret = abilities.first;
        }
        else
        {
            // Don't use LibPkmGC's function to get the ability. It has some mistakes.
            ret = _libpkmgc_pokemon_uptr->hasSecondAbility() ? abilities.second : abilities.first;
        }

        return ret;
    }

    void pokemon_gcnimpl::set_ability(pkmn::e_ability ability)
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _set_ability(ability);

        _libpkmgc_pokemon_uptr->PID = pkmn::calculations::generate_personality(
                                          get_species(),
                                          get_original_trainer_id(),
                                          is_shiny(),
                                          ability,
                                          get_gender(),
                                          get_nature()
                                      );
    }

    pkmn::e_ball pokemon_gcnimpl::get_ball()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        pkmn::e_ball ball = pkmn::e_ball(_libpkmgc_pokemon_uptr->ballCaughtWith);
        if((ball < pkmn::e_ball::NONE) || (ball > pkmn::e_ball::PREMIER_BALL))
        {
            ball = pkmn::e_ball::INVALID;
        }

        return ball;
    }

    void pokemon_gcnimpl::set_ball(
        pkmn::e_ball ball
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        // Try and instantiate an item_entry to validate the ball.
        pkmn::database::item_entry item(
            pkmn::database::ball_to_item(ball),
            get_game()
        );

        _libpkmgc_pokemon_uptr->ballCaughtWith = LibPkmGC::ItemIndex(ball);
    }


    int pokemon_gcnimpl::get_level_met()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->levelMet;
    }

    void pokemon_gcnimpl::set_level_met(
        int level
    )
    {
        pkmn::enforce_bounds("Level met", level, 0, 100);

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->levelMet = LibPkmGC::u8(level);
    }

    std::string pokemon_gcnimpl::get_location_met(
        bool as_egg
    )
    {
        if(as_egg)
        {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation III.");
        }
        else
        {
            boost::lock_guard<pokemon_gcnimpl> lock(*this);

            return pkmn::database::location_index_to_name(
                       _libpkmgc_pokemon_uptr->locationCaught,
                       _database_entry.get_game_id()
                   );
        }
    }

    void pokemon_gcnimpl::set_location_met(
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
            boost::lock_guard<pokemon_gcnimpl> lock(*this);

            _libpkmgc_pokemon_uptr->locationCaught = LibPkmGC::u16(pkmn::database::location_name_to_index(
                                           location,
                                           _database_entry.get_game_id()
                                       ));
        }
    }

    pkmn::e_game pokemon_gcnimpl::get_original_game()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        pkmn::e_game ret = pkmn::e_game::NONE;

        if(_libpkmgc_pokemon_uptr->version.game == LibPkmGC::Colosseum_XD)
        {
            ret = pkmn::e_game::COLOSSEUM;
        }
        else
        {
            ret = pkmn::database::game_index_to_enum(int(
                      _libpkmgc_pokemon_uptr->version.game
                  ));
        }

        return ret;
    }

    void pokemon_gcnimpl::set_original_game(pkmn::e_game game)
    {
        int generation = pkmn::database::game_enum_to_generation(game);
        if(generation != 3)
        {
            throw std::invalid_argument("Game must be from Generation III.");
        }

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        if((game == pkmn::e_game::COLOSSEUM) || (game == pkmn::e_game::XD))
        {
            _libpkmgc_pokemon_uptr->version.game = LibPkmGC::Colosseum_XD;
        }
        else
        {
            _libpkmgc_pokemon_uptr->version.game = LibPkmGC::GameIndex(
                                         pkmn::database::game_enum_to_index(game)
                                     );
        }
    }

    uint32_t pokemon_gcnimpl::get_personality() {
        return _libpkmgc_pokemon_uptr->PID;
    }

    void pokemon_gcnimpl::set_personality(
        uint32_t personality
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->PID = personality;
        _set_ability_from_personality();

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_personality();
        }
    }

    int pokemon_gcnimpl::get_experience()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return int(_libpkmgc_pokemon_uptr->experience);
    }

    void pokemon_gcnimpl::set_experience(
        int experience
    )
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        int max_experience = _database_entry.get_experience_at_level(100);
        pkmn::enforce_bounds("Experience", experience, 0, max_experience);

        _libpkmgc_pokemon_uptr->experience = LibPkmGC::u32(experience);
        _libpkmgc_pokemon_uptr->updateLevelFromExp();

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gcnimpl::get_level()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return int(_libpkmgc_pokemon_uptr->partyData.level);
    }

    void pokemon_gcnimpl::set_level(
        int level
    )
    {
        pkmn::enforce_bounds("Level", level, 1, 100);

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->partyData.level = LibPkmGC::u8(level);
        _libpkmgc_pokemon_uptr->experience = _libpkmgc_pokemon_uptr->getExpTable()[level];

        _update_stat_map();
    }

    void pokemon_gcnimpl::set_IV(
        pkmn::e_stat stat,
        int value
    )
    {
        pkmn::enforce_value_in_vector(
            "Stat",
            stat,
            pkmn::MODERN_STATS
        );
        pkmn::enforce_IV_bounds(stat, value, true);

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        const pkmgc::stat_bimap_t& STAT_BIMAP = pkmgc::get_stat_bimap();

        auto stat_bimap_iter = STAT_BIMAP.left.find(stat);
        BOOST_ASSERT(stat_bimap_iter != STAT_BIMAP.left.end());

        _libpkmgc_pokemon_uptr->IVs[int(stat_bimap_iter->second)] = LibPkmGC::u8(value);

        _IVs[stat] = value;

        _populate_party_data();
    }

    void pokemon_gcnimpl::set_marking(
        pkmn::e_marking marking,
        bool value
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Marking",
            marking,
            _markings
        );

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        switch(marking)
        {
            case pkmn::e_marking::CIRCLE:
                _libpkmgc_pokemon_uptr->markings.circle = value;
                break;

            case pkmn::e_marking::TRIANGLE:
                _libpkmgc_pokemon_uptr->markings.triangle = value;
                break;

            case pkmn::e_marking::SQUARE:
                _libpkmgc_pokemon_uptr->markings.square = value;
                break;

            case pkmn::e_marking::HEART:
                _libpkmgc_pokemon_uptr->markings.heart = value;
                break;

            default:
                BOOST_ASSERT(false);
                break;
        }

        _markings[marking] = value;
    }

    void pokemon_gcnimpl::set_ribbon(
        const std::string& ribbon,
        bool value
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Ribbon",
            ribbon,
            _ribbons
        );

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        static const pkmgc::ribbon_bimap_t& RIBBON_BIMAP = pkmgc::get_ribbon_bimap();

        // Non-contest ribbon
        if(RIBBON_BIMAP.left.count(ribbon) > 0)
        {
            _libpkmgc_pokemon_uptr->specialRibbons[int(RIBBON_BIMAP.left.at(ribbon))] = value;
            _ribbons[ribbon] = value;
        }
        else
        {
            static const pkmgc::contest_stat_bimap_t& CONTEST_STAT_BIMAP = pkmgc::get_contest_stat_bimap();

            std::vector<std::string> ribbon_parts;
            boost::split(ribbon_parts, ribbon, boost::is_any_of(" "));

            if(ribbon_parts.size() == 1)
            {
                ribbon_parts.emplace_back("");
            }

            // This should succeed, as we've already validated the ribbon name.
            auto contest_stat_iter = CONTEST_STAT_BIMAP.left.find(
                                         pkmn::string_to_contest_stat(
                                             ribbon_parts[0]
                                         )
                                     );
            BOOST_ASSERT(contest_stat_iter != CONTEST_STAT_BIMAP.left.end());

            LibPkmGC::ContestAchievementLevel contest_level = CONTEST_LEVEL_MAP.at(ribbon_parts[1]);

            // If we're removing the given ribbon, we need the next-lowest level.
            if(!value)
            {
                pkmn::decrement_enum(contest_level);
            }

            _libpkmgc_pokemon_uptr->contestAchievements[int(contest_stat_iter->second)] = contest_level;
            _update_ribbons_map();
        }
    }

    void pokemon_gcnimpl::set_contest_stat(
        pkmn::e_contest_stat stat,
        int value
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Contest stat",
            stat,
            _contest_stats
        );
        pkmn::enforce_bounds("Contest stat", value, 0, 255);

        if(stat == pkmn::e_contest_stat::FEEL)
        {
            _libpkmgc_pokemon_uptr->contestLuster = LibPkmGC::u8(value);
        }
        else
        {
            static const pkmgc::contest_stat_bimap_t& CONTEST_STAT_BIMAP = pkmgc::get_contest_stat_bimap();

            // This should have been validated above.
            auto contest_stat_iter = CONTEST_STAT_BIMAP.left.find(stat);
            BOOST_ASSERT(contest_stat_iter != CONTEST_STAT_BIMAP.left.end());

            _libpkmgc_pokemon_uptr->contestStats[int(contest_stat_iter->second)] = LibPkmGC::u8(value);
        }

        _contest_stats[stat] = value;
    }

    void pokemon_gcnimpl::set_move(
        pkmn::e_move move,
        int index
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        pkmn::database::move_entry entry(move, get_game());

        /*
         * Shadow moves are not stored in the save data itself and replace the given
         * move on demand, whether it be in-battle or on a stats screen. As such,
         * don't allow it to be set here.
         */
        if(entry.get_type() == pkmn::e_type::SHADOW)
        {
            throw std::invalid_argument("You cannot set Shadow moves.");
        }

        _libpkmgc_pokemon_uptr->moves[index].move = LibPkmGC::PokemonMoveIndex(entry.get_move_id());
        _libpkmgc_pokemon_uptr->moves[index].currentPPs = LibPkmGC::u8(entry.get_pp(0));
        _libpkmgc_pokemon_uptr->moves[index].nbPPUpsUsed = 0;

        _update_moves(index);
    }

    void pokemon_gcnimpl::set_move_pp(
        int index,
        int pp
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        // TODO: refactor to get vector of PPs
        std::vector<int> PPs;
        pkmn::database::move_entry entry(_moves[index].move, get_game());
        for(int i = 0; i < 4; ++i)
        {
            PPs.emplace_back(entry.get_pp(i));
        }

        pkmn::enforce_bounds("PP", pp, 0, PPs[3]);

        _moves[index].pp = pp;
        _libpkmgc_pokemon_uptr->moves[index].currentPPs = LibPkmGC::u8(pp);

        // Set the PP Up value to the minimum value that will accommodate the given PP.
        for(uint8_t i = 0; i < 4; ++i)
        {
            if(pp <= PPs[i])
            {
                _libpkmgc_pokemon_uptr->moves[index].nbPPUpsUsed = i;
                break;
            }
        }
    }

    void pokemon_gcnimpl::set_EV(
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

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        static const pkmgc::stat_bimap_t& STAT_BIMAP = pkmgc::get_stat_bimap();

        // This should have been validated above.
        auto stat_iter = STAT_BIMAP.left.find(stat);
        BOOST_ASSERT(stat_iter != STAT_BIMAP.left.end());

        _libpkmgc_pokemon_uptr->EVs[int(stat_iter->second)] = LibPkmGC::u8(value);
        _EVs[stat] = value;

        _populate_party_data();
    }

    int pokemon_gcnimpl::get_current_hp()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->partyData.currentHP;
    }

    void pokemon_gcnimpl::set_current_hp(
        int hp
    )
    {
        pkmn::enforce_bounds(
            "Current HP",
            hp,
            0,
            _stats[pkmn::e_stat::HP]
        );

        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->partyData.currentHP = static_cast<LibPkmGC::u16>(hp);
    }

    bool pokemon_gcnimpl::get_is_obedient()
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        return _libpkmgc_pokemon_uptr->obedient;
    }

    void pokemon_gcnimpl::set_is_obedient(bool is_obedient)
    {
        boost::lock_guard<pokemon_gcnimpl> lock(*this);

        _libpkmgc_pokemon_uptr->obedient = is_obedient;
    }

    void pokemon_gcnimpl::_populate_party_data() {
        _libpkmgc_pokemon_uptr->resetPartyData();
        _update_stat_map();
    }

    void pokemon_gcnimpl::_update_moves(
        int index
    )
    {
        _moves.resize(4);
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                // TODO: figure out converting invalid indices
                _moves[index] = pkmn::move_slot(
                    static_cast<pkmn::e_move>(_libpkmgc_pokemon_uptr->moves[index].move),
                    _libpkmgc_pokemon_uptr->moves[index].currentPPs
                );
                break;

            default:
                for(int i = 0; i < 4; ++i)
                {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gcnimpl::_update_ribbons_map()
    {
        static const pkmgc::contest_stat_bimap_t& CONTEST_STAT_BIMAP = pkmgc::get_contest_stat_bimap();

        // Contest ribbons
        for(const auto& contest_stat_iter: CONTEST_STAT_BIMAP.left)
        {
            pkmn::e_contest_stat libpkmn_contest_stat = contest_stat_iter.first;
            pkmgc::e_contest_stat libpkmgc_contest_stat = contest_stat_iter.second;

            std::string contest_stat_name = pkmn::contest_stat_to_string(libpkmn_contest_stat);

            for(const auto& contest_level_iter: CONTEST_LEVEL_MAP)
            {
                const std::string& contest_level_name = contest_level_iter.first;
                LibPkmGC::ContestAchievementLevel contest_level_enum = contest_level_iter.second;

                std::string ribbon_name = contest_stat_name;
                if(!contest_level_name.empty())
                {
                    ribbon_name.append(" ");
                    ribbon_name.append(contest_level_name);
                }

                _ribbons[ribbon_name] =
                    (_libpkmgc_pokemon_uptr->contestAchievements[int(libpkmgc_contest_stat)] >= contest_level_enum);
            }
        }

        static const pkmgc::ribbon_bimap_t& RIBBON_BIMAP = pkmgc::get_ribbon_bimap();

        // Non-contest ribbons
        for(const auto& ribbon_iter: RIBBON_BIMAP.left)
        {
            const std::string& ribbon_name = ribbon_iter.first;
            pkmgc::e_ribbon libpkmgc_ribbon = ribbon_iter.second;

            _ribbons[ribbon_name] = _libpkmgc_pokemon_uptr->specialRibbons[int(libpkmgc_ribbon)];
        }
    }

    void pokemon_gcnimpl::_update_EV_map()
    {
        static const pkmgc::stat_bimap_t& STAT_BIMAP = pkmgc::get_stat_bimap();

        for(const auto& stat_iter: STAT_BIMAP.left)
        {
            pkmn::e_stat libpkmn_stat = stat_iter.first;
            pkmgc::e_stat libpkmgc_stat = stat_iter.second;

            _EVs[libpkmn_stat] = int(_libpkmgc_pokemon_uptr->EVs[int(libpkmgc_stat)]);
        }
    }

    void pokemon_gcnimpl::_update_stat_map()
    {
        static const pkmgc::stat_bimap_t& STAT_BIMAP = pkmgc::get_stat_bimap();

        for(const auto& stat_iter: STAT_BIMAP.left)
        {
            pkmn::e_stat libpkmn_stat = stat_iter.first;
            pkmgc::e_stat libpkmgc_stat = stat_iter.second;

            _stats[libpkmn_stat] = int(_libpkmgc_pokemon_uptr->partyData.stats[int(libpkmgc_stat)]);
        }
    }

    void pokemon_gcnimpl::_set_ability(pkmn::e_ability ability)
    {
        pkmn::ability_pair_t abilities = _database_entry.get_abilities();
        if(ability == pkmn::e_ability::NONE)
        {
            throw std::invalid_argument("The ability cannot be set to None.");
        }
        else if(ability == abilities.first)
        {
                _libpkmgc_pokemon_uptr->setSecondAbilityFlag(false);
        }
        else if(ability == abilities.second)
        {
            BOOST_ASSERT(abilities.second != pkmn::e_ability::NONE);
            _libpkmgc_pokemon_uptr->setSecondAbilityFlag(true);
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

    void pokemon_gcnimpl::_set_unown_form_from_personality()
    {
        _database_entry.set_form(
            pkmn::calculations::gen3_unown_form(
                _libpkmgc_pokemon_uptr->PID
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

        uint8_t* pid_as_bytes = reinterpret_cast<uint8_t*>(&_libpkmgc_pokemon_uptr->PID);
        for(size_t i = 0; i < 4; ++i) {
            pid_as_bytes[i] &= ~0x3;
            pid_as_bytes[i] |= ((num & (0x3 << (2*i))) >> (2*i));
        }
    }

    void pokemon_gcnimpl::_init_IV_map()
    {
        static const pkmgc::stat_bimap_t& STAT_BIMAP = pkmgc::get_stat_bimap();

        for(const auto& stat_iter: STAT_BIMAP.left)
        {
            pkmn::e_stat libpkmn_stat = stat_iter.first;
            pkmgc::e_stat libpkmgc_stat = stat_iter.second;

            _IVs[libpkmn_stat] = int(_libpkmgc_pokemon_uptr->IVs[int(libpkmgc_stat)]);
        }
    }

    void pokemon_gcnimpl::_init_gcn_contest_stats_map()
    {
        static const pkmgc::contest_stat_bimap_t& CONTEST_STAT_BIMAP = pkmgc::get_contest_stat_bimap();

        for(const auto& contest_stat_iter: CONTEST_STAT_BIMAP.left)
        {
            pkmn::e_contest_stat libpkmn_contest_stat = contest_stat_iter.first;
            pkmgc::e_contest_stat libpkmgc_contest_stat = contest_stat_iter.second;

            _contest_stats[libpkmn_contest_stat] =
                int(_libpkmgc_pokemon_uptr->contestStats[int(libpkmgc_contest_stat)]);
        }

        _contest_stats[pkmn::e_contest_stat::FEEL] = int(_libpkmgc_pokemon_uptr->contestLuster);
    }

    void pokemon_gcnimpl::_init_markings_map()
    {
        _markings[pkmn::e_marking::CIRCLE]   = _libpkmgc_pokemon_uptr->markings.circle;
        _markings[pkmn::e_marking::TRIANGLE] = _libpkmgc_pokemon_uptr->markings.triangle;
        _markings[pkmn::e_marking::SQUARE]   = _libpkmgc_pokemon_uptr->markings.square;
        _markings[pkmn::e_marking::HEART]    = _libpkmgc_pokemon_uptr->markings.heart;
    }

    // TODO: region, until 3DS support brings in functions
    void pokemon_gcnimpl::_register_attributes()
    {
        using std::placeholders::_1;

        _boolean_attribute_engine.register_attribute_fcns(
            "Obedient?",
            std::bind(&pokemon_gcnimpl::get_is_obedient, this),
            std::bind(&pokemon_gcnimpl::set_is_obedient, this, _1)
        );
    }
}
