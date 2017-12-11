/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_impl.hpp"
#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"
#include "pokemon_gbaimpl.hpp"
#include "pokemon_gcnimpl.hpp"
#include "pokemon_gen6impl.hpp"

#include "misc_common.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "io/pk1.hpp"
#include "io/pk2.hpp"
#include "io/3gpkm.hpp"
#include "io/pk6.hpp"

#include "types/rng.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/personality.hpp>

#include <pksav/common/markings.h>
#include <pksav/common/pokerus.h>
#include <pksav/math/endian.h>

#include <boost/filesystem.hpp>
#include <boost/thread/lock_guard.hpp>

#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn
{

    const uint32_t pkmn::pokemon::DEFAULT_TRAINER_ID = 2105214279;
    const std::string pkmn::pokemon::DEFAULT_TRAINER_NAME = "LibPKMN";

    pokemon::sptr pokemon::make(
        const std::string &species,
        const std::string &game,
        const std::string &form,
        int level
    )
    {
        int game_id = pkmn::database::game_name_to_id(game);

        pkmn::database::pokemon_entry database_entry(
                                          species,
                                          game,
                                          form
                                      );

        switch(pkmn::database::game_id_to_generation(game_id))
        {
            case 1:
                return pkmn::make_shared<pokemon_gen1impl>(
                           std::move(database_entry),
                           level
                       );

            case 2:
                return pkmn::make_shared<pokemon_gen2impl>(
                           std::move(database_entry),
                           level
                       );

            case 3:
                if(game_is_gamecube(game_id))
                {
                    return pkmn::make_shared<pokemon_gcnimpl>(
                               std::move(database_entry),
                               level
                           );
                }
                else
                {
                    return pkmn::make_shared<pokemon_gbaimpl>(
                               std::move(database_entry),
                               level
                           );
                }

            case 4:
            case 5:
                throw pkmn::unimplemented_error();

            case 6:
                return pkmn::make_shared<pokemon_gen6impl>(
                           std::move(database_entry),
                           level
                       );

            default:
                throw std::runtime_error("Invalid game.");
        }
    }

    pokemon::sptr pokemon::from_file(
        const std::string &filepath
    )
    {
        pokemon::sptr ret;

        // If an extension is given, assume a type. If not, try each.
        std::string extension = fs::extension(filepath);
        if(extension == ".pk1")
        {
            ret =  pkmn::io::load_pk1(filepath);
        }
        else if(extension == ".pk2")
        {
            ret =  pkmn::io::load_pk2(filepath);
        }
        else if(extension == ".3gpkm")
        {
            ret =  pkmn::io::load_3gpkm(filepath);
        }
        else if(extension == ".pk6")
        {
            ret =  pkmn::io::load_pk6(filepath);
        }
        else if(extension == ".pkm")
        {
            throw pkmn::unimplemented_error();
        }
        else
        {
            std::vector<uint8_t> buffer(size_t(fs::file_size(filepath)));
            PKMN_UNUSED(int game_id) = 0;

            if(pkmn::io::vector_is_valid_pk1(buffer))
            {
                ret = pkmn::io::load_pk1(buffer);
            }
            else if(pkmn::io::vector_is_valid_pk2(buffer))
            {
                ret = pkmn::io::load_pk2(buffer);
            }
            else if(pkmn::io::vector_is_valid_3gpkm(buffer, &game_id))
            {
                ret = pkmn::io::load_3gpkm(buffer);
            }
            else if(pkmn::io::vector_is_valid_pk6(buffer, &game_id))
            {
                ret = pkmn::io::load_pk6(buffer);
            }
            else
            {
                throw std::runtime_error("Invalid file.");
            }
        }

        return ret;
    }

    pokemon_impl::pokemon_impl(
        int pokemon_index,
        int game_id
    ): pokemon(),
       _database_entry(pkmn::database::pokemon_entry(pokemon_index, game_id)),
       _generation(pkmn::database::game_id_to_generation(game_id)),
       _our_pc_mem(false),
       _our_party_mem(false),
       _native_pc(nullptr),
       _native_party(nullptr)
    {}

    pokemon_impl::pokemon_impl(
        pkmn::database::pokemon_entry&& database_entry
    ): pokemon(),
       _database_entry(std::move(database_entry)),
       _generation(pkmn::database::game_id_to_generation(_database_entry.get_game_id())),
       _our_pc_mem(false),
       _our_party_mem(false),
       _native_pc(nullptr),
       _native_party(nullptr)
    {}

    std::string pokemon_impl::get_species()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_name();
    }

    std::string pokemon_impl::get_form()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_form();
    }

    std::string pokemon_impl::get_game()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_game();
    }

    const pkmn::database::pokemon_entry& pokemon_impl::get_database_entry()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry;
    }

    const std::map<std::string, bool>& pokemon_impl::get_markings()
    {
        if(_generation < 3)
        {
            throw pkmn::feature_not_in_game_error("Markings", GENERATION_NAMES[_generation]);
        }

        boost::lock_guard<pokemon_impl> lock(*this);

        return _markings;
    }

    std::string pokemon_impl::get_current_trainer_name()
    {
        throw pkmn::feature_not_in_game_error("Current trainer name", GENERATION_NAMES[_generation]);
    }

    void pokemon_impl::set_current_trainer_name(
        PKMN_UNUSED(const std::string& trainer_name)
    )
    {
        throw pkmn::feature_not_in_game_error("Current trainer name", GENERATION_NAMES[_generation]);
    }

    int pokemon_impl::get_original_trainer_friendship()
    {
        throw pkmn::feature_not_in_game_error("Original trainer friendship", GENERATION_NAMES[_generation]);
    }

    void pokemon_impl::set_original_trainer_friendship(
        PKMN_UNUSED(int friendship)
    )
    {
        throw pkmn::feature_not_in_game_error("Original trainer friendship", GENERATION_NAMES[_generation]);
    }

    const std::map<std::string, bool>& pokemon_impl::get_ribbons()
    {
        if(_generation < 3)
        {
            throw pkmn::feature_not_in_game_error("Ribbons", GENERATION_NAMES[_generation]);
        }

        boost::lock_guard<pokemon_impl> lock(*this);

        return _ribbons;
    }

    const std::map<std::string, bool>& pokemon_impl::get_super_training_medals()
    {
        if(_generation < 6)
        {
            throw pkmn::feature_not_in_game_error("Super Training Medals", GENERATION_NAMES[_generation]);
        }

        boost::lock_guard<pokemon_impl> lock(*this);

        return _super_training_medals;
    }

    void pokemon_impl::set_super_training_medal(
        const std::string& super_training_medal,
        bool value
    )
    {
        (void)super_training_medal;
        (void)value;

        throw pkmn::feature_not_in_game_error("Super Training Medals", GENERATION_NAMES[_generation]);
    }

    const std::map<std::string, int>& pokemon_impl::get_contest_stats()
    {
        if(_generation < 3)
        {
            throw pkmn::feature_not_in_game_error("Contests", "Generation I-II");
        }

        boost::lock_guard<pokemon_impl> lock(*this);

        return _contest_stats;
    }

    const pkmn::move_slots_t& pokemon_impl::get_moves()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _moves;
    }

    const std::map<std::string, int>& pokemon_impl::get_EVs()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _EVs;
    }

    const std::map<std::string, int>& pokemon_impl::get_IVs()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _IVs;
    }

    const std::map<std::string, int>& pokemon_impl::get_stats()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _stats;
    }

    std::string pokemon_impl::get_icon_filepath()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_icon_filepath(
                    (get_gender() == "Female")
               );
    }

    std::string pokemon_impl::get_sprite_filepath()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_sprite_filepath(
                    (get_gender() == "Female"),
                    is_shiny()
               );
    }

    void* pokemon_impl::get_native_pc_data()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _native_pc;
    }

    void* pokemon_impl::get_native_party_data()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _native_party;
    }

    // Shared abstraction initializers

    void pokemon_impl::_init_gb_IV_map(
        const uint16_t* iv_data_ptr
    )
    {
        uint8_t IV = 0;

        PKSAV_CALL(
            pksav_get_gb_IV(
                iv_data_ptr,
                PKSAV_STAT_HP,
                &IV
            );
        )
        _IVs["HP"] = int(IV);

        PKSAV_CALL(
            pksav_get_gb_IV(
                iv_data_ptr,
                PKSAV_STAT_ATTACK,
                &IV
            );
        )
        _IVs["Attack"] = int(IV);

        PKSAV_CALL(
            pksav_get_gb_IV(
                iv_data_ptr,
                PKSAV_STAT_DEFENSE,
                &IV
            );
        )
        _IVs["Defense"] = int(IV);

        PKSAV_CALL(
            pksav_get_gb_IV(
                iv_data_ptr,
                PKSAV_STAT_SPEED,
                &IV
            );
        );
        _IVs["Speed"] = int(IV);

        PKSAV_CALL(
            pksav_get_gb_IV(
                iv_data_ptr,
                PKSAV_STAT_SPECIAL,
                &IV
            );
        )
        _IVs["Special"] = int(IV);
    }

    void pokemon_impl::_init_modern_IV_map(
        const uint32_t* iv_data_ptr
    )
    {
        uint8_t IV = 0;

        PKSAV_CALL(
            pksav_get_IV(
                iv_data_ptr,
                PKSAV_STAT_HP,
                &IV
            );
        )
        _IVs["HP"] = int(IV);

        PKSAV_CALL(
            pksav_get_IV(
                iv_data_ptr,
                PKSAV_STAT_ATTACK,
                &IV
            );
        )
        _IVs["Attack"] = int(IV);

        PKSAV_CALL(
            pksav_get_IV(
                iv_data_ptr,
                PKSAV_STAT_DEFENSE,
                &IV
            );
        )
        _IVs["Defense"] = int(IV);

        PKSAV_CALL(
            pksav_get_IV(
                iv_data_ptr,
                PKSAV_STAT_SPEED,
                &IV
            );
        )
        _IVs["Speed"] = int(IV);

        PKSAV_CALL(
            pksav_get_IV(
                iv_data_ptr,
                PKSAV_STAT_SPATK,
                &IV
            );
        )
        _IVs["Special Attack"] = int(IV);

        PKSAV_CALL(
            pksav_get_IV(
                iv_data_ptr,
                PKSAV_STAT_SPDEF,
                &IV
            );
        )
        _IVs["Special Defense"] = int(IV);
    }

    void pokemon_impl::_init_contest_stat_map(
        const pksav_contest_stats_t* native_ptr
    )
    {
        _contest_stats["Cool"]   = int(native_ptr->cool);
        _contest_stats["Beauty"] = int(native_ptr->beauty);
        _contest_stats["Cute"]   = int(native_ptr->cute);
        _contest_stats["Smart"]  = int(native_ptr->smart);
        _contest_stats["Tough"]  = int(native_ptr->tough);

        // Feel and sheen are a union in this struct, so this is fine.
        _contest_stats[(_generation == 3) ? "Feel" : "Sheen"] = int(native_ptr->feel);
    }

    void pokemon_impl::_init_markings_map(
        const uint8_t* native_ptr
    )
    {
        _markings["Circle"]   = bool((*native_ptr) & PKSAV_MARKING_CIRCLE);
        _markings["Triangle"] = bool((*native_ptr) & PKSAV_MARKING_TRIANGLE);
        _markings["Square"]   = bool((*native_ptr) & PKSAV_MARKING_SQUARE);
        _markings["Heart"]    = bool((*native_ptr) & PKSAV_MARKING_HEART);
        if(_generation > 3)
        {
            _markings["Star"]    = bool((*native_ptr) & PKSAV_MARKING_STAR);
            _markings["Diamond"] = bool((*native_ptr) & PKSAV_MARKING_DIAMOND);
        }
    }

    // Shared setters

    void pokemon_impl::_set_nature(
        uint32_t* personality_ptr,
        const std::string &nature
    ) {
        // Save time if possible.
        if(get_nature() == nature) {
            return;
        }

        *personality_ptr = pksav_littleendian32(pkmn::calculations::generate_personality(
                               get_species(),
                               get_original_trainer_id(),
                               is_shiny(),
                               get_ability(),
                               get_gender(),
                               nature
                           ));
    }

    int pokemon_impl::_get_pokerus_duration(
        uint8_t* pokerus_ptr
    )
    {
        uint8_t duration_from_pokerus = 0;

        PKSAV_CALL(
            pksav_pokerus_get_duration(
                pokerus_ptr,
                &duration_from_pokerus
            );
        )

        return int(duration_from_pokerus);
    }

    void pokemon_impl::_set_pokerus_duration(
        uint8_t* pokerus_ptr,
        int duration
    )
    {
        pkmn::enforce_bounds("Duration", duration, 0, 15);

        PKSAV_CALL(
            pksav_pokerus_set_duration(
                pokerus_ptr,
                uint8_t(duration)
            );
        )
    }

    void pokemon_impl::_set_modern_gender(
        uint32_t* personality_ptr,
        const std::string &gender
    )
    {
        float chance_male = _database_entry.get_chance_male();
        float chance_female = _database_entry.get_chance_female();

        // Check for invalid genders.
        if(pkmn::floats_close(chance_male, 0.0f) and pkmn::floats_close(chance_female, 0.0f))
        {
            if(gender != "Genderless")
            {
                throw std::invalid_argument("This Pokémon is genderless.");
            }
            else
            {
                // Nothing to do.
                return;
            }
        }
        else if(pkmn::floats_close(chance_male, 1.0f) and gender != "Male")
        {
            throw std::invalid_argument("This Pokémon is male-only.");
        }
        else if(pkmn::floats_close(chance_female, 1.0f) and gender != "Female")
        {
            throw std::invalid_argument("This Pokémon is female-only.");
        }
        else if(gender == "Genderless")
        {
            throw std::invalid_argument("gender: valid options \"Male\", \"Female\"");
        }

        if(gender == "Male")
        {
            *personality_ptr |= 0xFF;
        }
        else
        {
            *personality_ptr &= ~0xFF;

            pkmn::rng<uint32_t> rng;
            if(pkmn::floats_close(chance_male, 0.875f))
            {
                *personality_ptr |= rng.rand(0, 30);
            }
            else if(pkmn::floats_close(chance_male, 0.75f))
            {
                *personality_ptr |= rng.rand(0, 63);
            }
            else if(pkmn::floats_close(chance_male, 0.5f))
            {
                *personality_ptr |= rng.rand(0, 126);
            }
            else
            {
                *personality_ptr |= rng.rand(0, 190);
            }
        }
    }

    void pokemon_impl::_set_modern_shininess(
        uint32_t* personality_ptr,
        bool value
    )
    {
        uint32_t trainer_id = get_original_trainer_id();

        uint16_t* p = reinterpret_cast<uint16_t*>(personality_ptr);
        const uint16_t* t = reinterpret_cast<const uint16_t*>(&trainer_id);

        if(value)
        {
            for(size_t i = 3; i < 16; ++i)
            {
                size_t num_ones = 0;
                if(p[0] & (1 << i)) ++num_ones;
                if(p[1] & (1 << i)) ++num_ones;
                if(t[0] & (1 << i)) ++num_ones;
                if(t[1] & (1 << i)) ++num_ones;

                if(num_ones % 2)
                {
                    p[0] ^= (1 << i);
                }
            }
        }
        else
        {
            // Only one column has to satisfy the condition, so don't bother iterating
            uint16_t sum = (p[0] & 1) + (p[1] & 1) + (t[0] & 1) + (t[1] & 1);
            if((sum % 2) == 0) {
                (*personality_ptr) ^= 1;
            }
        }
    }

    void pokemon_impl::_set_gb_IV(
        const std::string &stat,
        int value,
        uint16_t* iv_data_ptr
    )
    {
        pkmn::enforce_value_in_vector("Stat", stat, pkmn::GEN1_STATS);
        pkmn::enforce_IV_bounds(stat, value, false);

        PKSAV_CALL(
            pksav_set_gb_IV(
                iv_data_ptr,
                pkmn_stats_to_pksav.at(stat),
                uint8_t(value)
            );
        )

        // Setting any IV affects HP, and vice versa, so we may as well update it all.
        _init_gb_IV_map(iv_data_ptr);

        _populate_party_data();
    }

    void pokemon_impl::_set_modern_IV(
        const std::string &stat,
        int value,
        uint32_t* iv_data_ptr
    )
    {
        pkmn::enforce_value_in_vector("Stat", stat, pkmn::MODERN_STATS);
        pkmn::enforce_IV_bounds(stat, value, true);

        PKSAV_CALL(
            pksav_set_IV(
                iv_data_ptr,
                pkmn_stats_to_pksav.at(stat),
                uint8_t(value)
            );
        )

        _IVs[stat] = value;
        _populate_party_data();
    }

    #define SET_CONTEST_STAT(str,field) \
    { \
        if(stat == (str)) { \
            native_ptr->field = uint8_t(value); \
            _contest_stats[(str)] = value; \
            return; \
        } \
    }

    void pokemon_impl::_set_contest_stat(
        const std::string &stat,
        int value,
        pksav_contest_stats_t* native_ptr
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Contest stat",
            stat,
            _contest_stats
        );
        pkmn::enforce_bounds("Contest stat", value, 0, 255);

        SET_CONTEST_STAT("Cool",   cool);
        SET_CONTEST_STAT("Beauty", beauty);
        SET_CONTEST_STAT("Cute",   cute);
        SET_CONTEST_STAT("Smart",  smart);
        SET_CONTEST_STAT("Tough",  tough);
        SET_CONTEST_STAT("Feel",   feel);
        SET_CONTEST_STAT("Sheen",  sheen);
    }

    #define SET_MARKING(str,mask) \
    { \
        if(marking == (str)) { \
            if(value) { \
                *native_ptr |= (mask); \
            } else { \
                *native_ptr &= ~(mask); \
            } \
            _markings[marking] = value; \
        } \
    }

    void pokemon_impl::_set_marking(
        const std::string &marking,
        bool value,
        uint8_t* native_ptr
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Marking",
            marking,
            _markings
        );

        SET_MARKING("Circle", PKSAV_MARKING_CIRCLE);
        SET_MARKING("Triangle", PKSAV_MARKING_TRIANGLE);
        SET_MARKING("Square", PKSAV_MARKING_SQUARE);
        SET_MARKING("Heart", PKSAV_MARKING_HEART);
        if(_generation > 3)
        {
            SET_MARKING("Star", PKSAV_MARKING_STAR);
            SET_MARKING("Diamond", PKSAV_MARKING_DIAMOND);
        }
    }

    std::string pokemon_impl::get_gen4_encounter_type(
        uint8_t* native_ptr
    )
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return pksav::GEN4_ENCOUNTER_TYPE_BIMAP.right.at(pksav_gen4_encounter_type_t(*native_ptr));
    }

    void pokemon_impl::set_gen4_encounter_type(
        uint8_t* native_ptr,
        const std::string& gen4_encounter_type
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Generation IV encounter type",
            gen4_encounter_type,
            pksav::GEN4_ENCOUNTER_TYPE_BIMAP.left
        );

        boost::lock_guard<pokemon_impl> lock(*this);

        *native_ptr = uint8_t(pksav::GEN4_ENCOUNTER_TYPE_BIMAP.left.at(gen4_encounter_type));
    }
}
