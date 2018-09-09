/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database/enum_conversions.hpp"
#include "pksav/enum_maps.hpp"
#include "types/rng.hpp"
#include "utils/misc.hpp"

#include <pkmn/pokemon.hpp>

#include <pkmn/calculations/stats.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/enums/enum_to_string.hpp>
#include <pkmn/enums/marking.hpp>

#include <pksav/common/stats.h>

#include <boost/algorithm/string.hpp>

#include <vector>

// Note: all functions in this class are expected to be implementation-agnostic.
//       They cannot make any calls into private members.

namespace pkmn
{
    // Don't instantiate until we need it.
    static const std::vector<std::string>& get_default_locations_met_for_version_groups()
    {
        static const std::vector<std::string> DEFAULT_LOCATIONS_MET_FOR_VERSION_GROUPS =
        {
            "",         // None
            "",         // Red/Blue
            "",         // Yellow
            "Special",  // Gold/Silver
            "Special",  // Crystal
            "Fateful encounter", // Ruby/Sapphire
            "Fateful encounter", // Emerald
            "Fateful encounter", // FireRed/LeafGreen

            // TODO
            "", // Diamond/Pearl
            "", // Platinum
            "", // HeartGold/SoulSilver
            "", // Black/White

            "Distant land", // Colosseum
            "Distant land", // XD

            // TODO
            "", // Black 2/White 2
            "", // X/Y
            "", // Omega Ruby/Alpha Sapphire
        };

        return DEFAULT_LOCATIONS_MET_FOR_VERSION_GROUPS;
    }

