/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/pokemon.hpp>

#include "private_exports.hpp"
#include "polymorphism/is_libpkmn_impl.hpp"

namespace pkmn { namespace polymorphism {

    pokemon::sptr pokemon_to_libpkmn_impl(
        const pokemon::sptr& input_pokemon
    )
    {
        pkmn::pokemon::sptr ret = pkmn::pokemon::make(
                                      input_pokemon->get_species(),
                                      input_pokemon->get_game(),
                                      input_pokemon->get_form(),
                                      input_pokemon->get_level()
                                  );
        int generation = pkmn::priv::game_enum_to_generation(ret->get_game());

        ret->set_condition(input_pokemon->get_condition());
        ret->set_nickname(input_pokemon->get_nickname());
        ret->set_original_trainer_name(input_pokemon->get_original_trainer_name());
        ret->set_original_trainer_id(input_pokemon->get_original_trainer_id());
        ret->set_experience(input_pokemon->get_experience());
        ret->set_current_hp(input_pokemon->get_current_hp());

        for(const auto& EV_pair: input_pokemon->get_EVs())
        {
            ret->set_EV(EV_pair.first, EV_pair.second);
        }
        for(const auto& IV_pair: input_pokemon->get_IVs())
        {
            ret->set_IV(IV_pair.first, IV_pair.second);
        }

        const pkmn::move_slots_t& moves = input_pokemon->get_moves();
        for(int move_index = 0;
            move_index < static_cast<int>(moves.size());
            ++move_index)
        {
            ret->set_move(
                moves[move_index].move,
                move_index
            );
            ret->set_move_pp(
                move_index,
                moves[move_index].pp
            );
        }

        if(generation >= 2)
        {
            ret->set_is_egg(input_pokemon->is_egg());
            ret->set_gender(input_pokemon->get_gender());
            ret->set_shininess(input_pokemon->is_shiny());
            ret->set_held_item(input_pokemon->get_held_item());
            ret->set_pokerus_duration(input_pokemon->get_pokerus_duration());
            ret->set_original_trainer_gender(input_pokemon->get_original_trainer_gender());
            ret->set_current_trainer_friendship(input_pokemon->get_current_trainer_friendship());
            ret->set_level_met(input_pokemon->get_level_met());
        }
        if(generation >= 3)
        {
            ret->set_nature(input_pokemon->get_nature());
            ret->set_personality(input_pokemon->get_personality());
            ret->set_language(input_pokemon->get_language());
            ret->set_ability(input_pokemon->get_ability());
            ret->set_ball(input_pokemon->get_ball());
            ret->set_location_met(input_pokemon->get_location_met(false), false);
            ret->set_original_game(input_pokemon->get_original_game());

            for(const auto& marking_pair: input_pokemon->get_markings())
            {
                ret->set_marking(marking_pair.first, marking_pair.second);
            }
            for(const auto& ribbon_pair: input_pokemon->get_ribbons())
            {
                ret->set_ribbon(ribbon_pair.first, ribbon_pair.second);
            }
            for(const auto& contest_stat_pair: input_pokemon->get_contest_stats())
            {
                ret->set_contest_stat(contest_stat_pair.first, contest_stat_pair.second);
            }
        }
        if(generation >= 4)
        {
            ret->set_location_met(input_pokemon->get_location_met(true), true);
        }
        if(generation >= 5)
        {
        }
        if(generation >= 6)
        {
        }
        if(generation >= 7)
        {
        }

        return ret;
    }

}}
