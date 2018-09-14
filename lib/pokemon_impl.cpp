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
#include "io/gamecube_pokemon.hpp"

#include "types/lock_guard.hpp"
#include "types/rng.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <pksav/common/markings.h>
#include <pksav/common/pokerus.h>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

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
            ret = pkmn::io::load_pk1(filepath);
        }
        else if(extension == ".pk2")
        {
            ret = pkmn::io::load_pk2(filepath);
        }
        else if((extension == ".3gpkm") || (extension == ".pk3"))
        {
            ret = pkmn::io::load_3gpkm(filepath);
        }
        else if(extension == ".ck3")
        {
            ret = pkmn::io::load_ck3(filepath);
        }
        else if(extension == ".xk3")
        {
            ret = pkmn::io::load_xk3(filepath);
        }
        else if((extension == ".pkm") || (extension == ".pk6"))
        {
            throw pkmn::unimplemented_error();
        }
        else
        {
            std::vector<uint8_t> buffer(size_t(fs::file_size(filepath)));
            PKMN_UNUSED(int game_id) = 0;

            // TODO: consistency in function names
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
            else if(pkmn::io::is_vector_valid_ck3(buffer))
            {
                ret = pkmn::io::load_ck3(buffer);
            }
            else if(pkmn::io::is_vector_valid_xk3(buffer))
            {
                ret = pkmn::io::load_xk3(buffer);
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
       _generation(pkmn::database::game_id_to_generation(game_id))
    {
        _database_entry = pkmn::database::pokemon_entry(pokemon_index, game_id);
    }

    pokemon_impl::pokemon_impl(
        pkmn::database::pokemon_entry&& database_entry
    ): pokemon(),
       boost::basic_lockable_adapter<boost::recursive_mutex>()
    {
        _database_entry = std::move(database_entry);
        _generation = pkmn::database::game_id_to_generation(_database_entry.get_game_id());
    }

    // This comes out to be the default constructor, without trying to call
    // into the non-existent boost::lockable_adapter copy constructor. As such,
    // the new instance will have its own mutex. The attribute engine cannot
    // be copied, so subclasses will need to re-register the attributes.
    //
    // It's also intentional to set fields in the constructor body itself so we
    // can lock the other's mutex before copying anything.
    pokemon_impl::pokemon_impl(const pokemon_impl& other):
        pokemon(),
        boost::basic_lockable_adapter<boost::recursive_mutex>()
    {
        pkmn::lock_guard<pokemon_impl> other_lock(other);

        _moves          = other._moves;
        _EVs            = other._EVs;
        _IVs            = other._IVs;
        _stats          = other._stats;
        _contest_stats  = other._contest_stats;
        _markings       = other._markings;
        _ribbons        = other._ribbons;
        _database_entry = other._database_entry;
        _generation     = other._generation;
    }

    pokemon_impl& pokemon_impl::operator=(const pokemon_impl& rhs)
    {
        pkmn::lock_guard<pokemon_impl> rhs_lock(rhs);

        _moves          = rhs._moves;
        _EVs            = rhs._EVs;
        _IVs            = rhs._IVs;
        _stats          = rhs._stats;
        _contest_stats  = rhs._contest_stats;
        _markings       = rhs._markings;
        _ribbons        = rhs._ribbons;
        _database_entry = rhs._database_entry;
        _generation     = rhs._generation;

        return *this;
    }

    pkmn::e_species pokemon_impl::get_species() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_species();
    }

    std::string pokemon_impl::get_form() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_form();
    }

    pkmn::e_game pokemon_impl::get_game() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _database_entry.get_game();
    }

    const pkmn::database::pokemon_entry& pokemon_impl::get_database_entry() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _database_entry;
    }

    const std::map<pkmn::e_marking, bool>& pokemon_impl::get_markings() const
    {
        if(_generation < 3)
        {
            throw pkmn::feature_not_in_game_error("Markings", "Generation I-II");
        }

        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _markings;
    }

    const std::map<std::string, bool>& pokemon_impl::get_ribbons() const
    {
        if(_generation < 3)
        {
            throw pkmn::feature_not_in_game_error("Ribbons", "Generation I-II");
        }

        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _ribbons;
    }

    const std::map<pkmn::e_contest_stat, int>& pokemon_impl::get_contest_stats() const
    {
        if(_generation < 3)
        {
            throw pkmn::feature_not_in_game_error("Contests", "Generation I-II");
        }

        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _contest_stats;
    }

    const pkmn::move_slots_t& pokemon_impl::get_moves() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _moves;
    }

    const std::map<pkmn::e_stat, int>& pokemon_impl::get_EVs() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _EVs;
    }

    const std::map<pkmn::e_stat, int>& pokemon_impl::get_IVs() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _IVs;
    }

    const std::map<pkmn::e_stat, int>& pokemon_impl::get_stats() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return _stats;
    }

    std::string pokemon_impl::get_icon_filepath() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return pokemon::get_icon_filepath();
    }

    std::string pokemon_impl::get_sprite_filepath() const
    {
        pkmn::lock_guard<pokemon_impl> lock(*this);

        return pokemon::get_sprite_filepath();
    }

    // Shared abstraction initializers

    void pokemon_impl::_init_default_moves_for_level()
    {
        _moves.resize(4);

        for(int move_index = 0; move_index < 4; ++move_index)
        {
            this->set_move(pkmn::e_move::NONE, move_index);
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
                    levelup_moves[levelup_move_index].move,
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
        const struct pksav_contest_stats* p_native
    )
    {
        _contest_stats[pkmn::e_contest_stat::COOL]   = int(p_native->cool);
        _contest_stats[pkmn::e_contest_stat::BEAUTY] = int(p_native->beauty);
        _contest_stats[pkmn::e_contest_stat::CUTE]   = int(p_native->cute);
        _contest_stats[pkmn::e_contest_stat::SMART]  = int(p_native->smart);
        _contest_stats[pkmn::e_contest_stat::TOUGH]  = int(p_native->tough);

        // Feel and sheen are a union in this struct, so this is fine.
        pkmn::e_contest_stat feel_or_sheen = (_generation == 3) ? pkmn::e_contest_stat::FEEL
                                                                : pkmn::e_contest_stat::SHEEN;

        _contest_stats[feel_or_sheen] = int(p_native->feel);
    }

    void pokemon_impl::_init_markings_map(
        const uint8_t* p_native
    )
    {
        _markings[pkmn::e_marking::CIRCLE]   = bool((*p_native) & PKSAV_MARKING_CIRCLE);
        _markings[pkmn::e_marking::TRIANGLE] = bool((*p_native) & PKSAV_MARKING_TRIANGLE);
        _markings[pkmn::e_marking::SQUARE]   = bool((*p_native) & PKSAV_MARKING_SQUARE);
        _markings[pkmn::e_marking::HEART]    = bool((*p_native) & PKSAV_MARKING_HEART);
        if(_generation > 3)
        {
            _markings[pkmn::e_marking::STAR]    = bool((*p_native) & PKSAV_MARKING_STAR);
            _markings[pkmn::e_marking::DIAMOND] = bool((*p_native) & PKSAV_MARKING_DIAMOND);
        }
    }

    // Shared setters

    int pokemon_impl::_get_pokerus_duration(
        const uint8_t* pokerus_ptr
    ) const
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

    #define SET_CONTEST_STAT(map_key, native_field) \
    { \
        if(stat == (map_key)) \
        { \
            p_native->native_field = uint8_t(value); \
            _contest_stats[(map_key)] = value; \
            return; \
        } \
    }

    void pokemon_impl::_set_contest_stat(
        pkmn::e_contest_stat stat,
        int value,
        struct pksav_contest_stats* p_native
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Contest stat",
            stat,
            _contest_stats
        );
        pkmn::enforce_bounds("Contest stat", value, 0, 255);

        SET_CONTEST_STAT(pkmn::e_contest_stat::COOL,   cool);
        SET_CONTEST_STAT(pkmn::e_contest_stat::BEAUTY, beauty);
        SET_CONTEST_STAT(pkmn::e_contest_stat::CUTE,   cute);
        SET_CONTEST_STAT(pkmn::e_contest_stat::SMART,  smart);
        SET_CONTEST_STAT(pkmn::e_contest_stat::TOUGH,  tough);
        SET_CONTEST_STAT(pkmn::e_contest_stat::FEEL,   feel);
        SET_CONTEST_STAT(pkmn::e_contest_stat::SHEEN,  sheen);
    }

    #define SET_MARKING(map_key, mask) \
    { \
        if(marking == (map_key)) \
        { \
            if(value) \
            { \
                *p_native |= (mask); \
            } else \
            { \
                *p_native &= ~(mask); \
            } \
            _markings[marking] = value; \
        } \
    }

    void pokemon_impl::_set_marking(
        pkmn::e_marking marking,
        bool value,
        uint8_t* p_native
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Marking",
            marking,
            _markings
        );

        SET_MARKING(pkmn::e_marking::CIRCLE, PKSAV_MARKING_CIRCLE);
        SET_MARKING(pkmn::e_marking::TRIANGLE, PKSAV_MARKING_TRIANGLE);
        SET_MARKING(pkmn::e_marking::SQUARE, PKSAV_MARKING_SQUARE);
        SET_MARKING(pkmn::e_marking::HEART, PKSAV_MARKING_HEART);
        if(_generation > 3)
        {
            SET_MARKING(pkmn::e_marking::STAR, PKSAV_MARKING_STAR);
            SET_MARKING(pkmn::e_marking::DIAMOND, PKSAV_MARKING_DIAMOND);
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
