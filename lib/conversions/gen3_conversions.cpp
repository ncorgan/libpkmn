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
#include <stdexcept>

namespace pkmn { namespace conversions {

    static pkmn::database::sptr _db;

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
        pkmn::database::get_connection(_db);

        const pksav_gba_pokemon_growth_t* from_growth = &from->blocks.growth;
        const pksav_gba_pokemon_attacks_t* from_attacks = &from->blocks.attacks;
        const pksav_gba_pokemon_effort_t* from_effort = &from->blocks.effort;
        const pksav_gba_pokemon_misc_t* from_misc = &from->blocks.misc;

        to->species = LibPkmGC::PokemonSpeciesIndex(pksav_littleendian16(from_growth->species));
        to->heldItem = LibPkmGC::ItemIndex(pksav_littleendian16(from_growth->held_item));
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

        for(auto iter = GEN3_RIBBON_BIMAP.left.begin(); iter != GEN3_RIBBON_BIMAP.left.end(); ++iter)
        {
            to->specialRibbons[iter->second] = bool(from_misc->ribbons_obedience & iter->first);
        }

        for(size_t i = 0; i < 4; ++i)
        {
            to->moves[i].move = LibPkmGC::PokemonMoveIndex(pksav_littleendian16(from_attacks->moves[i]));
            to->moves[i].currentPPs = from_attacks->move_pps[i];
            to->moves[i].nbPPUpsUsed = LibPkmGC::u8((from_growth->pp_up >> (i*2)) & 0x3);
        }

        to->EVs[LIBPKMGC_STAT_HP] = from_effort->ev_hp;
        to->EVs[LIBPKMGC_STAT_ATTACK] = from_effort->ev_atk;
        to->EVs[LIBPKMGC_STAT_DEFENSE] = from_effort->ev_def;
        to->EVs[LIBPKMGC_STAT_SPATK] = from_effort->ev_spatk;
        to->EVs[LIBPKMGC_STAT_SPDEF] = from_effort->ev_spdef;
        to->EVs[LIBPKMGC_STAT_SPEED] = from_effort->ev_spd;

        for(auto iter = GEN3_STAT_BIMAP.left.begin(); iter != GEN3_STAT_BIMAP.left.end(); ++iter)
        {
            PKSAV_CALL(
                pksav_get_IV(
                    &from_misc->iv_egg_ability,
                    iter->first,
                    &to->IVs[iter->second]
                );
            );
        }

        to->contestStats[LIBPKMGC_CONTEST_STAT_COOL]   = from_effort->contest_stats.cool;
        to->contestStats[LIBPKMGC_CONTEST_STAT_BEAUTY] = from_effort->contest_stats.beauty;
        to->contestStats[LIBPKMGC_CONTEST_STAT_CUTE]   = from_effort->contest_stats.cute;
        to->contestStats[LIBPKMGC_CONTEST_STAT_SMART]  = from_effort->contest_stats.smart;
        to->contestStats[LIBPKMGC_CONTEST_STAT_TOUGH]  = from_effort->contest_stats.tough;

        uint32_t cool_ribbons = from_misc->ribbons_obedience & PKSAV_GEN3_COOL_RIBBONS_MASK;
        to->contestAchievements[LIBPKMGC_CONTEST_STAT_COOL] = LibPkmGC::ContestAchievementLevel(cool_ribbons);

        uint32_t beauty_ribbons = from_misc->ribbons_obedience & PKSAV_GEN3_BEAUTY_RIBBONS_MASK;
        beauty_ribbons >>= PKSAV_GEN3_BEAUTY_RIBBONS_OFFSET;
        to->contestAchievements[LIBPKMGC_CONTEST_STAT_BEAUTY] = LibPkmGC::ContestAchievementLevel(beauty_ribbons);

        uint32_t cute_ribbons = from_misc->ribbons_obedience & PKSAV_GEN3_CUTE_RIBBONS_MASK;
        cute_ribbons >>= PKSAV_GEN3_CUTE_RIBBONS_OFFSET;
        to->contestAchievements[LIBPKMGC_CONTEST_STAT_CUTE] = LibPkmGC::ContestAchievementLevel(cute_ribbons);

        uint32_t smart_ribbons = from_misc->ribbons_obedience & PKSAV_GEN3_SMART_RIBBONS_MASK;
        smart_ribbons >>= PKSAV_GEN3_SMART_RIBBONS_OFFSET;
        to->contestAchievements[LIBPKMGC_CONTEST_STAT_SMART] = LibPkmGC::ContestAchievementLevel(smart_ribbons);

        uint32_t tough_ribbons = from_misc->ribbons_obedience & PKSAV_GEN3_TOUGH_RIBBONS_MASK;
        tough_ribbons >>= PKSAV_GEN3_TOUGH_RIBBONS_OFFSET;
        to->contestAchievements[LIBPKMGC_CONTEST_STAT_TOUGH] = LibPkmGC::ContestAchievementLevel(tough_ribbons);

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
        pkmn::database::get_connection(_db);
        (void)from;
        (void)to;
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

        // TODO: stats
    }

}}
