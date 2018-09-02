/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "gen3_conversions.hpp"
#include "database/database_common.hpp"
#include "pksav/pksav_call.hpp"

#include "pkmgc/enum_maps.hpp"

#include <pkmn/calculations/stats.hpp>

#include <pksav/common/condition.h>
#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gba/ribbons.h>
#include <pksav/gba/text.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/config.hpp>

// TODO: use new enums and #defines where applicable

namespace pkmn { namespace conversions {

    BOOST_STATIC_CONSTEXPR LibPkmGC::ItemIndex MAX_GBA_ITEM_INDEX = LibPkmGC::TM50;

    // TODO: PKSav enum
    typedef boost::bimap<uint16_t, LibPkmGC::LanguageIndex> gen3_language_bimap_t;
    static const gen3_language_bimap_t GEN3_LANGUAGE_BIMAP = boost::assign::list_of<gen3_language_bimap_t::relation>
        (0x0000, LibPkmGC::NoLanguage)
        (0x0201, LibPkmGC::Japanese)
        (0x0202, LibPkmGC::English)
        (0x0203, LibPkmGC::French)
        (0x0204, LibPkmGC::Italian)
        (0x0205, LibPkmGC::German)
        (0x0207, LibPkmGC::Spanish)
    ;

    typedef boost::bimap<enum pksav_condition_mask, LibPkmGC::PokemonStatus> gen3_condition_conversion_bimap_t;
    static const gen3_condition_conversion_bimap_t GEN3_CONDITION_CONVERSION_BIMAP =
    boost::assign::list_of<gen3_condition_conversion_bimap_t::relation>
        (PKSAV_CONDITION_NONE,       LibPkmGC::NoStatus)
        (PKSAV_CONDITION_SLEEP_MASK, LibPkmGC::Asleep)
        (PKSAV_CONDITION_POISON,     LibPkmGC::Poisoned)
        (PKSAV_CONDITION_BURN,       LibPkmGC::Burnt)
        (PKSAV_CONDITION_FROZEN,     LibPkmGC::Frozen)
        (PKSAV_CONDITION_PARALYZ,    LibPkmGC::Paralyzed)
        (PKSAV_CONDITION_BAD_POISON, LibPkmGC::BadlyPoisoned)
    ;

    typedef struct
    {
        uint32_t pksav_ribbon_mask;
        uint32_t pksav_ribbon_offset;
        pkmgc::e_contest_stat libpkmgc_contest_stat;
    } ribbon_values_t;
    static const std::vector<ribbon_values_t> GEN3_RIBBON_VALUES =
    {
        {PKSAV_GBA_COOL_RIBBONS_MASK,   PKSAV_GBA_COOL_RIBBONS_OFFSET,   pkmgc::e_contest_stat::COOL},
        {PKSAV_GBA_BEAUTY_RIBBONS_MASK, PKSAV_GBA_BEAUTY_RIBBONS_OFFSET, pkmgc::e_contest_stat::BEAUTY},
        {PKSAV_GBA_CUTE_RIBBONS_MASK,   PKSAV_GBA_CUTE_RIBBONS_OFFSET,   pkmgc::e_contest_stat::CUTE},
        {PKSAV_GBA_SMART_RIBBONS_MASK,  PKSAV_GBA_SMART_RIBBONS_OFFSET,  pkmgc::e_contest_stat::SMART},
        {PKSAV_GBA_TOUGH_RIBBONS_MASK,  PKSAV_GBA_TOUGH_RIBBONS_OFFSET,  pkmgc::e_contest_stat::TOUGH}
    };

