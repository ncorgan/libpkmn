/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "utils/misc.hpp"
#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"

#include "conversions/gb_conversions.hpp"

#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include "types/rng.hpp"

#include <pkmn/exception.hpp>

#include <pksav/common/stats.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <unordered_map>

/*
 * Catch rates
 * http://bulbapedia.bulbagarden.net/wiki/List_of_Pok%C3%A9mon_by_catch_rate
 */
static const std::vector<uint8_t> GEN1_CATCH_RATES =
{
    0,45,45,45,45,45,45,45,45,45,255,120,45,255,120,45,255,120,45,255,127,
    255,90,255,90,190,75,255,90,235,120,45,235,120,45,150,25,190,75,170,
    50,255,90,255,120,45,190,75,190,255,50,255,90,190,75,190,75,190,75,
    255,120,45,200,100,50,180,90,45,255,120,45,190,60,255,120,45,190,60,
    190,75,190,60,45,190,45,190,75,190,75,190,60,190,90,45,45,190,75,225,
    60,190,60,90,45,190,75,45,45,45,190,60,120,60,30,45,45,225,75,225,60,
    45,45,45,45,45,45,45,255,45,45,35,45,45,45,45,45,45,45,45,45,45,25,3,3,
    3,45,45,45,3,45,45,45,45,45,45,45,45,45,255,90,255,90,255,90,255,90,90,
    190,75,190,150,170,190,75,190,75,235,120,45,45,190,75,65,45,255,120,45,
    45,235,120,75,255,90,45,45,30,70,45,225,45,60,190,75,190,60,25,190,
    75,45,25,190,45,60,120,60,190,75,225,75,60,190,75,45,25,25,120,45,45,
    120,60,45,45,45,75,45,45,45,45,45,30,3,3,3,45,45,45,3,3,45
};

namespace fs = boost::filesystem;

namespace pkmn
{
    pokemon_gen1impl::pokemon_gen1impl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        std::memset(&_pksav_pokemon, 0, sizeof(_pksav_pokemon));

        _nickname = boost::algorithm::to_upper_copy(
                        _database_entry.get_name()
                    );
        _trainer_name = DEFAULT_TRAINER_NAME;

        // Set internal members
        _pksav_pokemon.pc_data.species = uint8_t(_database_entry.get_pokemon_index());

        std::pair<std::string, std::string> types = _database_entry.get_types();
        static const pksav::gen1_type_bimap_t& gen1_type_bimap = pksav::get_gen1_type_bimap();

        BOOST_ASSERT(gen1_type_bimap.left.find(types.first) != gen1_type_bimap.left.end());
        _pksav_pokemon.pc_data.types[0] = uint8_t(gen1_type_bimap.left.at(types.first));

        if(types.second == "None")
        {
            _pksav_pokemon.pc_data.types[1] = _pksav_pokemon.pc_data.types[0];
        }
        else
        {
            BOOST_ASSERT(gen1_type_bimap.left.find(types.second) != gen1_type_bimap.left.end());
            _pksav_pokemon.pc_data.types[1] = uint8_t(gen1_type_bimap.left.at(types.second));
        }

        BOOST_ASSERT(size_t(_database_entry.get_species_id()) <= GEN1_CATCH_RATES.size());
        _pksav_pokemon.pc_data.catch_rate = GEN1_CATCH_RATES[_database_entry.get_species_id()];

        _pksav_pokemon.pc_data.ot_id = pksav_bigendian16(uint16_t(DEFAULT_TRAINER_ID & 0xFFFF));

        pkmn::rng<uint16_t> rng;
        _pksav_pokemon.pc_data.iv_data = rng.rand();

