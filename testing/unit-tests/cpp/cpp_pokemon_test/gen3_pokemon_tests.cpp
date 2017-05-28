/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/shininess.hpp>
#include <pkmn/database/item_entry.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/text.h>
#include <pksav/math/endian.h>

#include "libpkmgc_includes.hpp"

#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/bimap.hpp>

static const std::vector<std::string> contest_types = boost::assign::list_of
    ("Cool")("Beauty")("Cute")("Smart")("Tough")
;

static const std::vector<std::string> contest_levels = boost::assign::list_of
    ("Super")("Hyper")("Master")
;

static const std::vector<std::string> ribbons = boost::assign::list_of
    ("Champion")("Winning")("Victory")("Artist")
    ("Effort")("Marine")("Land")("Sky")
    ("Country")("National")("Earth")("World")
;

class gba_pokemon_test: public pokemon_test {};
class gcn_pokemon_test: public pokemon_test {};

static void check_initial_ribbons_map(
    const std::map<std::string, bool>& ribbons_map
) {
    for(auto contest_type_iter = contest_types.begin();
        contest_type_iter != contest_types.end();
        ++contest_type_iter)
    {
        std::string ribbon_name = (*contest_type_iter);
        EXPECT_EQ(1, ribbons_map.count(ribbon_name));
        EXPECT_FALSE(ribbons_map.at(ribbon_name));

        for(auto contest_level_iter = contest_levels.begin();
            contest_level_iter != contest_levels.end();
            ++contest_level_iter)
        {
            ribbon_name = str(boost::format("%s %s")
                              % contest_type_iter->c_str()
                              % contest_level_iter->c_str());
            EXPECT_EQ(1, ribbons_map.count(ribbon_name));
            EXPECT_FALSE(ribbons_map.at(ribbon_name));
        }
    }

    for(auto ribbon_iter = ribbons.begin();
        ribbon_iter != ribbons.end();
        ++ribbon_iter)
    {
        EXPECT_EQ(1, ribbons_map.count(*ribbon_iter));
        EXPECT_FALSE(ribbons_map.at(*ribbon_iter));
    }
}

static void test_contest_ribbons(
    pkmn::pokemon::sptr pokemon,
    const std::string &contest_type
) {
    std::string ribbon_name = contest_type;
    std::string super_ribbon_name = contest_type + std::string(" Super");
    std::string hyper_ribbon_name = contest_type + std::string(" Hyper");
    std::string master_ribbon_name = contest_type + std::string(" Master");
    const std::map<std::string, bool>& ribbons = pokemon->get_ribbons();

    pokemon->set_ribbon(hyper_ribbon_name, true);
    EXPECT_TRUE(ribbons.at(ribbon_name));
    EXPECT_TRUE(ribbons.at(super_ribbon_name));
    EXPECT_TRUE(ribbons.at(hyper_ribbon_name));
    EXPECT_FALSE(ribbons.at(master_ribbon_name));

    pokemon->set_ribbon(super_ribbon_name, false);
    EXPECT_TRUE(ribbons.at(ribbon_name));
    EXPECT_FALSE(ribbons.at(super_ribbon_name));
    EXPECT_FALSE(ribbons.at(hyper_ribbon_name));
    EXPECT_FALSE(ribbons.at(master_ribbon_name));
}

static void test_ribbons(
    pkmn::pokemon::sptr pokemon,
    const std::string &ribbon
) {
    std::map<std::string, bool> ribbons_before = pokemon->get_ribbons();
    pokemon->set_ribbon(ribbon, true);
    const std::map<std::string, bool>& ribbons_after = pokemon->get_ribbons();
    for(auto ribbons_iter = ribbons_after.begin();
        ribbons_iter != ribbons_after.end();
        ++ribbons_iter)
    {
        if(ribbons_iter->first == ribbon) {
            EXPECT_TRUE(ribbons_iter->second);
        } else {
            EXPECT_EQ(ribbons_before.at(ribbons_iter->first), ribbons_iter->second);
        }
    }
}

