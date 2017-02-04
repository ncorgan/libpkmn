/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_party_tests.hpp> 

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>
#include <boost/format.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/gen1.h>
#include <pksav/gen2.h>
#include <pksav/gba.h>

#include <pkmn/exception.hpp>

// No database access here
static const std::map<std::string, int> game_generations = boost::assign::map_list_of
    ("Red", 1)
    ("Blue", 1)
    ("Yellow", 1)
    ("Gold", 2)
    ("Silver", 2)
    ("Crystal", 2)
    ("Ruby", 3)
    ("Sapphire", 3)
    ("Emerald", 3)
    ("FireRed", 3)
    ("LeafGreen", 3)
    ("Colosseum", 3)
    ("XD", 3)
    ("Diamond", 4)
    ("Pearl", 4)
    ("Platinum", 4)
    ("HeartGold", 4)
    ("SoulSilver", 4)
    ("Black", 5)
    ("White", 5)
    ("Black 2", 5)
    ("White 2", 5)
    ("X", 6)
    ("Y", 6)
    ("Omega Ruby", 6)
    ("Alpha Sapphire", 6)
;

namespace pkmntest {

    void test_empty_pokemon_party(
        pkmn::pokemon_party::sptr party,
        const std::string &game
    ) {
        // Check that the game is set properly.
        BOOST_CHECK_EQUAL(party->get_game(), game);

        // Check index bounds.
        BOOST_CHECK_THROW(
            (void)party->get_pokemon(-1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            (void)party->get_pokemon(6);
        , pkmn::range_error);

        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 0);

        const pkmn::pokemon_list_t& list = party->as_vector();
        BOOST_CHECK_EQUAL(list.size(), 6);

        for(auto party_iter = list.begin(); party_iter != list.end(); ++party_iter) {
            BOOST_CHECK_EQUAL((*party_iter)->get_species(), "None");
            BOOST_CHECK_EQUAL((*party_iter)->get_game(), game);

            const pkmn::move_slots_t& move_slots = (*party_iter)->get_moves();
            BOOST_CHECK_EQUAL(move_slots.size(), 4);
            for(auto move_iter = move_slots.begin(); move_iter != move_slots.end(); ++move_iter) {
                BOOST_CHECK_EQUAL(move_iter->move.get_name(), "None");
                BOOST_CHECK_EQUAL(move_iter->move.get_game(), game);
                BOOST_CHECK_EQUAL(move_iter->pp, 0);
            }
        }
    }

