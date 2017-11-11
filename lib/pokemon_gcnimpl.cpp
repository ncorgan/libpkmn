/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "misc_common.hpp"
#include "pokemon_gcnimpl.hpp"
#include "pokemon_gbaimpl.hpp"

#include "conversions/gen3_conversions.hpp"

#include "database/database_common.hpp"
#include "database/id_to_index.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "types/rng.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/shininess.hpp>

#include <pkmn/database/item_entry.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/bimap.hpp>

#include <cstring>
#include <stdexcept>

#define GC_RCAST   (reinterpret_cast<LibPkmGC::GC::Pokemon*>(_native_pc))
#define COLO_RCAST (reinterpret_cast<LibPkmGC::Colosseum::Pokemon*>(_native_pc))
#define XD_RCAST   (reinterpret_cast<LibPkmGC::XD::Pokemon*>(_native_pc))

namespace pkmn {

    static pkmn::database::sptr _db;

    BOOST_STATIC_CONSTEXPR int UNOWN_ID = 201;

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

    pokemon_gcnimpl::pokemon_gcnimpl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        if(_database_entry.get_game_id() == COLOSSEUM) {
            _native_pc = reinterpret_cast<void*>(new LibPkmGC::Colosseum::Pokemon);
        } else {
            _native_pc = reinterpret_cast<void*>(new LibPkmGC::XD::Pokemon);
        }
        _our_pc_mem = true;

        pkmn::rng<LibPkmGC::u8> rng8;
        pkmn::rng<LibPkmGC::u32> rng32;

        GC_RCAST->species = LibPkmGC::PokemonSpeciesIndex(_database_entry.get_pokemon_index());
        GC_RCAST->heldItem = LibPkmGC::NoItem;
        GC_RCAST->friendship = LibPkmGC::u8(database_entry.get_base_friendship());
        GC_RCAST->locationCaught = 0; // Met in a distant land
        GC_RCAST->ballCaughtWith = LibPkmGC::PremierBall;
        GC_RCAST->levelMet = LibPkmGC::u8(level);
        GC_RCAST->OTGender = LibPkmGC::Male;
        GC_RCAST->OTName->fromUTF8(pkmn::pokemon::DEFAULT_TRAINER_NAME.c_str());
        GC_RCAST->name->fromUTF8(boost::algorithm::to_upper_copy(_database_entry.get_name()).c_str());
        GC_RCAST->contestLuster = 0;
        GC_RCAST->pokerusStatus = 0;
        GC_RCAST->experience = LibPkmGC::u32(_database_entry.get_experience_at_level(level));
        GC_RCAST->SID = LibPkmGC::u16(pkmn::pokemon::DEFAULT_TRAINER_ID >> 16);
        GC_RCAST->TID = LibPkmGC::u16(pkmn::pokemon::DEFAULT_TRAINER_ID & 0xFFFF);
        GC_RCAST->PID = rng32.rand();
        GC_RCAST->setSecondAbilityFlag(bool(GC_RCAST->PID & 2));

        GC_RCAST->version.game = LibPkmGC::Colosseum_XD;
        GC_RCAST->version.currentRegion = LibPkmGC::NTSC_U;
        GC_RCAST->version.originalRegion = LibPkmGC::NTSC_U;
        GC_RCAST->version.language = LibPkmGC::English;

        GC_RCAST->obedient = true;
        for(size_t i = 0; i < 12; ++i) {
            GC_RCAST->specialRibbons[i] = false;
        }
        GC_RCAST->unimplementedRibbons = 0;

        for(size_t i = 0; i < 4; ++i) {
            GC_RCAST->moves[i].move = LibPkmGC::NoMove;
            GC_RCAST->moves[i].currentPPs = 0;
            GC_RCAST->moves[i].nbPPUpsUsed = 0;
        }

        for(size_t i = 0; i < 6; ++i) {
            GC_RCAST->EVs[i] = rng8.rand();
            GC_RCAST->IVs[i] = rng8.rand(0,31);
        }