    void pokemon::initialize_default_values(
        pkmn::e_species species,
        pkmn::e_game game
    )
    {
        pkmn::rng<int> int_rng;
        pkmn::rng<uint32_t> uint32_rng;

        // Clear any existing info.
        _EVs.clear();
        _IVs.clear();
        _stats.clear();

        // If the database entry is not instantiated, do it ourselves.

        if((_database_entry.get_species() != species) || (_database_entry.get_game() != game))
        {
            _database_entry = pkmn::database::pokemon_entry(
                                  species,
                                  game,
                                  ""
                              );
        }

        // TODO: for evolved forms, make default level the level they evolve,
        //       involves database fun
        const int default_level = 5;
        set_level(5);

        // Initialize generation-independent values.
        set_original_trainer_public_id(uint16_t(DEFAULT_TRAINER_ID & 0xFFFF));
        set_original_trainer_name(DEFAULT_TRAINER_NAME);
        set_condition(pkmn::e_condition::NONE);

        // Initialize generation-dependent values.
        int generation = pkmn::database::game_enum_to_generation(game);
        int version_group = pkmn::database::game_enum_to_version_group(game);

        std::string species_name = _database_entry.get_species_name();
        set_nickname(
            (generation >= 5) ? species_name
                              : boost::algorithm::to_upper_copy(species_name)
        );

        // Initialize maps.
        switch(generation)
        {
            case 1:
                for(pkmn::e_stat stat: pkmn::GEN1_STATS)
                {
                    _EVs.emplace(stat, 0);
                    _IVs.emplace(stat, 0);
                    _stats.emplace(stat, 0);
                }
                break;

            case 2:
                // Note: EVs and IVs still use Gen I stats.
                for(pkmn::e_stat stat: pkmn::GEN1_STATS)
                {
                    _EVs.emplace(stat, 0);
                    _IVs.emplace(stat, 0);
                }
                for(pkmn::e_stat stat: pkmn::MODERN_STATS)
                {
                    _stats.emplace(stat, 0);
                }
                break;

            default:
            {
                for(pkmn::e_stat stat: pkmn::MODERN_STATS)
                {
                    _EVs.emplace(stat, 0);
                    _IVs.emplace(stat, 0);
                    _stats.emplace(stat, 0);
                }

                // Initialize marking map.
                // TODO: use list function, once implemented
                _markings.emplace(pkmn::e_marking::CIRCLE, false);
                _markings.emplace(pkmn::e_marking::TRIANGLE, false);
                _markings.emplace(pkmn::e_marking::SQUARE, false);
                _markings.emplace(pkmn::e_marking::HEART, false);
                if(generation >= 4)
                {
                    _markings.emplace(pkmn::e_marking::STAR, false);
                    _markings.emplace(pkmn::e_marking::DIAMOND, false);
                }

                // Initialize contest stat map.
                // TODO: use list function, once implemented
                _contest_stats.emplace(pkmn::e_contest_stat::COOL, 0);
                _contest_stats.emplace(pkmn::e_contest_stat::BEAUTY, 0);
                _contest_stats.emplace(pkmn::e_contest_stat::CUTE, 0);
                _contest_stats.emplace(pkmn::e_contest_stat::SMART, 0);
                _contest_stats.emplace(pkmn::e_contest_stat::TOUGH, 0);
                _contest_stats.emplace(
                    (generation == 3) ? pkmn::e_contest_stat::FEEL
                                      : pkmn::e_contest_stat::SHEEN,
                    0
                );

                // Initialize ribbon map.
                for(const std::string& ribbon_name: pkmn::database::get_ribbon_name_list(generation))
                {
                    _ribbons.emplace(ribbon_name, false);
                }
                break;
            }
        }

        if(generation >= 2)
        {
            set_held_item(pkmn::e_item::NONE);
            set_original_trainer_gender(pkmn::e_gender::MALE);
            set_current_trainer_friendship(_database_entry.get_base_friendship());
            set_pokerus_duration(0);
            set_ball(pkmn::e_ball::POKE_BALL);
            set_level_met(default_level);
            set_original_game(game);

            const std::vector<std::string>& default_locations_met_for_version_groups =
                get_default_locations_met_for_version_groups();

            set_location_met(
                default_locations_met_for_version_groups.at(version_group),
                false // as_egg
            );
        }
        if(generation >= 3)
        {
            set_original_trainer_secret_id(
                uint16_t((DEFAULT_TRAINER_ID & 0xFFFF0000) >> 16)
            );

            set_personality(uint32_rng.rand());

            const pksav::nature_bimap_t& nature_bimap = pksav::get_nature_bimap();

            // TODO: move this to enum_conversions
            set_nature(
                nature_bimap.right.at(
                    static_cast<enum pksav_nature>(get_personality() % 25)
                )
            );

            const pkmn::ability_pair_t abilities = _database_entry.get_abilities();
            if(abilities.second == pkmn::e_ability::NONE)
            {
                set_ability(abilities.first);
            }
            else
            {
                set_ability(
                    ((get_personality() % 2) == 0) ? abilities.first
                                                   : abilities.second
                );
            }

            set_language(pkmn::e_language::ENGLISH);
        }
        if(generation >= 4)
        {
            const std::vector<std::string>& default_locations_met_for_version_groups =
                get_default_locations_met_for_version_groups();

            set_location_met(
                default_locations_met_for_version_groups.at(version_group),
                true // as_egg
            );
        }

        // Populate IVs
        for(const auto& IV_map_pair: _IVs)
        {
            const int max_IV = (generation >= 3) ? PKSAV_MAX_IV
                                                 : PKSAV_MAX_GB_IV;

            // Call set_IV instead of setting in map to enforce any policy
            // implemented in the subclass.
            set_IV(
                IV_map_pair.first,
                int_rng.rand(0, max_IV)
            );
        }

        // Populate stats
        const std::map<pkmn::e_stat, int> base_stats = _database_entry.get_base_stats();
        for(const auto& stat_pair: _stats)
        {
            const pkmn::e_stat stat = stat_pair.first;

            // In Generation II, Special Attack+Defense used the Special EV
            // and IV.
            pkmn::e_stat stat_for_EV_IV = pkmn::e_stat::NONE;
            if((generation == 2) &&
               ((stat == pkmn::e_stat::SPECIAL_ATTACK) || (stat == pkmn::e_stat::SPECIAL_DEFENSE)))
            {
                stat_for_EV_IV = pkmn::e_stat::SPECIAL;
            }
            else
            {
                stat_for_EV_IV = stat;
            }

            if(generation >= 3)
            {
                _stats[stat] = pkmn::calculations::get_modern_stat(
                                   stat,
                                   default_level,
                                   1.0, // nature modifier: TODO
                                   base_stats.at(stat),
                                   _EVs.at(stat_for_EV_IV),
                                   _IVs.at(stat_for_EV_IV)
                               );
            }
            else
            {
                _stats[stat] = pkmn::calculations::get_gb_stat(
                                   stat,
                                   default_level,
                                   base_stats.at(stat),
                                   _EVs.at(stat_for_EV_IV),
                                   _IVs.at(stat_for_EV_IV)
                               );
            }
        }
    }
}