TEST_P(gba_pokemon_test, gba_pokemon_test) {
    pkmn::pokemon::sptr pokemon = get_pokemon();

    pokemon_test_common(
        pokemon,
        {
            "Great Ball",
            {"Friend Ball", "Heal Ball"},

            "Razz Berry",
            {"Berry", "Mach Bike"},

            "Fateful encounter",
            {"Petalburg Woods", "Viridian Forest"},
            {"New Bark Town", "Twinleaf Town"},

            {"Swallow", "Flamethrower", "Return", "Fire Blast"},
            {"Shadow Sky", "Roost"},

            {"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum", "XD", "Colosseum/XD"},
            {"Gold", "HeartGold"}
        }
    );

    // Test ribbons.
    check_initial_ribbons_map(
        pokemon->get_ribbons()
    );
    for(auto contest_type_iter = contest_types.begin();
        contest_type_iter != contest_types.end();
        ++contest_type_iter)
    {
        test_contest_ribbons(pokemon, *contest_type_iter);
    }

    for(auto ribbon_iter = ribbons.begin();
        ribbon_iter != ribbons.end();
        ++ribbon_iter)
    {
        test_ribbons(pokemon, *ribbon_iter);
    }

    // Gender and personality are tied, so make sure they affect each other.
    pokemon->set_gender("Female");
    EXPECT_LT((pokemon->get_personality() & 0xFF), 0xFF);
    pokemon->set_gender("Male");
    EXPECT_EQ(0xFF, (pokemon->get_personality() & 0xFF));

    pokemon->set_personality(0x1234AB00);
    EXPECT_EQ("Female", pokemon->get_gender());
    pokemon->set_personality(0xCD5678FF);
    EXPECT_EQ("Male", pokemon->get_gender());

    // Setting shininess should affect personality.
    pokemon->set_shininess(false);
    EXPECT_FALSE(pokemon->is_shiny());
    uint32_t personality = pokemon->get_personality();

    pokemon->set_shininess(true);
    EXPECT_TRUE(pokemon->is_shiny());
    EXPECT_NE(pokemon->get_personality(), personality);

    /*
     * On the C++ level, check the underlying representation and make
     * sure our abstractions match.
     */
    const pksav_gba_pc_pokemon_t* native_pc_data = reinterpret_cast<const pksav_gba_pc_pokemon_t*>(
                                                       pokemon->get_native_pc_data()
                                                   );
    const pksav_gba_pokemon_growth_t* growth = &native_pc_data->blocks.growth;
    const pksav_gba_pokemon_attacks_t* attacks = &native_pc_data->blocks.attacks;
    const pksav_gba_pokemon_effort_t* effort = &native_pc_data->blocks.effort;
    const pksav_gba_pokemon_misc_t* misc = &native_pc_data->blocks.misc;

    EXPECT_EQ(pokemon->get_personality(), pksav_littleendian32(native_pc_data->personality));
    EXPECT_EQ(pokemon->get_trainer_id(), pksav_littleendian32(native_pc_data->ot_id.id));
    EXPECT_EQ(pokemon->get_trainer_public_id(), pksav_littleendian16(native_pc_data->ot_id.pid));
    EXPECT_EQ(pokemon->get_trainer_secret_id(), pksav_littleendian16(native_pc_data->ot_id.sid));

    char nickname[11] = {0};
    PKSAV_CALL(
        pksav_text_from_gba(
            native_pc_data->nickname,
            nickname,
            10
        );
    );
    EXPECT_EQ(pokemon->get_nickname(), std::string(nickname));

    // TODO: language

    char otname[8] = {0};
    PKSAV_CALL(
        pksav_text_from_gba(
            native_pc_data->otname,
            otname,
            7
        );
    );
    EXPECT_EQ(pokemon->get_trainer_name(), std::string(otname));

    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_CIRCLE);
    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_TRIANGLE);
    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_SQUARE);
    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_HEART);

    EXPECT_EQ(pokemon->get_database_entry().get_pokemon_index(), int(pksav_littleendian16(growth->species)));
    EXPECT_EQ(
        pkmn::database::item_entry(pokemon->get_held_item(), get_game()).get_item_index(),
        int(pksav_littleendian16(growth->held_item))
    );
    EXPECT_EQ(pokemon->get_experience(), int(pksav_littleendian32(growth->exp)));
    // TODO: PP Up
    EXPECT_EQ(pokemon->get_friendship(), int(growth->friendship));

    const pkmn::move_slots_t& move_slots = pokemon->get_moves();
    for(int i = 0; i < 4; ++i) {
        EXPECT_EQ(move_slots.at(i).move.get_move_id(), int(attacks->moves[i]));
        EXPECT_EQ(move_slots.at(i).pp, int(attacks->move_pps[i]));
    }

    const std::map<std::string, int>& EVs = pokemon->get_EVs();
    EXPECT_EQ(EVs.at("HP"), int(pksav_littleendian16(effort->ev_hp)));
    EXPECT_EQ(EVs.at("Attack"), int(pksav_littleendian16(effort->ev_atk)));
    EXPECT_EQ(EVs.at("Defense"), int(pksav_littleendian16(effort->ev_def)));
    EXPECT_EQ(EVs.at("Speed"), int(pksav_littleendian16(effort->ev_spd)));
    EXPECT_EQ(EVs.at("Special Attack"), int(pksav_littleendian16(effort->ev_spatk)));
    EXPECT_EQ(EVs.at("Special Defense"), int(pksav_littleendian16(effort->ev_spdef)));

    const std::map<std::string, int>& contest_stats = pokemon->get_contest_stats();
    EXPECT_EQ(contest_stats.at("Cool"), int(effort->contest_stats.cool));
    EXPECT_EQ(contest_stats.at("Beauty"), int(effort->contest_stats.beauty));
    EXPECT_EQ(contest_stats.at("Cute"), int(effort->contest_stats.cute));
    EXPECT_EQ(contest_stats.at("Smart"), int(effort->contest_stats.smart));
    EXPECT_EQ(contest_stats.at("Tough"), int(effort->contest_stats.tough));
    EXPECT_EQ(contest_stats.at("Feel"), int(effort->contest_stats.feel));

    // TODO: Pokérus
    // TODO: get location indices for what we set

    EXPECT_EQ(pokemon->get_level_met(), int(misc->origin_info & PKSAV_GBA_LEVEL_MET_MASK));

    // TODO: get ball ID for what we set
    EXPECT_TRUE(misc->origin_info & PKSAV_GBA_OTGENDER_MASK);

    const std::map<std::string, int>& IVs = pokemon->get_IVs();
    uint8_t IV = 0;

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_HP,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("HP"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_ATTACK,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Attack"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_DEFENSE,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Defense"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPEED,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Speed"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPATK,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Special Attack"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPDEF,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Special Defense"), int(IV));

    EXPECT_TRUE(!(misc->iv_egg_ability & PKSAV_GBA_EGG_MASK));
    EXPECT_TRUE(!(misc->iv_egg_ability & PKSAV_GBA_ABILITY_MASK));

    /*
     * Party data
     */
    const pksav_gba_pokemon_party_data_t* native_party_data = reinterpret_cast<const pksav_gba_pokemon_party_data_t*>(
                                                                      pokemon->get_native_party_data()
                                                                  );
    // TODO: condition

    EXPECT_EQ(pokemon->get_level(), int(native_party_data->level));

    // TODO: Pokérus

    const std::map<std::string, int>& stats = pokemon->get_stats();
    EXPECT_EQ(stats.at("HP"), int(pksav_littleendian16(native_party_data->current_hp)));
    EXPECT_EQ(stats.at("HP"), int(pksav_littleendian16(native_party_data->max_hp)));
    EXPECT_EQ(stats.at("Attack"), int(pksav_littleendian16(native_party_data->atk)));
    EXPECT_EQ(stats.at("Defense"), int(pksav_littleendian16(native_party_data->def)));
    EXPECT_EQ(stats.at("Speed"), int(pksav_littleendian16(native_party_data->spd)));
    EXPECT_EQ(stats.at("Special Attack"), int(pksav_littleendian16(native_party_data->spatk)));
    EXPECT_EQ(stats.at("Special Defense"), int(pksav_littleendian16(native_party_data->spdef)));
}

