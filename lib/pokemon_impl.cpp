/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

#include "utils/floating_point_comparison.hpp"
#include "utils/misc.hpp"

#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "io/pk1.hpp"
#include "io/pk2.hpp"
#include "io/3gpkm.hpp"

#include "types/rng.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <pksav/common/markings.h>
#include <pksav/common/pokerus.h>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/lock_guard.hpp>

#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn
{
    const uint32_t pkmn::pokemon::DEFAULT_TRAINER_ID = 2105214279;
    const std::string pkmn::pokemon::DEFAULT_TRAINER_NAME = "LibPKMN";

    pokemon::sptr pokemon::make(
        pkmn::e_species species,
        pkmn::e_game game,
        const std::string& form,
        int level
    )
    {
        int game_id = pkmn::database::game_enum_to_id(game);

        pkmn::database::pokemon_entry database_entry(
                                          species,
                                          game,
                                          form
                                      );

        switch(pkmn::database::game_id_to_generation(game_id))
        {
            case 1:
                return std::make_shared<pokemon_gen1impl>(
                           std::move(database_entry),
                           level
                       );

            case 2:
                return std::make_shared<pokemon_gen2impl>(
                           std::move(database_entry),
                           level
                       );

            case 3:
                if(game_is_gamecube(game_id))
                {
                    return std::make_shared<pokemon_gcnimpl>(
                               std::move(database_entry),
                               level
                           );
                }
                else
                {
                    return std::make_shared<pokemon_gbaimpl>(
                               std::move(database_entry),
                               level
                           );
                }

            case 4:
            case 5:
            case 6:
                throw pkmn::unimplemented_error();

            default:
                throw std::invalid_argument("Invalid game.");
        }
    }

    pokemon::sptr pokemon::from_file(
        const std::string& filepath
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
        else if(extension == ".pkm" or extension == ".pk6")
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
       _p_native_pc(nullptr),
       _p_native_party(nullptr)
    {}

    pokemon_impl::pokemon_impl(
        pkmn::database::pokemon_entry&& database_entry
    ): pokemon(),
       _database_entry(std::move(database_entry)),
       _generation(pkmn::database::game_id_to_generation(_database_entry.get_game_id())),
       _p_native_pc(nullptr),
       _p_native_party(nullptr)
    {}

    pkmn::e_species pokemon_impl::get_species()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_species();
    }

    std::string pokemon_impl::get_form()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_form();
    }

    pkmn::e_game pokemon_impl::get_game()
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
            throw pkmn::feature_not_in_game_error("Markings", "Generation I-II");
        }

        boost::lock_guard<pokemon_impl> lock(*this);

        return _markings;
    }

    const std::map<std::string, bool>& pokemon_impl::get_ribbons()
    {
        if(_generation < 3)
        {
            throw pkmn::feature_not_in_game_error("Ribbons", "Generation I-II");
        }

        boost::lock_guard<pokemon_impl> lock(*this);

        return _ribbons;
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

    const std::map<pkmn::e_stat, int>& pokemon_impl::get_EVs()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _EVs;
    }

    const std::map<pkmn::e_stat, int>& pokemon_impl::get_IVs()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _IVs;
    }

    const std::map<pkmn::e_stat, int>& pokemon_impl::get_stats()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _stats;
    }

    std::string pokemon_impl::get_icon_filepath()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_icon_filepath(
                    (get_gender() == pkmn::e_gender::FEMALE)
               );
    }

    std::string pokemon_impl::get_sprite_filepath()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_sprite_filepath(
                    (get_gender() == pkmn::e_gender::FEMALE),
                    is_shiny()
               );
    }

    void* pokemon_impl::get_native_pc_data()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _p_native_pc;
    }

    void* pokemon_impl::get_native_party_data()
    {
        boost::lock_guard<pokemon_impl> lock(*this);

        return _p_native_party;
    }

    // Shared abstraction initializers

    void pokemon_impl::_init_default_moves_for_level()
    {
        _moves.resize(4);

        for(int move_index = 0; move_index < 4; ++move_index)
        {
            this->set_move("None", move_index);
        }

        pkmn::database::levelup_moves_t levelup_moves = _database_entry.get_levelup_moves();
        if(!levelup_moves.empty())
        {
            int level = this->get_level();
            BOOST_ASSERT(level >= levelup_moves[0].level);

            auto levelup_move_iter =
                std::find_if(
                    levelup_moves.begin(),
                    levelup_moves.end(),
                    [&level](const pkmn::database::levelup_move& levelup_move)
                    {
                        return (levelup_move.level > level);
                    });

            size_t first_move_index = 0;
            size_t last_move_index = levelup_moves.size() - 1;

            if(levelup_move_iter == levelup_moves.end())
            {
                // The Pokémon is at a higher level than any levelup move.
                if(levelup_moves.size() > 4)
                {
                    first_move_index = levelup_moves.size() - 4;
                }
            }
            else
            {
                last_move_index = std::distance(
                                      levelup_moves.begin(),
                                      levelup_move_iter
                                  ) - 1;
                if(last_move_index > 3)
                {
                    first_move_index = last_move_index - 4;
                }
            }

            for(size_t move_index = 0, levelup_move_index = first_move_index;
                (move_index < 4) && (levelup_move_index <= last_move_index);
                ++move_index, ++levelup_move_index)
            {
                this->set_move(
                    levelup_moves[levelup_move_index].move.get_name(),
                    static_cast<int>(move_index)
                );
            }
        }
    }

    void pokemon_impl::_init_gb_IV_map(
        const uint16_t* iv_data_ptr
    )
    {
        uint8_t pksav_IVs[PKSAV_NUM_GB_IVS] = {0};

        PKSAV_CALL(
            pksav_get_gb_IVs(
                iv_data_ptr,
                pksav_IVs,
                sizeof(pksav_IVs)
            );
        )

        const pksav::gb_IV_bimap_t& gb_IV_bimap = pksav::get_gb_IV_bimap();
        for(const auto& IV_pair: gb_IV_bimap.left)
        {
            pkmn::e_stat libpkmn_stat = IV_pair.first;
            enum pksav_gb_IV pksav_IV = IV_pair.second;

            _IVs[libpkmn_stat] = pksav_IVs[pksav_IV];
        }
    }

    void pokemon_impl::_init_modern_IV_map(
        const uint32_t* iv_data_ptr
    )
    {
        uint8_t pksav_IVs[PKSAV_NUM_IVS] = {0};

        PKSAV_CALL(
            pksav_get_IVs(
                iv_data_ptr,
                pksav_IVs,
                sizeof(pksav_IVs)
            );
        )

        const pksav::IV_bimap_t& IV_bimap = pksav::get_IV_bimap();
        for(const auto& IV_pair: IV_bimap.left)
        {
            pkmn::e_stat libpkmn_stat = IV_pair.first;
            enum pksav_IV pksav_IV = IV_pair.second;

            _IVs[libpkmn_stat] = pksav_IVs[pksav_IV];
        }
    }

    void pokemon_impl::_init_contest_stat_map(
        const struct pksav_contest_stats* native_ptr
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
        pkmn::e_gender gender
    )
    {
        float chance_male = _database_entry.get_chance_male();
        float chance_female = _database_entry.get_chance_female();

        // Check for invalid genders.
        if(pkmn::fp_compare_equal(chance_male, 0.0f) and pkmn::fp_compare_equal(chance_female, 0.0f))
        {
            if(gender != pkmn::e_gender::GENDERLESS)
            {
                throw std::invalid_argument("This Pokémon is genderless.");
            }
            else
            {
                // Nothing to do.
                return;
            }
        }
        else if(pkmn::fp_compare_equal(chance_male, 1.0f) and gender != pkmn::e_gender::MALE)
        {
            throw std::invalid_argument("This Pokémon is male-only.");
        }
        else if(pkmn::fp_compare_equal(chance_female, 1.0f) and gender != pkmn::e_gender::FEMALE)
        {
            throw std::invalid_argument("This Pokémon is female-only.");
        }
        else if(gender == pkmn::e_gender::GENDERLESS)
        {
            throw std::invalid_argument("gender: valid options \"Male\", \"Female\"");
        }

        if(gender == pkmn::e_gender::MALE)
        {
            *personality_ptr |= 0xFF;
        }
        else
        {
            *personality_ptr &= ~0xFF;

            pkmn::rng<uint32_t> rng;
            if(pkmn::fp_compare_equal(chance_male, 0.875f))
            {
                *personality_ptr |= rng.rand(0, 30);
            }
            else if(pkmn::fp_compare_equal(chance_male, 0.75f))
            {
                *personality_ptr |= rng.rand(0, 63);
            }
            else if(pkmn::fp_compare_equal(chance_male, 0.5f))
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
        const uint32_t* trainer_id_ptr,
        bool value
    )
    {
        uint16_t* p = reinterpret_cast<uint16_t*>(personality_ptr);
        const uint16_t* t = reinterpret_cast<const uint16_t*>(trainer_id_ptr);

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
        pkmn::e_stat stat,
        int value,
        uint16_t* iv_data_ptr
    )
    {
        pkmn::enforce_value_in_vector("Stat", stat, pkmn::GEN1_STATS);
        pkmn::enforce_IV_bounds(stat, value, false);

        PKSAV_CALL(
            pksav_set_gb_IV(
                pksav::get_gb_IV_bimap().left.at(stat),
                uint8_t(value),
                iv_data_ptr
            );
        )

        // Setting any IV affects HP, and vice versa, so we may as well update it all.
        _init_gb_IV_map(iv_data_ptr);

        _populate_party_data();
    }

    void pokemon_impl::_set_modern_IV(
        pkmn::e_stat stat,
        int value,
        uint32_t* iv_data_ptr
    )
    {
        pkmn::enforce_value_in_vector("Stat", stat, pkmn::MODERN_STATS);
        pkmn::enforce_IV_bounds(stat, value, true);

        PKSAV_CALL(
            pksav_set_IV(
                pksav::get_IV_bimap().left.at(stat),
                uint8_t(value),
                iv_data_ptr
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
        const std::string& stat,
        int value,
        struct pksav_contest_stats* native_ptr
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
        const std::string& marking,
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

    void pokemon_impl::_set_ability_from_personality()
    {
        const pkmn::ability_pair_t abilities = _database_entry.get_abilities();
        if((abilities.second != pkmn::e_ability::NONE) && ((get_personality() % 2) == 1))
        {
            _set_ability(abilities.second);
        }
        else
        {
            _set_ability(abilities.first);
        }
    }
}