        for(size_t i = 0; i < 5; ++i) {
            GC_RCAST->contestStats[i] = 0;
            GC_RCAST->contestAchievements[i] = LibPkmGC::NoContestWon;
        }

        set_original_game("Colosseum/XD");

        // Populate abstractions
        _update_ribbons_map();
        _update_EV_map();
        _init_IV_map();
        _init_gcn_contest_stats_map();
        _init_markings_map();
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
        // Connect to database
        pkmn::database::get_connection(_db);

        _native_pc = reinterpret_cast<void*>(native);
        _our_pc_mem = false;

        // Populate abstractions
        _update_ribbons_map();
        _update_EV_map();
        _init_IV_map();
        _init_gcn_contest_stats_map();
        _init_markings_map();
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
        // Connect to database
        pkmn::database::get_connection(_db);

        _native_pc = reinterpret_cast<void*>(new LibPkmGC::Colosseum::Pokemon(native));
        _our_pc_mem = true;

        // Populate abstractions
        _update_ribbons_map();
        _update_EV_map();
        _init_IV_map();
        _init_gcn_contest_stats_map();
        _init_markings_map();
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
        // Connect to database
        pkmn::database::get_connection(_db);

        _native_pc = reinterpret_cast<void*>(new LibPkmGC::XD::Pokemon(native));
        _our_pc_mem = true;

        // Populate abstractions
        _update_ribbons_map();
        _update_EV_map();
        _init_IV_map();
        _init_gcn_contest_stats_map();
        _init_markings_map();
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

    pokemon::sptr pokemon_gcnimpl::to_game(
        const std::string& game
    )
    {
        pkmn::pokemon::sptr ret;

        int game_id = pkmn::database::game_name_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);
        switch(generation)
        {
            case 3:
                if(game_is_gamecube(game_id))
                {
                    if(game_id == COLOSSEUM)
                    {
                        LibPkmGC::Colosseum::Pokemon colosseum_pokemon;
                        if(_database_entry.get_game_id() == COLOSSEUM)
                        {
                            colosseum_pokemon = *COLO_RCAST;
                        }
                        else
                        {
                            colosseum_pokemon = LibPkmGC::Colosseum::Pokemon(*XD_RCAST);
                        }
                        ret = pkmn::make_shared<pokemon_gcnimpl>(colosseum_pokemon);
                    }
                    else
                    {
                        LibPkmGC::XD::Pokemon xd_pokemon;
                        if(_database_entry.get_game_id() == XD)
                        {
                            xd_pokemon = *XD_RCAST;
                        }
                        else
                        {
                            xd_pokemon = LibPkmGC::XD::Pokemon(*COLO_RCAST);
                        }
                        ret = pkmn::make_shared<pokemon_gcnimpl>(xd_pokemon);
                    }

                    ret->set_level_met(get_level());
                }
                else
                {
                    pksav_gba_party_pokemon_t pksav_pokemon;
                    pkmn::conversions::gcn_pokemon_to_gba_party(
                        GC_RCAST,
                        &pksav_pokemon
                    );

                    ret = pkmn::make_shared<pokemon_gbaimpl>(pksav_pokemon, game_id);
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
        const std::string &form
    ) {
        bool was_shadow = (_database_entry.get_form() == "Shadow");
        _database_entry.set_form(form);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_personality_from_form();
        }

        if(was_shadow and form != "Shadow")
        {
            GC_RCAST->shadowPkmID = 0;
        }
        else if(form == "Shadow")
        {
            static const char* shadow_query = \
                "SELECT shadow_pokemon_id FROM shadow_pokemon WHERE species_id=? AND "
                "colosseum=?";

            GC_RCAST->shadowPkmID = LibPkmGC::u16(
                                        pkmn::database::query_db_bind2<int, int, int>(
                                            _db, shadow_query,
                                            _database_entry.get_species_id(),
                                            ((_database_entry.get_game_id() == COLOSSEUM) ? 1 : 0)
                                        )
                                    );
        }
    }