    typedef boost::bimap<uint32_t, pkmgc::e_ribbon> gen3_ribbon_bimap_t;
    static const gen3_ribbon_bimap_t GEN3_RIBBON_BIMAP = boost::assign::list_of<gen3_ribbon_bimap_t::relation>
        (PKSAV_GBA_CHAMPION_RIBBON_MASK, pkmgc::e_ribbon::CHAMPION)
        (PKSAV_GBA_WINNING_RIBBON_MASK,  pkmgc::e_ribbon::WINNING)
        (PKSAV_GBA_VICTORY_RIBBON_MASK,  pkmgc::e_ribbon::VICTORY)
        (PKSAV_GBA_ARTIST_RIBBON_MASK,   pkmgc::e_ribbon::ARTIST)
        (PKSAV_GBA_EFFORT_RIBBON_MASK,   pkmgc::e_ribbon::EFFORT)
        (PKSAV_GBA_MARINE_RIBBON_MASK,   pkmgc::e_ribbon::MARINE)
        (PKSAV_GBA_LAND_RIBBON_MASK,     pkmgc::e_ribbon::LAND)
        (PKSAV_GBA_SKY_RIBBON_MASK,      pkmgc::e_ribbon::SKY)
        (PKSAV_GBA_COUNTRY_RIBBON_MASK,  pkmgc::e_ribbon::COUNTRY)
        (PKSAV_GBA_NATIONAL_RIBBON_MASK, pkmgc::e_ribbon::NATIONAL)
        (PKSAV_GBA_EARTH_RIBBON_MASK,    pkmgc::e_ribbon::EARTH)
        (PKSAV_GBA_WORLD_RIBBON_MASK,    pkmgc::e_ribbon::WORLD)
    ;

    typedef boost::bimap<enum pksav_IV, pkmgc::e_stat> gen3_IV_bimap_t;
    static const gen3_IV_bimap_t GEN3_IV_BIMAP = boost::assign::list_of<gen3_IV_bimap_t::relation>
        (PKSAV_IV_HP,      pkmgc::e_stat::HP)
        (PKSAV_IV_ATTACK,  pkmgc::e_stat::ATTACK)
        (PKSAV_IV_DEFENSE, pkmgc::e_stat::DEFENSE)
        (PKSAV_IV_SPATK,   pkmgc::e_stat::SPECIAL_ATTACK)
        (PKSAV_IV_SPDEF,   pkmgc::e_stat::SPECIAL_DEFENSE)
        (PKSAV_IV_SPEED,   pkmgc::e_stat::SPEED)
    ;

