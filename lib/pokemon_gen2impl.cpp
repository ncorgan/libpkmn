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
#include "database/index_to_string.hpp"

#include "io/read_write.hpp"

#include "utils/floating_point_comparison.hpp"

#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/shininess.hpp>

#include <pkmn/database/item_entry.hpp>

#include "pksav/enum_maps.hpp"
#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include "types/lock_guard.hpp"
#include "types/rng.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen2/time.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn
{
    BOOST_STATIC_CONSTEXPR int UNOWN_ID = 201;

    /*
     * The field that stores the level at which a Pokémon is met is 6 bits, which
     * stores a max value of 63. The highest-level Pokémon that can be caught is
     * a level 52 Parasect in Mt. Silver, so this is fine. However, we must now
     * account for this...
     */
    BOOST_STATIC_CONSTEXPR int MAX_LEVEL_MET = 63;

    pokemon_gen2impl::pokemon_gen2impl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry)),
       _is_egg(false)
    {
        std::memset(&_pksav_pokemon, 0, sizeof(_pksav_pokemon));

        _nickname = boost::algorithm::to_upper_copy(
                        _database_entry.get_species_name()
                    );
        _trainer_name = DEFAULT_TRAINER_NAME;

        // Set internal members
        _pksav_pokemon.pc_data.species = uint8_t(_database_entry.get_pokemon_index());

        _pksav_pokemon.pc_data.ot_id = pksav_bigendian16(uint16_t(DEFAULT_TRAINER_ID & 0xFFFF));

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_IVs_from_form(
                _database_entry.get_form()
            );
        }
        else
        {
            pkmn::rng<uint16_t> rng;
            _pksav_pokemon.pc_data.iv_data = rng.rand();
        }

        _pksav_pokemon.pc_data.friendship = uint8_t(_database_entry.get_base_friendship());

        // Don't restrict the level at which we can instantiate a Pokémon, but
        // we can't always set the level met to this value.
        set_level_met(std::min<int>(level, MAX_LEVEL_MET));
        set_location_met("Special", false);

        time_t now = 0;
        std::time(&now);
        PKSAV_CALL(
            pksav_gen2_set_caught_data_time_field(
                &now,
                &_pksav_pokemon.pc_data.caught_data
            );
        )

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&_pksav_pokemon.pc_data.iv_data);
        set_level(level);
        _init_default_moves_for_level();
    }

    pokemon_gen2impl::pokemon_gen2impl(
        const struct pksav_gen2_pc_pokemon* p_pc_pokemon,
        int game_id
    ): pokemon_impl(p_pc_pokemon->species, game_id),
       _is_egg(false)
    {
        BOOST_ASSERT(p_pc_pokemon != nullptr);

        _pksav_pokemon.pc_data = *p_pc_pokemon;
        _populate_party_data();

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&_pksav_pokemon.pc_data.iv_data);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_IVs();
        }

        // Nickname and trainer name aren't stored with the binary,
        // so use LibPKMN's defaults.
        _nickname = boost::algorithm::to_upper_copy(
                        _database_entry.get_species_name()
                    );
        _trainer_name = DEFAULT_TRAINER_NAME;
    }

    pokemon_gen2impl::pokemon_gen2impl(
        const struct pksav_gen2_party_pokemon* p_party_pokemon,
        int game_id
    ): pokemon_impl(p_party_pokemon->pc_data.species, game_id),
       _is_egg(false)
    {
        BOOST_ASSERT(p_party_pokemon != nullptr);

        _pksav_pokemon = *p_party_pokemon;

        // Populate abstractions
        _update_EV_map();
        _init_gb_IV_map(&_pksav_pokemon.pc_data.iv_data);
        _update_stat_map();
        _update_moves(-1);

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_IVs();
        }

        // Nickname and trainer name aren't stored with the binary,
        // so use LibPKMN's defaults.
        _nickname = boost::algorithm::to_upper_copy(
                        _database_entry.get_species_name()
                    );
        _trainer_name = DEFAULT_TRAINER_NAME;
    }

    pokemon::sptr pokemon_gen2impl::clone() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return std::make_shared<pokemon_gen2impl>(*this);
    }

    pokemon::sptr pokemon_gen2impl::to_game(pkmn::e_game game)
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        pkmn::pokemon::sptr ret;

        int game_id = pkmn::database::game_enum_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);
        switch(generation)
        {
            case 1:
            {
                struct pksav_gen1_party_pokemon gen1_pksav_pokemon;
                pkmn::conversions::gen2_party_pokemon_to_gen1(
                    &_pksav_pokemon,
                    &gen1_pksav_pokemon
                );
                ret = std::make_shared<pokemon_gen1impl>(&gen1_pksav_pokemon, game_id);
                break;
            }

            case 2:
                ret = std::make_shared<pokemon_gen2impl>(&_pksav_pokemon, game_id);
                ret->set_level_met(std::min<int>(MAX_LEVEL_MET, get_level()));
                break;

            default:
                throw std::invalid_argument("Generation II Pokémon can only be converted to Generation I-II.");
        }

        ret->set_nickname(get_nickname());
        ret->set_original_trainer_name(get_original_trainer_name());

        return ret;
    }

    void pokemon_gen2impl::export_to_file(
        const std::string& filepath
    )
    {
        std::string extension = fs::extension(filepath);
        if(extension == ".pk2")
        {
            pkmn::lock_guard<pokemon_gen2impl> lock(*this);

            pkmn::io::write_file(
                filepath,
                get_native(),
                sizeof(struct pksav_gen2_pc_pokemon)
            );
        }
        else
        {
            throw std::invalid_argument("Generation II Pokémon can only be saved to .pk2 files.");
        }
    }

    void pokemon_gen2impl::set_form(
        const std::string& form
    ) {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        _database_entry.set_form(form);

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_IVs_from_form(form);
        }
    }

    bool pokemon_gen2impl::is_egg() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return _is_egg;
    }

    void pokemon_gen2impl::set_is_egg(
        bool is_egg
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        _is_egg = is_egg;
    }

    pkmn::e_condition pokemon_gen2impl::get_condition() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        pkmn::e_condition ret = pkmn::e_condition::NONE;
        enum pksav_gb_condition gb_condition = static_cast<enum pksav_gb_condition>(_pksav_pokemon.party_data.condition);

        const pksav::gb_condition_bimap_t& gb_condition_bimap = pksav::get_gb_condition_bimap();

        // Allow it not to find a valid one to account for glitch Pokémon.
        auto gb_condition_iter = gb_condition_bimap.right.find(gb_condition);
        if(gb_condition_iter != gb_condition_bimap.right.end())
        {
            ret = gb_condition_iter->second;
        }

        return ret;
    }

    void pokemon_gen2impl::set_condition(
        pkmn::e_condition condition
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        const pksav::gb_condition_bimap_t& gb_condition_bimap = pksav::get_gb_condition_bimap();
        pkmn::enforce_value_in_map_keys(
            "Condition",
            condition,
            gb_condition_bimap.left
        );

        _pksav_pokemon.party_data.condition =
            static_cast<uint8_t>(gb_condition_bimap.left.at(condition));
    }

    std::string pokemon_gen2impl::get_nickname() const
    {
        return _nickname;
    }

    void pokemon_gen2impl::set_nickname(
        const std::string& nickname
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        pkmn::enforce_string_length(
            "Nickname",
            nickname,
            1,
            PKSAV_GEN2_POKEMON_NICKNAME_LENGTH
        );

        _nickname = nickname;
    }

    pkmn::e_gender pokemon_gen2impl::get_gender() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return pkmn::calculations::gen2_pokemon_gender(
                   get_species(),
                   _IVs.at(pkmn::e_stat::ATTACK)
               );
    }

    void pokemon_gen2impl::set_gender(
        pkmn::e_gender gender
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        float chance_male = _database_entry.get_chance_male();
        float chance_female = _database_entry.get_chance_female();

        if(pkmn::fp_compare_equal(chance_male, 0.0f) and pkmn::fp_compare_equal(chance_female, 0.0f))
        {
            if(gender != pkmn::e_gender::GENDERLESS)
            {
                throw std::invalid_argument("This Pokémon is genderless.");
            }
        }
        else
        {
            if(gender == pkmn::e_gender::MALE)
            {
                if(pkmn::fp_compare_equal(chance_male, 0.0f))
                {
                    throw std::invalid_argument("This Pokémon is female-only.");
                }
                else
                {
                    set_IV(pkmn::e_stat::ATTACK, PKSAV_MAX_GB_IV);
                }
            }
            else if(gender == pkmn::e_gender::FEMALE)
            {
                if(pkmn::fp_compare_equal(chance_female, 0.0f))
                {
                    throw std::invalid_argument("This Pokémon is male-only.");
                }
                else
                {
                    // Set the IV to the max it can be while still being female.
                    if(pkmn::fp_compare_equal(chance_male, 0.875f))
                    {
                        set_IV(pkmn::e_stat::ATTACK, 1);
                    }
                    else if(pkmn::fp_compare_equal(chance_male, 0.75f))
                    {
                        set_IV(pkmn::e_stat::ATTACK, 3);
                    }
                    else if(pkmn::fp_compare_equal(chance_male, 0.5f))
                    {
                        set_IV(pkmn::e_stat::ATTACK, 6);
                    }
                    else
                    {
                        set_IV(pkmn::e_stat::ATTACK, 11);
                    }
                }
            }
            else
            {
                throw std::invalid_argument("gender: valid options \"Male\", \"Female\"");
            }
        }
    }

    bool pokemon_gen2impl::is_shiny() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return pkmn::calculations::gen2_shiny(
                   _IVs.at(pkmn::e_stat::ATTACK),
                   _IVs.at(pkmn::e_stat::DEFENSE),
                   _IVs.at(pkmn::e_stat::SPEED),
                   _IVs.at(pkmn::e_stat::SPECIAL)
               );
    }

    void pokemon_gen2impl::set_shininess(
        bool value
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        if(value)
        {
            /*
             * This value sets all IVs to the maximum values that result in
             * a shiny Pokémon.
             */
            static const uint16_t shiny_iv_data = pksav_littleendian16(0xFAAA);
            _pksav_pokemon.pc_data.iv_data = shiny_iv_data;
            _init_gb_IV_map(&_pksav_pokemon.pc_data.iv_data);
            _populate_party_data();
        }
        else
        {
            set_IV(pkmn::e_stat::ATTACK, 13);
        }

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_IVs();
        }
    }

    pkmn::e_item pokemon_gen2impl::get_held_item() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return pkmn::e_item(
                   pkmn::database::item_index_to_id(
                       _pksav_pokemon.pc_data.held_item,
                       _database_entry.get_game_id()
                   )
               );
    }

    void pokemon_gen2impl::set_held_item(
        pkmn::e_item held_item
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        // Make sure item is valid and is holdable
        pkmn::database::item_entry item(
            held_item,
            get_game()
        );

        if(!item.holdable() && (held_item != pkmn::e_item::NONE))
        {
            throw std::invalid_argument("This item is not holdable.");
        }

        _pksav_pokemon.pc_data.held_item = uint8_t(item.get_item_index());
    }

    pkmn::e_nature pokemon_gen2impl::get_nature() const
    {
        throw pkmn::feature_not_in_game_error("Natures", "Generation II");
    }

    void pokemon_gen2impl::set_nature(pkmn::e_nature)
    {
        throw pkmn::feature_not_in_game_error("Natures", "Generation II");
    }

    int pokemon_gen2impl::get_pokerus_duration() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return _get_pokerus_duration(&_pksav_pokemon.pc_data.pokerus);
    }

    void pokemon_gen2impl::set_pokerus_duration(
        int duration
    )
    {
        pkmn::enforce_bounds("Duration", duration, 0, 15);

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        _set_pokerus_duration(
            &_pksav_pokemon.pc_data.pokerus,
            duration
        );
    }

    std::string pokemon_gen2impl::get_original_trainer_name() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return _trainer_name;
    }

    void pokemon_gen2impl::set_original_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            PKSAV_GEN2_POKEMON_OTNAME_LENGTH
        );

        _trainer_name = trainer_name;
    }

    uint16_t pokemon_gen2impl::get_original_trainer_public_id() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return pksav_bigendian16(_pksav_pokemon.pc_data.ot_id);
    }

    uint16_t pokemon_gen2impl::get_original_trainer_secret_id() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation II");
    }

    uint32_t pokemon_gen2impl::get_original_trainer_id() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return uint32_t(pksav_bigendian16(_pksav_pokemon.pc_data.ot_id));
    }

    void pokemon_gen2impl::set_original_trainer_public_id(
        uint16_t public_id
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        _pksav_pokemon.pc_data.ot_id = pksav_bigendian16(public_id);
    }

    void pokemon_gen2impl::set_original_trainer_secret_id(uint16_t)
    {
        throw pkmn::feature_not_in_game_error("Secret trainer ID", "Generation II");
    }

    void pokemon_gen2impl::set_original_trainer_id(
        uint32_t id
    )
    {
        pkmn::enforce_gb_trainer_id_bounds(id);

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        _pksav_pokemon.pc_data.ot_id = pksav_bigendian16(uint16_t(id));
    }

    pkmn::e_gender pokemon_gen2impl::get_original_trainer_gender() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return (_pksav_pokemon.pc_data.caught_data & PKSAV_GEN2_POKEMON_OT_GENDER_MASK)
                    ? pkmn::e_gender::FEMALE
                    : pkmn::e_gender::MALE;
    }

    void pokemon_gen2impl::set_original_trainer_gender(
        pkmn::e_gender gender
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Original trainer gender",
            gender,
            pksav::get_gen2_gender_bimap().left
        );

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        if(gender == pkmn::e_gender::MALE)
        {
            _pksav_pokemon.pc_data.caught_data &= ~PKSAV_GEN2_POKEMON_OT_GENDER_MASK;
        }
        else
        {
            _pksav_pokemon.pc_data.caught_data |= PKSAV_GEN2_POKEMON_OT_GENDER_MASK;
        }
    }

    pkmn::e_language pokemon_gen2impl::get_language() const
    {
        throw pkmn::feature_not_in_game_error("Generation II does not track origin language.");
    }

    void pokemon_gen2impl::set_language(pkmn::e_language)
    {
        throw pkmn::feature_not_in_game_error("Generation II does not track origin language.");
    }

    int pokemon_gen2impl::get_current_trainer_friendship() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return _pksav_pokemon.pc_data.friendship;
    }

    void pokemon_gen2impl::set_current_trainer_friendship(
        int friendship
    )
    {
        pkmn::enforce_bounds("Friendship", friendship, 0, 255);

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        _pksav_pokemon.pc_data.friendship = uint8_t(friendship);
    }

    pkmn::e_ability pokemon_gen2impl::get_ability() const
    {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation II");
    }

    void pokemon_gen2impl::set_ability(pkmn::e_ability)
    {
        throw pkmn::feature_not_in_game_error("Abilities", "Generation II");
    }

    pkmn::e_ball pokemon_gen2impl::get_ball() const
    {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation II.");
    }

    void pokemon_gen2impl::set_ball(pkmn::e_ball)
    {
        throw pkmn::feature_not_in_game_error("A Pokémon's ball is not recorded in Generation II.");
    }

    int pokemon_gen2impl::get_level_met() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return int(PKSAV_GEN2_POKEMON_LEVEL_CAUGHT(_pksav_pokemon.pc_data.caught_data));
    }

    void pokemon_gen2impl::set_level_met(
        int level
    )
    {
        pkmn::enforce_bounds("Level met", level, 2, MAX_LEVEL_MET);

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        uint16_t caught_data = uint16_t(level);
        caught_data <<= PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_OFFSET;

        _pksav_pokemon.pc_data.caught_data &= ~PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_MASK;
        _pksav_pokemon.pc_data.caught_data |= caught_data;
    }

    std::string pokemon_gen2impl::get_location_met(
        bool as_egg
    ) const
    {
        if(as_egg)
        {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation II.");
        }
        else
        {
            pkmn::lock_guard<pokemon_gen2impl> lock(*this);

            return pkmn::database::location_index_to_name(
                       (_pksav_pokemon.pc_data.caught_data & PKSAV_GEN2_POKEMON_LOCATION_MASK),
                       _database_entry.get_game_id()
                   );
        }
    }

    void pokemon_gen2impl::set_location_met(
        const std::string& location,
        bool as_egg
    )
    {
        if(as_egg)
        {
            throw pkmn::feature_not_in_game_error("In-egg met location is not recorded in Generation II.");
        }
        else
        {
            pkmn::lock_guard<pokemon_gen2impl> lock(*this);

            uint16_t location_index = uint16_t(pkmn::database::location_name_to_index(
                                                   location,
                                                   _database_entry.get_game_id()
                                               ));
            _pksav_pokemon.pc_data.caught_data &= ~PKSAV_GEN2_POKEMON_LOCATION_MASK;
            _pksav_pokemon.pc_data.caught_data |= location_index;
        }
    }

    pkmn::e_game pokemon_gen2impl::get_original_game() const
    {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation II.");
    }

    void pokemon_gen2impl::set_original_game(pkmn::e_game)
    {
        throw pkmn::feature_not_in_game_error("Original game is not recorded in Generation II.");
    }

    uint32_t pokemon_gen2impl::get_personality() const
    {
        throw pkmn::feature_not_in_game_error("Personality", "Generation II");
    }

    void pokemon_gen2impl::set_personality(uint32_t)
    {
        throw pkmn::feature_not_in_game_error("Personality", "Generation II");
    }

    int pokemon_gen2impl::get_experience() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        size_t ret = 0;
        PKSAV_CALL(
            pksav_import_base256(
                _pksav_pokemon.pc_data.exp,
                PKSAV_GEN2_POKEMON_EXPERIENCE_BUFFER_SIZE,
                &ret
            );
        )

        return int(ret);
    }

    void pokemon_gen2impl::set_experience(
        int experience
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        int max_experience = _database_entry.get_experience_at_level(100);
        pkmn::enforce_bounds("Experience", experience, 0, max_experience);

        PKSAV_CALL(
            pksav_export_base256(
                experience,
                _pksav_pokemon.pc_data.exp,
                PKSAV_GEN2_POKEMON_EXPERIENCE_BUFFER_SIZE
            );
        )

        _pksav_pokemon.pc_data.level = uint8_t(_database_entry.get_level_at_experience(experience));

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gen2impl::get_level() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return int(_pksav_pokemon.pc_data.level);
    }

    void pokemon_gen2impl::set_level(
        int level
    )
    {
        pkmn::enforce_bounds("Level", level, 2, 100);

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        _pksav_pokemon.pc_data.level = uint8_t(level);

        PKSAV_CALL(
            pksav_export_base256(
                uint32_t(_database_entry.get_experience_at_level(level)),
                _pksav_pokemon.pc_data.exp,
                PKSAV_GEN2_POKEMON_EXPERIENCE_BUFFER_SIZE
            );
        )

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_gen2impl::set_IV(
        pkmn::e_stat stat,
        int value
    )
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        _set_gb_IV(
            stat,
            value,
            &_pksav_pokemon.pc_data.iv_data
        );

        if(_database_entry.get_species_id() == UNOWN_ID)
        {
            _set_unown_form_from_IVs();
        }
    }

    void pokemon_gen2impl::set_marking(pkmn::e_marking, bool)
    {
        throw pkmn::feature_not_in_game_error("Markings", "Generation II");
    }

    void pokemon_gen2impl::set_ribbon(const std::string&, bool)
    {
        throw pkmn::feature_not_in_game_error("Ribbons", "Generation II");
    }

    void pokemon_gen2impl::set_contest_stat(pkmn::e_contest_stat, int)
    {
        throw pkmn::feature_not_in_game_error("Contests", "Generation II");
    }

    void pokemon_gen2impl::set_move(
        pkmn::e_move move,
        int index
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        // This will throw an error if the move is invalid.
        pkmn::database::move_entry entry(
            move,
            get_game()
        );
        _moves[index] = {move, entry.get_pp(0)};

        _pksav_pokemon.pc_data.moves[index] = uint8_t(entry.get_move_id());
        _pksav_pokemon.pc_data.move_pps[index] = uint8_t(_moves[index].pp);
    }

    void pokemon_gen2impl::set_move_pp(
        int index,
        int pp
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

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

    void pokemon_gen2impl::set_EV(
        pkmn::e_stat stat,
        int value
    )
    {
        // Generation II uses Generation I stats for EV's.
        pkmn::enforce_value_in_vector(
            "Stat",
            stat,
            pkmn::GEN1_STATS
        );
        pkmn::enforce_EV_bounds(stat, value, false);

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

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

    int pokemon_gen2impl::get_current_hp() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return pksav_bigendian16(_pksav_pokemon.party_data.current_hp);
    }

    void pokemon_gen2impl::set_current_hp(
        int hp
    )
    {
        pkmn::enforce_bounds(
            "Current HP",
            hp,
            0,
            _stats[pkmn::e_stat::HP]
        );

        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        _pksav_pokemon.party_data.current_hp = pksav_bigendian16(static_cast<uint16_t>(hp));
    }

    const void* pokemon_gen2impl::get_native() const
    {
        pkmn::lock_guard<pokemon_gen2impl> lock(*this);

        return &_pksav_pokemon;
    }

    void pokemon_gen2impl::_populate_party_data()
    {
        pksav::gen2_pc_pokemon_to_party_data(
            _database_entry,
            &_pksav_pokemon.pc_data,
            &_pksav_pokemon.party_data
        );

        _update_stat_map();
    }

    void pokemon_gen2impl::_update_moves(
        int index
    )
    {
        _moves.resize(PKSAV_GEN2_POKEMON_NUM_MOVES);
        switch(index)
        {
            case 0:
            case 1:
            case 2:
            case 3:
                // TODO: check move validity
                _moves[index] = pkmn::move_slot(
                    static_cast<pkmn::e_move>(_pksav_pokemon.pc_data.moves[index]),
                    (_pksav_pokemon.pc_data.move_pps[index] & PKSAV_GEN2_POKEMON_MOVE_PP_MASK)
                );
                break;

            default:
                for(int i = 0; i < 4; ++i)
                {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gen2impl::_update_EV_map()
    {
        _EVs[pkmn::e_stat::HP]      = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_hp));
        _EVs[pkmn::e_stat::ATTACK]  = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_atk));
        _EVs[pkmn::e_stat::DEFENSE] = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_def));
        _EVs[pkmn::e_stat::SPEED]   = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_spd));
        _EVs[pkmn::e_stat::SPECIAL] = int(pksav_bigendian16(_pksav_pokemon.pc_data.ev_spcl));
    }

    void pokemon_gen2impl::_update_stat_map()
    {
        _stats[pkmn::e_stat::HP]              = int(pksav_bigendian16(_pksav_pokemon.party_data.max_hp));
        _stats[pkmn::e_stat::ATTACK]          = int(pksav_bigendian16(_pksav_pokemon.party_data.atk));
        _stats[pkmn::e_stat::DEFENSE]         = int(pksav_bigendian16(_pksav_pokemon.party_data.def));
        _stats[pkmn::e_stat::SPEED]           = int(pksav_bigendian16(_pksav_pokemon.party_data.spd));
        _stats[pkmn::e_stat::SPECIAL_ATTACK]  = int(pksav_bigendian16(_pksav_pokemon.party_data.spatk));
        _stats[pkmn::e_stat::SPECIAL_DEFENSE] = int(pksav_bigendian16(_pksav_pokemon.party_data.spdef));
    }

    void pokemon_gen2impl::_set_unown_form_from_IVs()
    {
        _database_entry.set_form(
            pkmn::calculations::gen2_unown_form(
                _IVs[pkmn::e_stat::ATTACK],
                _IVs[pkmn::e_stat::DEFENSE],
                _IVs[pkmn::e_stat::SPEED],
                _IVs[pkmn::e_stat::SPECIAL]
            )
        );
    }

    void pokemon_gen2impl::_set_unown_IVs_from_form(
        const std::string& form
    )
    {
        // Set the maximum possible Special IV for the given form.
        uint16_t num = std::min<uint16_t>(uint16_t((form[0] - 'A') * 10) + 9, 255);

        uint8_t IV_attack = uint8_t(_IVs[pkmn::e_stat::ATTACK]);
        IV_attack &= ~0x6;
        IV_attack |= ((num & 0xC0) >> 5);

        uint8_t IV_defense = uint8_t(_IVs[pkmn::e_stat::DEFENSE]);
        IV_defense &= ~0x6;
        IV_defense |= ((num & 0x30) >> 3);

        uint8_t IV_speed = uint8_t(_IVs[pkmn::e_stat::SPEED]);
        IV_speed &= ~0x6;
        IV_speed |= ((num & 0xC) >> 1);

        uint8_t IV_special = uint8_t(_IVs[pkmn::e_stat::SPECIAL]);
        IV_special &= ~0x6;
        IV_special |= ((num & 0x3) << 1);

        _set_gb_IV(
            pkmn::e_stat::ATTACK,
            IV_attack,
            &_pksav_pokemon.pc_data.iv_data
        );

        _set_gb_IV(
            pkmn::e_stat::DEFENSE,
            IV_defense,
            &_pksav_pokemon.pc_data.iv_data
        );

        _set_gb_IV(
            pkmn::e_stat::SPEED,
            IV_speed,
            &_pksav_pokemon.pc_data.iv_data
        );

        _set_gb_IV(
            pkmn::e_stat::SPECIAL,
            IV_special,
            &_pksav_pokemon.pc_data.iv_data
        );
    }
}