    typedef boost::bimap<std::string, LibPkmGC::PokemonStatus> pokemon_status_bimap_t;
    static const pokemon_status_bimap_t POKEMON_STATUS_BIMAP =
    boost::assign::list_of<pokemon_status_bimap_t::relation>
        ("None",       LibPkmGC::NoStatus)
        ("Asleep",     LibPkmGC::Asleep)
        ("Poison",     LibPkmGC::Poisoned)
        ("Burn",       LibPkmGC::Burnt)
        ("Frozen",     LibPkmGC::Frozen)
        ("Paralysis",  LibPkmGC::Paralyzed)
        ("Bad Poison", LibPkmGC::BadlyPoisoned)
    ;

    std::string pokemon_gcnimpl::get_condition()
    {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        std::string ret = "None";
        LibPkmGC::PokemonStatus status = GC_RCAST->partyData.status;

        if(POKEMON_STATUS_BIMAP.right.count(status) > 0)
        {
            ret = POKEMON_STATUS_BIMAP.right.at(status);
        }

        return ret;
    }

    void pokemon_gcnimpl::set_condition(
        const std::string& condition
    )
    {
        auto condition_iter = POKEMON_STATUS_BIMAP.left.find(condition);

        if(condition_iter != POKEMON_STATUS_BIMAP.left.end())
        {
            boost::mutex::scoped_lock scoped_lock(_mem_mutex);

            GC_RCAST->partyData.status = condition_iter->second;

            if(condition == "Asleep")
            {
                // Sleep is stored as the number of turns asleep, so set a random value.
                GC_RCAST->partyData.turnsOfSleepRemaining = pkmn::rng<LibPkmGC::s8>().rand(1, 7);
            }
        }
        else
        {
            throw std::invalid_argument("Invalid condition.");
        }
    }