    void gba_pc_pokemon_to_gcn(
        const struct pksav_gba_pc_pokemon* from,
        LibPkmGC::GC::Pokemon* to
    )
    {
        const struct pksav_gba_pokemon_growth_block* from_growth = &from->blocks.growth;
        const struct pksav_gba_pokemon_attacks_block* from_attacks = &from->blocks.attacks;
        const struct pksav_gba_pokemon_effort_block* from_effort = &from->blocks.effort;
        const struct pksav_gba_pokemon_misc_block* from_misc = &from->blocks.misc;

        to->species = LibPkmGC::PokemonSpeciesIndex(pksav_littleendian16(from_growth->species));

        // Don't bring over GBA-exclusive items.
        if(pksav_littleendian16(from_growth->held_item) <= uint16_t(MAX_GBA_ITEM_INDEX))
        {
            to->heldItem = LibPkmGC::ItemIndex(pksav_littleendian16(from_growth->held_item));
        }
        else
        {
            to->heldItem = LibPkmGC::NoItem;
        }

        to->friendship = from_growth->friendship;
        to->locationCaught = 0; // Should be "Met in a distant land" in both games

        to->ballCaughtWith = LibPkmGC::ItemIndex(
                                 PKSAV_GBA_POKEMON_BALL(from_misc->origin_info)
                             );

        to->OTGender = (from_misc->origin_info & PKSAV_GBA_POKEMON_OTGENDER_MASK) ? LibPkmGC::Female
                                                                                  : LibPkmGC::Male;

        char otname[8] = {0};
        PKSAV_CALL(
            pksav_gba_import_text(
                from->otname,
                otname,
                7
            );
        );
        to->OTName->fromUTF8(otname);

        char nickname[11] = {0};
        PKSAV_CALL(
            pksav_gba_import_text(
                from->nickname,
                nickname,
                10
            );
        );
        to->name->fromUTF8(nickname);

        to->contestLuster = from_effort->contest_stats.sheen;
        to->pokerusStatus = from_misc->pokerus;

        to->markings.circle   = bool(from->markings & PKSAV_MARKING_CIRCLE);
        to->markings.square   = bool(from->markings & PKSAV_MARKING_SQUARE);
        to->markings.triangle = bool(from->markings & PKSAV_MARKING_TRIANGLE);
        to->markings.heart    = bool(from->markings & PKSAV_MARKING_HEART);

        to->experience = pksav_littleendian32(from_growth->exp);
        to->SID = pksav_littleendian16(from->ot_id.sid);
        to->TID = pksav_littleendian16(from->ot_id.pid);
        to->PID = pksav_littleendian32(from->personality);

        to->version.game = LibPkmGC::GameIndex(
                               PKSAV_GBA_POKEMON_ORIGIN_GAME(from_misc->origin_info)
                           );
        to->version.currentRegion = LibPkmGC::NTSC_U;
        to->version.originalRegion = LibPkmGC::NoRegion; // Not stored in GBA
        to->version.language = GEN3_LANGUAGE_BIMAP.left.at(pksav_littleendian16(from->language));

        to->obedient = bool(from_misc->ribbons_obedience & PKSAV_GBA_POKEMON_OBEDIENCE_MASK);

        for(const auto& gen3_ribbon_left_pair: GEN3_RIBBON_BIMAP.left)
        {
            to->specialRibbons[int(gen3_ribbon_left_pair.second)] =
                bool(from_misc->ribbons_obedience & gen3_ribbon_left_pair.first);
        }

        for(size_t i = 0; i < 4; ++i)
        {
            to->moves[i].move = LibPkmGC::PokemonMoveIndex(pksav_littleendian16(from_attacks->moves[i]));
            to->moves[i].currentPPs = from_attacks->move_pps[i];
            to->moves[i].nbPPUpsUsed = LibPkmGC::u8((from_growth->pp_up >> (i*2)) & 0x3);
        }

        to->EVs[int(pkmgc::e_stat::HP)]              = from_effort->ev_hp;
        to->EVs[int(pkmgc::e_stat::ATTACK)]          = from_effort->ev_atk;
        to->EVs[int(pkmgc::e_stat::DEFENSE)]         = from_effort->ev_def;
        to->EVs[int(pkmgc::e_stat::SPECIAL_ATTACK)]  = from_effort->ev_spatk;
        to->EVs[int(pkmgc::e_stat::SPECIAL_DEFENSE)] = from_effort->ev_spdef;
        to->EVs[int(pkmgc::e_stat::SPEED)]           = from_effort->ev_spd;

        uint8_t IVs[PKSAV_NUM_IVS] = {0};
        PKSAV_CALL(
            pksav_get_IVs(
                &from_misc->iv_egg_ability,
                IVs,
                sizeof(IVs)
            );
        )


        for(const auto& gen3_IV_left_pair: GEN3_IV_BIMAP.left)
        {
            to->IVs[int(gen3_IV_left_pair.second)] = IVs[gen3_IV_left_pair.first];
        }

        to->contestStats[int(pkmgc::e_contest_stat::COOL)]   = from_effort->contest_stats.cool;
        to->contestStats[int(pkmgc::e_contest_stat::BEAUTY)] = from_effort->contest_stats.beauty;
        to->contestStats[int(pkmgc::e_contest_stat::CUTE)]   = from_effort->contest_stats.cute;
        to->contestStats[int(pkmgc::e_contest_stat::SMART)]  = from_effort->contest_stats.smart;
        to->contestStats[int(pkmgc::e_contest_stat::TOUGH)]  = from_effort->contest_stats.tough;

        for(const ribbon_values_t& ribbon_values: GEN3_RIBBON_VALUES)
        {
            uint32_t contest_level = from_misc->ribbons_obedience & ribbon_values.pksav_ribbon_mask;
            contest_level >>= ribbon_values.pksav_ribbon_offset;
            to->contestAchievements[int(ribbon_values.libpkmgc_contest_stat)] =
                LibPkmGC::ContestAchievementLevel(contest_level);
        }

        // Let LibPkmGC do the work.
        to->setSecondAbilityFlag(bool(from_misc->iv_egg_ability & PKSAV_GBA_POKEMON_ABILITY_MASK));
        to->setEggFlag(bool(from_misc->iv_egg_ability & PKSAV_GBA_POKEMON_EGG_MASK));
        to->updateLevelFromExp();

        // The database lists level 1 as having 0 experience, but 0 experience
        // corresponds to level 0 in Gamecube. Address that here.
        if(!to->isEgg() && (to->partyData.level == 0))
        {
            to->partyData.level = 1;
        }

        to->updateStats();

        // Update level met for new trainer.
        to->levelMet = to->partyData.level;
    }