        _p_native_pc = &_pksav_pokemon.pc_data;
        _p_native_party = &_pksav_pokemon.party_data;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&_pksav_pokemon.pc_data.iv_data);
        set_level(level);
        _init_default_moves_for_level();

        _register_attributes();
    }

    pokemon_gen1impl::pokemon_gen1impl(
        const struct pksav_gen1_pc_pokemon* p_pc_pokemon,
        int game_id
    ): pokemon_impl(p_pc_pokemon->species, game_id)
    {
        BOOST_ASSERT(p_pc_pokemon != nullptr);

        _pksav_pokemon.pc_data = *p_pc_pokemon;
        _populate_party_data();

        _p_native_pc = &_pksav_pokemon.pc_data;
        _p_native_party = &_pksav_pokemon.party_data;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&_pksav_pokemon.pc_data.iv_data);
        _update_stat_map();
        _update_moves(-1);

        // Nickname and trainer name aren't stored with the binary,
        // so use LibPKMN's defaults.
        _nickname = boost::algorithm::to_upper_copy(
                        _database_entry.get_name()
                    );
        _trainer_name = DEFAULT_TRAINER_NAME;

        _register_attributes();
    }

    pokemon_gen1impl::pokemon_gen1impl(
        const struct pksav_gen1_party_pokemon* p_party_pokemon,
        int game_id
    ): pokemon_impl(p_party_pokemon->pc_data.species, game_id)
    {
        BOOST_ASSERT(p_party_pokemon != nullptr);

        _pksav_pokemon = *p_party_pokemon;

        _p_native_pc = &_pksav_pokemon.pc_data;
        _p_native_party = &_pksav_pokemon.party_data;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&_pksav_pokemon.pc_data.iv_data);
        _update_stat_map();
        _update_moves(-1);

        // Nickname and trainer name aren't stored with the binary,
        // so use LibPKMN's defaults.
        _nickname = boost::algorithm::to_upper_copy(
                        _database_entry.get_name()
                    );
        _trainer_name = DEFAULT_TRAINER_NAME;

        _register_attributes();
    }

    pokemon::sptr pokemon_gen1impl::to_game(pkmn::e_game game)
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        pkmn::pokemon::sptr ret;

        int game_id = pkmn::database::game_enum_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);
        switch(generation)
        {
            case 1:
                ret = std::make_shared<pokemon_gen1impl>(&_pksav_pokemon, game_id);
                break;

            case 2:
            {
                struct pksav_gen2_party_pokemon gen2_pksav_pokemon;
                pkmn::conversions::gen1_party_pokemon_to_gen2(
                    &_pksav_pokemon,
                    &gen2_pksav_pokemon
                );
                ret = std::make_shared<pokemon_gen2impl>(&gen2_pksav_pokemon, game_id);

                // Generation II stores level met in 6 bytes, which only allows values
                // up to 63. This shouldn't prevent us from converting a Generation I
                // Pokémon.
                ret->set_level_met(
                    std::min<int>(
                        63,
                        get_level()
                    )
                );
                break;
            }

            default:
                throw std::invalid_argument("Generation I Pokémon can only be converted to Generation I-II.");
        }

        ret->set_nickname(get_nickname());
        ret->set_original_trainer_name(get_original_trainer_name());

        return ret;
    }

    void pokemon_gen1impl::export_to_file(
        const std::string& filepath
    )
    {
        std::string extension = fs::extension(filepath);
        if(extension == ".pk1")
        {
            boost::lock_guard<pokemon_gen1impl> lock(*this);

            std::ofstream ofile(filepath, std::ios::binary);
            ofile.write(static_cast<const char*>(get_native_pc_data()), sizeof(struct pksav_gen1_pc_pokemon));
            ofile.close();
        }
        else
        {
            throw std::invalid_argument("Generation I Pokémon can only be saved to .pk1 files.");
        }
    }

    void pokemon_gen1impl::set_form(
        const std::string& form
    )
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        _database_entry.set_form(form);
    }

    bool pokemon_gen1impl::is_egg()
    {
        return false;
    }

    void pokemon_gen1impl::set_is_egg(bool)
    {
        throw pkmn::feature_not_in_game_error("Eggs", "Generation I games");
    }

    std::string pokemon_gen1impl::get_condition()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        std::string ret = "None";
        enum pksav_gb_condition gb_condition = static_cast<enum pksav_gb_condition>(_pksav_pokemon.pc_data.condition);

        const pksav::gb_condition_bimap_t& gb_condition_bimap = pksav::get_gb_condition_bimap();

        // Allow it not to find a valid one to account for glitch Pokémon.
        auto gb_condition_iter = gb_condition_bimap.right.find(gb_condition);
        if(gb_condition_iter != gb_condition_bimap.right.end())
        {
            ret = gb_condition_iter->second;
        }

        return ret;
    }

    void pokemon_gen1impl::set_condition(
        const std::string& condition
    )
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        const pksav::gb_condition_bimap_t& gb_condition_bimap = pksav::get_gb_condition_bimap();
        pkmn::enforce_value_in_map_keys(
            "Condition",
            condition,
            gb_condition_bimap.left
        );

        _pksav_pokemon.pc_data.condition =
            static_cast<uint8_t>(gb_condition_bimap.left.at(condition));
    }

    std::string pokemon_gen1impl::get_nickname()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        return _nickname;
    }

    void pokemon_gen1impl::set_nickname(
        const std::string& nickname
    )
    {
        pkmn::enforce_string_length(
            "Nickname",
            nickname,
            1,
            PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
        );

        boost::lock_guard<pokemon_gen1impl> lock(*this);

        _nickname = nickname;
    }

    pkmn::e_gender pokemon_gen1impl::get_gender()
    {
        throw pkmn::feature_not_in_game_error("Pokémon gender", "Generation I");
    }

    void pokemon_gen1impl::set_gender(pkmn::e_gender)
    {
        throw pkmn::feature_not_in_game_error("Pokémon gender", "Generation I");
    }

    bool pokemon_gen1impl::is_shiny()
    {
        throw pkmn::feature_not_in_game_error("Shininess", "Generation I");
    }

    void pokemon_gen1impl::set_shininess(bool)
    {
        throw pkmn::feature_not_in_game_error("Shininess", "Generation I");
    }

    pkmn::e_item pokemon_gen1impl::get_held_item()
    {
        throw pkmn::feature_not_in_game_error("Held items", "Generation I");
    }

    void pokemon_gen1impl::set_held_item(pkmn::e_item)
    {
        throw pkmn::feature_not_in_game_error("Held items", "Generation I");
    }

    pkmn::e_nature pokemon_gen1impl::get_nature()
    {
        throw pkmn::feature_not_in_game_error("Natures", "Generation I");
    }

    void pokemon_gen1impl::set_nature(pkmn::e_nature)
    {
        throw pkmn::feature_not_in_game_error("Natures", "Generation I");
    }

    int pokemon_gen1impl::get_pokerus_duration()
    {
        throw pkmn::feature_not_in_game_error("Pokérus", "Generation I");
    }

    void pokemon_gen1impl::set_pokerus_duration(int)
    {
        throw pkmn::feature_not_in_game_error("Pokérus", "Generation I");
    }

    std::string pokemon_gen1impl::get_original_trainer_name()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        return _trainer_name;
    }

    void pokemon_gen1impl::set_original_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            PKSAV_GEN1_POKEMON_OTNAME_LENGTH
        );

        boost::lock_guard<pokemon_gen1impl> lock(*this);

        _trainer_name = trainer_name;
    }

    uint16_t pokemon_gen1impl::get_original_trainer_public_id()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        return pksav_bigendian16(_pksav_pokemon.pc_data.ot_id);
    }

    uint16_t pokemon_gen1impl::get_original_trainer_secret_id()
    {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation I");
    }

    uint32_t pokemon_gen1impl::get_original_trainer_id()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        return uint32_t(pksav_bigendian16(_pksav_pokemon.pc_data.ot_id));
    }

    void pokemon_gen1impl::set_original_trainer_public_id(
        uint16_t public_id
    )
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        _pksav_pokemon.pc_data.ot_id = pksav_bigendian16(public_id);
    }

    void pokemon_gen1impl::set_original_trainer_secret_id(uint16_t)
    {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation I");
    }

    void pokemon_gen1impl::set_original_trainer_id(
        uint32_t id
    )
    {
        pkmn::enforce_gb_trainer_id_bounds(id);

        boost::lock_guard<pokemon_gen1impl> lock(*this);

        _pksav_pokemon.pc_data.ot_id = pksav_bigendian16(uint16_t(id));
    }

    pkmn::e_gender pokemon_gen1impl::get_original_trainer_gender()
    {
        return pkmn::e_gender::MALE;
    }

    void pokemon_gen1impl::set_original_trainer_gender(pkmn::e_gender)
    {
        throw pkmn::feature_not_in_game_error("All Generation I trainers are male.");
    }

    pkmn::e_language pokemon_gen1impl::get_language()
    {
        throw pkmn::feature_not_in_game_error("Generation I does not track origin language.");
    }

    void pokemon_gen1impl::set_language(pkmn::e_language)
    {
        throw pkmn::feature_not_in_game_error("Generation I does not track origin language.");
    }

    int pokemon_gen1impl::get_current_trainer_friendship()
    {
        throw pkmn::feature_not_in_game_error("Friendship", "Generation I");
    }

    void pokemon_gen1impl::set_current_trainer_friendship(int)
    {
        throw pkmn::feature_not_in_game_error("Friendship", "Generation I");
    }

    std::string pokemon_gen1impl::get_ability()
    {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation I");
    }

    void pokemon_gen1impl::set_ability(const std::string&)
    {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation I");
    }

    pkmn::e_ball pokemon_gen1impl::get_ball()
    {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation I.");
    }

    void pokemon_gen1impl::set_ball(pkmn::e_ball)
    {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation I.");
    }

    int pokemon_gen1impl::get_level_met()
    {
        throw pkmn::feature_not_in_game_error("Level caught is not recorded in Generation I.");
    }

    void pokemon_gen1impl::set_level_met(int)
    {
        throw pkmn::feature_not_in_game_error("Level caught is not recorded in Generation I.");
    }

    std::string pokemon_gen1impl::get_location_met(bool)
    {
        throw pkmn::feature_not_in_game_error("Location caught is not recorded in Generation I.");
    }

    void pokemon_gen1impl::set_location_met(const std::string&, bool)
    {
        throw pkmn::feature_not_in_game_error("Location caught is not recorded in Generation I.");
    }

    pkmn::e_game pokemon_gen1impl::get_original_game()
    {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation I.");
    }

    void pokemon_gen1impl::set_original_game(pkmn::e_game)
    {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation I.");
    }

    uint32_t pokemon_gen1impl::get_personality()
    {
        throw pkmn::feature_not_in_game_error("Personality", "Generation I");
    }

    void pokemon_gen1impl::set_personality(uint32_t)
    {
        throw pkmn::feature_not_in_game_error("Personality", "Generation I");
    }

    int pokemon_gen1impl::get_experience()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        size_t ret = 0;
        PKSAV_CALL(
            pksav_import_base256(
                _pksav_pokemon.pc_data.exp,
                PKSAV_GEN1_POKEMON_EXPERIENCE_BUFFER_SIZE,
                &ret
            );
        )

        return int(ret);
    }

    void pokemon_gen1impl::set_experience(
        int experience
    )
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        int max_experience = _database_entry.get_experience_at_level(100);
        pkmn::enforce_bounds("Experience", experience, 0, max_experience);

        PKSAV_CALL(
            pksav_export_base256(
                experience,
                _pksav_pokemon.pc_data.exp,
                PKSAV_GEN1_POKEMON_EXPERIENCE_BUFFER_SIZE
            );
        )

        _pksav_pokemon.pc_data.level = uint8_t(_database_entry.get_level_at_experience(experience));
        _pksav_pokemon.party_data.level = _pksav_pokemon.pc_data.level;

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gen1impl::get_level()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        return int(_pksav_pokemon.party_data.level);
    }

    void pokemon_gen1impl::set_level(
        int level
    )
    {
        pkmn::enforce_bounds("Level", level, 2, 100);

        boost::lock_guard<pokemon_gen1impl> lock(*this);

        _pksav_pokemon.pc_data.level = _pksav_pokemon.party_data.level = uint8_t(level);

        PKSAV_CALL(
            pksav_export_base256(
                uint32_t(_database_entry.get_experience_at_level(level)),
                _pksav_pokemon.pc_data.exp,
                PKSAV_GEN1_POKEMON_EXPERIENCE_BUFFER_SIZE
            );
        )

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_gen1impl::set_IV(
        pkmn::e_stat stat,
        int value
    )
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        _set_gb_IV(
            stat,
            value,
            &_pksav_pokemon.pc_data.iv_data
        );
    }

    void pokemon_gen1impl::set_marking(const std::string&, bool)
    {
        throw pkmn::feature_not_in_game_error("Markings", "Generation I");
    }

    void pokemon_gen1impl::set_ribbon(const std::string&, bool)
    {
        throw pkmn::feature_not_in_game_error("Ribbons", "Generation I");
    }

    void pokemon_gen1impl::set_contest_stat(const std::string&, int)
    {
        throw pkmn::feature_not_in_game_error("Contests", "Generation I");
    }

    void pokemon_gen1impl::set_move(
        const std::string& move,
        int index
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::lock_guard<pokemon_gen1impl> lock(*this);

        // This will throw an error if the move is invalid.
        pkmn::database::move_entry entry(
            move,
            get_game()
        );
        _moves[index].move = entry.get_name();
        _moves[index].pp   = entry.get_pp(0);

        _pksav_pokemon.pc_data.moves[index] = uint8_t(entry.get_move_id());
        _pksav_pokemon.pc_data.move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gen1impl::set_move_pp(
        int index,
        int pp
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::lock_guard<pokemon_gen1impl> lock(*this);

        std::vector<int> PPs;
        pkmn::database::move_entry entry(_moves[index].move, get_game());
        for(int num_PP_ups = 0; num_PP_ups < 4; ++num_PP_ups)
        {
            PPs.emplace_back(entry.get_pp(num_PP_ups));
        }

        pkmn::enforce_bounds("PP", pp, 0, PPs.back());

        _moves[index].pp = pp;
        _pksav_pokemon.pc_data.move_pps[index] = uint8_t(pp);

        // Set the PP Up mask to the minimum value that will accommodate the given PP.
        for(uint8_t num_PP_ups = 0; num_PP_ups < PPs.size(); ++num_PP_ups)
        {
            if(pp <= PPs[num_PP_ups])
            {
                _pksav_pokemon.pc_data.move_pps[index] |= (num_PP_ups << 6);
                break;
            }
        }
    }

    void pokemon_gen1impl::set_EV(
        pkmn::e_stat stat,
        int value
    )
    {
        pkmn::enforce_value_in_vector(
            "Stat",
            stat,
            pkmn::GEN1_STATS
        );
        pkmn::enforce_EV_bounds(stat, value, false);

        boost::lock_guard<pokemon_gen1impl> lock(*this);

        switch(stat)
        {
            case pkmn::e_stat::HP:
                _pksav_pokemon.pc_data.ev_hp = pksav_bigendian16(uint16_t(value));
                break;

            case pkmn::e_stat::ATTACK:
                _pksav_pokemon.pc_data.ev_atk = pksav_bigendian16(uint16_t(value));
                break;

            case pkmn::e_stat::DEFENSE:
                _pksav_pokemon.pc_data.ev_def = pksav_bigendian16(uint16_t(value));
                break;

            case pkmn::e_stat::SPEED:
                _pksav_pokemon.pc_data.ev_spd = pksav_bigendian16(uint16_t(value));
                break;

            default:
                _pksav_pokemon.pc_data.ev_spcl = pksav_bigendian16(uint16_t(value));
                break;
        }

        _update_EV_map();
        _populate_party_data();
    }

    int pokemon_gen1impl::get_current_hp()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        return pksav_bigendian16(_pksav_pokemon.pc_data.current_hp);
    }

    void pokemon_gen1impl::set_current_hp(
        int hp
    )
    {
        pkmn::enforce_bounds(
            "Current HP",
            hp,
            0,
            _stats[pkmn::e_stat::HP]
        );

        boost::lock_guard<pokemon_gen1impl> lock(*this);

        _pksav_pokemon.pc_data.current_hp = pksav_bigendian16(static_cast<uint16_t>(hp));
    }

    std::string pokemon_gen1impl::get_icon_filepath()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        return _database_entry.get_icon_filepath(false);
    }

    std::string pokemon_gen1impl::get_sprite_filepath()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        return _database_entry.get_sprite_filepath(false, false);
    }

    int pokemon_gen1impl::get_catch_rate()
    {
        boost::lock_guard<pokemon_gen1impl> lock(*this);

        return _pksav_pokemon.pc_data.catch_rate;
    }

    void pokemon_gen1impl::_populate_party_data()
    {
        pksav::gen1_pc_pokemon_to_party_data(
            _database_entry,
            &_pksav_pokemon.pc_data,
            &_pksav_pokemon.party_data
        );

        _pksav_pokemon.pc_data.current_hp = _pksav_pokemon.party_data.max_hp;

        _update_stat_map();
    }

    void pokemon_gen1impl::_update_moves(
        int index
    )
    {
        _moves.resize(PKSAV_GEN1_POKEMON_NUM_MOVES);
        switch(index)
        {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_id_to_name(
                        _pksav_pokemon.pc_data.moves[index],
                        1
                    ),
                    (_pksav_pokemon.pc_data.move_pps[index] & PKSAV_GEN1_POKEMON_MOVE_PP_MASK)
                );
                break;

            default:
                for(int move_index = 0; move_index < 4; ++move_index)
                {
                    _update_moves(move_index);
                }
        }
    }

    void pokemon_gen1impl::_update_EV_map()
    {
        _EVs[pkmn::e_stat::HP]      = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_hp));
        _EVs[pkmn::e_stat::ATTACK]  = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_atk));
        _EVs[pkmn::e_stat::DEFENSE] = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_def));
        _EVs[pkmn::e_stat::SPEED]   = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_spd));
        _EVs[pkmn::e_stat::SPECIAL] = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_spcl));
    }

    void pokemon_gen1impl::_update_stat_map()
    {
        _stats[pkmn::e_stat::HP]      = int(pksav_bigendian16(_pksav_pokemon.party_data.max_hp));
        _stats[pkmn::e_stat::ATTACK]  = int(pksav_bigendian16(_pksav_pokemon.party_data.atk));
        _stats[pkmn::e_stat::DEFENSE] = int(pksav_bigendian16(_pksav_pokemon.party_data.def));
        _stats[pkmn::e_stat::SPEED]   = int(pksav_bigendian16(_pksav_pokemon.party_data.spd));
        _stats[pkmn::e_stat::SPECIAL] = int(pksav_bigendian16(_pksav_pokemon.party_data.spcl));
    }

    void pokemon_gen1impl::_register_attributes()
    {
        // Read-only
        _numeric_attribute_engine.register_attribute_fcns(
            "Catch rate",
            std::bind(&pokemon_gen1impl::get_catch_rate, this),
            nullptr
        );
    }
}