    std::string pokemon_gcnimpl::get_nickname() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

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

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->name->fromUTF8(nickname.c_str());
    }

    std::string pokemon_gcnimpl::get_gender() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return pkmn::calculations::modern_pokemon_gender(
                   _database_entry.get_name(),
                   GC_RCAST->PID
               );
    }

    void pokemon_gcnimpl::set_gender(
        const std::string &gender
    ) {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        _set_modern_gender(
            &GC_RCAST->PID,
            gender
        );

        // Unown is genderless, so don't bother setting the form.
    }

    bool pokemon_gcnimpl::is_shiny() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return GC_RCAST->isShiny();
    }

    void pokemon_gcnimpl::set_shininess(
        bool value
    ) {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

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

    std::string pokemon_gcnimpl::get_held_item()
    {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return pkmn::database::item_entry(
                   GC_RCAST->heldItem,
                   _database_entry.get_game_id()
               ).get_name();
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

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->heldItem = LibPkmGC::ItemIndex(item.get_item_index());
    }

    std::string pokemon_gcnimpl::get_trainer_name() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

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

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->OTName->fromUTF8(trainer_name.c_str());
    }

    uint16_t pokemon_gcnimpl::get_trainer_public_id() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return GC_RCAST->TID;
    }

    uint16_t pokemon_gcnimpl::get_trainer_secret_id() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return GC_RCAST->SID;
    }

    uint32_t pokemon_gcnimpl::get_trainer_id() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return uint32_t(GC_RCAST->TID) | (uint32_t(GC_RCAST->SID) << 16);
    }

    void pokemon_gcnimpl::set_trainer_public_id(
        uint16_t public_id
    ) {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->TID = public_id;
    }

    void pokemon_gcnimpl::set_trainer_secret_id(
        uint16_t secret_id
    ) {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->SID = secret_id;
    }

    void pokemon_gcnimpl::set_trainer_id(
        uint32_t id
    ) {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->TID = uint16_t(id & 0xFFFF);
        GC_RCAST->SID = uint16_t(id >> 16);
    }

    std::string pokemon_gcnimpl::get_trainer_gender() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return GENDER_BIMAP.left.at(GC_RCAST->OTGender);
    }

    void pokemon_gcnimpl::set_trainer_gender(
        const std::string &gender
    ) {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        if(gender == "Male" or gender == "Female") {
            GC_RCAST->OTGender = GENDER_BIMAP.right.at(gender);
        } else {
            throw std::invalid_argument("gender: valid values \"Male\", \"Female\"");
        }
    }

    int pokemon_gcnimpl::get_friendship() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return GC_RCAST->friendship;
    }

    void pokemon_gcnimpl::set_friendship(
        int friendship
    ) {
        if(friendship < 0 or friendship > 255) {
            pkmn::throw_out_of_range("friendship", 0 , 255);
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->friendship = LibPkmGC::u8(friendship);
    }

    std::string pokemon_gcnimpl::get_ability()
    {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        std::string ret;

        std::pair<std::string, std::string> abilities = _database_entry.get_abilities();
        if(abilities.second == "None")
        {
            ret = abilities.first;
        }
        else
        {
            // Don't use LibPkmGC's call, it has some mistakes.
            ret = GC_RCAST->hasSecondAbility() ? abilities.second : abilities.first;
        }

        return ret;
    }

    void pokemon_gcnimpl::set_ability(
        const std::string &ability
    )
    {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        std::pair<std::string, std::string> abilities = _database_entry.get_abilities();
        if(ability == "None")
        {
            throw std::invalid_argument("The ability cannot be set to None.");
        }
        else if(ability == abilities.first)
        {
            GC_RCAST->setSecondAbilityFlag(false);
        }
        else if(ability == abilities.second)
        {
            GC_RCAST->setSecondAbilityFlag(true);
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

    std::string pokemon_gcnimpl::get_ball() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return pkmn::database::ball_id_to_name(int(GC_RCAST->ballCaughtWith));
    }

    void pokemon_gcnimpl::set_ball(
        const std::string &ball
    ) {
        // Try and instantiate an item_entry to validate the ball.
        pkmn::database::item_entry item(ball, get_game());

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->ballCaughtWith = LibPkmGC::ItemIndex(item.get_item_index());
    }


    int pokemon_gcnimpl::get_level_met() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return GC_RCAST->levelMet;
    }

    void pokemon_gcnimpl::set_level_met(
        int level
    ) {
        if(level < 0 or level > 100) {
            pkmn::throw_out_of_range("Level caught", 0, 100);
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->levelMet = LibPkmGC::u8(level);
    }

    std::string pokemon_gcnimpl::get_location_met(
        bool as_egg
    ) {
        if(as_egg) {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation III.");
        } else {
            boost::mutex::scoped_lock scoped_lock(_mem_mutex);

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
            boost::mutex::scoped_lock scoped_lock(_mem_mutex);

            GC_RCAST->locationCaught = LibPkmGC::u16(pkmn::database::location_name_to_index(
                                           location,
                                           _database_entry.get_game_id()
                                       ));
        }
    }

    std::string pokemon_gcnimpl::get_original_game() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        std::string ret;

        if(GC_RCAST->version.game == LibPkmGC::Colosseum_XD) {
            ret = "Colosseum/XD";
        } else {
            ret = pkmn::database::game_index_to_name(int(
                       GC_RCAST->version.game
                   ));
        }

        return ret;
    }

    void pokemon_gcnimpl::set_original_game(
        const std::string &game
    ) {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        std::string game_to_test;
        if(game == "Colosseum/XD") {
            game_to_test = "Colosseum";
        } else {
            game_to_test = game;
        }
        int generation = pkmn::database::game_name_to_generation(game_to_test);
        if(generation != 3) {
            throw std::invalid_argument("Game must be from Generation III.");
        }

        if(game == "Colosseum" or game == "XD" or game == "Colosseum/XD") {
            GC_RCAST->version.game = LibPkmGC::Colosseum_XD;
        } else {
            GC_RCAST->version.game = LibPkmGC::GameIndex(
                                         pkmn::database::game_name_to_index(game)
                                     );
        }
    }

    uint32_t pokemon_gcnimpl::get_personality() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return GC_RCAST->PID;
    }

    void pokemon_gcnimpl::set_personality(
        uint32_t personality
    )
    {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        // TODO: personality determines ability
        GC_RCAST->PID = personality;

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_personality();
        }
    }

    int pokemon_gcnimpl::get_experience() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return int(GC_RCAST->experience);
    }

    void pokemon_gcnimpl::set_experience(
        int experience
    ) {
        int max_experience = _database_entry.get_experience_at_level(100);

        if(experience < 0 or experience > max_experience) {
            pkmn::throw_out_of_range("experience", 0, max_experience);
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->experience = LibPkmGC::u32(experience);
        GC_RCAST->updateLevelFromExp();

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gcnimpl::get_level() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return int(GC_RCAST->partyData.level);
    }

    void pokemon_gcnimpl::set_level(
        int level
    ) {
        if(level < 1 or level > 100) {
            pkmn::throw_out_of_range("level", 1, 100);
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GC_RCAST->partyData.level = LibPkmGC::u8(level);
        GC_RCAST->experience = GC_RCAST->getExpTable()[level];

        _update_stat_map();
    }

    void pokemon_gcnimpl::set_IV(
        const std::string &stat,
        int value
    ) {
        if(not pkmn::string_is_modern_stat(stat)) {
            pkmn::throw_invalid_argument("stat", pkmn::MODERN_STATS);
        } else if(not pkmn::IV_in_bounds(value, true)) {
            pkmn::throw_out_of_range(stat, 0, 31);
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        if(stat == "HP") {
            GC_RCAST->IVs[LIBPKMGC_STAT_HP] = LibPkmGC::u8(value);
        } else if(stat == "Attack") {
            GC_RCAST->IVs[LIBPKMGC_STAT_ATTACK] = LibPkmGC::u8(value);
        } else if(stat == "Defense") {
            GC_RCAST->IVs[LIBPKMGC_STAT_DEFENSE] = LibPkmGC::u8(value);
        } else if(stat == "Speed") {
            GC_RCAST->IVs[LIBPKMGC_STAT_SPEED] = LibPkmGC::u8(value);
        } else if(stat == "Special Attack") {
            GC_RCAST->IVs[LIBPKMGC_STAT_SPATK] = LibPkmGC::u8(value);
        } else {
            GC_RCAST->IVs[LIBPKMGC_STAT_SPDEF] = LibPkmGC::u8(value);
        }

        _IVs[stat] = value;

        _populate_party_data();
    }

    void pokemon_gcnimpl::set_marking(
        const std::string &marking,
        bool value
    ) {
        if(marking == "Circle") {
            GC_RCAST->markings.circle = value;
        } else if(marking == "Triangle") {
            GC_RCAST->markings.triangle = value;
        } else if(marking == "Square") {
            GC_RCAST->markings.square = value;
        } else if(marking == "Heart") {
            GC_RCAST->markings.heart = value;
        } else {
            throw std::invalid_argument("Invalid marking.");
        }

        _markings[marking] = value;
    }

    void pokemon_gcnimpl::set_ribbon(
        const std::string &ribbon,
        bool value
    ) {
        if(_ribbons.find(ribbon) == _ribbons.end()) {
            throw std::invalid_argument("Invalid ribbon.");
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        // Non-contest ribbon
        if(RIBBON_BIMAP.right.count(ribbon) > 0) {
            GC_RCAST->specialRibbons[RIBBON_BIMAP.right.at(ribbon)] = value;
            _ribbons[ribbon] = value;
        } else {
            std::vector<std::string> ribbon_parts;
            boost::split(ribbon_parts, ribbon, boost::is_any_of(" "));

            // Validate input (which should already have been validated)
            if((ribbon_parts.size() == 0 or ribbon_parts.size() > 2) or
               (CONTEST_STAT_BIMAP.right.count(ribbon_parts.at(0)) == 0) or
               (ribbon_parts.size() == 2 and
                CONTEST_LEVEL_MAP.count(ribbon_parts.at(1)) == 0)
              )
            {
                throw std::invalid_argument("Invalid ribbon.");
            }

            if(ribbon_parts.size() == 1)
            {
                ribbon_parts.emplace_back("");
            }

            GC_RCAST->contestAchievements[CONTEST_STAT_BIMAP.right.at(ribbon_parts[0])] =
                value ? CONTEST_LEVEL_MAP.at(ribbon_parts[1])
                      : LibPkmGC::ContestAchievementLevel(CONTEST_LEVEL_MAP.at(ribbon_parts[1])-1);

            _update_ribbons_map();
        }
    }

    void pokemon_gcnimpl::set_contest_stat(
        const std::string &stat,
        int value
    ) {
        if(value < 0 or value > 255) {
            pkmn::throw_out_of_range("value", 0, 255);
        }

        if(stat == "Cool") {
            GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_COOL] = LibPkmGC::u8(value);
        } else if(stat == "Beauty") {
            GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_BEAUTY] = LibPkmGC::u8(value);
        } else if(stat == "Cute") {
            GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_CUTE] = LibPkmGC::u8(value);
        } else if(stat == "Smart") {
            GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_SMART] = LibPkmGC::u8(value);
        } else if(stat == "Tough") {
            GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_TOUGH] = LibPkmGC::u8(value);
        } else if(stat == "Feel") {
            GC_RCAST->contestLuster = LibPkmGC::u8(value);
        } else {
            throw std::invalid_argument("Invalid contest stat.");
        }

        _contest_stats[stat] = value;
    }

    void pokemon_gcnimpl::set_move(
        const std::string &move,
        int index
    ) {
        if(index < 0 or index > 3) {
            pkmn::throw_out_of_range("index", 0, 3);
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        pkmn::database::move_entry entry(move, get_game());

        /*
         * Shadow moves are not stored in the save data itself and replace the given
         * move on demand, whether it be in-battle or on a stats screen. As such,
         * don't allow it to be set here.
         */
        if(entry.get_type() == "Shadow")
        {
            throw std::invalid_argument("You cannot set Shadow moves.");
        }

        GC_RCAST->moves[index].move = LibPkmGC::PokemonMoveIndex(entry.get_move_id());
        GC_RCAST->moves[index].currentPPs = LibPkmGC::u8(entry.get_pp(0));
        GC_RCAST->moves[index].nbPPUpsUsed = 0;

        _update_moves(index);
    }

    void pokemon_gcnimpl::set_EV(
        const std::string &stat,
        int value
    ) {
        if(not pkmn::string_is_modern_stat(stat)) {
            pkmn::throw_invalid_argument("stat", pkmn::MODERN_STATS);
        } else if(not pkmn::EV_in_bounds(value, true)) {
            pkmn::throw_out_of_range(stat, 0, 255);
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        if(stat == "HP") {
            GC_RCAST->EVs[LIBPKMGC_STAT_HP] = LibPkmGC::u8(value);
        } else if(stat == "Attack") {
            GC_RCAST->EVs[LIBPKMGC_STAT_ATTACK] = LibPkmGC::u8(value);
        } else if(stat == "Defense") {
            GC_RCAST->EVs[LIBPKMGC_STAT_DEFENSE] = LibPkmGC::u8(value);
        } else if(stat == "Speed") {
            GC_RCAST->EVs[LIBPKMGC_STAT_SPEED] = LibPkmGC::u8(value);
        } else if(stat == "Special Attack") {
            GC_RCAST->EVs[LIBPKMGC_STAT_SPATK] = LibPkmGC::u8(value);
        } else {
            GC_RCAST->EVs[LIBPKMGC_STAT_SPDEF] = LibPkmGC::u8(value);
        }

        _EVs[stat] = value;

        _populate_party_data();
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
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_id_to_name(
                        int(GC_RCAST->moves[index].move), 3
                    ),
                    GC_RCAST->moves[index].currentPPs
                );
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gcnimpl::_update_ribbons_map() {
        // Contest ribbons
        for(auto iter = CONTEST_STAT_BIMAP.right.begin(); iter != CONTEST_STAT_BIMAP.right.end(); ++iter) {
            _ribbons[iter->first]             = (GC_RCAST->contestAchievements[iter->second] >= LibPkmGC::NormalContestWon);
            _ribbons[iter->first + " Super"]  = (GC_RCAST->contestAchievements[iter->second] >= LibPkmGC::SuperContestWon);
            _ribbons[iter->first + " Hyper"]  = (GC_RCAST->contestAchievements[iter->second] >= LibPkmGC::HyperContestWon);
            _ribbons[iter->first + " Master"] = (GC_RCAST->contestAchievements[iter->second] >= LibPkmGC::MasterContestWon);
        }

        // Non-contest ribbons
        for(auto iter = RIBBON_BIMAP.right.begin(); iter != RIBBON_BIMAP.right.end(); ++iter) {
            _ribbons[iter->first] = GC_RCAST->specialRibbons[iter->second];
        }
    }

    void pokemon_gcnimpl::_update_EV_map() {
        _EVs["HP"]              = int(GC_RCAST->EVs[LIBPKMGC_STAT_HP]);
        _EVs["Attack"]          = int(GC_RCAST->EVs[LIBPKMGC_STAT_ATTACK]);
        _EVs["Defense"]         = int(GC_RCAST->EVs[LIBPKMGC_STAT_DEFENSE]);
        _EVs["Speed"]           = int(GC_RCAST->EVs[LIBPKMGC_STAT_SPEED]);
        _EVs["Special Attack"]  = int(GC_RCAST->EVs[LIBPKMGC_STAT_SPATK]);
        _EVs["Special Defense"] = int(GC_RCAST->EVs[LIBPKMGC_STAT_SPDEF]);
    }

    void pokemon_gcnimpl::_update_stat_map() {
        _stats["HP"]              = int(GC_RCAST->partyData.stats[LIBPKMGC_STAT_HP]);
        _stats["Attack"]          = int(GC_RCAST->partyData.stats[LIBPKMGC_STAT_ATTACK]);
        _stats["Defense"]         = int(GC_RCAST->partyData.stats[LIBPKMGC_STAT_DEFENSE]);
        _stats["Speed"]           = int(GC_RCAST->partyData.stats[LIBPKMGC_STAT_SPEED]);
        _stats["Special Attack"]  = int(GC_RCAST->partyData.stats[LIBPKMGC_STAT_SPATK]);
        _stats["Special Defense"] = int(GC_RCAST->partyData.stats[LIBPKMGC_STAT_SPDEF]);
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

    void pokemon_gcnimpl::_init_IV_map() {
        _IVs["HP"]              = int(GC_RCAST->IVs[LIBPKMGC_STAT_HP]);
        _IVs["Attack"]          = int(GC_RCAST->IVs[LIBPKMGC_STAT_ATTACK]);
        _IVs["Defense"]         = int(GC_RCAST->IVs[LIBPKMGC_STAT_DEFENSE]);
        _IVs["Speed"]           = int(GC_RCAST->IVs[LIBPKMGC_STAT_SPEED]);
        _IVs["Special Attack"]  = int(GC_RCAST->IVs[LIBPKMGC_STAT_SPATK]);
        _IVs["Special Defense"] = int(GC_RCAST->IVs[LIBPKMGC_STAT_SPDEF]);
    }

    void pokemon_gcnimpl::_init_gcn_contest_stats_map() {
        _contest_stats["Cool"]   = int(GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_COOL]);
        _contest_stats["Beauty"] = int(GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_BEAUTY]);
        _contest_stats["Cute"]   = int(GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_CUTE]);
        _contest_stats["Smart"]  = int(GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_SMART]);
        _contest_stats["Tough"]  = int(GC_RCAST->contestStats[LIBPKMGC_CONTEST_STAT_TOUGH]);
        _contest_stats["Feel"]   = int(GC_RCAST->contestLuster);
    }

    void pokemon_gcnimpl::_init_markings_map() {
        _markings["Circle"]   = GC_RCAST->markings.circle;
        _markings["Triangle"] = GC_RCAST->markings.triangle;
        _markings["Square"]   = GC_RCAST->markings.square;
        _markings["Heart"]    = GC_RCAST->markings.heart;
    }
}
