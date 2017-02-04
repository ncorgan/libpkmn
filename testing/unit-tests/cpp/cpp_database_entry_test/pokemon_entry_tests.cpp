/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_database_entry_test.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/database/pokemon_entry.hpp>

// Only create one main
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

namespace fs = boost::filesystem;

static const std::pair<std::string, std::string> none_pair("None", "None");
static const std::map<std::string, int> bad_stat_map_old = boost::assign::map_list_of
    ("HP", 0)("Attack", 0)("Defense", 0)
    ("Speed", 0)("Special", 0)
;
static const std::map<std::string, int> bad_stat_map = boost::assign::map_list_of
    ("HP", 0)("Attack", 0)("Defense", 0)
    ("Speed", 0)("Special Attack", 0)("Special Defense", 0)
;

static void _pokemon_entry_none_test(
    pkmn::database::pokemon_entry &none_entry
) {
    BOOST_CHECK_EQUAL(none_entry.get_name(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_species(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_pokedex_entry(), "None");
    BOOST_CHECK_EQUAL(none_entry.get_form(), "None");
    BOOST_CHECK_CLOSE(none_entry.get_height(), -1.0f, 0.0001f);
    BOOST_CHECK_CLOSE(none_entry.get_weight(), -1.0f, 0.0001f);
    BOOST_CHECK_CLOSE(none_entry.get_chance_male(), -1.0f, 0.0001f);
    BOOST_CHECK_CLOSE(none_entry.get_chance_female(), -1.0f, 0.0001f);
    BOOST_CHECK(not none_entry.has_gender_differences());
    BOOST_CHECK_EQUAL(none_entry.get_base_friendship(), -1.0f);
    BOOST_CHECK(none_entry.get_types() == none_pair);
    BOOST_CHECK(none_entry.get_abilities() == none_pair);
    BOOST_CHECK_EQUAL(none_entry.get_hidden_ability(), "None");
    BOOST_CHECK(none_entry.get_egg_groups() == none_pair);
    if(none_entry.get_game_id() == 1) {
        BOOST_CHECK(none_entry.get_base_stats() == bad_stat_map_old);
        BOOST_CHECK(none_entry.get_EV_yields() == bad_stat_map_old);
    } else if(none_entry.get_game_id() == 4) {
        BOOST_CHECK(none_entry.get_base_stats() == bad_stat_map);
        BOOST_CHECK(none_entry.get_EV_yields() == bad_stat_map_old);
    } else {
        BOOST_CHECK(none_entry.get_base_stats() == bad_stat_map);
        BOOST_CHECK(none_entry.get_EV_yields() == bad_stat_map);
    }
    BOOST_CHECK_EQUAL(none_entry.get_experience_yield(), -1);
    BOOST_CHECK_EQUAL(none_entry.get_experience_at_level(2), -1);
    BOOST_CHECK_EQUAL(none_entry.get_level_at_experience(2), -1);
    BOOST_CHECK_EQUAL(none_entry.get_levelup_moves().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_tm_hm_moves().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_egg_moves().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_forms().size(), 0);
    BOOST_CHECK_EQUAL(none_entry.get_evolutions().size(), 0);
}

/*
 * Make sure "None" move entries output expected results for all generations.
 */
BOOST_AUTO_TEST_CASE(pokemon_entry_none_test) {
    for(int i = 0; i < 6; ++i) {
        pkmn::database::pokemon_entry none_byindex(0, game_ids[i]);
        pkmn::database::pokemon_entry none_byname("None", game_names[i], "");

        _pokemon_entry_none_test(none_byindex);
        _pokemon_entry_none_test(none_byname);
    }
}

/*
 * Make sure specifying "" as the form corresponds to the correct Standard form.
 */
BOOST_AUTO_TEST_CASE(pokemon_entry_standard_form_test) {
    pkmn::database::pokemon_entry pikachu1("Pikachu", "Black 2", "");
    pkmn::database::pokemon_entry pikachu2("Pikachu", "Black 2", "Standard");
    BOOST_CHECK(pikachu1 == pikachu2);
    BOOST_CHECK_EQUAL(pikachu1.get_species_id(), pikachu1.get_pokemon_id());
    BOOST_CHECK_EQUAL(pikachu1.get_form(), "Standard");
    BOOST_CHECK_EQUAL(pikachu1.get_icon_filepath(false), pikachu2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(pikachu1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(pikachu1.get_icon_filepath(true), pikachu2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(pikachu1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(pikachu1.get_sprite_filepath(false,false), pikachu2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(pikachu1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(pikachu1.get_sprite_filepath(false,true), pikachu2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(pikachu1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(pikachu1.get_sprite_filepath(true,false), pikachu2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(pikachu1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(pikachu1.get_sprite_filepath(true,true), pikachu2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(pikachu1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry unown1("Unown", "Black 2", "");
    pkmn::database::pokemon_entry unown2("Unown", "Black 2", "A");
    BOOST_CHECK(unown1 == unown2);
    BOOST_CHECK_EQUAL(unown1.get_species_id(), unown1.get_pokemon_id());
    BOOST_CHECK_EQUAL(unown1.get_form(), "A");
    BOOST_CHECK_EQUAL(unown1.get_icon_filepath(false), unown2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(unown1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(unown1.get_icon_filepath(true), unown2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(unown1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(unown1.get_sprite_filepath(false,false), unown2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(unown1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(unown1.get_sprite_filepath(false,true), unown2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(unown1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(unown1.get_sprite_filepath(true,false), unown2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(unown1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(unown1.get_sprite_filepath(true,true), unown2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(unown1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry castform1("Castform", "Black 2", "");
    pkmn::database::pokemon_entry castform2("Castform", "Black 2", "Standard");
    BOOST_CHECK(castform1 == castform2);
    BOOST_CHECK_EQUAL(castform1.get_species_id(), castform1.get_pokemon_id());
    BOOST_CHECK_EQUAL(castform1.get_form(), "Standard");
    BOOST_CHECK_EQUAL(castform1.get_icon_filepath(false), castform2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(castform1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(castform1.get_icon_filepath(true), castform2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(castform1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(castform1.get_sprite_filepath(false,false), castform2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(castform1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(castform1.get_sprite_filepath(false,true), castform2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(castform1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(castform1.get_sprite_filepath(true,false), castform2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(castform1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(castform1.get_sprite_filepath(true,true), castform2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(castform1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry deoxys1("Deoxys", "Black 2", "");
    pkmn::database::pokemon_entry deoxys2("Deoxys", "Black 2", "Normal");
    BOOST_CHECK(deoxys1 == deoxys2);
    BOOST_CHECK_EQUAL(deoxys1.get_species_id(), deoxys1.get_pokemon_id());
    BOOST_CHECK_EQUAL(deoxys1.get_form(), "Normal");
    BOOST_CHECK_EQUAL(deoxys1.get_icon_filepath(false), deoxys2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(deoxys1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(deoxys1.get_icon_filepath(true), deoxys2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(deoxys1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(deoxys1.get_sprite_filepath(false,false), deoxys2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(deoxys1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(deoxys1.get_sprite_filepath(false,true), deoxys2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(deoxys1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(deoxys1.get_sprite_filepath(true,false), deoxys2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(deoxys1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(deoxys1.get_sprite_filepath(true,true), deoxys2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(deoxys1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry burmy1("Burmy", "Black 2", "");
    pkmn::database::pokemon_entry burmy2("Burmy", "Black 2", "Plant");
    BOOST_CHECK(burmy1 == burmy2);
    BOOST_CHECK_EQUAL(burmy1.get_species_id(), burmy1.get_pokemon_id());
    BOOST_CHECK_EQUAL(burmy1.get_form(), "Plant");
    BOOST_CHECK_EQUAL(burmy1.get_icon_filepath(false), burmy2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(burmy1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(burmy1.get_icon_filepath(true), burmy2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(burmy1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(burmy1.get_sprite_filepath(false,false), burmy2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(burmy1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(burmy1.get_sprite_filepath(false,true), burmy2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(burmy1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(burmy1.get_sprite_filepath(true,false), burmy2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(burmy1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(burmy1.get_sprite_filepath(true,true), burmy2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(burmy1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry wormadam1("Wormadam", "Black 2", "");
    pkmn::database::pokemon_entry wormadam2("Wormadam", "Black 2", "Plant");
    BOOST_CHECK(wormadam1 == wormadam2);
    BOOST_CHECK_EQUAL(wormadam1.get_species_id(), wormadam1.get_pokemon_id());
    BOOST_CHECK_EQUAL(wormadam1.get_form(), "Plant");
    BOOST_CHECK_EQUAL(wormadam1.get_icon_filepath(false), wormadam2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(wormadam1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(wormadam1.get_icon_filepath(true), wormadam2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(wormadam1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(wormadam1.get_sprite_filepath(false,false), wormadam2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(wormadam1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(wormadam1.get_sprite_filepath(false,true), wormadam2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(wormadam1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(wormadam1.get_sprite_filepath(true,false), wormadam2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(wormadam1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(wormadam1.get_sprite_filepath(true,true), wormadam2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(wormadam1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry cherrim1("Cherrim", "Black 2", "Overcast");
    pkmn::database::pokemon_entry cherrim2("Cherrim", "Black 2", "Overcast");
    BOOST_CHECK(cherrim1 == cherrim2);
    BOOST_CHECK_EQUAL(cherrim1.get_species_id(), cherrim1.get_pokemon_id());
    BOOST_CHECK_EQUAL(cherrim1.get_form(), "Overcast");
    BOOST_CHECK_EQUAL(cherrim1.get_icon_filepath(false), cherrim2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(cherrim1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(cherrim1.get_icon_filepath(true), cherrim2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(cherrim1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(cherrim1.get_sprite_filepath(false,false), cherrim2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(cherrim1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(cherrim1.get_sprite_filepath(false,true), cherrim2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(cherrim1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(cherrim1.get_sprite_filepath(true,false), cherrim2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(cherrim1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(cherrim1.get_sprite_filepath(true,true), cherrim2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(cherrim1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry shellos1("Shellos", "Black 2", "");
    pkmn::database::pokemon_entry shellos2("Shellos", "Black 2", "West");
    BOOST_CHECK(shellos1 == shellos2);
    BOOST_CHECK_EQUAL(shellos1.get_species_id(), shellos1.get_pokemon_id());
    BOOST_CHECK_EQUAL(shellos1.get_form(), "West");
    BOOST_CHECK_EQUAL(shellos1.get_icon_filepath(false), shellos2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(shellos1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(shellos1.get_icon_filepath(true), shellos2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(shellos1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(shellos1.get_sprite_filepath(false,false), shellos2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(shellos1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(shellos1.get_sprite_filepath(false,true), shellos2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(shellos1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(shellos1.get_sprite_filepath(true,false), shellos2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(shellos1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(shellos1.get_sprite_filepath(true,true), shellos2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(shellos1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry gastrodon1("Gastrodon", "Black 2", "");
    pkmn::database::pokemon_entry gastrodon2("Gastrodon", "Black 2", "West");
    BOOST_CHECK(gastrodon1 == gastrodon2);
    BOOST_CHECK_EQUAL(gastrodon1.get_species_id(), gastrodon1.get_pokemon_id());
    BOOST_CHECK_EQUAL(gastrodon1.get_form(), "West");
    BOOST_CHECK_EQUAL(gastrodon1.get_icon_filepath(false), gastrodon2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(gastrodon1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(gastrodon1.get_icon_filepath(true), gastrodon2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(gastrodon1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(gastrodon1.get_sprite_filepath(false,false), gastrodon2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(gastrodon1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(gastrodon1.get_sprite_filepath(false,true), gastrodon2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(gastrodon1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(gastrodon1.get_sprite_filepath(true,false), gastrodon2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(gastrodon1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(gastrodon1.get_sprite_filepath(true,true), gastrodon2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(gastrodon1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry giratina1("Giratina", "Black 2", "");
    pkmn::database::pokemon_entry giratina2("Giratina", "Black 2", "Altered");
    BOOST_CHECK(giratina1 == giratina2);
    BOOST_CHECK_EQUAL(giratina1.get_species_id(), giratina1.get_pokemon_id());
    BOOST_CHECK_EQUAL(giratina1.get_form(), "Altered");
    BOOST_CHECK_EQUAL(giratina1.get_icon_filepath(false), giratina2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(giratina1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(giratina1.get_icon_filepath(true), giratina2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(giratina1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(giratina1.get_sprite_filepath(false,false), giratina2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(giratina1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(giratina1.get_sprite_filepath(false,true), giratina2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(giratina1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(giratina1.get_sprite_filepath(true,false), giratina2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(giratina1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(giratina1.get_sprite_filepath(true,true), giratina2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(giratina1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry shaymin1("Shaymin", "Black 2", "");
    pkmn::database::pokemon_entry shaymin2("Shaymin", "Black 2", "Land");
    BOOST_CHECK(shaymin1 == shaymin2);
    BOOST_CHECK_EQUAL(shaymin1.get_species_id(), shaymin1.get_pokemon_id());
    BOOST_CHECK_EQUAL(shaymin1.get_form(), "Land");
    BOOST_CHECK_EQUAL(shaymin1.get_icon_filepath(false), shaymin2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(shaymin1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(shaymin1.get_icon_filepath(true), shaymin2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(shaymin1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(shaymin1.get_sprite_filepath(false,false), shaymin2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(shaymin1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(shaymin1.get_sprite_filepath(false,true), shaymin2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(shaymin1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(shaymin1.get_sprite_filepath(true,false), shaymin2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(shaymin1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(shaymin1.get_sprite_filepath(true,true), shaymin2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(shaymin1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry arceus1("Arceus", "Black 2", "");
    pkmn::database::pokemon_entry arceus2("Arceus", "Black 2", "Normal");
    BOOST_CHECK(arceus1 == arceus2);
    BOOST_CHECK_EQUAL(arceus1.get_species_id(), arceus1.get_pokemon_id());
    BOOST_CHECK_EQUAL(arceus1.get_form(), "Normal");
    BOOST_CHECK_EQUAL(arceus1.get_icon_filepath(false), arceus2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(arceus1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(arceus1.get_icon_filepath(true), arceus2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(arceus1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(arceus1.get_sprite_filepath(false,false), arceus2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(arceus1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(arceus1.get_sprite_filepath(false,true), arceus2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(arceus1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(arceus1.get_sprite_filepath(true,false), arceus2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(arceus1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(arceus1.get_sprite_filepath(true,true), arceus2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(arceus1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry basculin1("Basculin", "Black 2", "");
    pkmn::database::pokemon_entry basculin2("Basculin", "Black 2", "Red-Striped");
    BOOST_CHECK(basculin1 == basculin2);
    BOOST_CHECK_EQUAL(basculin1.get_species_id(), basculin1.get_pokemon_id());
    BOOST_CHECK_EQUAL(basculin1.get_form(), "Red-Striped");
    BOOST_CHECK_EQUAL(basculin1.get_icon_filepath(false), basculin2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(basculin1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(basculin1.get_icon_filepath(true), basculin2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(basculin1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(basculin1.get_sprite_filepath(false,false), basculin2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(basculin1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(basculin1.get_sprite_filepath(false,true), basculin2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(basculin1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(basculin1.get_sprite_filepath(true,false), basculin2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(basculin1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(basculin1.get_sprite_filepath(true,true), basculin2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(basculin1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry darmanitan1("Darmanitan", "Black 2", "");
    pkmn::database::pokemon_entry darmanitan2("Darmanitan", "Black 2", "Standard");
    BOOST_CHECK(darmanitan1 == darmanitan2);
    BOOST_CHECK_EQUAL(darmanitan1.get_species_id(), darmanitan1.get_pokemon_id());
    BOOST_CHECK_EQUAL(darmanitan1.get_form(), "Standard");
    BOOST_CHECK_EQUAL(darmanitan1.get_icon_filepath(false), darmanitan2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(darmanitan1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(darmanitan1.get_icon_filepath(true), darmanitan2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(darmanitan1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(darmanitan1.get_sprite_filepath(false,false), darmanitan2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(darmanitan1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(darmanitan1.get_sprite_filepath(false,true), darmanitan2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(darmanitan1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(darmanitan1.get_sprite_filepath(true,false), darmanitan2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(darmanitan1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(darmanitan1.get_sprite_filepath(true,true), darmanitan2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(darmanitan1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry deerling1("Deerling", "Black 2", "");
    pkmn::database::pokemon_entry deerling2("Deerling", "Black 2", "Spring");
    BOOST_CHECK(deerling1 == deerling2);
    BOOST_CHECK_EQUAL(deerling1.get_species_id(), deerling1.get_pokemon_id());
    BOOST_CHECK_EQUAL(deerling1.get_form(), "Spring");
    BOOST_CHECK_EQUAL(deerling1.get_icon_filepath(false), deerling2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(deerling1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(deerling1.get_icon_filepath(true), deerling2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(deerling1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(deerling1.get_sprite_filepath(false,false), deerling2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(deerling1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(deerling1.get_sprite_filepath(false,true), deerling2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(deerling1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(deerling1.get_sprite_filepath(true,false), deerling2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(deerling1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(deerling1.get_sprite_filepath(true,true), deerling2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(deerling1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry sawsbuck1("Sawsbuck", "Black 2", "");
    pkmn::database::pokemon_entry sawsbuck2("Sawsbuck", "Black 2", "Spring");
    BOOST_CHECK(sawsbuck1 == sawsbuck2);
    BOOST_CHECK_EQUAL(sawsbuck1.get_species_id(), sawsbuck1.get_pokemon_id());
    BOOST_CHECK_EQUAL(sawsbuck1.get_form(), "Spring");
    BOOST_CHECK_EQUAL(sawsbuck1.get_icon_filepath(false), sawsbuck2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(sawsbuck1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(sawsbuck1.get_icon_filepath(true), sawsbuck2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(sawsbuck1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(sawsbuck1.get_sprite_filepath(false,false), sawsbuck2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(sawsbuck1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(sawsbuck1.get_sprite_filepath(false,true), sawsbuck2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(sawsbuck1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(sawsbuck1.get_sprite_filepath(true,false), sawsbuck2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(sawsbuck1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(sawsbuck1.get_sprite_filepath(true,true), sawsbuck2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(sawsbuck1.get_sprite_filepath(true,true)));

    static BOOST_CONSTEXPR const char* incarnates[] = {
        "Tornadus", "Thundurus", "Landorus"
    };
    for(int i = 0; i < 3; ++i) {
        pkmn::database::pokemon_entry incarnate1(incarnates[i], "Black 2", "");
        pkmn::database::pokemon_entry incarnate2(incarnates[i], "Black 2", "Incarnate");
        BOOST_CHECK(incarnate1 == incarnate2);
        BOOST_CHECK_EQUAL(incarnate1.get_species_id(), incarnate1.get_pokemon_id());
        BOOST_CHECK_EQUAL(incarnate1.get_form(), "Incarnate");
        BOOST_CHECK_EQUAL(incarnate1.get_icon_filepath(false), incarnate2.get_icon_filepath(false));
        BOOST_CHECK(fs::exists(incarnate1.get_icon_filepath(false)));
        BOOST_CHECK_EQUAL(incarnate1.get_icon_filepath(true), incarnate2.get_icon_filepath(true));
        BOOST_CHECK(fs::exists(incarnate1.get_icon_filepath(true)));
        BOOST_CHECK_EQUAL(incarnate1.get_sprite_filepath(false,false), incarnate2.get_sprite_filepath(false,false));
        BOOST_CHECK(fs::exists(incarnate1.get_sprite_filepath(false,false)));
        BOOST_CHECK_EQUAL(incarnate1.get_sprite_filepath(false,true), incarnate2.get_sprite_filepath(false,true));
        BOOST_CHECK(fs::exists(incarnate1.get_sprite_filepath(false,true)));
        BOOST_CHECK_EQUAL(incarnate1.get_sprite_filepath(true,false), incarnate2.get_sprite_filepath(true,false));
        BOOST_CHECK(fs::exists(incarnate1.get_sprite_filepath(true,false)));
        BOOST_CHECK_EQUAL(incarnate1.get_sprite_filepath(true,true), incarnate2.get_sprite_filepath(true,true));
        BOOST_CHECK(fs::exists(incarnate1.get_sprite_filepath(true,true)));
    }

    pkmn::database::pokemon_entry keldeo1("Keldeo", "Black 2", "");
    pkmn::database::pokemon_entry keldeo2("Keldeo", "Black 2", "Ordinary");
    BOOST_CHECK(keldeo1 == keldeo2);
    BOOST_CHECK_EQUAL(keldeo1.get_species_id(), keldeo1.get_pokemon_id());
    BOOST_CHECK_EQUAL(keldeo1.get_form(), "Ordinary");
    BOOST_CHECK_EQUAL(keldeo1.get_icon_filepath(false), keldeo2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(keldeo1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(keldeo1.get_icon_filepath(true), keldeo2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(keldeo1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(keldeo1.get_sprite_filepath(false,false), keldeo2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(keldeo1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(keldeo1.get_sprite_filepath(false,true), keldeo2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(keldeo1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(keldeo1.get_sprite_filepath(true,false), keldeo2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(keldeo1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(keldeo1.get_sprite_filepath(true,true), keldeo2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(keldeo1.get_sprite_filepath(true,true)));

    pkmn::database::pokemon_entry meloetta1("Meloetta", "Black 2", "");
    pkmn::database::pokemon_entry meloetta2("Meloetta", "Black 2", "Aria");
    BOOST_CHECK(meloetta1 == meloetta2);
    BOOST_CHECK_EQUAL(meloetta1.get_species_id(), meloetta1.get_pokemon_id());
    BOOST_CHECK_EQUAL(meloetta1.get_form(), "Aria");
    BOOST_CHECK_EQUAL(meloetta1.get_icon_filepath(false), meloetta2.get_icon_filepath(false));
    BOOST_CHECK(fs::exists(meloetta1.get_icon_filepath(false)));
    BOOST_CHECK_EQUAL(meloetta1.get_icon_filepath(true), meloetta2.get_icon_filepath(true));
    BOOST_CHECK(fs::exists(meloetta1.get_icon_filepath(true)));
    BOOST_CHECK_EQUAL(meloetta1.get_sprite_filepath(false,false), meloetta2.get_sprite_filepath(false,false));
    BOOST_CHECK(fs::exists(meloetta1.get_sprite_filepath(false,false)));
    BOOST_CHECK_EQUAL(meloetta1.get_sprite_filepath(false,true), meloetta2.get_sprite_filepath(false,true));
    BOOST_CHECK(fs::exists(meloetta1.get_sprite_filepath(false,true)));
    BOOST_CHECK_EQUAL(meloetta1.get_sprite_filepath(true,false), meloetta2.get_sprite_filepath(true,false));
    BOOST_CHECK(fs::exists(meloetta1.get_sprite_filepath(true,false)));
    BOOST_CHECK_EQUAL(meloetta1.get_sprite_filepath(true,true), meloetta2.get_sprite_filepath(true,true));
    BOOST_CHECK(fs::exists(meloetta1.get_sprite_filepath(true,true)));

    // TODO: Gen VI after sprites and icons brought in
}

/*
 * Make sure Pokémon entries can't be instantiated for games that didn't
 * have the given Pokémon.
 */
BOOST_AUTO_TEST_CASE(pokemon_entry_wrong_game_test) {
    // Pokémon from later generations
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry snubbull(
            "Snubbull", "Yellow", ""
        );
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry ralts(
            "Ralts", "Silver", ""
        );
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry budew(
            "Budew", "Sapphire", ""
        );
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry liepard(
            "Liepard", "Pearl", ""
        );
    , std::invalid_argument);
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry espurr(
            "Espurr", "White", ""
        );
    , std::invalid_argument);

    /*
     * Make sure forms can't be used in the wrong games, even if
     * the Pokémon existed.
     */
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry arceus(
            "Arceus", "Diamond", "Fairy"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus1(
        "Arceus", "Y", "Fairy"
    );
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry arceus(
            "Arceus", "Black", "???"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry arceus2(
        "Arceus", "Pearl", "???"
    );
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry pichu(
            "Pichu", "Silver", "Spiky-eared"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry pichu(
        "Pichu", "HeartGold", "Spiky-eared"
    );
    BOOST_CHECK_THROW(
        pkmn::database::pokemon_entry pikachu(
            "Pikachu", "Ruby", "Belle"
        );
    , std::invalid_argument);
    pkmn::database::pokemon_entry pikachu(
        "Pikachu", "Omega Ruby", "Belle"
    );
}

/*
 * Make sure Pokémon whose type has changed between generations
 * reflect that in their entries.
 */
BOOST_AUTO_TEST_CASE(changing_type_test) {
    pkmn::database::pokemon_entry clefairy5(
        "Clefairy", "Black", ""
    );
    pkmn::database::pokemon_entry clefairy6(
        "Clefairy", "Y", ""
    );

    std::pair<std::string, std::string> clefairy_types5 = clefairy5.get_types();
    std::pair<std::string, std::string> clefairy_types6 = clefairy6.get_types();

    BOOST_CHECK_EQUAL(clefairy_types5.first, "Normal");
    BOOST_CHECK_EQUAL(clefairy_types5.second, "None");

    BOOST_CHECK_EQUAL(clefairy_types6.first, "Fairy");
    BOOST_CHECK_EQUAL(clefairy_types6.second, "None");

    pkmn::database::pokemon_entry jigglypuff5(
        "Jigglypuff", "Black", ""
    );
    pkmn::database::pokemon_entry jigglypuff6(
        "Jigglypuff", "Y", ""
    );

    std::pair<std::string, std::string> jigglypuff_types5 = jigglypuff5.get_types();
    std::pair<std::string, std::string> jigglypuff_types6 = jigglypuff6.get_types();

    BOOST_CHECK_EQUAL(jigglypuff_types5.first, "Normal");
    BOOST_CHECK_EQUAL(jigglypuff_types5.second, "None");

    BOOST_CHECK_EQUAL(jigglypuff_types6.first, "Normal");
    BOOST_CHECK_EQUAL(jigglypuff_types6.second, "Fairy");
}

static void _check_eevee_evolutions(
    const pkmn::database::pokemon_entries_t &evolutions,
    int generation
) {
    BOOST_REQUIRE_GE(evolutions.size(), 3);

    BOOST_CHECK_EQUAL(evolutions.at(0).get_name(), "Vaporeon");
    BOOST_CHECK_EQUAL(evolutions.at(1).get_name(), "Jolteon");
    BOOST_CHECK_EQUAL(evolutions.at(2).get_name(), "Flareon");

    if(generation >= 2) {
        BOOST_CHECK_EQUAL(evolutions.at(3).get_name(), "Espeon");
        BOOST_CHECK_EQUAL(evolutions.at(4).get_name(), "Umbreon");
    } else {
        BOOST_CHECK_EQUAL(evolutions.size(), 3);
        return;
    }

    if(generation >= 4) {
        BOOST_CHECK_EQUAL(evolutions.at(5).get_name(), "Leafeon");
        BOOST_CHECK_EQUAL(evolutions.at(6).get_name(), "Glaceon");
    } else {
        BOOST_CHECK_EQUAL(evolutions.size(), 5);
        return;
    }

    if(generation >= 6) {
        BOOST_CHECK_EQUAL(evolutions.at(7).get_name(), "Sylveon");
    } else {
        BOOST_CHECK_EQUAL(evolutions.size(), 7);
    }
}

/*
 * Make sure the same Pokémon from different generations shows
 * the correct evolutions.
 */
BOOST_AUTO_TEST_CASE(right_evolutions_test) {
    pkmn::database::pokemon_entry eevee1(
        "Eevee", "Blue", ""
    );
    pkmn::database::pokemon_entry eevee2(
        "Eevee", "Crystal", ""
    );
    pkmn::database::pokemon_entry eevee3(
        "Eevee", "Colosseum", ""
    );
    pkmn::database::pokemon_entry eevee4(
        "Eevee", "SoulSilver", ""
    );
    pkmn::database::pokemon_entry eevee5(
        "Eevee", "White 2", ""
    );
    pkmn::database::pokemon_entry eevee6(
        "Eevee", "Alpha Sapphire", ""
    );

    _check_eevee_evolutions(
        eevee1.get_evolutions(), 1
    );
    _check_eevee_evolutions(
        eevee2.get_evolutions(), 2
    );
    _check_eevee_evolutions(
        eevee3.get_evolutions(), 3
    );
    _check_eevee_evolutions(
        eevee4.get_evolutions(), 4
    );
    _check_eevee_evolutions(
        eevee5.get_evolutions(), 5
    );
    _check_eevee_evolutions(
        eevee6.get_evolutions(), 6
    );
}

static void _pokemon_entry_test_common(
    pkmn::database::pokemon_entry &pokemon_entry_gen1,
    pkmn::database::pokemon_entry &pokemon_entry_gen2,
    pkmn::database::pokemon_entry &pokemon_entry_gba,
    pkmn::database::pokemon_entry &pokemon_entry_gcn,
    pkmn::database::pokemon_entry &pokemon_entry_gen4,
    pkmn::database::pokemon_entry &pokemon_entry_gen5,
    pkmn::database::pokemon_entry &pokemon_entry_gen6
) {
    /*
     * Generation I entry
     */
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_name(), "Articuno");
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_game(), "Yellow");
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_species(), "Freeze");
    BOOST_CHECK_GT(pokemon_entry_gen1.get_pokedex_entry().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_form(), "Standard");
    BOOST_CHECK_CLOSE(pokemon_entry_gen1.get_height(), 1.7f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen1.get_weight(), 55.4f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen1.get_chance_male(), 0.0f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen1.get_chance_female(), 0.0f, 0.0001f);
    BOOST_CHECK(not pokemon_entry_gen1.has_gender_differences());
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_base_friendship(), -1);

    std::pair<std::string, std::string> types1 = pokemon_entry_gen1.get_types();
    BOOST_CHECK_EQUAL(types1.first, "Ice");
    BOOST_CHECK_EQUAL(types1.second, "Flying");

    BOOST_CHECK(pokemon_entry_gen1.get_abilities() == none_pair);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_hidden_ability(), "None");
    BOOST_CHECK(pokemon_entry_gen1.get_egg_groups() == none_pair);

    std::map<std::string, int> base_stats1 = pokemon_entry_gen1.get_base_stats();
    BOOST_CHECK_EQUAL(base_stats1.at("HP"), 90);
    BOOST_CHECK_EQUAL(base_stats1.at("Attack"), 85);
    BOOST_CHECK_EQUAL(base_stats1.at("Defense"), 100);
    BOOST_CHECK_EQUAL(base_stats1.at("Speed"), 85);
    BOOST_CHECK_EQUAL(base_stats1.at("Special"), 125);

    BOOST_CHECK(pokemon_entry_gen1.get_EV_yields() == base_stats1);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_experience_yield(), 215);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_experience_at_level(50), 156250);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_level_at_experience(200000), 54);
    BOOST_CHECK_GT(pokemon_entry_gen1.get_levelup_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gen1.get_tm_hm_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_egg_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_forms().size(), 1);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_evolutions().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_species_id(), 144);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_pokemon_id(), 144);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_form_id(), 144);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_pokemon_index(), 74);
    BOOST_CHECK_EQUAL(pokemon_entry_gen1.get_game_id(), 3);

    BOOST_CHECK(fs::exists(pokemon_entry_gen1.get_icon_filepath(false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen1.get_icon_filepath(true)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen1.get_sprite_filepath(false,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen1.get_sprite_filepath(true,false)));

    // No shininess in Generation I
    BOOST_CHECK_THROW(
        (void)pokemon_entry_gen1.get_sprite_filepath(false,true);
    , pkmn::feature_not_in_game_error);
    BOOST_CHECK_THROW(
        (void)pokemon_entry_gen1.get_sprite_filepath(true,true);
    , pkmn::feature_not_in_game_error);

    /*
     * Generation II entry
     */
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_name(), "Feraligatr");
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_game(), "Gold");
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_species(), "Big Jaw");
    BOOST_CHECK_GT(pokemon_entry_gen2.get_pokedex_entry().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_form(), "Standard");
    BOOST_CHECK_CLOSE(pokemon_entry_gen2.get_height(), 2.3f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen2.get_weight(), 88.8f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen2.get_chance_male(), 0.875f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen2.get_chance_female(), 0.125f, 0.0001f);
    BOOST_CHECK(not pokemon_entry_gen2.has_gender_differences());
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_base_friendship(), 70);

    std::pair<std::string, std::string> types2 = pokemon_entry_gen2.get_types();
    BOOST_CHECK_EQUAL(types2.first, "Water");
    BOOST_CHECK_EQUAL(types2.second, "None");

    BOOST_CHECK(pokemon_entry_gen2.get_abilities() == none_pair);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_hidden_ability(), "None");

    std::pair<std::string, std::string> egg_groups2 = pokemon_entry_gen2.get_egg_groups();
    BOOST_CHECK_EQUAL(egg_groups2.first, "Monster");
    BOOST_CHECK_EQUAL(egg_groups2.second, "Water 1");

    std::map<std::string, int> base_stats2 = pokemon_entry_gen2.get_base_stats();
    BOOST_CHECK_EQUAL(base_stats2.at("HP"), 85);
    BOOST_CHECK_EQUAL(base_stats2.at("Attack"), 105);
    BOOST_CHECK_EQUAL(base_stats2.at("Defense"), 100);
    BOOST_CHECK_EQUAL(base_stats2.at("Speed"), 78);
    BOOST_CHECK_EQUAL(base_stats2.at("Special Attack"), 79);
    BOOST_CHECK_EQUAL(base_stats2.at("Special Defense"), 83);

    std::map<std::string, int> EV_yields2 = pokemon_entry_gen2.get_EV_yields();
    BOOST_CHECK_EQUAL(EV_yields2.at("HP"), 85);
    BOOST_CHECK_EQUAL(EV_yields2.at("Attack"), 105);
    BOOST_CHECK_EQUAL(EV_yields2.at("Defense"), 100);
    BOOST_CHECK_EQUAL(EV_yields2.at("Speed"), 78);
    BOOST_CHECK_EQUAL(EV_yields2.at("Special"), 79);

    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_experience_yield(), 210);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_experience_at_level(50), 117360);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_level_at_experience(200000), 59);
    BOOST_CHECK_GT(pokemon_entry_gen2.get_levelup_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gen2.get_tm_hm_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gen2.get_egg_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_forms().size(), 1);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_evolutions().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_species_id(), 160);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_pokemon_id(), 160);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_form_id(), 160);
    BOOST_CHECK_EQUAL(pokemon_entry_gen2.get_pokemon_index(), 160);

    BOOST_CHECK(fs::exists(pokemon_entry_gen2.get_icon_filepath(false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen2.get_icon_filepath(true)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen2.get_sprite_filepath(false,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen2.get_sprite_filepath(true,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen2.get_sprite_filepath(true,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen2.get_sprite_filepath(true,true)));

    /*
     * Game Boy Advance entry
     */
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_name(), "Regice");
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_game(), "Emerald");
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_species(), "Iceberg");
    BOOST_CHECK_GT(pokemon_entry_gba.get_pokedex_entry().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_form(), "Standard");
    BOOST_CHECK_CLOSE(pokemon_entry_gba.get_height(), 1.8f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gba.get_weight(), 175.0f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gba.get_chance_male(), 0.0f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gba.get_chance_female(), 0.0f, 0.0001f);
    BOOST_CHECK(not pokemon_entry_gba.has_gender_differences());
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_base_friendship(), 35);

    std::pair<std::string, std::string> types_gba = pokemon_entry_gba.get_types();
    BOOST_CHECK_EQUAL(types_gba.first, "Ice");
    BOOST_CHECK_EQUAL(types_gba.second, "None");

    std::pair<std::string, std::string> abilities_gba = pokemon_entry_gba.get_abilities();
    BOOST_CHECK_EQUAL(abilities_gba.first, "Clear Body");
    BOOST_CHECK_EQUAL(abilities_gba.second, "None");

    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_hidden_ability(), "None");

    std::pair<std::string, std::string> egg_groups_gba = pokemon_entry_gba.get_egg_groups();
    BOOST_CHECK_EQUAL(egg_groups_gba.first, "Undiscovered");
    BOOST_CHECK_EQUAL(egg_groups_gba.second, "None");

    std::map<std::string, int> base_stats_gba = pokemon_entry_gba.get_base_stats();
    BOOST_CHECK_EQUAL(base_stats_gba.at("HP"), 80);
    BOOST_CHECK_EQUAL(base_stats_gba.at("Attack"), 50);
    BOOST_CHECK_EQUAL(base_stats_gba.at("Defense"), 100);
    BOOST_CHECK_EQUAL(base_stats_gba.at("Speed"), 50);
    BOOST_CHECK_EQUAL(base_stats_gba.at("Special Attack"), 100);
    BOOST_CHECK_EQUAL(base_stats_gba.at("Special Defense"), 200);

    std::map<std::string, int> EV_yields_gba = pokemon_entry_gba.get_EV_yields();
    BOOST_CHECK_EQUAL(EV_yields_gba.at("HP"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gba.at("Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gba.at("Defense"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gba.at("Speed"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gba.at("Special Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gba.at("Special Defense"), 3);

    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_experience_yield(), 216);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_experience_at_level(50), 156250);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_level_at_experience(200000), 54);
    BOOST_CHECK_GT(pokemon_entry_gba.get_levelup_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gba.get_tm_hm_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_egg_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gba.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_forms().size(), 1);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_evolutions().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_species_id(), 378);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_pokemon_id(), 378);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_form_id(), 378);
    BOOST_CHECK_EQUAL(pokemon_entry_gba.get_pokemon_index(), 402);

    BOOST_CHECK(fs::exists(pokemon_entry_gba.get_icon_filepath(false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gba.get_icon_filepath(true)));
    BOOST_CHECK(fs::exists(pokemon_entry_gba.get_sprite_filepath(false,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gba.get_sprite_filepath(true,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gba.get_sprite_filepath(true,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gba.get_sprite_filepath(true,true)));

    /*
     * Gamecube entry
     */
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_name(), "Shroomish");
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_game(), "XD");
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_species(), "Mushroom");
    BOOST_CHECK_GT(pokemon_entry_gcn.get_pokedex_entry().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_form(), "Standard");
    BOOST_CHECK_CLOSE(pokemon_entry_gcn.get_height(), 0.4f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gcn.get_weight(), 4.5f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gcn.get_chance_male(), 0.5f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gcn.get_chance_female(), 0.5f, 0.0001f);
    BOOST_CHECK(not pokemon_entry_gcn.has_gender_differences());
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_base_friendship(), 70);

    std::pair<std::string, std::string> types_gcn = pokemon_entry_gcn.get_types();
    BOOST_CHECK_EQUAL(types_gcn.first, "Grass");
    BOOST_CHECK_EQUAL(types_gcn.second, "None");

    std::pair<std::string, std::string> abilities_gcn = pokemon_entry_gcn.get_abilities();
    BOOST_CHECK_EQUAL(abilities_gcn.first, "Effect Spore");
    BOOST_CHECK_EQUAL(abilities_gcn.second, "Poison Heal");

    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_hidden_ability(), "None");

    std::pair<std::string, std::string> egg_groups_gcn = pokemon_entry_gcn.get_egg_groups();
    BOOST_CHECK_EQUAL(egg_groups_gcn.first, "Fairy");
    BOOST_CHECK_EQUAL(egg_groups_gcn.second, "Grass");

    std::map<std::string, int> base_stats_gcn = pokemon_entry_gcn.get_base_stats();
    BOOST_CHECK_EQUAL(base_stats_gcn.at("HP"), 60);
    BOOST_CHECK_EQUAL(base_stats_gcn.at("Attack"), 40);
    BOOST_CHECK_EQUAL(base_stats_gcn.at("Defense"), 60);
    BOOST_CHECK_EQUAL(base_stats_gcn.at("Speed"), 35);
    BOOST_CHECK_EQUAL(base_stats_gcn.at("Special Attack"), 40);
    BOOST_CHECK_EQUAL(base_stats_gcn.at("Special Defense"), 60);

    std::map<std::string, int> EV_yields_gcn = pokemon_entry_gcn.get_EV_yields();
    BOOST_CHECK_EQUAL(EV_yields_gcn.at("HP"), 1);
    BOOST_CHECK_EQUAL(EV_yields_gcn.at("Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gcn.at("Defense"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gcn.at("Speed"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gcn.at("Special Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gcn.at("Special Defense"), 0);

    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_experience_yield(), 65);
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_experience_at_level(50), 142500);
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_level_at_experience(200000), 55);
    BOOST_CHECK_GT(pokemon_entry_gcn.get_levelup_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gcn.get_tm_hm_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gcn.get_egg_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gcn.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_forms().size(), 1); // TODO: Shadow
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_evolutions().size(), 1);
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_species_id(), 285);
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_pokemon_id(), 285);
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_form_id(), 285);
    BOOST_CHECK_EQUAL(pokemon_entry_gcn.get_pokemon_index(), 306);

    // TODO: sprites+icons

    /*
     * Generation IV entry
     */
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_name(), "Kricketot");
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_game(), "Pearl");
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_species(), "Cricket");
    BOOST_CHECK_GT(pokemon_entry_gen4.get_pokedex_entry().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_form(), "Standard");
    BOOST_CHECK_CLOSE(pokemon_entry_gen4.get_height(), 0.3f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen4.get_weight(), 2.2f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen4.get_chance_male(), 0.5f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen4.get_chance_female(), 0.5f, 0.0001f);
    BOOST_CHECK(pokemon_entry_gen4.has_gender_differences());
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_base_friendship(), 70);

    std::pair<std::string, std::string> types_gen4 = pokemon_entry_gen4.get_types();
    BOOST_CHECK_EQUAL(types_gen4.first, "Bug");
    BOOST_CHECK_EQUAL(types_gen4.second, "None");

    std::pair<std::string, std::string> abilities_gen4 = pokemon_entry_gen4.get_abilities();
    BOOST_CHECK_EQUAL(abilities_gen4.first, "Shed Skin");
    BOOST_CHECK_EQUAL(abilities_gen4.second, "None");

    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_hidden_ability(), "None");

    std::pair<std::string, std::string> egg_groups_gen4 = pokemon_entry_gen4.get_egg_groups();
    BOOST_CHECK_EQUAL(egg_groups_gen4.first, "Bug");
    BOOST_CHECK_EQUAL(egg_groups_gen4.second, "None");

    std::map<std::string, int> base_stats_gen4 = pokemon_entry_gen4.get_base_stats();
    BOOST_CHECK_EQUAL(base_stats_gen4.at("HP"), 37);
    BOOST_CHECK_EQUAL(base_stats_gen4.at("Attack"), 25);
    BOOST_CHECK_EQUAL(base_stats_gen4.at("Defense"), 41);
    BOOST_CHECK_EQUAL(base_stats_gen4.at("Speed"), 25);
    BOOST_CHECK_EQUAL(base_stats_gen4.at("Special Attack"), 25);
    BOOST_CHECK_EQUAL(base_stats_gen4.at("Special Defense"), 41);

    std::map<std::string, int> EV_yields_gen4 = pokemon_entry_gen4.get_EV_yields();
    BOOST_CHECK_EQUAL(EV_yields_gen4.at("HP"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen4.at("Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen4.at("Defense"), 1);
    BOOST_CHECK_EQUAL(EV_yields_gen4.at("Speed"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen4.at("Special Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen4.at("Special Defense"), 0);

    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_experience_yield(), 54);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_experience_at_level(50), 117360);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_level_at_experience(200000), 59);
    BOOST_CHECK_GT(pokemon_entry_gen4.get_levelup_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_tm_hm_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_egg_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_forms().size(), 1);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_evolutions().size(), 1);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_species_id(), 401);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_pokemon_id(), 401);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_form_id(), 401);
    BOOST_CHECK_EQUAL(pokemon_entry_gen4.get_pokemon_index(), 401);

    BOOST_CHECK(fs::exists(pokemon_entry_gen4.get_icon_filepath(false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen4.get_icon_filepath(true)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen4.get_sprite_filepath(false,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen4.get_sprite_filepath(true,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen4.get_sprite_filepath(true,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen4.get_sprite_filepath(true,true)));

    /*
     * Generation V entry
     */
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_name(), "Stunfisk");
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_game(), "Black 2");
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_species(), "Trap");
    BOOST_CHECK_GT(pokemon_entry_gen5.get_pokedex_entry().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_form(), "Standard");
    BOOST_CHECK_CLOSE(pokemon_entry_gen5.get_height(), 0.7f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen5.get_weight(), 11.0f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen5.get_chance_male(), 0.5f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen5.get_chance_female(), 0.5f, 0.0001f);
    BOOST_CHECK(not pokemon_entry_gen5.has_gender_differences());
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_base_friendship(), 70);

    std::pair<std::string, std::string> types_gen5 = pokemon_entry_gen5.get_types();
    BOOST_CHECK_EQUAL(types_gen5.first, "Ground");
    BOOST_CHECK_EQUAL(types_gen5.second, "Electric");

    std::pair<std::string, std::string> abilities_gen5 = pokemon_entry_gen5.get_abilities();
    BOOST_CHECK_EQUAL(abilities_gen5.first, "Static");
    BOOST_CHECK_EQUAL(abilities_gen5.second, "Limber");

    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_hidden_ability(), "Sand Veil");

    std::pair<std::string, std::string> egg_groups_gen5 = pokemon_entry_gen5.get_egg_groups();
    BOOST_CHECK_EQUAL(egg_groups_gen5.first, "Water 1");
    BOOST_CHECK_EQUAL(egg_groups_gen5.second, "Amorphous");

    std::map<std::string, int> base_stats_gen5 = pokemon_entry_gen5.get_base_stats();
    BOOST_CHECK_EQUAL(base_stats_gen5.at("HP"), 109);
    BOOST_CHECK_EQUAL(base_stats_gen5.at("Attack"), 66);
    BOOST_CHECK_EQUAL(base_stats_gen5.at("Defense"), 84);
    BOOST_CHECK_EQUAL(base_stats_gen5.at("Speed"), 32);
    BOOST_CHECK_EQUAL(base_stats_gen5.at("Special Attack"), 81);
    BOOST_CHECK_EQUAL(base_stats_gen5.at("Special Defense"), 99);

    std::map<std::string, int> EV_yields_gen5 = pokemon_entry_gen5.get_EV_yields();
    BOOST_CHECK_EQUAL(EV_yields_gen5.at("HP"), 2);
    BOOST_CHECK_EQUAL(EV_yields_gen5.at("Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen5.at("Defense"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen5.at("Speed"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen5.at("Special Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen5.at("Special Defense"), 0);

    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_experience_yield(), 165);
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_experience_at_level(50), 125000);
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_level_at_experience(200000), 58);
    BOOST_CHECK_GT(pokemon_entry_gen5.get_levelup_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gen5.get_tm_hm_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gen5.get_egg_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gen5.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_forms().size(), 1);
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_evolutions().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_species_id(), 618);
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_pokemon_id(), 618);
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_form_id(), 618);
    BOOST_CHECK_EQUAL(pokemon_entry_gen5.get_pokemon_index(), 618);

    BOOST_CHECK(fs::exists(pokemon_entry_gen5.get_icon_filepath(false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen5.get_icon_filepath(true)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen5.get_sprite_filepath(false,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen5.get_sprite_filepath(true,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen5.get_sprite_filepath(true,false)));
    BOOST_CHECK(fs::exists(pokemon_entry_gen5.get_sprite_filepath(true,true)));

    /*
     * Generation VI entry
     */
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_name(), "Sylveon");
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_game(), "Alpha Sapphire");
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_species(), "Intertwining");
    BOOST_CHECK_GT(pokemon_entry_gen6.get_pokedex_entry().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_form(), "Standard");
    BOOST_CHECK_CLOSE(pokemon_entry_gen6.get_height(), 1.0f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen6.get_weight(), 23.5f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen6.get_chance_male(), 0.875f, 0.0001f);
    BOOST_CHECK_CLOSE(pokemon_entry_gen6.get_chance_female(), 0.125f, 0.0001f);
    BOOST_CHECK(not pokemon_entry_gen6.has_gender_differences());
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_base_friendship(), 70);

    std::pair<std::string, std::string> types_gen6 = pokemon_entry_gen6.get_types();
    BOOST_CHECK_EQUAL(types_gen6.first, "Fairy");
    BOOST_CHECK_EQUAL(types_gen6.second, "None");

    std::pair<std::string, std::string> abilities_gen6 = pokemon_entry_gen6.get_abilities();
    BOOST_CHECK_EQUAL(abilities_gen6.first, "Cute Charm");
    BOOST_CHECK_EQUAL(abilities_gen6.second, "None");

    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_hidden_ability(), "Pixilate");

    std::pair<std::string, std::string> egg_groups_gen6 = pokemon_entry_gen6.get_egg_groups();
    BOOST_CHECK_EQUAL(egg_groups_gen6.first, "Field");
    BOOST_CHECK_EQUAL(egg_groups_gen6.second, "None");

    std::map<std::string, int> base_stats_gen6 = pokemon_entry_gen6.get_base_stats();
    BOOST_CHECK_EQUAL(base_stats_gen6.at("HP"), 95);
    BOOST_CHECK_EQUAL(base_stats_gen6.at("Attack"), 65);
    BOOST_CHECK_EQUAL(base_stats_gen6.at("Defense"), 65);
    BOOST_CHECK_EQUAL(base_stats_gen6.at("Speed"), 60);
    BOOST_CHECK_EQUAL(base_stats_gen6.at("Special Attack"), 110);
    BOOST_CHECK_EQUAL(base_stats_gen6.at("Special Defense"), 130);

    std::map<std::string, int> EV_yields_gen6 = pokemon_entry_gen6.get_EV_yields();
    BOOST_CHECK_EQUAL(EV_yields_gen6.at("HP"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen6.at("Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen6.at("Defense"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen6.at("Speed"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen6.at("Special Attack"), 0);
    BOOST_CHECK_EQUAL(EV_yields_gen6.at("Special Defense"), 2);

    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_experience_yield(), 184);
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_experience_at_level(50), 125000);
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_level_at_experience(200000), 58);
    BOOST_CHECK_GT(pokemon_entry_gen6.get_levelup_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gen6.get_tm_hm_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gen6.get_egg_moves().size(), 0);
    BOOST_CHECK_GT(pokemon_entry_gen6.get_tutor_moves().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_forms().size(), 1);
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_evolutions().size(), 0);
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_species_id(), 700);
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_pokemon_id(), 700);
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_form_id(), 700);
    BOOST_CHECK_EQUAL(pokemon_entry_gen6.get_pokemon_index(), 700);

    // TODO: sprites+icons
}

// Pokémon entries created by index
static pkmn::database::pokemon_entry byindex_gen1(74,3);
static pkmn::database::pokemon_entry byindex_gen2(160,4);
static pkmn::database::pokemon_entry byindex_gba(402,9);
static pkmn::database::pokemon_entry byindex_gcn(306,20);
static pkmn::database::pokemon_entry byindex_gen4(401,13);
static pkmn::database::pokemon_entry byindex_gen5(618,21);
static pkmn::database::pokemon_entry byindex_gen6(700,26);

// Pokémon entries created by name
static pkmn::database::pokemon_entry byname_gen1(
    "Articuno", "Yellow", ""
);
static pkmn::database::pokemon_entry byname_gen2(
    "Feraligatr", "Gold", ""
);
static pkmn::database::pokemon_entry byname_gba(
    "Regice", "Emerald", ""
);
static pkmn::database::pokemon_entry byname_gcn(
    "Shroomish", "XD", ""
);
static pkmn::database::pokemon_entry byname_gen4(
    "Kricketot", "Pearl", ""
);
static pkmn::database::pokemon_entry byname_gen5(
    "Stunfisk", "Black 2", ""
);
static pkmn::database::pokemon_entry byname_gen6(
    "Sylveon", "Alpha Sapphire", ""
);

BOOST_AUTO_TEST_CASE(pokemon_entry_equality) {
    BOOST_CHECK(byindex_gen1 == byname_gen1);
    BOOST_CHECK(byindex_gen2 == byname_gen2);
    BOOST_CHECK(byindex_gba  == byname_gba);
    BOOST_CHECK(byindex_gcn  == byname_gcn);
    BOOST_CHECK(byindex_gen4 == byname_gen4);
    BOOST_CHECK(byindex_gen5 == byname_gen5);
    BOOST_CHECK(byindex_gen6 == byname_gen6);

    BOOST_CHECK(byindex_gen1 != byname_gen2);
    BOOST_CHECK(byindex_gen2 != byname_gba);
    BOOST_CHECK(byindex_gba  != byname_gcn);
    BOOST_CHECK(byindex_gcn  != byname_gen4);
    BOOST_CHECK(byindex_gen4 != byname_gen5);
    BOOST_CHECK(byindex_gen5 != byname_gen6);
}

BOOST_AUTO_TEST_CASE(pokemon_entry_by_index_test) {
    _pokemon_entry_test_common(
        byindex_gen1, byindex_gen2, byindex_gba,
        byindex_gcn, byindex_gen4, byindex_gen5,
        byindex_gen6
    );
}

BOOST_AUTO_TEST_CASE(pokemon_entry_by_name_test) {
    _pokemon_entry_test_common(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );
}