    // Ignore items here, deal with where it's easier to detect invalid items
    void gba_party_pokemon_to_gcn(
        const struct pksav_gba_party_pokemon* from,
        LibPkmGC::GC::Pokemon* to
    )
    {
        gba_pc_pokemon_to_gcn(
            &from->pc_data,
            to
        );

        for(const auto& condition_pair: GEN3_CONDITION_CONVERSION_BIMAP.left)
        {
            if(from->party_data.condition & condition_pair.first)
            {
                to->partyData.status = condition_pair.second;
                if(condition_pair.first == PKSAV_CONDITION_SLEEP_MASK)
                {
                    // This stores the number of turns asleep.
                    to->partyData.turnsOfSleepRemaining = LibPkmGC::u8(from->party_data.condition);
                }
            }
        }

        to->partyData.pokerusDaysRemaining = LibPkmGC::s8(from->party_data.pokerus_time);
        to->partyData.currentHP = pksav_littleendian16(from->party_data.current_hp);
    }

    void gcn_pokemon_to_gba_pc(
        const LibPkmGC::GC::Pokemon* from,
        struct pksav_gba_pc_pokemon* to
    )
    {
        std::memset(to, 0, sizeof(*to));

        to->personality = pksav_littleendian32(from->PID);
        to->ot_id.pid   = pksav_littleendian16(from->TID);
        to->ot_id.sid   = pksav_littleendian16(from->SID);

        PKSAV_CALL(
            pksav_gba_export_text(
                from->name->toUTF8(),
                to->nickname,
                10
            );
        );

        to->language = pksav_littleendian16(GEN3_LANGUAGE_BIMAP.right.at(from->version.language));

        PKSAV_CALL(
            pksav_gba_export_text(
                from->OTName->toUTF8(),
                to->otname,
                7
            );
        );

        if(from->markings.circle)
        {
            to->markings |= PKSAV_MARKING_CIRCLE;
        }
        if(from->markings.triangle)
        {
            to->markings |= PKSAV_MARKING_TRIANGLE;
        }
        if(from->markings.square)
        {
            to->markings |= PKSAV_MARKING_SQUARE;
        }
        if(from->markings.heart)
        {
            to->markings |= PKSAV_MARKING_HEART;
        }

        struct pksav_gba_pokemon_growth_block* to_growth = &to->blocks.growth;
        struct pksav_gba_pokemon_attacks_block* to_attacks = &to->blocks.attacks;
        struct pksav_gba_pokemon_effort_block* to_effort = &to->blocks.effort;
        struct pksav_gba_pokemon_misc_block* to_misc = &to->blocks.misc;

        to_growth->species = pksav_littleendian16(uint16_t(from->species));

        to_growth->exp = pksav_littleendian32(from->experience);
        to_growth->friendship = from->friendship;

        for(size_t i = 0; i < 4; ++i)
        {
            to_attacks->moves[i] = pksav_littleendian16(uint16_t(from->moves[i].move));
            to_attacks->move_pps[i] = from->moves[i].currentPPs;

            // PP Up usage is stored in a bitfield in GBA games.
            to_growth->pp_up &= ~(0x3 << (i*2));
            to_growth->pp_up |= (from->moves[i].nbPPUpsUsed & 0x3) << (i*2);
        }

        to_effort->ev_hp    = from->EVs[int(pkmgc::e_stat::HP)];
        to_effort->ev_atk   = from->EVs[int(pkmgc::e_stat::ATTACK)];
        to_effort->ev_def   = from->EVs[int(pkmgc::e_stat::DEFENSE)];
        to_effort->ev_spd   = from->EVs[int(pkmgc::e_stat::SPEED)];
        to_effort->ev_spatk = from->EVs[int(pkmgc::e_stat::SPECIAL_ATTACK)];
        to_effort->ev_spdef = from->EVs[int(pkmgc::e_stat::SPECIAL_DEFENSE)];

        to_effort->contest_stats.cool   = from->contestStats[int(pkmgc::e_contest_stat::COOL)];
        to_effort->contest_stats.beauty = from->contestStats[int(pkmgc::e_contest_stat::BEAUTY)];
        to_effort->contest_stats.cute   = from->contestStats[int(pkmgc::e_contest_stat::CUTE)];
        to_effort->contest_stats.smart  = from->contestStats[int(pkmgc::e_contest_stat::SMART)];
        to_effort->contest_stats.tough  = from->contestStats[int(pkmgc::e_contest_stat::TOUGH)];
        to_effort->contest_stats.sheen  = from->contestLuster;

        to_misc->pokerus = from->pokerusStatus;

        /*
         * Not technically an in-game trade, but based on the origin game,
         * the status screen will just say "met in a trade" regardless.
         */
        to_misc->met_location = 254;

        to_misc->origin_info = from->partyData.level;
        to_misc->origin_info |= (uint16_t(from->version.game) << PKSAV_GBA_POKEMON_ORIGIN_GAME_OFFSET);
        to_misc->origin_info |= (uint16_t(from->ballCaughtWith) << PKSAV_GBA_POKEMON_BALL_OFFSET);
        if(from->OTGender == LibPkmGC::Female)
        {
            to_misc->origin_info |= PKSAV_GBA_POKEMON_OTGENDER_MASK;
        }

        for(auto iter = GEN3_IV_BIMAP.left.begin(); iter != GEN3_IV_BIMAP.left.end(); ++iter)
        {
            PKSAV_CALL(
                pksav_set_IV(
                    iter->first,
                    from->IVs[int(iter->second)],
                    &to_misc->iv_egg_ability
                );
            );
        }

        if(from->isEgg())
        {
            to_misc->iv_egg_ability |= PKSAV_GBA_POKEMON_EGG_MASK;
        }
        if(from->hasSecondAbility())
        {
            to_misc->iv_egg_ability |= PKSAV_GBA_POKEMON_ABILITY_MASK;
        }

        for(const ribbon_values_t& ribbon_values: GEN3_RIBBON_VALUES)
        {
            uint32_t contest_level = uint32_t(from->contestAchievements[int(ribbon_values.libpkmgc_contest_stat)]);
            contest_level <<= ribbon_values.pksav_ribbon_offset;
            to_misc->ribbons_obedience |= contest_level;
        }
        for(const auto& gen3_ribbon_right_pair: GEN3_RIBBON_BIMAP.right)
        {
            if(from->specialRibbons[int(gen3_ribbon_right_pair.first)])
            {
                to_misc->ribbons_obedience |= gen3_ribbon_right_pair.second;
            }
        }
        if(from->obedient)
        {
            to_misc->ribbons_obedience |= PKSAV_GBA_POKEMON_OBEDIENCE_MASK;
        }
    }