/*
 * LibPkmGC stores some values in arrays, with little indication as to what each
 * index actually corresponds to, so these enums make things easier.
 */

typedef enum {
    LIBPKMGC_STAT_HP = 0,
    LIBPKMGC_STAT_ATTACK,
    LIBPKMGC_STAT_DEFENSE,
    LIBPKMGC_STAT_SPATK,
    LIBPKMGC_STAT_SPDEF,
    LIBPKMGC_STAT_SPEED
} libpkmgc_stat_t;

typedef enum {
    LIBPKMGC_CONTEST_STAT_COOL = 0,
    LIBPKMGC_CONTEST_STAT_BEAUTY,
    LIBPKMGC_CONTEST_STAT_CUTE,
    LIBPKMGC_CONTEST_STAT_SMART,
    LIBPKMGC_CONTEST_STAT_TOUGH
} libpkmgc_contest_stat_t;

typedef enum {
    LIBPKMGC_RIBBON_CHAMPION = 0,
    LIBPKMGC_RIBBON_WINNING,
    LIBPKMGC_RIBBON_VICTORY,
    LIBPKMGC_RIBBON_ARTIST,
    LIBPKMGC_RIBBON_EFFORT,
    LIBPKMGC_RIBBON_MARINE,
    LIBPKMGC_RIBBON_LAND,
    LIBPKMGC_RIBBON_SKY,
    LIBPKMGC_RIBBON_COUNTRY,
    LIBPKMGC_RIBBON_NATIONAL,
    LIBPKMGC_RIBBON_EARTH,
    LIBPKMGC_RIBBON_WORLD
} libpkmgc_ribbon_t;

