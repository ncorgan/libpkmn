/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "misc_common.hpp"
#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"

#include "conversions/gb_conversions.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/shininess.hpp>

#include <pkmn/database/item_entry.hpp>

#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include "types/rng.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen2/time.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <stdexcept>

#define GEN2_PC_RCAST    (reinterpret_cast<pksav_gen2_pc_pokemon_t*>(_native_pc))
#define GEN2_PARTY_RCAST (reinterpret_cast<pksav_gen2_pokemon_party_data_t*>(_native_party))

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int UNOWN_ID = 201;

    pokemon_gen2impl::pokemon_gen2impl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gen2_pc_pokemon_t);
        std::memset(_native_pc, 0, sizeof(pksav_gen2_pc_pokemon_t));
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen2_pokemon_party_data_t);
        std::memset(_native_party, 0, sizeof(pksav_gen2_pokemon_party_data_t));
        _our_party_mem = true;

        _nickname = boost::algorithm::to_upper_copy(
                        _database_entry.get_name()
                    );
        _trainer_name = DEFAULT_TRAINER_NAME;

        // Set internal members
        GEN2_PC_RCAST->species = uint8_t(_database_entry.get_pokemon_index());

        GEN2_PC_RCAST->ot_id = pksav_bigendian16(uint16_t(DEFAULT_TRAINER_ID & 0xFFFF));

        pkmn::rng<uint16_t> rng;
        GEN2_PC_RCAST->ev_hp   = rng.rand();
        GEN2_PC_RCAST->ev_atk  = rng.rand();
        GEN2_PC_RCAST->ev_def  = rng.rand();
        GEN2_PC_RCAST->ev_spd  = rng.rand();
        GEN2_PC_RCAST->ev_spcl = rng.rand();

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_IVs_from_form(
                _database_entry.get_form()
            );
        } else {
            GEN2_PC_RCAST->iv_data = rng.rand();
        }

        GEN2_PC_RCAST->friendship = uint8_t(_database_entry.get_base_friendship());

        // The max for level met is 63, but that shouldn't restrict this.
        set_level_met(std::min<int>(level, 63));
        set_location_met("Special", false);

        time_t now = 0;
        std::time(&now);
        PKSAV_CALL(
            pksav_gen2_set_caught_data_time_field(
                &now,
                &GEN2_PC_RCAST->caught_data
            );
        )

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        set_level(level);
        _update_moves(-1);
    }

    pokemon_gen2impl::pokemon_gen2impl(
        pksav_gen2_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(pc->species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_gen2_pokemon_party_data_t);
        _populate_party_data();
        _our_party_mem = true;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_IVs();
        }
    }

    pokemon_gen2impl::pokemon_gen2impl(
        pksav_gen2_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(party->pc.species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_IVs();
        }
    }

    pokemon_gen2impl::pokemon_gen2impl(
        const pksav_gen2_pc_pokemon_t& pc,
        int game_id
    ): pokemon_impl(pc.species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(new pksav_gen2_pc_pokemon_t);
        *GEN2_PC_RCAST = pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen2_pokemon_party_data_t);
        _populate_party_data();
        _our_party_mem = true;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_IVs();
        }
    }

    pokemon_gen2impl::pokemon_gen2impl(
        const pksav_gen2_party_pokemon_t& party,
        int game_id
    ): pokemon_impl(party.pc.species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(new pksav_gen2_pc_pokemon_t);
        *GEN2_PC_RCAST = party.pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen2_pokemon_party_data_t);
        *GEN2_PARTY_RCAST = party.party_data;
        _our_party_mem = true;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_IVs();
        }
    }

    pokemon_gen2impl::~pokemon_gen2impl() {
        if(_our_pc_mem) {
            delete GEN2_PC_RCAST;
        }
        if(_our_party_mem) {
            delete GEN2_PARTY_RCAST;
        }
    }

    pokemon::sptr pokemon_gen2impl::to_game(
        const std::string& game
    )
    {
        pkmn::pokemon::sptr ret;

        pksav_gen2_party_pokemon_t pksav_pokemon;
        pksav_pokemon.pc = *GEN2_PC_RCAST;
        pksav_pokemon.party_data = *GEN2_PARTY_RCAST;

        int game_id = pkmn::database::game_name_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);
        switch(generation)
        {
            case 1:
            {
                pksav_gen1_party_pokemon_t gen1_pksav_pokemon;
                pkmn::conversions::gen2_party_pokemon_to_gen1(
                    &pksav_pokemon,
                    &gen1_pksav_pokemon
                );
                ret = pkmn::make_shared<pokemon_gen1impl>(gen1_pksav_pokemon, game_id);
                break;
            }

            case 2:
            {
                ret = pkmn::make_shared<pokemon_gen2impl>(pksav_pokemon, game_id);
                // 63 is the max this value can be.
                ret->set_level_met(std::min<int>(63, get_level()));
                break;
            }

            default:
                throw std::invalid_argument("Generation II Pokémon can only be converted to Generation I-II.");
        }

        ret->set_nickname(get_nickname());
        ret->set_trainer_name(get_trainer_name());

        return ret;
    }

    void pokemon_gen2impl::set_form(
        const std::string &form
    ) {
        _database_entry.set_form(form);

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_IVs_from_form(form);
        }
    }

    std::string pokemon_gen2impl::get_nickname() {
        return _nickname;
    }

    void pokemon_gen2impl::set_nickname(
        const std::string &nickname
    ) {
        if(nickname.size() < 1 or nickname.size() > 10) {
            throw std::invalid_argument(
                      "The nickname length must be 1-10."
                  );
        }

        _nickname = nickname;
    }

    std::string pokemon_gen2impl::get_gender() {
        return pkmn::calculations::gen2_pokemon_gender(
                   _database_entry.get_name(),
                   _IVs["Attack"]
               );
    }

    void pokemon_gen2impl::set_gender(
        const std::string &gender
    ) {
        float chance_male = _database_entry.get_chance_male();
        float chance_female = _database_entry.get_chance_female();

        if(pkmn::floats_close(chance_male, 0.0f) and pkmn::floats_close(chance_female, 0.0f)) {
            if(gender != "Genderless") {
                throw std::invalid_argument("This Pokémon is genderless.");
            }
        } else {
            if(gender == "Male") {
                if(pkmn::floats_close(chance_male, 0.0f)) {
                    throw std::invalid_argument("This Pokémon is female-only.");
                } else {
                    set_IV("Attack", 15);
                }
            } else if(gender == "Female") {
                if(pkmn::floats_close(chance_female, 0.0f)) {
                    throw std::invalid_argument("This Pokémon is male-only.");
                } else {
                    // Set the IV to the max it can be while still being female.
                    if(pkmn::floats_close(chance_male, 0.875f)) {
                        set_IV("Attack", 1);
                    } else if(pkmn::floats_close(chance_male, 0.75f)) {
                        set_IV("Attack", 3);
                    } else if(pkmn::floats_close(chance_male, 0.5f)) {
                        set_IV("Attack", 6);
                    } else {
                        set_IV("Attack", 11);
                    }
                }
            } else {
                throw std::invalid_argument("gender: valid options \"Male\", \"Female\"");
            }
        }
    }

    bool pokemon_gen2impl::is_shiny() {
        return pkmn::calculations::gen2_shiny(
                   _IVs["Attack"],
                   _IVs["Defense"],
                   _IVs["Speed"],
                   _IVs["Special"]
               );
    }

    void pokemon_gen2impl::set_shininess(
        bool value
    ) {
        if(value) {
            /*
             * This value sets all IVs to the maximum values that result in
             * a shiny Pokémon.
             */
            boost::mutex::scoped_lock scoped_lock(_mem_mutex);
            static const uint16_t shiny_iv_data = pksav_littleendian16(0xFAAA);
            GEN2_PC_RCAST->iv_data = shiny_iv_data;
            _init_gb_IV_map(&GEN2_PC_RCAST->iv_data);
        } else {
            set_IV("Attack", 13);
        }

        if(_database_entry.get_species_id() == UNOWN_ID) {
            _set_unown_form_from_IVs();
        }
    }

    std::string pokemon_gen2impl::get_held_item()
    {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return pkmn::database::item_index_to_name(
                   GEN2_PC_RCAST->held_item,
                   _database_entry.get_game_id()
               );
    }

    void pokemon_gen2impl::set_held_item(
        const std::string &held_item
    ) {
        // Make sure item is valid and is holdable
        pkmn::database::item_entry item(
            held_item,
            get_game()
        );

        if(not item.holdable()) {
            throw std::invalid_argument("This item is not holdable.");
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GEN2_PC_RCAST->held_item = uint8_t(item.get_item_index());
    }

    std::string pokemon_gen2impl::get_trainer_name() {
        return _trainer_name;
    }

    void pokemon_gen2impl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() < 1 or trainer_name.size() > 7) {
            throw std::invalid_argument(
                      "The trainer name length must be 1-7."
                  );
        }

        _trainer_name = trainer_name;
    }

    uint16_t pokemon_gen2impl::get_trainer_public_id() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return pksav_bigendian16(GEN2_PC_RCAST->ot_id);
    }

    uint16_t pokemon_gen2impl::get_trainer_secret_id() {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation II");
    }

    uint32_t pokemon_gen2impl::get_trainer_id() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return uint32_t(pksav_bigendian16(GEN2_PC_RCAST->ot_id));
    }

    void pokemon_gen2impl::set_trainer_public_id(
        uint16_t public_id
    ) {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GEN2_PC_RCAST->ot_id = pksav_bigendian16(public_id);
    }

    void pokemon_gen2impl::set_trainer_secret_id(
        PKMN_UNUSED(uint16_t secret_id)
    ) {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation II");
    }

    void pokemon_gen2impl::set_trainer_id(
        uint32_t id
    )
    {
        pkmn::enforce_gb_trainer_id_bounds(id);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GEN2_PC_RCAST->ot_id = pksav_bigendian16(uint16_t(id));
    }

    std::string pokemon_gen2impl::get_trainer_gender() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return (GEN2_PC_RCAST->caught_data & PKSAV_GEN2_OT_GENDER_MASK) ? "Female"
                                                                        : "Male";
    }

    void pokemon_gen2impl::set_trainer_gender(
        const std::string &gender
    ) {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        if(gender == "Male") {
            GEN2_PC_RCAST->caught_data &= ~PKSAV_GEN2_OT_GENDER_MASK;
        } else if(gender == "Female") {
            GEN2_PC_RCAST->caught_data |= PKSAV_GEN2_OT_GENDER_MASK;
        } else {
            throw std::invalid_argument("gender: valid values \"Male\", \"Female\"");
        }
    }

    int pokemon_gen2impl::get_friendship() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return GEN2_PC_RCAST->friendship;
    }

    void pokemon_gen2impl::set_friendship(
        int friendship
    )
    {
        pkmn::enforce_bounds("Friendship", friendship, 0, 255);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GEN2_PC_RCAST->friendship = uint8_t(friendship);
    }

    std::string pokemon_gen2impl::get_ability() {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation II");
    }

    void pokemon_gen2impl::set_ability(
        PKMN_UNUSED(const std::string &ability)
    ) {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation II");
    }

    std::string pokemon_gen2impl::get_ball() {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation II.");
    }

    void pokemon_gen2impl::set_ball(
        PKMN_UNUSED(const std::string &ball)
    ) {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation II.");
    }

    int pokemon_gen2impl::get_level_met() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return (GEN2_PC_RCAST->caught_data & PKSAV_GEN2_LEVEL_CAUGHT_MASK) >> PKSAV_GEN2_LEVEL_CAUGHT_OFFSET;
    }

    void pokemon_gen2impl::set_level_met(
        int level
    )
    {
        pkmn::enforce_bounds("Level met", level, 2, 63);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        uint16_t caught_data = uint16_t(level);
        caught_data <<= PKSAV_GEN2_LEVEL_CAUGHT_OFFSET;

        GEN2_PC_RCAST->caught_data &= ~PKSAV_GEN2_LEVEL_CAUGHT_MASK;
        GEN2_PC_RCAST->caught_data |= caught_data;
    }

    std::string pokemon_gen2impl::get_location_met(
        bool as_egg
    ) {
        if(as_egg) {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation II.");
        } else {
            boost::mutex::scoped_lock scoped_lock(_mem_mutex);

            return pkmn::database::location_index_to_name(
                       (GEN2_PC_RCAST->caught_data & PKSAV_GEN2_LOCATION_MASK),
                       _database_entry.get_game_id()
                   );
        }
    }

    void pokemon_gen2impl::set_location_met(
        const std::string &location,
        bool as_egg
    ) {
        if(as_egg) {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation II.");
        } else {
            boost::mutex::scoped_lock scoped_lock(_mem_mutex);

            uint16_t location_index = uint16_t(pkmn::database::location_name_to_index(
                                                   location,
                                                   _database_entry.get_game_id()
                                               ));
            GEN2_PC_RCAST->caught_data &= ~PKSAV_GEN2_LOCATION_MASK;
            GEN2_PC_RCAST->caught_data |= location_index;
        }
    }

    std::string pokemon_gen2impl::get_original_game() {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation II.");
    }

    void pokemon_gen2impl::set_original_game(
        PKMN_UNUSED(const std::string &game)
    ) {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation II.");
    }

    uint32_t pokemon_gen2impl::get_personality() {
        throw pkmn::feature_not_in_game_error("Personality", "Generation II");
    }

    void pokemon_gen2impl::set_personality(
        PKMN_UNUSED(uint32_t personality)
    ) {
        throw pkmn::feature_not_in_game_error("Personality", "Generation II");
    }

    int pokemon_gen2impl::get_experience() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        uint32_t ret = 0;
        PKSAV_CALL(
            pksav_from_base256(
                GEN2_PC_RCAST->exp,
                3,
                &ret
            );
        )

        return int(ret);
    }

    void pokemon_gen2impl::set_experience(
        int experience
    ) {
        int max_experience = _database_entry.get_experience_at_level(100);
        pkmn::enforce_bounds("Experience", experience, 0, max_experience);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        PKSAV_CALL(
            pksav_to_base256(
                experience,
                GEN2_PC_RCAST->exp,
                3
            );
        )

        GEN2_PC_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gen2impl::get_level() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return int(GEN2_PC_RCAST->level);
    }

    void pokemon_gen2impl::set_level(
        int level
    )
    {
        pkmn::enforce_bounds("Level", level, 2, 100);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        GEN2_PC_RCAST->level = uint8_t(level);

        PKSAV_CALL(
            pksav_to_base256(
                uint32_t(_database_entry.get_experience_at_level(level)),
                GEN2_PC_RCAST->exp,
                3
            );
        )

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_gen2impl::set_IV(
        const std::string &stat,
        int value
    ) {
        _set_gb_IV(
            stat,
            value,
            &GEN2_PC_RCAST->iv_data
        );

        if(_database_entry.get_species_id() == UNOWN_ID) {
            boost::mutex::scoped_lock scoped_lock(_mem_mutex);
            _set_unown_form_from_IVs();
        }
    }

    void pokemon_gen2impl::set_marking(
        PKMN_UNUSED(const std::string &marking),
        PKMN_UNUSED(bool value)
    ) {
        throw pkmn::feature_not_in_game_error("Markings", "Generation II");
    }

    void pokemon_gen2impl::set_ribbon(
        PKMN_UNUSED(const std::string &ribbon),
        PKMN_UNUSED(bool value)
    ) {
        throw pkmn::feature_not_in_game_error("Ribbons", "Generation II");
    }

    void pokemon_gen2impl::set_contest_stat(
        PKMN_UNUSED(const std::string &stat),
        PKMN_UNUSED(int value)
    ) {
        throw pkmn::feature_not_in_game_error("Contests", "Generation II");
    }

    void pokemon_gen2impl::set_move(
        const std::string &move,
        int index
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        // This will throw an error if the move is invalid.
        pkmn::database::move_entry entry(
            move,
            get_game()
        );
        _moves[index].move = entry.get_name();
        _moves[index].pp   = entry.get_pp(0);

        GEN2_PC_RCAST->moves[index] = uint8_t(entry.get_move_id());
        GEN2_PC_RCAST->move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gen2impl::_populate_party_data() {
        pksav::gen2_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_gen2_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gen2_pokemon_party_data_t*>(_native_party)
        );

        _update_stat_map();
    }

    void pokemon_gen2impl::set_EV(
        const std::string &stat,
        int value
    )
    {
        // Generation II uses Generation I stats for EV's
        if(not pkmn::string_is_gen1_stat(stat)) {
            pkmn::throw_invalid_argument("stat", pkmn::GEN1_STATS);
        }

        pkmn::enforce_EV_bounds(stat, value, false);

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        if(stat == "HP")
        {
            GEN2_PC_RCAST->ev_hp = pksav_bigendian16(uint16_t(value));
        }
        else if(stat == "Attack")
        {
            GEN2_PC_RCAST->ev_atk = pksav_bigendian16(uint16_t(value));
        }
        else if(stat == "Defense")
        {
            GEN2_PC_RCAST->ev_def = pksav_bigendian16(uint16_t(value));
        }
        else if(stat == "Speed")
        {
            GEN2_PC_RCAST->ev_spd = pksav_bigendian16(uint16_t(value));
        }
        else
        {
            GEN2_PC_RCAST->ev_spcl = pksav_bigendian16(uint16_t(value));
        }

        _update_EV_map();
        _populate_party_data();
    }

    void pokemon_gen2impl::_update_moves(
        int index
    ) {
        if(_moves.size() != 4) {
            _moves.resize(4);
        }
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_id_to_name(
                        GEN2_PC_RCAST->moves[index], 2
                    ),
                    (GEN2_PC_RCAST->move_pps[index] & PKSAV_GEN2_MOVE_PP_MASK)
                );
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gen2impl::_update_EV_map() {
        _EVs["HP"]      = int(pksav_bigendian16(GEN2_PC_RCAST->ev_hp));
        _EVs["Attack"]  = int(pksav_bigendian16(GEN2_PC_RCAST->ev_atk));
        _EVs["Defense"] = int(pksav_bigendian16(GEN2_PC_RCAST->ev_def));
        _EVs["Speed"]   = int(pksav_bigendian16(GEN2_PC_RCAST->ev_spd));
        _EVs["Special"] = int(pksav_bigendian16(GEN2_PC_RCAST->ev_spcl));
    }

    void pokemon_gen2impl::_update_stat_map() {
        _stats["HP"]              = int(pksav_bigendian16(GEN2_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_bigendian16(GEN2_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_bigendian16(GEN2_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_bigendian16(GEN2_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_bigendian16(GEN2_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_bigendian16(GEN2_PARTY_RCAST->spdef));
    }

    void pokemon_gen2impl::_set_unown_form_from_IVs() {
        _database_entry.set_form(
            pkmn::calculations::gen2_unown_form(
                _IVs["Attack"],
                _IVs["Defense"],
                _IVs["Speed"],
                _IVs["Special"]
            )
        );
    }

    void pokemon_gen2impl::_set_unown_IVs_from_form(
        const std::string &form
    ) {
        // Set the maximum possible Special IV for the given form.
        uint16_t num = std::min<uint16_t>(uint16_t((form[0] - 'A') * 10) + 9, 255);

        uint8_t IV_attack = uint8_t(_IVs["Attack"]);
        IV_attack &= ~0x6;
        IV_attack |= ((num & 0xC0) >> 5);

        uint8_t IV_defense = uint8_t(_IVs["Defense"]);
        IV_defense &= ~0x6;
        IV_defense |= ((num & 0x30) >> 3);

        uint8_t IV_speed = uint8_t(_IVs["Speed"]);
        IV_speed &= ~0x6;
        IV_speed |= ((num & 0xC) >> 1);

        uint8_t IV_special = uint8_t(_IVs["Special"]);
        IV_special &= ~0x6;
        IV_special |= ((num & 0x3) << 1);

        _set_gb_IV(
            "Attack",
            IV_attack,
            &GEN2_PC_RCAST->iv_data
        );

        _set_gb_IV(
            "Defense",
            IV_defense,
            &GEN2_PC_RCAST->iv_data
        );

        _set_gb_IV(
            "Speed",
            IV_speed,
            &GEN2_PC_RCAST->iv_data
        );

        _set_gb_IV(
            "Special",
            IV_special,
            &GEN2_PC_RCAST->iv_data
        );
    }
}
