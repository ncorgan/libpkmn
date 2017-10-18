/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "conversions_common.hpp"
#include "gen3_conversions.hpp"
#include "database/database_common.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/calculations/stats.hpp>

#include <pksav/common/gen3_ribbons.h>
#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gba/text.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/config.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <iostream>
#include <stdexcept>

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

    typedef struct
    {
        uint32_t pksav_ribbon_mask;
        uint32_t pksav_ribbon_offset;
        libpkmgc_contest_stat_t libpkmgc_contest_stat;
    } ribbon_values_t;
    static const std::vector<ribbon_values_t> GEN3_RIBBON_VALUES =
    {
        {PKSAV_GBA_COOL_MASK,   0,                       LIBPKMGC_CONTEST_STAT_COOL},
        {PKSAV_GBA_BEAUTY_MASK, PKSAV_GBA_BEAUTY_OFFSET, LIBPKMGC_CONTEST_STAT_BEAUTY},
        {PKSAV_GBA_CUTE_MASK,   PKSAV_GBA_CUTE_OFFSET,   LIBPKMGC_CONTEST_STAT_CUTE},
        {PKSAV_GBA_SMART_MASK,  PKSAV_GBA_SMART_OFFSET,  LIBPKMGC_CONTEST_STAT_SMART},
        {PKSAV_GBA_TOUGH_MASK,  PKSAV_GBA_TOUGH_OFFSET,  LIBPKMGC_CONTEST_STAT_TOUGH}
    };

    typedef boost::bimap<uint32_t, libpkmgc_ribbon_t> gen3_ribbon_bimap_t;
    static const gen3_ribbon_bimap_t GEN3_RIBBON_BIMAP = boost::assign::list_of<gen3_ribbon_bimap_t::relation>
        (PKSAV_GEN3_CHAMPION_RIBBON_MASK, LIBPKMGC_RIBBON_CHAMPION)
        (PKSAV_GEN3_WINNING_RIBBON_MASK,  LIBPKMGC_RIBBON_WINNING)
        (PKSAV_GEN3_VICTORY_RIBBON_MASK,  LIBPKMGC_RIBBON_VICTORY)
        (PKSAV_GEN3_ARTIST_RIBBON_MASK,   LIBPKMGC_RIBBON_ARTIST)
        (PKSAV_GEN3_EFFORT_RIBBON_MASK,   LIBPKMGC_RIBBON_EFFORT)
        (PKSAV_GEN3_MARINE_RIBBON_MASK,   LIBPKMGC_RIBBON_MARINE)
        (PKSAV_GEN3_LAND_RIBBON_MASK,     LIBPKMGC_RIBBON_LAND)
        (PKSAV_GEN3_SKY_RIBBON_MASK,      LIBPKMGC_RIBBON_SKY)
        (PKSAV_GEN3_COUNTRY_RIBBON_MASK,  LIBPKMGC_RIBBON_COUNTRY)
        (PKSAV_GEN3_NATIONAL_RIBBON_MASK, LIBPKMGC_RIBBON_NATIONAL)
        (PKSAV_GEN3_EARTH_RIBBON_MASK,    LIBPKMGC_RIBBON_EARTH)
        (PKSAV_GEN3_WORLD_RIBBON_MASK,    LIBPKMGC_RIBBON_WORLD)
    ;

    typedef boost::bimap<pksav_battle_stat_t, libpkmgc_stat_t> gen3_stat_bimap_t;
    static const gen3_stat_bimap_t GEN3_STAT_BIMAP = boost::assign::list_of<gen3_stat_bimap_t::relation>
        (PKSAV_STAT_HP,      LIBPKMGC_STAT_HP)
        (PKSAV_STAT_ATTACK,  LIBPKMGC_STAT_ATTACK)
        (PKSAV_STAT_DEFENSE, LIBPKMGC_STAT_DEFENSE)
        (PKSAV_STAT_SPATK,   LIBPKMGC_STAT_SPATK)
        (PKSAV_STAT_SPDEF,   LIBPKMGC_STAT_SPDEF)
        (PKSAV_STAT_SPEED,   LIBPKMGC_STAT_SPEED)
    ;

    void gba_pc_pokemon_to_gcn(
        const pksav_gba_pc_pokemon_t* from,
        LibPkmGC::GC::Pokemon* to
    )
    {
        const pksav_gba_pokemon_growth_t* from_growth = &from->blocks.growth;
        const pksav_gba_pokemon_attacks_t* from_attacks = &from->blocks.attacks;
        const pksav_gba_pokemon_effort_t* from_effort = &from->blocks.effort;
        const pksav_gba_pokemon_misc_t* from_misc = &from->blocks.misc;

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

        uint16_t ball = (from_misc->origin_info & PKSAV_GBA_BALL_MASK);
        ball >>= PKSAV_GBA_BALL_OFFSET;
        to->ballCaughtWith = LibPkmGC::ItemIndex(ball);

        to->OTGender = (from_misc->origin_info & PKSAV_GBA_OTGENDER_MASK) ? LibPkmGC::Female
                                                                          : LibPkmGC::Male;

        char otname[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gba(
                from->otname,
                otname,
                7
            );
        );
        to->OTName->fromUTF8(otname);

        char nickname[11] = {0};
        PKSAV_CALL(
            pksav_text_from_gba(
                from->nickname,
                nickname,
                10
            );
        );
        to->name->fromUTF8(nickname);

        to->contestLuster = from_effort->contest_stats.sheen;
        to->pokerusStatus = from_misc->pokerus;

        to->markings.circle = bool(from->markings & PKSAV_MARKING_CIRCLE);
        to->markings.square = bool(from->markings & PKSAV_MARKING_SQUARE);
        to->markings.triangle = bool(from->markings & PKSAV_MARKING_TRIANGLE);
        to->markings.heart = bool(from->markings & PKSAV_MARKING_HEART);

        to->experience = pksav_littleendian32(from_growth->exp);
        to->SID = pksav_littleendian16(from->ot_id.sid);
        to->TID = pksav_littleendian16(from->ot_id.pid);
        to->PID = pksav_littleendian32(from->personality);

        to->version.game = LibPkmGC::GameIndex(
                               (from_misc->origin_info & PKSAV_GBA_ORIGIN_GAME_MASK) >> PKSAV_GBA_ORIGIN_GAME_OFFSET
                           );
        to->version.currentRegion = LibPkmGC::NTSC_U;
        to->version.originalRegion = LibPkmGC::NoRegion; // Not stored in GBA
        to->version.language = GEN3_LANGUAGE_BIMAP.left.at(pksav_littleendian16(from->language));

        to->obedient = bool(from_misc->ribbons_obedience & PKSAV_GBA_OBEDIENCE_MASK);

        for(const auto& gen3_ribbon_left_pair: GEN3_RIBBON_BIMAP.left)
        {
            to->specialRibbons[gen3_ribbon_left_pair.second] = bool(from_misc->ribbons_obedience & gen3_ribbon_left_pair.first);
        }

        for(size_t i = 0; i < 4; ++i)
        {
            to->moves[i].move = LibPkmGC::PokemonMoveIndex(pksav_littleendian16(from_attacks->moves[i]));
            to->moves[i].currentPPs = from_attacks->move_pps[i];
            to->moves[i].nbPPUpsUsed = LibPkmGC::u8((from_growth->pp_up >> (i*2)) & 0x3);
        }

        to->EVs[LIBPKMGC_STAT_HP]      = from_effort->ev_hp;
        to->EVs[LIBPKMGC_STAT_ATTACK]  = from_effort->ev_atk;
        to->EVs[LIBPKMGC_STAT_DEFENSE] = from_effort->ev_def;
        to->EVs[LIBPKMGC_STAT_SPATK]   = from_effort->ev_spatk;
        to->EVs[LIBPKMGC_STAT_SPDEF]   = from_effort->ev_spdef;
        to->EVs[LIBPKMGC_STAT_SPEED]   = from_effort->ev_spd;

        for(const auto& gen3_stat_left_pair: GEN3_STAT_BIMAP.left)
        {
            PKSAV_CALL(
                pksav_get_IV(
                    &from_misc->iv_egg_ability,
                    gen3_stat_left_pair.first,
                    &to->IVs[gen3_stat_left_pair.second]
                );
            );
        }

        to->contestStats[LIBPKMGC_CONTEST_STAT_COOL]   = from_effort->contest_stats.cool;
        to->contestStats[LIBPKMGC_CONTEST_STAT_BEAUTY] = from_effort->contest_stats.beauty;
        to->contestStats[LIBPKMGC_CONTEST_STAT_CUTE]   = from_effort->contest_stats.cute;
        to->contestStats[LIBPKMGC_CONTEST_STAT_SMART]  = from_effort->contest_stats.smart;
        to->contestStats[LIBPKMGC_CONTEST_STAT_TOUGH]  = from_effort->contest_stats.tough;

        for(const ribbon_values_t& ribbon_values: GEN3_RIBBON_VALUES)
        {
            uint32_t contest_level = from_misc->ribbons_obedience & ribbon_values.pksav_ribbon_mask;
            contest_level >>= ribbon_values.pksav_ribbon_offset;
            to->contestAchievements[ribbon_values.libpkmgc_contest_stat] = LibPkmGC::ContestAchievementLevel(contest_level);
        }

        // Let LibPkmGC do the work.
        to->setSecondAbilityFlag(bool(from_misc->iv_egg_ability & PKSAV_GBA_ABILITY_MASK));
        to->setEggFlag(bool(from_misc->iv_egg_ability & PKSAV_GBA_EGG_MASK));
        to->updateLevelFromExp();
        to->updateStats();

        // Update level met for new trainer.
        to->levelMet = to->partyData.level;
    }

    void gba_party_pokemon_to_gcn(
        const pksav_gba_party_pokemon_t* from,
        LibPkmGC::GC::Pokemon* to
    )
    {
        gba_pc_pokemon_to_gcn(
            &from->pc,
            to
        );

        // TODO: condition
        to->partyData.pokerusDaysRemaining = LibPkmGC::s8(from->party_data.pokerus_time);
        to->partyData.currentHP = pksav_littleendian16(from->party_data.current_hp);
    }

    void gcn_pokemon_to_gba_pc(
        const LibPkmGC::GC::Pokemon* from,
        pksav_gba_pc_pokemon_t* to
    )
    {
        std::memset(to, 0, sizeof(*to));

        to->personality = pksav_littleendian32(from->PID);
        to->ot_id.pid   = pksav_littleendian16(from->TID);
        to->ot_id.sid   = pksav_littleendian16(from->SID);

        PKSAV_CALL(
            pksav_text_to_gba(
                from->name->toUTF8(),
                to->nickname,
                10
            );
        );

        to->language = pksav_littleendian16(GEN3_LANGUAGE_BIMAP.right.at(from->version.language));

        PKSAV_CALL(
            pksav_text_to_gba(
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

        pksav_gba_pokemon_growth_t* to_growth = &to->blocks.growth;
        pksav_gba_pokemon_attacks_t* to_attacks = &to->blocks.attacks;
        pksav_gba_pokemon_effort_t* to_effort = &to->blocks.effort;
        pksav_gba_pokemon_misc_t* to_misc = &to->blocks.misc;

        to_growth->species = pksav_littleendian16(uint16_t(from->species));

        // Don't bring over GCN-exclusive items.
        if(from->heldItem <= MAX_GBA_ITEM_INDEX)
        {
            to_growth->held_item = pksav_littleendian16(uint16_t(from->heldItem));
        }
        else
        {
            to_growth->held_item = 0;
        }

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

        to_effort->ev_hp    = from->EVs[LIBPKMGC_STAT_HP];
        to_effort->ev_atk   = from->EVs[LIBPKMGC_STAT_ATTACK];
        to_effort->ev_def   = from->EVs[LIBPKMGC_STAT_DEFENSE];
        to_effort->ev_spd   = from->EVs[LIBPKMGC_STAT_SPEED];
        to_effort->ev_spatk = from->EVs[LIBPKMGC_STAT_SPATK];
        to_effort->ev_spdef = from->EVs[LIBPKMGC_STAT_SPDEF];

        to_effort->contest_stats.cool   = from->contestStats[LIBPKMGC_CONTEST_STAT_COOL];
        to_effort->contest_stats.beauty = from->contestStats[LIBPKMGC_CONTEST_STAT_BEAUTY];
        to_effort->contest_stats.cute   = from->contestStats[LIBPKMGC_CONTEST_STAT_CUTE];
        to_effort->contest_stats.smart  = from->contestStats[LIBPKMGC_CONTEST_STAT_SMART];
        to_effort->contest_stats.tough  = from->contestStats[LIBPKMGC_CONTEST_STAT_TOUGH];
        to_effort->contest_stats.sheen  = from->contestLuster;

        to_misc->pokerus = from->pokerusStatus;

        /*
         * Not technically an in-game trade, but based on the origin game,
         * the status screen will just say "met in a trade" regardless.
         */
        to_misc->met_location = 254;

        to_misc->origin_info = from->partyData.level;
        to_misc->origin_info |= (uint16_t(from->version.game) << PKSAV_GBA_ORIGIN_GAME_OFFSET);
        to_misc->origin_info |= (uint16_t(from->ballCaughtWith) << PKSAV_GBA_BALL_OFFSET);
        if(from->OTGender == LibPkmGC::Female)
        {
            to_misc->origin_info |= PKSAV_GBA_OTGENDER_MASK;
        }

        for(auto iter = GEN3_STAT_BIMAP.left.begin(); iter != GEN3_STAT_BIMAP.left.end(); ++iter)
        {
            PKSAV_CALL(
                pksav_set_IV(
                    &to_misc->iv_egg_ability,
                    iter->first,
                    from->IVs[iter->second]
                );
            );
        }

        if(from->isEgg())
        {
            to_misc->iv_egg_ability |= PKSAV_GBA_EGG_MASK;
        }
        if(from->hasSecondAbility())
        {
            to_misc->iv_egg_ability |= PKSAV_GBA_ABILITY_MASK;
        }

        for(const ribbon_values_t& ribbon_values: GEN3_RIBBON_VALUES)
        {
            uint32_t contest_level = uint32_t(from->contestAchievements[ribbon_values.libpkmgc_contest_stat]);
            contest_level <<= ribbon_values.pksav_ribbon_offset;
            to_misc->ribbons_obedience |= contest_level;
        }
        for(const auto& gen3_ribbon_right_pair: GEN3_RIBBON_BIMAP.right)
        {
            if(from->specialRibbons[gen3_ribbon_right_pair.first])
            {
                to_misc->ribbons_obedience |= gen3_ribbon_right_pair.second;
            }
        }
        if(from->obedient)
        {
            to_misc->ribbons_obedience |= PKSAV_GBA_OBEDIENCE_MASK;
        }
    }

    void gcn_pokemon_to_gba_party(
        const LibPkmGC::GC::Pokemon* from,
        pksav_gba_party_pokemon_t* to
    )
    {
        gcn_pokemon_to_gba_pc(
            from,
            &to->pc
        );

        // TODO: condition
        to->party_data.level        = from->partyData.level;
        to->party_data.pokerus_time = uint8_t(from->partyData.pokerusDaysRemaining);
        to->party_data.current_hp   = pksav_littleendian16(from->partyData.currentHP);
        to->party_data.max_hp       = pksav_littleendian16(from->partyData.stats[LIBPKMGC_STAT_HP]);
        to->party_data.atk          = pksav_littleendian16(from->partyData.stats[LIBPKMGC_STAT_ATTACK]);
        to->party_data.def          = pksav_littleendian16(from->partyData.stats[LIBPKMGC_STAT_DEFENSE]);
        to->party_data.spd          = pksav_littleendian16(from->partyData.stats[LIBPKMGC_STAT_SPEED]);
        to->party_data.spatk        = pksav_littleendian16(from->partyData.stats[LIBPKMGC_STAT_SPATK]);
        to->party_data.spdef        = pksav_littleendian16(from->partyData.stats[LIBPKMGC_STAT_SPDEF]);
    }

}}