typedef boost::bimap<libpkmgc_contest_stat_t, std::string> contest_stat_bimap_t;
static const contest_stat_bimap_t CONTEST_STAT_BIMAP = boost::assign::list_of<contest_stat_bimap_t::relation>
    (LIBPKMGC_CONTEST_STAT_COOL,   "Cool")
    (LIBPKMGC_CONTEST_STAT_BEAUTY, "Beauty")
    (LIBPKMGC_CONTEST_STAT_CUTE,   "Cute")
    (LIBPKMGC_CONTEST_STAT_SMART,  "Smart")
    (LIBPKMGC_CONTEST_STAT_TOUGH,  "Tough")
;

static const std::map<std::string, LibPkmGC::ContestAchievementLevel> CONTEST_LEVEL_MAP =
boost::assign::map_list_of<std::string, LibPkmGC::ContestAchievementLevel>
    ("",       LibPkmGC::NormalContestWon)
    ("Super",  LibPkmGC::SuperContestWon)
    ("Hyper",  LibPkmGC::HyperContestWon)
    ("Master", LibPkmGC::MasterContestWon)
;

typedef boost::bimap<LibPkmGC::Gender, std::string> gender_bimap_t;
static const gender_bimap_t GENDER_BIMAP = boost::assign::list_of<gender_bimap_t::relation>
    (LibPkmGC::Male,       "Male")
    (LibPkmGC::Female,     "Female")
    (LibPkmGC::Genderless, "Genderless")
;

typedef boost::bimap<libpkmgc_ribbon_t, std::string> ribbon_bimap_t;
static const ribbon_bimap_t RIBBON_BIMAP = boost::assign::list_of<ribbon_bimap_t::relation>
    (LIBPKMGC_RIBBON_CHAMPION, "Champion")
    (LIBPKMGC_RIBBON_WINNING,  "Winning")
    (LIBPKMGC_RIBBON_VICTORY,  "Victory")
    (LIBPKMGC_RIBBON_ARTIST,   "Artist")
    (LIBPKMGC_RIBBON_EFFORT,   "Effort")
    (LIBPKMGC_RIBBON_MARINE,   "Marine")
    (LIBPKMGC_RIBBON_LAND,     "Land")
    (LIBPKMGC_RIBBON_SKY,      "Sky")
    (LIBPKMGC_RIBBON_COUNTRY,  "Country")
    (LIBPKMGC_RIBBON_NATIONAL, "National")
    (LIBPKMGC_RIBBON_EARTH,    "Earth")
    (LIBPKMGC_RIBBON_WORLD,    "World")
;

typedef boost::bimap<libpkmgc_stat_t, std::string> stat_bimap_t;
static const stat_bimap_t STAT_BIMAP = boost::assign::list_of<stat_bimap_t::relation>
    (LIBPKMGC_STAT_HP,      "HP")
    (LIBPKMGC_STAT_ATTACK,  "Attack")
    (LIBPKMGC_STAT_DEFENSE, "Defense")
    (LIBPKMGC_STAT_SPATK,   "Special Attack")
    (LIBPKMGC_STAT_SPDEF,   "Special Defense")
    (LIBPKMGC_STAT_SPEED,   "Speed")
;