    void gcn_pokemon_to_gba_party(
        const LibPkmGC::GC::Pokemon* from,
        struct pksav_gba_party_pokemon* to
    )
    {
        gcn_pokemon_to_gba_pc(
            from,
            &to->pc_data
        );

        for(const auto& condition_pair: GEN3_CONDITION_CONVERSION_BIMAP.right)
        {
            if(from->partyData.status == condition_pair.first)
            {
                if(condition_pair.first == LibPkmGC::Asleep)
                {
                    to->party_data.condition = pksav_littleendian32(uint32_t(from->partyData.turnsOfSleepRemaining));
                }
                else
                {
                    to->party_data.condition = uint32_t(condition_pair.second);
                }
            }
        }

        to->party_data.level        = from->partyData.level;
        to->party_data.pokerus_time = uint8_t(from->partyData.pokerusDaysRemaining);
        to->party_data.current_hp   = pksav_littleendian16(from->partyData.currentHP);
        to->party_data.max_hp       = pksav_littleendian16(from->partyData.stats[int(pkmgc::e_stat::HP)]);
        to->party_data.atk          = pksav_littleendian16(from->partyData.stats[int(pkmgc::e_stat::ATTACK)]);
        to->party_data.def          = pksav_littleendian16(from->partyData.stats[int(pkmgc::e_stat::DEFENSE)]);
        to->party_data.spd          = pksav_littleendian16(from->partyData.stats[int(pkmgc::e_stat::SPEED)]);
        to->party_data.spatk        = pksav_littleendian16(from->partyData.stats[int(pkmgc::e_stat::SPECIAL_ATTACK)]);
        to->party_data.spdef        = pksav_littleendian16(from->partyData.stats[int(pkmgc::e_stat::SPECIAL_DEFENSE)]);
    }

}}