    void test_setting_pokemon(
        pkmn::pokemon_party::sptr party
    ) {
        std::string game = party->get_game();
        int generation = game_generations.at(game);

        pkmn::pokemon::sptr original_first = party->get_pokemon(0);
        pkmn::pokemon::sptr original_second = party->get_pokemon(1);

        /*
         * Create new Pokémon and place in party. The original variables should
         * have the same underlying Pokémon.
         */
        pkmn::pokemon::sptr bulbasaur = pkmn::pokemon::make(
                                            "Bulbasaur",
                                            game,
                                            "",
                                            5
                                        );
        pkmn::pokemon::sptr charmander = pkmn::pokemon::make(
                                             "Charmander",
                                             game,
                                             "",
                                             5
                                         );
        pkmn::pokemon::sptr squirtle = pkmn::pokemon::make(
                                           "Squirtle",
                                           game,
                                           "",
                                           5
                                       );

        // Make sure we can't set them at invalid indices.
        BOOST_CHECK_THROW(
            party->set_pokemon(-1, bulbasaur);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            party->set_pokemon(6, bulbasaur);
        , pkmn::range_error);

        party->set_pokemon(0, bulbasaur);
        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 1);
        BOOST_CHECK_EQUAL(party->get_pokemon(0)->get_species(), "Bulbasaur");
        party->set_pokemon(1, charmander);
        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 2);
        BOOST_CHECK_EQUAL(party->get_pokemon(1)->get_species(), "Charmander");

        // Replace one of the new ones.
        party->set_pokemon(0, squirtle);
        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 2);
        BOOST_CHECK_EQUAL(party->get_pokemon(0)->get_species(), "Squirtle");

        // Make sure we can't copy a Pokémon to itself.
        BOOST_CHECK_THROW(
            party->set_pokemon(1, party->get_pokemon(1));
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 2);

        // Copy a Pokémon whose memory is already part of the party.
        party->set_pokemon(2, party->get_pokemon(1));
        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 3);

        // We should always be able to clear the last contiguous Pokémon.
        party->set_pokemon(2, original_first);
        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 2);
        BOOST_CHECK_EQUAL(party->get_pokemon(2)->get_species(), "None");

        // Put it back.
        party->set_pokemon(2, party->get_pokemon(1));
        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 3);

        // Check that Pokémon cannot be placed non-contiguously.
        BOOST_CHECK_THROW(
            party->set_pokemon(1, original_first);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 3);
        BOOST_CHECK_EQUAL(party->get_pokemon(1)->get_species(), "Charmander");

        BOOST_CHECK_THROW(
            party->set_pokemon(4, bulbasaur);
        , pkmn::range_error);
        BOOST_CHECK_EQUAL(party->get_num_pokemon(), 3);
        BOOST_CHECK_EQUAL(party->get_pokemon(4)->get_species(), "None");

        /*
         * Now check everything we've created. Each variable should have the
         * same Pokémon underneath, even if the pointer has changed.
         */
        BOOST_CHECK_EQUAL(party->get_pokemon(0)->get_species(), "Squirtle");
        BOOST_CHECK_EQUAL(party->get_pokemon(1)->get_species(), "Charmander");
        BOOST_CHECK_EQUAL(party->get_pokemon(2)->get_species(), "Charmander");
        BOOST_CHECK_EQUAL(original_first->get_species(), "None");
        BOOST_CHECK_EQUAL(original_second->get_species(), "None");
        BOOST_CHECK_EQUAL(bulbasaur->get_species(), "Bulbasaur");
        BOOST_CHECK_EQUAL(charmander->get_species(), "Charmander");
        BOOST_CHECK_EQUAL(squirtle->get_species(), "Squirtle");

        // On the C++ level, make sure the expected equal pointers are equal.
        BOOST_CHECK_NE(party->get_pokemon(0)->get_native_pc_data(), original_first->get_native_pc_data());
        BOOST_CHECK_NE(party->get_pokemon(1)->get_native_pc_data(), original_second->get_native_pc_data());

        // On the C++ level, check the underlying PKSav structs.
        switch(generation) {
            case 1: {
                pksav_gen1_pokemon_party_t* native = reinterpret_cast<pksav_gen1_pokemon_party_t*>(party->get_native());
                BOOST_CHECK_EQUAL(native->count, 3);
                BOOST_CHECK_EQUAL(native->species[0], squirtle->get_database_entry().get_pokemon_index());
                BOOST_CHECK_EQUAL(native->species[1], charmander->get_database_entry().get_pokemon_index());
                BOOST_CHECK_EQUAL(native->species[2], charmander->get_database_entry().get_pokemon_index());
                BOOST_CHECK_EQUAL(native->species[3], 0);
                BOOST_CHECK_EQUAL(native->species[4], 0);
                BOOST_CHECK_EQUAL(native->species[5], 0);
                for(int i = 0; i < 3; ++i) {
                    BOOST_CHECK_EQUAL(&native->party[i], party->get_pokemon(i)->get_native_pc_data());
                    BOOST_CHECK_EQUAL(int(native->species[i]), party->get_pokemon(i)->get_database_entry().get_pokemon_index());
                    BOOST_CHECK_EQUAL(int(native->party[i].pc.species), party->get_pokemon(i)->get_database_entry().get_pokemon_index());

                    char nickname[11] = {};
                    PKSAV_CALL(
                        pksav_text_from_gen1(
                            native->nicknames[i],
                            nickname,
                            10
                        );
                    )
                    BOOST_CHECK(std::strlen(nickname) > 0);
                    BOOST_CHECK_EQUAL(
                        party->get_pokemon(i)->get_nickname(),
                        nickname
                    );

                    char otname[11] = {};
                    PKSAV_CALL(
                        pksav_text_from_gen1(
                            native->otnames[i],
                            otname,
                            10
                        );
                    )
                    BOOST_CHECK(std::strlen(otname) > 0);
                    BOOST_CHECK_EQUAL(
                        party->get_pokemon(i)->get_trainer_name(),
                        otname 
                    );
                }
                break;
            }

            case 2: {
                pksav_gen2_pokemon_party_t* native = reinterpret_cast<pksav_gen2_pokemon_party_t*>(party->get_native());
                BOOST_CHECK_EQUAL(native->count, 3);
                BOOST_CHECK_EQUAL(native->species[0], squirtle->get_database_entry().get_pokemon_index());
                BOOST_CHECK_EQUAL(native->species[1], charmander->get_database_entry().get_pokemon_index());
                BOOST_CHECK_EQUAL(native->species[2], charmander->get_database_entry().get_pokemon_index());
                BOOST_CHECK_EQUAL(native->species[3], 0);
                BOOST_CHECK_EQUAL(native->species[4], 0);
                BOOST_CHECK_EQUAL(native->species[5], 0);
                for(int i = 0; i < 3; ++i) {
                    BOOST_CHECK_EQUAL(&native->party[i], party->get_pokemon(i)->get_native_pc_data());
                    BOOST_CHECK_EQUAL(int(native->species[i]), party->get_pokemon(i)->get_database_entry().get_pokemon_index());
                    BOOST_CHECK_EQUAL(int(native->party[i].pc.species), party->get_pokemon(i)->get_database_entry().get_pokemon_index());

                    char nickname[11] = {};
                    PKSAV_CALL(
                        pksav_text_from_gen2(
                            native->nicknames[i],
                            nickname,
                            10
                        );
                    )
                    BOOST_CHECK(std::strlen(nickname) > 0);
                    BOOST_CHECK_EQUAL(
                        party->get_pokemon(i)->get_nickname(),
                        nickname
                    );

                    char otname[11] = {};
                    PKSAV_CALL(
                        pksav_text_from_gen2(
                            native->otnames[i],
                            otname,
                            10
                        );
                    )
                    BOOST_CHECK(std::strlen(otname) > 0);
                    BOOST_CHECK_EQUAL(
                        party->get_pokemon(i)->get_trainer_name(),
                        otname 
                    );
                }
                break;
            }

            case 3:
                if(game == "Colosseum" or game == "XD") {
                    break;
                } else {
                    pksav_gba_pokemon_party_t* native = reinterpret_cast<pksav_gba_pokemon_party_t*>(party->get_native());
                    BOOST_CHECK_EQUAL(native->count, 3);
                    BOOST_CHECK_EQUAL(native->party[0].pc.blocks.growth.species, squirtle->get_database_entry().get_pokemon_index());
                    BOOST_CHECK_EQUAL(native->party[1].pc.blocks.growth.species, charmander->get_database_entry().get_pokemon_index());
                    BOOST_CHECK_EQUAL(native->party[2].pc.blocks.growth.species, charmander->get_database_entry().get_pokemon_index());
                    BOOST_CHECK_EQUAL(native->party[3].pc.blocks.growth.species, 0);
                    BOOST_CHECK_EQUAL(native->party[4].pc.blocks.growth.species, 0);
                    BOOST_CHECK_EQUAL(native->party[5].pc.blocks.growth.species, 0);
                    for(int i = 0; i < 3; ++i) {
                        BOOST_CHECK_EQUAL(&native->party[i], party->get_pokemon(i)->get_native_pc_data());
                        BOOST_CHECK_EQUAL(native->party[i].pc.blocks.growth.species, party->get_pokemon(i)->get_database_entry().get_pokemon_index());
                    }
                }

            default:
                break;
        }
    }

}
