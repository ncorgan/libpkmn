/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database/enum_conversions.hpp"
#include "types/rng.hpp"
#include "utils/misc.hpp"

#include <pkmn/pokemon.hpp>

#include <pkmn/database/lists.hpp>
#include <pkmn/enums/marking.hpp>

#include <boost/algorithm/string.hpp>

// Note: all functions in this class are expected to be implementation-agnostic.
//       They cannot make any calls into private members.

namespace pkmn
{
    void pokemon::initialize_default_values(
        pkmn::e_species species,
        pkmn::e_game game
    )
    {
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

        // Initialize generation-independent values.
        set_original_trainer_name(DEFAULT_TRAINER_NAME);
        set_condition(pkmn::e_condition::NONE);

        // Initialize generation-dependent values.
        int generation = pkmn::database::game_enum_to_generation(game);

        std::string species_name = _database_entry.get_species_name();
        set_nickname(
            (generation >= 5) ? species_name
                              : boost::algorithm::to_upper_copy(species_name)
        );

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
                for(pkmn::e_stat stat: pkmn::GEN2_STATS)
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

                set_personality(uint32_rng.rand());

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

                break;
            }
        }
    }
}