TEST_P(gcn_pokemon_test, gcn_pokemon_test) {
    pkmn::pokemon::sptr pokemon = get_pokemon();

    pokemon_test_common(
        pokemon,
        {
            "Great Ball",
            {"Friend Ball", "Heal Ball"},

            "Razz Berry",
            {"Berry", "Mach Bike"},

            "Distant land",
            {"Phenac City", "Orre Colosseum"},
            {"New Bark Town", "Twinleaf Town"},

            {"Swallow", "Flamethrower", "Return", "Fire Blast"},
            {"Roost", "Flame Burst"},

            {"Colosseum", "XD", "Colosseum/XD", "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"},
            {"Gold", "HeartGold"}
        }
    );

    // Test ribbons.
    check_initial_ribbons_map(
        pokemon->get_ribbons()
    );
    for(auto contest_type_iter = contest_types.begin();
        contest_type_iter != contest_types.end();
        ++contest_type_iter)
    {
        test_contest_ribbons(pokemon, *contest_type_iter);
    }

    for(auto ribbon_iter = ribbons.begin();
        ribbon_iter != ribbons.end();
        ++ribbon_iter)
    {
        test_ribbons(pokemon, *ribbon_iter);
    }

    // Gender and personality are tied, so make sure they affect each other.
    pokemon->set_gender("Female");
    EXPECT_LT((pokemon->get_personality() & 0xFF), 0xFF);
    pokemon->set_gender("Male");
    EXPECT_EQ(0xFF, (pokemon->get_personality() & 0xFF));

    pokemon->set_personality(0x1234AB00);
    EXPECT_EQ("Female", pokemon->get_gender());
    pokemon->set_personality(0xCD5678FF);
    EXPECT_EQ("Male", pokemon->get_gender());

    // Setting shininess should affect personality.
    pokemon->set_shininess(false);
    EXPECT_FALSE(pokemon->is_shiny());
    uint32_t personality = pokemon->get_personality();

    pokemon->set_shininess(true);
    EXPECT_TRUE(pokemon->is_shiny());
    EXPECT_NE(pokemon->get_personality(), personality);

    /*
     * On the C++ level, check the underlying representation and make
     * sure our abstractions match.
     */
    const LibPkmGC::GC::Pokemon* native = reinterpret_cast<const LibPkmGC::GC::Pokemon*>(pokemon->get_native_pc_data());
    ASSERT_NE(nullptr, native);
    ASSERT_EQ(nullptr, pokemon->get_native_party_data());

    EXPECT_EQ(pokemon->get_database_entry().get_pokemon_index(), int(native->species));
    EXPECT_EQ(
        pkmn::database::item_entry(pokemon->get_held_item(), get_game()).get_item_index(),
        int(native->heldItem)
    );
    EXPECT_EQ(pokemon->get_friendship(), int(native->friendship));
    EXPECT_EQ(pkmn::database::item_entry(pokemon->get_ball(), get_game()).get_item_index(), int(native->ballCaughtWith));
    EXPECT_EQ(pokemon->get_level_met(), int(native->levelMet));
    // TODO: OTGender, probably bring in bimaps
    EXPECT_STREQ(pokemon->get_trainer_name().c_str(), native->OTName->toUTF8());
    EXPECT_STREQ(pokemon->get_nickname().c_str(), native->name->toUTF8());
    EXPECT_EQ(pokemon->get_contest_stats().at("Feel"), int(native->contestLuster));

    const std::map<std::string, bool>& markings = pokemon->get_markings();
    EXPECT_EQ(markings.at("Circle"), native->markings.circle);
    EXPECT_EQ(markings.at("Square"), native->markings.square);
    EXPECT_EQ(markings.at("Triangle"), native->markings.triangle);
    EXPECT_EQ(markings.at("Heart"), native->markings.heart);

    EXPECT_EQ(pokemon->get_experience(), int(native->experience));
    EXPECT_EQ(pokemon->get_trainer_secret_id(), native->SID);
    EXPECT_EQ(pokemon->get_trainer_public_id(), native->TID);
    EXPECT_EQ(pokemon->get_personality(), native->PID);

    // TODO: original game

    const pkmn::move_slots_t& moves = pokemon->get_moves();
    for(size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(moves.at(i).move.get_move_id(), int(native->moves[i].move));
        EXPECT_EQ(moves.at(i).pp, int(native->moves[i].currentPPs));
    }

    const std::map<std::string, int>& EVs = pokemon->get_EVs();
    const std::map<std::string, int>& IVs = pokemon->get_IVs();
    const std::map<std::string, int>& stats = pokemon->get_stats();
    for(auto iter = STAT_BIMAP.right.begin(); iter != STAT_BIMAP.right.end(); ++iter) {
        EXPECT_EQ(EVs.at(iter->first), int(native->EVs[iter->second]));
        EXPECT_EQ(IVs.at(iter->first), int(native->IVs[iter->second]));
        EXPECT_EQ(stats.at(iter->first), int(native->partyData.stats[iter->second]));
    }

    const std::map<std::string, bool>& ribbons = pokemon->get_ribbons();
    for(auto iter = RIBBON_BIMAP.right.begin(); iter != RIBBON_BIMAP.right.end(); ++iter) {
        EXPECT_EQ(ribbons.at(iter->first), native->specialRibbons[iter->second]);
    }

    for(auto type_iter = contest_types.begin(); type_iter != contest_types.end(); ++type_iter) {
        if(ribbons.at(*type_iter)) {
            EXPECT_GE(
                native->contestAchievements[CONTEST_STAT_BIMAP.right.at(*type_iter)],
                CONTEST_LEVEL_MAP.at("")
            );
        } else {
            EXPECT_LT(
                native->contestAchievements[CONTEST_STAT_BIMAP.right.at(*type_iter)],
                CONTEST_LEVEL_MAP.at("")
            );
        }
        for(auto level_iter = contest_levels.begin(); level_iter != contest_levels.end(); ++level_iter) {
            std::string ribbon_name = (*type_iter) + " " + (*level_iter);
            if(ribbons.at(ribbon_name)) {
                EXPECT_GE(
                    native->contestAchievements[CONTEST_STAT_BIMAP.right.at(*type_iter)],
                    CONTEST_LEVEL_MAP.at(*level_iter)
                );
            } else {
                EXPECT_LT(
                    native->contestAchievements[CONTEST_STAT_BIMAP.right.at(*type_iter)],
                    CONTEST_LEVEL_MAP.at(*level_iter)
                );
            }
        }
    }
    EXPECT_EQ(pokemon->get_level(), int(native->partyData.level));

    EXPECT_EQ(
        pkmn::calculations::modern_shiny(pokemon->get_personality(), pokemon->get_trainer_id()),
        native->isShiny()
    );
    EXPECT_NE(
        (pokemon->get_ability() == pokemon->get_database_entry().get_abilities().first),
        native->hasSecondAbility()
    );
    EXPECT_EQ(
        GENDER_BIMAP.right.at(pokemon->get_gender()),
        native->getGender()
    );
    EXPECT_EQ(
        pokemon->get_level(),
        int(native->calculateLevelFromExp())
    );
    EXPECT_GE(
        pokemon->get_experience(),
        int(native->calculateExpFromLevel(native->partyData.level))
    );
    EXPECT_LT(
        pokemon->get_experience(),
        int(native->calculateExpFromLevel(native->partyData.level+1))
    );

    // Confirm setting the Shadow form works properly under the hood.
    bool colosseum = (get_game() == "Colosseum");
    std::string species = colosseum ? "Ledian" : "Ledyba";
    LibPkmGC::u16 shadow_pokemon_id = colosseum ? 22 : 83;

    pkmn::pokemon::sptr shadow_pokemon = pkmn::pokemon::make(
                                             species,
                                             get_game(),
                                             "",
                                             50
                                         );
    const LibPkmGC::GC::Pokemon* native_shadow_pokemon = reinterpret_cast<const LibPkmGC::GC::Pokemon*>(
                                                             shadow_pokemon->get_native_pc_data()
                                                         );
    EXPECT_EQ("Standard", shadow_pokemon->get_form());
    EXPECT_EQ(0, native_shadow_pokemon->shadowPkmID);

    shadow_pokemon->set_form("Shadow");
    EXPECT_EQ("Shadow", shadow_pokemon->get_form());
    EXPECT_EQ(shadow_pokemon_id, native_shadow_pokemon->shadowPkmID);

    shadow_pokemon->set_form("Standard");
    EXPECT_EQ("Standard", shadow_pokemon->get_form());
    EXPECT_EQ(0, native_shadow_pokemon->shadowPkmID);
}

static const std::vector<std::pair<std::string, std::string>> gba_params = {
    {"Ruby", "Torchic"},
    {"Sapphire", "Mudkip"},
    {"Emerald", "Treecko"},
    {"FireRed", "Charmander"},
    {"LeafGreen", "Bulbasaur"},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gba_pokemon_test,
    gba_pokemon_test,
    ::testing::ValuesIn(gba_params)
);

static const std::vector<std::pair<std::string, std::string>> gcn_params = {
    {"Colosseum", "Espeon"},
    {"XD", "Umbreon"}
};

INSTANTIATE_TEST_CASE_P(
    cpp_gcn_pokemon_test,
    gcn_pokemon_test,
    ::testing::ValuesIn(gcn_params)
);
